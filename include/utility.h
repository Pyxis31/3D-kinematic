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
#define sqrt3	1.7320508075688772
#define sin120	sqrt3/2.0   
#define cos120	-0.5        
#define tan60	sqrt3
#define sin30	0.5
#define tan30	1.0/sqrt3
#define sin270	-1.0   
#define cos270	0.0


// Modèles de structures


// Modèles d'unions


// Modèles d'énumérations


/**********************************************
 ********* Prototypes de fonctions ************
 **********************************************
*/


#endif
