# Awali
Multiplayer Awali implementation in C

# Organisation des fichiers
Les fichiers sont séparé en sources (.c) et en includes (.h)
Le jeu contient 2 applications : 
    - server : le serveur de jeu de awale
    - client : l'application d'awale en ligne qui va se connecter au server

Il y a également un répertoire shared/ qui contient le code pour la gestion du protocole que nous avons créé.
fatpp = Fatih & Alexandre Transfer Private Protocol

# Compilation
Se placer dans le répartoire Awali
Lancer la commande make pour compiler les 2 applications

Plus d'options du make :
    - make : créé les executables pour les 2 applications
    - make clean : supprime tous les executables
    - make server : contruit uniquement l'application server
    - make client : contruit uniquement l'application client
    - make debug : contruit toutes les applications avec les options de debuggages

Tous les executables sont contruit dans le dossier output/

# Execution
Pour lancer le jeu en multijouer il faut d'abord lancer le serveur de jeu : ./output/server
Chaque joueur doit ensuite lancer l'application client en mettant en paramètre l'adresse IP du serveur et son nom. Par exemple : ./output/client 192.168.1.150 Alex

# Utilisation de l'application
Pour utiliser le jeu il faut ensuite suivre les indications à l'écran.
Toutes les actions disponibles sont numérotés.
Il suffit de rentrer le numéro de l'action et taper sur entrée.

Le plateau affiche toujours le coté du joueur sur la ligne du bas.
Les graines tournent dans le sens des aiguilles d'une montre.

Pour choisir la case, il faut donner la lettre correspondant à la case.

# git repo
https://github.com/swordfatih/Awali.git