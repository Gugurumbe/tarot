#include "ia.hpp"

//#define TESTER_REPARTITION
//#define TESTER_PSEUDOVALEURS

enum Contenu{COEURS = 0, PIQUES, CARREAUX, TREFLES, ATOUTS, 
	     EXCUSES, BOUTS, ROIS, DAMES, CAVALIERS, VALETS};

std::vector<int> informations_calculees(const std::vector<Carte> & cartes)
{
  std::vector<int> resultat(11);
  unsigned int place = 0;
  for(unsigned int i = 0 ; i < cartes.size() ; i++)
    {
      switch(cartes[i].couleur())
	{
	case Carte::COEUR: place = COEURS; break;
	case Carte::PIQUE: place = PIQUES; break;
	case Carte::CARREAU: place = CARREAUX; break;
	case Carte::TREFLE: place = TREFLES; break;
	default:
	  if(cartes[i] == EXCUSE)
	    {
	      place = EXCUSES;
	    }
	  else
	    {
	      place = ATOUTS;
	    }
	}
      resultat[place]++;
      if(cartes[i].bout())resultat[BOUTS]++;
      if(!(cartes[i].atout()) && cartes[i] != EXCUSE)
	switch(cartes[i].valeur())
	  {
	  case Carte::ROI: resultat[ROIS]++; break;
	  case Carte::DAME: resultat[DAMES]++; break;
	  case Carte::CAVALIER: resultat[CAVALIERS]++; break;
	  case Carte::VALET: resultat[VALETS]++; break;
	  default: break;
	  }
    }
  return resultat;
}

std::vector<int> pseudo_valeurs()
{
  std::vector<int> p;
  p.resize(78);
  Carte c(1);
  for(unsigned int i = 0 ; i < 78 ; i++)
    {
      c = i;
      if(!c.atout())
	{
	  if(c.valeur() < Carte::VALET)
	    {
	      p[i] = 0;
	    }
	  else
	    {
	      switch(c.valeur())
		{
		case Carte::VALET: p[i] = 2; break;
		case Carte::CAVALIER: p[i] = 3; break;
		case Carte::DAME: p[i] = 4; break;
		case Carte::ROI: p[i] = 6; break;
		default: break;
		}
	    }
	}
      else
	{
	  if(c.valeur () >= 2 && c.valeur() <= 14)
	    {
	      p[i] = 2;
	    }
	  else if(c.valeur() >= 15 && c.valeur() <= 20)
	    {
	      p[i] = 3;
	    }
	  else if(c == PETIT || c == EXCUSE)
	    {
	      p[i] = 7;
	    }
	  else
	    {
	      p[i] = 9;
	    }
	}
    }
  return p;
}

Ia::Ia(const QString & nom, const QVector<QString> & equipe, QObject * parent):
  QObject(parent),
  nom_souhaite(nom),
  equipe_souhaitee(equipe),
  m_deconnexion_voulue(false),
  test_suivant(0),
  jeu(this),
  pseudo_valeurs_cartes(pseudo_valeurs())
{
#ifdef TESTER_REPARTITION
  std::vector<Carte> truc;
  unsigned int x;
  for(unsigned int i = 0 ; i < 15 ; i++)
    {
      x = rand () % 78;
      std::cout<<Carte(x)<<std::endl;
      truc.push_back(Carte(x));
    }
  std::cout<<"Résultat : "<<informations_calculees(truc)<<std::endl;
#endif

#ifdef TESTER_PSEUDOVALEURS
  std::cout<<"Pseudo-valeurs : "<<std::endl;
  for(unsigned int i = 0; i < pseudo_valeurs_cartes.size() ; i++)
    {
      std::cout<<"Pseudo-valeur de "<<Carte(i)<<" : "
	       <<pseudo_valeurs_cartes[i]<<"."<<std::endl;
    }
#endif

#define c(signal) QObject::connect(&jeu, SIGNAL(signal),	\
				   this, SLOT(signal))
  c(deconnecte()); //Quitte le programme.
  c(doit_identifier()); //Donne son nom.
  c(identification_refusee(std::string)); //Quitte
  c(adversaires(std::vector<std::string>)); //Vérifie si ce sont bien
  //ceux qu'on veut, se reconnecte dans le cas contraire
  c(doit_priser(Option<Enchere>)); //Passe.
  c(jeu_est(std::vector<Carte>)); //Tient mon jeu à jour.
  c(doit_jouer()); //Joue la carte 
  c(requete_refusee(Carte)); //Passe à la suivante.
#define r(signal) QObject::connect(this, SIGNAL(signal),	\
				   &jeu, SLOT(signal));
  r(formuler_identification(std::string));
  r(formuler_prise(Enchere::Prise)); //Nécessaire
  r(formuler_requete(Carte));
  r(deconnecter()); //Nécessaire pour refuser une invitation.
  r(reconnecter());
}

void Ia::connecter(QHostAddress hote, quint16 port)
{
  jeu.connecter(hote, port);
}

void Ia::deconnecte()
{
  //Si j'ai souhaité me déconnecter, je me reconnecte :
  if(m_deconnexion_voulue)
    {
      m_deconnexion_voulue = false;
      emit reconnecter();
    }
  //Sinon, j'ai fini mon job (la partie a été fermée)
  else
    {
      emit doit_quitter();
    }
}

void Ia::doit_identifier()
{
  //Conversion en std::string
  std::string nom(nom_souhaite.toUtf8().data());
  emit formuler_identification(nom);
}

void Ia::identification_refusee(std::string)
{
  //ERREUR : je n'ai pas réussi à prendre ce nom !
  emit doit_quitter();
}

void Ia::adversaires(std::vector<std::string> adversaires)
{
  bool trouve = false; //A-t-on trouvé cet adversaire dans l'équipe
		       //souhaitée ?  
  bool ok = true; //A-t-on trouvé tous ceux qu'on voulait ?
  //Parcourons la liste d'adversaires...
  for(unsigned int i = 0 ; i < adversaires.size() && ok ; i++)
    {
      trouve = false;
      //Parcourons l'équipe souhaitée...
      for(int j = 0 ; j < equipe_souhaitee.size() && (!trouve) ; j++)
	{
	  if(equipe_souhaitee[j] == QString::fromUtf8(adversaires[i].c_str()))
	    {
	      //Trouvé !
	      trouve = true;
	    }
	}
      ok = (ok && trouve) ;
    }
  if(!ok)
    {
      //Je me reconnecte
      emit deconnecter();
    }
}

void Ia::doit_priser(Option<Enchere>)
{
  //Je passse :)
  emit formuler_prise(Enchere::PASSE);
}

void Ia::jeu_est(std::vector<Carte>)
{
}

void Ia::doit_jouer()
{
  //Je joue la carte suivante...
  const PartieClient & partie = jeu.partie();
  const Main & ma_main = partie.mon_jeu();
  const std::vector<Carte> & mon_jeu = ma_main.cartes();
  emit formuler_requete(mon_jeu[test_suivant % mon_jeu.size()]);
}

void Ia::requete_refusee(Carte)
{
  //Je teste la carte suivante
  test_suivant++;
}
