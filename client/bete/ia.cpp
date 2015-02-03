#include "ia.hpp"

Ia::Ia(const QString & nom, const QVector<QString> & equipe, QObject * parent):
  QObject(parent),
  m_deconnexion_voulue(false),
  nom_souhaite(nom),
  equipe_souhaitee(equipe),
  test_suivant(0)
{
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

void Ia::jeu_est(std::vector<Carte> cartes)
{
  //Normalement, l'ordre ne change pas.
  mon_jeu = cartes;
}

void Ia::doit_jouer()
{
  //Je joue la carte suivante...
  emit formuler_requete(mon_jeu[test_suivant % mon_jeu.size()]);
}

void Ia::requete_refusee(Carte)
{
  //Je teste la carte suivante
  test_suivant++;
}
