/*
  Project tarot virtuel is a card game.
  Copyright (C) 2015  Vivien Kraus
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QCoreApplication>
#include <iostream>
#include <QStringList>
#include <QHostAddress>
#include "interface_moche.hpp"
#include "client_jeu.hpp"

#define ENDL std::endl;

Q_DECLARE_METATYPE (std::string)
Q_DECLARE_METATYPE (Enchere::Prise)
Q_DECLARE_METATYPE (Carte)
Q_DECLARE_METATYPE (std::vector<std::string>)
Q_DECLARE_METATYPE (std::vector<Carte>)

int main(int argc, char * argv[])
{

  std::cout<<"Tarot virtuel - client Copyright (C) 2015 Vivien Kraus"
	   <<std::endl
	   <<"This program comes with ABSOLUTELY NO WARRANTY."
	   <<std::endl
	   <<"This is free software, and you are welcome to redistribute it"
	   <<std::endl
	   <<"under certain conditions."<<std::endl;
  
  QCoreApplication app(argc, argv);
  QStringList arguments = app.arguments();
  ClientJeu jeu;
  InterfaceMoche interface(std::cout);
  int r = 0;
  qRegisterMetaType<std::string>("<std::string>");
  qRegisterMetaType<Enchere::Prise>("<Enchere::Prise>");
  qRegisterMetaType<Carte>("<Carte>");
  qRegisterMetaType<std::vector<std::string> >("<std::vector<std::string> >");
  qRegisterMetaType<std::vector<std::string> >("<std::vector<std::string>>");
  qRegisterMetaType<std::vector<Carte> >("<std::vector<Carte> >");
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
      QObject::connect(&interface, SIGNAL(doit_quitter()),
		       &app, SLOT(quit()));
      jeu.connecter(hote, port);
      interface.start();
      r = app.exec();
    }
  return r;
}
