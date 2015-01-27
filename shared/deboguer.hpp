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
   @file shared/deboguer.hpp
   @brief À inclure pour déboguer ce fichier.

   @note Vous devez définir NOM_CLASSE avec la valeur (sous forme de
   chaîne) du nom de la classe. 
*/

#include "debogueur.hpp"

/**
   @brief Smart alias for ENTER_CLASS.

   @note Vous devez avoir défini NOM_CLASSE.
 */
#define ENTER(meth) ENTER_CLASS(meth, NOM_CLASSE)

/**
   @brief Alias pour Debogueur::arg.
 */
#define ADD_ARG(nom, val) Debogueur::arg(nom, val)

/**
   @brief Alias pour Debogueur::ret.

   @warning L'argument est évalué 2 fois !!!
*/
#define EXIT(val)				\
  Debogueur::ret(val);				\
  return val

/**
   @brief Flux de débogage.
 */
#define DEBUG Debogueur::debug()

/**
   @brief Flux d'erreur.
 */
#define ERROR Debogueur::error()
