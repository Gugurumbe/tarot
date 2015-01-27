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

#include "./config.hpp"
#include "debogueur.hpp"
#include <iostream>
#include <QCoreApplication>

#include "serveur_jeu.hpp"

#include <cstdlib>
#include <ctime>

#ifndef TESTER_POINTS

int main(int argc, char * argv[])
{
  std::cout<<"Tarot virtuel - serveur Copyright (C) 2015 Vivien Kraus"
	   <<std::endl
	   <<"This program comes with ABSOLUTELY NO WARRANTY."
	   <<std::endl
	   <<"This is free software, and you are welcome to redistribute it"
	   <<std::endl
	   <<"under certain conditions."<<std::endl;

  ENTER_FUNCTION("main(int argc, char * argv[])");
  Debogueur::arg("argc", argc);
  for(int i = 0 ; i < argc ; i++)
    {
      std::stringstream str;
      str<<"argv["<<i<<"]";
      Debogueur::arg(str.str(), 
		     "\"" + std::string(argv[i]) + "\"");
    }
  QCoreApplication app(argc, argv);
  srand(time(NULL));
  SERVEUR s;
  Debogueur::debug()<<"Ouverture du port "<<s.ouvrir_global()<<std::endl;
  long int ret = app.exec();
  Debogueur::ret(ret);
  return ret;
}

#else

#include "score.hpp"

int main(int argc, char * argv[])
{
  return 0;
}

#endif
