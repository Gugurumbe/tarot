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


#include "debogueur.hpp"
#include <iostream>
#include <iomanip>

#ifdef COULEURS
#define C_NORMAL    "\x1B[0m"
#define C_ROUGE     "\x1B[31m"
#define C_VERT      "\x1B[32m"
#define C_JAUNE     "\x1B[33m"
#define C_BLEU      "\x1B[34m"
#define C_MAGENTA   "\x1B[35m"
#define C_CYAN      "\x1B[36m"
#define C_BLANC     "\x1B[37m"
#else
#define C_NORMAL    ""
#define C_ROUGE     ""
#define C_VERT      ""
#define C_JAUNE     ""
#define C_BLEU      ""
#define C_MAGENTA   "" 
#define C_CYAN      ""
#define C_BLANC     ""*/
#endif

#define C_NOMCLASSE C_MAGENTA
#define C_NOMMETHODE C_BLEU
#define C_OBJET C_VERT
#define C_NOMATTRIBUT C_JAUNE
#define C_VALEUR C_CYAN
#define C_RETOUR C_ROUGE

Debogueur * Debogueur::current = 0;

Debogueur::Debogueur(std::string const & n_methode,
		     std::string const & n_classe,
		     const void * n_objet) :
  parent(current), nom_methode(n_methode),
  nom_classe(n_classe), objet(n_objet)
{
  std::cout<<std::setw(parent?parent->indentation() + 1:1)<<" ";
  std::cout<<"->";
  std::cout<<C_NOMCLASSE<<nom_classe<<C_NORMAL<<"::";
  std::cout<<C_NOMMETHODE<<nom_methode<<C_NORMAL;
  std::cout<<", with ";
  std::cout<<C_NOMATTRIBUT<<"object"<<C_NORMAL<<" = "<<C_OBJET<<objet<<std::endl;
  std::cout<<C_NORMAL;
  current = this;
}

Debogueur::Debogueur(std::string const & nom_fonction) :
  parent(current), nom_methode(nom_fonction),
  nom_classe(""), objet(NULL)
{
  std::cout<<std::setw(parent?parent->indentation() + 1:1)<<" ";
  std::cout<<"->";
  std::cout<<C_NOMCLASSE<<nom_classe<<C_NORMAL<<"::";
  std::cout<<C_NOMMETHODE<<nom_methode<<C_NORMAL;
  std::cout<<", with ";
  std::cout<<C_NOMATTRIBUT<<"object"<<C_NORMAL<<" = "<<C_OBJET<<objet<<std::endl;
  std::cout<<C_NORMAL;
  current = this;
}

Debogueur::~Debogueur()
{
  std::cout<<std::setw(parent?parent->indentation() + 1:1)<<" ";
  std::cout<<"<-";
  std::cout<<C_NOMCLASSE<<nom_classe<<C_NORMAL<<"::";
  std::cout<<C_NOMMETHODE<<nom_methode<<C_NORMAL;
  std::cout<<", with ";
  std::cout<<C_NOMATTRIBUT<<"object"<<C_NORMAL<<" = "<<C_OBJET<<objet;
  std::cout<<C_NORMAL;
  std::cout<<" : return ";
  std::cout<<C_RETOUR;
  std::cout<<retour<<std::endl;
  std::cout<<C_NORMAL;
  current = parent;
}

void Debogueur::ajouter_argument(std::string const & nom, std::string const & valeur)
{
  std::cout<<std::setw(parent?parent->indentation():1)<<" ";
  std::cout<<"  [given "<<C_NOMATTRIBUT<<nom<<C_NORMAL<<" = "<<C_VALEUR<<valeur<<C_NORMAL<<"]"<<std::endl;
}

void Debogueur::retourner(std::string const & c)
{
  retour=c;
}

unsigned int Debogueur::indentation() const
{
  if(parent)
    {
      return 1 + parent->indentation();
    }
  return 1;
}

std::ostream & Debogueur::debug()
{
  if(current)
    {
      return std::cout<<(std::setw(current->indentation()))
		      <<"."
		      <<C_JAUNE
		      <<"[DBG] : "
		      <<C_NORMAL;
    }
  return std::cout<<"."<<C_JAUNE<<"[DBG] : "<<C_NORMAL;
}

std::ostream & Debogueur::error()
{
  if(current)
    {
      return std::cout<<C_ROUGE
		      <<"!"
		      <<(std::setw(current->indentation() - 1))
		      <<"!"
		      <<"[ERR] : "
		      <<C_NORMAL;
    }
  return std::cout<<C_ROUGE<<"![ERR] : "<<C_NORMAL;
}
/*
template<typename T>
void Debogueur::arg(std::string const & nom, 
		    const T & valeur)
{
  std::stringstream out;
  out<<valeur;
  if(current)
    current->ajouter_argument(nom, out.str());
}

template<typename T>
void Debogueur::ret(const T & valeur)
{
  std::stringstream out;
  out<<valeur;
  if(current)
    current->retourner(out.str());
}
//*/
