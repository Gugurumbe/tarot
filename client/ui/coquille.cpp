#include "coquille.hpp"
#include "client_graphique.hpp"

Coquille::Coquille(QWidget * parent):
  QWidget(parent)
{
  ui.setupUi(this);
}

void Coquille::on_bouton_plus_clicked()
{
  ClientGraphique * cg = new ClientGraphique(this);
  ui.onglets->addTab(cg, "Client");
}

void Coquille::on_bouton_moins_clicked()
{
  QWidget * w = ui.onglets->currentWidget();
  if(w)
    {
      w->deleteLater();
      ui.onglets->removeTab(ui.onglets->currentIndex());
    }
}
