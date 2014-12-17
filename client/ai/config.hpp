/**
   @file client/ai/config.hpp
   Procure toutes les options de compilation de l'IA.
 */
#ifndef AUTO_PORT
/**
   @brief Port de listage par défaut.
 */
#define AUTO_PORT 45678
#endif
#ifndef AUTO_ADDR
/**
   @brief Adresse du serveur par défaut.
   92.144.7.179
 */
#define AUTO_ADDR QHostAddress("192.168.1.17")
#endif