/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#include <gtk/gtk.h>	// Gimp Tool Kit
#include <pthread.h>
#include <stdio.h>		// sprintf, FILE, fopen, fclose
#include <stdlib.h>		// atoi, malloc, calloc, sizeof, free
#include <string.h>		// strlen, strcmp
// #include <ctype.h>		// isdigit
#include <winsock2.h>							// Sockets TCP (avec la librairie ws2_32.lib)
#include <windows.h> 	// ShellExecute
#include <shellapi.h> 	// ShellExecute
#include <float.h>		// Limites
// #include <mmsystem.h>	// Sons
#include <fsjslib.h>	// Ma librairie
#include <epoxy/gl.h>	// OpenGL

// Des Headers situés dans le répertoire de l'application
#include "callback.h"
#include "draw.h"
#include "functions.h"
#include "kinematics.h"
#include "udp.h"
#include "opc.h"

// Des définitions locales


/**********************************************
 ********** Déclarations (globales) ***********
 **********************************************
*/

// Externes   
extern GtkBuilder* pConstInterface; // pConstInterface vient d'un autre fichier source
extern sViewControlArg viewControlArg;
extern sSYSMAC_datas plc_datas;

// Types de base
float rotate_joint1,rotate_joint2,rotate_joint3,rotate_joint4,rotate_joint5,rotate_joint6;
float memXpos,memYpos,mouseWheel,FOVy_sliderValue;
int height,width,frame;

// Structures, unions, énumérations
sArgsComputeThread computeThreadArgs;

// Pointeurs (sur tableaux, sur chaînes, etc.)
GSocket* pSock;
float pIMUcoordinates[3];
void* (*pComputeThread) (void*);
sArgsComputeThread* pComputeThreadArgs;

// Pointeurs de widgets de l'interface
GtkWidget 	*pMaFenetre,*pAbout_dialog,*pMessages_display,*p3Dview,*pRotate_slider_joints,
			*pImu_check_button,*pCompass_value,*pPitch_value,*pRoll_value,*pGpu_period,*pGpu_frequency,*pGpu_opengl_ver,
			*pGpu_renderer_type,*pGpu_shading_lang_ver,*pGpu_manufacturer,*pCross_button,*pArrows_button,
			*pProjMatrix_switch,*pAspect_switch,*pF_trans_check_button,*pWireframe_switch,*pZero_button,
			*pFOVy_slider,*pJoint_choice_combobox,*pOpc_check_button,*pPlc_ip_address_entry,*pFrame_choice_combobox,
			*pRobotsSettings_dialog,*pEntry_Rf,*pEntry_Re,*pEntry_Lf,*pEntry_Le,*pEntry_L1,*pEntry_DX,
			*pEntry_DY,*pEntry_DZ,*pEntry_Nr1,*pEntry_Dr1,*pEntry_Nr12,*pEntry_Dr12,*pEntry_Nr2,*pEntry_Dr2,*pDark_switch;
	
/**********************************************
 ************ Fonctions callback **************
 **********************************************
*
* Ces fonctions sont liées à des événements (button click, etc.)
* Remarque : la macro G_MODULE_EXPORT exporte la fonction associée dans une table de symboles d'application au moment de la compilation
* Cette table est en suite surveillée par le "gtk_builder_connect_signals" du main.c
*
*/


// Ouverture de la fenêtre principale 
G_MODULE_EXPORT void on_window1_map_event(GtkWindow* pWindow,gpointer pData)
{
	// ********** Déclarations (locales) ************
	// *********** Fin de déclarations **************
		

	// Initialise les pointeurs des widgets de l'interface
	pMaFenetre=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"window1"));
	pAbout_dialog=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"About_dialog"));
	pMessages_display=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"status_viewer"));
	p3Dview=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"3Dview"));
	pRotate_slider_joints=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"rotate_slider_joints"));
	pImu_check_button=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"imu_check_button"));
	pCompass_value=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"compass_value"));
	pPitch_value=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"pitch_value"));
	pRoll_value=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"roll_value"));
	pGpu_period=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"gpu_period"));
	pGpu_frequency=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"gpu_frequency"));
	pGpu_opengl_ver=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"gpu_opengl_ver"));
	pGpu_renderer_type=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"gpu_renderer_type"));
	pGpu_shading_lang_ver=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"gpu_shading_lang_ver"));
	pGpu_manufacturer=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"gpu_manufacturer"));
	pCross_button=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"cross_button"));
	pArrows_button=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"arrows_button"));
	pProjMatrix_switch=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"projMatrix_switch"));
	pAspect_switch=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"aspect_switch"));
	pF_trans_check_button=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"f_trans_check_button"));
	pWireframe_switch=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"wireframe_switch"));
	pFOVy_slider=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"FOVy_slider"));
	pJoint_choice_combobox=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"joint_choice_combobox"));
	pOpc_check_button=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"opc_check_button"));
	pPlc_ip_address_entry=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"plc_ip_address_entry"));
	pFrame_choice_combobox=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"frame_choice_combobox"));
	pRobotsSettings_dialog=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"robotsSettings_dialog"));
	pEntry_Rf=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_Rf"));
	pEntry_Re=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_Re"));
	pEntry_Lf=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_Lf"));
	pEntry_Le=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_Le"));
	pEntry_L1=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_L1"));
	pEntry_DX=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_DX"));
	pEntry_DY=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_DY"));
	pEntry_DZ=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_DZ"));
	pEntry_Nr1=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_Nr1"));
	pEntry_Dr1=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_Dr1"));
	pEntry_Nr12=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_Nr12"));
	pEntry_Dr12=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_Dr12"));
	pEntry_Nr2=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_Nr2"));
	pEntry_Dr2=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"entry_Dr2"));
	pDark_switch=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"dark_switch"));

	
	// Configuration du widget rotate_slider_joints
	gtk_scale_set_digits(GTK_SCALE(pRotate_slider_joints),1);				// Nombre de chiffres après la virgule
	gtk_range_set_range(GTK_RANGE(pRotate_slider_joints),-180.0,180.0); 	// Limites haute et basse
	gtk_range_set_increments(GTK_RANGE(pRotate_slider_joints),1.0,1.0); 	// Valeur d'incrémentation des flèches et des touches Up/Down
	gtk_range_set_value(GTK_RANGE(pRotate_slider_joints),0.0); 				// Valeur par défaut

	// Configuration du widget FOVy_slider
	gtk_scale_set_digits(GTK_SCALE(pFOVy_slider),1);
	gtk_range_set_range(GTK_RANGE(pFOVy_slider),44.0,47.0);
	gtk_range_set_increments(GTK_RANGE(pFOVy_slider),0.01,0.1);
	gtk_range_set_value(GTK_RANGE(pFOVy_slider),45.0);
	
	// Active l'écoute des signaux de bas niveau
	gtk_widget_add_events(pMaFenetre,GDK_POINTER_MOTION_MASK);
	gtk_widget_add_events(pMaFenetre,GDK_SCROLL_MASK);
	gtk_widget_add_events(pMaFenetre,GDK_BUTTON_PRESS_MASK);
	gtk_widget_add_events(pMaFenetre,GDK_BUTTON_RELEASE_MASK);

	// Initialisation du socket UDP
	udpInit(pSock);

	// Initialisation du point de vue
	viewControlArg.initView=true;

	// Point de vue initial
	mouseWheel=2;							// Distance (axe z) entre l'objet et la caméra
	viewControlArg.mouseWheel=mouseWheel;	// Renseigne le calcul de point de vue

	// Initialisation du FOVy à la valeur standard
	gtk_range_set_value(GTK_RANGE(pFOVy_slider),45.0);

	// Initialisation de la position du robot
	rotate_joint1=0.0;
	rotate_joint2=0.0;
	rotate_joint3=0.0;
	rotate_joint4=0.0;
	rotate_joint5=0.0;
	rotate_joint6=0.0;

	// Initialisation du type de frame
	frame=2;
	gtk_combo_box_set_active(GTK_COMBO_BOX(pFrame_choice_combobox),frame);
	
	// Appel périodique de fonction fPeriodicRead
	g_timeout_add(100,fPeriodicRead,pData);

	// Thread de calcul
	
		// Initialisation du pointeur de la fonction exécutée dans un thread
		pComputeThread=&(computeThread);

		// Initialisation du pointeur d'arguments de la fonction
		pComputeThreadArgs=&(computeThreadArgs);

		// Variable contenant le thread
		pthread_t compute_thread;
		
		// Création du thread
		pthread_create(&compute_thread,NULL,pComputeThread,(void*)pComputeThreadArgs);

	// Initialisation des paramètres du delta 3 (en mètres)
	gtk_entry_set_text(GTK_ENTRY(pEntry_Rf),"0.2"); 	// Rayon de la plateforme fixe supérieure (base)
	gtk_entry_set_text(GTK_ENTRY(pEntry_Re),"0.05");	// Rayon de la nacelle mobile
	gtk_entry_set_text(GTK_ENTRY(pEntry_Lf),"0.42");	// Longueur des bras
	gtk_entry_set_text(GTK_ENTRY(pEntry_Le),"0.95");	// Longueur des avant-bras

	// Initialisation des paramètres du poignet du delta 5 (en mètres)
	gtk_entry_set_text(GTK_ENTRY(pEntry_L1),"0.05");	// Longueur du bras balancier
	gtk_entry_set_text(GTK_ENTRY(pEntry_DX),"0.0");		// Offset par raport au TCP du delta
	gtk_entry_set_text(GTK_ENTRY(pEntry_DY),"0.0");		// Offset par raport au TCP du delta
	gtk_entry_set_text(GTK_ENTRY(pEntry_DZ),"-0.05");	// Offset par raport au TCP du delta
	gtk_entry_set_text(GTK_ENTRY(pEntry_Nr1),"1");		// Numérateur du ratio axe 1/rotation yaw du poignet
	gtk_entry_set_text(GTK_ENTRY(pEntry_Dr1),"2");		// Dénominateur du ratio axe 1/rotation yaw du poignet
	gtk_entry_set_text(GTK_ENTRY(pEntry_Nr12),"1");		// Numérateur du ratio axe 1/rotation pitch du bras balancier
	gtk_entry_set_text(GTK_ENTRY(pEntry_Dr12),"2");		// Dénominateur du ratio axe 1/rotation pitch du bras balancier
	gtk_entry_set_text(GTK_ENTRY(pEntry_Nr2),"1");		// Numérateur du ratio axe 2/rotation pitch du bras balancier
	gtk_entry_set_text(GTK_ENTRY(pEntry_Dr2),"2");		// Dénominateur du ratio axe 2/rotation pitch du bras balancier
}


// Réception des données UDP
gboolean dataUDP_coming(GIOChannel* pSource,GIOCondition condition,gpointer pData)
{
	// Réception, traitement et affichage
	udpDataReceipt(pSource,pIMUcoordinates,pCompass_value,pPitch_value,pRoll_value);

	// Maintien la surveillance de l'événement
	return TRUE;
}


// Fermeture de la fenêtre principale
G_MODULE_EXPORT void on_window1_destroy_event(GtkWindow* pWindow,gpointer pData)
{
	// ********** Déclarations (locales) ************
	
    // *********** Fin de déclarations **************
    
	// Fermeture du socket UDP
	g_socket_close(pSock,NULL);
	
	// Libère les ressources
	ending();
}


// Pop-up "A propos"
G_MODULE_EXPORT void on_about_button_clicked(GtkButton* pButton,gpointer pData)
{
	// ********** Déclarations (locales) ************

	// *********** Fin de déclarations **************
	
	// Affiche (show) le widget et bloc l'exécution du programme dans l'attente d'une réponse
	gtk_dialog_run(GTK_DIALOG(pAbout_dialog));
	
	/*
	 * Dès que la fonction gtk_dialog_run sort de sa boucle récursive,
	 * cette fonction cache le widget mais ne le détruit pas
	 */
	gtk_widget_hide(pAbout_dialog);
}


// Initialisation du rendu
G_MODULE_EXPORT void on_3Dview_resize(GtkGLArea* p3Dview)
{
	// ********** Déclarations (locales) ************
	sVersionArg versionArg;
	// *********** Fin de déclarations **************

	// We need to make the context current if we want to
	// call GL API
	gtk_gl_area_make_current(GTK_GL_AREA(p3Dview));

	// If there were errors during the initialization or
	// when trying to make the context current, this
	// function will return a #GError for you to catch
	if (gtk_gl_area_get_error(GTK_GL_AREA(p3Dview))!=NULL)
	{
		printf("Failed to make the context current\n");
	}
    
	// Activation du depth buffer
	gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(p3Dview),TRUE);

	// Nécessairement à FALSE pour forcer le rendu avec gtk_gl_area_queue_render() ?
	gtk_gl_area_set_auto_render(GTK_GL_AREA(p3Dview),FALSE);
	
	// Récupère la taille de la GtkGLarea
	height=gtk_widget_get_allocated_height(GTK_WIDGET(p3Dview));
	width=gtk_widget_get_allocated_width(GTK_WIDGET(p3Dview));
	
	viewControlArg.glAreaHeight=height;
	viewControlArg.glAreaWidth=width;
    
	// Initialisation des shaders, vbo, vao, ibo, etc.
	init(pMessages_display);

	// Affichage de la version du GPU et celle de l'OpenGL
	versionArg.pGpu_opengl_ver=pGpu_opengl_ver;
	versionArg.pGpu_renderer_type=pGpu_renderer_type;
	versionArg.pGpu_shading_lang_ver=pGpu_shading_lang_ver;
	versionArg.pGpu_manufacturer=pGpu_manufacturer;
	version(versionArg);

	// Horloge de synchronisation d'image
	GdkGLContext* pGLcontext=gtk_gl_area_get_context(p3Dview);
	GdkWindow* pGLwindow=gdk_gl_context_get_window(pGLcontext);
	GdkFrameClock* pFrameClock=gdk_window_get_frame_clock(pGLwindow);

	// Connexion du signal de synchronisation à la fonction de rendu gtk_gl_area_queue_render
	g_signal_connect_swapped(pFrameClock,"update",G_CALLBACK(gtk_gl_area_queue_render),p3Dview);

	// Démarrage de l'horloge
	gdk_frame_clock_begin_updating(pFrameClock);
}


// Rendu
G_MODULE_EXPORT gboolean on_3Dview_render(GtkGLArea* p3Dview, GdkGLContext* pContext)
{
	// ********** Déclarations (locales) ************
	sDrawingArg drawingArg;
	// *********** Fin de déclarations **************

	// Affichage d'informations
	viewControlArg.deltatime_sec=frameRate(pGpu_period,pGpu_frequency);

	// Nettoyage de la fenêtre OpenGL avec une couleur de fond (RGB de 0.0 à 1.0)
	glClearColor(0.4, 0.4, 0.4, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Dimensions, etc.
    drawingArg.width=width;
    drawingArg.height=height;
    drawingArg.pImu_check_button=pImu_check_button;
    drawingArg.pIMUcoordinates=pIMUcoordinates;
    drawingArg.FOVy_sliderValue=FOVy_sliderValue;

    // Joints robot
	if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pOpc_check_button)))
	{
		drawingArg.Rotate_sliderValue_Joint1=rotate_joint1;
		drawingArg.Rotate_sliderValue_Joint2=rotate_joint2;
		drawingArg.Rotate_sliderValue_Joint3=rotate_joint3;
		drawingArg.Rotate_sliderValue_Joint4=rotate_joint4;
		drawingArg.Rotate_sliderValue_Joint5=rotate_joint5;
		drawingArg.Rotate_sliderValue_Joint6=rotate_joint6;
	}
	else
	{
		drawingArg.Rotate_sliderValue_Joint1=plc_datas.joint1;
		drawingArg.Rotate_sliderValue_Joint2=plc_datas.joint2;
		drawingArg.Rotate_sliderValue_Joint3=plc_datas.joint3;
		drawingArg.Rotate_sliderValue_Joint4=plc_datas.joint4;
		drawingArg.Rotate_sliderValue_Joint5=plc_datas.joint5;
		drawingArg.Rotate_sliderValue_Joint6=plc_datas.joint6;

		//system("cls");
		//printf("J1 = %f, J2 = %f, J3 = %f\n",plc_datas.joint1,plc_datas.joint2,plc_datas.joint3);
	}

	// Exécution du dessin
	drawing(drawingArg,pMessages_display);

	return TRUE;
}


// Le choix du joint change
G_MODULE_EXPORT void on_joint_choice_combobox_changed(GtkComboBox* pCombo,gpointer pData)
{
	// *********** Déclarations locales *************

	// *********** Fin de déclarations **************
	
	// Recharge l'angle en fonction du choix
	if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 1")==0)
		gtk_range_set_value(GTK_RANGE(pRotate_slider_joints),rotate_joint1);
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 2")==0)
		gtk_range_set_value(GTK_RANGE(pRotate_slider_joints),rotate_joint2);
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 3")==0)
		gtk_range_set_value(GTK_RANGE(pRotate_slider_joints),rotate_joint3);
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 4")==0)
		gtk_range_set_value(GTK_RANGE(pRotate_slider_joints),rotate_joint4);
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 5")==0)
		gtk_range_set_value(GTK_RANGE(pRotate_slider_joints),rotate_joint5);
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 6")==0)
		gtk_range_set_value(GTK_RANGE(pRotate_slider_joints),rotate_joint6);
}

// Le curseur rotate_slider_joints bouge
G_MODULE_EXPORT void on_rotate_slider_joints_value_changed(GtkScale* pScale,gpointer pData)
{
	// *********** Déclarations locales *************

	// *********** Fin de déclarations **************

	// Définit l'angle en fonction du joint choisi
	if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 1")==0)
		rotate_joint1=gtk_range_get_value(GTK_RANGE(pRotate_slider_joints));
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 2")==0)
		rotate_joint2=gtk_range_get_value(GTK_RANGE(pRotate_slider_joints));
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 3")==0)
		rotate_joint3=gtk_range_get_value(GTK_RANGE(pRotate_slider_joints));
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 4")==0)
		rotate_joint4=gtk_range_get_value(GTK_RANGE(pRotate_slider_joints));
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 5")==0)
		rotate_joint5=gtk_range_get_value(GTK_RANGE(pRotate_slider_joints));
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pJoint_choice_combobox)),"Joint 6")==0)
		rotate_joint6=gtk_range_get_value(GTK_RANGE(pRotate_slider_joints));
}


// La souris bouge
G_MODULE_EXPORT void on_window1_motion_notify_event(GtkWindow* pWindow,GdkEvent *pEvent,gpointer pData)
{
	// *********** Déclarations locales *************
	
	// *********** Fin de déclarations **************

	// Cast l'union GdkEvent vers un GdkEventMotion pour obtenir les membres x et y
	//printf("Position souris = x=%f, y=%f\r",((GdkEventMotion*)pEvent)->x,((GdkEventMotion*)pEvent)->y);

	viewControlArg.xPos=((GdkEventMotion*)pEvent)->x;
	viewControlArg.yPos=((GdkEventMotion*)pEvent)->y;	
}


// La roulette de la souris bouge
G_MODULE_EXPORT void on_window1_scroll_event(GtkWindow* pWindow,GdkEvent *pEvent,gpointer pData)
{
	// *********** Déclarations locales *************
	
	// *********** Fin de déclarations **************

	if (((GdkEventScroll*)pEvent)->direction==GDK_SCROLL_UP)
	{
		mouseWheel-=.1;
	}
	else if (((GdkEventScroll*)pEvent)->direction==GDK_SCROLL_DOWN)
	{
		mouseWheel+=.1;
	}
	mouseWheel=CLAMP(mouseWheel,1.00,10.00);

	// Renseigne le calcul de point de vue
	viewControlArg.mouseWheel=mouseWheel;
}


// Click d'un bouton de la souris
G_MODULE_EXPORT void on_window1_button_press_event(GtkWindow* pWindow,GdkEvent *pEvent,gpointer pData)
{
	// *********** Déclarations locales *************

	// *********** Fin de déclarations **************

	switch (((GdkEventButton*)pEvent)->button)
	{
		case 1 :
			// Bouton de gauche
			viewControlArg.mouseLbutton=TRUE;
			break;
		case 2 :
			// Bouton du milieu 
			break;
		case 3 :
			// Bouton de droite
			break;
	}
}


// Relachement d'un bouton de la souris
G_MODULE_EXPORT void on_window1_button_release_event(GtkWindow* pWindow,GdkEvent *pEvent,gpointer pData)
{
	// *********** Déclarations locales *************

	// *********** Fin de déclarations **************

	viewControlArg.mouseLbutton=FALSE;
}


// Bouton croix
G_MODULE_EXPORT void on_cross_button_toggled(GtkButton* pButton,gpointer pData)
{
	// ********** Déclarations (locales) ************

	// *********** Fin de déclarations **************

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pCross_button)))
	{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pArrows_button),FALSE);
	}

	viewControlArg.CrossButton=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pCross_button));
	viewControlArg.ArrowsButton=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pArrows_button));
}


// Bouton flèches
G_MODULE_EXPORT void on_arrows_button_toggled(GtkButton* pButton,gpointer pData)
{
	// ********** Déclarations (locales) ************

	// *********** Fin de déclarations **************
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pArrows_button)))
	{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pCross_button),FALSE);
	viewControlArg.CrossButton=false;
	}

	viewControlArg.CrossButton=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pCross_button));
	viewControlArg.ArrowsButton=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pArrows_button));
}


// Bouton init
G_MODULE_EXPORT void on_zero_button_clicked(GtkButton* pButton,gpointer pData)
{
	// ********** Déclarations (locales) ************

	// *********** Fin de déclarations **************

	// Initialisation du point de vue
	viewControlArg.initView=true;

	// Initialisation du zoom
	mouseWheel=2;
	viewControlArg.mouseWheel=mouseWheel;

	// Initialisation du FOVy à la valeur standard
	gtk_range_set_value(GTK_RANGE(pFOVy_slider),45.0);
}


// Le curseur FOVy_slider bouge
G_MODULE_EXPORT void on_FOVy_slider_value_changed(GtkScale* pScale,gpointer pData)
{
	// *********** Déclarations locales *************

	// *********** Fin de déclarations **************
	
	// Récupère la valeur du FOVy_slider
	FOVy_sliderValue=gtk_range_get_value(GTK_RANGE(pFOVy_slider));
}


// Bouton Online OPC 
G_MODULE_EXPORT void on_opc_check_button_toggled(GtkButton* pButton,gpointer pData)
{
	// ********** Déclarations (locales) ************

	// *********** Fin de déclarations **************

	fOpcConnection();

}


// Le choix du robot change
G_MODULE_EXPORT void on_frame_choice_combobox_changed(GtkComboBox* pCombo,gpointer pData)
{
	// *********** Déclarations locales *************

	// *********** Fin de déclarations **************
	
	if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pFrame_choice_combobox)),"Serial 4-axis")==0)
		frame=0;
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pFrame_choice_combobox)),"Serial 6-axis")==0)
		frame=1;
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pFrame_choice_combobox)),"Parallel 3-axis")==0)
		frame=2;
	else if (strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pFrame_choice_combobox)),"Parallel 5-axis")==0)
		frame=3;
}


// Pop-up paramètres des robots
G_MODULE_EXPORT void on_robotsSettings_button_clicked(GtkButton* pButton,gpointer pData)
{
	// ********** Déclarations (locales) ************

	// *********** Fin de déclarations **************
	
	// Affiche (show) le widget et bloc l'exécution du programme dans l'attente d'une réponse
	gtk_dialog_run(GTK_DIALOG(pRobotsSettings_dialog));
	
	/*
	 * Dès que la fonction gtk_dialog_run sort de sa boucle récursive,
	 * cette fonction cache le widget mais ne le détruit pas
	 */
	gtk_widget_hide(pRobotsSettings_dialog);
}
