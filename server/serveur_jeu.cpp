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
}

void ServeurJeu::reagir_connexion(unsigned int i)
{
  ENTER("reagir_connexion(unsigned int i)");
  ADD_ARG("i", i);
  paillasson.push_back(i);
  DEBUG<<"Voici le paillasson : "<<paillasson<<std::endl;
  QObject::connect(this, SIGNAL(message(unsigned int, Protocole::Message)),
		   this, SLOT(lire(unsigned int, Protocole::Message)));
}
void ServeurJeu::lire(unsigned int client, Protocole::Message m)
{
  ENTER("lire(unsigned int client, Protocole::Message m)");
  ADD_ARG("client", client);
  ADD_ARG("m", m);
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
	      bool ok = true;
	      for(unsigned int j = 0 ; j < noms.size() ; j++)
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
		      emit envoyer(vestibule[j], nouveau);
		      for(unsigned int k = 0 ; k < TAILLE_NOM ; k++)
			{
			  if(k < noms[j].size())
			    ancien.m.entree.nom[k] = noms[j][k];
			  else
			    ancien.m.entree.nom[k] = '\0';
			}
		      emit envoyer(client, ancien);
		    }
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
		  emit envoyer(client, refus);
		}
	    }
	  else
	    {
	      //Erreur de protocole
	      DEBUG<<"Erreur de protocole détectée : "<<m<<std::endl;
	      Protocole::Message erreur;
	      erreur.type = Protocole::ERREUR_PROTOCOLE;
	      emit envoyer(client, erreur);
	    }
	  i = paillasson.size();
	}
    }
  //Présence du client dans le vestibule
  for(unsigned int i = 0 ; i < vestibule.size() ; i++)
    {
      if(vestibule[i] == client)
	{
	  DEBUG<<"C'est un client du vestibule qui cause."<<std::endl;
	  if(m.compris && m.type == Protocole::INVITER)
	    {
	      std::vector<std::string> joueurs;
	      joueurs.push_back(noms[i]);
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
		}
	      std::vector<unsigned int> sockets;
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
			  sockets.push_back(vestibule[k]);
			  indice_vestibule.push_back(k);
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
			  emit envoyer(vestibule[j], sorties[k]);
			}
		    }
		  //Les 5 joueurs vont être envoyés dans une table. 
		  Table * t = new Table(this);
		  connect(t, SIGNAL(envoyer
				    (unsigned int, Protocole::Message)),
			  this, SLOT(envoyer(unsigned int, Protocole::Message)));
		  connect(this, SIGNAL(deconnexion(unsigned int)),
			  t, SLOT(enlever(unsigned int)));
		  connect(this, SIGNAL(message
				       (unsigned int, Protocole::Message)),
			  t, SLOT(comprendre
				      (unsigned int, Protocole::Message)));
		  for(unsigned int j = 0 ; j < sockets.size() ; j++)
		    {
		      t->ajouter(sockets[j], noms[sockets[j]]);
		    }
		}
	      else
		{
		  //On refuse !
		  DEBUG<<"On refuse."<<std::endl;
		  Protocole::Message refus;
		  refus.type = Protocole::REFUSE;
		  emit envoyer(client, refus);
		}
	    }
	  else
	    {
	      //Erreur de protocole
	      DEBUG<<"Erreur de protocole détectée : "<<m<<std::endl;
	      Protocole::Message erreur;
	      erreur.type = Protocole::ERREUR_PROTOCOLE;
	      emit envoyer(client, erreur);
	    }
	  i = vestibule.size();
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
	  vestibule.erase(vestibule.begin() + i);
	  i = vestibule.size() ;
	  DEBUG<<vestibule<<std::endl;
	}
    }
}
