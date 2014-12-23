/**
   @file client/ui/config.hpp
   @brief Définit les options de compilation du client.
*/

/**
   @brief Oui ou non.
 */
enum {YES, NO};

/**
   @brief Se connecte automatiquement sur l'adresse.
   
   109.213.155.197
   192.168.1.17
 */
#define AUTO_ADDR "156.18.26.70"

/**
   @brief Se connecte automatiquement sur le port
 */
#define AUTO_PORT 80

/**
   @brief Définit la localisation de la feuille de style pour le journal.
 */
#define STYLE "style_journal.css"

/**
   @brief Définit le type d'interface.

   Prend les valeurs Coquille (5 clients à la fois)
   ou ClientGraphique (1 client)
 */
#define INTERFACE Coquille
