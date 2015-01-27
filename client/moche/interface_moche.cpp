#include "interface_moche.hpp"

#include <iostream>
#include <cstdio>

#define ENDL std::endl;

#define NOM_CLASSE "InterfaceMoche"

#include "deboguer.hpp"

#define lock m_mutex.lock()
#define unlock m_mutex.unlock()

InterfaceMoche::InterfaceMoche
(std::ostream & out, QObject * parent): 
  QThread(parent), o(out), continuer(true)
{
  lock;
  ENTER("InterfaceMoche(std::ostream & out, QObject * parent)");
  ADD_ARG("parent", parent);
  unlock;
}

InterfaceMoche::~InterfaceMoche()
{
  lock;
  ENTER("~InterfaceMoche()");
  continuer = false;
  fclose(stdin);
  std::cin.sync();
  DEBUG<<"Quitte..."<<std::endl;
  wait();
  DEBUG<<"Terminé."<<std::endl;
  unlock;
}
			
void InterfaceMoche::connecte()
{
  lock;
  ENTER("connecte()");
  o<<"Vous êtes connecté.\n";
  unlock;
}			  
void InterfaceMoche::deconnecte()
{
  lock;
  ENTER("deconnecte()");
  o<<"Vous êtes déconnecté.\n";
  unlock;
}
void InterfaceMoche::doit_identifier()
{
  lock;
  ENTER("doit_identifier()");
  if(mon_nom == "")
    {
      o<<"Vous devez vous identifier.\n";
    }
  else
    {
      emit formuler_identification
	(std::string(mon_nom.toUtf8().data()));
    }
  unlock;
}
void InterfaceMoche::identification_acceptee(std::string nom)
{
  lock;
  ENTER("identification_acceptee(std::string nom)");
  ADD_ARG("nom", nom);
  mon_nom = QString::fromUtf8(nom.c_str());
  o<<"Vous êtes connecté en tant que "<<mon_nom<<"\n";
  m_adversaires.clear();
  unlock;
}
void InterfaceMoche::identification_refusee(std::string nom)
{
  lock;
  ENTER("identification_refusee(std::string nom)");
  ADD_ARG("nom", nom);
  mon_nom = "";
  o<<"Vous n'avez pas réussi à prendre le nom "
   <<QString::fromUtf8(nom.c_str())<<ENDL;
  unlock;
}
void InterfaceMoche::entree(std::string nom)
{
  lock;
  ENTER("entree(std::string nom)");
  ADD_ARG("nom", nom);
  QString str = QString::fromUtf8(nom.c_str());
  vestibule.append(str);
  o<<str<<" est disponible."<<ENDL;
  unlock;
}
void InterfaceMoche::sortie(std::string nom)
{
  lock;
  ENTER("sortie(std::string nom)");
  ADD_ARG("nom", nom);
  QString str = QString::fromUtf8(nom.c_str());
  int i = vestibule.indexOf(str);
  do
    {
      vestibule.remove(i);
      i = vestibule.indexOf(str);
    }while(i < vestibule.size());
  o<<str<<" n'est plus disponible."<<ENDL;
  unlock;
}
void InterfaceMoche::adversaires(std::vector<std::string> noms)
{
  lock;
  ENTER("adversaires(std::vector<std::string> noms)");
  ADD_ARG("noms", noms);
  vestibule.clear();
  m_adversaires.clear();
  for(unsigned int i = 0 ; i < noms.size() ; i++)
    {
      QString str = QString::fromUtf8(noms[i].c_str());
      m_adversaires.append(str);
    }
  o<<"Les joueurs à votre table sont ";
  o<<m_adversaires[0];
  for(int i = 1 ; i < m_adversaires.size() ; i++)
    {
      o<<", "<<m_adversaires[i];
    }
  o<<" dans cet ordre."<<ENDL;
  unlock;
}
void InterfaceMoche::invitation_acceptee(std::vector<std::string> 
					 adversaires)
{
  lock;
  ENTER("invitation_acceptee(std::vector<std::string> adversaires)");
  ADD_ARG("adversaires", adversaires);
  o<<"Vous avez demandé à ";
  o<<QString::fromUtf8
    (adversaires[0].c_str());
  for(unsigned int i = 1 ; i < adversaires.size() ; i++)
    {
      o<<", "<<QString::fromUtf8(adversaires[i].c_str());
    }
  o<<" de se joindre à vous."<<ENDL;
  unlock;
}
void InterfaceMoche::invitation_refusee(std::vector<std::string> 
					adversaires)
{
  lock;
  ENTER("invitation_refusee(std::vector<std::string> adversaires)");
  ADD_ARG("adversaires", adversaires);
  o<<"Vous n'avez pas le droit d'inviter ";
  o<<QString::fromUtf8(adversaires[0].c_str());
  for(unsigned int i = 1 ; i < adversaires.size() ; i++)
    {
      o<<", "<<QString::fromUtf8(adversaires[i].c_str());
    }
  o<<"."<<ENDL;
  unlock;
}
void InterfaceMoche::numero_change(unsigned int n)
{
  lock;
  ENTER("numero_change(unsigned int n)");
  ADD_ARG("n", n);
  o<<"Vous jouez en ";
  switch(n)
    {
    case 0:o<<"premier";break;
    case 1:o<<"deuxième";break;
    case 2:o<<"troisième";break;
    case 3:o<<"quatrième";break;
    case 4:o<<"dernier";break;
    default:o<<n<<"ième (?)";
    }
  o<<"."<<ENDL;
  unlock;
}
void InterfaceMoche::doit_priser(Option<Enchere> max)
{
  lock;
  ENTER("doit_priser(Option<Enchere> max)");
  ADD_ARG("max.aucun()", max.aucun());
  o<<"Vous devez priser. ";
  if(max.aucun())
    {
      o<<"Personne n'a rien dit pour le moment."<<ENDL;
    }
  else
    {
      unsigned int n = max.get().joueur();
      Enchere::Prise p = max.get().prise();
      switch(p)
	{
	case Enchere::PASSE:
	  o<<"Jusqu'à présent, tout le monde a passé."<<ENDL;
	  break;
	case Enchere::PRISE:
	  o<<m_adversaires[n]<<" prise."<<ENDL;
	  break;
	case Enchere::GARDE:
	  o<<m_adversaires[n]<<" garde."<<ENDL;
	  break;
	case Enchere::GARDE_SANS:
	  o<<m_adversaires[n]<<" garde sans le chien !"<<ENDL;
	  break;
	case Enchere::GARDE_CONTRE:
	  o<<m_adversaires[n]<<" garde contre le chien !!"<<ENDL;
	  break;
	default:
	  o<<"Je n'ai pas compris la prise de "<<m_adversaires[n]
	   <<"."<<ENDL;
	}
    }
  unlock;
}
void InterfaceMoche::enchere_acceptee(Enchere e)
{
  lock;
  ENTER("enchere_acceptee(Enchere e)");
  ADD_ARG("e", e);
  Enchere::Prise p = e.prise();
  switch(p)
    {
    case Enchere::PASSE:
      o<<"Vous avez réussi à passer."<<ENDL;
      break;
    case Enchere::PRISE:
      o<<"Vous avez réussi à priser."<<ENDL;
      break;
    case Enchere::GARDE:
      o<<"Vous avez réussi à garder."<<ENDL;
      break;
    case Enchere::GARDE_SANS:
      o<<"Vous avez réussi à garder sans le chien !"<<ENDL;
      break;
    case Enchere::GARDE_CONTRE:
      o<<"Vous avez réussi à garde contre le chien !!"<<ENDL;
      break;
    default:
      o<<"Je n'ai pas compris votre prise..."<<ENDL;
    }
  unlock;
}
void InterfaceMoche::enchere_refusee(Enchere e)
{
  lock;
  ENTER("enchere_refusee(Enchere e)");
  ADD_ARG("e", e);
  Enchere::Prise p = e.prise();
  switch(p)
    {
    case Enchere::PASSE:
      o<<"Vous n'avez pas réussi à passer."<<ENDL;
      break;
    case Enchere::PRISE:
      o<<"Vous n'avez pas réussi à priser."<<ENDL;
      break;
    case Enchere::GARDE:
      o<<"Vous n'avez pas réussi à garder."<<ENDL;
      break;
    case Enchere::GARDE_SANS:
      o<<"Vous n'avez pas réussi à garder sans le chien !"<<ENDL;
      break;
    case Enchere::GARDE_CONTRE:
      o<<"Vous n'avez pas réussi à garde contre le chien !!"<<ENDL;
      break;
    default:
      o<<"Je n'ai pas compris votre prise... "
	"De toutes façons, elle est refusée."<<ENDL;
    }
  unlock;
}
void InterfaceMoche::contrat_intermediaire(Enchere e)
{
  lock;
  ENTER("contrat_intermediaire(Enchere e)");
  ADD_ARG("e", e);
  unsigned int n = e.joueur();
  Enchere::Prise p = e.prise();
  switch(p)
    {
    case Enchere::PASSE:
      o<<m_adversaires[n]<<" passe."<<ENDL;
      break;
    case Enchere::PRISE:
      o<<m_adversaires[n]<<" prise."<<ENDL;
      break;
    case Enchere::GARDE:
      o<<m_adversaires[n]<<" garde."<<ENDL;
      break;
    case Enchere::GARDE_SANS:
      o<<m_adversaires[n]<<" garde sans le chien !"<<ENDL;
      break;
    case Enchere::GARDE_CONTRE:
      o<<m_adversaires[n]<<" garde contre le chien !!"<<ENDL;
      break;
    default:
      o<<"Je n'ai pas compris la prise de "<<m_adversaires[n]
       <<"."<<ENDL;
    }
  unlock;
}
void InterfaceMoche::doit_appeler(std::vector<Carte> acceptees)
{
  lock;
  ENTER("doit_appeler(std::vector<Carte> acceptees)");
  ADD_ARG("acceptees", acceptees);
  appelables.clear();
  o<<"Vous devez appeler l'une de ces cartes : ";
  o<<QString::fromUtf8(acceptees[0].nom().c_str());
  appelables.append(QSharedPointer<Carte>(new Carte(acceptees[0])));
  for(unsigned int i = 1 ; i < acceptees.size() ; i++)
    {
      appelables.append(QSharedPointer<Carte>(new Carte(acceptees[i])));
      o<<QString::fromUtf8(acceptees[i].nom().c_str())<<", ";
    }
  o<<"."<<ENDL;
  unlock;
}
void InterfaceMoche::appel_accepte(Carte c)
{
  lock;
  ENTER("appel_accepte(Carte c)");
  ADD_ARG("c", c);
  appelables.clear();
  o<<"Vous avez réussi à appeler la carte "
   <<QString::fromUtf8(c.nom().c_str())<<ENDL;
  unlock;
}
void InterfaceMoche::appel_refuse(Carte c)
{
  lock;
  ENTER("appel_refusee(Carte c)");
  ADD_ARG("c", c);
  o<<"Vous n'avez pas réussi à appeler la carte "
   <<QString::fromUtf8(c.nom().c_str())<<ENDL;
  unlock;
}
void InterfaceMoche::contrat_final(Enchere e)
{
  lock;
  ENTER("contrat_final(Enchere e)");
  ADD_ARG("e", e);
  unsigned int n = e.joueur();
  Enchere::Prise p = e.prise();
  Carte c = *(e.carte_appelee());
  switch(p)
    {
    case Enchere::PASSE:
      o<<m_adversaires[n]<<" passe, et "<<ENDL;
      break;
    case Enchere::PRISE:
      o<<m_adversaires[n]<<" prise, et "<<ENDL;
      break;
    case Enchere::GARDE:
      o<<m_adversaires[n]<<" garde, et "<<ENDL;
      break;
    case Enchere::GARDE_SANS:
      o<<m_adversaires[n]<<" garde sans le chien, et "<<ENDL;
      break;
    case Enchere::GARDE_CONTRE:
      o<<m_adversaires[n]<<" garde contre le chien, et "<<ENDL;
      break;
    default:
      o<<"Je n'ai pas compris la prise de "<<m_adversaires[n]
       <<", mais "<<ENDL;
    }
  o<<" il(elle) appelle "<<QString::fromUtf8(c.nom().c_str())
   <<ENDL;
  unlock;
}
void InterfaceMoche::chien(Carte c1, Carte c2, Carte c3)
{
  lock;
  ENTER("chien(Carte c1, Carte c2, Carte c3)");
  ADD_ARG("c1", c1);
  ADD_ARG("c2", c2);
  ADD_ARG("c3", c3);
  o<<"Le chien est dévoilé, il contient "
   <<QString::fromUtf8(c1.nom().c_str())<<", "
   <<QString::fromUtf8(c2.nom().c_str())<<" et "
   <<QString::fromUtf8(c3.nom().c_str())<<"."<<ENDL;
  unlock;
}
void InterfaceMoche::doit_ecarter(std::vector<Carte> possibles,
				  std::vector<Carte> atouts)
{
  lock;
  ENTER("doit_ecarter(std::vector<Carte> possibles, std::vector<Carte> atouts)");
  ADD_ARG("possibles", possibles);
  ADD_ARG("atouts", atouts);
  ecartables.clear();
  o<<"Vous devez faire un écart. ";
  QVector<QString> obligatoires;
  QVector<QString> n_possibles;
  //Par défaut, on a moins de 3 cartes écartables :
  QString nom;
  for(unsigned int i = 0 ; i < possibles.size() ; i++)
    {
      nom = QString::fromUtf8(atouts[i].nom().c_str());
      ecartables.append(QSharedPointer<Carte>(new Carte(atouts[i])));
      obligatoires.append(nom);
    }
  for(unsigned int i = 0 ; i < atouts.size() ; i++)
    {
      nom = QString::fromUtf8(atouts[i].nom().c_str());
      ecartables.append(QSharedPointer<Carte>(new Carte(atouts[i])));
      n_possibles.append(nom);
    }
  if(obligatoires.size() >= 3)
    {
      //Mais si on en a plus, rien n'est obligatoire.
      n_possibles = obligatoires;
      obligatoires.clear();
    }
  if(obligatoires.size() > 0)
    {
      o<<"Vous devez écarter ";
      for(int i = 0 ; i < obligatoires.size() ; i++)
	{
	  o<<obligatoires[i];
	  if(i == obligatoires.size() - 2)
	    {
	      o<<", et ";
	    }
	  else if(i == obligatoires.size() - 1)
	    {
	      o<<", "; //Conclusion
	    }
	  else
	    {
	      o<<", ";
	    }
	}
    }
  if(obligatoires.size() < 3)
    {
      if(obligatoires.size() == 0)
	{
	  o<<"Vous devez écarter ";
	}
      else
	{
	  o<<"et vous devez écarter ";
	}
      o<<3-obligatoires.size()<<" cartes parmi ";
      for(int i = 0 ; i < n_possibles.size() ; i++)
	{
	  o<<n_possibles[i];
	  if(i == n_possibles.size() - 2)
	    {
	      o<<", et ";
	    }
	  else if(i == n_possibles.size() - 1)
	    {
	      o<<"."<<ENDL; //Conclusion
	    }
	  else
	    {
	      o<<", ";
	    }
	}
    }
  unlock;
}
void InterfaceMoche::ecart_accepte(std::vector<Carte> ecart)
{
  lock;
  ENTER("ecart_accepte(std::vector<Carte> ecart)");
  ADD_ARG("ecart", ecart);
  ecartables.clear();
  o<<"Vous avez réussi à écarter ";
  for(unsigned int i = 0 ; i < ecart.size() ; i++)
    {
      o<<QString::fromUtf8(ecart[i].nom().c_str());
      if(i == ecart.size() - 2)
	{
	  o<<", et ";
	}
      else if(i == ecart.size() - 1)
	{
	  o<<"."<<ENDL; //Conclusion
	}
      else
	{
	  o<<", ";
	}
    }
  unlock;
}
void InterfaceMoche::ecart_refuse(std::vector<Carte> ecart)
{
  lock;
  ENTER("ecart_refuse(std::vector<Carte> ecart)");
  ADD_ARG("ecart", ecart);
  o<<"Vous n'avez pas réussi à écarter ";
  for(unsigned int i = 0 ; i < ecart.size() ; i++)
    {
      o<<QString::fromUtf8(ecart[i].nom().c_str());
      if(i == ecart.size() - 2)
	{
	  o<<", et ";
	}
      else if(i == ecart.size() - 1)
	{
	  o<<"."<<ENDL; //Conclusion
	}
      else
	{
	  o<<", ";
	}
    }
  unlock;
}
void InterfaceMoche::atout_au_chien(std::vector<Carte> atouts)
{
  lock;
  ENTER("atout_au_chien(std::vector<Carte> atouts)");
  ADD_ARG("atouts", atouts);
  o<<"Les atouts suivants : ";
  for(unsigned int i = 0 ; i < atouts.size() ; i++)
    {
      o<<QString::fromUtf8(atouts[i].nom().c_str());
      if(i == atouts.size() - 2)
	{
	  o<<", et ";
	}
      else if(i == atouts.size() - 1)
	{
	  o<<" vont au chien."<<ENDL; //Conclusion
	}
      else
	{
	  o<<", ";
	}
    }
  unlock;
}
void InterfaceMoche::maitre_change(unsigned int joueur)
{
  lock;
  ENTER("maitre_change(unsigned int joueur)");
  ADD_ARG("joueur", joueur);
  o<<"Le joueur maître est maintenant "
   <<m_adversaires[joueur]<<"."<<ENDL;
  unlock;
}
void InterfaceMoche::jeu_change(std::vector<Carte> gagnees,
				std::vector<Carte> perdues)
{
  lock;
  ENTER("jeu_change(std::vector<Carte> gagnees, std::vector<Carte> perdues)");
  ADD_ARG("gagnees", gagnees);
  ADD_ARG("perdues", perdues);
  QVector<QString> g, p;
  for(unsigned int i = 0 ; i < gagnees.size() ; i++)
    {
      g.append(QString::fromUtf8(gagnees[i].nom().c_str()));
      mon_jeu.append(QSharedPointer<Carte>(new Carte(gagnees[i])));
    }
  for(unsigned int i = 0 ; i < perdues.size() ; i++)
    {
      p.append(QString::fromUtf8(perdues[i].nom().c_str()));
      for(int j = 0 ; j < mon_jeu.size() ; j++)
	{
	  if(*(mon_jeu[j]) == perdues[i])
	    {
	      mon_jeu.remove(j);
	      j = mon_jeu.size();
	    }
	}
    }
  if(g.size() != 0)
    {
      o<<"Vous avez reçu : ";
      for(int i = 0 ; i < g.size() ; i++)
	{
	  o<<g[i];
	  if(i == g.size() - 2)
	    {
	      o<<", et ";
	    }
	  else if(i == g.size() - 1)
	    {
	      o<<"."<<ENDL; //Conclusion
	    }
	  else
	    {
	      o<<", ";
	    }
	}
    }
  if(p.size() != 0)
    {
      o<<"Vous vous séparez de : ";
      for(int i = 0 ; i < p.size() ; i++)
	{
	  o<<p[i];
	  if(i == p.size() - 2)
	    {
	      o<<", et ";
	    }
	  else if(i == p.size() - 1)
	    {
	      o<<"."<<ENDL; //Conclusion
	    }
	  else
	    {
	      o<<", ";
	    }
	}
    }
  unlock;
}
void InterfaceMoche::jeu_est(std::vector<Carte> cartes)
{
  lock;
  ENTER("jeu_est(std::vector<Carte> cartes)");
  ADD_ARG("cartes", cartes);
  QVector<QString> jeu;
  mon_jeu.clear();
  for(unsigned int i = 0 ; i < cartes.size() ; i++)
    {
      jeu.append(QString::fromUtf8(cartes[i].nom().c_str()));
      mon_jeu.append(QSharedPointer<Carte>(new Carte(cartes[i])));
    }
  o<<"Vous disposez de : ";
  for(int i = 0 ; i < jeu.size() ; i++)
    {
      o<<jeu[i];
      if(i == jeu.size() - 2)
	{
	  o<<", et ";
	}
      else if(i == jeu.size() - 1)
	{
	  o<<"."<<ENDL; //Conclusion
	}
      else
	{
	  o<<", ";
	}
    }
  unlock;
}
void InterfaceMoche::doit_jouer()
{
  lock;
  ENTER("doit_jouer()");
  o<<"Vous devez jouer. "<<ENDL;
  unlock;
}
void InterfaceMoche::requete_acceptee(Carte posee)
{
  lock;
  ENTER("requete_acceptee(Carte posee)");
  ADD_ARG("posee", posee);
  o<<"Vous avez réussi à jouer "
   <<QString::fromUtf8(posee.nom().c_str())<<"."<<ENDL;
  unlock;
}
void InterfaceMoche::requete_refusee(Carte posee)
{
  lock;
  ENTER("requete_refusee(Carte posee)");
  ADD_ARG("posee", posee);
  o<<"Vous n'avez pas réussi à jouer "
   <<QString::fromUtf8(posee.nom().c_str())<<"."<<ENDL;
  unlock;
}
void InterfaceMoche::carte_jouee(unsigned int joueur, Carte carte)
{
  lock;
  ENTER("carte_jouee(unsigned int joueur, Carte carte)");
  ADD_ARG("joueur", joueur);
  ADD_ARG("carte", carte);
  o<<m_adversaires[joueur]<<" joue " 
   <<QString::fromUtf8(carte.nom().c_str())<<"."<<ENDL;
  unlock;
}
void InterfaceMoche::carte_gagnee(Carte c, unsigned int poseur,
				  unsigned int gagnant)
{
  lock;
  ENTER("carte_gagnee(Carte c, unsigned int poseur, unsigned int gagnant)");
  ADD_ARG("c", c);
  ADD_ARG("poseur", poseur);
  ADD_ARG("gagnant", gagnant);
  o<<m_adversaires[gagnant]<<" remporte "<<
    QString::fromUtf8(c.nom().c_str())<<
    " (carte posée par "<<m_adversaires[poseur]<<")."<<ENDL;
  unlock;
}
void InterfaceMoche::pli_termine(unsigned int joueur)
{
  lock;
  ENTER("pli_termine(unsigned int joueur)");
  ADD_ARG("joueur", joueur);
  o<<m_adversaires[joueur]<<" remporte le pli."<<ENDL;
  unlock;
}
void InterfaceMoche::tapis_change(Tapis tapis)
{
  lock;
  ENTER("tapis_change(Tapis tapis)");
  ADD_ARG("tapis", tapis);
  std::vector<Carte> posees;
  std::vector<unsigned int> poseurs;
  tapis.contenu(posees, poseurs);
  QVector<QString> assertions;
  for(unsigned int i = 0 ; i < posees.size() ; i++)
    {
      assertions.append(m_adversaires[poseurs[i]] + " a posé "
			+ QString::fromUtf8
			(posees[i].nom().c_str()));
    }
  if(assertions.size() == 0)
    {
      o<<"Un nouveau pli commence."<<ENDL;
    }
  else
    {
      o<<"Pli en cours : ";
      for(int i = 0 ; i < assertions.size() ; i++)
	{
	  o<<assertions[i];
	  if(i == assertions.size() - 2)
	    {
	      o<<", et "<<ENDL;
	    }
	  else if(i == assertions.size() - 1)
	    {
	      o<<".";
	    }
	  else
	    {
	      o<<", ";
	    }
	}
    }
  unlock;
}
void InterfaceMoche::partie_terminee(std::vector<int> scores)
{
  lock;
  ENTER("partie_terminee(std::vector<int> scores)");
  ADD_ARG("scores", scores);
  for(unsigned int i = 0 ; i < scores.size() ; i++)
    {
      o<<m_adversaires[i]<<" fait un score de "<<scores[i]
       <<"."<<ENDL;
    }
  unlock;
}

std::istream & operator>>(std::istream & in, QString & str)
{
  std::string s;
  in>>s;
  str = QString::fromUtf8(s.c_str());
  return in;
} 

void InterfaceMoche::run()
{
  ENTER("run()");
  int choix;
  bool b;
  lock; b = continuer; unlock;
  while(b && std::cin)
    {
      std::ostream & o = std::cout;
      lock;
      o<<"Entrez l'une de ces commandes :"<<ENDL;
      o<<"* -> Quitter"      <<ENDL;
      o<<"1 -> Identifier"   <<ENDL;
      o<<"2 -> Inviter"      <<ENDL;
      o<<"3 -> Priser"       <<ENDL;
      o<<"4 -> Appeler"      <<ENDL;
      o<<"5 -> Écarter"      <<ENDL;
      o<<"6 -> Jouer"        <<ENDL;
      o<<"7 -> Reconnecter"  <<ENDL;
      o<<"8 -> audit"        <<ENDL;
      unlock;
      std::cin>>choix;
      QString tmp;
      if(std::cin)
	switch(choix)
	  {
	  case 0: emit doit_quitter(); break;
	  case 1:
	    lock;
	    o<<"Entrez votre nom :"<<ENDL;
	    unlock;
	    std::cin>>tmp;
	    lock; mon_nom = tmp; unlock;
	    if(std::cin)
	      {
		lock;
		o<<"Voulez-vous vous identifier comme \""
		 <<mon_nom<<"\" ?"<<ENDL;
		o<<"0 / autre -> Non" <<ENDL;
		o<<"1         -> Oui" <<ENDL;
		unlock;
		std::cin>>choix;
		if(choix == 1 && std::cin) 
		  {
		    emit formuler_identification
		      (std::string(mon_nom.toUtf8().data()));
		  }
	      }
	    break;
	  case 2:
	    if(true)
	      {
		lock; 
		QVector<QString> copie = vestibule; 
		o<<"Voici les gens dans le vestibule :"<<ENDL;
		o<<"0 / autre -> annuler"<<ENDL;
		for(int i = 0 ; i < copie.size() ; i++)
		  {
		    o<<(i + 1)<<" -> "<<copie[i]<<ENDL;
		  }
		o<<copie.size() + 1<<" -> un autre type"<<ENDL;
		o<<"Quels numéros voulez-vous inviter ?"<<ENDL;
		unlock;
		std::vector<std::string> liste;
		choix = 1;
		for(int i = 0 ; i < 4 && choix != 0 && std::cin ; i++)
		  {
		    std::cin>>choix;
		    if(choix > 0 && choix <= copie.size() && std::cin)
		      {
			liste.push_back
			  (std::string(copie[choix - 1]
				       .toUtf8().data()));
		      }
		    else if(choix == copie.size() + 1 && std::cin)
		      {
			lock;
			o<<"Entrez le nom de la personne, tel quel :"
			 <<ENDL;
			unlock;
			QString nom;
			std::cin>>nom;
			liste.push_back
			  (std::string(nom.toUtf8().data()));
		      }
		    else
		      {
			choix = 0;
			lock; o<<"Annulation."<<ENDL; unlock;
		      }
		  }
		if(choix != 0 && std::cin)
		  {
		    lock;
		    o<<"Vous êtes sur le point d'inviter "<<liste
		     <<ENDL;
		    o<<"Vous confirmez ?"<<ENDL;
		    o<<"* -> non"<<ENDL;
		    o<<"1 -> oui"<<ENDL;
		    unlock;
		    std::cin>>choix;
		    if(choix == 1 && std::cin)
		      {
			emit formuler_invitation(liste);
		      }
		  }
	      }
	    break;
	  case 3:
	    lock;
	    o<<"Quelle prise souhaiteriez-vous faire ?"<<ENDL;
	    o<<"0 -> passer"<<ENDL;
	    o<<"1 -> priser"<<ENDL;
	    o<<"2 -> garder"<<ENDL;
	    o<<"3 -> garder sans le chien"<<ENDL;
	    o<<"4 -> garder contre le chien"<<ENDL;
	    o<<"Vous pouvez tester d'autres nombres."<<ENDL;
	    unlock;
	    std::cin>>choix;
	    if(std::cin) 
	      {
		emit formuler_prise(static_cast<Enchere::Prise>(choix - 1));
	      }
	    break;
	  case 4:
	    lock;
	    o<<"Quelle carte voulez-vous appeler ?"<<ENDL;
	    for(int i = 0 ; i < appelables.size() ; i++)
	      {
		o<<appelables[i]->numero()<<" -> "
		 <<appelables[i]->nom()<<ENDL;
	      }
	    o<<"Vous pouvez tester d'autres nombres."<<ENDL;
	    unlock;
	    std::cin>>choix;
	    if(std::cin)
	      {
		emit formuler_appel(Carte(choix));
	      }
	    break;
	  case 5:
	    lock;
	    o<<"Quelles cartes voulez-vous écarter ?"<<ENDL;
	    for(int i = 0 ; i < appelables.size() ; i++)
	      {
		o<<appelables[i]->numero()<<" -> "
		 <<appelables[i]->nom()<<ENDL;
	      }
	    o<<"Vous pouvez tester d'autres nombres."<<ENDL;
	    unlock;
	    if(true)
	      {
		std::vector<Carte> selectionnees(3, 0);
		for(unsigned int i = 0 ; i < 3 && std::cin ; i++)
		  {
		    std::cin>>choix;
		    selectionnees[i] = Carte(choix);
		  }
		if(std::cin)
		  {
		    lock;
		    o<<"Vous êtes sur le point d'écarter "
		     <<selectionnees[0].nom()<<", "
		     <<selectionnees[1].nom()<<", "
		     <<selectionnees[2].nom()<<". Continuer ?"<<std::endl;
		    o<<"* -> non"<<ENDL;
		    o<<"1 -> oui"<<ENDL;
		    unlock;
		    std::cin>>choix;
		    if(std::cin && choix == 1)
		      {
			emit formuler_ecart(selectionnees);
		      }
		  }
	      }
	    break;
	  case 6:
	    lock;
	    o<<"Quelle carte voulez-vous jouer ?"<<ENDL;
	    for(int i = 0 ; i < mon_jeu.size() ; i++)
	      {
		o<<mon_jeu[i]->numero()<<" -> "
		 <<mon_jeu[i]->nom()<<ENDL;
	      }
	    o<<"Vous pouvez tester d'autres nombres."<<ENDL;
	    unlock;
	    std::cin>>choix;
	    if(std::cin)
	      {
		lock;
		o<<"Vous êtes prêt à jouer "<<Carte(choix).nom()
		 <<". Continuer ?"<<ENDL;
		o<<"* -> non"<<ENDL;
		o<<"1 -> oui"<<ENDL;
		unlock;
		std::cin>>choix;
		if(std::cin && choix == 1)
		  {
		    emit formuler_requete(Carte(choix));
		  }
	      }
	    break;
	  case 7:
	    lock;
	    o<<"Voulez-vous changer de nom ?"<<ENDL;
	    o<<"* -> annuler"<<ENDL;
	    o<<"1 -> oui"<<ENDL;
	    o<<"2 -> non"<<ENDL;
	    unlock;
	    std::cin>>choix;
	    if(std::cin && choix == 1)
	      {
		lock; mon_nom = ""; unlock;
	      }
	    if(std::cin && (choix == 1 || choix == 2))
	      {
		emit deconnecter();
		emit reconnecter();
	      }
	    break;
	  case 8:
	    lock;
	    o<<"Pas encore implémenté."<<ENDL;
	    unlock;
	    break;
	  default:
	    break;
	  }
      lock; b = continuer; unlock;
    }
  if(continuer)
    {
      o<<"Doit quitter."<<ENDL;
      emit doit_quitter();
    }
}

std::ostream & operator<<(std::ostream & out, const QString & str)
{
  std::string s(str.toUtf8().data());
  return out << s;
}
