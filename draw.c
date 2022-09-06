/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#include <gtk/gtk.h>		// Gimp Tool Kit
#include <epoxy/gl.h>		// OpenGL
#include <unistd.h>			// getcwd


// Des Headers situés dans le répertoire de l'application
#include "functions.h"
#include "draw.h"
#include "utility.h"
#include "kinematics.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"





/**********************************************
 ********** Déclarations (globales) ***********
 **********************************************
*/

// Externes
extern GtkWidget *pProjMatrix_switch,*pAspect_switch,*pF_trans_check_button,*pWireframe_switch;
extern int frame;
extern sArgsComputeThread computeThreadArgs;

// Types de base
GLuint program,vboID[3],vaoID[1],textureID;


// Structures
sViewControlArg viewControlArg;

// Pointeurs (sur tableaux, sur chaînes, etc.)

// Pointeurs de widgets de l'interface

// Variables de la fonction drawing (globales pour gagner du temps CPU à chaque frame)
GLint uMatrix;
vec3 dot;
mat4 projection,view,viewProj,identity,stack[5];
float aspect_ratio;
sViewControlRet responseViewControl;
int loop, lineNb;
double Lf,Le,Rf,Re;
sDeltaACS Delta3ACS;
sDeltaMCS Delta3MCS;
sBase* pBasePos;
sShoulder* pShoulderPos;
sElbow* pElbowPos;
//sElbow* pElbowAngles;
//sWrist* pWristPos;


// Initialisation
void init(GtkWidget* pMessages_display)
{
	// ********** Déclarations (locales) ************
	sReturnFnc_LdSha responseLdShaVS;
	sReturnFnc_LdSha responseLdShaFS;
	sReturnFnc_LdTex responseLdTexture;
	char pathToFile[UCHAR_MAX];
	int numVBOs, numVAOs, numTEXTs;
	// *********** Fin de déclarations **************	

    /****************************************** 
	 ******* Initialisation des shaders *******
	 ******************************************
	*/
	// Chargement et compilation du code source du Vertex Shader
	getcwd(pathToFile,UCHAR_MAX);
	strcat(pathToFile,"\\shaders\\glarea-vertex.glsl");
	responseLdShaVS=LoadShader(GL_VERTEX_SHADER,pathToFile);
	fMessage(responseLdShaVS.pcharMessFnc,pMessages_display);
	free(responseLdShaVS.pcharMessFnc);

	// Chargement et compilation du code source du Fragment Shader
	getcwd(pathToFile,UCHAR_MAX);
	strcat(pathToFile,"\\shaders\\glarea-fragment.glsl");
	responseLdShaFS=LoadShader(GL_FRAGMENT_SHADER,pathToFile);	
	fMessage(responseLdShaFS.pcharMessFnc,pMessages_display);
	free(responseLdShaFS.pcharMessFnc);

	// Création d'un "program"
	program=glCreateProgram();

	// Attache les Shaders au "program"
	glAttachShader(program,responseLdShaVS.shader);
	glAttachShader(program,responseLdShaFS.shader);

	// Edition de lien des shaders
	fMessage(LinkProgSha(program),pMessages_display);
	free(LinkProgSha(program));

	// Utilisation du "program"
	glUseProgram(program);

	// Détache les shaders du "program"
	glDetachShader(program,responseLdShaVS.shader);
	glDetachShader(program,responseLdShaFS.shader);

	// Supprime les shaders
	glDeleteShader(responseLdShaVS.shader);
	glDeleteShader(responseLdShaFS.shader);

    /******************************************
	 *                Modèles                 *
	 ******************************************
	*/
	// Cube
	sVertex3Dcolor cube[]=
	{
		{{-0.5,-0.5,-0.5},	{37.0/255.0,150.0/255.0,190.0/255.0},	{0,0}},	// Face arrière
		{{0.5,-0.5,-0.5},	{37.0/255.0,150.0/255.0,190.0/255.0},	{1,0}},
		{{0.5,0.5,-0.5},	{37.0/255.0,150.0/255.0,190.0/255.0},	{1,1}},
		{{0.5,0.5,-0.5},	{37.0/255.0,150.0/255.0,190.0/255.0},	{1,1}},
		{{-0.5,0.5,-0.5},	{37.0/255.0,150.0/255.0,190.0/255.0},	{0,1}},
		{{-0.5,-0.5,-0.5},	{37.0/255.0,150.0/255.0,190.0/255.0},	{0,0}},
		{{-0.5,-0.5,0.5},	{37.0/255.0,150.0/255.0,190.0/255.0},	{0,0}},	// Face avant
		{{0.5,-0.5,0.5},	{37.0/255.0,150.0/255.0,190.0/255.0},	{1,0}},
		{{0.5,0.5,0.5},		{37.0/255.0,150.0/255.0,190.0/255.0},	{1,1}},
		{{0.5,0.5,0.5},		{37.0/255.0,150.0/255.0,190.0/255.0},	{1,1}},
		{{-0.5,0.5,0.5},	{37.0/255.0,150.0/255.0,190.0/255.0},	{0,1}},
		{{-0.5,-0.5,0.5},	{37.0/255.0,150.0/255.0,190.0/255.0},	{0,0}},
		{{-0.5,0.5,0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{0,1}},	// Face gauche
		{{-0.5,0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,1}},
		{{-0.5,-0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,0}},
		{{-0.5,-0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,0}},
		{{-0.5,-0.5,0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{0,0}},
		{{-0.5,0.5,0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{0,1}},
		{{0.5,0.5,0.5},		{181.0/255.0,176.0/255.0,161.0/255.0},	{0,1}},	// Face droite
		{{0.5,0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,1}},
		{{0.5,-0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,0}},
		{{0.5,-0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,0}},
		{{0.5,-0.5,0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{0,0}},
		{{0.5,0.5,0.5},		{181.0/255.0,176.0/255.0,161.0/255.0},	{0,1}},
		{{-0.5,-0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,1}},	// Face inférieure
		{{0.5,-0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,0}},
		{{0.5,-0.5,0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{0,0}},
		{{0.5,-0.5,0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{0,0}},
		{{-0.5,-0.5,0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{0,1}},
		{{-0.5,-0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,1}},
		{{-0.5,0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,1}},	// Face supérieure
		{{0.5,0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,0}},
		{{0.5,0.5,0.5},		{181.0/255.0,176.0/255.0,161.0/255.0},	{0,0}},
		{{0.5,0.5,0.5},		{181.0/255.0,176.0/255.0,161.0/255.0},	{0,0}},
		{{-0.5,0.5,0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{0,1}},
		{{-0.5,0.5,-0.5},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,1}},
	};

	// Repère cartésien
	sVertex3Dcolor coordSys[]=
	{
		{{0.0,0.0,0.0},	{1,0,0},	{0,0}},	// Abscisses X
		{{0.5,0.0,0.0},	{1,0,0},	{0,0}},
		{{0.0,0.0,0.0},	{0,1,0},	{0,0}},	// Ordonnées Y
		{{0.0,0.5,0.0},	{0,1,0},	{0,0}},
		{{0.0,0.0,0.0},	{0,0,1},	{0,0}},	// Profondeur Z
		{{0.0,0.0,0.5},	{0,0,1},	{0,0}},
	};

	// Robot delta
	sVertex3Dcolor delta[]=
	{
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}}, // Plateforme fixe supérieure (base)
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}}, // Epaules
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}}, // Coudes
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}}, // Poignets
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
	};


    /***************************************************
	 * VAO : spécifie les formats des attributs au GPU *
	 ***************************************************
	*/
	// Réserve des blocs mémoire et définit des ID
	numVAOs=1;
	glGenVertexArrays(numVAOs,vaoID);
	// Définit le VAO 0 (un seul VAO pour tous les VBO)
	glBindVertexArray(vaoID[0]);
	

    /********************************************************
	 * VBO : charge les attributs des vertices dans le GPU  *
	 ********************************************************
	*/
	// Réserve des blocs mémoire et définit des ID
	numVBOs=3;
	glGenBuffers(numVBOs,vboID);
	
	// Définit le VBO 0 (cube)
	glBindBuffer(GL_ARRAY_BUFFER,vboID[0]);
	// Transfert les données dans le GPU
	glBufferData(GL_ARRAY_BUFFER,sizeof(cube),cube,GL_STATIC_DRAW);

	// Définit le VBO 1 (Repère cartésien)
	glBindBuffer(GL_ARRAY_BUFFER,vboID[1]);
	// Transfert les données dans le GPU
	glBufferData(GL_ARRAY_BUFFER,sizeof(coordSys),coordSys,GL_STATIC_DRAW);

	// Définit le VBO 2 (Robot delta)
	glBindBuffer(GL_ARRAY_BUFFER,vboID[2]);
	// Transfert les données dans le GPU et permet leur modification dans la boucle de rendu
	glBufferData(GL_ARRAY_BUFFER,sizeof(delta),delta,GL_STREAM_DRAW);


    /****************************************
	 * TEXTURE : charge un fichier image	*
	 * en tant que nouvelle texture OpenGL	*
	 ****************************************
	*/
	// Réserve des blocs mémoire et définit des ID
	numTEXTs=1;
	glGenTextures(numTEXTs,&textureID);

	// Définit une texture 2D
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,textureID);
	
	// Options de la texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);   
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Charge le fichier image et génère la texture
	responseLdTexture=LoadTexture("pixmap\\textureAlu.jpg");
	fMessage(responseLdTexture.pcharMessFnc,pMessages_display);
	// Libère le tas
	free(responseLdTexture.pcharMessFnc);
}


// Affichage
void drawing(sDrawingArg drawingArg)
{
	// ********** Déclarations (locales) ************
	// Globales pour gagner du temps CPU à chaque frame
	// *********** Fin de déclarations **************

	// Contrôle le point de vue
	viewControlArg.IMUbutton=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(drawingArg.pImu_check_button));
	viewControlArg.IMUcoordinates_x=drawingArg.pIMUcoordinates[2];
	viewControlArg.IMUcoordinates_y=360-drawingArg.pIMUcoordinates[0];
	viewControlArg.IMUcoordinates_z=drawingArg.pIMUcoordinates[1];
	responseViewControl=viewControl(viewControlArg);
	viewControlArg.initView=false;

	// Ratio (dimensions de la fenêtre ou carré)
	aspect_ratio=(!gtk_switch_get_active(GTK_SWITCH(pAspect_switch))) ? (float)(drawingArg.width)/(float)(drawingArg.height) : 1.0/1.0;

	// Rendu (primitives filaires ou peintes)
	(!gtk_switch_get_active(GTK_SWITCH(pWireframe_switch))) ? glPolygonMode(GL_FRONT_AND_BACK,GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	// Type de projection (perspective ou orthogonale)
	// glm_perspective() : Champ de vision,Ratio d'aspect,Profondeur de champ (plage d'affichage entre 0.1 et 100)
	(!gtk_switch_get_active(GTK_SWITCH(pProjMatrix_switch))) ? glm_perspective(drawingArg.FOVy_sliderValue,aspect_ratio,0.1,1000.0,projection) : glm_ortho_default(aspect_ratio,projection);
				
	// Matrice de vue
	glm_lookat(responseViewControl.eye,responseViewControl.center,responseViewControl.up,view); // Position de la caméra,position du point visé,rouli caméra (fixe, toujours tête en haut)

	// Matrice de vue x projection (VP)
	glm_mat4_mul(projection,view,viewProj);

	// Range la matrice de vue en tête de la pile
	glm_mat4_copy(viewProj,stack[0]);


	/**********************************************
	 *********** Modèles hiérarchiques ************
	 ** Les transformées doivent être appliquées **
	 *********** dans l'ordre inverse ! ***********
	 **********************************************
	*/

	// VBO et ses attributs utilisés pour le SCARA et le Delta 3 
	//
	// Bind le VBO
	glBindBuffer(GL_ARRAY_BUFFER,vboID[0]);
	// Format des attributs de positions
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(sVertex3Dcolor),(const GLvoid*) (G_STRUCT_OFFSET(sVertex3Dcolor,position))); // (Index,Composantes(XYZ),Types,...)
	glEnableVertexAttribArray(0);	
	// Format des attributs de couleurs
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(sVertex3Dcolor),(const GLvoid*) (G_STRUCT_OFFSET(sVertex3Dcolor,color)));	// (Index,Composantes(RVB),Types,...)
	glEnableVertexAttribArray(1);
	// Format des attributs de texture
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(sVertex3Dcolor),(const GLvoid*) (G_STRUCT_OFFSET(sVertex3Dcolor,texture)));	// (Index,Composantes(XY),Types,...)
	glEnableVertexAttribArray(2);

	// SCARA
	if (frame==0)
	{
		// Pied (Cube VBO 0)

		// Applique les transformées globales
		glm_translate(stack[0],(vec3){0.2,0.25,0.2});

		// Pousse la pile
		push(stack);

			// Applique les transformées locales
			glm_scale(stack[0],(vec3){0.2,0.5,0.2}); // Mise à l'échelle du cube

			// Récupération des ID
			uMatrix=glGetUniformLocation(program,"uMVP");
			// Envoi la matrice au shader
			glUniformMatrix4fv(uMatrix,1,GL_FALSE,(float*)stack[0]);

			// Dessine
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glDrawArrays(GL_TRIANGLES,0,36); // (type de primitive,vertex de départ,nombre total de vertices)
		
		// Remonte la pile
		pop(stack);


		// Bras (Cube VBO 0)

		// Applique les transformées globales
		glm_rotate(stack[0],degreesToRadians(drawingArg.Rotate_sliderValue_Joint1),(vec3){0.0,1.0,0.0});
		glm_translate(stack[0],(vec3){0.5,0.0,0.0});
		glm_translate(stack[0],(vec3){0.0,0.30,0.0});

		// Pousse la pile
		push(stack);

			// Applique les transformées locales
			glm_scale(stack[0],(vec3){1.0, 0.1, 0.2}); // Mise à l'échelle du cube

			// Récupération des ID
			uMatrix=glGetUniformLocation(program,"uMVP");
			// Envoi la matrice au shader
			glUniformMatrix4fv(uMatrix,1,GL_FALSE,(float*)stack[0]);

			// Dessine
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glDrawArrays(GL_TRIANGLES,0,36); // (type de primitive,vertex de départ,nombre total de vertices)

		// Remonte la pile
		pop(stack);


		// Avant bras (Cube VBO 0)

		// Applique les transformées globales
		glm_translate(stack[0],(vec3){0.5,0.0,0.0});													
		glm_rotate(stack[0],degreesToRadians(drawingArg.Rotate_sliderValue_Joint2),(vec3){0.0,1.0,0.0});
		glm_translate(stack[0],(vec3){0.5,0.0,0.0});														

		// Pousse la pile
		push(stack);

			// Applique les transformées locales
			glm_scale(stack[0],(vec3){1.0,0.1,0.2}); // Mise à l'échelle du cube

			// Récupération des ID
			uMatrix=glGetUniformLocation(program,"uMVP");
			// Envoi la matrice au shader
			glUniformMatrix4fv(uMatrix,1,GL_FALSE,(float*)stack[0]);

			// Dessine
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glDrawArrays(GL_TRIANGLES,0,36); // (type de primitive,vertex de départ,nombre total de vertices)

		// Remonte la pile
		pop(stack);


		// Tige plongeante (Cube VBO 0)

		// Applique les transformées globales
		glm_translate(stack[0],(vec3){0.5,-CLAMP(drawingArg.Rotate_sliderValue_Joint3,0,180)/720.0,0.0});
		glm_rotate(stack[0],degreesToRadians(drawingArg.Rotate_sliderValue_Joint4),(vec3){0.0,1.0,0.0});												

		// Pousse la pile
		push(stack);

			// Applique les transformées locales
			glm_scale(stack[0],(vec3){0.05,0.5,0.05}); // Mise à l'échelle du cube

			// Récupération des ID
			uMatrix=glGetUniformLocation(program,"uMVP");
			// Envoi la matrice au shader
			glUniformMatrix4fv(uMatrix,1,GL_FALSE,(float*)stack[0]);

			// Dessine
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glDrawArrays(GL_TRIANGLES,0,36); // (type de primitive,vertex de départ,nombre total de vertices)

		// Remonte la pile
		pop(stack);
	}

	// Delta 3
	if (frame==2)
	{
		// Paramètres du robot delta 3
		Rf=0.20;	// Rayon de la plateforme fixe supérieure (base)
		Re=0.05;	// Rayon de la nacelle mobile (wrist)
		Lf=0.42;	// longueur du bras
		Le=0.95;	// longueur de l'avant-bras
		Delta3ACS.thetaA=drawingArg.Rotate_sliderValue_Joint1;	// position angulaire du premier bras (degrés)
		Delta3ACS.thetaB=drawingArg.Rotate_sliderValue_Joint2;	// position angulaire du deuxième bras (degrés)
		Delta3ACS.thetaC=drawingArg.Rotate_sliderValue_Joint3;	// position angulaire du troisième bras (degrés)
/*
		// Calcule la position de la nacelle mobile par rapport au repère cartésien du robot (XYZ) 
		Delta3MCS=forward(Re,Rf,Lf,Le,Delta3ACS);	

		// Calcule les positions des poignets par rapport aux repères cartésiens des bras (XiYiZi)
		pWristPos=wrist(Delta3MCS,Rf,Re);

		// Calcule les orientations latérales et longitudinales des avant-bras (à la jointure des coudes)
		pElbowAngles=elbow(Delta3MCS,pWristPos,Lf,Le);

		// Dessine les trois bras
		for (loop=0;loop<=2;loop++)
		{
			// Sauvegarde la matrice de vue originale
			push(stack);

				// Bras (Cube VBO 0)
		
				// Applique les transformées globales
				glm_rotate(stack[0],degreesToRadians(loop*120),(vec3){0.0,1.0,0.0}); // Bras 0,1,2 (0,120,240 degrés)
				glm_translate(stack[0],(vec3){Rf,0.0,0.0}); // Rf
				switch(loop)
				{
					case 0 :
					glm_rotate(stack[0],degreesToRadians(-Delta3ACS.thetaA),(vec3){0.0,0.0,1.0}); // Inclinaison bras 0
					break;
					case 1 :
					glm_rotate(stack[0],degreesToRadians(-Delta3ACS.thetaB),(vec3){0.0,0.0,1.0}); // Inclinaison bras 1
					break;
					case 2 :
					glm_rotate(stack[0],degreesToRadians(-Delta3ACS.thetaC),(vec3){0.0,0.0,1.0}); // Inclinaison bras 2
					break;
				}
				glm_translate(stack[0],(vec3){(Lf/2),0.0,0.0}); // Lf/2

				// Pousse la pile
				push(stack);
					// Applique les transformées locales
					glm_scale(stack[0],(vec3){Lf, 0.025, 0.025}); // Lf

					// Récupération des ID
					uMatrix=glGetUniformLocation(program,"uMVP");
					// Envoi la matrice au shader
					glUniformMatrix4fv(uMatrix,1,GL_FALSE,(float*)stack[0]);

					// Dessine
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LEQUAL);
					glDrawArrays(GL_TRIANGLES,0,36); // (type de primitive,vertex de départ,nombre total de vertices)
				// Remonte la pile
				pop(stack);


				// Avant bras (Cube VBO 0)

				// Applique les transformées globales
				glm_translate(stack[0],(vec3){(Lf/2),0.0,0.0}); // Lf/2
				glm_rotate(stack[0],degreesToRadians(-(pElbowAngles[loop].q3i-90)),(vec3){1.0,0.0,0.0}); // q3i : orientation latérale
				glm_rotate(stack[0],degreesToRadians(-pElbowAngles[loop].q2i),(vec3){0.0,0.0,1.0}); // q2i : orientation longitudinale
				glm_translate(stack[0],(vec3){(Le/2),0.0,0.0}); // Le/2													

				// Pousse la pile
				push(stack);
					// Applique les transformées locales
					glm_scale(stack[0],(vec3){Le,0.025,0.025}); // Le

					// Récupération des ID
					uMatrix=glGetUniformLocation(program,"uMVP");
					// Envoi la matrice au shader
					glUniformMatrix4fv(uMatrix,1,GL_FALSE,(float*)stack[0]);

					// Dessine
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LEQUAL);
					glDrawArrays(GL_TRIANGLES,0,36); // (type de primitive,vertex de départ,nombre total de vertices)
				// Remonte la pile
				pop(stack);
			
			// Restitue la matrice de vue originale
			pop(stack);	
		}
*/


// *********************
// ** TEST newdelta2  **
// *********************

		// VBO et ses attributs
		//
		// Bind le VBO
		glBindBuffer(GL_ARRAY_BUFFER,vboID[2]);
		// Format des attributs de positions
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(sVertex3Dcolor),(const GLvoid*) (G_STRUCT_OFFSET(sVertex3Dcolor,position))); // (Attribut,Composantes(XYZ),Types,...)
		glEnableVertexAttribArray(0);	
		// Format des attributs de couleurs
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(sVertex3Dcolor),(const GLvoid*) (G_STRUCT_OFFSET(sVertex3Dcolor,color)));	// (Attribut,Composantes(RVB),Types,...)
		glEnableVertexAttribArray(1);
		// Format des attributs de texture
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(sVertex3Dcolor),(const GLvoid*) (G_STRUCT_OFFSET(sVertex3Dcolor,texture)));	// (Index,Composantes(XY),Types,...)
		glEnableVertexAttribArray(2);
		
		// Dessine la plateforme fixe supérieure (base)
		pBasePos=base(Rf);
		
		for (lineNb=0;lineNb<2;lineNb++)
		{
			dot[0]=pBasePos[lineNb].x; // X
			dot[1]=pBasePos[lineNb].y; // Y
			dot[2]=pBasePos[lineNb].z; // Z
			glBufferSubData(GL_ARRAY_BUFFER,(lineNb*2)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
			dot[0]=pBasePos[lineNb+1].x; // X
			dot[1]=pBasePos[lineNb+1].y; // Y
			dot[2]=pBasePos[lineNb+1].z; // Z
			glBufferSubData(GL_ARRAY_BUFFER,(lineNb*2+1)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
		}

		lineNb=2;
		dot[0]=pBasePos[lineNb].x; // X
		dot[1]=pBasePos[lineNb].y; // Y
		dot[2]=pBasePos[lineNb].z; // Z
		glBufferSubData(GL_ARRAY_BUFFER,(lineNb*2)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
		dot[0]=pBasePos[lineNb-2].x; // X
		dot[1]=pBasePos[lineNb-2].y; // Y
		dot[2]=pBasePos[lineNb-2].z; // Z
		glBufferSubData(GL_ARRAY_BUFFER,(lineNb*2+1)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);

		// Dessine les bras
		pShoulderPos=shoulder(Rf);
		pElbowPos=elbow(Delta3ACS,Re,Rf,Lf);

		dot[0]=pShoulderPos[0].x; // X
		dot[1]=pShoulderPos[0].y; // Y
		dot[2]=pShoulderPos[0].z; // Z
		glBufferSubData(GL_ARRAY_BUFFER,(6)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
		dot[0]=pElbowPos[0].x; // X
		dot[1]=pElbowPos[0].y; // Y
		dot[2]=pElbowPos[0].z; // Z
		glBufferSubData(GL_ARRAY_BUFFER,(7)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);

		dot[0]=pShoulderPos[1].x; // X
		dot[1]=pShoulderPos[1].y; // Y
		dot[2]=pShoulderPos[1].z; // Z
		glBufferSubData(GL_ARRAY_BUFFER,(8)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
		dot[0]=pElbowPos[1].x; // X
		dot[1]=pElbowPos[1].y; // Y
		dot[2]=pElbowPos[1].z; // Z
		glBufferSubData(GL_ARRAY_BUFFER,(9)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
/*
		dot[0]=pShoulderPos[2].x; // X
		dot[1]=pShoulderPos[2].y; // Y
		dot[2]=pShoulderPos[2].z; // Z
		glBufferSubData(GL_ARRAY_BUFFER,(10)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
		dot[0]=pElbowPos[2].x; // X
		dot[1]=pElbowPos[2].y; // Y
		dot[2]=pElbowPos[2].z; // Z
		glBufferSubData(GL_ARRAY_BUFFER,(11)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
*/
		// Matrice d'identité
		glm_mat4_identity(identity);

		// MVP
		glm_mat4_mul(viewProj,identity,stack[0]);

		// Récupération des ID
		uMatrix=glGetUniformLocation(program,"uMVP");
		// Envoi la matrice au shader
		glUniformMatrix4fv(uMatrix,1,GL_FALSE,(float*)stack[0]);

		// Dessine
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_LINES,0,10); // (type de primitive,vertex de départ,nombre total de vertices)	
	}


	#ifndef Repère_Cartésien_OpenGL
		// VBO et ses attributs utilisés pour le repère cartésien 
		//
		// Bind le VBO
		glBindBuffer(GL_ARRAY_BUFFER,vboID[1]);
		// Format des attributs de positions
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(sVertex3Dcolor),(const GLvoid*) (G_STRUCT_OFFSET(sVertex3Dcolor,position))); // (Attribut,Composantes(XYZ),Types,...)
		glEnableVertexAttribArray(0);	
		// Format des attributs de couleurs
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(sVertex3Dcolor),(const GLvoid*) (G_STRUCT_OFFSET(sVertex3Dcolor,color)));	// (Attribut,Composantes(RVB),Types,...)
		glEnableVertexAttribArray(1);

		// Repère cartésien (Lignes VBO 1)

		// Matrice d'identité
		glm_mat4_identity(identity);

		// MVP
		glm_mat4_mul(viewProj,identity,stack[0]);

		// Récupération des ID
		uMatrix=glGetUniformLocation(program,"uMVP");
		// Envoi la matrice au shader
		glUniformMatrix4fv(uMatrix,1,GL_FALSE,(float*)stack[0]);

		// Dessine
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_LINES,0,6); // (type de primitive,vertex de départ,nombre total de vertices)
	#endif

	#ifndef Débug
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pF_trans_check_button)))
	{
		system("cls");
		
		// Affiche la matrice du modèle hiérarchique
		//print_mat4(stack[0]);

		// Affiche la position de la nacelle mobile par rapport au repère cartésien du robot (XYZ)
		printf("MCS : X = %f, Y = %f, Z = %f\n",Delta3MCS.x,Delta3MCS.y,Delta3MCS.z);	
	}
	#endif
}


// Clôture
void ending()
{
	// Libération des ressources
	glDeleteProgram(program);		
	glDeleteBuffers(3,vboID);			// (nombre de buffer(s) à supprimer, buffer(s))
	glDeleteVertexArrays(1,vaoID);
}
