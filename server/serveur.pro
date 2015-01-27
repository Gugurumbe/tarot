########################################################################
# serveur of project tarot virtuel is a server for a card game.        #
# Copyright (C) 2015  Vivien Kraus                                     #
#                                                                      #
# This program is free software: you can redistribute it and/or modify #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# This program is distributed in the hope that it will be useful,      #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with this program.  If not, see <http://www.gnu.org/licenses/> #
########################################################################


TEMPLATE = app
TARGET = serveur
DEPENDPATH += .
INCLUDEPATH += . ../shared

QT -= gui

# Input
HEADERS += serveur.hpp             \
serveur_jeu.hpp                    \
../shared/protocole.hpp            \
table.hpp                          \
../shared/partie.hpp               \
../shared/carte.hpp                \
../shared/main.hpp                 \
../shared/tapis.hpp                \
partie_serveur.hpp                 \
../shared/enchere.hpp              \
../shared/debogueur.hpp            \
score.hpp                          \
config.hpp

SOURCES += main_program.cpp        \
serveur.cpp                        \
../shared/protocole.cpp            \
serveur_jeu.cpp                    \
table.cpp                          \
../shared/partie.cpp               \
../shared/carte.cpp                \
../shared/main.cpp                 \
../shared/tapis.cpp                \
partie_serveur.cpp                 \
../shared/debogueur.cpp            \
../shared/enchere.cpp              \
score.cpp

QT += network
