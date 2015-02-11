#include "ia.hpp"

//#define TESTER_REPARTITION
//#define TESTER_PSEUDOVALEURS
#define TESTER_COMPTEUR

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

int compteur(const std::vector<Carte> & main, const std::vector<int> & pv)
{
  //Test
  int compteur = 0;
  for(unsigned int i = 0 ; i < main.size() ; i++)
    {
      //Ajout des pseudovaleurs de la main au compteur
      compteur += pv[main[i].numero()];
    }
  //Statistiques sur ma main
  std::vector<int> info = informations_calculees(main);
  //Bonus en fonction du nombre d'atouts
  if(info[ATOUTS] >= 5 && info[ATOUTS] < 8) compteur += 25;
  if(info[ATOUTS] >= 8 && info[ATOUTS] < 10) compteur += 35;
  if(info[ATOUTS] >= 10 && info[ATOUTS] < 12) compteur += 50;
  if(info[ATOUTS] >= 12) compteur += 1000;
  std::vector<int> nombre_longues(9);
  //Nombre de longues à n cartes
  for(int n = 4 ; n <= 13 ; n++)
    {
      nombre_longues[n - 4] = 0;
      for(unsigned int i = 0 ; i < 4 ; i++)
	{
	  if(info[i] >= n)nombre_longues[n - 4]++;
	}
    }
  //Bonus-malus en fonction des longues.
  if(nombre_longues[0] >= 2) compteur -= 20; //2 longues de plus de 4
					     //cartes 
  compteur += 10 * nombre_longues[1];
  compteur += 13 * nombre_longues[2];
  compteur += 16 * nombre_longues[3];
  compteur += 18 * nombre_longues[4];
  compteur -= 15 * nombre_longues[7];// Malus (longue encombrante) 
  compteur -= 15 * nombre_longues[8];
  compteur -= 1000 * nombre_longues[9];
  switch(info[BOUTS]) //Bonus en fonction du nombre de bouts 
    {
    case 1 : compteur += 5; break;
    case 2 : compteur += 15; break;
    case 3 : compteur += 20;
    default : break;
    }
  for(unsigned int i = 0 ; i < 4 ; i++)
    {
      if(info[i] == 0)compteur+=10;
      if(info[i] == 1)compteur+=8;
    }
  return compteur;
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

#ifdef TESTER_COMPTEUR
  std::cout<<"Test du compteur : 20 distributions."<<std::endl;
  for(unsigned int d = 0 ; d < 20 ; d++)
    {
      std::cout<<"Distribution "<<d<<" : "<<std::endl;
      std::vector<Carte> main;
      for(unsigned int c = 0 ; c < 15 ; c++)
	{
	  unsigned int j = 0;
	  Carte carte(rand() % 78);
	  while(j < main.size())
	    {
	      carte = (rand() % 78);
	      for(j = 0 ; j < main.size() && main[j] != carte ; j++);
	    }
	  main.push_back(carte);
	}
      for(unsigned int i = 0 ; i < 78 ; i++)
	{
	  unsigned int j = 0;
	  for(j = 0 ; j < main.size() && main[j] != i ; j++);
	  if(j < main.size())
	    {
	      std::cout<<Carte(i)<<std::endl;
	    }
	}
      std::vector<int> pv = pseudo_valeurs();
      std::cout<<"Compteur : "<<compteur(main, pv)<<std::endl;
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

void Ia::connecter(QString hote, quint16 port)
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
