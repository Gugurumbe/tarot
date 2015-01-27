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
   @file client/shared/client.hpp
   @brief Définit le moteur "réseau" du client et de l'IA.
 */

#ifndef CLIENT_DEFINI

/**
   Empêche les inclusions récursives.
 */
#define CLIENT_DEFINI

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include "protocole.hpp"
#include <queue>

/**
   @brief Définit une classe communiquant avec un serveur sur le
   réseau.
   
   La classe Client utilise les outils Qt pour établir une
   transmission en mode connecté (Tcp), grâce à laquelle il peut
   envoyer et recevoir des Messages tels que définis dans
   shared/protocole.hpp. 
   
   Telle quelle, cette classe ne sait pas quoi faire des Messages
   reçus et de la déconnexion du serveur, et ne sait pas quand envoyer
   des Messages au serveur. Il y a pour l'instant 2 comportements :
   ClientDebogage, qui demande à l'utilisateur d'entrer un message
   puis qui attend de recevoir un message, redemande à l'utilisateur
   d'entrer un message et ainsi de suite jusqu'à la déconnexion, et
   ClientJeu, en construction, qui va réagir dans un environnement de
   tarot. 

   Cette classe peut être utilisée aussi bien pour un client graphique
   que pour un client autonome (IA).

   Il existe une symétrie de construction entre le Client et le
   Serveur : une classe générale qui relaye des Message à une classe
   spécialisée pour le débogage ou à une classe spécialisée pour le
   jeu, qui utilise une spécialisation de Partie.
 */
class Client: public QObject
{
  Q_OBJECT;
public:

  /**
     @brief Constructeur par défaut.
     
     N'ouvre aucune connexion pour l'instant.
   */
  Client(QObject * parent = 0);

  /**
     @brief Considère des octets comme n'ayant pas été reçus.
     
     @param paquet : les octets lus par erreur.
   */
  void unread(QByteArray const & paquet);
public slots:

  /**
     @brief Tente de se reconnecter.
     
     Utilise les informations de la précédente connexion.
     
     @note La connexion échouera si on n'a pas établi une première
     connexion avec Client::connecter(QHostAddress, unsigned int).
   */
  void reconnecter();

  /**
     @brief Tente d'établir une connexion.

     Tente d'établir une connexion à une certaine adresse sur un
     certain port. 

     @param hote : l'adresse IP (selon Qt) de l'hôte. La façon la plus
     simple de construire une QHostAddress valide est : 
     QHostAddress("123.45.67.89"); si votre adresse IP est
     123.45.67.89. Dans tous les cas, l'adresse de votre machine
     depuis cette même machine est 127.0.0.1, ou "localhost".

     @param port : le port de listage. Celui retourné par la méthode
     Serveur::ouvrir_global() ou Serveur::ouvrir_local().
   */
  void connecter(QHostAddress hote, unsigned int port);

  /**
     @brief Quitte la connexion.

     En fin d'exécution, ce n'est pas nécessaire.

     @warning En cours d'exécution, il faut vérifier que le signal
     Client::deconnecte() ne soit pas connecté au slot
     Client::reconnecter(), car sinon la connexion est immédiatement
     rétablie.
   */
  void deconnecter();

  /**
     @brief Envoie un message au serveur.
     
     @param m : le message à envoyer.
     
     Ce slot est à préférer à Client::envoyer(QByteArray).
   */
  void envoyer(Protocole::Message m);

  /**
     @brief Envoie un paquet au serveur.

     @note Sauf en cas de débogage, il est préférable d'utiliser la
     méthode Client::envoyer(Message).
     
     @param p : le paquet brut à envoyer.
   */
  void envoyer(QByteArray p);
  
signals:

  /**
     @brief Émis lorsque le client est connecté.
     
     Détermine également le moment où le client à finalement réussi à
     se connecter au serveur.
   */
  void connecte();

  /**
     @brief Émis lorsque le client est déconnecté.

     Également émis lorsque le client est déconnecté par
     Client::deconnecter(). 
   */
  void deconnecte();

  /**
     @brief Émis lorsqu'un message est reçu du serveur.

     Il n'y a pas d'équivalent « Client::recu_brut(QByteArray) ». Si
     cela manque vraiment, je le rajouterai.

     @param m Le Message reçu du serveur.
   */
  void recu(Protocole::Message m);
		      
  /**
     @brief Émis lorsqu'un message est envoyé au serveur.
     
     @param m Le message en question.
   */
  void emis(Protocole::Message m);
private slots:

  /**
     @brief Vérifie si de nouvelles données sont disponibles.

     Ce slot est déclenché automatiquement.
   */
  void recevoir();

  /**
     @brief Envoie les données suivantes.
     @param taille_ecrite La taille de ce qu'on a écrit depuis la
     dernière fois.
   */
  void envoyer_suivant(qint64 taille_ecrite);
  
private:
  
  /**
     @brief Objet Qt pour représenter une socket.
  */
  QTcpSocket sock;

  /**
     @brief Utilisé pour la reconnexion.
   */
  QHostAddress hote;

  /**
     @brief Utilisé pour la reconnexion.
   */
  unsigned int port;
  
  int taille_paquet; //Utilisé pour la lecture

  qint64 taille_restante;
  std::queue<QByteArray> file_attente;
};

#ifndef PRINT_ARRAY

/**
   @brief Empêche de définir deux fois l'écriture d'un tableau.
   
   @see shared/debogueur.hpp
 */
#define PRINT_ARRAY

/**
   @brief Permet d'écrire un tableau dans un flux.
   @param[out] out Le flux de sortie où il faut écrire.
   @param t Le tableau d'objets.

   L'écriture se fait sous la forme [|<obj 1> ; <obj 2>|]
   
   @warning La méthode 
   std::ostream & operator<<(std::ostream &, const T &)
   doit être définie.

   @return La suite du flux.
 */
template<class T>
std::ostream & operator<<(std::ostream & out, const std::vector<T> & t)
{
  out<<"[|";
  for(unsigned int i = 0 ; i < t.size() - 1 ; i++)
    {
      out<<t[i]<<" ; ";
    }
  out<<t[t.size() - 1]<<"|]";
  return out;
}

#endif

#endif
