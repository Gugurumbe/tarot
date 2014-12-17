/**
   @mainpage PE 46 : tarot virtuel
   @section intro_sec Introduction
   
   Dans le cadre du projet d'étude n° 46 de l'École Centrale de Lyon
   (2014), notre équipe travaille sur une implémentation du jeu de
   tarot afin d'élaborer une intelligence artificielle. Nous avons
   choisi Qt et C++ pour la partie programmation. La documentation est
   créée avec Doxygen. Par souci de compréhension, et en nous 
   excusant auprès des anglicistes, nous utiliserons uniquement la 
   langue de Molière.

   @section install_sec Installation
   @subsection etape1 Étape 1 : obtenir les sources
   
   Les sources sont disponibles sur
   [GitHub](https://github.com/Gugurumbe/tarot). Une archive
   [.zip](https://github.com/Gugurumbe/tarot/archive/master.zip) 
   ou une archive 
   [.tar.gz](https://github.com/Gugurumbe/tarot/archive/master.tar.gz)
   est également disponible. Pour le moment, nous n'avons pas réglé
   le problème de la license. Profitez-en !
   Une fois l'archive décompressée, on peut compiler le tout.
   
   @subsection etape2 Étape 2 : Obtenez les outils

   Il faut Qt. Les versions testées par notre équipe sont 4.8 et
   5.3. Pensez à prendre les paquets de développement.
   Pour gcc, les versions 4.6 (Debian wheezy) et 4.8 (Windows) sont testées.
   Il vous faut également Doxygen si vous voulez compiler la
   documentation (avec tout le mal que nous nous sommes donné pour l'écrire,
   vous pourriez bien le faire, tout de même !)

   Qt : http://www.qt.io/download-open-source/
   MinGW sans Qt : http://www.mingw.org/wiki/Getting_Started
   Doxygen : http://www.stack.nl/~dimitri/doxygen/download.html 

   @subsection etape3 Étape 3 : Compilez les programmes
   
   Il y a deux programmes : un dans le dossier client et un autre dans
   le dossier server. Pour les deux programmes, c'est la même
   méthode. Il faut tout d'abord utiliser qmake pour générer le
   Makefile, puis lancer make. Alternativement, on peut ouvrir les
   fichiers server/serveur.pro et client/client.pro avec QtCreator.

   Compilation bash : 
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   cd chemin/vers/tarot
   doxygen doc.conf              #Compile la doc
   cd server                     #Répertoire du serveur
   qmake                         #Prépare le Makefile
   make                          #Compile le serveur
   cd ../client                  #Répertoire du client
   qmake                         #Prépare le Makefile
   make                          #Compile le client
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   @subsection etape4 Étape 4 : Compilez la documentation
   
   Après avoir installé doxygen, lancez-le sur le fichier doc.conf. La
   documentation est disponible dans le dossier doc.
   
   @subsection etape5 Étape 5 : Utilisez

   Pour l'instant, ce n'est pas jouable. Il faut lancer un serveur et
   lancer des clients.

   Quand ce sera jouable, il n'y aura pas besoin de lancer le
   serveur. 
*/

/**
   @file shared/config.hpp
   @brief Définit les options de compilation.
*/
