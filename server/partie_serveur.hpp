/**
   @file server/partie_serveur.hpp
   @brief Spécialise une Partie côté client.
 */

#ifndef PARTIE_SERVEUR_DEFINIE

/**
   Empêche les inclusions récursives.
*/
#define PARTIE_SERVEUR_DEFINIE

#include "partie.hpp"
#include "main.hpp"
#include "score.hpp"
#include <QObject>

/**
   @brief Précision d'une Partie, côté serveur.

   La classe PartieServeur comprend les messages du point de vue d'un
   arbitre. Ses caractéristiques principales sont :
   1. Une capacité à émettre des Messages, grâce à l'héritage de
   QObject. 
   2. La connaissance de l'ensemble des cartes du jeu, et donc des
   équipes. 
 */
class PartieServeur: public QObject, public Partie
{
  Q_OBJECT;
public:

  /**
     @brief Constructeur par défaut.

     Ne lance pas la distribution des cartes.
   */
  PartieServeur(QObject * parent = 0);

  /**
     @brief Réimplémente Partie::assimiler(const Message &).
     
     Met à jour le jeu réel, les cartes gagnées par le camp attaquant.
     @param message : le Message à assimiler. Peut provenir du serveur
     lui-même. 
   */
  void assimiler(const Protocole::Message & message);

  /**
     @brief Teste une requête.
     
     @param joueur : le joueur effectuant cette requête.
     @param m : la requête en question.
     
     @return 0 si la requête est acceptée.
     @return 1 si la requête est une erreur de protocole.
     @return 2 si la requête est refusée.
   */
  int tester(unsigned int joueur, const Protocole::Message & m) const;

  /**
     @brief Réinitialise la partie.

     - Réinitialise la Partie
     - Réinitialise les scores, le chien et l'enchère max
     - Réinitialise le joueur appelé
     - Réinitialise le nombre de plis joués.
   */
  void reinitialiser();
private:

  /**
     @brief Le jeu réel.
  */
  std::vector<Main> jeu_reel;

  /**
     @brief Les cartes gagnées par l'attaque.
     
     Chaque ligne correspond à un tour. Contient exactement 15 lignes
     à la fin, éventuellement vides.
     On compte les points à la fin.
  */
  std::vector<std::vector<Carte> > cartes_attaque;

  std::vector<std::vector<Carte> > cartes_defense;

  /**
     @brief Le chien.
   */
  std::vector<Carte> chien;

  /**
     @brief L'enchère maximale pendant la phase des Enchères. 
  */
  Enchere e_max;

  /**
     @brief Le joueur appelé.
  */
  unsigned int joueur_appele;
  
  /**
     @brief Plis restants à jouer.
     
     La valeur est décrémentée APRÈS l'assimilation du message pli.
   */
  unsigned int plis_restants;

protected:
    
  virtual void cartes_gagnees
  (std::vector<Carte> const & cartes,
   std::vector<unsigned int> const & poseurs,
   std::vector<unsigned int> const & gagnants);
			  
public slots:

  /**
     @brief Distribution des cartes.
     
     Émet des signaux doit_emettre(). Doit être appelée au début. 
     
     Au cas où une Main ne contiendrait que le Petit, on redistribue.
   */
  void distribuer();
  
signals:
  
  /**
     @brief Signal à connecter à 
     Table::doit_transmettre(unsigned int,Message,bool)

     @param j : le joueur à qui adresser le Message.
     @param m : le Message à adresser.
     @param assimiler : dit si le Message doit être assimilé.
   */
  void doit_emettre(unsigned int j, Protocole::Message m, bool assimiler);

  /**
     @brief Signal à connecter à
     Table::doit_transmettre_a_tous(Message)

     Lorsqu'un Message issu du serveur doit être envoyé à tous, il est
     analysé 5 fois par la Partie, au lieu d'une.

     @param m : le Message à adresser.
   */
  void doit_emettre_a_tous(Protocole::Message m);

  /**
     @brief La partie est finie.

     Émis uniquement lorsque le message resultat est transféré.
   */
  void termine();
};

#endif
