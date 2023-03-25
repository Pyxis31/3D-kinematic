############################
# Définition des variables #
############################

# Chemins d'installation :

# De la bibliothèque de MinGW (version 32 bits de MSYS2)
DIR_MGW = "C:/msys64/mingw32"

# De la bibliothèque standard du C (version 32 bits)
DIR_STD = "C:/msys64/mingw32/i686-w64-mingw32"

# De la bibliothèque de GTK+ 3 (version 32 bits)
DIR_GTK = "C:/msys64/mingw32"

# Des bibliothèques applicatives
DIR_3D_kinematic = "C:/Users/simfra.EU_OMRON_NET/Documents/Informatique/3Dkinematic"

# De l'exécutable
OUTPUT_DIR = "C:/Users/simfra.EU_OMRON_NET/Documents/Informatique/3Dkinematic exe/bin"

# Répertoires dans lesquels se trouvent les fichiers d'en-têtes (.h)
# -I : ajoute un répertoire de recherche de fichiers
INCLUDES = 	-I $(DIR_GTK)/include/gtk-3.0 \
			-I $(DIR_GTK)/include/pango-1.0 \
			-I $(DIR_GTK)/include \
			-I $(DIR_GTK)/include/glib-2.0 \
			-I $(DIR_GTK)/lib/glib-2.0/include \
			-I $(DIR_GTK)/include/harfbuzz \
			-I $(DIR_GTK)/include/freetype2 \
			-I $(DIR_GTK)/include/libpng16 \
			-I $(DIR_GTK)/include/fribidi \
			-I $(DIR_GTK)/include/cairo \
			-I $(DIR_GTK)/include/lzo \
			-I $(DIR_GTK)/include/pixman-1 \
			-I $(DIR_GTK)/include/gdk-pixbuf-2.0 \
			-I $(DIR_GTK)/include/atk-1.0 \
			-I $(DIR_MGW)/include \
			-I $(DIR_STD)/include \
			-I $(DIR_3D_kinematic)/include			
	
# Répertoires dans lesquels se trouvent les bibliothèques statiques (.lib ou .a) ou les bibliothèques d'importation (.lib)
# -L : ajoute un répertoire de recherche de fichiers
# -l : ajoute une bibliothèque (sans son préfixe "lib" s'il existe et sans son extension)
LIBS 	= 	-L $(DIR_GTK)/lib \
			-L $(DIR_MGW)/lib \
			-L $(DIR_STD)/lib \
			-L $(DIR_3D_kinematic)/lib \
			-lgtk-3 \
			-lgdk-3 \
			-lz \
			-lgdi32 \
			-limm32 \
			-lshell32 \
			-lole32 \
			-Wl,-luuid \
			-lwinmm \
			-ldwmapi \
			-lsetupapi \
			-lcfgmgr32 \
			-lhid \
			-lwinspool \
			-lcomctl32 \
			-lcomdlg32 \
			-lpangowin32-1.0 \
			-lpangocairo-1.0 \
			-lpango-1.0 \
			-lharfbuzz \
			-latk-1.0 \
			-lcairo-gobject \
			-lcairo \
			-lgdk_pixbuf-2.0 \
			-lgio-2.0 \
			-lgobject-2.0 \
			-lglib-2.0 \
			-lintl \
			-lws2_32 \
			-lfsjslib \
			-lepoxy \
			-lcglm \
			-lopen62541 \
			

########################################
# Définition des règles de compilation #
########################################

# Commande de suppression de fichier
RM = del

# Choix du mode DEBUG ou RELEASE
DEBUG=yes

ifeq ($(DEBUG),yes)
# Mode DEBUG (sans optimisation avec les symboles de débogage et le terminal de commande)
CompileFLAGS= -O0 -ggdb -Wall -mms-bitfields -pthread -mms-bitfields
LinkFLAGS= -Wall -mms-bitfields -pthread -mms-bitfields
else
# Mode RELEASE (avec optimisation sans les symboles de débogage sans assertion sans le terminal de commande)
CompileFLAGS= -O3 -s -DNDEBUG -mwindows -Wall -mms-bitfields -pthread -mms-bitfields
LinkFLAGS= -mwindows -Wall -mms-bitfields -pthread -mms-bitfields
endif

# Nom de l'exécutable
EXEC=3Dkinematic.exe

# Fichiers sources (*.c) wildcard permet d'utiliser le caractère étoile (joker)
SRC=$(wildcard *.c)

# Noms des fichiers objets (fichiers sources avec l'extension .c remplacée par .o)
OBJ=$(SRC:.c=.o)

########################################
#   Compilation --> Edition de lien    #
########################################

# II) Edition de lien : construction de l'exécutable à partir des fichiers objets (de l'application et des librairies)
$(EXEC): $(OBJ)
		gcc -std=c11 $(LinkFLAGS) $(OBJ) ressources.o $(LIBS) -o $(OUTPUT_DIR)/$(EXEC)

# I) Compilation : génération des fichiers objets ".o" à partir des fichiers sources ".c" :
$(OBJ): $(SRC)
		gcc -std=c11 -D _WIN32 $(CompileFLAGS) $(INCLUDES) -c $(SRC)

# Nettoyage
clean:
		$(RM) *.o *.exe *~
				
