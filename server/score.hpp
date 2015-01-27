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


#ifndef SCORE_DEFINI
#define SCORE_DEFINI

#include "carte.hpp"

#include "enchere.hpp"

#include <vector>

/**
   @brief À la fin de la partie, compte les points.
   
   @param contrat_final Le contrat final.
   @param joueur_appele Le joueur appelé.
   @param gagnees Les cartes engrangées par l'attaque à chaque tour de
   jeu. Certaines lignes sont vides. La première est possiblement le
   chien. 
   @param perdues Les cartes engrangées par la défense. Sert à dire si
   elle a mené le petit au bout.
   @param poignees Les tailles des poignées anoncées.

   @return Les points de chacun. La somme est nulle.
 */
std::vector<int> compter(const Enchere & contrat_final, 
			 unsigned int joueur_appele, 
			 const std::vector<std::vector<Carte> > 
			 & gagnees,
			 const std::vector<std::vector<Carte> >
			 & perdues,
			 const std::vector<unsigned int> & poignees);

#endif
