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
   @file client/shared/option.hpp
   
   Définit un type "option", qui permet de contenir un objet d'un
   certain type (COPIABLE) ou rien.
 */

#ifndef OPTION_DEFINIE

/**
   @brief Empêche les inclusions récursives.
 */
#define OPTION_DEFINIE

#include <stdexcept>

/**
   @brief Classe gérant une copie d'un objet ou rien.
   
   Utilise un pointeur, qui appartient en propre à cette classe. Elle
   travaille donc avec des copies des objets à représenter.
 */
template<typename T>
class Option
{
public:
  /**
     @brief Exception levée lorsqu'on essaye de récupérer un objet
     s'il n'y en a pas.
   */
  class none : public std::logic_error
  {
  public:
    /**
       @brief Constructeur par défaut.
       
       C'est une exception logique affichant :
       "Option vide."
     */
    explicit none();
  };
  /**
     @brief Constructeur par défaut.
     
     Par défaut, aucun objet n'est présent.
   */
  Option();
  /**
     @brief Constructeur de copie.
     
     Copie une fois de plus l'objet réel s'il est présent.
     @param o L'option à copier.
   */
  Option(const Option<T> & o);
  /**
     @brief Constructeur standard.
     
     Copie l'objet pour l'utiliser.
     @param obj L'objet à représenter.
   */
  Option(const T & obj);
  /**
     @brief Destructeur.
   */
  virtual ~Option();
  /**
     @brief Donne un objet à représenter.
     
     Copie cet objet.
     @param obj L'objet à représenter.
   */
  void set(const T & obj);
  /**
     @brief S'assure qu'aucun objet n'est représenté.
   */
  void set();
  /**
     @brief Permet de savoir s'il y a un objet représenté.
     
     @return Vrai ssi il n'y a pas d'objet représenté.
   */
  bool aucun() const;
  /**
     @brief Retourne l'objet contenu.
     
     @return L'objet contenu.
     @throw Option::none
   */
  T get() const;
private:
  T * content;
};

#include <iostream>

template<typename T>
std::ostream & operator<<(std::ostream & out, const Option<T> & t);

#endif
