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
   @file shared/enchere.hpp
   @brief Définit une classe d'enchère.
 */

#ifndef ENCHERE_DEFINIE

/**
   Empêche les inclusions récursives
 */
#define ENCHERE_DEFINIE

/**
   Une enchère utilise des Carte, et peut se construire à partir d'un
   Message.
 */
#include "carte.hpp"
#include "protocole.hpp"

/**
   @brief Définit une classe pour les enchères.

   Les enchères sont importantes au tarot. Elles interviennent entre
   le moment de la distribution des cartes et le jeu. Leur
   particularité est de se construire en 2 temps :
   1. Chaque joueur énonce une Prise.
   2. Le joueur ayant annoncé la plus forte Prise complète son contrat
   en appelant une Carte. Le joueur possédant cette Carte sera dans le
   même camp que celui-là.
 */
class Enchere 
{
public:

  /**
     Énumération des Prises possibles.
     Lors du moment des enchères, si un joueur ne passe pas, il
     effectue une des 4 Prises prévues par les règles du tarot.
   */
  enum Prise
    {
      /** 
	  Si un joueur le souhaite, il peut passer.
       */
      PASSE = 0,
      
      /**
	 La simple Prise met en jeu peu de points.
      */
      PRISE,
      
      /**
	 La Garde met en jeu plus de points.
       */
      GARDE,

      /**
	 La Garde Sans le Chien est une Garde avec interdiction de
	 consulter le Chien.
       */
      GARDE_SANS,

      /**
	 La Garde Contre le Chien est une Garde avec don des trois
	 cartes du Chien à la défense.
       */
      GARDE_CONTRE
    };

  /**
     Constructeur par défaut.
     Le joueur qui effectue cette Prise est inconnu, ainsi que sa
     Prise. Il n'a pas encore appelé de Carte.
   */
  Enchere();

  /**
     Constructeur de copie.
     La Carte appelée est copiée.
     @param e : une enchère à copier.
   */
  Enchere(const Enchere & e);

  /**
     Constructeur à partir d'un Msg_prise.
     Du côté du serveur, on peut construire une Enchère (et en os) à
     partir du message de prise du client, si on l'a accepté bien
     sûr. La Carte appelée n'est pas définie.
     @param joueur : le numéro de joueur ayant fait cette requête.
     @param m : le message qu'il a envoyé.
     @see Enchere::Enchere(unsigned int, const Msg_contrat &)
     @see Enchere::Enchere(const Msg_contrat_final &)
     @see Msg_prise
   */
  Enchere(unsigned int joueur, const Protocole::Msg_prise & m);

  /**
     Constructeur à partir d'un Msg_contrat.
     Du côté du client, on peut construire une Enchère intermédiaire à
     partir des informations fournies par le serveur. La Carte appelée
     n'est pas définie, mais le joueur ayant effectué cette Enchère
     est celui dont c'était le tour.
     @param tour : le numéro du joueur dont c'était le tour.
     @param c : le contrat effectué
     @see Enchere::Enchere(unsigned int, const Msg_prise &)
     @see Enchere::Enchere(const Msg_contrat_final &)
     @see Msg_contrat
   */
  Enchere(unsigned int tour, const Protocole::Msg_contrat & c);

  /**
     Constructeur à partir d'un Msg_contrat_final.
     Du côté du client comme du côté serveur, le contrat définitif
     permet d'obtenir toutes les informations d'un coup !
     @param m : le message du serveur.
     @see Enchere::Enchere(unsigned int, const Msg_prise &)
     @see Enchere::Enchere(unsigned int, const Msg_contrat &)
     @see Msg_contrat_final
   */
  Enchere(const Protocole::Msg_contrat_final & m);

  /**
     Destructeur.
     Il est nécessaire de libérer la Carte appelée (dans le
     fonctionnement interne de la classe).
   */
  virtual ~Enchere();

  /**
     Donne la Carte appelée.
     Dans le cas où la carte n'est pas définie, retourne 0.
     @return un pointeur vers cette Carte ou 0.
     @see Enchere::appeler(const Carte &)
   */
  const Carte * carte_appelee() const;

  /**
     Modifie la carte appelée.
     Dans le cadre de cette enchère, il peut arriver qu'on ne
     connaisse pas encore la carte appelée.
     @param carte : la nouvelle carte appelée.
     @see Enchere::carte_appelee() const
   */
  void appeler(const Carte & carte);

  /**
     Retourne la Prise effectuée...
     ... dans le cadre de cette enchère.
     Grâce au fait que PASSE aie la valeur 0, on peut tester
     la validité d'une enchère en
     ~~~~~~~~~~~~~~~~{.cpp}
     (!requete.prise()) || (requete > derniere_enchere)
     ~~~~~~~~~~~~~~~~
     @return la prise effectuée.
     @see Enchere::operator>(const Enchere &)
   */
  Prise prise() const;

  /**
     Retourne le numéro de joueur.
     C'est celui-ci qui a effectué l'enchère.
     @return le numéro du joueur.
   */
  unsigned int joueur() const;

  /**
     Modifie en place l'Enchère.
     Utile pour stocker des tableaux d'enchères.
     @param e : l'enchère à copier.
     @return une copie de l'Enchère.
     @see Enchere::Enchere(const Enchere &)
     @see Enchere::operator=(const Msg_contrat_final &)
   */
  const Enchere & operator=(const Enchere & e);

  /**
     Modifie en place l'Enchère.
     Utile pour mettre à jour l'Enchère de l'attaquant lors de la
     réception (du côté du client) ou de l'émission (du côté du
     serveur) d'un Message de type contrat_final.

     @note Il n'existe pas d'équivalent pour construire une Enchère
     sans Carte appelée (i.e. à partir d'un Msg_prise ou d'un
     Msg_contrat). On peut contourner ce problème grâce à
     e = Enchere(joueur, message);

     @param m : le message envoyé par le serveur.
     @return une copie de l'Enchère.
     @see Enchere::Enchere(const Msg_contrat_final &)
     @see Enchere::operator=(const Enchere &)
   */
  const Enchere & operator=(const Protocole::Msg_contrat_final & m);

  /**
     Teste la supériorité d'une enchère.
     Lors de la phase d'enchères, les joueurs doivent passer ou
     effectuer des Prises supérieures à la plus ambitieuse. Il est
     nécessaire de tester cette règle facilement.
     @param e : Enchère de référence.
     @return vrai ssi cette enchère est valable.
     @see Enchere::operator>(Prise p) const
   */
  bool operator>(const Enchere & e) const;

  /**
     Teste la supériorité d'une enchère.
     Dans le cas où l'on ne dispose que de la valeur de la Prise, il
     peut être intéressant de tester la validité de cette enchère par
     rapport à elle.
     @param p : la Prise de référence.
     @return vrai ssi cette enchère est valable.
     @see Enchere::operator>(const Enchere &) const
  */
  bool operator>(Prise p) const;

  /**
     @brief Présente l'enchère.

     @param[out] out Le flux avant l'écriture.
     
     @return Le flux après écriture.
   */
  std::ostream & presenter(std::ostream & out) const;
private:
  unsigned int m_joueur; //vaut 5 par défaut
  Prise m_prise; //vaut PASSE par défaut
  Carte * m_carte_appelee; //Vaut 0 tant qu'on ne le sait pas
};

/**
   @brief Écrit une Enchère.
   
   @param[out] out Le flux à utiliser.
   @param e L'enchère à écrire.
   @return La suite du flux.
 */
std::ostream & operator<<(std::ostream & out, const Enchere & e);

#endif
