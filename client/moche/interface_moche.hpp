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
   @file client/moche/interface_moche.hpp

   @brief Interface en console.
 */

#ifndef INTERFACE_MOCHE_DEFINIE

/**
   @brief Empêche les inclusions multiple. 
 */
#define INTERFACE_MOCHE_DEFINIE

#include <QObject>
#include "enchere.hpp"
#include "option.hpp"
#include <vector>
#include "carte.hpp"
#include "tapis.hpp"
#include <QVector>
#include <iostream>
#include <QThread>
#include <QSharedPointer>
#include <QMutex>

/**
   @brief Classe d'interface moche.

   Les signaux/slots correspondent à ceux de ClientJeu.
 */
class InterfaceMoche: public QThread
{
  Q_OBJECT;
public:
  InterfaceMoche(std::ostream & out, QObject * parent = 0);
  virtual ~InterfaceMoche();
public slots:
  void connecte();
  void deconnecte();
  void doit_identifier();
  void identification_acceptee(std::string nom);
  void identification_refusee(std::string nom);
  void entree(std::string nom);
  void sortie(std::string nom);
  void adversaires(std::vector<std::string> noms);
  void invitation_acceptee(std::vector<std::string> 
			   adversaires);
  void invitation_refusee(std::vector<std::string>
			  adversaires);
  void numero_change(unsigned int n);
  void doit_priser(Option<Enchere> max);
  void enchere_acceptee(Enchere e);
  void enchere_refusee(Enchere e);
  void contrat_intermediaire(Enchere e);
  void doit_appeler(std::vector<Carte> acceptees);
  void appel_accepte(Carte c);
  void appel_refuse(Carte c);
  void contrat_final(Enchere e);
  void chien(Carte c1, Carte c2, Carte c3);
  void doit_ecarter(std::vector<Carte> possibles,
		    std::vector<Carte> atouts);
  void ecart_accepte(std::vector<Carte> ecart);
  void ecart_refuse(std::vector<Carte> ecart);
  void atout_au_chien(std::vector<Carte> atouts);
  void maitre_change(unsigned int joueur);
  void jeu_change(std::vector<Carte> gagnees,
		  std::vector<Carte> perdues);
  void jeu_est(std::vector<Carte> cartes);
  void doit_jouer();
  void requete_acceptee(Carte posee);
  void requete_refusee(Carte refusee);
  void carte_jouee(unsigned int joueur, Carte carte);
  void carte_gagnee(Carte c, unsigned int poseur,
		    unsigned int gagnant);
  void pli_termine(unsigned int joueur);
  void tapis_change(Tapis tapis);
  void partie_terminee(std::vector<int> scores);
signals:
  void formuler_identification(std::string nom);
  void formuler_invitation(std::vector<std::string>
			   adversaires);
  void formuler_prise(Enchere::Prise p);
  void formuler_appel(Carte requete);
  void formuler_ecart(std::vector<Carte> ecart);
  void formuler_requete(Carte requete);
  void deconnecter();
  void reconnecter();

  void doit_quitter();
protected:
  virtual void run();
private:
  QString mon_nom;
  QVector<QString> m_adversaires;
  QVector<QString> vestibule;
  QVector<QSharedPointer<Carte> > appelables;
  QVector<QSharedPointer<Carte> > ecartables;
  QVector<QSharedPointer<Carte> > mon_jeu;
  std::ostream & o;
  bool continuer;
  QMutex m_mutex;
  void trier_jeu();
};

std::ostream & operator<<(std::ostream & out, const QString & str);

#endif
