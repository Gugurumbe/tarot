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
