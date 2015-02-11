#include <QCoreApplication>
#include <QStringList>
#include <QHostAddress>
#include "ia.hpp"

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
      QString hote(arguments[1]);
      quint16 port(arguments[2].toInt());
      QString nom(arguments[3]);
      QVector<QString> equipe = 
	QVector<QString>()<<arguments[3]<<arguments[4]
			  <<arguments[5]<<arguments[6]
			  <<arguments[7];
      Ia ia(nom, equipe) ;
      QObject::connect(&ia, SIGNAL(doit_quitter()),
		       &app, SLOT(quit()));
      ia.connecter(hote, port);
      resultat = app.exec();
    }
  return resultat;
}
