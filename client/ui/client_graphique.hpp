/**
   @file client/ui/client_graphique.hpp
   @brief Classe d'interface graphique pour le client de tarot virtuel.
 */

#ifndef CLIENT_GRAPHIQUE_DEFINI

/**
   @brief Empêche les inclusions récursives.
 */
#define CLIENT_GRAPHIQUE_DEFINI

#include "ui_client.h"

#include "client_jeu.hpp"

#include <QWidget>

/**
   @brief Définition de la classe d'interface graphique principale.
 */
class ClientGraphique : public QWidget
{
  Q_OBJECT;
public:
  
  /**
     @brief Construit le Widget.
     
     @param parent Le widget parent au sens de Qt.
  */
  ClientGraphique(QWidget * parent = 0);

public slots:

  /**
     @brief Envoie un message "prise".
     
     Voir l'interface graphique client.ui.
   */
  void on_bouton_enchere_clicked();
  /**
     @brief Envoie un message "appeler"
   */
  void on_bouton_appel_clicked();
  /**
     @brief Envoie un message "ecarter"
   */
  void on_bouton_ecart_clicked();
  /**
     @brief Envoie un message "requete"
   */
  void on_bouton_requete_clicked();

  /**
     @brief Traité lorsque le chien est dévoilé.
     
     @param c1 La première carte du chien.
     @param c2 La deuxième carte du chien.
     @param c3 La troisième carte du chien.
   */
  void chien(Carte c1, Carte c2, Carte c3);

  /**
     @brief Traité en cas de connexion réussie.
   */
  void enable();

  /**
     @brief Traité en cas de déconnexion.
   */
  void disable();
  
  /**
     @brief Connecte le client.
     
     @param hote L'adresse de l'hôte.
     @param port Le numéro de port.

     @see client/ui/config.hpp
   */
  void connecter(const QHostAddress & hote, quint16 port);
  
private:
  Ui::ClientGraphique ui;
  ClientJeu jeu;
};

#endif
