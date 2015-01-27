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
   @file server/serveur_jeu.hpp
   @brief Définit un Serveur qui joue au tarot.
 */

#ifndef SERVEUR_JEU_DEFINI

/**
   Empêche les inclusions récursives.
 */
#define SERVEUR_JEU_DEFINI

#include "serveur.hpp"
#include "table.hpp"
#include <vector>

/**
   @brief Classe de Serveur de jeu.
   
   Utilise des Table de 5 joueurs pour répartir tous les clients.
 */
class ServeurJeu : public Serveur
{
  Q_OBJECT;
public:

  /**
     @brief Constructeur par défaut.

     Ce constructeur lance le listage sur un port qu'il affiche dans
     la sortie standard.

     @see Serveur::ouvrir_global()
   */
  ServeurJeu(QObject * parent = 0);
public slots :

  /**
     @brief Réaction personnalisée (connexion).
     
     Ajoute un type sur le paillasson.
     
     @param c : le numéro d'identification du client.
   */
  void reagir_connexion(unsigned int c);

  /**
     @brief Ne cherche plus à compléter la Table.

     Utilisé quand une Table contient 5 joueurs.
     
     @param t : la Table pleine.

     @note Inutilisé depuis qu'on fait des tables authentifiées.
   */
  void detacher_table(Table * t);

  /**
     @brief Cherche à remplir cette Table.
     
     Utilisé quand une Table pleine perd un joueur.

     @note Dans un premier temps, dans l'implémentation, la Table
     s'autodétruira si un client se déconnecte.

     @param t : la Table incomplète.

     @note Inutilisé depuis qu'on fait des tables authentifiées.
   */
  void rattacher_table(Table * t);

  /**
     @brief Oublie une Table détruite.

     @param t : la Table sur le point d'être détruite.

     @note Inutilisé depuis qu'on fait des tables authentifiées.
   */
  void oublier_table(QObject * t);

  /**
     @brief Une socket s'est déconnectée.

     @param client Le client qui s'est déconnecté.

     @note Il n'est pas forcément dans le vestibule, ni sur le
     paillasson. 
   */
  void oublier_socket(unsigned int client);

  /**
     @brief Identifie.

     @param client Le client sur le paillasson qui veut s'identifier.
     @param m Un message, probablement d'identification.

     émet "refuse", "erreur_protocole" ou fait les présentations avec
     le reste du vestibule.
   */
  void lire(unsigned int client, Protocole::Message m);
private:
  
  std::vector<unsigned int> paillasson; // Sockets non encore
					// identifiées 
  std::vector<unsigned int> vestibule; //Sockets identifiées mais pas
				       //en jeu.
  std::vector<std::string> noms; //Noms correspondant au vestibule.
};

#endif
