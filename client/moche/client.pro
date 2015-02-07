#########################################################################
# Project tarot virtuel is a card game.                                 # 
# Copyright (C) 2015  Vivien Kraus                                      #
#                                                                       #
# This program is free software: you can redistribute it and/or modify  #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation, either version 3 of the License, or     #
# (at your option) any later version.                                   #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program.  If not, see <http://www.gnu.org/licenses/>. #
#########################################################################

DEFINES += "NE_RIEN_DEBOGUER"

TEMPLATE = app
TARGET = client
CONFIG += console
DEPENDPATH += .
INCLUDEPATH += . ../shared ../../shared

QT -= gui widgets
QT += core

# Input
HEADERS +=                                          \
interface_moche.hpp                                 \
../shared/client.hpp                                \
../shared/client_jeu.hpp                            \
../shared/option.hpp                                \
../shared/partie_client.hpp                         \
../shared/transaction.hpp                           \
../../shared/carte.hpp                              \
../../shared/deboguer.hpp                           \
../../shared/debogueur.hpp                          \
../../shared/enchere.hpp                            \
../../shared/main.hpp                               \
../../shared/ne_pas_deboguer.hpp                    \
../../shared/partie.hpp                             \
../../shared/protocole.hpp                          \
../../shared/tapis.hpp       

# Sources
SOURCES +=                                          \
main_program.cpp                                    \
interface_moche.cpp                                 \
../shared/client.cpp                                \
../shared/client_jeu.cpp                            \
../shared/option.cpp                                \
../shared/partie_client.cpp                         \
../shared/transaction.cpp                           \
../../shared/carte.cpp                              \
../../shared/debogueur.cpp                          \
../../shared/enchere.cpp                            \
../../shared/main.cpp                               \
../../shared/partie.cpp                             \
../../shared/protocole.cpp                          \
../../shared/tapis.cpp       

QT += network
