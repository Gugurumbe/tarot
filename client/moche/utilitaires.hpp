/**
   @file client/moche/utilitaires.hpp
   @brief Définit des outils pour la console.
 */

#ifndef UTILITAIRES_DEFINIS

/**
   @brief Empêche les inclusions multiples.
 */
#define UTILITAIRES_DEFINIS

#include "carte.hpp"
#include <string>
#include <vector>

/**
   @brief Trouve le "meilleur" mot qui correspond au mot à tester.

   @param motifs Les mots contre lesquels on veut tester le test.
   @param test Le test.
   @param decalages_max Le nombre de décalages que l'on s'autorise.
   @return L'indice du meilleur partenaire dans le tableau des
   motifs. 

   On commence par simplifier les motifs et le test, en remplaçant
   toutes les minuscules par les majuscules associées, en remplaçant
   les nombres entre 1 et 21 par les nombres en français, et en
   supprimant les caractères non ASCII. NB : le 1 se traduit par
   "AS". Ensuite, on calcule une forme d'intercorrélation entre chaque
   motif et le test, et on garde la somme des decalages_max pics de
   l'intercorrélation. L'intercorrélation se calcule comme le nombre
   de caractères en commun après avoir décalé le test par rapport au
   motif. 

   Ça marche à peu près...
 */
unsigned int meilleur_partenaire
(const std::vector<std::string> & motifs,
 const std::string & test,
 unsigned int decalages_max);

/**
   @brief Essaye de deviner la carte qu'on a entrée.

   @param entree Le texte entré.
   
   @return La Carte devinée.
 */
Carte retrouver(const std::string & entree);

#endif
