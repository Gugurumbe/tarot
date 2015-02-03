/*
  serveur of project tarot virtuel is a server for a card game.
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


#include "partie_serveur.hpp"
#include "debogueur.hpp"
#include "config.hpp"

#define NOM_CLASSE "PartieServeur"

#include "deboguer.hpp"

#define EMETTRE_A_TOUS(m) 		\
  for(unsigned int i = 0 ; i < 4 ; i++) \
    emit doit_emettre(i, m, false);	\
  emit doit_emettre(4, m, true);

PartieServeur::PartieServeur(QObject * parent):
  QObject(parent), Partie(), jeu_reel(5),
  joueur_appele(5), plis_restants(15)
{
  ENTER("PartieServeur(QObject * parent)");
  ADD_ARG("parent", (void *)parent);
}

void PartieServeur::assimiler(Protocole::Message const & message)
{
  ENTER("assimiler(Message message)");
  ADD_ARG("message", message);
  Partie::assimiler(message);
  /* Le serveur met à jour les mains des joueurs et le tapis. */
  switch(message.type)
    {
    case Protocole::ERREUR_PROTOCOLE:
      DEBUG<<"Un client n'a pas compris mon message."<<std::endl;
      break;
    case Protocole::REFUSE:
      DEBUG<<"Le Message a été refusé."<<std::endl;
      break;
    case Protocole::NUMERO:
      DEBUG<<"Rien à faire."<<std::endl;
      break;
    case Protocole::DISTRIBUTION:
      //les jeux sont déjà à jour.
      DEBUG<<"Rien à faire."<<std::endl;
      break;
    case Protocole::PRISE:
      DEBUG<<"Fabrication d'un Message de type contrat."<<std::endl;
      Protocole::Message contrat;
      contrat.type = Protocole::CONTRAT;
      contrat.m.contrat.niveau = message.m.prise.niveau;
      EMETTRE_A_TOUS(contrat);
      break;
    case Protocole::CONTRAT:
      if(true)
	{
	  Enchere e((tour() + 4) % 5, message.m.contrat);
	  DEBUG<<"L'enchère a la valeur "
	       <<(int)(e.prise())
	       <<" = "
	       <<message.m.contrat.niveau<<std::endl;
	  DEBUG<<"L'enchère maximale a la valeur "
	       <<(int)(e_max.prise())
	       <<std::endl;
	  if(e > e_max)
	    {
	      e_max = e;
	    }
	  DEBUG<<"e_max a la valeur "<<e_max.prise()<<std::endl;
	}
      if(tour() >= 5)
	{
	  //Les enchères sont terminées.
	  //Détermination de l'enchère maximale.
	  DEBUG<<"Enchères terminées."<<std::endl;
	  int i_attaquant = 0;
	  for(int i = 0 ; i < 5 ; i++)
	    {
	      if(enchere_de(i) > enchere_de(i_attaquant))
		{
		  i_attaquant = i;
		}
	    }
	  if(enchere_de(i_attaquant).prise())
	    {
	      //Quelqu'un a pris.
	      set_attaquant(i_attaquant);
	      DEBUG<<"Attaquant : "<<i_attaquant<<std::endl;
	      //Envoi du message "appel"
	      Protocole::Message appel;
	      appel.type = Protocole::APPEL;
	      emit doit_emettre(i_attaquant, appel, true);
	    }
	  else
	    {
	      //Personne n'a pris.
	      DEBUG<<"Personne n'a pris."<<std::endl;
	      set_phase(Partie::FIN);
	      Protocole::Message fin;
	      fin.type = Protocole::RESULTAT;
	      for(unsigned int i = 0 ; i < 5 ; i++)
		{
		  fin.m.resultat.resultats[i] = 0;
		}
	      EMETTRE_A_TOUS(fin);
	    }
	}
    case Protocole::APPEL:
      DEBUG<<"Rien à faire."<<std::endl;
      break;
    case Protocole::APPELER:
      Protocole::Message mess_contrat_final;
      mess_contrat_final.type = Protocole::CONTRAT_FINAL;
      mess_contrat_final.m.contrat_final.preneur = attaquant();
      mess_contrat_final.m.contrat_final.niveau = 
	(int)donner_contrat_final().prise();
      mess_contrat_final.m.contrat_final.appel = 
	message.m.appeler.carte;
      EMETTRE_A_TOUS(mess_contrat_final);
      if(donner_contrat_final().prise() >= Enchere::GARDE_SANS)
	{
	  DEBUG<<"On ne montre pas le chien."<<std::endl;
	  if(donner_contrat_final().prise() == Enchere::GARDE_SANS)
	    {
	      DEBUG<<"L'attaque gagne quand même les cartes du chien."
		   <<std::endl;
	      cartes_attaque.push_back(chien);
	    }
	  else
	    {
	      DEBUG<<"La défense gagne les cartes du chien."
		   <<std::endl;
	      cartes_defense.push_back(chien);
	    }
	  Protocole::Message jeu;
	  jeu.type = Protocole::JEU;
	  jeu.m.jeu.ouvreur = (chelem() >= 0 ? chelem() : 0);
	  EMETTRE_A_TOUS(jeu);
	}
      else
	{
	  // Il faut montrer le chien
	  Protocole::Message mess_chien;
	  DEBUG<<"Chien : ["<<std::endl;
	  mess_chien.type = Protocole::CHIEN;
	  for(unsigned int i = 0 ; i < 3 ; i++)
	    {
	      DEBUG<<chien[i].numero()<<std::endl;
	      mess_chien.m.chien.chien[i] = chien[i].numero();
	    }
	  DEBUG<<"]"<<std::endl;
	  EMETTRE_A_TOUS(mess_chien);
	}
      // Cherche le joueur appelé
      for(unsigned int i = 0 ; i < jeu_reel.size() ; i++)
	{
	  if(jeu_reel[i].possede(Carte(message.m.appeler.carte)))
	    {
	      DEBUG<<"Le joueur appelé est "<<i<<std::endl;
	      joueur_appele = i;
	      i = jeu_reel.size();
	    }
	}
      if(joueur_appele >= 5)
	{
	  //Dans le chien
	  DEBUG<<"La carte appelée est dans le chien."<<std::endl;
	  joueur_appele = attaquant();
	}
      break;
    case Protocole::CHIEN:
      //On donne à l'attaquant les cartes du chien.
      DEBUG<<"Don des 3 cartes du chien à l'attaquant."<<std::endl;
      for(unsigned int i = 0 ; i < chien.size() ; i++)
	{
	  jeu_reel[attaquant()].ajouter(chien[i]);
	}
      break;
    case Protocole::ECART:
      if(true) //On n'aime pas les déclarations dans les "case"
	{
	  DEBUG<<"Gestion d'un écart..."<<std::endl;
	  std::vector<Carte> ecart;
	  for(unsigned int i = 0 ; i < 3 ; i++)
	    {
	      ecart.push_back(message.m.ecart.ecart[i]);
	    }
	  DEBUG<<"Écart compris."<<std::endl;
	  //On sauve ces cartes.
	  cartes_attaque.push_back(ecart);
	  //Gestion des atouts :
	  std::vector<Carte::ModaliteEcart> resultat =
	    jeu_reel[attaquant()].peut_ecarter(ecart);
	  std::vector<Carte> montrees;
	  for(unsigned int i = 0 ; i < resultat.size() ; i++)
	    {
	      if(resultat[i] == Carte::MONTRER_CARTE)
		{
		  montrees.push_back(ecart[i]);
		}
	    }
	  DEBUG<<"Cartes à montrer déterminées."<<std::endl;
	  //On enlève les cartes de la main de l'attaquant.
	  for(unsigned int i = 0 ; i < 3 ; i++)
	    {
	      jeu_reel[attaquant()].enlever
		(Carte(message.m.ecart.ecart[i]));
	    }
	  DEBUG<<"Cartes enlevées."<<std::endl;
	  if(montrees.size() != 0)
	    {
	      Protocole::Message atout;
	      atout.type = Protocole::ATOUT;
	      atout.m.atout.nombre = montrees.size();
	      for(unsigned int i = 0 ; i < montrees.size() ; i++)
		{
		  atout.m.atout.cartes[i] = montrees[i].numero();
		}
	      DEBUG<<"Construction du Msg_atout terminée."<<std::endl;
	      EMETTRE_A_TOUS(atout);
	    }
	  //Envoi du message Jeu
	  Protocole::Message mess_jeu;
	  mess_jeu.type = Protocole::JEU;
	  mess_jeu.m.jeu.ouvreur = (chelem() >= 0 ? chelem() : 5);
	  EMETTRE_A_TOUS(mess_jeu);
	}
      break;
    case Protocole::REQUETE:
      if(true)
	{
	  DEBUG<<"Jeu de la carte "<<message.m.requete.carte
	       <<" accepté."<<std::endl;
	  Protocole::Message msg_carte;
	  msg_carte.type = Protocole::CARTE;
	  msg_carte.m.carte.carte = message.m.requete.carte;
	  EMETTRE_A_TOUS(msg_carte);
	}
      break;
    case Protocole::CARTE:
      if(true)
	{
	  DEBUG<<"Jeu de la carte "<<message.m.carte.carte
	       <<" par "<<tour_precedent()<<std::endl;
	  Carte c(message.m.carte.carte);
	  jeu_reel[tour_precedent()].enlever(c);
	}
    case Protocole::PLI:
      DEBUG<<"Prochain joueur : "<<tour()<<std::endl;
      break;
    case Protocole::RESULTAT:
      break;
    case Protocole::IDENTIFIER:
      break;
    case Protocole::ENTREE:
      break;
    case Protocole::SORTIE:
      break;
    case Protocole::INVITER:
      break;
    default :
      DEBUG<<"Penser à assimiler côté serveur.\n";
    }
  DEBUG<<"Tapis : "<<tapis()<<std::endl;
}

int PartieServeur::tester(unsigned int joueur, 
			  Protocole::Message const & message) const
{
  ENTER("tester(unsigned int joueur, Message message) const");
  ADD_ARG("joueur", joueur);
  ADD_ARG("message", message);
  int ok = 0; //1 : erreur de protocole, 2 : refusé
  switch(message.type)
    {
    case Protocole::PRISE:
      // Ok si on en est aux enchères, que c'est son tour et qu'il
      // monte (et que c'est une enchère de poids <= GARDE_CONTRE).
      if(phase() == ENCHERES && tour() == joueur)
	{
	  Enchere e(joueur, message.m.prise);
	  if((!(e.prise()) || e > e_max) && e.prise() 
	     <= Enchere::GARDE_CONTRE)
	    {
	      DEBUG<<"L'enchère "<<e.prise()
		   <<" est validée."<<std::endl;
	    }
	  else ok = 2;
	}
      else
	{
	  ok = 1;
	  ERROR<<"Erreur de protocole sur Protocole::PRISE. "
	       <<std::endl;
	  if(phase() != ENCHERES)
	    ERROR<<"On n'en est pas aux enchères."<<std::endl;
	  else ERROR<<"Ce n'est pas le tour du joueur "<<joueur
			<<" mais plutôt celui de "<<tour()<<std::endl;
	}
      break;
    case Protocole::APPELER:
      if(phase() == ENCHERES && joueur == attaquant())
	{
	  if(jeu_reel[joueur].peut_appeler
	     (Carte(message.m.appeler.carte)))
	    {
	      //C'est bon !
	    }
	  else ok = 2;
	}
      else ok = 1;
      break;
    case Protocole::ECART:
      DEBUG<<"Test d'un écart : "<<std::endl;
      if(phase() == CONSTITUTION_ECART &&
	 joueur == attaquant() &&
	 Enchere::GARDE_SANS > enchere_de(joueur).prise())
	{
	  DEBUG<<"c'est bien à son tour."<<std::endl;
	  DEBUG<<"Cartes demandées : ";
	  std::vector<Carte> ecart;
	  for(unsigned int i = 0 ; i < 3 ; i ++)
	    {
	      ecart.push_back(message.m.ecart.ecart[i]);
	      std::cout<<ecart[i].numero()<<", ";
	    }
	  std::cout<<std::endl;
	  std::vector<Carte::ModaliteEcart> resultat;
	  resultat = jeu_reel[joueur].peut_ecarter(ecart);
	  for(unsigned int i = 0 ; i < resultat.size() ; i++)
	    {
	      switch(resultat[i])
		{
		case Carte::NON_ECARTABLE :
		  DEBUG<<"La carte "<<i<<" ("
		       <<ecart[i]
		       <<") ne peut pas être écartée."
		       <<std::endl;
		  ok = 2 ;
		default:
		  break;
		}
	    }
	}
      else ok = 1;
      DEBUG<<"Fin du test, ok = "<<ok<<std::endl;
      break;
    case Protocole::CHELEM:
      ok = 1; //Pas encore implémenté.
      break;
    case Protocole::MONTRER_POIGNEE:
      ok = 1;
      break;
    case Protocole::REQUETE:
      //On teste :
      //1. Si on en est à la phase de jeu.
      //2. Si c'est bien au tour de ce joueur.
      // (si 1. ou 2. n'est pas vérifié, c'est une erreur de protocole)
      //3. Si la main du joueur accepte cette carte.
      //4. Si le tapis accepte cette carte.
      if(phase() == Partie::PHASE_JEU && tour() == joueur)
	{
	  //1, 2 : ok
	  DEBUG<<"Déjà, c'est le tour du joueur."<<std::endl;
	  ok = 0;
	  Carte c(message.m.requete.carte);
	  Carte appelee(*(donner_contrat_final().carte_appelee()));
	  Carte demandee(78);
	  bool entame = tapis().entame(demandee);
	  Carte plus_gros_atout = Carte(56);
	  bool contient_atout = tapis().plus_gros_atout
	    (plus_gros_atout);
	  bool carte_possedee = jeu_reel[joueur].possede(c);
	  if(carte_possedee)
	    {
	      DEBUG<<"Je possède la carte "<<c<<std::endl;
	      if(c == EXCUSE)
		{
		  DEBUG<<"J'ai toujours le droit de jouer l'excuse."
		       <<std::endl;
		  ok = 0;
		}
	      else if(entame && c.couleur() == demandee.couleur())
		{
		  DEBUG<<"Je fournis."<<std::endl;
		  if(c.couleur() == Carte::ATOUT)
		    {
		      DEBUG<<"Comme c'est de l'atout, je dois monter."
			   <<std::endl;
		      if((contient_atout && c > plus_gros_atout))
			{
			  DEBUG<<"Je monte sur un atout existant."
			       <<std::endl;
			  ok = 0;
			}
		      else if(contient_atout)
			{
			  DEBUG<<"Je ne monte pas sur "
			       <<plus_gros_atout<<"."<<std::endl;
			  if(jeu_reel[joueur].peut_pisser
			     (plus_gros_atout.valeur()))
			    {
			      DEBUG<<"Je peux pisser."
				   <<std::endl;
			      ok = 0;
			    }
			  else
			    {
			      DEBUG<<"Je ne peux pas pisser."
				   <<std::endl;
			      ok = 2;
			    }
			}
		      else
			{
			  ERROR<<"Il n'y a pas encore d'atout "
			       <<"alors qu'il y a une entame à "
			       <<"la couleur atout..."<<std::endl;
			  ok = 2;
			}
		    }
		  else
		    {
		      DEBUG<<"Je fournis à une couleur non atout."
			   <<std::endl;
		      ok = 0;
		    }
		}
	      else if(entame && c.couleur() == Carte::ATOUT)
		{
		  DEBUG<<"Je ne suis pas le premier à jouer, "
		       <<"et je coupe."<<std::endl;
		  if(jeu_reel[joueur].peut_couper(demandee.couleur()))
		    {
		      DEBUG<<"J'ai le droit de couper."
			   <<std::endl;
		      if(contient_atout && c > plus_gros_atout)
			{
			  DEBUG<<"D'autres ont coupé, mais je surcoupe."
			       <<std::endl;
			  ok = 0;
			}
		      else if(contient_atout)
			{
			  DEBUG<<"Je ne monte pas sur "
			       <<plus_gros_atout<<"."<<std::endl;
			  if(jeu_reel[joueur].peut_pisser
			     (plus_gros_atout.valeur()))
			    {
			      DEBUG<<"Je peux pisser."
				   <<std::endl;
			      ok = 0;
			    }
			  else
			    {
			      DEBUG<<"Je ne peux pas pisser."
				   <<std::endl;
			      ok = 2;
			    }
			}
		      else
			{
			  DEBUG<<"Aucun problème, je suis le premier à couper."
			       <<std::endl;
			  ok = 0;
			}
		    }
		  else
		    {
		      DEBUG<<"Je ne peux pas couper."
			   <<std::endl;
		      ok = 2;
		    }
		}
	      else if(entame)
		{
		  DEBUG<<"Je ne fournis ni ne coupe."
		       <<std::endl;
		  if(jeu_reel[joueur].peut_couper(demandee.couleur()) 
		     && jeu_reel[joueur].peut_defausser())
		    {
		      DEBUG<<"J'ai le droit de défausser une carte."
			   <<std::endl;
		      ok = 0;
		    }
		  else
		    {
		      DEBUG<<"Je n'ai pas le droit de défausser."
			   <<std::endl;
		      ok = 2;
		    }
		}
	      else
		{
		  DEBUG<<"Vous êtes le premier à jouer."
		       <<std::endl;
		  if(jeu_reel[(joueur + 1) % 5].nombre_cartes() == 15)
		    {
		      DEBUG<<"On en est au premier tour."<<std::endl;
		      if(c == appelee || 
			 c.couleur() != appelee.couleur())
			{
			  DEBUG<<"Ce n'est pas la couleur appelée ("
			       <<"ou alors c'est la carte appelée)."
			       <<std::endl;
			  ok = 0;
			}
		      else
			{
			  DEBUG<<"Cette carte ne peut pas être "
			       <<"jouée au premier tour."
			       <<std::endl;
			  ok = 2;
			}
		    }
		  else
		    {
		      DEBUG<<"On n'en est pas au premier tour."
			   <<std::endl;
		      ok = 0;
		    }
		}
	    }
	  else
	    {
	      DEBUG<<"Vous n'avez pas cette carte."<<std::endl;
	      ok = 2;
	    }
	}
      else ok = 1;
      break;
    case Protocole::IDENTIFIER:
      ok = 1;
      break;
    case Protocole::ENTREE:
      ok = 1;
      break;
    case Protocole::SORTIE:
      ok = 1;
      break;
    case Protocole::INVITER:
      ok = 1;
      break;
    default :
      ok = 1;
      break;
    }
  EXIT(ok);
  return ok;
}

template <class T>
void swap(unsigned int i, unsigned int j, std::vector<T> & tab)
{
  T temp(tab[i]);
  tab[i] = tab[j];
  tab[j] = temp;
}

template<class T>
void shuffle(std::vector<T> & tab)
{
  for(unsigned int i = 1 ; i < tab.size() ; i++)
    {
      swap(i, rand() % (i + 1), tab);
    }
}

void PartieServeur::distribuer()
{
  ENTER("distribuer()");
  std::vector<int> nums (78);
  for(unsigned int i = 0 ; i < nums.size() ; i++)
    {
      nums[i] = i;
    }
#ifdef MELANGER
  shuffle(nums);
#endif
  // Les 3 dernières cartes sont pour le chien, les autres pour les
  // joueurs.
  chien.clear();
  for(unsigned int i = 1 ; i <= 3 ; i++)
    {
      chien.push_back(Carte(nums[nums.size() - i]));
    }
  jeu_reel.clear();
  //On prévoit le cas "petit sec"
  int nbr_atouts = 0;
  bool a_petit = false;
  bool petit_sec = false;
  Carte c(PETIT);
  for(unsigned int i = 0 ; i < 5 ; i++)
    {
      Main m;
      nbr_atouts = 0;
      a_petit = false;
      for(unsigned int j = 0 ; j < 15 ; j++)
	{
	  c = nums[i*15 + j];
	  if(c == PETIT) a_petit = true;
	  if(c.atout()) nbr_atouts++;
	  m.ajouter(c);
	}
      jeu_reel.push_back(m);
      if(a_petit && nbr_atouts == 1)
	{
	  petit_sec = true;
	}
    }
  if(!petit_sec)
    {
      for(unsigned int i = 0 ; i < 5 ; i++)
	{
	  Protocole::Message mess;
	  mess.type = Protocole::DISTRIBUTION;
	  jeu_reel[i].distribution(mess.m.distribution);
	  emit doit_emettre(i, mess, true);
	}
      set_phase(Partie::ENCHERES);
    }
  else distribuer();
}

void PartieServeur::cartes_gagnees
(std::vector<Carte> const & cartes,
 std::vector<unsigned int> const & poseurs,
 std::vector<unsigned int> const & gagnants)
{
  ENTER("cartes_gagnees(vector<Carte> cartes, vector<uint> poseurs, vector<uint> gagnants)");
  ADD_ARG("cartes", cartes);
  ADD_ARG("poseurs", poseurs);
  ADD_ARG("gagnants", gagnants);
  std::vector<Carte> gagnees;
  std::vector<Carte> perdues;
  for(unsigned int i = 0 ; i < cartes.size() ; i++)
    {
      if(gagnants[i] == attaquant() || gagnants[i] == joueur_appele)
	{
	  gagnees.push_back(cartes[i]);
	}
      else
	{
	  perdues.push_back(cartes[i]);
	}
    }
  cartes_attaque.push_back(gagnees);
  cartes_defense.push_back(perdues);
  Protocole::Message pli;
  pli.type = Protocole::PLI;
  unsigned int maitre = tour();
  DEBUG<<"Constitution d'un message pli : "
       <<maitre<<" gagne."<<std::endl;
  pli.m.pli.joueur = maitre;
  EMETTRE_A_TOUS(pli);
  plis_restants--;
  if(plis_restants == 0)
    {
      DEBUG<<"La partie est finie."<<std::endl;
      Protocole::Message res;
      res.type = Protocole::RESULTAT;
      std::vector<unsigned int> poignees;
      for(unsigned int i = 0 ; i < 5 ; i++)
	{
	  poignees.push_back(poignee(i));
	}
      std::vector<int> r = compter(donner_contrat_final(),
				   joueur_appele,
				   cartes_attaque,
				   cartes_defense,
				   poignees);
      for(unsigned int i = 0 ; i < r.size() ; i++)
	{
	  res.m.resultat.resultats[i] = r[i];
	}
      EMETTRE_A_TOUS(res);
      emit termine();
    }
}

void PartieServeur::reinitialiser()
{
  Partie::reinitialiser();
  jeu_reel.clear(); //Inutile en fait
  cartes_attaque.clear();
  cartes_defense.clear();
  chien.clear();//Idem
  e_max = Enchere();
  joueur_appele = 5;
  plis_restants = 15;
}
