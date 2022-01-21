// Evite que functions.h soit inclus plusieurs fois (à chaque #include "functions.h")
#ifndef FUNCTIONS_H
#define FUNCTIONS_H


/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#include <cglm/cglm.h>		// OpenGL Mathematics
#include <cglm/struct.h>	// OpenGL Mathematics

// Des Headers situés dans le répertoire de l'application
#include "kinematics.h"


/**********************************************
 **************** Définitions *****************
 **********************************************
*/
// Constantes de préprocesseur, macros, etc.


// Modèles de structures
typedef struct sReturnFnc_LdSrc sReturnFnc_LdSrc;
struct sReturnFnc_LdSrc
{
	char* pSource;				// Code source retourné par la fonction (NULL en cas d'erreur)
	char* pcharMessFnc;			// Message retourné par la fonction
};

typedef struct sReturnFnc_LdSha sReturnFnc_LdSha;
struct sReturnFnc_LdSha
{
	GLuint shader;				// Identifiant du shader retourné par la fonction (0 en cas d'erreur)
	char* pcharMessFnc;			// Message retourné par la fonction
};

typedef struct sReturnFnc_LdTex sReturnFnc_LdTex;
struct sReturnFnc_LdTex
{
	char* pcharMessFnc;			// Message retourné par la fonction
};

typedef struct sVersionArg sVersionArg;
struct sVersionArg
{
	GtkWidget* pGpu_opengl_ver;
	GtkWidget* pGpu_renderer_type;
	GtkWidget* pGpu_shading_lang_ver;
	GtkWidget* pGpu_manufacturer;
};

typedef struct sViewControlRet sViewControlRet;
struct sViewControlRet
{
	vec3 eye;
	vec3 center;
	vec3 up;
};

typedef struct sViewControlArg sViewControlArg;
struct sViewControlArg
{
	bool initView;
	bool mouseLbutton;
	float mouseWheel;
	float deltatime_sec;
	float xPos;
	float yPos;
	int glAreaHeight;
	int glAreaWidth;
	bool CrossButton;
	bool ArrowsButton;
	bool IMUbutton;
	float IMUcoordinates_x;
	float IMUcoordinates_y;
	float IMUcoordinates_z;
};

typedef struct sArgsComputeThread sArgsComputeThread;
struct sArgsComputeThread
{
	sForward forward;
};

// Modèles d'unions


// Modèles d'énumérations


/**********************************************
 ********* Prototypes de fonctions ************
 **********************************************
*/

sReturnFnc_LdSrc LoadSource(const char* pFilename);
sReturnFnc_LdSha LoadShader(GLenum type,const char* pFilename);
sReturnFnc_LdTex LoadTexture(const char* pFilename);
char* LinkProgSha(GLuint program);
void fMessage(char* pcharMessage,GtkWidget* pMessages_display);
float frameRate(GtkWidget* pGpu_period,GtkWidget* pGpu_frequency);
void version(sVersionArg versionArg);
void ending();
sViewControlRet viewControl(sViewControlArg viewControlArg);
void print_mat4(mat4 matrix);
void ptr_to_mat4(float* pMatrix,mat4 matrix);
void push(mat4* pStack);
void pop(mat4* pStack);
void* computeThread (void* pArgs);
#endif
