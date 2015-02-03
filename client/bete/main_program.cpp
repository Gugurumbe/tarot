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
#include <QStringList>
#include <QHostAddress>
#include "ia.hpp"
#include "client_jeu.hpp"

// Il y a 8 arguments :
//(-> le nom de l'exécutable)
// -> l'adresse de l'hôte
// -> le port de l'hôte
// -> le nom à utiliser
// -> les 4 autres noms avec qui jouer

//En cas d'échec d'identification, retourne -1.
//Ne demande jamais d'inviter qui que ce soit, ne prend pas.
int main(int argc, char * argv[])
{  
  QCoreApplication app(argc, argv);
  QStringList arguments = app.arguments();
  int resultat;
  if(arguments.size() != 8)
    {
      if(arguments.size() >= 1)
	{
	  std::cout<<"Usage : "
		   <<std::string(arguments[0].toUtf8().data())
		   <<" <adresse_hote> <port_hote> <nom> "
		   <<"<adversaire1> <adversaire2> "
		   <<"<adversaire3> <adversaire4>."
		   <<std::endl;
	  std::cout<<"En general, le port est 45678."
		   <<std::endl;
	}
      else
	{
	  std::cout<<
	    "Usage : <programme> <adresse_hote> <port_hote> <nom> "
		   <<"<adversaire1> <adversaire2> "
		   <<"<adversaire3> <adversaire4>."
		   <<std::endl;
	  std::cout<<"En general, le port est 45678."<<std::endl;
	}
      resultat = 0;
    }
  else
    {
      QHostAddress hote(arguments[1]);
      quint16 port(arguments[2].toInt());
      QString nom(arguments[3]);
      QVector<QString> equipe = 
	QVector<QString>()<<arguments[3]<<arguments[4]
			  <<arguments[5]<<arguments[6]
			  <<arguments[7];
      Ia ia(nom, equipe) ;
      ClientJeu jeu;
#define c(signal) QObject::connect(&jeu, SIGNAL(signal),	\
				   &ia, SLOT(signal))
      c(deconnecte()); //Quitte le programme.
      c(doit_identifier()); //Donne son nom.
      c(identification_refusee(std::string)); //Quitte
      c(adversaires(std::vector<std::string>)); //Vérifie si ce sont bien
      //ceux qu'on veut, se reconnecte dans le cas contraire
      c(doit_priser(Option<Enchere>)); //Passe.
      c(jeu_est(std::vector<Carte>)); //Tient mon jeu à jour.
      c(doit_jouer()); //Joue la carte 
      c(requete_refusee(Carte)); //Passe à la suivante.
#define r(signal) QObject::connect(&ia, SIGNAL(signal),	\
				   &jeu, SLOT(signal));
      r(formuler_identification(std::string));
      r(formuler_prise(Enchere::Prise)); //Nécessaire
      r(formuler_requete(Carte));
      r(deconnecter()); //Nécessaire pour refuser une invitation.
      r(reconnecter());
      QObject::connect(&ia, SIGNAL(doit_quitter()),
		       &app, SLOT(quit()));
      jeu.connecter(hote, port);
      resultat = app.exec();
    }
  return resultat;
}
