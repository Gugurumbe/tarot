#include "client_graphique.hpp"
#include "config.hpp" 
ClientGraphique::ClientGraphique(QWidget * parent): 
  QWidget(parent), jeu(this)
{
  ui.setupUi(this);
  //Remplissage des SelecteurCarte :
  ui.selecteur_appel->afficher_toutes();
  ui.ecart_1->afficher_toutes();
  ui.ecart_2->afficher_toutes();
  ui.ecart_3->afficher_toutes();
  //L'autre est cartes_jouables
#define C(signal) connect(&jeu, SIGNAL(signal), \
			  ui.journal, SLOT(signal));
  setEnabled(false);
  C(connecte());
  C(deconnecte());
  C(numero_change(unsigned int));
  C(doit_priser(Option<Enchere>));
  C(enchere_acceptee(Enchere));
  C(enchere_refusee(Enchere));
  C(contrat_intermediaire(Enchere));
  C(doit_appeler(std::vector<Carte>));
  C(appel_accepte(Carte));
  C(appel_refuse(Carte));
  C(contrat_final(Enchere));
  C(chien(Carte, Carte, Carte));
  C(doit_ecarter(std::vector<Carte>, std::vector<Carte>));
  C(ecart_accepte(std::vector<Carte>));
  C(ecart_refuse(std::vector<Carte>));
  C(atout_au_chien(std::vector<Carte>));
  C(maitre_change(unsigned int));
  C(jeu_change(std::vector<Carte>, std::vector<Carte>));
  C(doit_jouer());
  C(requete_acceptee(Carte));
  C(requete_refusee(Carte));
  C(carte_jouee(unsigned int, Carte));
  C(carte_gagnee(Carte, unsigned int, unsigned int));
  C(pli_termine(unsigned int));
  C(tapis_change(Tapis));
  C(partie_terminee(std::vector<int>));
  connect(&jeu, SIGNAL(numero_change(unsigned int)),
	  ui.nom_joueur, SLOT(set_num(unsigned int)));
  connect(&jeu, SIGNAL(jeu_change
		       (std::vector<Carte>, std::vector<Carte>)),
	  ui.mes_cartes, SLOT(gagner
			      (std::vector<Carte>, 
			       std::vector<Carte>)));
  connect(&jeu, SIGNAL(jeu_est(std::vector<Carte>)),
	  ui.mes_cartes, SLOT(set(std::vector<Carte>)));
  connect(&jeu, SIGNAL(jeu_change
		       (std::vector<Carte>, std::vector<Carte>)),
	  ui.cartes_jouables, SLOT(modifier_cartes
				   (std::vector<Carte>,
				    std::vector<Carte>)));
  connect(&jeu, SIGNAL(jeu_est(std::vector<Carte>)),
	  ui.cartes_jouables, SLOT(set_cartes(std::vector<Carte>)));
  connect(&jeu, SIGNAL(tapis_change(Tapis)),
	  ui.tapis, SLOT(recalculer(Tapis)));
  connect(&jeu, SIGNAL(chien(Carte, Carte, Carte)), 
	  this, SLOT(chien(Carte, Carte, Carte)));
  connect(&jeu, SIGNAL(maitre_change(unsigned int)),
	  ui.nom_maitre, SLOT(set_num(unsigned int)));
  connect(&jeu, SIGNAL(tapis_change(const Tapis &)),
	  ui.tapis, SLOT(recalculer(const Tapis &)));
  connect(&jeu, SIGNAL(connecte()),
	  this, SLOT(enable()));
  connect(&jeu, SIGNAL(deconnecte()),
	  this, SLOT(disable()));
}

void ClientGraphique::on_bouton_enchere_clicked()
{
  //Récupération de l'enchère
  Enchere::Prise p;
  if(ui.bouton_passe->isChecked())
    {
      p = Enchere::PASSE;
    }
  else if(ui.bouton_prise->isChecked())
    {
      p = Enchere::PRISE;
    }
  else if(ui.bouton_garde->isChecked())
    {
      p = Enchere::GARDE;
    }
  else if(ui.bouton_garde_sans->isChecked())
    {
      p = Enchere::GARDE_SANS;
    }
  else if(ui.bouton_garde_contre->isChecked())
    {
      p = Enchere::GARDE_CONTRE;
    }
  else p = static_cast<Enchere::Prise>(ui.selection_enchere->value());
  jeu.formuler_prise(p);
}

void ClientGraphique::on_bouton_appel_clicked()
{
  Carte c = ui.selecteur_appel->carte_selectionnee();
  jeu.formuler_appel(c);
}

void ClientGraphique::on_bouton_ecart_clicked()
{
  std::vector<Carte> ecart;
  ecart.push_back(ui.ecart_1->carte_selectionnee());
  ecart.push_back(ui.ecart_2->carte_selectionnee());
  ecart.push_back(ui.ecart_3->carte_selectionnee());
  jeu.formuler_ecart(ecart);
}

void ClientGraphique::chien(Carte c1, Carte c2, Carte c3)
{
  std::vector<Carte> cartes;
  cartes.push_back(c1);
  cartes.push_back(c2);
  cartes.push_back(c3);
  ui.chien_1->setText(QString::fromUtf8(c1.nom().c_str()));
  ui.chien_2->setText(QString::fromUtf8(c2.nom().c_str()));
  ui.chien_3->setText(QString::fromUtf8(c3.nom().c_str()));
}

void ClientGraphique::on_bouton_requete_clicked()
{
  Carte c(ui.carte_hack->value());
  if(ui.bouton_possede->isChecked())
    {
      //On demande une Carte possédée.
      c = ui.cartes_jouables->carte_selectionnee();
    }
  jeu.formuler_requete(c);
}

void ClientGraphique::enable()
{
  setEnabled(true);
}

void ClientGraphique::disable()
{
  setDisabled(true);
}

void ClientGraphique::connecter(const QHostAddress & hote,
				quint16 port)
{
  jeu.connecter(hote, port);
}
