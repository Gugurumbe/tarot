#include "partie.hpp"
#include "debogueur.hpp"
#include <iostream>

#define NOM_CLASSE "Partie"

#include "deboguer.hpp"

/**
   @brief Tapis personnalisé pour usage dans la classe Partie.
 */
class TapisPartie : public Tapis
{
public:
  /**
     @brief Constructeur par défaut
     @param parent La classe de Partie sur laquelle on va agir.
   */
  TapisPartie(Partie * parent);

protected:
  /**
     @brief Réaction personnalisée.
     
     Transmet l'information via 
     Partie::changement_maitre(unsigned int, unsigned int)

     @param ancien L'ancien joueur maître.
     @param nouveau Le nouveau joueur maître.
   */
  virtual void changement_maitre(unsigned int ancien, unsigned int nouveau);

  /**
     @brief Réaction personnalisée.
     
     Transmet l'information via
     Partie::nouveau_maitre(unsigned int)
     
     @param j Le nouveau joueur maître.
   */
  virtual void nouveau_maitre(unsigned int j);

  /**
     @brief Réaction personnalisée.
     
     Transmet l'information via 
     Partie::cartes_gagnees(std::vector<Carte> const &, std::vector<unsigned int> const &, std::vector<unsigned int> const &)
     @param c Les cartes en question.
     @param poseurs Le poseur de chaque carte.
     @param gagnants Le gagnant de chaque carte.
   */
  virtual void cartes_gagnees(std::vector<Carte> const & c,
			      std::vector<unsigned int> const & poseurs,
			      std::vector<unsigned int> const & gagnants);
  /**
     @brief Le tour de jeu change.
   */
  virtual void tour_change(unsigned int tour);
private:
  Partie * const m_parent;
};

Partie::Partie(): m_encheres(5), m_chelem(false), 
		  m_tapis(new TapisPartie(this)), 
		  m_attaquant(5),
		  m_tour(0), m_tour_precedent(5), plis_joues(0),
		  m_phase(CONSTITUTION_TABLE),
		  m_tailles_poignees(5, 0)
{
  ENTER("Partie()");
}

Partie::~Partie()
{
  ENTER("~Partie()");
  delete m_tapis;
}

const Enchere & Partie::donner_contrat_final() const
{
  ENTER("donner_contrat_final() const");
  /* assert(m_attaquant < 5); */
  EXIT(m_encheres[m_attaquant]);
}

const Enchere & Partie::enchere_de(unsigned int joueur) const
{
  ENTER("enchere_de(unsigned int joueur) const");
  ADD_ARG("joueur", joueur);
  EXIT(m_encheres[joueur]);
}

int Partie::chelem() const
{
  ENTER("chelem() const");
  EXIT(m_chelem);
}

unsigned int Partie::attaquant() const
{
  ENTER("attaquant() const");
  EXIT(m_attaquant);
}

void Partie::set_attaquant(unsigned int j)
{
  ENTER("set_attaquant(unsigned int j)");
  ADD_ARG("j", j);
  if(m_attaquant >= 5)
    {
      m_attaquant = j;
    }
}

unsigned int Partie::tour() const
{
  ENTER("tour() const");
  EXIT(m_tour);
}

unsigned int Partie::tour_precedent() const
{
  ENTER("tour_precedent() const");
  EXIT(m_tour_precedent);
}

unsigned int Partie::poignee(unsigned int joueur) const
{
  ENTER("poignee(unsigned int joueur const");
  ADD_ARG("joueur", joueur);
  EXIT(m_tailles_poignees[joueur]);
}

Partie::PhaseJeu Partie::phase() const
{
  ENTER("phase() const");
  EXIT(m_phase);
}

void Partie::set_phase(Partie::PhaseJeu p)
{
  ENTER("set_phase(PhaseJeu p)");
  ADD_ARG("p", p);
  m_phase = p;
}

void Partie::assimiler(const Protocole::Message & m)
{
  ENTER("assimiler(Message m)");
  ADD_ARG("m", m);
  switch(m.type)
    {
    case Protocole::ERREUR_PROTOCOLE:
      break;
    case Protocole::REFUSE:
      break;
    case Protocole::NUMERO:
      m_phase = CONSTITUTION_TABLE;
      break;
    case Protocole::DISTRIBUTION:
      m_phase = ENCHERES;
      break;
    case Protocole::PRISE:
      m_phase = ENCHERES;
      break;
    case Protocole::CONTRAT:
      // Le joueur dont c'est le tour a formulé le contrat suivant
      m_phase = ENCHERES;
      m_encheres[m_tour] = Enchere(m_tour, m.m.contrat);
      if(m.m.contrat.niveau > 0)
	{
	  // On suppose que cette enchère est la meilleure.
	  m_attaquant = m_tour;
	}
      m_tour_precedent = m_tour;
      m_tour = m_tour + 1; // pas modulo 5, car une fois que les 
      //enchères sont passées ce n'est plus le tour de personne.
      break;
    case Protocole::APPEL:
      m_phase = ENCHERES;
      break;
    case Protocole::APPELER:
      m_phase = ENCHERES;
      break;
    case Protocole::CONTRAT_FINAL:
      m_phase = CONSTITUTION_ECART;
      m_attaquant = m.m.contrat_final.preneur;
      m_encheres[m_attaquant] = m.m.contrat_final;
      break;
    case Protocole::CHIEN:
      m_phase = CONSTITUTION_ECART;
      break;
    case Protocole::ECART:
      m_phase = CONSTITUTION_ECART;
      break;
    case Protocole::ATOUT:
      m_phase = CONSTITUTION_ECART;
      //Il faut en informer le client !
      break;
    case Protocole::CHELEM:
      //?? à supprimer
      break;
    case Protocole::JEU:
      m_phase = PHASE_JEU;
      //Ça n'a pas bcp de sens, mais je le rajoute :
      m_tour_precedent = m_tour;
      //là, par contre, c'est importnt :
      m_tour = m.m.jeu.ouvreur;
      //Le joueur ayant demandé un chelem joue.
      m_tapis->set_ouverture(m_tour);
      break;
    case Protocole::POIGNEE:
      m_phase = PHASE_JEU;
      m_tailles_poignees[m_tour] = m.m.poignee.taille;
      break;
    case Protocole::REQUETE:
      m_phase = PHASE_JEU;
      break;
    case Protocole::CARTE:
      m_phase = PHASE_JEU;
      if(m_tour == attaquant() && 
	 m_chelem &&
	 m.m.carte.carte == EXCUSE && 
	 plis_joues == 14)
	m_tapis->ajouter(m.m.carte, Carte::EXCUSE_GAGNANTE);
      else if(plis_joues == 14)
	m_tapis->ajouter(m.m.carte, Carte::EXCUSE_PRENABLE);
      else
	m_tapis->ajouter(m.m.carte, Carte::EXCUSE_IMPRENABLE);
      break;
    case Protocole::PLI:
      m_phase = PHASE_JEU;
      if(m_tour != static_cast<unsigned int>(m.m.pli.joueur))
	{
	  std::cerr<<"Erreur : le tour selon moi ("
		   <<m_tour<<") est différent du tour selon le sereur ("
		   <<m.m.pli.joueur<<") !"<<std::endl;
	  exit(-1);
	}
      plis_joues++;
      break;
    case Protocole::RESULTAT:
      m_phase = FIN;
    default:
      break;
    }
  DEBUG<<"Phase : "<<m_phase<<", tour : "<<m_tour
       <<" (après "<<m_tour_precedent<<")."
       <<std::endl;;
}

const Tapis & Partie::tapis() const
{
  return *m_tapis;
}

void Partie::throw_changement_maitre(unsigned int ancien,
			       unsigned int nouveau)
{
  changement_maitre(ancien, nouveau);
}

void Partie::throw_nouveau_maitre(unsigned int maitre)
{
  nouveau_maitre(maitre);
}

void Partie::throw_tour_change(unsigned int tour)
{
  m_tour_precedent = m_tour;
  m_tour = tour;
}

void Partie::throw_cartes_gagnees
(std::vector<Carte> const & cartes,
 std::vector<unsigned int> const & poseurs,
 std::vector<unsigned int> const & gagnants)
{
  cartes_gagnees(cartes, poseurs, gagnants);
}

Enchere Partie::meilleure_enchere() const
{
  if(m_attaquant >= m_encheres.size())
    {
      Protocole::Msg_contrat c;
      c.niveau = 0;
      return Enchere(0, c);
    }
  // Dans les faits, l'attaquant est le dernier qui a effectué une
  // enchère validée par le serveur.
  return donner_contrat_final();
}

void Partie::changement_maitre(unsigned int, unsigned int)
{
}

void Partie::nouveau_maitre(unsigned int)
{
}

void Partie::cartes_gagnees(std::vector<Carte> const &,
			    std::vector<unsigned int> const &,
			    std::vector<unsigned int> const &)
{
}

TapisPartie::TapisPartie(Partie * parent) :
  Tapis(), m_parent(parent)
{
}

void TapisPartie::changement_maitre(unsigned int ancien,
					    unsigned int nouveau)
{
  m_parent->throw_changement_maitre(ancien, nouveau);
}

void TapisPartie::nouveau_maitre(unsigned int maitre)
{
  m_parent->throw_nouveau_maitre(maitre);
}

void TapisPartie::cartes_gagnees
(std::vector<Carte> const & cartes,
 std::vector<unsigned int> const & poseurs,
 std::vector<unsigned int> const & gagnants)
{
  m_parent->throw_cartes_gagnees(cartes, poseurs, gagnants);
}

void TapisPartie::tour_change(unsigned int tour)
{
  m_parent->throw_tour_change(tour);
}
