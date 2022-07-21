/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#define _USE_MATH_DEFINES	// Définition des constantes mathématiques de math.h
#include <math.h>


// Des Headers situés dans le répertoire de l'application
#include "kinematics.h"
#include "utility.h"

// Des définitions locales


/**********************************************
 ********** Déclarations (globales) ***********
 **********************************************
*/

// Externes

// Types de base

// Pointeurs (sur tableaux, sur chaînes, etc.)

// Pointeurs de widgets de l'interface


/*************************************************
 ** Lf : longueur du bras						**
 ** Le : longueur de l'avant-bras (Lm)			**
 ** Rf : Rayon de la plateforme fixe supérieure	**
 ** Re : Rayon de la nacelle mobile (Rm)		**
 *************************************************
*/

// Calcule les positions des poignets par rapport aux repères cartésiens des bras (XiYiZi)
sWrist* wrist(sDeltaMCS MCS,double Rf,double Re)
{
	// ********** Déclarations (locales) ************
	static sWrist wristPos[3];
	int i;
	// *********** Fin de déclarations **************

	// Bras 0=0 degrés, 1=120 degrés, 2=240 degrés (pour un tripod)
	for (i=0;i<=2;i++)
	{
		wristPos[i].x=-Rf+Re+MCS.x*cos(degToRad(i*120))+MCS.y*sin(degToRad(i*120));
		wristPos[i].y=-MCS.x*sin(degToRad(i*120))+MCS.y*cos(degToRad(i*120));
		wristPos[i].z=MCS.z;
	}

	return wristPos; // wristPos est un pointeur sur tableau de type structure sWrist..
}


// Calcule les orientations latérale et longitudinale de l'avant-bras (à la jointure du coude)
sElbow* elbow(sDeltaMCS MCS,sWrist* pWristPos,double Lf,double Le)
{
	// ********** Déclarations (locales) ************
	static sElbow elbowAngles[3];
	int i;
	// *********** Fin de déclarations **************

	// Bras 0=0 degrés, 1=120 degrés, 2=240 degrés (pour un tripod)

	for (i=0;i<=2;i++)
	{
		elbowAngles[i].q3i=radToDeg(acos((-MCS.x*sin(degToRad(i*120))+MCS.y*cos(degToRad(i*120)))/Le));
		elbowAngles[i].q2i=radToDeg(acos((pow(pWristPos[i].x,2)+pow(pWristPos[i].y,2)+pow(pWristPos[i].z,2)-pow(Lf,2)-pow(Le,2))/(2*Lf*Le*sin(degToRad(elbowAngles[i].q3i)))));
	}

	return elbowAngles; // elbowAngles est un pointeur sur tableau de type structure sElbow..
}
 

// Cinématique directe d'un robot delta 3 (ACS --> MCS)
sDeltaMCS forward(double Re,double Rf,double Lf,double Le,sDeltaACS ACS,sKnee Knee)
{
	// ********** Déclarations (locales) ************
	double t,dtr,y1,z1,y2,x2,z2,y3,x3,z3,dnm,w1,w2,w3,a1,b1,a2,b2,aV,bV,cV,dV;
	double x,y,k;
	sDeltaMCS MCS;
	// *********** Fin de déclarations **************

	t = (Re-Rf)*tan30/2.0;
	dtr = M_PI/180.0;

	ACS.thetaA *= dtr;
	ACS.thetaB *= dtr;
	ACS.thetaC *= dtr;

	// Calculate knee A
	Knee.x=0;
	Knee.y=Lf*cos(ACS.thetaA)-Rf;
	Knee.z=Lf*sin(ACS.thetaA);

	// Calculate knee B
	k=-Lf*cos(ACS.thetaB)+Rf;
	//Knee.x=;
	//Knee.y=;
	//Knee.z=;

	y1 = -(t + Lf*cos(ACS.thetaA));
	z1 = -Lf*sin(ACS.thetaA);

	y2 = (t + Lf*cos(ACS.thetaB))*sin30;
	x2 = y2*tan60;
	z2 = -Lf*sin(ACS.thetaB);

	y3 = (t + Lf*cos(ACS.thetaC))*sin30;
	x3 = -y3*tan60;
	z3 = -Lf*sin(ACS.thetaC);

	dnm = (y2-y1)*x3-(y3-y1)*x2;

	w1 = y1*y1 + z1*z1;
	w2 = x2*x2 + y2*y2 + z2*z2;
	w3 = x3*x3 + y3*y3 + z3*z3;

	// x = (a1*z + b1)/dnm
	a1 = (z2-z1)*(y3-y1)-(z3-z1)*(y2-y1);
	b1 = -((w2-w1)*(y3-y1)-(w3-w1)*(y2-y1))/2.0;

	// y = (a2*z + b2)/dnm;
	a2 = -(z2-z1)*x3+(z3-z1)*x2;
	b2 = ((w2-w1)*x3 - (w3-w1)*x2)/2.0;

	// a*z^2 + b*z + c = 0
	aV = a1*a1 + a2*a2 + dnm*dnm;
	bV = 2.0*(a1*b1 + a2*(b2-y1*dnm) - z1*dnm*dnm);
	cV = (b2-y1*dnm)*(b2-y1*dnm) + b1*b1 + dnm*dnm*(z1*z1 - Le*Le);

	// discriminant
	dV = bV*bV - 4.0*aV*cV;
	
	// Carré négatif : imaginaire --> pas de solution
	if (dV < 0.0)
	{
		MCS.x=0.0;
		MCS.y=0.0;
		MCS.z=0.0;
	}
	else
	{
		MCS.z = 1*(-0.5*(bV+sqrt(dV))/aV);
		x = (a1*MCS.z + b1)/dnm;
		y = (a2*MCS.z + b2)/dnm;
		// Rotation de -90° autour de z pour l'alignement du bras 1 avec l'axe des x
		MCS.x=x*cos270+y*sin270;
		MCS.y=-x*sin270+y*cos270;

	}

	return MCS;
}

