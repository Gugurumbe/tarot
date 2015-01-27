/*
  Project tarot virtuel is a card game.
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


#include "option.hpp"

template<typename T>
Option<T>::none::none(): std::logic_error("Option vide.")
{
}

template<typename T>
Option<T>::Option(): content(0)
{
}

template<typename T>
Option<T>::Option(const Option<T> & option): content(0)
{
  if(option.content) content = new T(*option.content);
}

template<typename T>
Option<T>::Option(const T & obj): content(new T(obj))
{
}

template<typename T>
Option<T>::~Option()
{
  if(content) delete content;
}

template<typename T>
void Option<T>::set(const T & obj)
{
  if(content) delete content;
  content = new T(obj);
}

template<typename T>
void Option<T>::set()
{
  if(content) delete content;
  content = 0;
}

template<typename T>
bool Option<T>::aucun() const
{
  return content == 0;
}

template<typename T>
T Option<T>::get() const
{
  if(content)
    {
      return *content;
    }
  else 
    {
      throw Option<T>::none();
    }
}


template<typename T>
std::ostream & operator<<(std::ostream & out, const Option<T> & t)
{
  if(t.aucun())
    {
      out<<"<none>";
    }
  else
    {
      out<<"<some "<<t.get()<<">";
    }
  return out;
}

#include "enchere.hpp"

template class Option<Enchere>;
