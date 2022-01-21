// Evite que udp.h soit inclus plusieurs fois (à chaque #include "udp.h")
#ifndef UDP_H
#define UDP_H


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

// Modèles de structures

// Modèles d'unions

// Modèles d'énumérations


/**********************************************
 ********* Prototypes de fonctions ************
 **********************************************
*/
void udpInit(GSocket* pSock);
gboolean dataUDP_coming(GIOChannel* pSource,GIOCondition condition,gpointer pData);
void udpDataReceipt(GIOChannel* pSource,float* pIMUcoordinates,GtkWidget *pCompass_value,GtkWidget *pPitch_value,GtkWidget *pRoll_value);

#endif
