//Le nom de ce fichier est risqué...

#include "main.hpp"

#include <iostream>

#define NOM_CLASSE "Main"

#include "ne_pas_deboguer.hpp"

Main::Main()
{
  ENTER("Main()");
  m_cartes.reserve(18);
  //15 plus 3 pour le chien
}

Main::Main(const Main & m): m_cartes(m.m_cartes)
{
  ENTER("Main(const Main & m)");
  ADD_ARG("m.nombre_cartes()", m.nombre_cartes());
}

Main::Main(const Protocole::Msg_distribution & m)
{
  ENTER("Main(const Msg_distribution & m)");
  ADD_ARG("m.cartes[0]", m.cartes[0]);
  m_cartes.reserve(18);
  //Charge les 15 Cartes du message
  for(unsigned int i = 0 ; i < 15 ; i++)
    {
      m_cartes.push_back(m.cartes[i]);
    }
}

void Main::enlever(const Carte & carte)
{
  ENTER("enlever(const Carte & carte)");
  ADD_ARG("carte", carte);
  for(unsigned int i = 0;
      i < m_cartes.size() ; i++)
    {
      if(m_cartes[i] == carte)
	{
	  DEBUG<<"Carte trouvée en i="<<i<<std::endl;
	  m_cartes.erase(m_cartes.begin() + i);
	  i=m_cartes.size();
	}
    }
}

bool Main::possede(const Carte & carte) const
{
  ENTER("possede(const Carte & carte) const");
  ADD_ARG("carte", carte);
  std::vector<Carte>::const_iterator i = m_cartes.begin();
  for(i = m_cartes.begin();
      i != m_cartes.end() && *i != carte; i++);
  EXIT(i != m_cartes.end());
}

void Main::ajouter(const Carte & carte)
{
  ENTER("ajouter(const Carte & carte)");
  ADD_ARG("carte", carte);
  m_cartes.push_back(carte);
}

bool Main::peut_appeler(const Carte & carte) const
{
  ENTER("peut_appeler(const Carte & carte) const");
  ADD_ARG("carte", carte);
  //Si le joueur n'a pas tous les rois, il ne peut pas appeler
  //une dame.
  Carte::Valeur valeur_incomplete = Carte::ROI; 
  //La première valeur pour laquelle
  // il manque une carte dans la main du joueur.
  while(valeur_incomplete > carte.valeur() &&
	(
	 possede(Carte(Carte::PIQUE * 14 + valeur_incomplete)) &&
	 possede(Carte(Carte::TREFLE * 14 + valeur_incomplete))&&
	 possede(Carte(Carte::CARREAU* 14 + valeur_incomplete))&&
	 possede(Carte(Carte::COEUR * 14 + valeur_incomplete))))
    {
      DEBUG<<"Le joueur possède toutes les valeurs "
	   <<valeur_incomplete
	   <<std::endl;
      valeur_incomplete = (Carte::Valeur)((int)valeur_incomplete - 1);
    }
  bool b = carte.tete() && valeur_incomplete <= carte.valeur();
  if(!b)
    {
      if(!carte.tete())
	DEBUG<<"Erreur : ce n'est même pas une tête."<<std::endl;
      if(valeur_incomplete > carte.valeur())
	DEBUG<<"Erreur : avant d'appeler une Carte de valeur "
	     <<carte.valeur()
	     <<", il faut avoir toutes les Cartes de valeur "
	     <<valeur_incomplete<<std::endl;
    }
  EXIT(b);
}

bool Main::peut_declarer(const std::vector<Carte> & poignee) const
{
  ENTER("peut_declarer(const vector<Carte> & poignee) const");
  ADD_ARG("poignee", poignee);
  if(poignee.size() == 8 || poignee.size() == 10 || poignee.size() == 13)
    {
      //On possède toutes ces cartes ?
      bool possession = true;
      for(unsigned int i = 0 ; i < poignee.size() ; i++)
	{
	  if(!possede(poignee[i]))
	    {
	      possession = false;
	      i = poignee.size();
	    }
	}
      if(possession)
	{
	  //Sont-elles différentes ?
	  bool differentes = true;
	  for(unsigned int i = 0 ; i < poignee.size() ; i++)
	    for(unsigned int j = i + 1 ; j < poignee.size() ; j++)
	      {
		if(poignee[i] == poignee[j])
		  {
		    differentes = false;
		    i = poignee.size();
		    j = poignee.size();
		  }
	      }
	  if(differentes)
	    {
	      //Y a-t-il l'excuse ?
	      bool excuse = false;
	      for(unsigned int i = 0 ; i < poignee.size() ; i++)
		{
		  if(poignee[i]==EXCUSE)
		    {
		      excuse = true;
		      i = poignee.size();
		    }
		}
	      if(excuse)
		{
		  //Le joueur doit posséder exactement 
		  //poignee.size() atouts,
		  //puisque l'excuse est un atout.
		  unsigned int nombre_atouts = 0;
		  for(unsigned int i = 0 ; i < m_cartes.size() ; i++)
		    {
		      if(m_cartes[i].atout())nombre_atouts++;
		    }
		  EXIT(nombre_atouts == poignee.size());
		}
	      EXIT(true);
	    }
	  EXIT(false);
	}
      EXIT(false);
    }
  EXIT(false);
}

const Main & Main::operator=(const Main & m)
{
  ENTER("operator=(const Main & m)");
  ADD_ARG("m", m);
  m_cartes = m.m_cartes;
  EXIT(*this);
}

void Main::distribution(Protocole::Msg_distribution & m)
{
  ENTER("distribution(Msg_distribution & m)");
  //Remplit un Msg_distribution
  for(unsigned int i = 0 ; i < 15 && i < m_cartes.size() ; i++)
    {
      m.cartes[i] = m_cartes[i].numero();
    }
}

std::vector<Carte::ModaliteEcart> 
Main::peut_ecarter(const std::vector<Carte> & ecart) const
{
  ENTER("peut_ecarter(const vector<Carte> & ecart) const");
  ADD_ARG("ecart", ecart);
  std::vector<Carte::ModaliteEcart> resultat(ecart.size());
  std::vector<Carte> cartes_restantes = m_cartes;
  // On commence par traiter les cartes qui ne sont pas des atouts
  for(unsigned int i = 0 ; i < ecart.size() ; i++)
    {
      if(ecart[i].ecartable() == Carte::ECARTABLE)
	{
	  if(possede(ecart[i]))
	    resultat[i] = Carte::ECARTABLE;
	  else
	    resultat[i] = Carte::NON_ECARTABLE;
	}
      else if(ecart[i].ecartable() == Carte::NON_ECARTABLE)
	{
	  //On reporte cette information
	  resultat[i] = Carte::NON_ECARTABLE;
	}
    }
  //On regarde s'il reste des cartes écartables 
  //dans la main du bonhomme. Auquel cas, on refuse un atout
  //dans le chien.
  bool peut_atout = true;
  for(unsigned int i = 0 ; i < m_cartes.size() && peut_atout ; i++)
    {
      if(m_cartes[i].ecartable() == Carte::ECARTABLE)
	{
	  peut_atout = false;
	  //... sauf si on a déjà prévu d'écarter m_cartes[i]
	  for(unsigned int j = 0 ; j < ecart.size() ; j++)
	    {
	      if(ecart[j] == m_cartes[i])
		{
		  peut_atout = true;
		  j = ecart.size();
		}
	    }
	}
    }
  // On traite le cas des atouts
  for(unsigned int i = 0 ; i < ecart.size() ; i++)
    {
      if(ecart[i].ecartable() == Carte::MONTRER_CARTE)
	{
	  if(peut_atout)resultat[i] = Carte::MONTRER_CARTE;
	  else resultat[i] = Carte::NON_ECARTABLE;
	}
    }
  // On vérifie si on possède bien ces 3 cartes
  for(unsigned int i = 0 ; i < ecart.size() ; i++)
    {
      if(!possede(ecart[i])) resultat[i] = Carte::NON_ECARTABLE;
    }
  //On vérifie si elles sont toutes différentes
  for(unsigned int i = 0 ; i < ecart.size() ; i++)
    {
      for(unsigned int j = i + 1 ; j < ecart.size() ; j++)
	{
	  if(ecart[i] == ecart[j]) resultat[j] = Carte::NON_ECARTABLE;
	}
    }
  EXIT(resultat);
}

bool Main::peut_couper(Carte::Couleur col) const
{
  ENTER("peut_couper(Couleur col)");
  ADD_ARG("col", col);
  //Si on n'a plus de cette couleur, ou si c'est un atout.
  bool b = true;
  if(col == Carte::ATOUT)
    {
      b = true; //Inutile, en fait.
    }
  else
    for(unsigned int i = 0 ; i < m_cartes.size() ; i++)
      {
	if(m_cartes[i].couleur() == col)
	  {
	    b = false;
	    i = m_cartes.size();
	  }
      }
  EXIT(b);
}

bool Main::peut_pisser(Carte::Valeur atout_max) const
{
  ENTER("peut_pisser(Valeur atout_max) const");
  ADD_ARG("atout_max", atout_max);
  bool b = true;
  for(unsigned int i = 0 ; i < m_cartes.size() ; i++)
    {
      if(m_cartes[i].atout() && m_cartes[i].valeur() > atout_max)
	{
	  b = false ;
	  i = m_cartes.size();
	}
    }
  EXIT(b);
}

bool Main::peut_defausser() const
{
  ENTER("peut_defausser() const");
  bool b = true;
  for(unsigned int i = 0 ; i < m_cartes.size() ; i++)
    {
      if(m_cartes[i].atout()
	 && m_cartes[i] != EXCUSE)
	{
	  b = false;
	  i = m_cartes.size();
	}
    }
  EXIT(b);
}

unsigned int Main::nombre_cartes() const
{
  //ENTER("nombre_cartes() const");
  //EXIT(m_cartes.size());
  return m_cartes.size();
}

std::vector<Carte> const & Main::cartes() const
{
  //ENTER("cartes() const");
  //EXIT(m_cartes)
  return m_cartes;
}

std::ostream & operator<<(std::ostream & out, const Main & main)
{
  out<<"Main { "<<main.nombre_cartes()<<" cartes : ";
  const std::vector<Carte> & c = main.cartes();
  for(unsigned int i = 0 ; i < c.size() ; i++)
    {
      out<<c[i].nom()<<" ";
    }
  out<<"}";
  return out;
}
