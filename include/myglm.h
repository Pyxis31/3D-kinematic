// Evite que matrix.h soit inclus plusieurs fois (à chaque #include "matrix.h")
#ifndef MATRIX_H
#define MATRIX_H


/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile


/**********************************************
 **************** Définitions *****************
 **********************************************

// Constantes de préprocesseur, macros, etc.

// Modèles de structures

// Modèles d'unions

// Modèles d'énumérations


/**********************************************
 ********* Prototypes de fonctions ************
 **********************************************
*/
void mat_frustum(float* pMatrix,float angle_of_view,float aspect_ratio,float z_near,float z_far);
void mat_translate(float* pMatrix,float dx,float dy,float dz);
void mat_scale(float* pMatrix,float dx,float dy,float dz);
static void normalize(float* pX,float* pY,float* pZ);
void mat_rotate(float* pMatrix,float x, float y, float z, float angle);
void mat_multiply(float* pMatrix,float* pA,float* pB);
float* vec_getOrthoVector(float x,float y,float z);

#endif
