/**
   @file shared/protocole.hpp
   @brief Définit le protocole de communication.
   
   Le protocole de communication consiste en un ensemble de messages
   de différentes natures. Chaque type de message a sa structure
   propre.

   @see Partie::assimiler(const Message &)
*/

#ifndef PROTOCOLE_DEFINI

/**
   Empêche les inclusions récursives.
*/
#define PROTOCOLE_DEFINI

#include <QDataStream>
#include <iostream>
#include <string>
#include <vector>

/**
   @brief Nombre maximal d'octet utilisés par un nom. Comme ils seront
   encodés en UTF-8 (unicode), il y aura AU PLUS ce nombre de
   caractères, parfois strictement moins.
 */
#define TAILLE_NOM 16

/**
   @brief Regroupe les traitements du protocole.

   Utilité : le type de message "atout" est en concurrence avec la
   définition ATOUT pour la couleur des atouts.

   @see shared/carte.hpp
 */
namespace Protocole
{
  /**
     @brief Les différentes natures de message.
     @see Partie::PhaseJeu
     @see Message
  */
  enum
    {    
      /**
	 Transmission mixte :
	 1. client -> serveur : le client signale qu'il n'a pas compris
	 le message du serveur. Risque d'être ignoré par le serveur.
	 2. serveur -> client : le serveur informe le client qu'il ne
	 respecte pas le même protocole que lui, ou qu'il essaye de
	 jouer sans que ce soit son tour.
	 @see Msg_erreur_protocole
      */
      ERREUR_PROTOCOLE,

      /**
	 Transmission serveur -> client : la requête du client a été
	 refusée. Il n'a pas la bonne Carte, il a appelé une Dame alors
	 qu'il n'a pas tous les rois, il met un bout au chien, ou une
	 autre tentative de triche.
	 @see Msg_refuse
	 @see PartieClient::action_refusee()
      */
      REFUSE,

      /**
	 Transmission serveur -> client : le serveur donne son numéro de
	 tour au client.
	 @see Msg_numero
      */
      NUMERO,

      /**
	 Transmission serveur -> client : le serveur distribue 15 cartes
	 au client.
	 @see Msg_distribution
      */
      DISTRIBUTION,

      /**
	 Transmission client -> serveur : le client dit s'il prend.
	 @see Msg_prise
      */
      PRISE,

      /**
	 Transmission serveur -> client : le serveur informe les 5
	 clients de la prise du précédent joueur.
	 @see Msg_contrat
      */
      CONTRAT,

      /**
	 Transmission serveur -> client : le serveur informe le client
	 qui a la plus grosse Enchère qu'il doit appeler une
	 carte. Ainsi, le client n'a pas à vérifier s'il a l'enchère
	 maximale.
	 @see Msg_appel
      */
      APPEL,

      /**
	 Transmission client -> serveur : le client informe le serveur
	 qu'il veut écarter cette carte.
	 @see Msg_appeler
      */
      APPELER,

      /**
	 Transmission serveur -> client : le serveur informe tous les
	 clients du contrat final.
	 @see Msg_contrat_final
      */
      CONTRAT_FINAL,

      /**
	 Transmission facultative serveur -> client : si la prise n'est
	 ni Garde Sans ni Garde Contre, on montre le chien à tous. Le
	 preneur se doit d'effectuer un écart.
	 @see Msg_chien
      */
      CHIEN,

      /**
	 Transmission facultative client -> serveur : si la prise n'est
	 ni Garde Sans ni Garde Contre, le client fait une proposition
	 des 3 cartes qu'il veut écarter.
	 @see Msg_ecart
      */
      ECART,

      /**
	 Transmission facultative serveur -> client : si un atout est
	 présent dans l'écart, il doit être montré.
	 @see Msg_atout
      */
      ATOUT,

      /**
	 Transmission client -> serveur nécessaire : le client décide
	 s'il demande un chelem ou pas.
	 @warning Ce type de Message va bientôt être supprimé, car seul
	 le preneur doit décider s'il demande un chelem ou pas.
	 @see Msg_chelem
      */
      CHELEM,

      /**
	 Transmission serveur -> client : le serveur annonce que le jeu
	 commence, et donne le numéro du premier joueur.
	 @see Msg_jeu
      */
      JEU,

      /**
	 Transmission client -> serveur : le client tente de montrer une
	 poignée. Elle doit être validée par le serveur.
	 @see Msg_montrer_poignee
      */
      MONTRER_POIGNEE,

      /**
	 Transmission serveur -> client : le serveur informe tous les
	 clients que le joueur dont c'est encore le tour a déclaré une
	 poignée.
	 @see Msg_poignee
      */
      POIGNEE,

      /**
	 Transmission client -> serveur : le client demande à jouer une
	 certaine Carte.
	 @see Msg_requete
      */
      REQUETE,

      /**
	 Transmission serveur -> client : le joueur dont c'était le tour
	 a joué cette Carte.
	 @see Msg_carte
      */
      CARTE,

      /**
	 Transmission serveur -> client : c'est tel joueur qui remporte
	 le pli (c'est donc à lui de jouer). Envoyé juste après le
	 Msg_carte.
	 @see Msg_pli
      */
      PLI,

      /**
	 Transmission serveur -> client : voici le résultat de cette
	 partie.
	 @see Msg_resultat
      */
      RESULTAT,
      
      /**
	Transmission client -> serveur : je voudrais porter tel nom,
	s'il te plaît gentil serveur.
	@see Msg_refuse en cas de refus : nom déjà pris dans le
	vestibule. 
	@see Msg_entree si le serveur accepte.
      */
      IDENTIFIER,
      
      /**
	 Transmission serveur -> client : ce client a refusé une
	 invitation, ou il vient d'arriver.
       */
      ENTREE,

      /**
	 Transmission serveur -> client : ce client a accepté une
	 invitation, ou il vient de partir.
       */
      SORTIE,
      
      /**
	 Transmission serveur -> client : vous jouez maintenant avec
	 ces 4 autres types, dans cet ordre. Émis avant chaque manche.
       */
      NOMS,
      
      /**
	 Transmission client -> serveur : vous demandez à faire une
	 partie avec 4 autres types.
       */
      INVITER
    };

  /**
     @brief Contenu d'un Message d'erreur de protocole.

     Pour respecter la forme générale des Msg_*, il se présente sous la
     forme d'une structure vide, d'où le warning à la compilation.

     Circonstances dans lesquelles il apparaît :
     - Du côté du client : si le client a envoyé n'importe quoi, ou un
     Message hors de la phase de jeu correspondante, ou un Message sans
     avoir attendu son tour.  
     @note Selon les options de compilation, le serveur peut tout aussi
     bien rompre la connexion.
     - Du côté du serveur : si le client n'a pas compris un ordre du
     serveur. Cette incompréhension risque hélas d'être ignorée par le
     serveur. Le client n'a plus qu'à rompre la connexion.
  */
  struct Msg_erreur_protocole
  {
  };

  /**
     @brief Contenu d'un Message de refus.

     Pour respecter la forme générale des Msg_*, il se présente sous la
     forme d'une structure vide, d'où le warning à la compilation.

     Circonstances dans lesquelles il apparaît : lorsque le client a
     effectué une action sans en avoir le droit. En effectuant une
     Enchere inférieure à la précédente, en appelant une Carte
     interdite, en écartant une Carte interdite ou qu'il n'a pas, en
     souhaitant montrer une Poignée incompatible avec les règles, en
     souhaitant jouer une Carte interdite ou qu'il n'a pas, le client 
     reçoit un tel message. De plus, s'il demande un nom déjà pris par
     quelqu'un dans le vestibule (lors de la phase d'identification),
     il le reçoit également.
     @see PartieClient::refuse()
     @see PartieServeur::tester
  */
  struct Msg_refuse
  {
  };

  /**
     @brief Contenu d'un Message de transmission de numéro.

     Circonstances dans lesquelles il apparaît : en début de chaque
     manche, tous les joueurs reçoivent leur numéro de tour.
  */
  struct Msg_numero
  {
    /**
       @brief Numéro du joueur.

       Numéro entre 0 et 4.
    */
    int n;
  };

  /**
     @brief Contenu d'un Message de distribution des cartes.
   
     Circonstances dans lesquelles il apparaît : en début de chaque
     manche, après la distribution de numéro, tous les joueurs
     reçoivent 15 cartes.
     @see Main::Main(const Msg_distribution &)
  */
  struct Msg_distribution
  {
    /**
       @brief Les cartes distribuées.
     
       Ce sont les 15 numéros des cartes.
    */
    int cartes[15];
  };

  /**
     @brief Contenu d'un Message de prise.

     Circonstances dans lesquelles il apparaît : pendant la phase des
     enchères, chacun des joueurs décide s'il prise.
     @see Enchere::Enchere(const Msg_prise &)
  */
  struct Msg_prise
  {
    /**
       @brief Enchère.

       Numéro entre 0 (passe) et 4 (garde sans).
       @see Enchere::Prise
    */
    int niveau;
  };

  /**
     @brief Contenu d'un Message de contrat.
   
     Circonstances dans lesquelles il apparaît : pendant la phase des
     enchères, valide le Msg_prise d'un client et en informe tous les
     autres.
  */
  struct Msg_contrat
  {
    /**
       @brief Enchère.

       Numéro entre 0 (passe) et 4 (garde sans).
       @see Enchere::Prise
    */
    int niveau;
  };

  /**
     @brief Contenu d'un Message d'appel de carte.

     Pour respecter la forme générale des Msg_*, il se présente sous la
     forme d'une structure vide, d'où le warning à la compilation.

     Circonstances dans lesquelles il apparaît : le serveur informe le
     joueur concerné par le contrat final et l'invite à appeler une
     Carte.
  */
  struct Msg_appel
  {
  };

  /**
     @brief Contenu d'une requête pour appeler une Carte.

     Circonstances dans lesquelles il apparaît : le client demande s'il
     peut appeler cette Carte.
  */
  struct Msg_appeler
  {
    /**
       @brief Le numéro de la Carte.
    */
    int carte;
  };

  /**
     @brief Contenu d'un Message d'information du contrat final.

     Circonstances dans lesquelles il apparaît : le serveur informe tous
     les clients du contrat final.
     @see Enchere::Enchere(const Msg_contrat_final &)
  */
  struct Msg_contrat_final
  {
    /**
       @brief Numéro du preneur.
    */
    int preneur;

    /**
       @brief Prise.
    */
    int niveau;

    /**
       @brief Carte appelée.
    */
    int appel;
  };

  /**
     @brief Contenu d'un Message d'information du Chien.

     Circonstances dans lesquelles il apparaît : si la Prise le permet,
     le serveur montre les trois cartes du chien aux joueurs. Le joueur
     ayant pris doit alors faire un écart.
  */
  struct Msg_chien
  {
    /**
       @brief Les trois cartes du chien.
    */
    int chien[3];
  };

  /**
     @brief Contenu d'un Message de requête d'écart.

     Circonstances dans lesquelles il apparaît : si la Prise le permet,
     le client demande d'écarter 3 cartes parmi les 18 qui sont à sa
     disposition.
  */
  struct Msg_ecart
  {
    /**
       @brief Les trois cartes du chien.
    */
    int ecart[3];
  };

  /**
     @brief Contenu d'un Message d'information de l'écart.

     Circonstances dans lesquelles il apparaît : si la Prise le permet,
     et si le preneur met un ou des atouts dans son écart, le serveur
     l'annonce à tous.
  */
  struct Msg_atout
  {
    /**
       @brief Le nombre d'atouts écartés.
    */
    int nombre;
  
    /**
       @brief Les atouts de l'écart.
    */
    int cartes[3];
  };

  /**
     @brief Contenu d'un Message de demande de chelem.

     Circonstances dans lesquelles il apparaît : après avoir effectué
     une Prise, le joueur peut demander un chelem.
  */
  struct Msg_chelem
  {
    /**
       @brief Chelem, oui ou non ?
    */
    bool demande;
  };

  /**
     @brief Contenu d'un Message d'annonce de jeu.

     Circonstances dans lesquelles il apparaît : après la phase des
     enchères, le serveur dit "Jeu !". La partie commence.
  */
  struct Msg_jeu
  {
    /**
       @brief Numéro du premier joueur.
       
       En général, c'est 0. Lorsqu'on pourra faire un chelem, ça
       deviendra utile.
    */
    int ouvreur;
  };

  /**
     @brief Contenu d'un Message de requête de poignée.

     Circonstances dans lesquelles il apparaît : au premier tour, de
     jeu, chaque joueur peut essayer d'annoncer une poignée avant de
     jouer.
  */
  struct Msg_montrer_poignee
  {
    /**
       @brief Nombre d'atouts.
     
       Dans le tarot à 5, la triple poignée est de 13 cartes.
    */
    int taille;

    /**
       @brief Atouts à annoncer.

       Seuls les (taille) premiers atouts sont pertinents.
    */
    int atouts[13]; 
  };

  /**
     @brief Contenu d'un Message d'annonce de poignée.

     Circonstances dans lesquelles il apparaît : au premier tour, avant
     que le joueur ne joue une Carte.
     @see Msg_montrer_poignee
  */
  struct Msg_poignee
  {
    /**
       @brief Taille de la poignée.
    */
    int taille;
  
    /**
       @brief Atouts à annoncer.
    */
    int atouts[13];
  };

  /**
     @brief Contenu d'un Message de requête de jeu.

     Circonstances dans lesquelles il apparaît : dans la phase de Jeu,
     chaque joueur doit tour à tour demander à jouer une Carte.
  */
  struct Msg_requete
  {
    /**
       @brief La carte à jouer.
    */
    int carte;
  };

  /**
     @brief Contenu d'un Message d'information de carte jouée.

     Circonstances dans lesquelles il apparaît : dans la phase de Jeu,
     après validation, chacun joue une Carte. Le serveur informe tout le
     monde de la Carte jouée.
  */
  struct Msg_carte
  {
    /**
       @brief La carte jouée.
    */
    int carte;
  };

  /**
     @brief Contenu d'un Message d'information de pli.

     Circonstances dans lesquelles il apparaît : dans la phase de Jeu,
     lorsque les 5 joueurs ont joué une Carte, le pli revient à qui de
     droit.
  */
  struct Msg_pli
  {
    /**
       Le joueur qui prend la main.
    */
    int joueur;
  };

  /**
     @brief Contenu d'un Message d'information de fin de partie.

     Circonstances dans lesquelles il apparaît : à la fin de chaque
     manche, retourne les points de chacun pour cette manche.
  */
  struct Msg_resultat
  {
    /**
       @brief Les résultats.
    */
    int resultats[5];
  };

  /**
     @brief Contenu d'un Message de requête d'identification.

     Circonstances dans lesquelles il apparaît : à chaque fois qu'un
     client se connecte, ce qui comprend également les fois où on
     l'invite et où il refuse en se déconnectant.
   */
  struct Msg_identifier
  {
    /**
       @brief Le nom que j'aimerais utiliser.
     */
    char nom[TAILLE_NOM];
  };
  
  /**
     @brief Contenu d'un message d'information de connexion.

     Circonstances dans lesquelles il apparaît : à chaque fois qu'un
     Msg_identifier est accepté par le serveur, tous les membres du
     vestibule en sont informés.
  */
  struct Msg_entree
  {
    /**
       @brief Nom du nouveau venu.
     */
    char nom[TAILLE_NOM];
  };

  /**
     @brief Contenu d'un message d'information de cessation de
     disponibilité. 

     Circonstances dans lesquelles il apparaît : à chaque fois qu'un
     client est invité, et à chaque fois qu'un client du vestibule
     quitte. 
   */
  struct Msg_sortie
  {
    /**
       @brief Nom de celui qui s'en va.
     */
    char nom[TAILLE_NOM];
  };

  /**
     @brief Contenu d'un message de dénomination de vos adversaires.

     Circonstances dans lesquelles il apparaît : à chaque nouvelle
     manche, et donc en particulier lorsque vous êtes invité. Vous
     recevrez votre Msg_numero juste après.
   */
  struct Msg_noms
  {
    /**
       @brief Noms dans l'ordre de jeu.
     */
    char noms[5][TAILLE_NOM];
  };

  /**
     @brief Contenu d'un message de demande d'invitation.

     Circonstances dans lesquelles il apparaît : à chaque fois qu'une
     table se crée.
   */
  struct Msg_inviter
  {
    /**
       @brief Noms de vos adversaires, dans le désordre.
     */
    char noms[4][TAILLE_NOM];
  };

  /**
     @brief Somme de tous les messages possibles.
   
     @see Message
  */
  union Msg
  {
    /**
       @brief Champ de ERREUR_PROTOCOLE.
    */
    struct Msg_erreur_protocole erreur_protocole;
    /**
       @brief Champ de REFUSE.
    */
    struct Msg_refuse refuse;
    /**
       @brief Champ de NUMERO.
    */
    struct Msg_numero numero;
    /**
       @brief Champ de DISTRIBUTION.
    */
    struct Msg_distribution distribution;
    /**
       @brief Champ de PRISE.
    */
    struct Msg_prise prise;
    /**
       @brief Champ de CONTRAT.
    */
    struct Msg_contrat contrat;
    /**
       @brief Champ de APPEL.
    */
    struct Msg_appel appel;
    /**
       @brief Champ de APPELER.
    */
    struct Msg_appeler appeler;
    /**
       @brief Champ de CONTRAT_FINAL.
    */
    struct Msg_contrat_final contrat_final;
    /**
       @brief Champ de CHIEN.
    */
    struct Msg_chien chien;
    /**
       @brief Champ de ECART.
    */
    struct Msg_ecart ecart;
    /**
       @brief Champ de ATOUT.
    */
    struct Msg_atout atout;
    /**
       @brief Champ de CHELEM.
    */
    struct Msg_chelem chelem;
    /**
       @brief Champ de JEU.
    */
    struct Msg_jeu jeu;
    /**
       @brief Champ de MONTRER_POIGNEE.
    */
    struct Msg_montrer_poignee montrer_poignee;
    /**
       @brief Champ de POIGNEE.
    */
    struct Msg_poignee poignee;
    /**
       @brief Champ de REQUETE.
    */
    struct Msg_requete requete;
    /**
       @brief Champ de CARTE.
    */
    struct Msg_carte carte;
    /**
       @brief Champ de PLI.
    */
    struct Msg_pli pli;
    /**
       @brief Champ de RESULTAT.
    */
    struct Msg_resultat resultat;
    /**
       @brief Champ de IDENTIFIER.
     */
    struct Msg_identifier identifier;
    /**
       @brief Champ de ENTREE.
     */
    struct Msg_entree entree;
    /**
       @brief Champ de SORTIE.
     */
    struct Msg_sortie sortie;
    /**
       @brief Champ de NOMS.
     */
    struct Msg_noms noms;
    /**
       @brief Champ de INVITER.
     */
    struct Msg_inviter inviter;
  };

  /**
     @brief Structure de Message universel.

     Permet d'utiliser tous les Msg_*.

     @see Partie::assimiler(const Message &)
   
     Pour transformer un Message de/vers un paquet d'octets,
     la traduction est la suivante :
     1. Un octet, qui contient la taille du paquet.
     2. Un octet, qui prend la valeur du type.
     3. Des octets réservés à la partie spécialisée du Message. En règle
     générale, les entiers sont transformés en octets (non signés) dans
     l'ordre d'apparition des champs dans la structure Msg_*, les
     booléens sont transformés en "1" ou "0" sur un octet (non
     signé). Lorsqu'il faut envoyer un nombre variable d'octets
     (poignées, atouts au chien), on les envoie sans spécifier la
     taille. La lecture reste possible. Confère le code source.
  */
  struct Message
  {
    /**
       @brief Type du Message.
     
       Une valeur énumérée : ERREUR_PROTOCOLE, REFUSE, ...
    */
    int type;
    /**
       @brief Sait si le Message a pu être déchiffré.
    */
    bool compris;
    /**
       @brief Contenu du Message.
    */
    union Msg m;
  };

  /**
     @brief Lit un Message.
   
     @note On suppose qu'on a déjà lu l'otet de taille et qu'il y a
     exactement le nombre d'octets spécifiés disponible dans in.
     
     @param[in, out] in : flux d'entrée brut (Qt).
     @param[out] m : Message à remplir.
     @return Vrai ssi on a pu lire le Message en entier. En général,
     cette valeur est présente dans Message::compris.
  */
  bool lire(QDataStream & in, struct Message & m);

  /**
     @brief Écrit un Message.

     @note Ne rajoute pas la taille du message.

     @param m : Message à envoyer.
     @param[out] out : flux de sortie brut (Qt).
  */
  void ecrire(struct Message const & m, QDataStream & out);
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_erreur_protocole & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_refuse & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_numero & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_distribution & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_prise & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_contrat & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_appel & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_appeler & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_contrat_final & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_chien & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_ecart & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_atout & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_chelem & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_jeu & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_montrer_poignee & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_poignee & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_requete & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_carte & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_pli & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_resultat & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_identifier & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_entree & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_sortie & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_noms & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_inviter & m);
std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Message & m);

#endif
