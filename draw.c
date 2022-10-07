/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#include <gtk/gtk.h>		// Gimp Tool Kit
#include <epoxy/gl.h>		// OpenGL
#include <unistd.h>			// getcwd
#include <fsjslib.h>		// Entry parser


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
extern GtkWidget 	*pProjMatrix_switch,*pAspect_switch,*pF_trans_check_button,*pWireframe_switch,*pRobotsSettings_dialog,
					*pEntry_Rf,*pEntry_Re,*pEntry_Lf,*pEntry_Le,*pEntry_L1,*pEntry_DX,*pEntry_DY,*pEntry_DZ,*pEntry_Nr1,
					*pEntry_Dr1,*pEntry_Nr12,*pEntry_Dr12,*pEntry_Nr2,*pEntry_Dr2;
extern int frame;
extern sArgsComputeThread computeThreadArgs;

// Types de base
GLuint program,vboID[3],vaoID[1],textureID;


// Structures
sViewControlArg viewControlArg;

// Pointeurs (sur tableaux, sur chaînes, etc.)
FILE *pFileSettings;

// Pointeurs de widgets de l'interface

// Variables de la fonction drawing (globales pour gagner du temps CPU à chaque frame)
GLint uMatrix;
vec3 dot;
mat4 projection,view,viewProj,identity,stack[5];
float aspect_ratio;
sViewControlRet responseViewControl;
sfEisIntFloat EntryParser;
int loop,lineNb,dotNb;
double Lf,Le,Rf,Re,yawRot,pitchRot,Nr1,Dr1,Nr12,Dr12,Nr2,Dr2,L1,DX,DY,DZ,Pn[15]; // Paramètre d'un delta 5
const char* pcharEntry[15];
char pcharMessage[50+1]="";
sDeltaACS Delta3ACS;
sDeltaMCS Delta3MCS;
sBase* pBasePos;
sShoulder* pShoulderPos;
sElbow* pElbowPos;
sWrist* pWristPos;
bool entryNG[12];


// INITIALISATIONS
void init(GtkWidget* pMessages_display)
{
	// ********** Déclarations (locales) ************
	sReturnFnc_LdSha responseLdShaVS;
	sReturnFnc_LdSha responseLdShaFS;
	sReturnFnc_LdTex responseLdTexture;
	char pathToFile[UCHAR_MAX];
	int numVBOs, numVAOs, numTEXTs;
	// *********** Fin de déclarations **************

	// Ouvre le fichier de paramètres
	pFileSettings=fopen("3D_kinematic_settings.txt", "w");

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
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}}, // Base
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}}, // Bras
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}}, // Avant-bras
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{0.0,1.0,1.0},	{0,0}},
		{{0.0,0.0,0.0},	{181.0/255.0,176.0/255.0,161.0/255.0},	{0,0}}, // Nacelle mobile
		{{0.0,0.0,0.0},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,0}},
		{{0.0,0.0,0.0},	{181.0/255.0,176.0/255.0,161.0/255.0},	{1,1}},
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

	// Récupère les paramètres initiaux du delta
	Rf=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_Rf)));
	Re=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_Re)));
	Lf=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_Lf)));
	Le=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_Le)));
	L1=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_L1)));
	DX=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_DX)));		
	DY=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_DY)));
	DZ=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_DZ)));
	Nr1=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_Nr1)));
	Dr1=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_Dr1)));
	Nr12=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_Nr12)));
	Dr12=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_Dr12)));
	Nr2=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_Nr2)));
	Dr2=atof(gtk_entry_get_text(GTK_ENTRY(pEntry_Dr2)));
	}

// AFFICHAGE
void drawing(sDrawingArg drawingArg,GtkWidget* pMessages_display)
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

	// SCARA
	if (frame==0)
	{
		// VBO et ses attributs
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

	// DELTA
	if (frame==2 || frame==3)
	{
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

		// Analyse la saisie des paramètres
		if (gtk_window_is_active(GTK_WINDOW(pRobotsSettings_dialog)))
		{
			// Récupère le contenu alphanumérique des zones de saisies
			pcharEntry[0]=gtk_entry_get_text(GTK_ENTRY(pEntry_Rf));
			pcharEntry[1]=gtk_entry_get_text(GTK_ENTRY(pEntry_Re));
			pcharEntry[2]=gtk_entry_get_text(GTK_ENTRY(pEntry_Lf));
			pcharEntry[3]=gtk_entry_get_text(GTK_ENTRY(pEntry_Le));
			pcharEntry[4]=gtk_entry_get_text(GTK_ENTRY(pEntry_L1));
			pcharEntry[5]=gtk_entry_get_text(GTK_ENTRY(pEntry_DX));		
			pcharEntry[6]=gtk_entry_get_text(GTK_ENTRY(pEntry_DY));
			pcharEntry[7]=gtk_entry_get_text(GTK_ENTRY(pEntry_DZ));
			pcharEntry[8]=gtk_entry_get_text(GTK_ENTRY(pEntry_Nr1));
			pcharEntry[9]=gtk_entry_get_text(GTK_ENTRY(pEntry_Dr1));
			pcharEntry[10]=gtk_entry_get_text(GTK_ENTRY(pEntry_Nr12));
			pcharEntry[11]=gtk_entry_get_text(GTK_ENTRY(pEntry_Dr12));
			pcharEntry[12]=gtk_entry_get_text(GTK_ENTRY(pEntry_Nr2));
			pcharEntry[13]=gtk_entry_get_text(GTK_ENTRY(pEntry_Dr2));

			// Analyse les caractères saisis
			for (loop=0;loop<=13;loop++)
			{
				EntryParser.pcharEntry=pcharEntry[loop];
				EntryParser.type=typeFloat;
				EntryParser.min=-DBL_MAX; // limite basse
				EntryParser.max=DBL_MAX; // limite haute
				fEntryIsIntOrFloat(&EntryParser);
				if (EntryParser.errNumber!=0)
				{
					if (entryNG[loop]!=true)
					{
						sprintf(pcharMessage,"%s\n",EntryParser.pcharErrMessage);
						fMessage(pcharMessage,pMessages_display);
						entryNG[loop]=true;
					}
				}
				else
				{
					Pn[loop]=EntryParser.doubleResult;
					entryNG[loop]=false;
				}
			}
			
			// Modifie les paramètres
			Rf=Pn[0];
			Re=Pn[1];
			Lf=Pn[2];
			Le=Pn[3];
			L1=Pn[4];
			DX=Pn[5];
			DY=Pn[6];
			DZ=Pn[7];
			Nr1=Pn[8];
			Dr1=Pn[9];
			Nr12=Pn[10];
			Dr12=Pn[11];
			Nr2=Pn[12];
			Dr2=Pn[13];
		}

		// positions angulaires des bras
		Delta3ACS.thetaA=drawingArg.Rotate_sliderValue_Joint1;
		Delta3ACS.thetaB=drawingArg.Rotate_sliderValue_Joint2;
		Delta3ACS.thetaC=drawingArg.Rotate_sliderValue_Joint3;

		// Transformée directe (ACS --> MCS)
		Delta3MCS=forward(Re,Rf,Lf,Le,Delta3ACS);	

		// Points des lignes formant la plateforme fixe supérieure (base)
		pBasePos=base(Rf);
		
		for (lineNb=0;lineNb<=2;lineNb++)
		{
			if (lineNb<2)
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
			else
			{
				dot[0]=pBasePos[lineNb].x; // X
				dot[1]=pBasePos[lineNb].y; // Y
				dot[2]=pBasePos[lineNb].z; // Z
				glBufferSubData(GL_ARRAY_BUFFER,(lineNb*2)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
				dot[0]=pBasePos[lineNb-2].x; // X
				dot[1]=pBasePos[lineNb-2].y; // Y
				dot[2]=pBasePos[lineNb-2].z; // Z
				glBufferSubData(GL_ARRAY_BUFFER,(lineNb*2+1)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
			}
		}

		// Points des lignes formant les bras et les avant-bras
		pShoulderPos=shoulder(Rf);
		pElbowPos=elbow(Delta3ACS,Re,Rf,Lf);
		pWristPos=wrist(Delta3MCS,Re);

		for (lineNb=0;lineNb<=2;lineNb++)
		{
			// Bras
			dot[0]=pShoulderPos[lineNb].x; // X
			dot[1]=pShoulderPos[lineNb].y; // Y
			dot[2]=pShoulderPos[lineNb].z; // Z
			glBufferSubData(GL_ARRAY_BUFFER,(lineNb*2+6)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
			dot[0]=pElbowPos[lineNb].x; // X
			dot[1]=pElbowPos[lineNb].y; // Y
			dot[2]=pElbowPos[lineNb].z; // Z
			glBufferSubData(GL_ARRAY_BUFFER,(lineNb*2+7)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);

			// Avant-bras
			dot[0]=pElbowPos[lineNb].x; // X
			dot[1]=pElbowPos[lineNb].y; // Y
			dot[2]=pElbowPos[lineNb].z; // Z
			glBufferSubData(GL_ARRAY_BUFFER,(lineNb*2+12)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
			dot[0]=pWristPos[lineNb].x; // X
			dot[1]=pWristPos[lineNb].y; // Y
			dot[2]=pWristPos[lineNb].z; // Z
			glBufferSubData(GL_ARRAY_BUFFER,(lineNb*2+13)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
		}

		// Points du triangle formant la plateforme mobile
		for (dotNb=0;dotNb<=2;dotNb++)
		{
			dot[0]=pWristPos[dotNb].x; // X
			dot[1]=pWristPos[dotNb].y; // Y
			dot[2]=pWristPos[dotNb].z; // Z
			glBufferSubData(GL_ARRAY_BUFFER,(dotNb+18)*sizeof(sVertex3Dcolor),sizeof(vec3),&dot);
		}
		
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
		glDrawArrays(GL_LINES,0,18); // (type de primitive,vertex de départ,nombre total de vertices)
		glDrawArrays(GL_TRIANGLES,17,3); // (type de primitive,vertex de départ,nombre total de vertices)

		// Axes 4 et 5 du delta 5
		if (frame==3)
		{
			// VBO et ses attributs
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

			// Joint 4 (axe 1 poignet) (Cube VBO 0)
			yawRot=drawingArg.Rotate_sliderValue_Joint4*(Nr1/Dr1);

			// Applique les transformées globales
			glm_translate(stack[0],(vec3){Delta3MCS.x+DX,Delta3MCS.y,Delta3MCS.z+DY});
			glm_rotate(stack[0],degreesToRadians(yawRot),(vec3){0.0,1.0,0.0});

			// Pousse la pile
			push(stack);

				// Applique les transformées locales
				glm_scale(stack[0],(vec3){0.02,-DZ,0.02});
				glm_translate(stack[0],(vec3){0.0,-0.5,0.0});
				
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

			// Joint 5 (axe 2 poignet) (Cube VBO 0)
			pitchRot=(drawingArg.Rotate_sliderValue_Joint4*(Nr12/Dr12)+drawingArg.Rotate_sliderValue_Joint5)*(Nr2/Dr2);

			// Applique les transformées globales

			// Pousse la pile
			//push(stack);

				// Applique les transformées locales
				glm_translate(stack[0],(vec3){0.0,DZ,0.0});
				glm_rotate(stack[0],degreesToRadians(pitchRot),(vec3){0.0,0.0,1.0});
				glm_translate(stack[0],(vec3){0.0,-L1/2,0.0});
				glm_scale(stack[0],(vec3){0.02,L1,0.02});

				// Récupération des ID
				uMatrix=glGetUniformLocation(program,"uMVP");
				// Envoi la matrice au shader
				glUniformMatrix4fv(uMatrix,1,GL_FALSE,(float*)stack[0]);

				// Dessine
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				glDrawArrays(GL_TRIANGLES,0,36); // (type de primitive,vertex de départ,nombre total de vertices)
			
			// Remonte la pile
			//pop(stack);
		}
	}

	// REPERE CARTESIEN
	;{
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
	;}

	// DEBUG
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pF_trans_check_button)))
	{
		system("cls");
		
		// Affiche la matrice du modèle hiérarchique
		//print_mat4(stack[0]);

		// Affiche la position de la nacelle mobile par rapport au repère cartésien du robot (XYZ)
		printf("MCS : X = %f, Y = %f, Z = %f\n",Delta3MCS.x,Delta3MCS.y,Delta3MCS.z);	
	}
}

// CLOTURE
void ending()
{
	// Libération des ressources
	glDeleteProgram(program);		
	glDeleteBuffers(3,vboID);			// (nombre de buffer(s) à supprimer, buffer(s))
	glDeleteVertexArrays(1,vaoID);

	// Ferme le fichier de paramètres
	fclose(pFileSettings);
}

