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
   @file shared/main.hpp
   @brief Définit une classe de main.
 */

#ifndef MAIN_DEFINIE

/**
   Empêche les inclusions récursives.
 */
#define MAIN_DEFINIE

#include <vector>
#include <iostream>
#include "carte.hpp"
#include "protocole.hpp"

/**
   @brief Une Main est un ensemble de cartes.

   Elle représente l'intégralité des cartes qu'un joueur détient.
   Certaines fonctionnalités du jeu de Tarot requièrent une décision
   sur une carte par rapport à d'autres. Par exemple, l'appel d'une
   Dame ne peut se faire que si le joueur possède déjà les quatre
   rois. Autre exemple, le joueur ne peut écarter d'atout que s'il ne
   possède pas d'autre carte écartable.
 */
class Main
{
public:

  /**
     Constructeur par défaut.
   */
  Main();
  
  /**
     Constructeur de copie.
     @param m : la main à copier.
   */
  Main(const Main & m);

  /** Constructeur à partir d'un Message.
      On peut remplir cette main d'un coup en analysant un
      Msg_distribution.
      @param m : le message à analyser.
   */
  Main(const Protocole::Msg_distribution & m);

  /**
     Enlève une carte de la main du joueur.
     Il y a deux utilités :
     1. À la fin des enchères, lorsqu'il faut écarter trois cartes.
     2. Pendant le jeu, lorsqu'une Carte a été jouée.

     @note Si la Carte ne fait pas partie de la Main du joueur, il ne
     se passe rien.

     @param carte : la carte à enlever.
     @see Main::ajouter(const Carte &)
     @see Main::possede(const Carte &) const
   */
  void enlever(const Carte & carte);

  /**
     Vérifie que le joueur possède cette carte.
     @param carte : la carte dont la présence est testée.
     @return vrai ssi le joueur la possède effectivement.
   */
  bool possede(const Carte & carte) const;

  /**
     Ajoute une Carte dans la Main du joueur.
     @param carte : la Carte à ajouter.
     @see Main::enlever(const Carte &)
   */
  void ajouter(const Carte & carte);

  /**
     Vérifie qu'un joueur peut appeler cette Carte.
     
     Les caractéristiques testées sont :
     1. Le joueur possède cette Carte.
     2. La Carte est une tête.
     3. Pour chacune des valeurs de cartes strictement supérieures, le
     joueur possède les cartes des 4 couleurs.

     @param carte : la Carte que le joueur voudrait écarter.
     @return vrai ssi le serveur peut accepter cet appel.
   */
  bool peut_appeler(const Carte & carte) const;

  /**
     Vérifie qu'un joueur peut écarter les cartes qu'il propose.

     Pour chaque carte, les caractéristiques testées sont :
     1. Le joueur possède cette Carte.
     2. La Carte est écartable.
     3. Si la Carte est un atout, aucune Carte de la Main du joueur
     n'est écartable après avoir décompté les autres Cartes de
     l'écart.
     4. De plus, on vérifie que les cartes sont différentes.

     @note Le fait que les cartes soient au nombre de 3 n'est pas 
     testé.
     @param ecart : les cartes que le joueur souhaite écarter.
     @return les réponses pour chaque Carte de l'écart.
     @see Carte::ecartable() const
   */
  std::vector<Carte::ModaliteEcart>
  peut_ecarter(const std::vector<Carte> & ecart) const;

  /**
     Vérifie qu'un joueur peut déclarer cette Poignée.

     Les caractéristiques testées sont :
     1. La taille est 8, 10 ou 13.
     2. Les Cartes sont toutes différentes.
     3. S'il y a l'excuse, il n'y a pas d'autre atout dans la Main du
     joueur.

     @param poignee : la déclaration à tester.
     @return vrai ssi on peut déclarer la poignée.
   */
  bool peut_declarer(const std::vector<Carte> & poignee) const;

  /**
     Affecte la Main.
     @param m : la Main à copier.
     @return une copie de la Main.
  */
  const Main & operator=(const Main & m);

  /**
     Affecte la main à partir d'un Msg_distribution.
     @param m : le message du serveur.
     @return une copie de la Main.
  */
  const Main & operator=(const Protocole::Msg_distribution & m);

  /**
     Crée un message de type distribution.

     À partir des Cartes qu'elle possède, la Main du côté du serveur
     peut remplir ce message pour l'envoyer au client.
     @param[out] m : le message à remplir.
   */
  void distribution(Protocole::Msg_distribution & m);

  /**
     @brief Indique si le joueur n'a plus d'une couleur.

     @note Même si le joueur n'a plus d'atout, ou s'il ne peut pas
     monter, cette méthode renvoie vrai. 

     @param col La couleur jouée.
     @return vrai ssi on peut couper.
   */
  bool peut_couper(Carte::Couleur col) const;

  /**
     @brief Indique si le joueur peut "pisser".
     
     @note Même si le joueur n'a plus d'atout, cette méthode renvoie
     vrai.

     @note Si la couleur est Atout, renvoie toujours vrai.

     @param atout_max La plus grosse valeur d'atout jouée,
     éventuellement 0.

     @return vrai ssi on peut pisser.
   */
  bool peut_pisser(Carte::Valeur atout_max) const;

  /**
     @brief Indique si le joueur peut "défausser".

     @warning Retourne vrai ssi on n'a pas d'atout. Il faut vérifier
     si on peut couper pour qu'on puisse accepter n'importe quelle
     carte. 
     
     @return vrai ssi on n'a pas d'atout.
   */
  bool peut_defausser() const;

  /**
     @brief Nombre de Cartes.
     
     @return Le nombre de cartes de la main du joueur.
   */
  unsigned int nombre_cartes() const;

  /**
     @brief Ensemble des Cartes.
     
     @return L'ensemble des Cartes disponibles.
   */
  std::vector<Carte> const & cartes() const;
private:
  std::vector<Carte> m_cartes;
};

/**
   @brief Écrit une Main.
   
   @param[out] out Le flux à utiliser.
   @param main La Main à écrire.
   @return La suite du flux.
 */
std::ostream & operator<<(std::ostream & out, const Main & main);

#endif
