// Evite que draw.h soit inclus plusieurs fois (à chaque #include "draw.h")
#ifndef DRAW_H
#define DRAW_H


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

typedef struct sDrawingArg sDrawingArg;
struct sDrawingArg
{
	int width;
	int height;
	GtkWidget *pImu_check_button;
	float* pIMUcoordinates;
	float Rotate_sliderValue_Joint1;
	float Rotate_sliderValue_Joint2;
	float Rotate_sliderValue_Joint3;
	float Rotate_sliderValue_Joint4;
	float Rotate_sliderValue_Joint5;
	float Rotate_sliderValue_Joint6;
	float FOVy_sliderValue;
	float deltatime_sec;
};

// Structure permettant d'indiquer à glVertexAttribPointer la taille totale et les positions de chaque attribut
typedef struct sVertex3Dcolor sVertex3Dcolor;
struct sVertex3Dcolor
{
	vec3 position;
	vec3 color;
	vec2 texture;
};

// Modèles d'unions


// Modèles d'énumérations


/**********************************************
 ********* Prototypes de fonctions ************
 **********************************************
*/
void init(GtkWidget* pMessages_display);
void drawing(sDrawingArg drawingArg,GtkWidget* pMessages_display);

#endif
