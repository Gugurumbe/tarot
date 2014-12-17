#include "ia.hpp"

IA::IA(QObject * parent): QObject(parent),
			  jeu(this), 
			  testee(0) //On commence par essayer de jouer
				    //le deux de pique (cf doit_jouer)
{
  //Pour plus de commodité :
#define C(signal) \
  QObject::connect(&jeu, SIGNAL(signal), this, SLOT(signal));

  //C'est plus facile de connecter les signaux du ClientJeu aux slots
  //du même nom.
  C(doit_priser(Option<Enchere>));
  C(doit_jouer());
  C(partie_terminee(std::vector<int>));
}
// Fonction appelée dans main_program.cpp
void IA::connecter(QHostAddress hote, quint16 port)
{
  //Simple comme bonjour :
  jeu.connecter(hote, port);
}
void IA::doit_priser(Option<Enchere>)
{
  //Je dois priser. Comme je suis une IA basique, je passe.
  jeu.formuler_prise(Enchere::PASSE);
}
void IA::doit_jouer()
{
  //Je dois jouer. Je commence par changer de carte :
  Carte suivante(testee.numero() + 1);
  
  //Je fais dire au serveur que je veux jouer cette carte :
  jeu.formuler_requete(suivante);
  
  //Je m'en souviens pour le prochain refus (et donc, le prochain
  //doit_jouer) 
  testee = suivante;
}
void IA::partie_terminee(std::vector<int>)
{
  //La partie est terminée, j'arrête le programme. Le moyen le plus
  //simple est de détruire le QObject (voir main_program.cpp)
  deleteLater();
}
