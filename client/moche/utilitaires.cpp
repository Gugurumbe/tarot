#include "utilitaires.hpp"

#include <string>
#include <vector>
#include <algorithm>

#define MAX(a, b) (a < b ? b : a)

int matcher(const std::string & source, const std::string & test,
	    unsigned int nombre_max)
{
  //std::cout<<"Corrélation entre "<<source<<" et "<<test<<" : ";
  std::vector<int> intercorrelations;
  int s = 0;
  for(int decalage = -(static_cast<int>(test.size())) ; 
      decalage < static_cast<int>(source.size ()) ;
      decalage++)
    {
      s = 0;
      for(unsigned int i = MAX(0, -decalage) ; 
	  i + decalage < source.size() && i < test.size() ; i++)
	{
	  if(source[i + decalage] == test[i])
	    {
	      s++;
	    }
	}
      intercorrelations.push_back(s);
    }
  //On s'intéresse aux pics d'intercorrélation. Pour ça, il faut trier
  //les intercorrélations.
  std::sort(intercorrelations.begin(), intercorrelations.end());
  //On prend la somme des nombre_max premiers termes
  s = 0;
  for(unsigned int i = 0 ; 
      i < intercorrelations.size() && i < nombre_max ; i++)
    {
      s += intercorrelations[intercorrelations.size() - 1 - i];
    }
  //std::cout<<s<<std::endl;
  return s;
}

#include <cctype>

std::string enoncer_nombre_tarot(unsigned int n)
{
  std::string r;
  std::string noms[22] = {"ZERO","AS","DEUX","TROIS","QUATRE","CINQ",
			  "SIX","SEPT","HUIT","NEUF","DIX","ONZE",
			  "DOUZE","TREIZE","QUATORZE","QUINZE","SEIZE",
			  "DIX SEPT","DIX HUIT","DIX NEUF","VINGT",
			  "VINGT ET UN"};
  if(n <= 21)
    {
      r = noms[n];
    }
  return r;
}

std::string lire_nombre(const std::string & chaine, unsigned int & i)
{
  int n = 0;
  while(i < chaine.length() && chaine[i] >= '0' && chaine[i] <= '9')
    {
      n *= 10;
      n += chaine[i] - '0';
      i++;
    }
  return enoncer_nombre_tarot(n);
}

std::string simplifier(const std::string & complique)
{
  std::string resultat;
  int caractere = 0;
  unsigned int i = 0;
  while(i < complique.size())
    {
      caractere = toupper(complique[i]);
      if(caractere >= 'A' && caractere <= 'Z')
	{
	  resultat.push_back(caractere);
	  i++;
	}
      else if(caractere >= '0' && caractere <= '9')
	{
	  resultat += lire_nombre(complique, i);
	}
      else 
	{
	  resultat.push_back(' ');
	  i++;
	}
    }
  return resultat;
}

unsigned int meilleur_partenaire(const std::vector<std::string> & motifs,
				 const std::string & test,
				 unsigned int decalages_max)
{
  unsigned int i_max = 0;
  std::string test_simple = simplifier(test);
  int corr_max = matcher(simplifier(motifs[0]), test, decalages_max);
  int corr = 0;
  for(unsigned int i = 1 ; i < motifs.size() ; i++)
    {
      corr = matcher(simplifier(motifs[i]), test_simple, 
		     decalages_max);
      if(corr > corr_max)
	{
	  i_max = i;
	  corr_max = corr;
	}
    }
  return i_max;
}

Carte retrouver(const std::string & entree)
{
  std::vector<std::string> cartes;
  for(unsigned int i = 0 ; i < 78 ; i++)
    {
      cartes.push_back(Carte(i).nom());
    }
  Carte(meilleur_partenaire(cartes, entree, 2));
}
