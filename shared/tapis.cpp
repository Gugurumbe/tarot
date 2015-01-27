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


#include "tapis.hpp"

#define NOM_CLASSE "Tapis"

#include "deboguer.hpp"

Tapis::Tapis():joueur_ouverture(-1), joueur_maitre(-1), 
	       maitre_fixe(false)
{
  ENTER("Tapis()");
  m_tapis.reserve(5);
}

Tapis::Tapis(const Tapis & tap):
  joueur_ouverture(tap.joueur_ouverture),
  joueur_maitre(tap.joueur_maitre),
  maitre_fixe(tap.maitre_fixe),
  m_tapis(tap.m_tapis)
{
  ENTER("Tapis(const Tapis & tap)");
  DEBUG<<"Joueur maître : "<<joueur_maitre<<std::endl;
}

void Tapis::reinitialiser()
{
  ENTER("reinitialiser()");
  joueur_ouverture = -1;
  joueur_maitre = -1;
  maitre_fixe = false;
  m_tapis.clear();
  m_tapis.reserve(5);
}

Tapis::~Tapis()
{
  ENTER("~Tapis()");
}

void Tapis::ajouter(const Protocole::Msg_carte & carte, 
		    Carte::ForceExcuse exc)
{
  ENTER("ajouter(Msg_carte carte, ForceExcuse exc)");
  ADD_ARG("carte", carte);
  ADD_ARG("exc", exc);
  //On ajoute une ligne 
  Carte c(carte.carte);
  std::vector<Carte> ligne;
  ligne.push_back(c);
  int ancien_maitre = joueur_maitre;
  if(c == EXCUSE) ligne.push_back(Carte(DETTE_EXCUSE));
  if(exc == Carte::EXCUSE_GAGNANTE)
    {
      DEBUG<<"Le joueur "<<(joueur_ouverture + m_tapis.size())%5
	   <<" a été déclaré maître quoi qu'il advienne."<<std::endl;
      maitre_fixe = true;
      joueur_maitre = m_tapis.size();
    }
  else if(m_tapis.size() == 0) joueur_maitre = 0;
  else if(m_tapis[joueur_maitre][0] > c || maitre_fixe)
    {
      DEBUG<<"Le joueur "<<(joueur_maitre + joueur_ouverture)%5
	   <<" reste maître."<<std::endl;
    }
  else
    {
      DEBUG<<"Le joueur "<<(m_tapis.size() + joueur_ouverture)%5
	   <<" passe maître devant "
	   <<(joueur_maitre + joueur_ouverture)%5<<std::endl;
      joueur_maitre=m_tapis.size();
    }
  m_tapis.push_back(ligne);
  if(m_tapis.size() == 1)
    {
      nouveau_maitre((joueur_maitre + joueur_ouverture) % 5);
    }
  else if(ancien_maitre != joueur_maitre)
    {
      changement_maitre((ancien_maitre + joueur_ouverture) % 5, 
			(joueur_maitre + joueur_ouverture) % 5);
    }
  if(m_tapis.size() == 5)
    {
      //Le pli est terminé
      std::vector<Carte> posees;
      std::vector<unsigned int> poseurs;
      std::vector<unsigned int> gagnants;
      unsigned int maitre_final = joueur_maitre;
      for(unsigned int i = 0 ; i < m_tapis.size() ; i++)
	for(unsigned int j = 0 ; j < m_tapis[i].size() ; j++)
	  {
	    posees.push_back(m_tapis[i][j]);
	    poseurs.push_back((i + joueur_ouverture) % 5);
	    gagnants.push_back((i + joueur_ouverture) % 5);
	  }
      //Assignation des cartes au gagnant :
      for(unsigned int i = 0 ; i < posees.size() ; i++)
	{
	  if(posees[i] != EXCUSE || exc == Carte::EXCUSE_PRENABLE)
	    gagnants[i] = (maitre_final + joueur_ouverture) % 5;
	}
      joueur_maitre = maitre_final;
      joueur_ouverture = (maitre_final + joueur_ouverture) % 5;
      m_tapis.clear();
      maitre_fixe = false;
      tour_change(joueur_ouverture);
      cartes_gagnees(posees, poseurs, gagnants);
      joueur_maitre = -1;
    }
  else
    {
      //Le tour a quand même changé.
      tour_change((joueur_ouverture + m_tapis.size()) % 5);
    }
}

void Tapis::set_ouverture(unsigned int joueur)
{
  ENTER("set_ouverture(unsigned int joueur)");
  ADD_ARG("joueur", joueur);
  joueur_ouverture = (int)joueur;
  m_tapis.clear();
}

bool Tapis::plus_gros_atout(Carte & c) const
{
  ENTER("plus_gros_atout(Carte & c) const");
  bool b = false;
  //Vrai si c a été modifiée
  for(unsigned int i = 0 ; i < m_tapis.size() ; i++)
    for(unsigned int j = 0 ; j < m_tapis[i].size() ; j++)
      {
	if(m_tapis[i][j] != EXCUSE && m_tapis[i][j].atout())
	  {
	    if((b && m_tapis[i][j] > c) || !b)
	      {
		c = m_tapis[i][j];
	      }
	    b = true;
	  }
    }
  DEBUG<<"Plus gros atout enregistré : "<<c<<std::endl;
  EXIT(b);
}

bool Tapis::entame(Carte & c) const
{
  ENTER("entame(Carte & c) const");
  if(m_tapis.size() == 0) 
    {
      EXIT(false);
    }
  if(m_tapis[0][0] == EXCUSE && m_tapis.size() == 1) 
    {
      EXIT(false);
    }
  if(m_tapis[0][0] == EXCUSE) c = m_tapis[1][0];
  else c = m_tapis[0][0];
  DEBUG<<"Entame : "<<c<<std::endl;
  EXIT(true);
}

bool Tapis::maitre(unsigned int & j) const
{
  ENTER("maitre(unsigned int & j) const");
  DEBUG<<"maître : "<<(joueur_maitre + joueur_ouverture) % 5
       <<std::endl;
  if(joueur_maitre < 0) 
    {
      EXIT(false);
    }
  j = (joueur_maitre + joueur_ouverture) % 5;
  DEBUG<<"maître : "<<j<<std::endl;
  EXIT(true);
}

void Tapis::changement_maitre(unsigned int ancien,
			      unsigned int nouveau)
{
  ENTER("changement_maitre(unsigned int ancien, unsigned int nouveau)");
  ADD_ARG("ancien", ancien);
  ADD_ARG("nouveau", nouveau);
  DEBUG<<"Spécialisez cette classe."<<std::endl;
}

void Tapis::nouveau_maitre(unsigned int maitre)
{
  ENTER("nouveau_maitre(unsigned int maitre)");
  ADD_ARG("maitre", maitre);
  DEBUG<<"Spécialisez cette classe."<<std::endl;
}

void Tapis::cartes_gagnees(std::vector<Carte> const & cartes,
			   std::vector<unsigned int> const & poseurs,
			   std::vector<unsigned int> const & gagnants)
{
  ENTER("cartes_gagnees(vector<Carte> cartes, vector<uint> poseurs, vector<uint> gagnants)");
  ADD_ARG("cartes", cartes);
  ADD_ARG("poseurs", poseurs);
  ADD_ARG("gagnants", gagnants);
  DEBUG<<"Spécialisez cette classe."<<std::endl;
}

void Tapis::tour_change(unsigned int)
{
}

std::ostream & Tapis::presenter(std::ostream & out) const
{
  out<<"(Tapis : ";
  for(unsigned int i = 0 ; i < m_tapis.size() ; i++)
    {
      out<<((joueur_ouverture + i) % 5)
	 <<" a joué "<<m_tapis[i];
      if((int)i == joueur_maitre) out<<" [maitre]";
      if(i + 1 < m_tapis.size()) out<<", ";
    }
  out<<")";
  return out;
}

void Tapis::contenu(std::vector<Carte> & cartes, 
		    std::vector<unsigned int> & poseurs) const
{
  ENTER("contenu(std::vector<Carte> & cartes, std::vector<unsigned int> & poseurs) const");
  ADD_ARG("cartes", cartes);
  ADD_ARG("poseurs", poseurs);
  cartes.clear();
  poseurs.clear();
  for(unsigned int i = 0 ; i < m_tapis.size() ; i++)
    {
      //On ajoute la carte du joueur joueur_ouverture + i
      for(unsigned int j = 0 ; j < m_tapis[i].size() ; j++)
	{
	  if(m_tapis[i][j] != DETTE_EXCUSE)
	    {
	      DEBUG<<"Dans le tapis, "<<(i + joueur_ouverture) % 5
		   <<" a joué la carte "<<m_tapis[i][j]<<std::endl;
	      cartes.push_back(m_tapis[i][j]);
	      poseurs.push_back((i + joueur_ouverture) % 5);
	    }
	}
    }
  DEBUG<<"cartes : "<<cartes<<", poseurs : "<<poseurs<<std::endl;
}

std::ostream & operator<<(std::ostream & out, const Tapis & tap)
{
  return tap.presenter(out);
}
