#include "tapis.hpp"

#define NOM_CLASSE "Tapis"

#include "deboguer.hpp"

Tapis::Tapis():joueur_ouverture(-1), joueur_maitre(-1), 
	       maitre_fixe(false)
{
  ENTER("Tapis()");
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
	    poseurs.push_back(i);
	    gagnants.push_back(i);
	  }
      //Assignation des cartes au gagnant :
      for(unsigned int i = 0 ; i < posees.size() ; i++)
	{
	  if(posees[i] != EXCUSE || exc == Carte::EXCUSE_PRENABLE)
	    gagnants[i] = (maitre_final + joueur_ouverture) % 5;
	}
      joueur_ouverture = (maitre_final + joueur_ouverture) % 5;
      m_tapis.clear();
      //La variable joueur_maitre doit avoir un sens dans la chaîne
      //d'appels de cartes_gagnees (serveur)
      //Le tapis doit être vide pour que l'afficheur affiche la bonne
      //chose (client)
      cartes_gagnees(posees, poseurs, gagnants);
      joueur_maitre = -1;
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
  if(m_tapis.size() == 0) EXIT(false);
  if(m_tapis[0][0] == EXCUSE && m_tapis.size() == 2) EXIT(false);
  if(m_tapis[0][0] == EXCUSE) c = m_tapis[1][0];
  else c = m_tapis[0][0];
  DEBUG<<"Entame : "<<c<<std::endl;
  EXIT(true);
}

bool Tapis::maitre(unsigned int & j) const
{
  ENTER("entame(unsigned int & j) const");
  if(joueur_maitre < 0) EXIT(false);
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
  for(unsigned int i = 0 ; i < 5 ; i++)
    {
      //On ajoute la carte du joueur joueur_ouverture + i
      for(unsigned int j = 0 ; j < m_tapis[i].size() ; j++)
	{
	  if(m_tapis[i][j] != DETTE_EXCUSE)
	    {
	      DEBUG<<"Dans le tapis, "<<(i + joueur_ouverture) % 5
		   <<" a joué la carte "<<m_tapis[i][j]<<std::endl;;
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
