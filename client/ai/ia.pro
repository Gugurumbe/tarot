TEMPLATE = app
TARGET = ia
DEPENDPATH += .
INCLUDEPATH += . ../shared ../../shared

# Input
HEADERS += config.hpp
HEADERS += ia.hpp
HEADERS += ../shared/client.hpp
HEADERS += ../shared/client_jeu.hpp
HEADERS += ../shared/option.hpp
HEADERS += ../shared/partie_client.hpp
HEADERS += ../shared/transaction.hpp
HEADERS += ../../shared/carte.hpp
HEADERS += ../../shared/deboguer.hpp
HEADERS += ../../shared/debogueur.hpp
HEADERS += ../../shared/enchere.hpp
HEADERS += ../../shared/main.hpp
HEADERS += ../../shared/ne_pas_deboguer.hpp
HEADERS += ../../shared/partie.hpp
HEADERS += ../../shared/protocole.hpp
HEADERS += ../../shared/tapis.hpp

# Sources
SOURCES += ia.cpp
SOURCES += main_program.cpp
SOURCES += ../shared/client.cpp
SOURCES += ../shared/client_jeu.cpp
SOURCES += ../shared/option.cpp
SOURCES += ../shared/partie_client.cpp
SOURCES += ../shared/transaction.cpp
SOURCES += ../../shared/carte.cpp
SOURCES += ../../shared/debogueur.cpp
SOURCES += ../../shared/enchere.cpp
SOURCES += ../../shared/main.cpp
SOURCES += ../../shared/partie.cpp
SOURCES += ../../shared/protocole.cpp
SOURCES += ../../shared/tapis.cpp      

QT += network 

QT -= gui
