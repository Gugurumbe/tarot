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
   @file shared/ne_pas_deboguer.hpp
   @brief À inclure pour ne pas déboguer ce fichier.
*/

#include <iostream>

//Nécessaire pour afficher un vector.
#include "debogueur.hpp"

/**
   @brief Stub.
 */
#define ENTER(meth)

/**
   @brief Stub.
 */
#define ADD_ARG(nom, val)

/**
   @brief Stub.
*/
#define EXIT(val) return val

/**
   @brief Stub.
 */
#define DEBUG std::ostream(0)

/**
   @brief Flux d'erreur.
 */
#define ERROR std::cerr<<"!"<<
