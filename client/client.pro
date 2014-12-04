TEMPLATE = app
TARGET = client
DEPENDPATH += .
INCLUDEPATH += . ../shared

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Input
HEADERS += client.hpp            \
../shared/protocole.hpp          \
client_jeu.hpp                   \
config.hpp                       \
../shared/partie.hpp             \
partie_client.hpp                \
../shared/tapis.hpp              \
../shared/enchere.hpp            \
../shared/carte.hpp              \
../shared/main.hpp               \
../shared/debogueur.hpp          \
../shared/deboguer.hpp           \
../shared/ne_pas_deboguer.hpp    \
client_graphique.hpp             \
label_nom.hpp                    \
journal.hpp                      \
coquille.hpp

SOURCES += main_program.cpp      \
client.cpp                       \
../shared/protocole.cpp          \
client_jeu.cpp                   \
../shared/partie.cpp             \
partie_client.cpp                \
../shared/tapis.cpp              \
../shared/enchere.cpp            \
../shared/carte.cpp              \
../shared/main.cpp               \
../shared/debogueur.cpp          \
client_graphique.cpp             \
label_nom.cpp                    \
journal.cpp                      \
coquille.cpp

FORMS += client.ui 
FORMS += coquille.ui

QT += network
