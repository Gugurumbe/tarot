#include "table.hpp"

#include <cstdlib>

#include <iostream>

#define NOM_CLASSE "Table"

#include "deboguer.hpp"

int Table::nombre_tables = 0;

Table::Table(QObject * parent) : QObject(parent)
{
  ENTER("Table(QObject * parent)");
  ADD_ARG("parent", parent);
  //Création des ordres :
  for(int i = 0 ; i < 5 ; i++)
    {
      joueurs.push_back(-1); // -1 : pas de joueur
      ordre.push_back(i);
      noms.push_back("");
    }
  //Mélange de ordre :
  int r = 0 ;
  int tmp;
  for(unsigned int i = 1 ; i < ordre.size() ; i++)
    {
      r = rand() % (i + 1) ;
      //échange des cases i et r
      tmp = ordre[r];
      ordre[r] = ordre[i];
      ordre[i] = tmp;
    }
  QObject::connect(&partie, SIGNAL(doit_emettre
				   (unsigned int, Protocole::Message, bool)),
		   this, SLOT(doit_transmettre
			      (unsigned int, Protocole::Message, bool)));
  QObject::connect(&partie, SIGNAL(termine()), this, SLOT(doit_recommencer()));
  nombre_tables++;
  DEBUG<<"Il y a maintenant "<<nombre_tables<<" table(s)."<<std::endl;
}

Table::~Table()
{
  ENTER("~Table()");
  for(unsigned int i = 0 ; i < joueurs.size() ; i++)
    {
      if(joueurs[i]>=0)
	emit doit_deconnecter(joueurs[i]);
    }
  nombre_tables--;
  DEBUG<<"Il n'y a plus que "<<nombre_tables<<" table(s)."<<std::endl;
}

void Table::ajouter(unsigned int sock, std::string nom)
{
  ENTER("ajouter(unsigned int sock, std::string nom)");
  ADD_ARG("sock", sock);
  ADD_ARG("nom", nom);
  unsigned int i = 0;
  while(i < joueurs.size() && joueurs[i] >= 0) i++;
  if(i < joueurs.size())
    {
      joueurs[i] = sock;
      noms[i] = nom;
      //Si on est complet, on lance la partie
      i = 0 ;
      while(i < joueurs.size() && joueurs[i] >= 0) i++;
      if(i >= joueurs.size())
	{
	  DEBUG<<"La partie commence..."<<std::endl;
	  doit_recommencer();
	  emit complet(this);
	}
    }
  else
    {
      //En fait, la table était déjà pleine !
      ERROR<<"Erreur : la table est pleine."<<std::endl;
      emit complet(this);
      emit doit_deconnecter(sock);
      //Sinon, la socket sera perdue
    }
}

void Table::comprendre(unsigned int sock, Protocole::Message m)
{
  ENTER("comprendre(unsigned int sock, Message m)");
  ADD_ARG("sock", sock);
  ADD_ARG("m", m);
  //Attention : je ne suis pas sûr que sock fasse partie de la table !
  for(unsigned int i = 0 ; i < joueurs.size() ; i++)
    {
      if(joueurs[i] == (int)sock)
	{
	  Protocole::Message reponse;
	  switch(partie.tester(ordre[i], m))
	    {
	    case 1 :
	      reponse.type = Protocole::ERREUR_PROTOCOLE;
	      DEBUG<<"Erreur de protocole détectée."<<std::endl;
	      emit doit_emettre(sock, reponse);
	      break;
	    case 2 :
	      reponse.type = Protocole::REFUSE;
	      emit doit_emettre(sock, reponse);
	      break;
	    default :
	      partie.assimiler(m);
	    }
	  i = joueurs.size();
	}
    }
}

void Table::enlever(unsigned int sock)
{
  ENTER("enlever(unsigned int sock)");
  ADD_ARG("sock", sock);
  //Attention : sock ne fait peut-être pas partie de la table !
  unsigned int i = 0 ;
  while(i < joueurs.size() && 
	(joueurs[i] < 0 || (unsigned int)joueurs[i] != sock)) i++;
  if(i < joueurs.size())
    {
      joueurs[i] = -1 ;
      noms[i] = "";
      //Comptons les joueurs restants. S'ils sont 4, il faut envoyer
      //le signal incomplet.
      unsigned int j = 0 ;
      for(unsigned int k = 0 ; k < joueurs.size() ; k++)
	if(joueurs[k]>=0) j++;
      if(j == 4)
	{
	  DEBUG<<"Émission de Table::incomplet(Table *)..."<<std::endl;
	  emit incomplet(this);
	}
    } 
}
void Table::doit_transmettre(unsigned int j, Protocole::Message m,
			     bool analyser)
{
  ENTER("doit_transmettre(unsigned int j, Protocole::Message m, bool analyser)");
  ADD_ARG("j", j);
  ADD_ARG("m", m);
  ADD_ARG("analyser", analyser);
  DEBUG<<"Transmission à "<<j<<std::endl;
  for(unsigned int i = 0 ; i < joueurs.size() ; i++)
    {
      if(ordre[i] == j)
	{
	  emit doit_emettre(joueurs[i], m);
	  DEBUG<<"C'est "<<i<<std::endl;
	}
    }
  if(analyser)
    partie.assimiler(m);
}
void Table::doit_recommencer()
{
  //Réinitialisation de la partie :
  partie.reinitialiser();
  //Permutations
  std::vector<unsigned int> nouvel_ordre;
  for(unsigned int i = 0 ; i < joueurs.size() ; i++)
    {
      nouvel_ordre.push_back(ordre[(i + 1) % ordre.size()]);
    }
  ordre = nouvel_ordre;
  //Attention : on ne change pas l'ordre des noms des joueurs !
  //Émission de noms
  Protocole::Message msg_noms;
  msg_noms.type = Protocole::NOMS;
  for(unsigned int j = 0 ; j < joueurs.size() ; j++)
    {
      for(unsigned int k = 0 ; k < noms[ordre[j]].size() 
	    && k < TAILLE_NOM ; k++)
	{
	  msg_noms.m.noms.noms[j][k] = noms[ordre[j]][k];
	}
    }
  for(unsigned int j = 0 ; j < joueurs.size() ; j++)
    {
      emit doit_emettre(joueurs[j], msg_noms);
    }
  //La partie a besoin de le savoir !
  partie.assimiler(msg_noms);
  //Émission des numéros : la partie s'en moque
  for(unsigned int j = 0 ; j < joueurs.size() ; j++)
    {
      Protocole::Message m;
      m.type = Protocole::NUMERO;
      m.m.numero.n = ordre[j];
      emit doit_emettre(joueurs[j], m);
    }
  partie.distribuer();
}
