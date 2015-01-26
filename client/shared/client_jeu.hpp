/**
   @file client/shared/client_jeu.hpp
   @brief Spécialise le moteur "réseau" pour jouer avec un serveur.
 */

#ifndef CLIENT_JEU_DEFINI

/**
   Empêche les inclusions récursives.
 */
#define CLIENT_JEU_DEFINI

#include "client.hpp"
#include "partie.hpp"
#include "partie_client.hpp"
#include "option.hpp"

#include <iostream>
#include <iomanip>

/**
   @brief Spécialise la classe Client, en permettant de comprendre
   l'environnement de jeu du tarot.

   Utilisera la classe PartieClient.
 */

class ClientJeu : public Client
{
  Q_OBJECT;
public:
  /**
     @brief Constructeur par défaut.
   */
  ClientJeu(QObject * parent = 0);

  /**
     @brief Destructeur.
   */
  virtual ~ClientJeu();

  /**
     @brief Présente l'état des cartes.
   */
  void presenter_etat();
  /**
     @brief Retourne la partie en cours.
     @return La partie en cours.
   */
  const PartieClient & partie() const;
public slots:
  
  /**
     @brief Réaction personnalisée (connexion).
     
     @see Client::connecte()
   */
  void traiter_connexion();

  /**
     @brief Réaction personnalisée (déconnexion).

     @see Client::deconnecte()
   */
  void traiter_deconnexion();

  /**
     @brief Réaction personnalisée (message).
     
     @param m : Message reçu du serveur.

     @see Client::recu(Message)
   */
  void traiter_message(Protocole::Message m);

  /**
     @brief Envoie un message pour formuler une Prise.
     
     @param p La prise en question.

     @see PartieClient::formuler_prise(Enchere::Prise);
   */
  void formuler_prise(Enchere::Prise p);
  
  /**
     @brief Envoie un message pour formuler un appel.
     
     @param c La carte à appeler.

     @see PartieClient::appeler(const Carte & c)
   */
  void formuler_appel(Carte);

  /**
     @brief Envoie un message pour formuler un écart.
     
     @param ecart Les cartes à écarter.
     
     @see PartieClient::ecarter(const std::vector<Carte> &)
   */
  void formuler_ecart(std::vector<Carte> ecart);
  
  /**
     @brief Traité lorsqu'on demande à jouer une Carte.
     
     @param requete La Carte qu'on demande à jouer.
   */
  void formuler_requete(Carte requete);

  /**
     @brief Traité lorsqu'on demande une identification.

     @param nom Le nom que j'aimerais porter.
     @see PartieClient::identifier(const std::string &)
   */
  void formuler_identification(std::string nom);

  /**
     @brief Traité lorsqu'on demande d'inviter des adversaires.

     @param adversaires Les noms des adversaires.
     @see PartieClient::inviter(const std::vector<std::string> &)
   */
  void formuler_invitation(std::vector<std::string> adversaires); 

signals:
  
  /**
     @brief Émis lorsqu'on m'informe de mon numéro.
     
     @param n Mon numéro, entre 0 inclus et 5 exclus.
   */
  void numero_change(unsigned int n);

  /**
     @brief Émis lorsque je dois priser.
     
     @param max L'enchère maximale. Si je suis le premier, elle
     n'existe pas.
     @see PartieClient::doit_priser(Enchere)
   */
  void doit_priser(Option<Enchere> max);

  /**
     @brief Émis lorsque l'enchère a été acceptée.
     
     @param e L'enchère formulée.
   */
  void enchere_acceptee(Enchere e);

  /**
     @brief Émis lorsque mon enchère a été refusée.
     
     @param e L'enchère que j'avais essayé de formuler.
  */
  void enchere_refusee(Enchere e);

  /**
     @brief Émis lorsqu'un contrat intermédiaire est formulé.
     
     @param e L'enchère en question.
   */
  void contrat_intermediaire(Enchere e);

  /**
     @brief Émis lorsque je dois appeler une carte.
     
     @param acceptees Les cartes que je peux appeler.
     
     @see Main::peut_appeler(const Carte &)
   */
  void doit_appeler(std::vector<Carte> acceptees);

  /**
     @brief Émis lorsque mon appel a été accepté.
     
     @param c La carte que vous avez appelée.
   */
  void appel_accepte(Carte c);

  /**
     @brief Émis lorsque mon appel a été refusé.
     
     @param c La carte refusée.
   */
  void appel_refuse(Carte c);

  /**
     @brief Émis lorsque le contrat final est divulgué.
     
     @param e L'enchère correspondante.
   */
  void contrat_final(Enchere e);

  /**
     @brief Émis lorsque le Chien est dévoilé.
     
     @param c1 La première carte.
     @param c2 ...
     @param c3 ...
   */
  void chien(Carte c1, Carte c2, Carte c3);
  
  /**
     @brief Émis lorsque je dois faire un écart.

     @param possibles Toutes les cartes que je peux écarter.
     @param atouts Les atouts que je peux écarter en dernier recours.
   */
  void doit_ecarter(std::vector<Carte> possibles,
		    std::vector<Carte> atouts);

  /**
     @brief Émis lorsque l'écart a été accepté.
     
     @param ecart Votre écart.
   */
  void ecart_accepte(std::vector<Carte> ecart);
  
  /**
     @brief Émis lorsque l'écart est refusé.
     
     @param ecart L'écart que vous aviez essayé de formuler. 
   */
  void ecart_refuse(std::vector<Carte> ecart);

  /**
     @brief Émis lorsqu'un ou plusieurs atouts est/sont écarté(s).
     @param atouts Les atouts en question.
   */
  void atout_au_chien(std::vector<Carte> atouts);

  /**
     @brief Émis lorsque le maître du tapis change.
   */
  void maitre_change(unsigned int joueur);

  /**
     @brief Émis lorsque mon jeu change.
     
     @param gagnees Les cartes reçues.
     @param perdues Les cartes perdues.
   */
  void jeu_change(std::vector<Carte> gagnees,
		  std::vector<Carte> perdues);
  /**
     @brief Émis lorsque mon jeu change.
     @see jeu_change(std::vector<Carte>, std::vector<Carte>)
     
     Émis à chaque fois qu'on distribue des cartes uniquement.
   */
  void jeu_est(std::vector<Carte> cartes);

  /**
     @brief Émis lorsque je dois jouer.
   */
  void doit_jouer();  

  /**
     @brief Émis lorsque la carte que je voulais jouer a été
     acceptée. 
     
     @param posee La carte posée.
   */
  void requete_acceptee(Carte posee);
  
  /**
     @brief Émis lorsque la carte que je voulais jouer a été refusée.
     
     @param refusee La carte que vous aviez essayé de poser.
   */
  void requete_refusee(Carte refusee);

  /**
     @brief Émis lorsqu'une carte a été jouée.
     
     @param joueur Le numéro du joueur qui a joué cette Carte.
     @param carte La carte en question.
   */
  void carte_jouee(unsigned int joueur, Carte carte);

  /**
     @brief Émis lors du mouvement d'une carte.
     
     @param c La carte en question.
     @param poseur Le joueur qui l'avait posée.
     @param gagnant Le joueur qui l'a remportée.
   */
  void carte_gagnee(Carte c, unsigned int poseur, 
		    unsigned int gagnant);
  
  /**
     @brief Émis lorsqu'un pli est terminé.
     
     @param joueur Le joueur qui a remporté le pli.
   */
  void pli_termine(unsigned int joueur);
  
  /**
     @brief Émis lorsque le tapis est modifié.

     @param tapis Le tapis.
   */
  void tapis_change(Tapis tapis);

  /**
     @brief Émis lorsque la partie est terminée.
     
     @param scores Les scores à la fin de la partie.
   */
  void partie_terminee(std::vector<int> scores);

  /**
     @brief Émis lorsque je dois m'identifier.
   */
  void doit_identifier();

  /**
     @brief Émis lorsque mon identification a été acceptée.
     
     @param nom Le nom que j'ai reçu.
   */
  void identification_acceptee(std::string nom);

  /**
     @brief Émis lorsque mon identification a été refusée.

     @param nom Le nom que j'avais tenté.
   */
  void identification_refusee(std::string nom);
  
  /**
     @brief Émis lorsqu'un nouveau joueur est disponible.
     
     @param nom Le nom du nouveau venu.
   */
  void entree(std::string nom);

  /**
     @brief Émis lorsqu'un joueur n'est plus disponible.

     @param nom Le nom du joueur qui se casse.
   */
  void sortie(std::string nom);

  /**
     @brief Émis lorsqu'on prépare une manche.

     @param noms Les noms des joueurs, dans l'ordre de jeu.
   */
  void adversaires(std::vector<std::string> noms);

  /**
     @brief Émis lorsque mon invitation a été acceptée.
     @param adversaires Mes adversaires.
   */
  void invitation_acceptee(std::vector<std::string> adversaires);
  
  /**
     @brief Émis lorsque mon invitation a été refusée.
     @param adversaires Les adversaires que j'avais demandés.
   */
  void invitation_refusee(std::vector<std::string> adversaires);
private:
  PartieClient m_partie;
};

#endif
