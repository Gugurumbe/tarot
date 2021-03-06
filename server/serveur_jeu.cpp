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


#include "serveur_jeu.hpp"
#include "debogueur.hpp"
#include <iostream>

#define NOM_CLASSE "ServeurJeu"

#include "deboguer.hpp"

ServeurJeu::ServeurJeu(QObject * parent) : Serveur(parent)
{
  ENTER("ServeurJeu(QObject * parent)");
  ADD_ARG("parent", parent);
  QObject::connect(this, SIGNAL(connexion(unsigned int)),
		   this, SLOT(reagir_connexion(unsigned int)));
  // Les déconnexions et messages seront directement transmis à toutes
  // les tables.
  QObject::connect(this, SIGNAL(deconnexion(unsigned int)),
		   this, SLOT(oublier_socket(unsigned int)));
  QObject::connect(this, SIGNAL(message(unsigned int, Protocole::Message)),
		   this, SLOT(lire(unsigned int, Protocole::Message)));
}

void ServeurJeu::reagir_connexion(unsigned int i)
{
  ENTER("reagir_connexion(unsigned int i)");
  ADD_ARG("i", i);
  paillasson.push_back(i);
  DEBUG<<"Voici le paillasson : "<<paillasson<<std::endl;
}
void ServeurJeu::lire(unsigned int client, Protocole::Message m)
{
  ENTER("lire(unsigned int client, Protocole::Message m)");
  ADD_ARG("client", client);
  ADD_ARG("m", m);
  //Présence du client dans le vestibule : avant le paillasson, car au
  //cours du traitement le client du paillasson est ajouté au vestibule.
  for(unsigned int i = 0 ; i < vestibule.size() ; i++)
    {
      if(vestibule[i] == client)
	{
	  DEBUG<<"C'est un client du vestibule qui cause."<<std::endl;
	  if(m.compris && m.type == Protocole::INVITER)
	    {
	      DEBUG<<"Il demande une invitation."<<std::endl;
	      std::vector<std::string> joueurs;
	      joueurs.push_back(noms[i]);
	      //Il est sûr de s'inviter.
	      for(unsigned int j = 0 ; j < 4 ; j++)
		{
		  std::string n;
		  for(unsigned int k = 0 ; 
		      k < TAILLE_NOM && m.m.inviter.noms[j][k] != '\0' ; 
		      k++)
		    {
		      n.push_back(m.m.inviter.noms[j][k]);
		    }
		  joueurs.push_back(n);
		  DEBUG<<"Il demande à inviter "<<n<<"."<<std::endl;
		}
	      std::vector<unsigned int> sockets;
	      //On ajoute lui-même.
	      std::vector<unsigned int> indice_vestibule;
	      std::vector<unsigned int> vestibule_filtre;
	      std::vector<std::string> noms_filtres;
	      vestibule_filtre.reserve(vestibule.size());
	      noms_filtres.reserve(noms.size());
	      for(unsigned int j = 0 ; j < joueurs.size() ; j++)
		{
		  for(unsigned int k = 0 ; k < noms.size() ; k++)
		    {
		      if(noms[k] == joueurs[j])
			{
			  DEBUG<<"J'ai trouvé "<<joueurs[j]<<"."<<std::endl;
			  sockets.push_back(vestibule[k]);
			  indice_vestibule.push_back(k);
			  k = noms.size();
			}
		    }
		}
	      for(unsigned int j = 0 ; j < sockets.size() ; j++)
		{
		  for(unsigned int k = j + 1 ; k < sockets.size() ; k++)
		    {
		      if(sockets[j] == sockets[k])
			{
			  //Doublon, on refuse
			  DEBUG<<"Doublon ("<<j<<"ième nom)."<<std::endl;
			  sockets.clear();
			}
		    }
		}
	      if(sockets.size() == 5)
		{
		  //On filtre
		  DEBUG<<"Invitation de "<<sockets<<std::endl;
		  for(unsigned int j = 0 ; j < vestibule.size() ; j++)
		    {
		      unsigned int k = 0;
		      while(k < 5 && sockets[k] != vestibule[j])k++;
		      if(k >= 5)
			{
			  DEBUG<<noms[j]<<" n'est pas concerné."<<std::endl;
			  vestibule_filtre.push_back(vestibule[j]);
			  noms_filtres.push_back(noms[j]);
			}		      
		    }
		  //On gère les sorties
		  std::vector<Protocole::Message> sorties;
		  sorties.resize(5);
		  for(unsigned int j = 0 ; j < sorties.size() ; j++)
		    {
		      sorties[j].type = Protocole::SORTIE;
		      for(unsigned int k = 0 ; k < TAILLE_NOM ; k++)
			{
			  if(k < noms[indice_vestibule[j]].size())
			    sorties[j].m.sortie.nom[k] =      
			      noms[indice_vestibule[j]][k];
			  else sorties[j].m.sortie.nom[k] = '\0';
			}
		    }
		  //Les sorties sont relatives aux NOMS des joueurs.
		  vestibule = vestibule_filtre; // On n'envoie pas
						// sortie aux
						// joueurs. 
		  noms = noms_filtres;
		  for(unsigned int j = 0 ; j < vestibule.size() ; j++)
		    {
		      for(unsigned int k = 0 ; k < sorties.size() ; k++)
			{
			  envoyer(vestibule[j], sorties[k]);
			}
		    }
		  //Les 5 joueurs vont être envoyés dans une table. 
		  Table * t = new Table(this);
		  connect(t, SIGNAL(doit_emettre
				    (unsigned int, Protocole::Message)),
			  this, SLOT(envoyer
				     (unsigned int, Protocole::Message)));
		  connect(this, SIGNAL(deconnexion(unsigned int)),
			  t, SLOT(enlever(unsigned int)));
		  connect(this, SIGNAL(message
				       (unsigned int, Protocole::Message)),
			  t, SLOT(comprendre
				      (unsigned int, Protocole::Message)));
		  connect(t, SIGNAL(incomplet(Table *)), 
			  t, SLOT(deleteLater()));
		  connect(t, SIGNAL(doit_deconnecter(unsigned int)),
			  this, SLOT(deconnecter(unsigned int)));
		  for(unsigned int j = 0 ; j < sockets.size() ; j++)
		    {
		      t->ajouter(sockets[j], joueurs[j]);
		    }
		}
	      else
		{
		  //On refuse !
		  DEBUG<<"On refuse."<<std::endl;
		  Protocole::Message refus;
		  refus.type = Protocole::REFUSE;
		  envoyer(client, refus);
		}
	    }
	  else
	    {
	      //Erreur de protocole
	      DEBUG<<"Erreur de protocole détectée : "<<m<<std::endl;
	      Protocole::Message erreur;
	      erreur.type = Protocole::ERREUR_PROTOCOLE;
	      envoyer(client, erreur);
	    }
	  i = vestibule.size();
	}
    }
  //Présence du client dans le paillasson
  for(unsigned int i = 0 ; i < paillasson.size() ; i++)
    {
      if(paillasson[i] == client)
	{
	  DEBUG<<"Celui qui cause est sur le paillasson ("<<i<<")."<<std::endl;
	  if(m.compris && m.type == Protocole::IDENTIFIER)
	    {
	      std::string nom;
	      for(unsigned int j = 0 ; 
		  j < TAILLE_NOM && m.m.identifier.nom[j] != '\0' ; j++)
		{
		  nom.push_back(m.m.identifier.nom[j]);
		}
	      DEBUG<<"Il veut s'appeler "<<nom<<std::endl;
	      //Est-ce que le nom est dispo ?
	      bool ok = (nom != "");
	      for(unsigned int j = 0 ; j < noms.size() && ok ; j++)
		{
		  if(noms[j] == nom)
		    {
		      //Refusé
		      DEBUG<<"Le client du vestibule "<<j<<" a déjà ce nom."
			   <<std::endl;
		      ok = false;
		      j = noms.size();
		    }
		}
	      if(ok)
		{
		  //Accepté
		  DEBUG<<"Le nom est accepté."<<std::endl;
		  Protocole::Message nouveau;
		  nouveau.type = Protocole::ENTREE;
		  for(unsigned int j = 0 ; j < TAILLE_NOM ; j++)
		    {
		      if(j < nom.size())
			nouveau.m.entree.nom[j] = nom[j];
		      else nouveau.m.entree.nom[j] = '\0';
		    }
		  Protocole::Message ancien;
		  ancien.type = Protocole::ENTREE;
		  for(unsigned int j = 0 ; j < vestibule.size() ; j++)
		    {
		      //Échange de poignée de main
		      envoyer(vestibule[j], nouveau);
		      for(unsigned int k = 0 ; k < TAILLE_NOM ; k++)
			{
			  if(k < noms[j].size())
			    ancien.m.entree.nom[k] = noms[j][k];
			  else
			    ancien.m.entree.nom[k] = '\0';
			}
		      envoyer(client, ancien);
		    }
		  //Cas du premier client
		  DEBUG<<"Cas du premier client."<<std::endl;
		  envoyer(client, nouveau);
		  //Ajout au vestibule
		  vestibule.push_back(client);
		  noms.push_back(nom);
		  //Sortie du paillasson
		  paillasson.erase(paillasson.begin() + i);
		  DEBUG<<"Voici le paillasson : "<<paillasson<<std::endl;
		  DEBUG<<"Voici le vestibule : "<<vestibule<<std::endl;
		  DEBUG<<"Les noms correspondants sont : "<<noms<<std::endl;
		}
	      else
		{
		  //On refuse !
		  DEBUG<<"On refuse."<<std::endl;
		  Protocole::Message refus;
		  refus.type = Protocole::REFUSE;
		  envoyer(client, refus);
		}
	    }
	  else
	    {
	      //Erreur de protocole
	      DEBUG<<"Erreur de protocole détectée : "<<m<<std::endl;
	      Protocole::Message erreur;
	      erreur.type = Protocole::ERREUR_PROTOCOLE;
	      envoyer(client, erreur);
	    }
	  i = paillasson.size();
	}
    }
}

void ServeurJeu::detacher_table(Table *)
{
}

void ServeurJeu::rattacher_table(Table *)
{
}

void ServeurJeu::oublier_table(QObject *)
{
}

void ServeurJeu::oublier_socket(unsigned int client)
{
  ENTER("oublier_socket(unsigned int client)");
  ADD_ARG("client", client);
  for(unsigned int i = 0 ; i < paillasson.size() ; i++)
    {
      if(paillasson[i] == client)
	{
	  DEBUG<<"J'enlève un client du paillasson."<<std::endl;
	  paillasson.erase(paillasson.begin() + i);
	  i = paillasson.size() ;
	  DEBUG<<paillasson<<std::endl;
	}
    }
  for(unsigned int i = 0 ; i < vestibule.size() ; i++)
    {
      if(vestibule[i] == client)
	{
	  DEBUG<<"J'enlève un client du vestibule."<<std::endl;
	  Protocole::Message depart;
	  depart.type = Protocole::SORTIE;
	  for(unsigned int k = 0 ; k < TAILLE_NOM ; k++)
	    {
	      if(k < noms[i].size())
		depart.m.sortie.nom[k] =      
		  noms[i][k];
	      else depart.m.sortie.nom[k] = '\0';
	    }
	  vestibule.erase(vestibule.begin() + i);
	  noms.erase(noms.begin() + i);
	  i = vestibule.size() ;
	  for(unsigned int j = 0 ; j < vestibule.size() ; j++)
	    {
	      envoyer(vestibule[j], depart);
	    }
	  DEBUG<<vestibule<<std::endl;
	}
    }
}
