// Evite que opc.h soit inclus plusieurs fois (à chaque #include "opc.h")
#ifndef OPC_H
#define OPC_H


/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#include <open62541/client.h>					// OPC UA
#include <open62541/client_config_default.h>	// OPC UA
#include <open62541/client_highlevel.h>			// OPC UA


/**********************************************
 **************** Définitions *****************
 **********************************************
*/
// Constantes de préprocesseur, macros, etc.

// Modèles de structures
typedef struct sSYSMAC_datas sSYSMAC_datas;
struct sSYSMAC_datas
{
	double joint1;
	double joint2;
	double joint3;
	double joint4;
	double joint5;
	double joint6;
};

// Modèles d'unions

// Modèles d'énumérations

/**********************************************
 ********* Prototypes de fonctions ************
 **********************************************
*/
gboolean fPeriodicRead(gpointer pData);
gboolean fOpcConnection(void);
sSYSMAC_datas fRead(void);

#endif
