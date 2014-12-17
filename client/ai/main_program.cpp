#include <QCoreApplication>

//Sert uniquement à définir AUTO_ADDR et AUTO_PORT
#include "./config.hpp"

//Classe D'IA
#include "ia.hpp"

//Fonction principale
int main(int argc, char * argv[])
{
  QCoreApplication app(argc, argv);
  //Création d'une IA
  IA * ia = new IA;
  //Dit au CLientJeu sous-jacent de se connecter
  ia->connecter(AUTO_ADDR, AUTO_PORT);
  //On tue l'IA dès qu'elle se déconnecte.
  QObject::connect(ia, SIGNAL(destroyed()),
		   &app, SLOT(quit()));
  return app.exec();
}
