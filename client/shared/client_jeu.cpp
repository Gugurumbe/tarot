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
}

void ClientJeu::traiter_deconnexion()
{
  ENTER("traiter_deconnexion()");
  m_partie.disconnect(this);
  reconnecter();
  //C'est critiquable : si le serveur nous déconnecte délibérément,
  //c'est la boucle infinie. Mais bon, le serveur doit savoir résister
  //à ce genre d'attaque.
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

void ClientJeu::formuler_appel(const Carte & c)
{
  ENTER("formuler_appel(const Carte & c)");
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
