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


#include "transaction.hpp"
#include "option.cpp"

Transaction::Enchere::Enchere(){}
Transaction::Enchere::Enchere(const Transaction::Enchere & e):
  m_enchere(e.m_enchere), m_indice(e.m_indice) {}
Transaction::Enchere::Enchere(const ::Enchere & e):
  m_enchere(e) {}
Transaction::Enchere::Enchere(const ::Enchere & e,
			      const ::Enchere & indice):
  m_enchere(e), m_indice(indice) {}
Transaction::Enchere & Transaction::Enchere::operator=
(const Transaction::Enchere & e)
{
  m_enchere = e.m_enchere;
  m_indice = e.m_indice;
  return *this;
}
Transaction::Enchere & Transaction::Enchere::operator=
(const ::Enchere & e)
{
  m_enchere = e;
  m_indice.set();
  return *this;
}
const Option< ::Enchere> & Transaction::Enchere::obtenir() const
{
  return m_enchere;
}
const Option< ::Enchere> & Transaction::Enchere::indice() const
{
  return m_indice;
}
////////////////////////////////////////////////////////////////
Transaction::Appel::Appel() {}
Transaction::Appel::Appel(const Transaction::Appel & a):
  m_carte(a.m_carte), m_indice(a.m_indice) {}
Transaction::Appel::Appel(const Carte & c): m_carte(c) {}
Transaction::Appel::Appel(const Carte & c, 
			  const std::vector<Carte> & indice):
  m_carte(c), m_indice(indice) {}
Transaction::Appel & Transaction::Appel::operator=
(const Transaction::Appel & a)
{
  m_carte = a.m_carte;
  m_indice = a.m_indice;
  return *this;
}
Transaction::Appel & Transaction::Appel::operator=
(const Carte & c)
{
  m_carte = c;
  m_indice.set();
  return *this;
}
const Option<Carte> & Transaction::Appel::obtenir() const
{
  return m_carte;
}
const Option<std::vector<Carte> > & 
Transaction::Appel::indice() const
{
  return m_indice;
}
////////////////////////////////////////////////////////////////
Transaction::Ecart::Ecart(){}
Transaction::Ecart::Ecart(const Transaction::Ecart & e):
  m_ecart(e.m_ecart), m_indices(e.m_indices) {}
Transaction::Ecart::Ecart(const std::vector<Carte> & cartes):
  m_ecart(cartes) {}
Transaction::Ecart::Ecart(const std::vector<Carte> & cartes,
			  const std::vector<Carte> & indice,
			  const std::vector<Carte> & indice_2):
  m_ecart(cartes)
{
  m_indices.push_back(indice);
  m_indices.push_back(indice_2);
}
Transaction::Ecart & Transaction::Ecart::operator=
(const Transaction::Ecart & e)
{
  m_ecart = e.m_ecart;
  m_indices = e.m_indices;
  return *this;
}
Transaction::Ecart & Transaction::Ecart::operator=
(const std::vector<Carte> & cartes)
{
  m_ecart = cartes;
  m_indices.clear();
  return *this;
}
const Option<std::vector<Carte> > & 
Transaction::Ecart::obtenir() const
{
  return m_ecart;
}
Option<std::vector<Carte> >  
Transaction::Ecart::indice1() const
{
  if(m_indices.size() > 0)
    return m_indices[0];
  return Option<std::vector<Carte> >();
}
 Option<std::vector<Carte> >
Transaction::Ecart::indice2() const
{
  if(m_indices.size() > 1)
    return m_indices[1];
  return Option<std::vector<Carte> >();
}
////////////////////////////////////////////////////////////////////
Transaction::Jeu::Jeu(){}
Transaction::Jeu::Jeu(const Transaction::Jeu & j):
  m_carte(j.m_carte){}
Transaction::Jeu::Jeu(const Carte & c):
  m_carte(c) {}
Transaction::Jeu & Transaction::Jeu::operator=
(const Transaction::Jeu & j)
{
  m_carte = j.m_carte;
  return *this;
}
Transaction::Jeu & Transaction::Jeu::operator=
(const Carte & c)
{
  m_carte = c;
  return *this;
}
const Option<Carte> & Transaction::Jeu::obtenir() const
{
  return m_carte;
}
//////////////////////////////////////////////////////////////////
Transaction::Identification::Identification(){}
Transaction::Identification::Identification
(const Transaction::Identification & i):
  m_nom(i.m_nom){}
Transaction::Identification::Identification(const std::string & nom):
  m_nom(nom) {}
Transaction::Identification & Transaction::Identification::operator=
(const Transaction::Identification & i)
{
  m_nom = i.m_nom;
  return *this;
}
Transaction::Identification & Transaction::Identification::operator=
(const std::string & nom)
{
  m_nom = nom;
  return *this;
}
const Option<std::string> & Transaction::Identification::obtenir() const
{
  return m_nom;
}
//////////////////////////////////////////////////////////////////
Transaction::Invitation::Invitation(){}
Transaction::Invitation::Invitation
(const Transaction::Invitation & i):
  m_adv(i.m_adv){}
Transaction::Invitation::Invitation(const std::vector<std::string> & adv):
  m_adv(adv) {}
Transaction::Invitation & Transaction::Invitation::operator=
(const Transaction::Invitation & i)
{
  m_adv = i.m_adv;
  return *this;
}
Transaction::Invitation & Transaction::Invitation::operator=
(const std::vector<std::string> & adv)
{
  m_adv = adv;
  return *this;
}
const Option<std::vector<std::string> > & Transaction::Invitation::obtenir() const
{
  return m_adv;
}
//////////////////////////////////////////////////////////////////
Transaction::Transaction() {}
Transaction::Transaction(const Transaction & t):
  m_enchere(t.m_enchere), m_appel(t.m_appel), 
  m_ecart(t.m_ecart), m_jeu(t.m_jeu),
  m_identification(t.m_identification),
  m_invitation(t.m_invitation){}
Transaction::Transaction(const Transaction::Enchere & t):
  m_enchere(t) {}
Transaction::Transaction(const Transaction::Appel & t):
  m_appel(t) {}
Transaction::Transaction(const Transaction::Ecart & t):
  m_ecart(t) {}
Transaction::Transaction(const Transaction::Jeu & t):
  m_jeu(t) {}
Transaction::Transaction(const Transaction::Identification & t):
  m_identification(t) {}
Transaction::Transaction(const Transaction::Invitation & t):
  m_invitation(t) {}
Transaction & Transaction::operator=(const Transaction & t)
{
  m_enchere = t.m_enchere;
  m_appel = t.m_appel;
  m_ecart = t.m_ecart;
  m_jeu = t.m_jeu;
  m_identification = t.m_identification;
  m_invitation = t.m_invitation;
  return *this;
}
Transaction & Transaction::operator=(const Transaction::Enchere & t)
{
  m_enchere = t;
  m_appel.set();
  m_ecart.set();
  m_jeu.set();
  m_identification.set();
  m_invitation.set();
  return *this;
}
Transaction & Transaction::operator=(const Transaction::Appel & t)
{
  m_enchere.set();
  m_appel = t;
  m_ecart.set();
  m_jeu.set();
  m_identification.set();
  m_invitation.set();
  return *this;
}
Transaction & Transaction::operator=(const Transaction::Ecart & t)
{
  m_enchere.set();
  m_appel.set();
  m_ecart = t;
  m_jeu.set();
  m_identification.set();
  m_invitation.set();
  return *this;
}
Transaction & Transaction::operator=(const Transaction::Jeu & t)
{
  m_enchere.set();
  m_appel.set();
  m_ecart.set();
  m_jeu = t;
  m_identification.set();
  m_invitation.set();
  return *this;
}
Transaction & Transaction::operator=(const Transaction::Identification & t)
{
  m_enchere.set();
  m_appel.set();
  m_ecart.set();
  m_jeu.set();
  m_identification = t;
  m_invitation.set();
  return *this;
}
Transaction & Transaction::operator=(const Transaction::Invitation & t)
{
  m_enchere.set();
  m_appel.set();
  m_ecart.set();
  m_jeu.set();
  m_identification.set();
  m_invitation = t;
  return *this;
}
const Option<Transaction::Enchere> & Transaction::enchere() const
{
  return m_enchere;
}
const Option<Transaction::Appel> & Transaction::appel() const
{
  return m_appel;
}
const Option<Transaction::Ecart> & Transaction::ecart() const
{
  return m_ecart;
}
const Option<Transaction::Jeu> & Transaction::jeu() const
{
  return m_jeu;
}
const Option<Transaction::Identification> & Transaction::identification() const
{
  return m_identification;
}
const Option<Transaction::Invitation> & Transaction::invitation() const
{
  return m_invitation;
}
template class Option<Transaction::Enchere>;
template class Option<Transaction::Appel>;
template class Option<Transaction::Ecart>;
template class Option<Transaction::Jeu>;
template class Option<Transaction::Identification>;
template class Option<Transaction::Invitation>;
template class Option<Carte>;
template class Option<std::vector<Carte> >;
