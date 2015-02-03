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
   @file server/table.hpp
   @brief Définit une interface entre le Serveur et la Partie. 
 */

#ifndef TABLE_DEFINIE

/**
   Empêche les inclusions récursives.
 */
#define TABLE_DEFINIE

#include <QObject>

#include "protocole.hpp"
#include "partie_serveur.hpp"

#include <vector>

/**
   @brief Classe de Table.

   Une Table est une réunion de 5 joueurs qui s'envoient des Message. 
   
   La Table a la fonction suivante : à partir du numéro
   d'identification d'un client, elle en déduit le numéro de joueur
   utilisable par la classe Partie et vice versa.

   L'autre fonction est de relayer les Message entre la Partie et le
   Serveur. 
 */
class Table : public QObject
{
  Q_OBJECT;
public:
  /**
     @brief Constructeur par défaut.
   */
  Table(QObject * parent = 0);

  /**
     @brief Déconnecte tous les joueurs encore présents.
   */
  virtual ~Table();
signals:
  
  /**
     @brief Relaye le Message.

     Indique au ServeurJeu qu'il doit envoyer ce Message à ce numéro
     d'identification. 
     
     @param c : l'identification du destinataire.
     @param m : le Message à transmettre.

     @see Serveur::envoyer(unsigned int, Message)
   */
  void doit_emettre(unsigned int c, Protocole::Message m);

  /**
     @brief Relaye l'ordre de déconnexion.
     
     @param c : l'identification à déconnecter.

     @see Serveur::deconnecter(unsigned int)
   */
  void doit_deconnecter(unsigned int c);
  
  /**
     @brief Signale au ServeurJeu que la Table est pleine.
     
     @param t : la Table courante (this).

     @see ServeurJeu::detacher_table(Table *)
   */
  void complet(Table * t);

  /**
     @brief Signale au ServeurJeu que la Table doit être complétée. 

     @note Pour l'instant, les Table émettant ce signal seront
     détruites. 

     @param t : la Table courante (this).

     @see ServeurJeu::rattacher_table(Table *)
   */
  void incomplet(Table * t);			 
public slots:

  /**
     @brief Ajoute un joueur à la Table.

     Si la Table est complète, celui-ci est immédiatement déconnecté.

     Peut émettre complet(Table *) avec 'this' en argument.

     @param c : l'identification du nouveau client.
     @param nom Le nom du joueur.

     @see Serveur::connexion(unsigned int)
   */
  void ajouter(unsigned int c, std::string nom = "");

  /**
     @brief Transmet un Message à la Partie

     @warning Il faut d'abord vérifier que le client dont il est
     question est bien un des joueurs, car ce slot est appelé dans
     chaque Table pour chaque Message.

     @param c : l'identification du client bavard.

     @param m : le Message reçu.

     @see Serveur::message(unsigned int, Message)
   */
  void comprendre(unsigned int c, Protocole::Message m);

  /**
     @brief Enlève un joueur de la Table.

     Peut émettre le signal Table::incomplet()

     @warning Il faut d'abord vérifier que le client dont il est
     question est bien un des joueurs, car ce slot est appelé dans
     chaque Table pour chaque déconnexion.

     @param c : l'identification du client sortant.
     
     @see Serveur::deconnexion(unsigned int)
   */
  void enlever(unsigned int c);

  /**
     @brief Transmet un message.
     
     Émet le signal Table::doit_emettre(unsigned int,Message) en
     transformant le numéro du joueur en l'identification du client
     correspondant. 

     Fait analyser le Message par la Partie en cas de demande.
     
     @param j : le joueur concerné (0..4).
     @param m : le message à transmettre.
     @param analyser : vrai s'il faut faire analyser le message.
   */
  void doit_transmettre(unsigned int j, Protocole::Message m, bool analyser);

  /**
     @brief Commence une nouvelle partie.

     Permute les joueurs, réemet noms, numero, et recommence la partie.
   */
  void doit_recommencer();
  
private:
  
  /**
     @brief Indentifications.
   */
  std::vector<int> joueurs ;

  /**
     @brief Numéros des joueurs.

     ordre[i] est le numéro du joueur qui doit jouer en position
     i. Par exemple, ordre[0] est le numéro du premier joueur.
   */
  std::vector<unsigned int> ordre;
  
  /**
     @brief Noms des joueurs.
   */
  std::vector<std::string> noms;

  /**
     @brief Environnement de jeu.
   */
  PartieServeur partie;

  /**
     @brief Nombre de Tables
  */
  static int nombre_tables;
};

#endif
