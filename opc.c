/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#include <gtk/gtk.h>	// Gimp Tool Kit
#include <stdio.h>		// sprintf, FILE, fopen, fclose
#include <stdlib.h>		// atoi, malloc, calloc, sizeof, free
#include <epoxy/gl.h>	// OpenGL
//#include <winsock2.h>	// Sockets TCP (avec la librairie ws2_32.lib)
#include <fsjslib.h>	// Ma librairie
#include <functions.h>
#include <opc.h>

// Des Headers situés dans le répertoire de l'application

// Des définitions locales


/**********************************************
 ********** Déclarations (globales) ***********
 **********************************************
*/

// Externes
extern GtkWidget *pOpc_check_button,*pMessages_display,*pPlc_ip_address_entry;

// Types de base
gboolean opcConnectionSwitch;

// Pointeurs (sur tableaux, sur chaînes, etc.)
UA_Client* pPlc_OPCclient;
const char* pcharIPaddress;

// Pointeurs de widgets de l'interface

// Structures, unions, énumérations
sSYSMAC_datas plc_datas;


// Lecture périodique
gboolean fPeriodicRead(gpointer pData)
{
	// ********** Déclarations (locales) ************
	
	// *********** Fin de déclarations **************

	// Récupère l'état de la case à cocher
	if (opcConnectionSwitch)
	{
		plc_datas=fRead();
	}
	return TRUE; // FALSE : appel périodique arrêté
}


// Connexion OPC
gboolean fOpcConnection(void)
{
	// ********** Déclarations (locales) ************
	char pcharMessage[50+1]="";
	char pcharStrUAconnect[30+1];
	UA_StatusCode UAretval;
	// *********** Fin de déclarations **************

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pOpc_check_button)))
	{
		// Récupère le contenu alphanumérique des zones de saisies
		pcharIPaddress=gtk_entry_get_text(GTK_ENTRY(pPlc_ip_address_entry));

		// Vérifie l'adresse IP
		if (inet_addr(pcharIPaddress)==-1)
		{
			sprintf(pcharMessage,"The IP address is invalid\n");
		}
		else
		{
			// Initialise le pointeur de structure UA_Client
			pPlc_OPCclient=UA_Client_new();
			UA_ClientConfig_setDefault(UA_Client_getConfig(pPlc_OPCclient));
			
			// Connexion
			sprintf(pcharStrUAconnect,"opc.tcp://%s%s",pcharIPaddress,":4840");
			UAretval=UA_Client_connect(pPlc_OPCclient,pcharStrUAconnect);
			
			if(UAretval!=UA_STATUSCODE_GOOD)
			{
				// Le serveur ne répond pas
				sprintf(pcharMessage,"OPC UA server not responding. Status : %i\n", UAretval);
			}
			else
			{
				// Connecté
				opcConnectionSwitch=TRUE;
				sprintf(pcharMessage,"Connection to the OPC UA server successful !\n");
			}

		}			
	}
	else
	{
		// Déconnexion
		opcConnectionSwitch=FALSE;
		sprintf(pcharMessage,"Connection to the OPC UA server closed\n");
	}
	fMessage(pcharMessage,pMessages_display);
	
	return opcConnectionSwitch;
}


// Lecture des variables
sSYSMAC_datas fRead(void)
{
	// ********** Déclarations (locales) ************
	UA_Variant value;
	UA_StatusCode UAretval;
	sSYSMAC_datas datas;
	// *********** Fin de déclarations **************
	
    UA_Variant_init(&value);

    // Joint1
    UAretval=UA_Client_readValueAttribute(pPlc_OPCclient,UA_NODEID_STRING(4,"joint1"),&value);
    if(UAretval==UA_STATUSCODE_GOOD&&UA_Variant_hasScalarType(&value,&UA_TYPES[UA_TYPES_DOUBLE]))
    {
		datas.joint1=*(UA_Double*)value.data;
    }

    // Joint2
    UAretval=UA_Client_readValueAttribute(pPlc_OPCclient,UA_NODEID_STRING(4,"joint2"),&value);
    if(UAretval==UA_STATUSCODE_GOOD&&UA_Variant_hasScalarType(&value,&UA_TYPES[UA_TYPES_DOUBLE]))
    {
		datas.joint2=*(UA_Double*)value.data;
    }

    // Joint3
    UAretval=UA_Client_readValueAttribute(pPlc_OPCclient,UA_NODEID_STRING(4,"joint3"),&value);
    if(UAretval==UA_STATUSCODE_GOOD&&UA_Variant_hasScalarType(&value,&UA_TYPES[UA_TYPES_DOUBLE]))
    {
		datas.joint3=*(UA_Double*)value.data;
    }

    // Joint4
    UAretval=UA_Client_readValueAttribute(pPlc_OPCclient,UA_NODEID_STRING(4,"joint4"),&value);
    if(UAretval==UA_STATUSCODE_GOOD&&UA_Variant_hasScalarType(&value,&UA_TYPES[UA_TYPES_DOUBLE]))
    {
		datas.joint4=*(UA_Double*)value.data;
    }

    // Joint5
    UAretval=UA_Client_readValueAttribute(pPlc_OPCclient,UA_NODEID_STRING(4,"joint5"),&value);
    if(UAretval==UA_STATUSCODE_GOOD&&UA_Variant_hasScalarType(&value,&UA_TYPES[UA_TYPES_DOUBLE]))
    {
		datas.joint5=*(UA_Double*)value.data;
    }

    // Joint6
    UAretval=UA_Client_readValueAttribute(pPlc_OPCclient,UA_NODEID_STRING(4,"joint6"),&value);
    if(UAretval==UA_STATUSCODE_GOOD&&UA_Variant_hasScalarType(&value,&UA_TYPES[UA_TYPES_DOUBLE]))
    {
		datas.joint6=*(UA_Double*)value.data;
    }
    
    return datas;
}

