TEMPLATE = app
TARGET = client
CONFIG += console
DEPENDPATH += .
INCLUDEPATH += . ../shared ../../shared

QT -= gui widgets
QT += core

# Input
HEADERS +=                                          \#
ia.hpp                                              \#
../shared/client.hpp                                \#
../shared/client_jeu.hpp                            \#
../shared/option.hpp                                \#
../shared/partie_client.hpp                         \#
../shared/transaction.hpp                           \#
../../shared/carte.hpp                              \#
../../shared/deboguer.hpp                           \#
../../shared/debogueur.hpp                          \#
../../shared/enchere.hpp                            \#
../../shared/main.hpp                               \#
../../shared/ne_pas_deboguer.hpp                    \#
../../shared/partie.hpp                             \#
../../shared/protocole.hpp                          \#
../../shared/tapis.hpp                               #

# Sources
SOURCES +=                                          \#
main_program.cpp                                    \#
ia.cpp                                              \#
../shared/client.cpp                                \#
../shared/client_jeu.cpp                            \#
../shared/option.cpp                                \#
../shared/partie_client.cpp                         \#
../shared/transaction.cpp                           \#
../../shared/carte.cpp                              \#
../../shared/debogueur.cpp                          \#
../../shared/enchere.cpp                            \#
../../shared/main.cpp                               \#
../../shared/partie.cpp                             \#
../../shared/protocole.cpp                          \#
../../shared/tapis.cpp                           

QT += network
