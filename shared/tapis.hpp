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
   @file shared/tapis.hpp
   @brief Définit l'état d'un tour de jeu.
 */

#ifndef TAPIS_DEFINI

/**
   Empêche les inclusions récursives.
 */
#define TAPIS_DEFINI

/**
   Il y a une relation de composition entre un Tapis et des Carte.

   Pour utiliser directement un Message, il est nécessaire de lier le
   protocole.
   @see Carte
   @see Message
 */
#include "carte.hpp"
#include "protocole.hpp"
#include <vector>

/**
   @brief Représente les cartes face visible sur la table.

   La classe Tapis permet de connaître l'état du tour de jeu,
   c'est-à-dire les cartes jouées ce tour-ci et les numéros des
   joueurs (0 à 4) qui jouent ces cartes.

   La méthode Tapis::terminer(unsigned int, unsigned int, bool)
   retourne la liste des cartes gagnées par l' « attaque »,
   c'est-à-dire le joueur qui a pris et le joueur appelé, lorsque le
   pli est Tapis::complet() const.
   
   @see Tapis::terminer(unsigned int, unsigned int, bool)
   @see Tapis::complet() const
 */
class Tapis
{
public:
  
  /**
     Constructeur principal.
  */
  Tapis();

  /**
     @brief constructeur de copie.
     @param tap Le tapis à copier.
   */
  Tapis(const Tapis & tap);

  /**
     @brief Réinitialise le tapis.
   */
  void reinitialiser();
  
  /**
     @brief Destructeur virtuel.
   */
  virtual ~Tapis();

  /**
     Ajoute une carte.
     L'ajout d'une carte au tapis se fait directement à partir d'un
     sous-type de Message.
     @param carte Le message provenant du serveur.
     @param exc La force de l'excuse.
     @see Message
     @see Tapis::set_ouverture(unsigned int)
  */
  void ajouter(const Protocole::Msg_carte & carte, 
	       Carte::ForceExcuse exc = Carte::EXCUSE_IMPRENABLE);

  /**
     @brief Retourne le plus gros atout joué.

     @param[out] c La Carte qui contiendra le plus gros atout.
     @return vrai ssi la carte a été remplie.
   */
  bool plus_gros_atout(Carte & c) const;

  /**
     @brief Retourne la carte d'entame.

     @note Si la Carte d'entame est l'Excuse, la carte considérée est
     la suivante.

     @param[out] c La Carte à remplir.
     @return vrai ssi il y a une carte d'entame.
   */
  bool entame(Carte & c) const;

  /**
     @brief Retourne le joueur maître.

     @param[out] j Le joueur.
     @return vrai ssi il y a une carte jouée.
  */
  bool maitre(unsigned int & j) const;

  /**
     @brief Force le premier joueur.

     À utiliser au premier tour, car le premier joueur n'est pas
     forcément le numéro 0 (en cas de Chelem).
     @param joueur Le premier joueur.
   */
  void set_ouverture(unsigned int joueur);

  /**
     @brief Retourne le contenu du tapis.

     @note La Fausse excuse n'est pas présente !

     @param[out] cartes Le tableau des cartes posées.
     @param[out] poseurs Le tableau des poseurs.
   */
  void contenu(std::vector<Carte> & cartes, 
	       std::vector<unsigned int> & poseurs) const;

  /**
     @brief Présente le tapis.
   */
  std::ostream & presenter(std::ostream & out) const;


protected:
  
  /**
     @brief Appelé lorsque le maître change.
     
     Elle est appelée une fois que le joueur maître a effectivement
     changé. 
     
     @param ancien L'ancien joueur maître.
     @param nouveau Le nouveau joueur maître.

     @see nouveau_maitre(unsigned int)
   */
  virtual void changement_maitre(unsigned int ancien,
				 unsigned int nouveau);

  /**
     @brief Appelé lorsque l'on définit le maître.
     
     @param maitre Le numéro de tour du maître.
   */
  virtual void nouveau_maitre(unsigned int maitre);

  /**
     @brief Appelé lorsqu'un pli est terminé.

     @param cartes Les cartes du pli.
     @param poseurs Les poseurs de ces cartes respectives.
     @param gagnants Les gagnants de ces cartes.

     @warning Au moment de l'appel, le tour a déjà été
     changé. 
   */
  virtual void cartes_gagnees
  (std::vector<Carte> const & cartes,
   std::vector<unsigned int> const & poseurs,
   std::vector<unsigned int> const & gagnants);
  
  /**
     @brief Appelé lorsque le tour change.
     @param tour Le nouveau tour.
   */
  virtual void tour_change(unsigned int tour);
private:

  /**
     Numéro du joueur ayant effectué l'ouverture.
     Vaut -1 à l'origine.
     @see Tapis::set_ouverture(unsigned int)
   */
  int joueur_ouverture;

  /**
     Nombre de joueurs ayant joué avant le joueur maitre.
     
     C'est le numéro du joueur maitre - joueur_ouverture (mod 5).
   */
  int joueur_maitre;

  /**
     Indique si le joueur maître est fixé.
     En fin de chelem, le joueur 0 est maître s'il joue l'excuse.
   */
  bool maitre_fixe;

  /**
     Cartes sur le tapis.
     Vidé à chaque tour. Contient jusqu'à 5 lignes de 0, 1 ou 2 cartes.
   */
  std::vector<std::vector<Carte> > m_tapis;
};

/**
   @brief Écrit un Tapis.
   
   @param[out] out Le flux à utiliser.
   @param tap Le tapis à écrire.
   @return La suite du flux.
 */
std::ostream & operator<<(std::ostream & out, const Tapis & tap);

#endif
