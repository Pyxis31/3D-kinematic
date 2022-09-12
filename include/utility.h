// Evite que utility.h soit inclus plusieurs fois (à chaque #include "utility.h")
#ifndef UTILTY_H
#define UTILTY_H


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

// Get number of elements in an array:
#define NELEM(array)	(sizeof(array) / sizeof(*(array)))

// Loop over an array of given size:
#define FOREACH_NELEM(array, nelem, iter) \
	for (__typeof__(*(array)) *iter = (array); \
		iter < (array) + (nelem); \
		iter++)

// Loop over an array of known size:
#define FOREACH(array, iter) \
	FOREACH_NELEM(array, NELEM(array), iter)

// Conversion degrés vers radians
#define degreesToRadians(angleDegrees) ((angleDegrees)*M_PI/180.0)
#define degToRad(angleDeg) ((angleDeg)*M_PI/180.0)
#define radToDeg(angleRad) ((angleRad)*180.0/M_PI)

// Constantes mathématiques
#define sqrt3	1.73205080756888
#define sin120	0.866025403784439 
#define cos120	-0.5        
#define tan60	1.73205080756888
#define cos30	0.866025403784439
#define sin30	0.5
#define tan30	0.577350269189626
#define sin270	-1.0   
#define cos270	0.0
#define sin240	-0.866025403784439 
#define cos240	0.5


// Modèles de structures


// Modèles d'unions


// Modèles d'énumérations


/**********************************************
 ********* Prototypes de fonctions ************
 **********************************************
*/


#endif
