#include "ia.hpp"

Ia::Ia(const QString & nom, const QVector<QString> & equipe, QObject * parent):
  QObject(parent),
  nom_souhaite(nom),
  equipe_souhaitee(equipe),
  m_deconnexion_voulue(false),
  test_suivant(0),
  jeu(this)
{
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
