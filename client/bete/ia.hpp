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
   @file client/bete/ia.hpp
   @brief Interface de la classe D'IA dite "de base".
 */

#ifndef IA_DEFINIE
/**
   @brief Empêche les inclusions multiples.
 */
#define IA_DEFINIE

#include <QObject>
#include "enchere.hpp"
#include "carte.hpp"
#include "option.hpp"
#include <QVector>
#include <QString>

/**
   @brief La classe IA de base.

   Elle ne connaît pas le ClientJeu qui la sollicite. Donc, on stocke
   un peu plus d'information que nécessaire, surtout en ce qui
   concerne les cartes appelables / écartables / "jouables".
   
   @see client/bete/main_program.cpp
 */
class Ia: public QObject
{
  Q_OBJECT
public:
  /**
     @brief Constructeur de l'IA.

     Comme l'ia n'a pas d'entrée interactive, il faut qu'elle dispose
     de suffisemment d'information à son lancement pour se connecter
     au jeu.
     @param nom Le nom que l'IA doit emprunter.
     @param equipe Les gens qu'elle doit affronter. Si une partie se
     présente alors que les adversaires ne sont pas l'équipe prévue,
     elle se déconnecte et se reconnecte.
     @param parent Le parent Qt.
   */
  Ia(const QString & nom, const QVector<QString> & equipe, 
     QObject * parent = 0);
private:
  bool m_deconnexion_voulue;
  const QString nom_souhaite;
  const QVector<QString> equipe_souhaitee;
  std::vector<Carte> mon_jeu;
  unsigned int test_suivant;
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
