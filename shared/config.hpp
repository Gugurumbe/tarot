/*
  Project tarot virtuel is a card game.
  Copyright (C) 2015  Vivien Kraus
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


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
   est également disponible. Le code est sous license GPL.
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
   cd ../client/moche            #Répertoire du client moche
   qmake                         #Prépare le Makefile
   make                          #Compile le client
   cd ../bete                    #Répertoire de l'IA bête
   qmake                         
   make                          #Compilation de l'IA bête
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   Pour lancer les différents programmes : le client moche doit
   recevoir 2 paramètres supplémentaires, l'adresse et le port du
   serveur. L'IA bête, elle, doit en plus recevoir le nom qu'elle doit
   emprunter et les noms des 4 autres personnes dont elle acceptera
   l'invitation. 

   @subsection etape4 Étape 4 : Compilez la documentation
   
   Après avoir installé doxygen, lancez-le sur le fichier doc.conf. La
   documentation est disponible dans le dossier doc.
   
   @subsection etape5 Étape 5 : Utilisez

   Pour l'instant, ce n'est pas jouable. Il faut lancer un serveur et
   lancer des clients.

   Quand ce sera jouable, il n'y aura pas besoin de lancer le
   serveur. 

   En revanche, quelques tests seraient très précieux, notamment en ce
   qui concerne les cas inattendus voire pathologiques : l'IA est
   invitée par de mauvaises personnes -vérifier qu'elle se déconnecte,
   se reconnecte et attend les bonnes invitations-, tester plusieurs
   tables à la fois -penser à désactiver certaines options de débogage
   pour éviter de faire exploser la machine-, et surtout, tester le
   comportement après la fin de la première manche.
*/

/**
   @file shared/config.hpp
   @brief Définit les options de compilation et la page d'accueil de
   la documentation.
*/
