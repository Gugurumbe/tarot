/*
  serveur of project tarot virtuel is a server for a card game.
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
   @file server/config.hpp
   @brief Définit les options de compilations du serveur.
*/

#ifndef SERVEUR_CONFIGURE

/**
   Empêche les inclusions récursives.
 */
#define SERVEUR_CONFIGURE

/**
   @brief Définit le mode de lancement.

   Il y a 2 modes de lancement possibles :
   1. Lancement en mode débogage. L'entité a des réactions à tous les
   types de Message, mais elles ne sont pas pertinentes pour le tarot.
   2. Lancement en mode jeu. L'entité ne tient pas toujours compte de
   tous les types de Message, mais elle est capable de comprendre un
   environnement de jeu.

   @see ServeurDebogage
   @see ServeurJeu
 */
#define SERVEUR ServeurJeu

/**
   @brief Définit le port de listage.
 */
#define PORT 45678

/**
   @brief Définit le comportement en cas de déconnexion.
   
   On n'a malheureusement pas d'autre choix que de détruire la Table
   toute entière si un client sort.
 */
#define DETRUIRE_INCOMPLETE 1

/**
   @brief Donne plus d'informations sur l'écriture dans les socket.
   
   Comme on n'utilise pas flush(), il y a des signaux qui sont envoyés
   lorsque la socket est prête à écrire. Il faut contrôler ça parfois.

   Prend la valeur 0 (non) ou 1 (oui).
 */
#define DEBUG_WRITESTACK 0

/**
   @brief Active le mélange des cartes.

   Lorsqu'on veut tester la possibilité d'écarter des atouts en cas de
   nécessité, on peut désactiver le mélange pour être sûr que le
   dernier client n'aura que des atouts.
 */
#define MELANGER //Mélange les cartes

#endif
