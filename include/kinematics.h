// Evite que matrix.h soit inclus plusieurs fois (à chaque #include "kinematics.h")
#ifndef KINEMATICS_H
#define KINEMATICS_H


/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#include <cglm/cglm.h>		// OpenGL Mathematics
#include <cglm/struct.h>	// OpenGL Mathematics


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

typedef struct sBase sBase;
struct sBase
{
	double x;
	double y;
	double z;
};

typedef struct sShoulder sShoulder;
struct sShoulder
{
	double x;
	double y;
	double z;
};

typedef struct sElbow sElbow;
struct sElbow
{
	double x;
	double y;
	double z;
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
sBase* base(double Rf);
sShoulder* shoulder(double Rf);
sElbow* elbow(sDeltaACS ACS,double Re,double Rf,double Lf);
sWrist* wrist(sDeltaMCS MCS,double Re);
sDeltaMCS forward(double Re,double Rf,double Lf,double Le,sDeltaACS ACS);

#endif
