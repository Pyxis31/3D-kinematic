/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#include <gtk/gtk.h>		// Gimp Tool Kit
#include <pthread.h>
#include <stdio.h>			// sprintf, FILE, fopen, fclose
#include <stdlib.h>			// atoi, malloc, calloc, sizeof, free
#include <unistd.h>			// sleep, getcwd
#include <epoxy/gl.h>		// OpenGL
#include <string.h>			// strlen, strcmp, memset
#define _USE_MATH_DEFINES	// Définition des constantes mathématiques de math.h
#include <math.h>

// Des Headers situés dans le répertoire de l'application
#include "functions.h"
#include "utility.h"
#include "kinematics.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Des définitions locales


/**********************************************
 ********** Déclarations (globales) ***********
 **********************************************
*/

// Externes


// Types de base


// Pointeurs (sur tableaux, sur chaînes, etc.)


// Fonction de chargement d'un fichier de shader et de stockage du code source dans une chaîne de caractères
sReturnFnc_LdSrc LoadSource(const char* pFilename)
{
	// ********** Déclarations (locales) ************
    FILE* pFile=NULL; 		   			// Init du fichier
    long size;     		     			// Taille du fichier
	sReturnFnc_LdSrc responseLdSrc;		// Structure retournée par la fonction
    responseLdSrc.pSource=NULL;			// Init de la chaîne du code source
    responseLdSrc.pcharMessFnc=NULL;
	// *********** Fin de déclarations **************
	
    // Ouverture du fichier
    pFile=fopen(pFilename,"r");
    // Vérification de l'ouverture
    if(pFile==NULL)
    {
		char string[]="Impossible d'ouvrir le fichier :\n";
		responseLdSrc.pcharMessFnc=malloc((strlen(string)+strlen(pFilename)+1)*sizeof(char));
		sprintf(responseLdSrc.pcharMessFnc,"%s%s\n",string,pFilename);
		return responseLdSrc;
    }
    
    // Récupération de la longueur du fichier
    fseek(pFile,0,SEEK_END);
    size=ftell(pFile);
    
    // Positionnement en début du fichier
    rewind(pFile);
    
    // Allocation mémoire pour y placer le code source
    responseLdSrc.pSource=(char*)malloc(size+1); // +1 pour le caractère de fin de chaîne '\0'
    // Vérification de l'allocation
    if(responseLdSrc.pSource==NULL)
    {
        fclose(pFile);        
        char string[]="Erreur d'allocation mémoire\n";
		responseLdSrc.pcharMessFnc=malloc((strlen(string)+1)*sizeof(char));
		sprintf(responseLdSrc.pcharMessFnc,string);
		return responseLdSrc;
    }

    // Lecture du fichier
    fread(responseLdSrc.pSource,size,1,pFile);
    
    // Ajout du caractère '\0' en fin de fichier
    responseLdSrc.pSource[size]='\0';
    
    fclose(pFile);

	char string[]="Fichier lu avec succès\n";
	responseLdSrc.pcharMessFnc=malloc((strlen(string)+1)*sizeof(char));
	sprintf(responseLdSrc.pcharMessFnc,string);
	return responseLdSrc;
}


// Fonction complète de création d'un shader, de chargement du code source et de compilation
sReturnFnc_LdSha LoadShader(GLenum type,const char* pFilename)
{
	// ********** Déclarations (locales) ************
	sReturnFnc_LdSrc responseLdSrc;	// Structure retournée par la fonction LoadSource
	responseLdSrc.pSource=NULL;		// Init de la chaîne du code source
	GLsizei logSize=0;
	GLint compileStatus=GL_TRUE;
	char* pcharLog=NULL;
	sReturnFnc_LdSha responseLdSha;	// Structure retournée par la fonction
	responseLdSha.shader=0;			// Init de l'identifiant du shader
	responseLdSha.pcharMessFnc=NULL;
	GLint length;
	// *********** Fin de déclarations **************
	
	// Création d'un shader de sommet
	responseLdSha.shader=glCreateShader(type);
	if(responseLdSha.shader==0)
	{
		char string[]="Impossible de créer le shader\n";
		responseLdSha.pcharMessFnc=malloc((strlen(string)+1)*sizeof(char));
		sprintf(responseLdSha.pcharMessFnc,string);
		responseLdSha.shader=0;
		return responseLdSha;
	}
	
	// Chargement du code source
	responseLdSrc=LoadSource(pFilename);
	if 	(responseLdSrc.pSource==NULL)
	{
		glDeleteShader(responseLdSha.shader);
		responseLdSha.shader=0;
		strcpy(responseLdSha.pcharMessFnc,responseLdSrc.pcharMessFnc);
        return responseLdSha;
	}
	
	// Assignation du code source
	length=strlen(responseLdSrc.pSource);
	glShaderSource(responseLdSha.shader,1,(const GLchar**)&responseLdSrc.pSource,&length);
	
	// Compilation du shader
	glCompileShader(responseLdSha.shader);
	
	// Libération de la mémoire du code source
	free(responseLdSrc.pSource);
	responseLdSrc.pSource=NULL;
	
	// Vérification du succès de la compilation
	glGetShaderiv(responseLdSha.shader,GL_COMPILE_STATUS,&compileStatus);
	if(compileStatus!=GL_TRUE)
	{
		// Erreur à la compilation récupération du log d'erreurs
		
		// Récupération de la taille du message d'erreur
		glGetShaderiv(responseLdSha.shader,GL_INFO_LOG_LENGTH,&logSize);
		
		// Allocation d'un espace mémoire dans lequel OpenGL écrira le message
		pcharLog=(char*)malloc(logSize+1);
		if(pcharLog==NULL)
		{
			char string[]="Impossible d'allouer de la mémoire\n";
			responseLdSha.pcharMessFnc=malloc((strlen(string)+1)*sizeof(char));
			sprintf(responseLdSha.pcharMessFnc,string);
			responseLdSha.shader=0;
			return responseLdSha;
		}
		
		// Initialisation du contenu
		memset(pcharLog,'\0',logSize+1);

		char string[]="Impossible de compiler le shader";
		responseLdSha.pcharMessFnc=malloc((strlen(string)+strlen(pFilename)+logSize+1)*sizeof(char));
		sprintf(responseLdSha.pcharMessFnc,"%s %s :\n%s\n",string,pFilename,pcharLog);
		
		// Libération de la mémoire et du shader
		free(pcharLog);
		glDeleteShader(responseLdSha.shader);

		responseLdSha.shader=0;
		return responseLdSha;		
	}

	// Message en cas de succès
	if (type==GL_VERTEX_SHADER)
	{
		char string[]="Vertex shader succesfully loaded and built";
		responseLdSha.pcharMessFnc=malloc((strlen(string)+1)*sizeof(char));
		sprintf(responseLdSha.pcharMessFnc,"%s\n",string);
	}
	else
	{
		char string[]="Fragment shader succesfully loaded and built";
		responseLdSha.pcharMessFnc=malloc((strlen(string)+1)*sizeof(char));
		sprintf(responseLdSha.pcharMessFnc,"%s\n",string);
	}
	
	return responseLdSha;
}

// Fonction d'édition de lien des shaders au program
char* LinkProgSha(GLuint program)
{
	// ********** Déclarations (locales) ************
	GLsizei logSize=0;
	GLint linkStatus=GL_TRUE;
	char* pcharLog=NULL;
	char* pcharMessFnc=NULL;
	// *********** Fin de déclarations **************

	//// Pas de message
	//char string[]="";
	//pcharMessFnc=malloc((strlen(string)+1)*sizeof(char));
	//sprintf(pcharMessFnc,string);
	
	// Edition de lien
	glLinkProgram(program);

	// Vérification du succès de l'édition de lien
	glGetProgramiv(program,GL_LINK_STATUS,&linkStatus);
	if(linkStatus!=GL_TRUE)
	{
		// Erreur d'édition de lien et récupération du log d'erreurs
		
		// Récupération de la taille du message d'erreur
		glGetProgramiv(program,GL_INFO_LOG_LENGTH,&logSize);
		
		// Allocation d'un espace mémoire dans lequel OpenGL écrira le message
		pcharLog=(char*)malloc(logSize+1);
		if(pcharLog==NULL)
		{
			char string[]="Impossible d'allouer de la mémoire\n";
			pcharMessFnc=malloc((strlen(string)+1)*sizeof(char));
			sprintf(pcharMessFnc,string);
			return pcharMessFnc;
		}
		
		// Initialisation du contenu
		memset(pcharLog,'\0',logSize+1);
		
		glGetProgramInfoLog(program,logSize,&logSize,pcharLog);
		
		char string[]="Impossible de réaliser l'édition de lien :\n";
		pcharMessFnc=malloc((strlen(string)+logSize+1)*sizeof(char));
		sprintf(pcharMessFnc,"%s%s\n",string,pcharLog);
		return pcharMessFnc;		
	}

	// Message en cas de succès
	char string[]="Shaders's link-edit succesfully done";
	pcharMessFnc=malloc((strlen(string)+1)*sizeof(char));
	sprintf(pcharMessFnc,"%s\n",string);

	return pcharMessFnc;
}


// Affiche un message dans la fenêtre de texte
void fMessage(char* pcharMessage, GtkWidget* pMessages_display)
{	
	// ********** Déclarations (locales) ************
	GtkTextBuffer* pTextbuffer;
	// *********** Fin de déclarations **************

	// Récupère le buffer de la zone de texte
	pTextbuffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(pMessages_display));

	// Affiche le message
	gtk_text_buffer_insert_at_cursor(pTextbuffer,pcharMessage,-1);
}


// Calcul le taux de rafraîchissement GPU
float frameRate(GtkWidget* pGpu_period,GtkWidget* pGpu_frequency)
{
	// ********** Déclarations (locales) ************
	gint64 deltaTime=0;
	static gint64 lastTime;
	char pFormatted_Gpu_period[10+1];
	char pFormatted_Gpu_frequency[10+1];
	static int frameCounter;
	// *********** Fin de déclarations **************
	
	// Calcule le taux de rafraîchissement du GPU
	deltaTime=g_get_monotonic_time()-lastTime;
	lastTime=g_get_monotonic_time();
	// Affichage (filtré)
	frameCounter+=1;
	if (frameCounter>30)
	{
		frameCounter=0;
		sprintf(pFormatted_Gpu_period,"%i msec",(int)(deltaTime*1e-3));
		gtk_label_set_text(GTK_LABEL(pGpu_period),pFormatted_Gpu_period);
		sprintf(pFormatted_Gpu_frequency,"%i Hz",(int)(1/(deltaTime*1e-6)));
		gtk_label_set_text(GTK_LABEL(pGpu_frequency),pFormatted_Gpu_frequency);
	}
	return (float)(deltaTime*1e-6);
}


// Récupère et affiche la version du GPU et celle de l'OpenGL
void version(sVersionArg versionArg)
{
	// ********** Déclarations (locales) ************
	char pFormatted_Gpu_opengl_ver [60+1];
	char pFormatted_Gpu_renderer_type [60+1];
	char pFormatted_Gpu_shading_lang_ver [60+1];
	char pFormatted_Gpu_manufacturer [60+1];
	// *********** Fin de déclarations **************

	// Version de l'OpenGL et du GPU
	const GLubyte* version=glGetString(GL_VERSION);
	const GLubyte* renderer=glGetString(GL_RENDERER);
	const GLubyte* shadingVersion=glGetString(GL_SHADING_LANGUAGE_VERSION);
	const GLubyte* vendor=glGetString(GL_VENDOR);

	// Affichage
	sprintf(pFormatted_Gpu_opengl_ver,"%s",version);
	gtk_label_set_text(GTK_LABEL(versionArg.pGpu_opengl_ver),pFormatted_Gpu_opengl_ver);
	sprintf(pFormatted_Gpu_renderer_type,"%s",renderer);
	gtk_label_set_text(GTK_LABEL(versionArg.pGpu_renderer_type),pFormatted_Gpu_renderer_type);
	sprintf(pFormatted_Gpu_shading_lang_ver,"%s",shadingVersion);
	gtk_label_set_text(GTK_LABEL(versionArg.pGpu_shading_lang_ver),pFormatted_Gpu_shading_lang_ver);
	sprintf(pFormatted_Gpu_manufacturer,"%s",vendor);
	gtk_label_set_text(GTK_LABEL(versionArg.pGpu_manufacturer),pFormatted_Gpu_manufacturer);
}


// Calcule la position du point de vue
sViewControlRet viewControl(sViewControlArg viewControlArg)
{
	// ********** Déclarations (locales) ************
	static sViewControlRet responseViewControl;
	static float memXpos,memYpos,pitch,yaw;
	float dX,dY,sensitivity=0.2;
	vec3 rightVector,forwardVector,dXVector,dYVector;
	static bool initView;
	// *********** Fin de déclarations **************
	

	// Distances X et Y parcourues par le curseur entre chaque frame
	if (viewControlArg.mouseLbutton)
	{
		dX=viewControlArg.xPos-memXpos;
		memXpos=viewControlArg.xPos;
		dY=viewControlArg.yPos-memYpos;
		memYpos=viewControlArg.yPos;
	}
	else
	{
		memXpos=viewControlArg.xPos;
		memYpos=viewControlArg.yPos;
		dX=0.0;
		dY=0.0;
	}

	// Sensibilité de la souris
	dX*=sensitivity;
	dY*=sensitivity;

	// Initialisation des positions et des angles
	if (!initView||viewControlArg.initView)
	{
		// Caméra en Z+
		responseViewControl.eye[0]=0;
		responseViewControl.eye[1]=0;
		responseViewControl.eye[2]=viewControlArg.mouseWheel;

		// Modèle au centre
		responseViewControl.center[0]=0;
		responseViewControl.center[1]=0;
		responseViewControl.center[2]=0;

		// Angles pour une orientation en Z+
		pitch=0.0;
		yaw=90.0;

		initView=true;
	}

	// Repère caméra
	// Up
	glm_vec3_copy((vec3){0,1,0},responseViewControl.up);

	// Forward
	glm_vec3_sub(responseViewControl.center,responseViewControl.eye,forwardVector);
	glm_vec3_normalize(forwardVector);

	// Right
	glm_vec3_cross(forwardVector,responseViewControl.up,rightVector);

	// Translation de la scène
	if (viewControlArg.CrossButton)
	{
		// Vecteur right * dX
		glm_vec3_scale(rightVector,-dX*viewControlArg.deltatime_sec,dXVector);

		// Vecteur up * dY
		glm_vec3_scale(responseViewControl.up,dY*viewControlArg.deltatime_sec,dYVector);

		// Translation en X de la caméra
		glm_vec3_add(responseViewControl.eye,dXVector,responseViewControl.eye);
		
		// Translation en X du modèle
		glm_vec3_add(responseViewControl.center,dXVector,responseViewControl.center);

		// Translation en Y de la caméra
		glm_vec3_add(responseViewControl.eye,dYVector,responseViewControl.eye);
		
		// Translation en Y du modèle
		glm_vec3_add(responseViewControl.center,dYVector,responseViewControl.center);
	}
	// Changement de point de vue
	else if (viewControlArg.ArrowsButton)
	{
		// Angles d'Euler pitch et yaw
		if (viewControlArg.IMUbutton)
		{
			//  Par l'IMU
			pitch=CLAMP(-viewControlArg.IMUcoordinates_z,-90,90);
			yaw=viewControlArg.IMUcoordinates_y;
		}
		else
		{
			// Par la souris
			pitch+=dY;
			yaw+=dX;
			pitch=CLAMP(pitch,-90,90);
			yaw=CLAMP(yaw,-180,180);
		}	
		// Conversion des coordonnées sphériques en coordonnées cartésiennes
		responseViewControl.eye[0]=responseViewControl.center[0]+viewControlArg.mouseWheel*(cos(degreesToRadians(yaw))*cos(degreesToRadians(pitch)));
		responseViewControl.eye[1]=responseViewControl.center[1]+viewControlArg.mouseWheel*(sin(degreesToRadians(pitch)));
		responseViewControl.eye[2]=responseViewControl.center[2]+viewControlArg.mouseWheel*(sin(degreesToRadians(yaw))*cos(degreesToRadians(pitch)));
	}						
	return responseViewControl;
}


// Reconstitue une matrice (mat4) depuis un pointeur sur float
void ptr_to_mat4(float* pMatrix,mat4 matrix)
{
	// ********** Déclarations (locales) ************
	int row,column,index=0;
	// *********** Fin de déclarations **************
	
	for(row=0;row<=3;row++)
	{
		for(column=0;column<=3;column++)
		{
			matrix[row][column]=*(pMatrix+index);
			index++;
		}
	}
}


// DEBUG : affiche le contenu d'une matrice (mat4)
void print_mat4(mat4 matrix)
{
	// ********** Déclarations (locales) ************
	int row,column;
	// *********** Fin de déclarations **************

	system("cls");
	printf("Matrix 4 x 4 :\n");
	
	for(row=0;row<=3;row++)
	{
		for(column=0;column<=3;column++)
		{
			printf("%f\t",matrix[row][column]);
			if(column==3)
			{
				printf("\n");
			}
		}
	}
}

// Push matrix
void push(mat4* pStack)
{
	glm_mat4_copy(pStack[3],pStack[4]);
	glm_mat4_copy(pStack[2],pStack[3]);
	glm_mat4_copy(pStack[1],pStack[2]);
	glm_mat4_copy(pStack[0],pStack[1]);
}

// Pop matrix
void pop(mat4* pStack)
{
	glm_mat4_copy(pStack[1],pStack[0]);
	glm_mat4_copy(pStack[2],pStack[1]);
	glm_mat4_copy(pStack[3],pStack[2]);
	glm_mat4_copy(pStack[4],pStack[3]);
	glm_mat4_identity(pStack[4]);
}

// Fonction exécutée dans un thread
void* computeThread(void* pArgs)
{
	// ********** Déclarations (locales) ************
	//sArgsComputeThread* pValues;
	// *********** Fin de déclarations **************

	//pValues=(sArgsComputeThread*)pArgs;

	// Boucle d'exécution
	while(1)
	{
		usleep(1e6); // Pause en microsecondes
		//printf("Thread alive !\n");
		//pValues->forward.deltaMCS=forward(pValues->forward.Re,pValues->forward.Rf,pValues->forward.Lf,pValues->forward.Le,pValues->forward.deltaACS);

	}
	return(NULL);
}

// Fonction de chargement d'un fichier image
sReturnFnc_LdTex LoadTexture(const char* pFilename)
{
	// ********** Déclarations (locales) ************
	sReturnFnc_LdTex responseLdTex;		// Structure retournée par la fonction
    responseLdTex.pcharMessFnc=NULL;	// Initialise le pointeur sur char de la structure (sur le tas : voir malloc)
	char pcharMessBuf[CHAR_MAX];		// Initialise un buffer de 255 octets (sur la pile : libéré automatiquement)
    unsigned char *pcharDataTexture;
	int width,height,nrChannels;
	// *********** Fin de déclarations **************

	// Charge l'image
	pcharDataTexture=stbi_load(pFilename,&width,&height,&nrChannels,0);
	if (pcharDataTexture)
	{
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,pcharDataTexture);
		glGenerateMipmap(GL_TEXTURE_2D);
		sprintf(pcharMessBuf,"Texture successfully loaded\n"); // Ou strcpy(pchar,"texte\0");
	}
	else
	{
		sprintf(pcharMessBuf,"Texture loading error\n");
	}
	
	// Libère la mémoire
	stbi_image_free(pcharDataTexture);

	// Dimensionne le pointeur sur char de la structure en fonction de la longueur du message
	responseLdTex.pcharMessFnc=(char*)malloc((strlen(pcharMessBuf)+1)*sizeof(char));
	assert(responseLdTex.pcharMessFnc!=NULL);
	
	// Copie la chaîne de caractères de la pile vers le tas
	strcpy(responseLdTex.pcharMessFnc,pcharMessBuf);
	
	return responseLdTex;
}

