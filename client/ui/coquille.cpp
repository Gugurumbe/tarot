#include "coquille.hpp"
#include "config.hpp"
#include "client_graphique.hpp"

Coquille::Coquille(QWidget * parent):
  QWidget(parent)
{
  ui.setupUi(this);
  ui.adresse->setText(AUTO_ADDR);
  ui.port->setValue(AUTO_PORT);
}

void Coquille::on_bouton_plus_clicked()
{
  ClientGraphique * cg = new ClientGraphique(this);
  QHostAddress adresse(ui.adresse->text());
  quint16 port(ui.port->value());
  cg->connecter(adresse, port);
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
