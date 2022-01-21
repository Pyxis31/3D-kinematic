// Evite que callback.h soit inclus plusieurs fois (à chaque #include "callback.h")
#ifndef CALLBACK_H
#define CALLBACK_H


/**********************************************
 **************** Définitions *****************
 **********************************************
*/
// Constantes de préprocesseur, macros, etc.


// Modèles de structures
// Structure de pointeurs pour les paramètres de certaines fonctions
typedef struct sParamFnc_np sParamFnc_np; // "sParamFnc_np" alias de "struct sParamFnc_np"
struct sParamFnc_np
{
	gpointer pData1;
	gpointer pData2;
	gpointer pData3;
};

// Modèles d'unions


// Modèles d'énumérations
typedef enum eAxis eAxis;
enum eAxis
{
	X_Axis, Y_Axis, Z_Axis
};


/**********************************************
 ********* Prototypes de fonctions ************
 **********************************************
*/
// Fonctions callback
G_MODULE_EXPORT void on_window1_map_event(GtkWindow* pWindow,gpointer pData);
G_MODULE_EXPORT void on_window1_destroy_event(GtkWindow* pWindow,gpointer pData);
G_MODULE_EXPORT void on_about_button_clicked(GtkButton* pButton,gpointer pData);
G_MODULE_EXPORT gboolean on_3Dview_render(GtkGLArea* pGLarea,GdkGLContext* pContext);
G_MODULE_EXPORT void on_3Dview_resize(GtkGLArea* pGLarea);
G_MODULE_EXPORT void on_rotate_slider_joints_value_changed(GtkScale* pScale,gpointer pData);
G_MODULE_EXPORT void on_translate_slider_quill_value_changed(GtkScale* pScale,gpointer pData);
G_MODULE_EXPORT void on_window1_motion_notify_event(GtkWindow* pWindow,GdkEvent *pEvent,gpointer pData);
G_MODULE_EXPORT void on_window1_scroll_event(GtkWindow* pWindow,GdkEvent *pEvent,gpointer pData);
G_MODULE_EXPORT void on_window1_button_press_event(GtkWindow* pWindow,GdkEvent *pEvent,gpointer pData);
G_MODULE_EXPORT void on_window1_button_release_event(GtkWindow* pWindow,GdkEvent *pEvent,gpointer pData);
G_MODULE_EXPORT void on_cross_button_toggled(GtkButton* pButton,gpointer pData);
G_MODULE_EXPORT void on_arrows_button_toggled(GtkButton* pButton,gpointer pData);
G_MODULE_EXPORT void on_zero_button_clicked(GtkButton* pButton,gpointer pData);
G_MODULE_EXPORT void on_FOVy_slider_value_changed(GtkScale* pScale,gpointer pData);
G_MODULE_EXPORT void on_joint_choice_combobox_changed(GtkComboBox* pCombo,gpointer pData);
G_MODULE_EXPORT void on_opc_check_button_toggled(GtkButton* pButton,gpointer pData);
G_MODULE_EXPORT void on_frame_choice_combobox_changed(GtkComboBox* pCombo,gpointer pData);

#endif
