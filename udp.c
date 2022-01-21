/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#include <gtk/gtk.h>	// Gimp Tool Kit
#include <stdio.h>		// sprintf, FILE, fopen, fclose
#include <stdlib.h>		// atoi, malloc, calloc, sizeof, free

// Des Headers situés dans le répertoire de l'application
#include "udp.h"

// Des définitions locales


/**********************************************
 ********** Déclarations (globales) ***********
 **********************************************
*/

// Externes

// Types de base

// Pointeurs (sur tableaux, sur chaînes, etc.)

// Pointeurs de widgets de l'interface


// Fonction d'initialisation du socket UDP
void udpInit(GSocket* pSock)
{
	// ********** Déclarations (locales) ************
	GError* pError=NULL;
	GSocketAddress* pSockAdd;
	GInetAddress* pInetAdd;
	GIOChannel* pChannel;
	int WinsockHandle;
	gpointer pDataUDP=NULL;
	// *********** Fin de déclarations **************

	// Spécifie l'adresse IP et le numéro de port écoutés
	//pSockAdd=g_inet_socket_address_new_from_string("192.168.1.10",2055);

	// Spécifie le port écouté en IPv4 quelque soit l'adresse IP
	pInetAdd=g_inet_address_new_any(G_SOCKET_FAMILY_IPV4);
	pSockAdd=g_inet_socket_address_new(pInetAdd,2055);

	// Socket UDP/IP IPv4
	pSock=g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_DATAGRAM,G_SOCKET_PROTOCOL_UDP,&pError);
	if (pError)
	{
		printf("%s\n",pError->message);
	}

	// Bind du socket à l'adresse spécifiée
	//g_socket_bind(pSock,pSockAdd,TRUE,&pError);
	g_socket_bind(pSock,G_SOCKET_ADDRESS(pSockAdd),TRUE,&pError);
	if (pError)
	{
		printf("%s\n",pError->message);
	}

	// Evénement de réception de données
	WinsockHandle=g_socket_get_fd(pSock);
	pChannel=g_io_channel_win32_new_socket(WinsockHandle);
	g_io_add_watch(pChannel,G_IO_IN,(GIOFunc)dataUDP_coming,pDataUDP); // dataUDP_coming : fonction callback de réception de données
	g_io_channel_unref(pChannel);
}


// Fonction de réception, de traitement et d'affichage des données UDP
void udpDataReceipt(GIOChannel* pSource,float* pIMUcoordinates,GtkWidget *pCompass_value,GtkWidget *pPitch_value,GtkWidget *pRoll_value)
{
	// ********** Déclarations (locales) ************
	GError* pError=NULL;
	gchar charRead;
	int bufferSize=1; // 1 octet -> lecture caratère par caractère
	gsize bytes_read;
	static char completeMessage[49+1]="";
	static int  loop=0;
	char pFormatted_compass_v[19+1];
	char pFormatted_pitch_v[19+1];
	char pFormatted_roll_v[19+1];
	// *********** Fin de déclarations **************

	// Séparation des champs de données (token)
	void split()
	{
		// ********** Déclarations (locales) ************
		char* token;
		int delim=0;
		// *********** Fin de déclarations **************

		// Première donnée
		token=strtok(completeMessage,",");
		pIMUcoordinates[delim]=atof(token);
		//printf("Coordonnee %i : %f\n",delim,pIMUcoordinates[delim]);

		// Données suivantes
		while(token!=NULL)
		{
			delim++;
			token=strtok(NULL,",");
			if (delim<3)
			{
				pIMUcoordinates[delim]=atof(token);
				//printf("Coordonnee %i : %f\n",delim,pIMUcoordinates[delim]);
			}
		}
	}

	// Lecture tant que le buffer n'est pas vide
	do
	{
		// Lecture du caractère dans le buffer
		g_io_channel_read_chars(pSource,&charRead,bufferSize,&bytes_read,&pError);
		if (pError)
		{
			printf("%s\n",pError->message);
		}
		
		// Stockage du caractère (constitution du message complet)
		if (bytes_read>0)
		{
			completeMessage[loop++]=charRead;
		}

		// Le caractère est égal à <LF> -> fin du message
		if (charRead=='\n')
		{
			completeMessage[loop-2]='\0';	// Suppression des <CR><LF>
			split();						// Séparation des données
			bytes_read=0;
			loop=0; 
			completeMessage[0]='\0';
		}
	}
	while (bytes_read>0);

	// Affichage
	sprintf(pFormatted_compass_v,"(raw) %-.3f%s",pIMUcoordinates[0]," °");
	gtk_label_set_text(GTK_LABEL(pCompass_value),pFormatted_compass_v);
	sprintf(pFormatted_pitch_v,"(raw) %-.3f%s",pIMUcoordinates[1]," °");
	gtk_label_set_text(GTK_LABEL(pPitch_value),pFormatted_pitch_v);
	sprintf(pFormatted_roll_v,"(raw) %-.3f%s",pIMUcoordinates[2]," °");
	gtk_label_set_text(GTK_LABEL(pRoll_value),pFormatted_roll_v);
}
