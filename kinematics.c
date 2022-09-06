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

/*
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
*/

// *********************
// *** TEST newdelta ***
// *********************

// Calcule les positions cartésiennes de la plateforme fixe supérieure (base)
sBase* base(double Rf)
{
	// ********** Déclarations (locales) ************
	static sBase basePos[3];
	// *********** Fin de déclarations **************

	// base A
	basePos[0].x=-Rf/tan30;
	basePos[0].y=0;
	basePos[0].z=-Rf;

	// base B
	basePos[1].x=Rf/tan30;
	basePos[1].y=0;
	basePos[1].z=-Rf;

	// base C
	basePos[2].x=0;
	basePos[2].y=0;
	basePos[2].z=Rf/sin30;

	return basePos; // basePos est un pointeur sur tableau de type structure sBase..
}

// Calcule les positions cartésiennes des épaules
sShoulder* shoulder(double Rf)
{
	// ********** Déclarations (locales) ************
	static sShoulder shoulderPos[3];
	vec3 v;
	//mat4 v1={0.0,0.0,-Rf,1};
	// *********** Fin de déclarations **************

	// shoulder A
	shoulderPos[0].x=0;
	shoulderPos[0].y=0;
	shoulderPos[0].z=-Rf;

	// shoulder B
	glm_vec3_copy((vec3){shoulderPos[0].x,shoulderPos[0].y,shoulderPos[0].z},v);
	glm_vec3_rotate(v,degToRad(120.0),(vec3){0.0,1.0,0.0});
	shoulderPos[1].x=v[0];
	shoulderPos[1].y=v[1];
	shoulderPos[1].z=v[2];

	// shoulder C
	glm_vec3_copy((vec3){shoulderPos[0].x,shoulderPos[0].y,shoulderPos[0].z},v);
	glm_vec3_rotate(v,degToRad(240.0),(vec3){0.0,1.0,0.0});
	shoulderPos[2].x=v[0];
	shoulderPos[2].y=v[1];
	shoulderPos[2].z=v[2];

	return shoulderPos; // shoulderPos est un pointeur sur tableau de type structure sShoulder..
}

// Calcule les positions cartésiennes des coudes
sElbow* elbow(sDeltaACS ACS,double Re,double Rf,double Lf)
{
	// ********** Déclarations (locales) ************
	static sElbow elbowPos[3];
	double k;
	// *********** Fin de déclarations **************

	// elbow A
	elbowPos[0].x=0;
	elbowPos[0].y=Lf*sin(degToRad(ACS.thetaA));
	elbowPos[0].z=Lf*cos(degToRad(ACS.thetaA))-Rf;

	// elbow B
	k=-Lf*cos(degToRad(ACS.thetaB))+Rf;
	elbowPos[1].x=-k*cos30;
	elbowPos[1].y=Lf*sin(degToRad(ACS.thetaB));
	elbowPos[1].z=k*sin30;

	// elbow C
	k=-Lf*cos(degToRad(ACS.thetaC))+Rf;
	elbowPos[2].x=k*cos30;
	elbowPos[2].y=Lf*sin(degToRad(ACS.thetaC));
	elbowPos[2].z=k*sin30;

	return elbowPos; // elbowPos est un pointeur sur tableau de type structure sElbow..
}

// Calcule les positions cartésiennes des poignets
sWrist* wrist(sDeltaMCS MCS,double Re)
{
	// ********** Déclarations (locales) ************
	static sWrist wristPos[3];
	// *********** Fin de déclarations **************

	// wrist A
	wristPos[0].x=MCS.x;
	wristPos[0].y=MCS.y-Re;
	wristPos[0].z=MCS.z;

	// wrist B
	wristPos[1].x=MCS.x+sin120*Re;
	wristPos[1].y=MCS.y-cos120*Re;
	wristPos[1].z=MCS.z;

	// wrist C
	wristPos[2].x=MCS.x-sin120*Re;
	wristPos[2].y=MCS.y-cos120*Re;
	wristPos[2].z=MCS.z;


	return wristPos; // wristPos est un pointeur sur tableau de type structure sWrist..
}

// Cinématique directe d'un robot delta 3 (ACS --> MCS)
sDeltaMCS forward(double Re,double Rf,double Lf,double Le,sDeltaACS ACS)
{
	// ********** Déclarations (locales) ************
	double t,dtr,y1,z1,y2,x2,z2,y3,x3,z3,dnm,w1,w2,w3,a1,b1,a2,b2,aV,bV,cV,dV;
	double x,y;
	sDeltaMCS MCS;
	// *********** Fin de déclarations **************

	t = (Re-Rf)*tan30/2.0;
	dtr = M_PI/180.0;

	ACS.thetaA *= dtr;
	ACS.thetaB *= dtr;
	ACS.thetaC *= dtr;

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

