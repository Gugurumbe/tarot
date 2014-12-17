/**
   @file client/ai/ia.hpp
 */
#ifndef IA_DEFINIE
/**
   @brief Empêche les inclusions récursives.
 */
#define IA_DEFINIE
#include "client_jeu.hpp"
/**
   @brief Classe d'intelligence artificielle.
   
   Même fonctionnement qu'un client graphique, à ceci près qu'elle
   prend elle-même ses décisions.
 */
class IA : public QObject
{
  Q_OBJECT;
public:
  /**
     @brief Constructeur par défaut.
     @param parent Le parent Qt.
   */
  IA(QObject * parent = 0);
public slots:
  /**
     @brief Initie une connexion à un serveur.
     @param hote L'adresse du serveur.
     @param port Le port d'écoute.
   */
  void connecter(QHostAddress hote, quint16 port);
  void doit_priser(Option<Enchere> plus_forte);
  void doit_jouer();
  void partie_terminee(std::vector<int> scores);
private:
  ClientJeu jeu; //TRÈS IMPORTANT : le ClientJeu sous-jacent !
  Carte testee; //La carte que j'essaie de jouer.
};
#endif
