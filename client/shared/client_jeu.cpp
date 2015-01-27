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


#include "client_jeu.hpp"
#include <iostream>

#define NOM_CLASSE "ClientJeu"

#include "deboguer.hpp"

ClientJeu::ClientJeu(QObject * parent) : 
  Client(parent), m_partie(this)
{
  ENTER("ClientJeu(QObject * parent)");
  ADD_ARG("parent", parent);
  QObject::connect(this, SIGNAL(connecte()),
		   this, SLOT(traiter_connexion()));
  QObject::connect(this, SIGNAL(deconnecte()),
		   this, SLOT(traiter_deconnexion()));
  QObject::connect(this, SIGNAL(recu(Protocole::Message)),
		   this, SLOT(traiter_message(Protocole::Message)));
}

ClientJeu::~ClientJeu()
{
  ENTER("~ClientJeu()");
}

void ClientJeu::traiter_connexion()
{
  ENTER("traiter_connexion()");
  m_partie.reinitialiser();
  QObject::connect(&m_partie, SIGNAL(doit_emettre(Protocole::Message)),
		   this, SLOT(envoyer(Protocole::Message)));

#define C(signal_entree, signal_sortie) \
  connect(&m_partie, SIGNAL(signal_entree), this, \
	  SIGNAL(signal_sortie));
#define S(signal) C(signal, signal)
  S(numero_change(unsigned int));
  S(doit_priser(Option<Enchere>));
  S(enchere_acceptee(Enchere));
  S(enchere_refusee(Enchere));
  S(contrat_intermediaire(Enchere));
  S(doit_appeler(std::vector<Carte>));
  S(appel_accepte(Carte));
  S(appel_refuse(Carte));
  S(contrat_final(Enchere));
  C(chien_devoile(Carte, Carte, Carte), chien(Carte, Carte, Carte));
  S(doit_ecarter(std::vector<Carte>, std::vector<Carte>));
  S(ecart_accepte(std::vector<Carte>));
  S(ecart_refuse(std::vector<Carte>));
  S(atout_au_chien(std::vector<Carte>));
  C(maitre(unsigned int), maitre_change(unsigned int));
  S(jeu_change(std::vector<Carte>, std::vector<Carte>));
  S(jeu_est(std::vector<Carte>));
  S(doit_jouer());
  S(requete_acceptee(Carte));
  S(requete_refusee(Carte));
  S(carte_jouee(unsigned int, Carte));
  S(carte_gagnee(Carte, unsigned int, unsigned int));
  C(pli(unsigned int), pli_termine(unsigned int));
  S(tapis_change(Tapis));
  C(score(std::vector<int>), partie_terminee(std::vector<int>));
  S(identification_acceptee(std::string));
  S(identification_refusee(std::string));
  C(identification_refusee(std::string), doit_identifier());
  S(entree(std::string));
  S(sortie(std::string));
  S(adversaires(std::vector<std::string>));
  S(invitation_acceptee(std::vector<std::string>));
  S(invitation_refusee(std::vector<std::string>));

  emit doit_identifier();
}

void ClientJeu::traiter_deconnexion()
{
  ENTER("traiter_deconnexion()");
  m_partie.disconnect(this);
}

void ClientJeu::presenter_etat()
{/*
  std::cout<<"Vous êtes le numéro "<<m_partie.mon_numero()
	   <<", on en est à la phase "<<m_partie.phase()
	   <<", vos cartes sont "<<m_partie.mon_jeu()
	   <<"."<<std::endl;*/
}

const PartieClient & ClientJeu::partie() const
{
  ENTER("partie() const");
  return m_partie;
}

void ClientJeu::traiter_message(Protocole::Message m)
{
  ENTER("traiter_message(Message m)");
  ADD_ARG("m", m);
  m_partie.assimiler(m);
}

void ClientJeu::formuler_prise(Enchere::Prise p)
{
  ENTER("formuler_prise(Enchere::Prise p)");
  ADD_ARG("p", p);
  m_partie.formuler_prise(p);
}

void ClientJeu::formuler_appel(Carte c)
{
  ENTER("formuler_appel(Carte c)");
  ADD_ARG("c", c);
  m_partie.appeler(c);
}

void ClientJeu::formuler_ecart(std::vector<Carte> ecart)
{
  ENTER("formuler_ecart(std::vector<Carte> ecart)");
  ADD_ARG("ecart", ecart);
  m_partie.ecarter(ecart);
}

void ClientJeu::formuler_requete(Carte requete)
{
  ENTER("formuler_requete(Carte requete)");
  ADD_ARG("requete", requete);
  m_partie.jouer(requete);
}

void ClientJeu::formuler_identification(std::string nom)
{
  ENTER("formuler_identification(std::string nom)");
  ADD_ARG("nom", nom);
  m_partie.identifier(nom);
}
void ClientJeu::formuler_invitation
(std::vector<std::string> adversaires)
{
  ENTER("formuler_invitation(std::vector<std::string> adversaires)");
  ADD_ARG("adversaires", adversaires);
  m_partie.formuler_invitation(adversaires);
}
