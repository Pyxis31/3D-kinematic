// Evite que matrix.h soit inclus plusieurs fois (à chaque #include "kinematics.h")
#ifndef KINEMATICS_H
#define KINEMATICS_H


/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile


/**********************************************
 **************** Définitions *****************
 **********************************************
*/
// Constantes de préprocesseur, macros, etc.


// Modèles de structures
typedef struct sDeltaMCS sDeltaMCS;
struct sDeltaMCS
{
	double x;
	double y;
	double z;
};

typedef struct sDeltaACS sDeltaACS;
struct sDeltaACS
{
	double thetaA;
	double thetaB;
	double thetaC;
};

typedef struct sForward sForward;
struct sForward
{
	double Re;
	double Rf;
	double Lf;
	double Le;
	sDeltaACS deltaACS;
	sDeltaMCS deltaMCS;
};

typedef struct sElbow sElbow;
struct sElbow
{
	double q2i;
	double q3i;
};

typedef struct sWrist sWrist;
struct sWrist
{
	double x;
	double y;
	double z;
};

// Modèles d'unions

// Modèles d'énumérations


/**********************************************
 ********* Prototypes de fonctions ************
 **********************************************
*/
sWrist* wrist(sDeltaMCS MCS,double Rf,double Re);
sElbow* elbow(sDeltaMCS MCS,sWrist* pWristPos,double Lf,double Le);
sDeltaMCS forward(double Re,double Rf,double Lf,double Le,sDeltaACS ACS);

#endif
