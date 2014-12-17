#include "protocole.hpp"

//Les premières fonctions sont là pour lire la partie spécifique des
//messages. Celle appelée sera déterminée en fonction du premier octet.

bool lire_erreur_protocole(QDataStream & in, 
			   Protocole::Msg_erreur_protocole &)
{
  //Rien à remplir dans un Msg_erreur_protocole
  return in.status() == QDataStream::Ok;
}

void ecrire_erreur_protocole(Protocole::Msg_erreur_protocole 
			     const &,
			     QDataStream &)
{
}

bool lire_refuse(QDataStream & in, 
		 Protocole::Msg_refuse &)
{
  //Idem
  return in.status() == QDataStream::Ok;
}

void ecrire_refuse(Protocole::Msg_refuse const &,
		   QDataStream &)
{
}

bool lire_numero(QDataStream & in,
		 Protocole::Msg_numero & numero)
{
  quint8 num;
  in>>num;
  numero.n = num;
  return in.status() == QDataStream::Ok;
}

void ecrire_numero(Protocole::Msg_numero const & numero,
		   QDataStream & out)
{
  out<<(quint8)numero.n;
}

bool lire_distribution(QDataStream & in, 
		       Protocole::Msg_distribution & distribution)
{
  quint8 num;
  for(int i = 0 ; i < 15 ; i++)
    {
      in>>num;
      distribution.cartes[i] = num;
    }
  return in.status() == QDataStream::Ok;
}

void ecrire_distribution(Protocole::Msg_distribution 
			 const & distribution,
			 QDataStream & out)
{
  for(int i = 0 ; i < 15 ; i ++)
    {
      out<<(quint8)distribution.cartes[i];
    }
}

bool lire_prise(QDataStream & in,
		   Protocole::Msg_prise & prise)
{
  quint8 niv;
  in>>niv;
  prise.niveau = niv;
  return in.status() == QDataStream::Ok;
}

void ecrire_prise(Protocole::Msg_prise const & prise,
		     QDataStream & out)
{
  out<<(quint8)prise.niveau;
}

bool lire_contrat(QDataStream & in,
		  Protocole::Msg_contrat & contrat)
{
  quint8 niv;
  in>>niv;
  contrat.niveau = niv;
  return in.status() == QDataStream::Ok;
}

void ecrire_contrat(Protocole::Msg_contrat const & contrat,
		    QDataStream & out)
{
  out<<(quint8)contrat.niveau;
}

bool lire_appel(QDataStream & in,
		Protocole::Msg_appel &)
{
  return in.status() == QDataStream::Ok;
}

void ecrire_appel(Protocole::Msg_appel const &,
		  QDataStream &)
{
}

bool lire_appeler(QDataStream & in,
		  Protocole::Msg_appeler & appeler)
{
  quint8 carte;
  in>>carte;
  appeler.carte = carte;
  return in.status() == QDataStream::Ok;
}

void ecrire_appeler(Protocole::Msg_appeler const & appeler,
		    QDataStream & out)
{
  out<<(quint8)appeler.carte;
}

bool lire_contrat_final(QDataStream & in,
			Protocole::Msg_contrat_final & contrat_final)
{
  quint8 preneur, niv, appel;
  in>>preneur;
  in>>niv;
  in>>appel;
  contrat_final.preneur = preneur;
  contrat_final.niveau = niv;
  contrat_final.appel = appel;
  return in.status() == QDataStream::Ok;
}

void ecrire_contrat_final(Protocole::Msg_contrat_final const & contrat_final,
			  QDataStream & out)
{
  out<<(quint8)contrat_final.preneur;
  out<<(quint8)contrat_final.niveau;
  out<<(quint8)contrat_final.appel;
}

bool lire_chien(QDataStream & in,
		Protocole::Msg_chien & chien)
{
  quint8 carte;
  for(int i = 0 ; i < 3 ; i++)
    {
      in>>carte;
      chien.chien[i] = carte;
    }
  return in.status() == QDataStream::Ok;
}

void ecrire_chien(Protocole::Msg_chien const & chien,
		  QDataStream & out)
{
  for(int i = 0 ; i < 3 ; i++)
    {
      out<<(quint8)chien.chien[i];
    }
}

bool lire_ecart(QDataStream & in,
		Protocole::Msg_ecart & ecart)
{
  quint8 carte;
  for(int i = 0 ; i < 3 ; i++)
    {
      in>>carte;
      ecart.ecart[i] = carte;
    }
  return in.status() == QDataStream::Ok;
}

void ecrire_ecart(Protocole::Msg_ecart const & ecart,
		  QDataStream & out)
{
  for(int i = 0 ; i < 3 ; i ++)
    {
      out<<(quint8)ecart.ecart[i];
    }
}

bool lire_atout(QDataStream & in, 
		Protocole::Msg_atout & atout)
{
  quint8 carte;
  int i = 0;
  while(i < 3 && in.status() == QDataStream::Ok && !(in.atEnd()))
    {
      in>>carte;
      atout.cartes[i] = carte;
      i++;
    }
  atout.nombre = i;
  return in.status() == QDataStream::Ok;
}

void ecrire_atout(Protocole::Msg_atout const & atout,
		  QDataStream & out)
{
  for(int i = 0 ; i < atout.nombre ; i++)
    {
      out<<(quint8)atout.cartes[i];
    }
}

bool lire_chelem(QDataStream & in, Protocole::Msg_chelem & chelem)
{
  quint8 b;
  in>>b;
  chelem.demande = (b != 0);
  return in.status() == QDataStream::Ok;
}

void ecrire_chelem(Protocole::Msg_chelem const & chelem,
		   QDataStream & out)
{
  out<<(quint8)(chelem.demande?1:0);
}

bool lire_jeu(QDataStream & in,
	      Protocole::Msg_jeu & jeu)
{
  quint8 demandeur;
  in>>demandeur;
  jeu.chelem = demandeur;
  return in.status() == QDataStream::Ok;
}

void ecrire_jeu(Protocole::Msg_jeu const & jeu,
		QDataStream & out)
{
  out<<(quint8)jeu.chelem;
}

bool lire_montrer_poignee(QDataStream & in,
			  Protocole::Msg_montrer_poignee & montrer_poignee)
{
  quint8 atout;
  // On regarde si c'est plus qu'une simple poignée...
  for(int i = 0 ; i < 8 ; i++)
    {
      in>>atout;
      montrer_poignee.atouts[i] = atout;
    }
  if(in.atEnd() && in.status() == QDataStream::Ok)
    {
      montrer_poignee.taille = 8 ;
      return true;
    }
  // On regarde si c'est plus qu'une double poignée...
  for(int i = 8 ; i < 10 ; i++)
    {
      in>>atout;
      montrer_poignee.atouts[i] = atout;
    }
  if(in.atEnd() && in.status() == QDataStream::Ok)
    {
      montrer_poignee.taille = 10;
      return true;
    }
  //C'est peut-être une triple poignée...
  for(int i = 10 ; i < 13 ; i++)
    {
      in>>atout;
      montrer_poignee.atouts[i] = atout;
    }
  montrer_poignee.taille = 13;
  return (in.status() == QDataStream::Ok);
}

void ecrire_montrer_poignee(Protocole::Msg_montrer_poignee
			    const & montrer_poignee,
			    QDataStream & out)
{
  for(int i = 0 ; i < montrer_poignee.taille && i < 13 ; i++)
    {
      out<<(quint8)(montrer_poignee.atouts[i]);
    }
}

bool lire_poignee(QDataStream & in, Protocole::Msg_poignee & poignee)
{
  quint8 atout;
  // On regarde si c'est plus qu'une simple poignée...
  for(int i = 0 ; i < 8 ; i++)
    {
      in>>atout;
      poignee.atouts[i] = atout;
    }
  if(in.atEnd() && in.status() == QDataStream::Ok)
    {
      poignee.taille = 8 ;
      return true;
    }
  // On regarde si c'est plus qu'une double poignée...
  for(int i = 8 ; i < 10 ; i++)
    {
      in>>atout;
      poignee.atouts[i] = atout;
    }
  if(in.atEnd() && in.status() == QDataStream::Ok)
    {
      poignee.taille = 10;
      return true;
    }
  //C'est peut-être une triple poignée...
  for(int i = 10 ; i < 13 ; i++)
    {
      in>>atout;
      poignee.atouts[i] = atout;
    }
  poignee.taille = 13;
  return (in.status() == QDataStream::Ok);
}

void ecrire_poignee(Protocole::Msg_poignee const & poignee,
			    QDataStream & out)
{
  for(int i = 0 ; i < poignee.taille && i < 13 ; i++)
    {
      out<<(quint8)(poignee.atouts[i]);
    }
}

bool lire_requete(QDataStream & in,
		  Protocole::Msg_requete & requete)
{
  quint8 carte;
  in>>carte;
  requete.carte = carte;
  return in.status() == QDataStream::Ok;
}

void ecrire_requete(Protocole::Msg_requete const & requete,
		    QDataStream & out)
{
  out<<(quint8)requete.carte;
}

bool lire_carte(QDataStream & in,
		Protocole::Msg_carte & carte)
{
  quint8 carte_lue;
  in>>carte_lue;
  carte.carte = carte_lue;
  return in.status() == QDataStream::Ok;
}

void ecrire_carte(Protocole::Msg_carte const & carte,
		  QDataStream & out)
{
  out<<(quint8)carte.carte;
}

bool lire_pli(QDataStream & in,
	      Protocole::Msg_pli & pli)
{
  quint8 j;
  in>>j;
  pli.joueur = j;
  return in.status() == QDataStream::Ok;
}

void ecrire_pli(Protocole::Msg_pli const & pli,
		QDataStream & out)
{
  out<<(quint8)pli.joueur;
}

bool lire_resultat(QDataStream & in,
		   Protocole::Msg_resultat & resultat)
{
  qint16 points;
  for(int i = 0 ; i < 5 ; i ++)
    {
      in>>points;
      resultat.resultats[i] = points;
    }
  return in.status() == QDataStream::Ok;
}

void ecrire_resultat(Protocole::Msg_resultat const & resultat,
		     QDataStream & out)
{
  for(int i = 0 ; i < 5 ; i ++)
    {
      out<<(qint16)resultat.resultats[i];
    }
}

//Fonctions générale :

bool Protocole::lire(QDataStream & in, Protocole::Message & m)
{
  quint8 type;
  in>>type;
  bool lu = false;
  m.compris = true;
  if(in.status() == QDataStream::Ok)
    {
      m.type = type;
      switch(type)
	{
	case Protocole::ERREUR_PROTOCOLE:
	  //C'est moi qui ai fait une erreur de protocole la dernière fois,
	  //le serveur me le signale
	  lu = lire_erreur_protocole(in, m.m.erreur_protocole);
	  break;
	case Protocole::REFUSE:
	  lu = lire_refuse(in, m.m.refuse);
	  break;
	case Protocole::NUMERO:
	  lu = lire_numero(in, m.m.numero);
	  break;
	case Protocole::DISTRIBUTION:
	  lu = lire_distribution(in, m.m.distribution);
	  break;
	case Protocole::PRISE:
	  lu = lire_prise(in, m.m.prise);
	  break;
	case Protocole::CONTRAT:
	  lu = lire_contrat(in, m.m.contrat);
	  break;
	case Protocole::APPEL:
	  lu = lire_appel(in, m.m.appel);
	  break;
	case Protocole::APPELER:
	  lu = lire_appeler(in, m.m.appeler);
	  break;
	case Protocole::CONTRAT_FINAL:
	  lu = lire_contrat_final(in, m.m.contrat_final);
	  break;
	case Protocole::CHIEN:
	  lu = lire_chien(in, m.m.chien);
	  break;
	case Protocole::ECART:
	  lu = lire_ecart(in, m.m.ecart);
	  break;
	case Protocole::ATOUT:
	  lu = lire_atout(in, m.m.atout);
	  break;
	case Protocole::CHELEM:
	  lu = lire_chelem(in, m.m.chelem);
	  break;
	case Protocole::JEU:
	  lu = lire_jeu(in, m.m.jeu);
	  break;
	case Protocole::MONTRER_POIGNEE:
	  lu = lire_montrer_poignee(in, m.m.montrer_poignee);
	  break;
	case Protocole::POIGNEE:
	  lu = lire_poignee(in, m.m.poignee);
	  break;
	case Protocole::REQUETE:
	  lu = lire_requete(in, m.m.requete);
	  break;
	case Protocole::CARTE:
	  lu = lire_carte(in, m.m.carte);
	  break;
	case Protocole::PLI:
	  lu = lire_pli(in, m.m.pli);
	  break;
	case Protocole::RESULTAT:
	  lu = lire_resultat(in, m.m.resultat);
	  break;
	default:
	  // Je n'ai pas compris le message du serveur. Je peux si je le 
	  // souhaite enoyer au serveur un message de type "erreur_protocole".
	  break;
	}
    }
  m.compris = lu;
  return lu;
}

void Protocole::ecrire(Protocole::Message const & m, QDataStream & out)
{
  out<<(quint8)m.type;
  switch(m.type)
    {
    case Protocole::ERREUR_PROTOCOLE:
      ecrire_erreur_protocole(m.m.erreur_protocole, out);
      break;
    case Protocole::REFUSE:
      ecrire_refuse(m.m.refuse, out);
      break;
    case Protocole::NUMERO:
      ecrire_numero(m.m.numero, out);
      break;
    case Protocole::DISTRIBUTION:
      ecrire_distribution(m.m.distribution, out);
      break;
    case Protocole::PRISE:
      ecrire_prise(m.m.prise, out);
      break;
    case Protocole::CONTRAT:
      ecrire_contrat(m.m.contrat, out);
      break;
    case Protocole::APPEL:
      ecrire_appel(m.m.appel, out);
      break;
    case Protocole::APPELER:
      ecrire_appeler(m.m.appeler, out);
      break;
    case Protocole::CONTRAT_FINAL:
      ecrire_contrat_final(m.m.contrat_final, out);
      break;
    case Protocole::CHIEN:
      ecrire_chien(m.m.chien, out);
      break;
    case Protocole::ECART:
      ecrire_ecart(m.m.ecart, out);
      break;
    case Protocole::ATOUT:
      ecrire_atout(m.m.atout, out);
      break;
    case Protocole::CHELEM:
      ecrire_chelem(m.m.chelem, out);
      break;
    case Protocole::JEU:
      ecrire_jeu(m.m.jeu, out);
      break;
    case Protocole::MONTRER_POIGNEE:
      ecrire_montrer_poignee(m.m.montrer_poignee, out);
      break;
    case Protocole::POIGNEE:
      ecrire_poignee(m.m.poignee, out);
      break;
    case Protocole::REQUETE:
      ecrire_requete(m.m.requete, out);
      break;
    case Protocole::CARTE:
      ecrire_carte(m.m.carte, out);
      break;
    case Protocole::PLI:
      ecrire_pli(m.m.pli, out);
      break;
    case Protocole::RESULTAT:
      ecrire_resultat(m.m.resultat, out);
      break;
    default:
      break;
    }
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_erreur_protocole &)
{
  return out<<"<erreur_protocole>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_refuse &)
{
  return out<<"<refuse>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_numero & m)
{
  return out<<"<numero : n="<<m.n<<">";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_distribution & m)
{
  out<<"<distribution : cartes=[|";
  for(unsigned int i = 0 ; i < 14 ; i++)
    {
      out<<m.cartes[i]<<" ; ";
    }
  out<<m.cartes[14];
  out<<"|]>";
  return out;
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_prise & m)
{
  return out<<"<prise : niveau="<<m.niveau<<">";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_contrat & m)
{
  return out<<"<contrat : niveau="<<m.niveau<<">";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_appel &)
{
  return out<<"<appel>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_appeler & m)
{
  return out<<"<appeler : carte="<<m.carte<<">";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_contrat_final & m)
{
  return out<<"<contrat_final : preneur="<<m.preneur
	    <<", niveau="<<m.niveau
	    <<", appel="<<m.appel
	    <<">";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_chien & m)
{
  return out<<"<chien : chien=[|"
	    <<m.chien[0]<<" ; "
	    <<m.chien[1]<<" ; "
	    <<m.chien[2]<<"|]>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_ecart & m)
{
  return out<<"<ecart : ecart=[|"
	    <<m.ecart[0]<<" ; "
	    <<m.ecart[1]<<" ; "
	    <<m.ecart[2]<<"|]>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_atout & m)
{
  out<<"<atout : cartes=[|";
  for(int i = 0 ; i + 1 < m.nombre ; i++)
    {
      out<<m.cartes[i]<<" ; ";
    }
  return out<<m.cartes[m.nombre - 1]<<"|]>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_chelem &)
{
  return out<<"<chelem>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_jeu &)
{
  return out<<"<jeu>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_montrer_poignee & m)
{
  out<<"<montrer_poignee : atouts=[|";
  for(int i = 0 ; i + 1 < m.taille ; i++)
    {
      out<<m.atouts[i]<<" ; ";
    }
  return out<<m.atouts[m.taille - 1]<<"|]>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_poignee & m)
{
  out<<"<poignee : atouts=[|";
  for(int i = 0 ; i + 1 < m.taille ; i++)
    {
      out<<m.atouts[i]<<" ; ";
    }
  return out<<m.atouts[m.taille - 1]<<"|]>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_requete & m)
{
  return out<<"<requete : carte="<<m.carte<<">";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_carte & m)
{
  return out<<"<carte : carte="<<m.carte<<">";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_pli & m)
{
  return out<<"<pli : joueur="<<m.joueur<<">";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Msg_resultat & m)
{
  out<<"<resultat : resultats=[|";
  for(unsigned int i = 0 ; i < 4 ; i++)
    {
      out<<m.resultats[i]<<" ; ";
    }
  return out<<m.resultats[4]<<"|]>";
}

std::ostream & operator<<(std::ostream & out, 
			  const Protocole::Message & m)
{
  if(m.compris) out<<" compris : ";
  else out<<"incompris : ";
  switch(m.type)
    {
    case Protocole::ERREUR_PROTOCOLE:out<<m.m.erreur_protocole;break;
    case Protocole::REFUSE:out<<m.m.refuse;break;
    case Protocole::NUMERO:out<<m.m.numero;break;
    case Protocole::DISTRIBUTION:out<<m.m.distribution;break;
    case Protocole::PRISE:out<<m.m.prise;break;
    case Protocole::CONTRAT:out<<m.m.contrat;break;
    case Protocole::APPEL:out<<m.m.appel;break;
    case Protocole::APPELER:out<<m.m.appeler;break;
    case Protocole::CONTRAT_FINAL:out<<m.m.contrat_final;break;
    case Protocole::CHIEN:out<<m.m.chien;break;
    case Protocole::ECART:out<<m.m.ecart;break;
    case Protocole::ATOUT:out<<m.m.atout;break;
    case Protocole::CHELEM:out<<m.m.chelem;break;
    case Protocole::JEU:out<<m.m.jeu;break;
    case Protocole::MONTRER_POIGNEE:out<<m.m.montrer_poignee;break;
    case Protocole::POIGNEE:out<<m.m.poignee;break;
    case Protocole::REQUETE:out<<m.m.requete;break;
    case Protocole::CARTE:out<<m.m.carte;break;
    case Protocole::PLI:out<<m.m.pli;break;
    case Protocole::RESULTAT:out<<m.m.resultat;break;
    default:out<<"<type inconnu>";
    }
  return out;
}
