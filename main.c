/**********************************************
 *               Visionneuse 3D               *
 *         pour modèles cinématiques          *
 *		    	  F.SIMON 2019	    		  *
 **********************************************
*/
    
    
/**********************************************
 ********** Directives d'inclusions ***********
 **********************************************
*/
    
// * Des Headers système *
#include <stdlib.h>
#include <gtk/gtk.h>
       
// * Des Headers d'application *
    

    
/**********************************************
 *********** Déclarations globales ************
 **********************************************
*/
    
//	Pointeur de type "classe créateur d'interface GTK" --> l'interface est décrite dans un fichier XML produit par GLADE
GtkBuilder* pConstInterface;


/**********************************************
 ********* Fonctions sans prototypes **********
 **********************************************
*/

static void fStartApplication(GtkApplication* MonApplication,gpointer pData)
{
	// *********** Déclarations locales *************
	GtkWidget* pMaFenetre = NULL; // Type "classe de base GTK"
	// *********** Fin de déclarations **************


	/* 	Le "new" de cette fonction est un constructeur (comme en C++),
	 * 	il crée un objet vide de type builder (pConstInterface) et fournit son adresse
	 * 	Cet objet analyse (parse) le fichier XML afin de construire et de charger tous les widgets constituant l'interface
	 */	
	pConstInterface=gtk_builder_new_from_file("3D kinematic.glade");


	/* 	Cette fonction récupère dans l'objet builder la référence (le pointeur) du widget de la fenêtre principale "window1" (voir glade)
	 *	La macro GTK_WIDGET transtype (cast) le GObject renvoyé par la fonction vers un type GtkWidget compatible avec pMaFenetre
	 */ 
	pMaFenetre=GTK_WIDGET(gtk_builder_get_object(pConstInterface,"window1"));


	/*	Cette fonction définit les propriétés de l'application (MonApplication),
	 *	notamment le widget de la fenêtre principal qu'elle doit prendre en charge
	 */
	g_object_set(pMaFenetre,"application",MonApplication,NULL);


	/* 	Cette fonction recherche dans la table de symboles d'application (voir la section callback des fichiers sources)
	 *  une fonction callback portant le nom de l'événement (signal) venant de se produire
	 * 	Ce nom est défini dans GLADE, précisément dans l'onglet "signaux" des propriétés de l'objet (bouton, etc.)
	 */     
	gtk_builder_connect_signals(pConstInterface,NULL);
	

	// 	Cette fonction affiche le contenu de pMaFenetre
	gtk_widget_show_all(pMaFenetre);
}

       
/**********************************************
 ******************* Main *********************
 **********************************************
*
* Fonction principale "main", appelée au lancement de l'exécutable
* argc = quantité de chaînes de caractères non nulles dans le tableau argv
* argv = tableau de chaînes de caractères transmises par le système
* La première chaîne contient le chemin d'accès de l'exécutable
* Les suivantes contiennent les éventuels arguments passés en mode console
*/

int main(int argc, char *argv[]) 
{
	// *********** Déclarations locales *************
    GtkApplication* MonApplication;
    int Statut=0;
    // *********** Fin de déclarations **************

    
	// Création d'une nouvelle instance "MonApplication"
    MonApplication=gtk_application_new(NULL,G_APPLICATION_FLAGS_NONE);
    
    // Attente du signal "activate" pour l'appel de la fonction "fStartApplication"
    // Il existe aussi les signaux "startup", "shutdown" et "open" (par ex. pour sauvegarder des données à la fermeture de la session)
	g_signal_connect(MonApplication,"activate",G_CALLBACK(fStartApplication),NULL);

	// Exécution de l'application avec les arguments argc et argv (à vérifier sous win32...)
	Statut=g_application_run(G_APPLICATION(MonApplication),argc,argv);

	// On libère la mémoire
	g_object_unref(MonApplication);

	// Réponse de la fonction "main"
	return Statut;
	
}
