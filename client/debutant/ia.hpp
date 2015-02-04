/**
   @file client/debutan/ia.hpp
 */

#ifndef DEBUTANT_DEFINI

/**
   @brief Empêche les inclusions multiples.
 */
#define DEBUTANT_DEFINI

#include <QObject>
#include "client_jeu.hpp"

class Ia: public QObject
{
  Q_OBJECT
public:
  Ia(const QString & nom, const QVector<QString> & equipe, 
     QObject * parent = 0);
  void connecter(QHostAddress hote, quint16 port);
private:
  const QString nom_souhaite;
  const QVector<QString> equipe_souhaitee;
  bool m_deconnexion_voulue;
  unsigned int test_suivant;
  ClientJeu jeu;
  std::vector<int> pseudo_valeurs_cartes;
public slots:
  /**
     @brief Lance la connexion.
   */
  void deconnecte();
  /**
     @brief Lance la procédure d'identification.
  */
  void doit_identifier();
  /**
     @brief Quitte.
     
     @param nom Inutilisé : c'est le nom que j'avais demandé.
   */
  void identification_refusee(std::string nom);
  /**
     @brief Vérifie que je suis dans la bonne partie, ou me
     déconnecte.

     @param adv Les adversaires. Doivent correspondre à l'équipe
     demandée (dans le désordre).
     @see Ia::Ia
   */
  void adversaires(std::vector<std::string> adv);
  /**
     @brief Demande à passer.

     @param opt Inutilisé : c'est l'enchère maximale jusqu'à présent.
   */
  void doit_priser(Option<Enchere> opt);
  /**
     @brief Tient mon jeu à jour.

     @param cartes Les cartes dont je dispose.
   */
  void jeu_est(std::vector<Carte> cartes);
  /**
     @brief Tente de jouer la carte suivante.
   */
  void doit_jouer();
  /**
     @brief Change le numéro de la prochaine carte à tenter.
     
     @param c La carte refusée. OSEF.
   */
  void requete_refusee(Carte c);
signals:
  /**
     @brief Émis lorsque je dois m'identifier.

     @param nom Mon nom.
   */
  void formuler_identification(std::string nom);
  /**
     @brief Émis pour dire que je passe.

     @param p Mon enchère : Enchere::PASSE.
   */
  void formuler_prise(Enchere::Prise p);
  /**
     @brief Émis pour dire que je veux jouer une carte.

     @param c La carte à tester. C'est une des cartes de mon jeu, mais
     je ne sais pas si elle sera acceptée.
   */
  void formuler_requete(Carte c);
  /**
     @brief Émis pour dire que je souhaite me déconnecter (quitter une
     partie où je ne souhaite pas être).
   */
  void deconnecter();
  /**
     @brief Émis pour dire que je souhaite me reconnecter.
   */
  void reconnecter();
  /**
     @brief Émis pour dire que j'ai fini mon travail.
   */
  void doit_quitter();
};

#endif
