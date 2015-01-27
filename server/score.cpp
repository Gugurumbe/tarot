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


#include "score.hpp"

#include "debogueur.hpp"

#define dbg Debogueur::debug()

std::vector<int> compter(const Enchere & contrat_final,
			 unsigned int joueur_appele,
			 const std::vector<std::vector<Carte> >
			 & gagnees,
			 const std::vector<std::vector<Carte> >
			 & perdues,
			 const std::vector<unsigned int> 
			 & poignees)
{
  ENTER_FUNCTION
    ("compter(const Enchere & contrat_final, "
     "unsigend int joueur_appele, "
     "const std::vector<std::vector<Carte> > & gagnees, "
     "const std::vector<std::vector<Carte> > & perdues, "
     "const std::vector<unsigned int> & poignees)");
  Debogueur::arg("contrat_final", contrat_final);
  Debogueur::arg("joueur_appele", joueur_appele);
  Debogueur::arg("gagnees", gagnees);
  Debogueur::arg("perdues", perdues);
  Debogueur::arg("poignees", poignees);
  //Décompte des points de cartes :
  int demipoints_attaque = 0;
  int nombre_bouts = 0;
  unsigned int preneur = contrat_final.joueur();
  bool pab_attaque = false, pab_defense = false;
  dbg<<"Comptage des cartes : "<<std::endl;
  for(unsigned int i = 0 ; i < gagnees.size() ; i++)
    {
      dbg<<"Tour "<<i<<"..."<<std::endl;
      for(unsigned int j = 0 ; j < gagnees[i].size() ; j++)
	{
	  dbg<<"La carte "<<gagnees[i][j]<<" rapporte "
	     <<gagnees[i][j].demipoints()<<" demi-points."
	     <<std::endl;
	  demipoints_attaque+=gagnees[i][j].demipoints();
	  if(gagnees[i][j].bout())
	    {
	      dbg<<"C'est un bout."<<std::endl;
	      nombre_bouts++;
	    }
	}
    }
  //Recherche du petit dans un dernier tour :
  //attaque :
  for(unsigned int i = 0 ; 
      i < gagnees[-1 + gagnees.size()].size() ; i++)
    {
      if(gagnees[-1 + gagnees.size()][i] == PETIT)
	{
	  dbg<<"L'attaque mène le petit au bout."
	     <<std::endl;
	  pab_attaque = true;
	}
    }
  //Défense :
  if(!pab_attaque)
    for(unsigned int i = 0 ;
	i < perdues[-1 + perdues.size()].size() ; i++)
      {
	if(perdues[-1 + perdues.size()][i] == PETIT)
	  {
	    dbg<<"La défense mène le petit au bout."
	       <<std::endl;
	    pab_defense = true;
	  }
      }
  unsigned int points = demipoints_attaque / 2;
  unsigned int doit_faire = 0;
  switch(nombre_bouts)
    {
    case 0 :
      dbg<<"Sans bout, il faut faire 56 points."
	 <<std::endl;
      doit_faire = 56;
      break;
    case 1 :
      dbg<<"Avec un bout, il faut faire 51 points."
	 <<std::endl;
      doit_faire = 51;
      break;
    case 2 :
      dbg<<"2 Bouts ? Il faut faire 41."
	 <<std::endl;
      doit_faire = 41;
      break;
    default :
      dbg<<"Avec "<<nombre_bouts<<", il faut faire 36 points."
	 <<std::endl;
      doit_faire = 36;
      break;
    }
  int total = 0;
  if(points >= doit_faire)
    {
      dbg<<"Le contrat est rempli, on part de +25."
	 <<std::endl;
      total = 25;
    }
  else
    {
      dbg<<"Le contrat est chuté, on part de -25."
	 <<std::endl;
      total = -25;
    }
  dbg<<"On ajoute le nombre de points de l'attaque."
     <<std::endl;
  total += points - doit_faire;
  if(pab_attaque)
    {
      dbg<<"Petit au bout pour l'attaque : +10."
	 <<std::endl;
      total += 10;
    }
  else if(pab_defense)
    {
      dbg<<"Petit au bout pour la défense : -10."
	 <<std::endl;
      total -= 10;
    }
  switch(contrat_final.prise())
    {
    case Enchere::GARDE :
      total *= 2 ;
      dbg<<"Garde : multiplication par 2."
	 <<std::endl;
      break;
    case Enchere::GARDE_SANS :
      total *= 4 ;
      dbg<<"Garde sans : multiplication par 4 !"
	 <<std::endl;
      break;
    case Enchere::GARDE_CONTRE :
      total *= 6 ;
      dbg<<"Garde contre : multiplication par 6 !"
	 <<std::endl;
      break;
    case Enchere::PASSE : //absurde
    case Enchere::PRISE :
      dbg<<"Pas de multiplication."<<std::endl;
    default:
      break;
    }
  dbg<<"Traitement des poignées :"<<std::endl;
  for(unsigned int i = 0 ; i < poignees.size() ; i++)
    {
      switch(poignees[i])
	{
	case 8 :
	  dbg<<i<<" déclare une simple poignée."
	     <<std::endl;
	  if(i == preneur || i == joueur_appele)
	    {
	      dbg<<"Pour l'attaque : +20"<<std::endl;
	      total += 20;
	    }
	  else
	    {
	      dbg<<"Pour la défense : +20"<<std::endl;
	      total -= 20;
	    }
	  break;
	case 10 :
	  dbg<<i<<" déclare une double poignée."
	     <<std::endl;
	  if(i == preneur || i == joueur_appele)
	    {
	      dbg<<"Pour l'attaque : +30"<<std::endl;
	      total += 30;
	    }
	  else
	    {
	      dbg<<"Pour la défense : +30"<<std::endl;
	      total -= 30;
	    }
	  break;
	case 13 :
	  dbg<<i<<" déclare une triple poignée."
	     <<std::endl;
	  if(i == preneur || i == joueur_appele)
	    {
	      dbg<<"Pour l'attaque : +40"<<std::endl;
	      total += 40;
	    }
	  else
	    {
	      dbg<<"Pour la défense : +40"<<std::endl;
	      total -= 40;
	    }
	  break;
	default:
	  dbg<<"Pas de poignée déclarée par "<<i<<std::endl;
	}
    }
  dbg<<"Tout le monde part de "<<-total<<std::endl;
  std::vector<int> resultat;
  for(unsigned int i = 0 ; i < 5 ; i++)
    {
      resultat.push_back(-total);
    }
  dbg<<"Le joueur appelé marque "<<total<<std::endl;
  resultat[joueur_appele] = total;
  dbg<<"Pour l'instant, le preneur marque 0"<<std::endl;
  resultat[preneur] = 0;
  int somme = 0;
  for(unsigned int i = 0 ; i < resultat.size() ; i++)
    {
      somme += resultat[i];
    }
  dbg<<"Le preneur marque "<<-somme<<std::endl;
  resultat[preneur] = -somme;
  Debogueur::ret(resultat);
  return resultat;
}
