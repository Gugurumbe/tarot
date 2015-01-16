/**
   @file shared/partie.hpp
   @brief Définit un environnement de partie.
   
   Cet environnement est réutilisable du côté client (PartieClient)
   aussi bien que du côté du serveur (PartieServeur).
 */
#ifndef PARTIE_DEFINIE

/**
   Empêche les inclusions récursives.
 */
#define PARTIE_DEFINIE

#include "protocole.hpp"
#include "carte.hpp"
#include "enchere.hpp"
#include "tapis.hpp"
#include <vector>

/**
   @brief Environnement de partie de Tarot.

   La classe Partie fait un intermédiaire entre le réseau et le jeu.
   Elle analyse tous les Message entrants. Elle n'en émet pas, car on
   peut analyser n'importe quel Message, mais il ne faut pas émettre
   les mêmes Message côté serveur et côté client. Les Message à
   envoyer sont donc la responsabilité de PartieClient et
   PartieServeur.
 */
class Partie
{
public:

  /**
     @brief Étape d'une partie de Tarot.
     
     La progression est irréversible d'une phase à l'autre.

     Le message Msg_erreur_protocole pourra être envoyé côté serveur
     si le message envoyé ne correspond pas à la phase de jeu, ou si
     le message est envoyé par quelqu'un dont ce n'est pas le tour.  
     Du côté client, l'intérêt n'est pas forcément visible hors phases
     de débogage du serveur, mais côté serveur, cela permettrait
     d'indiquer au programme client qu'il n'est peut-être pas à jour.
     Au cas où le client fait des erreurs de protocole, on peut aussi
     le déconnecter de la partie, voire le bannir, sans prendre la
     peine de lui envoyer un Msg_erreur_protocole ni de garder sa
     connexion active.
     @see Partie::set_phase(Partie::PhaseJeu)
   */
  enum PhaseJeu
    {
      /**
	 Phase initiale où on peut trouver les Messages :
	 1. Msg_entree
	 2. Msg_sortie
	 3. Msg_noms
	 4. Msg_numero
	 5. Msg_distribution
       */
      CONSTITUTION_TABLE,

      /**
	 Phase des enchères où on peut trouver :
	 1. Msg_refuse
	 2. Msg_prise
	 3. Msg_contrat
	 4. Msg_appel
	 5. Msg_appeler
	 6. Msg_contrat_final

	 @note Le tour (numéro de joueur) a une signification dans la
	 première partie de cette phase. Les enchères se font à tour
	 de rôle. Dans la deuxième partie (appel), le serveur
	 sollicite explicitement le joueur concerné.
       */
      ENCHERES,

      /**
	 Phase de constitution de l'écart.
	 On y trouve :
	 1. Msg_refuse
	 2. Msg_chien
	 3. Msg_ecart
	 4. Msg_atout

	 @note Cette phase est optionnelle en cas de Garde Sans ou
	 Garde Contre.
       */
      CONSTITUTION_ECART,

      /**
	 Cette phase va être supprimée, car on n'attend pas que les
	 joueurs décident s'ils demandent un chelem. C'est uniquement
	 le preneur qui a ce pouvoir.
       */
      ATTENTE_CHELEM,

      /**
	 Phase principale du jeu.
	 On y trouve :
	 1. Msg_jeu
	 2. Msg_refuse
	 3. Msg_requete
	 4. Msg_carte
	 5. Msg_pli

	 @note Le tour a une signification dans cette phase : pour
	 chaque levée, les joueurs jouent dans l'ordre, en commençant
	 par celui qui a gagné le pli précédent.

	 @note Les déclarations de poignées se feront ici.
       */
      PHASE_JEU,

      /**
	 Phase de fin de jeu.
	 On y trouve :
	 1. Msg_resultat

	 @note Le serveur n'est plus réactif dans cette phase.
	 Elle n'existe que pour que les joueurs puissent finir de
	 recevoir le Msg_resultat avant de se déconnecter.
       */
      FIN
    };

  /**
     @brief Constructeur par défaut.
     
     Il est de la responsabilité de la Table (côté serveur)
     d'attribuer les numéros. Il est de la responsabilité de la
     PartieServeur de distribuer les cartes.
   */
  Partie();

  /**
     @brief Réinitialise la partie.
   */
  void reinitialiser();

  /**
     @brief Destructeur.
   */
  virtual ~Partie();

  /**
     @brief Retourne le contrat final.

     @warning Si on en est encore à la phase Enchères, le contrat
     final n'a pas encore été décidé. Il y aura une tentative d'accès
     à l'adresse '5' d'un tableau à 5 cases.
     @return L'enchère retenue.
   */
  const Enchere & donner_contrat_final() const;

  /**
     @brief Retourne l'enchère d'un joueur donné.

     @warning Il faut vérifier que le joueur a bien effectué une
     Enchère auparavant.

     @param joueur : le joueur à tester.
     @return L'enchère effectuée.
   */
  const Enchere & enchere_de(unsigned int joueur) const;

  /**
     @brief Teste si un chelem a été déclaré.
     
     @return -1 si aucun chelem n'a été demandé.
     @return Le numéro du joueur sinon.
   */
  int chelem() const;

  /**
     @brief Accès au joueur qui prise.

     @return Le numéro du joueur.
   */
  unsigned int attaquant() const;

  /**
     @brief Modification de l'attaquant.

     @warning L'attaquant est déterminé lors de la réception du
     Msg_contrat_final. Cependant, du côté du serveur, il faut pouvoir
     obtenir le numéro de l'attaquant pour valider un
     Msg_appeler, qui arrive avant. Donc, le serveur doit donner un
     indice sur l'attaquant avant qu'il soit donné.

     Ne pas utiliser hors du serveur.

     Ne fait rien si un attaquant a déjà été déterminé.

     @param j : le numéro du joueur attaquant.
   */
  void set_attaquant(unsigned int j);

  /**
     @brief À qui de jouer ?

     @return Le numéro du joueur dont c'est le tour.
   */
  unsigned int tour() const;

  /**
     @brief Qui a joué ?
     
     @return Le numéro du joueur qui vient de jouer.
   */
  unsigned int tour_precedent() const;

  /**
     @brief Taille des poignées déclarées.

     @param joueur : celui dont on veut connaître la taille de la
     poignée déclarée.
     @return 0 si le joueur n'a pas déclaré de poignée.
     @return La taille de la poignée déclarée par ce joueur sinon.
   */
  unsigned int poignee(unsigned int joueur) const;

  /**
     @brief Où en est-on ?

     @return La phase du jeu en cours.
     @see Partie::set_phase(PhaseJeu)
   */
  PhaseJeu phase() const;

  /**
     @brief On en est là !

     Force la phase du jeu.
     @warning À utiliser avec précaution. Normalement,
     Partie::assimiler(Message) suffit dans la plupart des cas à
     déterminer la phase du jeu. En cas de valeur erronée de la phase,
     le serveur peut envoyer des Msg_erreur_protocole à des clients
     qui ne sont pas en tort.
     @param p : la phase du jeu en cours.
     @see Partie::phase()
     @see Partie::assimiler(const Message &)
   */
  void set_phase(PhaseJeu p);

  /**
     @brief Analyse un Message.
     
     Analyse le message envoyé par le serveur, ou envoyé par le client
     et accepté par le serveur. En analysant ce Message, la Partie met
     à jour :
     1. Les Enchere de tout le monde.
     2. Le chelem (pour plus tard).
     3. Le tapis.
     4. Le joueur attaquant.
     5. Le tour du jeu.
     6. Les poignées déclarées (pour plus tard).
     7. Et le plus important : la phase du jeu !
     
     @param m : le message analysé.
     @see PartieClient::assimiler(const Message &)
     @see PartieServeur::assimiler(const Message &)
   */
  void assimiler(const Protocole::Message & m);

  /**
     @brief Montre le tapis de jeu.

     @return Le tapis de jeu.
   */
  const Tapis & tapis() const;

  /**
     @brief Ne pas regarder.
   */
  void throw_changement_maitre(unsigned int, unsigned int);

  /**
     @brief Ne pas regarder.
  */
  void throw_nouveau_maitre(unsigned int);

  /**
     @brief Ne pas regarder.
   */
  void throw_cartes_gagnees(std::vector<Carte> const &,
			    std::vector<unsigned int> const &,
			    std::vector<unsigned int> const &);
  /**
     @brief Ne pas regarder.
   */
  void throw_tour_change(unsigned int);
  /**
     @brief Donne l'enchère maximale.
     
     @return L'enchère maximale jusqu'à présent, ou une Passe de la
     part du joueur 0.
   */
  Enchere meilleure_enchere() const;
  /**
     @brief Donne le nom de tel joueur.

     @param i Le numéro du joueur à trouver.
     @return Le nom du joueur i.
   */
  std::string nom_de(unsigned int i) const;

protected:
  
  /**
     @brief Relaye Tapis::changement_maitre(unsigned int, unsigned int)
   */
  virtual void changement_maitre(unsigned int ancien, 
				 unsigned int nouveau);
  
  /**
     @brief Relaye Tapis::nouveau_maitre(unsigned int)
   */
  virtual void nouveau_maitre(unsigned int maitre);
  
  /**
     @brief Relaye Tapis::cartes_gagnees
   */
  virtual void cartes_gagnees
  (std::vector<Carte> const & cartes,
   std::vector<unsigned int> const & poseurs,
   std::vector<unsigned int> const & gagnants);
  
private:

  std::vector<std::string> m_noms_types;

  /**
     @brief Enchères enregistrées.
     
     Pour 4 d'entre elles, on n'enregistre pas plus que la Prise.
     @see Partie::contrat_final()
   */
  std::vector<Enchere> m_encheres;
  
  /**
     @brief Déclaration du chelem

     Dans la plupart des cas, vaut -1. En cas de chelem demandé,
     prend la valeur du joueur qui l'a demandé.
  */
  bool m_chelem;

  /**
     @brief Tapis de jeu.
   */
  Tapis * const m_tapis;

  /**
     @brief Joueur qui a pris.

     Prend initialement la valeur 5.
   */
  unsigned int m_attaquant;

  /**
     @brief Numéro de tour.

     Au début, vaut 0.
   */
  unsigned int m_tour;
  
  /**
     @brief Numéro du tour précédent.
     
     Au début, vaut 5.
   */
  unsigned int m_tour_precedent;

  /**
     @brief Nombre de plis joués.
   */
  unsigned int plis_joues;

  /**
     @brief La phase du jeu.
     
     Vaut initialement CONSTITUTION_TABLE.
   */
  PhaseJeu m_phase;

  /**
     @brief La taille des poignées déclarées.

     À l'adresse i, on trouve 0 ou la taille de la poignée déclarée.
   */
  std::vector<unsigned int> m_tailles_poignees;
};

#endif
