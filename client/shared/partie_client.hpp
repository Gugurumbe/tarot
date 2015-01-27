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


/**
   @file client/shared/partie_client.hpp
   @brief Particularise une Partie côté client.
 */

#ifndef PARTIE_CLIENT_DEFINIE

/**
   Empêche les inclusions récursives.
 */
#define PARTIE_CLIENT_DEFINIE

#include "partie.hpp"
#include "main.hpp"
#include "transaction.hpp"

#include <QObject>
#include <queue>

/**
   @brief Précision d'une Partie, côté client.

   La classe PartieClient est une Partie qui possède les propriétés
   suivantes :
   1. C'est un QObject, donc elle peut émettre des signaux et définir
   des slots pour faciliter la compréhension de l'avancement d'une
   partie.
   2. Contrairement à la PartieServeur, elle contient de l'information
   particulière pour un seul joueur. Elle connaît son numéro, ses
   cartes, peut deviner son équipe en fonction des cartes qui se
   jouent.
 */
class PartieClient: public QObject, public Partie
{
  Q_OBJECT;
public:

  /**
     @brief Construit un QObject.
     
     @param parent : l'objet parent dans le système Qt.
   */
  PartieClient(QObject * parent = 0);

  /**
     @brief Réinitialise la partie
   */
  void reinitialiser();

  /**
     @brief Indique si c'est mon tour.
     
     @return vrai ssi je dois faire une Enchere ou demander à jouer
     une Carte.
   */
  bool mon_tour() const;

  /**
     @brief Retourne le numéro du joueur.

     mon_numero() == tour() est équivalent à mon_tour().
     
     @return Le numéro du joueur.
     @see PartieClient::mon_tour() const
  */
  unsigned int mon_numero() const;

  /**
     @brief Regarder mon jeu.

     @return Mon jeu, augmenté des Cartes de l'écart si elles sont
     disponibles. 
   */
  const Main & mon_jeu() const;
  
  /**
     @brief Indique quelles cartes je peux appeler.
     
     @return Les cartes que je peux appeler.
   */
  std::vector<Carte> cartes_appelables() const;
  
  /**
     @brief Indique quelles cartes je peux écarter.
     
     @return Un tableau à 2 lignes, dont la première est les cartes
     écartables en toute circonstance, et la seconde les atouts
     écartables en dernier recours.
  */
  std::vector<std::vector<Carte> > cartes_ecartables() const;

  /**
     @brief Indique quel est mon nom.

     @return Mon nom, que l'identification ait réussi ou pas.
   */
  std::string mon_nom() const;

  /**
     @brief Retourne la liste des gens disponibles.
     
     @return Les noms.
   */
  std::vector<std::string> vestibule() const;
private:

  /**
     Retient mon nom
   */
  std::string m_mon_nom;
  /** Retient les noms des gens présents */
  std::vector<std::string> m_vestibule;

  /** Retient le numéro de mon tour. */
  unsigned int m_mon_tour;
  /** Retient mes Cartes. */
  Main mes_cartes;
  /** Retient les 3 cartes du chien.*/
  std::vector<Carte> chien_si_devoile;
  std::queue<Transaction> en_cours;
  void annuler_transaction();
  //Une fois acceptée, la transaction doit mettre à jour les 4
  //booléens plus bas, ainsi que le jeu (retirer les cartes jouées). 
  void transaction_enchere_acceptee();
  void transaction_appel_acceptee();
  void transaction_ecart_acceptee();
  void transaction_jeu_acceptee();
  void transaction_identification_acceptee();
  void transaction_invitation_acceptee();
  void oublier_transaction();
  //Ajouter une transaction, ça ajoute la bonne transaction à la
  //pile.
  void ajouter_transaction_enchere(unsigned int prise);
  void ajouter_transaction_appel(unsigned int carte);
  void ajouter_transaction_ecart(const int ecart[3]);
  void ajouter_transaction_jeu(unsigned int carte);
  void ajouter_transaction_identification(const std::string & nom);
  void ajouter_transaction_invitation(const std::vector<std::string> & nom);
  bool m_doit_identifier;
  bool m_doit_priser; //Vaut vrai jusqu'à ce que notre prise soit
		      //acceptée. 
  bool m_doit_appeler;//Vaut vrai jusqu'à ce que notre appel soit
		      //accepté. 
  bool m_doit_ecarter;//Vaut vrai jusqu'à ce que notre écart soit
		      //accepté.
  bool m_doit_jouer;  //Vaut vrai jusqu'à ce que notre carte soit
		      //acceptée. 
  //Pour le débogage :
  void presenter() const; 
  std::vector<Carte> m_cartes_gagnees;
  std::vector<unsigned int> m_poseurs;
  std::vector<unsigned int> m_gagnants;
  std::queue<unsigned int> m_changements_maitres;
public slots:  

  /**
     @brief Comprend un Protocole::Message.
    
     La mise à jour du contenu de la Partie côté client s'effectue de
     la façon suivante :
     1. Appel de Partie::assimiler(unsigned int).
     2. Récupération de mon numéro en début de partie.
     3. Récupération de mes cartes en début de partie.
     4. Mise à jour de mes cartes si je prends le Chien et fais un
     écart.
     5. Mise à jour de mes cartes lorsque je joue.
     6. Mise à jour de mon équipe lorsque la Carte appelée se dévoile
     dans mon jeu, dans le Chien et au cours de la partie.

     @see Partie::assimiler(unsigned int)
   */
  void assimiler(const Protocole::Message & message);

  /**
     @brief Construit un Message.

     Construit un message de type "prise".
     @param p : la prise que souhaite effectuer le client.
     @see PartieClient::doit_emettre(Message)
   */
  void formuler_prise(Enchere::Prise p);

  /**
     @brief Construit un Message.

     Construit un message de type "appeler".
     @param carte : la carte que l'on souhaite appeler.
     @see PartieClient::doit_emettre(Message)
   */
  void appeler(const Carte & carte);

  /**
     @brief Construit un Message.

     Construit un message de type "ecarter".
     @param cartes : les cartes que l'on souhaite écarter.
     @see PartieClient::doit_emettre(Message)
   */
  void ecarter(std::vector<Carte> const & cartes);

  /**
     @brief Construit un Message.

     Construit un message de type "chelem".
     @see PartieClient::demander_jeu()
     @see PartieClient::doit_emettre(Message)
   */
  void demander_chelem();

  /**
     @brief Construit un Message.
     
     Construit un message de type "chelem".
     @see PartieClient::demander_chelem()
     @see PartieClient::doit_emettre(Message)
   */
  void demander_jeu();

  /**
     @brief Construit un Message.

     Construit un message de type "montrer_poignee".
     @param requete : les cartes que l'on souhaite montrer.
     @see PartieClient::doit_emettre(Message)
   */
  void montrer_poignee(std::vector<Carte> const & requete);

  /**
     @brief Construit un Message.
     
     Construit un Message de type "requete".
     @param carte : la carte que l'on souhaite jouer.
     @see PartieClient::doit_emettre(Message)
   */
  void jouer(const Carte & carte);

  /**
     @brief Construit un Message.
     
     Construit un message de type "identifier".
     @param nom Le nom à tester.
     @see PartieClient::doit_emettre(Message)
   */
  void identifier(const std::string & nom);

  /**
     @brief Construit un Message.

     Construit un message de type "inviter".
     @param adversaires Les adversaires que je demande.
     @see PartieClient::doit_emettre(Message)
   */
  void formuler_invitation(const std::vector<std::string> & adversaires);
  
signals:
  
  /**
     @brief Envoyer un Message

     Indique à la partie "réseau" du programme client qu'il faut
     envoyer un Message au serveur.

     Ce Message peut être constitué dans 
     PartieClient::assimiler(const Message &),
     ou par l'un des slots de PartieClient.
     @param m : le Message à envoyer au serveur.
   */
  void doit_emettre(Protocole::Message m);

  /**
     @brief Quitter l'application.

     Indique que la relation avec le serveur n'est plus valable.

     Ce signal est déclenché lorsqu'une erreur de protocole est
     signalée par le serveur.
   */
  void doit_quitter();

  /**
     @brief Le numéro a changé
     @param num Le numéro du client.
   */
  void numero_change(unsigned int num);

  /**
     @brief Votre jeu a été modifié.
     
     @param gagnees Les cartes gagnées.
     @param perdues Les cartes perdues.
   */
  void jeu_change(std::vector<Carte> gagnees, 
		  std::vector<Carte> perdues);

  /**
     @brief Votre jeu vous est donné.
     
     @param cartes Mon jeu.
  */
  void jeu_est(std::vector<Carte> cartes);

  /**
     @brief Le tapis a changé.
     @param tapis Le tapis de la partie.
   */
  void tapis_change(Tapis tapis);

  /**
     @brief Action refusée.

     Indique que l'action du client a été refusée par le serveur. Ce
     peut être :
     1. une enchère,
     2. un appel,
     3. un écart,
     4. un chelem,
     5. une poignée,
     6. une carte à jouer.
   */
  void action_refusee();

  /**
     @brief C'est à vous de priser.

     Émis lorsque le tour du client arrive lors de la phase des
     enchères.
     
     @param max L'enchère max jusqu'à présent.
     @warning max n'est pas défini au premier tour !
   */
  void doit_priser(Option<Enchere> max);

  /**
     @brief Votre enchère a été acceptée.
     
     @param e Votre enchère.
   */
  void enchere_acceptee(Enchere e);
  
  /**
     @brief Votre enchère a été refusée.
     
     @param e L'enchère que vous aviez faite.
   */
  void enchere_refusee(Enchere e);

  /**
     @brief Une enchère vient d'être annoncée.

     Émis à la réception d'un Msg_contrat.
     @param e : l'enchère effectuée.
     @see PartieClient::doit_priser()
   */
  void contrat_intermediaire(Enchere e);

  /**
     @brief Vous devez appeler une Carte.
     
     Émis à la réception d'un Msg_appel.
     
     @param acceptees Les cartes que je peux appeler.
     
     @see ClientJeu::doit_appeler(std::vector<Carte>)
   */
  void doit_appeler(std::vector<Carte> acceptees);

  /**
     @brief Votre appel a été accepté.
     
     @param c La carte appelée.
   */
  void appel_accepte(Carte c);

  /**
     @brief Vous ne pouvez pas appeler cette Carte.
     
     @param c La carte que vous aviez appelé.
   */
  void appel_refuse(Carte c);

  /**
     @brief Les enchères sont terminées.

     Émis à la réception d'un... Msg_contrat_final, bien sûr !
     @param e : l'enchère finale.
   */
  void contrat_final(Enchere e);

  /**
     @brief Voici les 3 cartes du Chien.
     
     Émis à la réception d'un Msg_chien, avant l'émission du signal
     PartieClient::doit_ecarter().
     @param carte1 : la première carte.
     @param carte2 : la deuxième carte.
     @param carte3 : la troisième carte.
   */
  void chien_devoile(Carte carte1, Carte carte2, Carte carte3);

  /**
     @brief Vous devez écarter 3 cartes.

     Émis juste après le signal
     PartieClient::chien_devoile()

     @param possibles L'ensemble des cartes écartables.
     @param atouts L'ensemble des atouts écartables en dernier
     recours.
   */
  void doit_ecarter(std::vector<Carte> possibles,
		    std::vector<Carte> atouts);

  /**
     @brief Votre écart a été accepté.
     
     @param ecart Votre écart.
   */
  void ecart_accepte(std::vector<Carte> ecart);

  /**
     @brief Votre écart a été refusé.
     
     @param ecart L'écart que vous aviez formulé.
   */
  void ecart_refuse(std::vector<Carte> ecart);

  /**
     @brief On a écarté cette (ces) carte(s).

     Ce signal est émis lorsqu'un Msg_atout est reçu.
     @param atouts : 1, 2 ou 3 cartes.
   */
  void atout_au_chien(std::vector<Carte> atouts);

  /**
     @brief Vous devez dire si vous souhaitez un chelem.

     Ce signal n'est pas utilisé, mais il pourra l'être bientôt.
   */
  void doit_demander_chelem();

  /**
     @brief Un chelem a été demandé !
     
     Émis éventuellement à la réception d'un Msg_jeu.
     Le premier joueur sera celui-ci !
     @note Si c'est vous, le signal doit_jouer() sera émis juste
     après.
     
     @param joueur : le joueur ayant demandé le chelem.
   */
  void chelem_demande(unsigned int joueur);

  /**
     @brief Une poignée a été montrée !

     Lorsque ce signal est émis, c'est encore le tour de celui qui a
     déclaré la poignée.
     @param poignee : les cartes montrées.
   */
  void poignee_montree(std::vector<Carte> poignee);

  /**
     @brief C'est à vous de jouer.

     Ce signal est émis lorsque la PartieClient a déterminé que
     c'était votre tour. C'est-à-dire :
     1. En début de jeu, si mon numéro est le 0 et qu'aucun chelem n'a
     été demandé, ou si j'ai demandé un chelem.
     2. Lorsque le joueur (numéro - 1) a joué une Carte.
   */
  void doit_jouer();

  /**
     @brief Votre requête a été acceptée.

     Une requête est une demande de jouer une carte.
     
     @param posee La carte que vous avez réussi à poser.
   */
  void requete_acceptee(Carte posee);

  /**
     @brief Votre requête a été refusée.
     
     Une requête est la demande de jouer une carte.
     @param refusee La carte que vous aviez essayé de poser.
   */
  void requete_refusee(Carte refusee);

  /**
     @brief Une Carte a été jouée;
     
     Ce signal est émis lorsqu'un Msg_carte est reçu. Le numéro du
     joueur est déterminé par la classe Partie.
     @param joueur : le numéro de celui qui vient de jouer.
     @param carte : la Carte qui a été jouée.
   */
  void carte_jouee(unsigned int joueur, Carte carte);

  /**
     @brief La partie est finie.

     Ce signal est émis lorsqu'un Msg_resultat est reçu.
     @param scores : les 5 scores des joueurs.
   */
  void score(std::vector<int> scores);

  /**
     @brief La Carte a été gagnée.
     
     Ce signal est émis lorsqu'une Carte a été gagnée par l'un des
     joueurs.

     @param c La carte en question.
     @param poseur Le numéro du joueur qui l'avait posée.
     @param gagnant Le numéro du joueur qui l'a gagnée.
   */
  void carte_gagnee(Carte c, unsigned int poseur, 
		    unsigned int gagnant);

  /**
     @brief Le pli a été gagné.

     Ce signal est émis lorsqu'un pli a été remporté.
     
     @param tour Le tour du joueur ayant gagné ce pli.
   */
  void pli(unsigned int tour);

  /**
     @brief Le joueur est maître.
     
     Ce signal est émis lorsque le maître change.

     @param joueur Le joueur maître.
   */
  void maitre(unsigned int joueur);

  /**
     @brief J'ai réussi à m'identifier.
     
     Ce signal est émis lorsque je suis identifié.
     
     @param nom Le nom que j'ai reçu.
   */
  void identification_acceptee(std::string nom);

  /**
     @brief Quelqu'un porte déjà ce nom dans le vestibule.

     @param nom Le nom que j'avais choisi.
   */
  void identification_refusee(std::string nom);

  /**
     @brief On vient de se connecter.
     
     @param nom Le nom du joueur qui vient de se connecter.

     @note Si c'est moi, ce signal est émis juste après 
     PartieClient::identification_acceptee.
   */
  void entree(std::string nom);

  /**
     @brief On vient de se déconnecter.
     @param nom Le nom du joueur qui s'est déconnecté.
     @note Si c'est moi, est émis juste avant PartieClient::adversaires.
   */
  void sortie(std::string nom);

  /**
     @brief Mes adversaires ont changé de nom ou de numéro.
     @param noms Les noms dans l'ordre de jeu.
   */
  void adversaires(std::vector<std::string> noms);

  /**
     @brief Mon invitation a été acceptée.

     @param adversaires Mes adversaires.
   */
  void invitation_acceptee(std::vector<std::string> adversaires);

  /**
     @brief Mon invitation a été refusée.

     @param adversaires Les adversaires que j'avais demandés.

     @note Ça n'a pas échoué parce que l'un d'entre eux a refusé, mais
     parce que les noms n'existaient pas dans le vestibule ou qu'un
     nom était dupliqué.
   */
  void invitation_refusee(std::vector<std::string> adversaires);

protected:
  virtual void changement_maitre(unsigned int ancien,
				 unsigned int nouveau);
  virtual void nouveau_maitre(unsigned int maitre);
  virtual void cartes_gagnees
  (std::vector<Carte> const & cartes,
   std::vector<unsigned int> const & poseurs,
   std::vector<unsigned int> const & gagnants);
};

#endif
