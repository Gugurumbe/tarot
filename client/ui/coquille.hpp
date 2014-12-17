/**
   @file client/ui/coquille.hpp
   @brief Permet d'afficher 5 clients à la fois.

   Une coquille est une interface graphique regroupant 5 clients
   différents dans des onglets différents, avec possibilité de les
   surveiller tous ensemble.
 */

#ifndef COQUILLE_DEFINIE

/**
   @brief Empêche les inclusions récursives.
 */
#define COQUILLE_DEFINIE

#include "ui_coquille.h"

#include <QWidget>

/**
   @brief Définition de l'interface utilisateur de la coquille.
   
   Elle comprend 5 onglets, chacun représentant un ClientGraphique à
   part entière.
 */
class Coquille : public QWidget
{
  Q_OBJECT;
public:
  
  /**
     @brief Construit le Widget.
     
     @param parent Le widget parent au sens de Qt.
   */
  Coquille(QWidget * parent = 0);

public slots:
  /**
     @brief Traité lorsque je demande à ajouter un client.
   */
  void on_bouton_plus_clicked();
  /**
     @brief Traité lorsque je demande à supprimer un client.
   */
  void on_bouton_moins_clicked();

private:
  Ui::Coquille ui;
};

#endif
