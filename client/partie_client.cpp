#include "partie_client.hpp"

#define NOM_CLASSE "PartieClient"

#include "deboguer.hpp"

#include "option.cpp"

PartieClient::PartieClient(QObject * parent):
  QObject(parent), Partie(), m_mon_tour(5),
  m_doit_priser(false), m_doit_appeler(false),
  m_doit_ecarter(false), m_doit_jouer(false)
{
  ENTER("PartieClient(QObject * parent)");
  ADD_ARG("parent", parent);
  connect(this, SIGNAL(doit_emettre(Protocole::Message)),
	  this, SLOT(assimiler(Protocole::Message)));
}

bool PartieClient::mon_tour() const
{
  return tour() == m_mon_tour;
}

unsigned int PartieClient::mon_numero() const
{
  ENTER("mon_numero() const");
  EXIT(m_mon_tour);
}

void PartieClient::assimiler(const Protocole::Message & m)
{
  ENTER("assimiler(Message m)");
  ADD_ARG("m", m);
  //DEBUG<<"Présentation avant traitement : "<<std::endl;
  //presenter();
  Partie::assimiler(m);
  switch(m.type)
    {
    case Protocole::ERREUR_PROTOCOLE:
      annuler_transaction();
      break;
    case Protocole::REFUSE:
      annuler_transaction();
      break;
    case Protocole::NUMERO:
      m_mon_tour = m.m.numero.n;
      emit numero_change(m_mon_tour);
      break;
    case Protocole::DISTRIBUTION:
      mes_cartes = Main(m.m.distribution);
      set_phase(ENCHERES);
      emit jeu_change(std::vector<Carte>(mes_cartes.cartes()), 
		      std::vector<Carte>());
      if(m_mon_tour == 0) 
	{
	  m_doit_priser = true;
	  emit doit_priser(Option<Enchere>());
	}
      break;
    case Protocole::PRISE:
      ajouter_transaction_prise(m.m.prise.niveau);
      break;
    case Protocole::CONTRAT:
      emit contrat_intermediaire
	(Enchere(tour_precedent(), m.m.contrat));
      if(mon_tour() && phase() == ENCHERES) 
	{
	  m_doit_priser = true;
	  emit doit_priser(Option<Enchere>(meilleure_enchere()));
	}
      else if(m_mon_tour == tour_precedent() && phase() == ENCHERES)
	{ 
	  transaction_acceptee();
	}
      break;
    case Protocole::APPEL:
      if(true)
	{
	  m_doit_appeler = true;
	  emit doit_appeler(cartes_appelables());
	}
      break;
    case Protocole::APPELER:
      ajouter_transaction_appel(m.m.appeler.carte);
      break;
    case Protocole::CONTRAT_FINAL:
      // L'appel a été acepté.
      transaction_acceptee();
      emit contrat_final(Enchere(m.m.contrat_final));
      if(m.m.contrat_final.niveau >= Enchere::GARDE_SANS)
	{
	  // On ne parle pas du chien, donc c'est à nous !
	  emit doit_demander_chelem();
	}
      break;
    case Protocole::CHIEN:
      chien_si_devoile.clear();
      for(unsigned int i = 0 ; i < 3 ; i++)
	chien_si_devoile.push_back(Carte(m.m.chien.chien[i]));
      emit chien_devoile(chien_si_devoile[0],
			 chien_si_devoile[1],
			 chien_si_devoile[2]);
      if(attaquant() == m_mon_tour)
	{
	  //Je dois écarter !
	  for(unsigned int i = 0 ; i < 3 ; i++)
	    {
	      mes_cartes.ajouter(chien_si_devoile[i]);
	    }
	  emit jeu_change(std::vector<Carte>(chien_si_devoile),
			  std::vector<Carte>());
	  std::vector<std::vector<Carte> > ecartables =
	    cartes_ecartables();
	  m_doit_ecarter = true;
	  emit doit_ecarter(ecartables[0], ecartables[1]);
	}
      break;
    case Protocole::ECART:
      ajouter_transaction_ecart(m.m.ecart.ecart);
      break;
    case Protocole::ATOUT:
      if(true)
	{
	  std::vector<Carte> atouts;
	  for(int i = 0 ; i < m.m.atout.nombre && i < 3 ; i++)
	    {
	      atouts.push_back(Carte(m.m.atout.cartes[i]));
	    }
	  emit atout_au_chien(atouts);
	}
      break;
    case Protocole::CHELEM:
      break;
    case Protocole::JEU:
      if(mon_numero() == attaquant())
	{
	  //L'écart a été accepté.
	  transaction_acceptee();
	}
      if(mon_tour())
	{
	  m_doit_jouer = true;
	  emit doit_jouer();
	}
      DEBUG<<"Commencement de la partie."<<std::endl;
      //presenter();
      break;
    case Protocole::MONTRER_POIGNEE:
      break;
    case Protocole::POIGNEE:
      break;
    case Protocole::REQUETE:
      ajouter_transaction_jeu(m.m.requete.carte);
      break;
    case Protocole::CARTE:
      if(m_mon_tour == tour_precedent())
	{
	  transaction_acceptee();
	}
      emit carte_jouee(tour_precedent(), Carte(m.m.carte.carte));
      if(mon_tour()) 
	{
	  m_doit_jouer = true;
	  emit doit_jouer();
	}
      emit tapis_change(tapis());
      break;
    case Protocole::PLI:
      /* 
      if(mon_tour()) 
	{
	  m_doit_jouer = true;
	  emit doit_jouer();
	  }
      //*/ //Le test est fait dans le cas CARTE.

      //Normalement, Protocole::Carte vient d'être analysé, et le
      //prochain joueur sait déjà qu'il doit jouer.
      //if(mon_tour()) emit doit_jouer();
      //Le mouvement des cartes est traité dans la méthode virtuelle
      //dédiée.
      break;
    case Protocole::RESULTAT:
      if(true)
	{
	  std::vector<int> scores;
	  for(unsigned int i = 0 ; i < 5 ; i++)
	    {
	      scores.push_back(m.m.resultat.resultats[i]);
	    }
	  emit score(scores);
	}
      break;
    default:
      break;
    }
}

void PartieClient::montrer_poignee(std::vector<Carte> const & p)
{
  ENTER("montrer_poignee(std::vector<Carte> p)");
  ADD_ARG("p", p);
  Protocole::Message m;
  m.type = Protocole::MONTRER_POIGNEE;
  m.m.montrer_poignee.taille = p.size();
  for(unsigned int i = 0 ; i < p.size() ; i++)
    {
      m.m.montrer_poignee.atouts[i] = p[i].numero();
    }
  emit doit_emettre(m);
}

void PartieClient::jouer(const Carte & c)
{
  ENTER("jouer(const Carte & c)");
  ADD_ARG("c", c);
  Protocole::Message m;
  m.type = Protocole::REQUETE;
  m.m.requete.carte = c.numero();
  emit doit_emettre(m);
}

void PartieClient::formuler_prise(Enchere::Prise p)
{
  ENTER("formuler_prise(Prise p)");
  ADD_ARG("p", p);
  Protocole::Message m;
  m.type = Protocole::PRISE;
  m.m.prise.niveau = (int)p;
  emit doit_emettre(m);
}

void PartieClient::appeler(const Carte & c)
{
  ENTER("appeler(const Carte & c)");
  ADD_ARG("c", c);
  Protocole::Message m;
  m.type = Protocole::APPELER;
  m.m.appeler.carte = c.numero();
  emit doit_emettre(m);
}

void PartieClient::ecarter(std::vector<Carte> const & c)
{
  ENTER("ecarter(vector<Carte> c)");
  ADD_ARG("c", c);
  Protocole::Message m;
  m.type = Protocole::ECART;
  for(unsigned int i = 0 ; i < 3 && i < c.size() ; i++)
    {
      m.m.ecart.ecart[i] = c[i].numero();
    }
  emit doit_emettre(m);
}

void PartieClient::demander_chelem()
{
  ENTER("demander_chelem()");
}

void PartieClient::demander_jeu()
{
  ENTER("demander_jeu()");
}

const Main & PartieClient::mon_jeu() const
{
  ENTER("mon_jeu() const");
  EXIT(mes_cartes);
}

void PartieClient::changement_maitre(unsigned int,
				     unsigned int nouveau)
{
  ENTER("changement_maitre(unsigned int, unsigned int nouveau)");
  ADD_ARG("nouveau", nouveau);
  emit maitre(nouveau);
}

void PartieClient::nouveau_maitre(unsigned int maitre)
{
  ENTER("nouveau_maitre(unsigned int maitre)");
  ADD_ARG("maitre", maitre);
  emit PartieClient::maitre(maitre);
}

void PartieClient::cartes_gagnees
(std::vector<Carte> const & cartes,
 std::vector<unsigned int> const & poseurs,
 std::vector<unsigned int> const & gagnants)
{
  ENTER("cartes_gagnees(vector<Carte> cartes, vector<uint> poseurs, vector<uint> gagnants)");
  ADD_ARG("cartes", cartes);
  ADD_ARG("poseurs", poseurs);
  ADD_ARG("gagnants", gagnants);
  for(unsigned int i = 0 ; i < cartes.size() ; i++)
    {
      emit carte_gagnee(cartes[i], poseurs[i], gagnants[i]);
    }
}

void PartieClient::annuler_transaction()
{
  ENTER("annuler_transaction()");
  if(en_cours.empty())
    {
      //Euh... Problème de protocole.
      DEBUG<<"Aucune transaction n'a été annulée..."<<std::endl;
    }
  else
    {
      Transaction t = en_cours.front();
      en_cours.pop();
      DEBUG<<"Annulation d'une transaction..."<<std::endl;
      if(!(t.enchere().aucun()))
	{
	  DEBUG<<"Annulation d'une enchère."<<std::endl;
	  //C'est une enchère !
	  Enchere enchere = t.enchere().get().obtenir().get();
	  emit enchere_refusee(enchere);
	  if(m_doit_priser)
	    {
	      DEBUG<<"Je relance le joueur..."<<std::endl;
	      //Je dois reformuler mon enchère.
	      Enchere indice = t.enchere().get().indice().get();
	      emit doit_priser(indice);
	    }
	}
      if(!(t.appel().aucun()))
	{
	  DEBUG<<"Annulation d'un appel."<<std::endl;
	  //C'est un appel !
	  Carte appelee = t.appel().get().obtenir().get();
	  emit appel_refuse(appelee);
	  if(m_doit_appeler)
	    {
	      DEBUG<<"Je relance le joueur..."<<std::endl;
	      //Je dois reformuler mon appel.
	      std::vector<Carte> indice = 
		t.appel().get().indice().get();
	      emit doit_appeler(indice);
	    }
	}
      if(!(t.ecart().aucun()))
	{
	  DEBUG<<"Annulation d'un écart."<<std::endl;
	  //C'est un écart refusé.
	  std::vector<Carte> ecart = t.ecart().get().obtenir().get();
	  emit ecart_refuse(ecart);
	  if(m_doit_ecarter)
	    {
	      DEBUG<<"Je relance le joueur..."<<std::endl;
	      std::vector<Carte> possibles = 
		t.ecart().get().indice1().get();
	      std::vector<Carte> atouts = 
		t.ecart().get().indice2().get();
	      emit doit_ecarter(possibles, atouts);
	    }
	}
      if(!(t.jeu().aucun()))
	{
	  DEBUG<<"Annulation du jeu d'une carte."<<std::endl;
	  //Je dois jouer une autre carte.
	  Carte carte = t.jeu().get().obtenir().get();
	  emit requete_refusee(carte);
	  if(m_doit_jouer)
	    {
	      DEBUG<<"Je relance le joueur..."<<std::endl;
	      emit doit_jouer();
	    }
	}
    }
}
void PartieClient::transaction_acceptee()
{
  ENTER("transaction_acceptee()");
  if(en_cours.empty())
    {
      //Euh...
    }
  else
    {
      Transaction t = en_cours.front();
      en_cours.pop();
      if(!(t.enchere().aucun()))
	{
	  //C'est une enchère !
	  Enchere enchere = t.enchere().get().obtenir().get();
	  emit enchere_acceptee(enchere);
	  m_doit_priser = false;
	}
      if(!(t.appel().aucun()))
	{
	  //C'est un appel !
	  Carte appelee = t.appel().get().obtenir().get();
	  emit appel_accepte(appelee);
	  m_doit_appeler = false;
	}
      if(!(t.ecart().aucun()))
	{
	  std::vector<Carte> ecart = t.ecart().get().obtenir().get();
	  emit ecart_accepte(ecart);
	  for(unsigned int i = 0 ; i < ecart.size() ; i++)
	    mes_cartes.enlever(ecart[i]);
	  emit jeu_change(std::vector<Carte>(), ecart);
	  m_doit_ecarter = false;
	}
      if(!(t.jeu().aucun()))
	{
	  Carte carte = t.jeu().get().obtenir().get();
	  emit requete_acceptee(carte);
	  std::vector<Carte> temp;
	  temp.push_back(carte);
	  mes_cartes.enlever(carte);
	  emit jeu_change(std::vector<Carte>(), temp);
	  m_doit_jouer = false;
	}
    }
}
std::vector<Carte> PartieClient::cartes_appelables() const
{
  std::vector<Carte> acceptables;
  for(unsigned int i = 0 ; i < 78 ; i++)
    {
      if(mes_cartes.peut_appeler(Carte(i)))
	acceptables.push_back(Carte(i));
    }
  return acceptables;
}
std::vector<std::vector<Carte> > 
PartieClient::cartes_ecartables() const
{
  std::vector<Carte> ecartables;
  std::vector<Carte> atouts;
  for(unsigned int i = 0 ; i < 78 ; i++)
    {
      Carte c(i);
      if(mes_cartes.possede(c))
	{
	  switch(c.ecartable())
	    {
	    case Carte::ECARTABLE:
	      ecartables.push_back(c);
	      break;
	    case Carte::MONTRER_CARTE:
	      atouts.push_back(c);
	      break;
	    default:
	      break;
	    }
	}
    }
  std::vector<std::vector<Carte> > resultat;
  resultat.push_back(ecartables);
  resultat.push_back(atouts);
  return resultat;
}
void PartieClient::ajouter_transaction_prise(unsigned int prise)
{
  ENTER("ajouter_transaction_prise(unsigned int prise)");
  ADD_ARG("prise", prise);
  Protocole::Msg_prise m;
  m.niveau = prise;
  Enchere e(tour(), m);
  Transaction::Enchere ench(e, meilleure_enchere());
  Transaction t(ench);
  en_cours.push(t);
}
void PartieClient::ajouter_transaction_appel(unsigned int carte)
{
  Carte c(carte);
  Transaction::Appel a(c, cartes_appelables());
  Transaction t(a);
  en_cours.push(t);
}
void PartieClient::ajouter_transaction_ecart
(const int ecart[3])
{
  std::vector<Carte> ec;
  for(unsigned int i = 0 ; i < 3 ; i++)
    {
      ec.push_back(Carte(static_cast<int>(ecart[i])));
    }
  std::vector<std::vector<Carte> > possibles = cartes_ecartables();
  Transaction::Ecart e(ec, possibles[0], possibles[1]);
  Transaction t(e);
  en_cours.push(t);
}
void PartieClient::ajouter_transaction_jeu(unsigned int carte)
{
  Carte c(carte);
  Transaction::Jeu j(c);
  Transaction t(j);
  en_cours.push(t);
}
void PartieClient::presenter() const
{
  ENTER("presenter() const");
  for(unsigned int i = 0 ; i < 5 ; i++)
    {
      try
	{
	  DEBUG<<"Enchère de "<<i<<" : "
	       <<enchere_de(i)<<std::endl;
	}
      catch(...)
	{
	}
    }
  DEBUG<<"chelem : "<<chelem()<<std::endl;
  DEBUG<<"attaquant : "<<attaquant()<<std::endl;
  DEBUG<<"tour : "<<tour()<<std::endl;
  DEBUG<<"tour précédent : "<<tour_precedent()<<std::endl;
  for(unsigned int i = 0 ; i < 5 ; i++)
    {
      DEBUG<<"taille de la poignée de "<<i<<" : "
	   <<poignee(i)<<std::endl;
    }
  DEBUG<<"phase : "<<phase()<<std::endl;
  DEBUG<<"tapis : "<<tapis()<<std::endl;
  DEBUG<<"meilleure enchère : "<<meilleure_enchere()<<std::endl;
  DEBUG<<"mon numéro de tour : "<<m_mon_tour<<std::endl;
  DEBUG<<"mes cartes : "<<mes_cartes<<std::endl;
  DEBUG<<"le chien (si dévoilé) : "<<chien_si_devoile<<std::endl;
  if(en_cours.empty())
    DEBUG<<"pas de transaction en cours."<<std::endl;
  else
    DEBUG<<"transaction en cours. "<<std::endl;
  DEBUG<<"si je dois priser : "<<m_doit_priser<<std::endl;
  DEBUG<<"si je dois appeler une carte : "<<m_doit_appeler<<std::endl;
  DEBUG<<"si je dois écarter : "<<m_doit_ecarter<<std::endl;
  DEBUG<<"si je dois jouer : "<<m_doit_jouer<<std::endl;
  DEBUG<<"si c'est mon tour : "<<mon_tour()<<std::endl;
  DEBUG<<"les cartes appelables : "<<cartes_appelables()<<std::endl;
  DEBUG<<"les cartes écartables : "<<cartes_ecartables()<<std::endl;
}
