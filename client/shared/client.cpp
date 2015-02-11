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


#include "client.hpp"
#include <iostream>

#define NOM_CLASSE "Client"

#include "deboguer.hpp"
//À des fins de débogage.

//#include <QTimer>

Client::Client(QObject * parent): QObject(parent),
				  taille_paquet(-1),
				  taille_restante(0)
{
  ENTER("Client(QObject * parent)");
  ADD_ARG("parent", parent);
  QObject::connect(&sock, SIGNAL(connected()), 
		   this, SLOT(connexion_reussie()));
  QObject::connect(&sock, SIGNAL(disconnected()),
		   this, SIGNAL(deconnecte()));
  QObject::connect(&sock, SIGNAL(readyRead()),
		   this, SLOT(recevoir()));
  QObject::connect(&sock, SIGNAL(bytesWritten(qint64)),
		   this, SLOT(envoyer_suivant(qint64)));
  //Connexion de la socket pour transmettre les signaux connected() et
  //disconnected(), et pour vérifier la présence d'un message.
}

void Client::connexion_reussie()
{
  ENTER("connexion_reussie()");
  hote = sock.peerAddress();
  emit connecte();
}

void Client::connecter(QString addr, unsigned int port)
{
  ENTER("connecter(QString addr, unsigned int port)");
  ADD_ARG("addr.toStdString()", 
	  addr.toStdString());
  ADD_ARG("port", port);
  sock.connectToHost(addr, port);
  //Connexion
  Client::port = port;
  //Préparation de la reconnexion
}

void Client::reconnecter()
{
  ENTER("reconnecter()");
  sock.connectToHost(hote, port);
}

void Client::deconnecter()
{
  ENTER("deconnecter()");
  sock.disconnectFromHost();
}

void Client::envoyer(Protocole::Message m)
{
  ENTER("envoyer(Message m)");
  ADD_ARG("m", m);
  QByteArray paquet;
  QDataStream out(&paquet, QIODevice::WriteOnly);
  Protocole::ecrire(m, out);
  envoyer(paquet.prepend((quint8)(1 + paquet.size())));
  //On envoie.
  emit emis(m);
}

void Client::envoyer(QByteArray p)
{
  ENTER("envoyer(QByteArray p)");
  ADD_ARG("p.toHex().data", p.toHex().data());
  if(taille_restante <= 0)
    {
      sock.write(p);
      taille_restante = p.size();
      //On envoie le paquet tel quel.
    }
  else file_attente.push(p);
}

void Client::recevoir()
{
  ENTER("recevoir()");
  if(taille_paquet>=0)
    {
      DEBUG<<"J'attends un paquet de taille "
	   <<taille_paquet<<std::endl;
      if(sock.bytesAvailable() >= taille_paquet)
	{
	  DEBUG<<"Je peux lire !"<<std::endl;
	  Protocole::Message m;
	  QByteArray paquet = sock.read(taille_paquet);
	  QDataStream in(paquet);
	  if(Protocole::lire(in, m))
	    {
	      DEBUG<<"J'ai lu : "<<m<<std::endl;
	      taille_paquet = -1;
	      emit recu(m);
	      recevoir();
	    }
	  else
	    {
	      ERROR<<"Erreur : la taille n'est pas convenable."
		   <<std::endl;
	    }
	}
      else
	{
	  DEBUG<<"C'est pas pour tout de suite."
	       <<std::endl;
	}
    }
  else if(sock.bytesAvailable() >= 1)
    {
      DEBUG<<"Je peux lire la taille..."<<std::endl;
      QDataStream in(&sock);
      quint8 tmp;
      in>>tmp;
      taille_paquet = static_cast<int>(tmp) - 1;
      DEBUG<<"Taille : "<<taille_paquet<<std::endl;
      recevoir();
    }
  
  //QTimer::singleShot(1000, this, SLOT(recevoir()));
}

void Client::unread(QByteArray const & paquet)
{
  ENTER("unread(QByteArray const & paquet)");
  ADD_ARG("paquet.toHex()", paquet.toHex().data());
  for(int i = paquet.size() - 1 ; i >= 0 ; i--)
    {
      DEBUG<<"unget de "<<paquet[i]<<"."<<std::endl;
      sock.ungetChar(paquet[i]);
    }
}

void Client::envoyer_suivant(qint64 taille_ecrite)
{
  taille_restante -= taille_ecrite;
  if(taille_restante <= 0)
    {
      if(file_attente.empty())
	{
	  taille_restante = 0;
	}
      else
	{
	  QByteArray paquet = file_attente.front();
	  file_attente.pop();
	  taille_restante = paquet.size();
	  sock.write(paquet);
	}
    }
}
