#include <QCoreApplication>
#include <iostream>
#include <QStringList>
#include <QHostAddress>
#include "interface_moche.hpp"
#include "client_jeu.hpp"

#define ENDL std::endl;

Q_DECLARE_METATYPE (std::string)

int main(int argc, char * argv[])
{
  QCoreApplication app(argc, argv);
  QStringList arguments = app.arguments();
  ClientJeu jeu;
  InterfaceMoche interface(std::cout);
  int r = 0;
  qRegisterMetaType<std::string>("<std::string>");
  if(arguments.size() != 3)
    {
      if(arguments.size() >= 1)
	{
	  std::cout<<"Usage : "
		   <<arguments[0]
		   <<" <adresse_hote> <port_hote>."
		   <<ENDL;
	  std::cout<<"En general, le port est 45678."
		   <<ENDL;
	}
      else
	{
	  std::cout<<
	    "Usage : <programme> <adresse_hote> <port_hote>."
		   <<ENDL;
	  std::cout<<"En general, le port est 45678."<<ENDL;
	}
    }
  else
    {
      QHostAddress hote(arguments[1]);
      quint16 port(arguments[2].toInt());
#define C(signal) QObject::connect(&jeu, SIGNAL(signal),	\
				   &interface, SLOT(signal))
      C(connecte());
      C(deconnecte());
      C(doit_identifier());
      C(identification_acceptee(std::string));
      C(identification_refusee(std::string));
      C(entree(std::string));
      C(sortie(std::string));
      C(adversaires(std::vector<std::string>));
      C(invitation_acceptee(std::vector<std::string>));
      C(invitation_refusee(std::vector<std::string>));
      C(numero_change(unsigned int));
      C(doit_priser(Option<Enchere>));
      C(enchere_acceptee(Enchere));
      C(enchere_refusee(Enchere));
      C(contrat_intermediaire(Enchere));
      C(doit_appeler(std::vector<Carte>));
      C(appel_accepte(Carte));
      C(appel_refuse(Carte));
      C(contrat_final(Enchere));
      C(chien(Carte, Carte, Carte));
      C(doit_ecarter(std::vector<Carte>, std::vector<Carte>));
      C(ecart_accepte(std::vector<Carte>));
      C(ecart_refuse(std::vector<Carte>));
      C(atout_au_chien(std::vector<Carte>));
      C(maitre_change(unsigned int));
      C(jeu_change(std::vector<Carte>, std::vector<Carte>));
      C(jeu_est(std::vector<Carte>));
      C(doit_jouer());
      C(requete_acceptee(Carte));
      C(requete_refusee(Carte));
      C(carte_jouee(unsigned int, Carte));
      C(carte_gagnee(Carte, unsigned int, unsigned int));
      C(pli_termine(unsigned int));
      C(tapis_change(Tapis));
      C(partie_terminee(std::vector<int>));
#define R(signal) QObject::connect(&interface, SIGNAL(signal),	\
				   &jeu, SLOT(signal));
      R(formuler_identification(std::string));
      R(formuler_invitation(std::vector<std::string>));
      R(formuler_prise(Enchere::Prise));
      R(formuler_appel(Carte));
      R(formuler_ecart(std::vector<Carte>));
      R(formuler_requete(Carte));
      R(deconnecter());
      R(reconnecter());
      jeu.connecter(hote, port);
      interface.start();
      r = app.exec();
    }
  return r;
}