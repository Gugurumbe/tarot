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

#include "carte.hpp"
#include <sstream>

#define NOM_CLASSE "Carte"

#include "ne_pas_deboguer.hpp"

//Commenter si c'est trop moche.
#define INDIQUER_NUMERO_CARTE

Carte::Carte(): m_numero(0)
{
  ENTER("Carte()");
  DEBUG<<"ATTENTION : UTILISATION DU MAUVAIS CONSTRUCTEUR"<<std::endl;
}

Carte::Carte(unsigned int numero): m_numero(numero % 79)
{
  ENTER("Carte(unsigned int numero)");
  ADD_ARG("numero",numero);
  //Note : si le numéro est supérieur à 79,
  //m_numero retombe en-dessous de 79.
}

Carte::Carte(const Carte & carte): m_numero(carte.m_numero)
{
  ENTER("Carte(const Carte & carte)");
  ADD_ARG("carte.nom()", carte.nom());
}

bool Carte::operator>(unsigned int numero) const
{
  ENTER("operator>(unsigned int numero) const");
  ADD_ARG("numero", numero);
  //pas besoin de se fatiguer, inutile de réimplanter une fonction
  //déjà existante.
  bool b = operator>(Carte(numero));
  EXIT(b);
}

bool Carte::operator>(const Carte & carte) const
{
  ENTER("operator>(const Carte & carte) const");
  ADD_ARG("carte.nom()", carte.nom());
  //On compare this (qui contient m_numero) et carte.
  //Si la carte est l'excuse, alors elle ne remporte pas le pli
  if(m_numero == EXCUSE || m_numero == DETTE_EXCUSE) EXIT(false);
  if(carte.m_numero == EXCUSE || 
     carte.m_numero == DETTE_EXCUSE) EXIT(true);
  //Si on est de la même couleur, c'est la valeur qui décide
  if(couleur() == carte.couleur()) 
    {
      bool b = valeur() > carte.valeur();
      EXIT(b);
    }
  //Sinon, on perd ssi l'autre carte est un atout
  bool b = !(carte.atout());
  EXIT(b);
}

bool Carte::operator==(unsigned int carte) const
{
  ENTER("operator==(unsigned int carte) const");
  ADD_ARG("carte", carte);
  EXIT(m_numero == carte);
}

bool Carte::operator==(const Carte & carte) const
{
  ENTER("operator==(const Carte & carte) const");
  ADD_ARG("carte.nom()", carte.nom());
  EXIT(carte.m_numero == m_numero);
}

bool Carte::operator!=(unsigned int carte) const
{
  return !operator==(carte);
}

bool Carte::operator!=(const Carte & carte) const
{
  return !operator==(carte);
}

unsigned int Carte::numero() const
{
  return m_numero;
}

Carte::Couleur Carte::couleur() const
{
  ENTER("couleur() const");
  //Si je ne suis pas un atout, ma couleur est numéro / 14.
  if(atout()) 
    {
      EXIT(ATOUT);
    }
  EXIT((Carte::Couleur)(m_numero / 14));
}

bool Carte::atout() const
{
  return m_numero >= DEBUT_ATOUT; //L'excuse est un atout !
}

bool Carte::tete() const
{
  return !atout() && valeur() >= VALET;
}

bool Carte::roi() const
{
  return !atout() && valeur() == ROI;
}

bool Carte::bout() const
{
  return m_numero == PETIT
    || m_numero == EXCUSE
    || m_numero == VINGTETUN;
  //La fausse excuse n'est pas un bout, c'est une carte basse.
}

Carte::Valeur Carte::valeur() const
{
  if(!(atout())) 
    return Carte::Valeur(m_numero % 14);
  if(m_numero == EXCUSE || m_numero == DETTE_EXCUSE) 
    return Carte::Valeur(0);
  return Carte::Valeur(m_numero - DEBUT_ATOUT + 1);
  /* Ainsi, le petit a la valeur 1 (supérieure à l'excuse) */
}

unsigned int Carte::demipoints() const
{
  if(!(atout()))
    {
      if(valeur() < VALET) return 1; //Un demi-point
      return (valeur() - VALET + 1) * 2 + 1;
      //Valet : valeur=10, points=3 demi-points
      //Ainsi de suite
    }
  else if(m_numero == PETIT 
	  || m_numero == VINGTETUN) return 9; //4.5 points
  else if(m_numero == EXCUSE) return 8; //4 points
  else return 1; //Les autres atouts et la dette d'excuse
}

Carte::ModaliteEcart Carte::ecartable() const
{
  if(bout() || roi()) return NON_ECARTABLE;
  if(atout()) return MONTRER_CARTE;
  return ECARTABLE;
}

Carte const & Carte::operator=(const Carte & carte)
{
  m_numero = carte.m_numero;
  return *this;
}

Carte const & Carte::operator=(unsigned int carte)
{
  m_numero = carte % 79;
  return *this;
}

std::string Carte::couleur_simple() const
{
  std::string res;
  switch(couleur())
    {
    case Carte::PIQUE : res = "pique" ; break ;
    case Carte::TREFLE : res = "trefle" ; break ;
    case Carte::CARREAU : res = "carreau" ; break ;
    case Carte::COEUR : res = "coeur" ; break ;
    default : res = "atout" ; break ;
    }
  return res;
}

std::string Carte::nom() const
{
  std::stringstream resultat;
  #ifdef INDIQUER_NUMERO_CARTE
  resultat<<"["<<m_numero<<"] ";
  #endif
  if(*this == EXCUSE)
    resultat<<"Excuse";
  else if(*this == DETTE_EXCUSE)
    resultat<<"Fausse excuse";
  else if(atout()) 
    switch((int)valeur())
      {
      case 1: resultat<<"Petit";break;
      case 2: resultat<<"Deux d'Atout";break;
      case 3: resultat<<"Trois d'Atout";break;
      case 4: resultat<<"Quatre d'Atout";break;
      case 5: resultat<<"Cinq d'Atout";break;
      case 6: resultat<<"Six d'Atout";break;
      case 7: resultat<<"Sept d'Atout";break;
      case 8: resultat<<"Huit d'Atout";break;
      case 9: resultat<<"Neuf d'Atout";break;
      case 10:resultat<<"Dix d'Atout";break;
      case 11:resultat<<"Onze d'Atout";break;
      case 12:resultat<<"Douze d'Atout";break;
      case 13:resultat<<"Treize d'Atout";break;
      case 14:resultat<<"Quatorze d'Atout";break;
      case 15:resultat<<"Quinze d'Atout";break;
      case 16:resultat<<"Seize d'Atout";break;
      case 17:resultat<<"Dix-sept d'Atout";break;
      case 18:resultat<<"Dix-huit d'Atout";break;
      case 19:resultat<<"Dix-neuf d'Atout";break;
      case 20:resultat<<"Vingt d'Atout";break;
      case 21:resultat<<"Vingt et un";break;
      default:resultat<<"Atout inconnu";break;
      }
  else
    {
      switch(valeur())
	{
	case AS:resultat<<"As";break;
	case DEUX:resultat<<"Deux";break;
	case TROIS:resultat<<"Trois";break;
	case QUATRE:resultat<<"Quatre";break;
	case CINQ:resultat<<"Cinq";break;
	case SIX:resultat<<"Six";break;
	case SEPT:resultat<<"Sept";break;
	case HUIT:resultat<<"Huit";break;
	case NEUF:resultat<<"Neuf";break;
	case DIX:resultat<<"Dix";break;
	case VALET:resultat<<"Valet";break;
	case CAVALIER:resultat<<"Cavalier";break;
	case DAME:resultat<<"Dame";break;
	case ROI:resultat<<"Roi";break;
	default: resultat<<"?(valeur="<<(int)valeur()<<")";break;
	}
      resultat<<" de ";
      switch(couleur())
	{
	case PIQUE:resultat<<"Pique";break;
	case CARREAU:resultat<<"Carreau";break;
	case TREFLE:resultat<<"Trèfle";break;
	case COEUR:resultat<<"Cœur";break;
	default:resultat<<"?(couleur="<<(int)couleur()<<")";break;
	}
    }
  std::string r = resultat.str();
  return r;
}

std::ostream & operator<<(std::ostream & out, const Carte & carte)
{
  out<<carte.nom();
  return out;
}

std::ostream & operator<<(std::ostream & out, Carte::Couleur couleur)
{
  switch(couleur)
    {
    case Carte::PIQUE:out<<"Pique";break;
    case Carte::CARREAU:out<<"Carreau";break;
    case Carte::TREFLE:out<<"Trèfle";break;
    case Carte::COEUR:out<<"Cœur";break;
    case Carte::ATOUT:out<<"Atout";break;
    default:out<<"Une autre Couleur";break;
    }
  return out;
}

std::ostream & operator<<(std::ostream & out, Carte::Valeur valeur)
{
  switch(valeur)
    {
    case Carte::AS:out<<"As/Excuse";break;
    case Carte::DEUX:out<<"Deux/Petit";break;
    case Carte::TROIS:out<<"Trois/Deux d'atout";break;
    case Carte::QUATRE:out<<"Quatre/Trois d'atout";break;
    case Carte::CINQ:out<<"Cinq/Quatre d'atout";break;
    case Carte::SIX:out<<"Six/Cinq d'atout";break;
    case Carte::SEPT:out<<"Sept/Six d'atout";break;
    case Carte::HUIT:out<<"Huit/Sept d'atout";break;
    case Carte::NEUF:out<<"Neuf/Huit d'atout";break;
    case Carte::DIX:out<<"Dix/Neuf d'atout";break;
    case Carte::VALET:out<<"Valet/Dix d'atout";break;
    case Carte::CAVALIER:out<<"Cavalier/Onze d'atout";break;
    case Carte::DAME:out<<"Dame/Douze d'atout";break;
    case Carte::ROI:out<<"Roi/Treize d'atout";break;
    default:out<<"Une autre valeur d'atout : "<<(int)valeur;break;
    }
  return out;
}

std::ostream & operator<<(std::ostream & out, Carte::ModaliteEcart mod)
{
  switch(mod)
    {
    case Carte::ECARTABLE:out<<"écartable";break;
    case Carte::MONTRER_CARTE:out<<"montrer la carte";break;
    case Carte::NON_ECARTABLE:out<<"non écartable";break;
    default:out<<"modalité d'écart inconnue ("<<(int)mod<<")";
    }
  return out;
}
std::ostream & operator<<(std::ostream & out, Carte::ForceExcuse exc)
{
  switch(exc)
    {
    case Carte::EXCUSE_GAGNANTE:out<<"excuse gagnante";break;
    case Carte::EXCUSE_IMPRENABLE:out<<"excuse imprenable";break;
    case Carte::EXCUSE_PRENABLE:out<<"excuse prenable";break;
    default:out<<"autre force d'excuse";
    }
  return out;
}
