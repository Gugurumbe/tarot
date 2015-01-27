/**
   @file shared/carte.hpp
   @brief définit une classe de carte à jouer.
 */

#ifndef CARTE_DEFINIE

/**
   empêche les inclusions récursives.
 */
#define CARTE_DEFINIE

#include <string>
#include <iostream>

/**
   Premier atout.
 */
#define DEBUT_ATOUT 56

/**
   Première carte de pique.
 */
#define DEBUT_PIQUE Carte::PIQUE * 14

/**
   Première carte de cœur.
 */
#define DEBUT_COEUR Carte::COEUR * 14

/**
   Première carte de carreau.
 */
#define DEBUT_CARREAU Carte::CARREAU * 14

/**
   Première carte de trèfle.
 */
#define DEBUT_TREFLE Carte::TREFLE * 14

/**
   Numéro du Petit.
 */
#define PETIT 56

/**
   Numéro du Vingt et Un.
*/
#define VINGTETUN 76

/**
   Numéro de l'Excuse.
*/
#define EXCUSE 77

/**
   Numéro de la Dette d'Excuse.
   La Dette d'Excuse est une carte artificielle qui est jouée en même
   temps que l'Excuse. Aucun joueur ne possède la Dette d'Excuse, mais
   elle apparaît dans les plis ramassés. Elle rapporte 0.5 points,
   n'est pas un bout mais est considérée comme un atout de valeur
   0. Elle est tout le temps gagnée par le joueur qui remporte le pli.

   L'Excuse est un bout, rapporte 4 points et est considérée comme un
   atout de valeur 0. Sauf circonstances exceptionnelles, elle est
   gagnée par celui qui l'a jouée.
 */
#define DETTE_EXCUSE 78

/**
   @brief Définition d'une Carte du jeu.

   Une classe permettant de définir les comportements d'une carte au
   tarot. Une Carte est définie par un numéro unique entre 0 et 78. 
   Les cartes du tarot sont au nombre de 78, mais pour des raisons de
   commodité on considère qu'il y en a 79. L'excuse est un bout qui
   vaut 4 points, et la dette d'excuse est une carte basse qui revient
   à celui qui fait le pli.
 */
class Carte
{
public:

  /**
     @brief Énumère les 5 couleurs.

     L'excuse est un atout. C'est la dernière carte d'atout. La fausse
     excuse est également un atout, elle arrive après la vraie.
   */
  enum Couleur{PIQUE, CARREAU, TREFLE, COEUR, ATOUT};
  
  /**
     @brief Représente la valeur des cartes de couleur non atout.

     @see Carte::valeur() const
  */
  enum Valeur{AS, DEUX, TROIS, QUATRE, CINQ, SIX,
       SEPT, HUIT, NEUF, DIX, VALET, CAVALIER, DAME, ROI};
  
  /**
     @brief Sert à dire si une carte peut être écartée ou non, et les
     modalités éventuelles (montrer les atouts).

     @see Carte::ecartable() const
  */  
  enum ModaliteEcart
    {
      ECARTABLE,     /**<@brief La carte est écartable.*/
      MONTRER_CARTE, /**<@brief Il faut montrer la carte.*/
      NON_ECARTABLE  /**<@brief On ne peut pas écarter cette carte.*/
    };

  /**
     @brief Définit la "force" de l'excuse.
   */
  enum ForceExcuse
    {
      /**
	 @brief En cas de Chelem.
	 
	 En cas de Chelem, si le preneur pose l'excuse comme dernière
	 carte, elle remporte le pli.
       */
      EXCUSE_GAGNANTE,   

      /**
	 @brief Dans la plupart des cas.
	 
	 L'excuse dont il est question est le bout qui vaut 4.0
	 points. 
       */
      EXCUSE_IMPRENABLE,

      /**
	 @brief Au dernier tour.
	 
	 Au dernier tour, sauf cas de chelem réussi jusqu'à présent,
	 l'excuse est remportée.
       */
      EXCUSE_PRENABLE
    };
  
  /** 
      @brief Constructeur principal.

      Le constructeur attend un numéro de carte valide.
      @param numero : le numéro entre 0 et 78.
  */
  Carte(unsigned int numero);
  /**
     @brief Constructeur par défaut. À PROSCRIRE.

     @note Sa présence sert pour émettre des signaux contenant des
     Carte entre plusieurs QThread. Lorsqu'on n'utilisera plus la
     console moche, ce constructeur sera supprimé.
   */
  Carte();
  /**
     @brief Constructeur de copie.

     @param carte : la carte à copier.
  */
  Carte(const Carte & carte);
  /**
     @brief Comparaison des cartes.

     Les cartes sont toutes comparables. La relation carte1 > carte2
     veut dire : "Si je joue carte1 puis qu'on joue carte2, c'est a
     priori moi qui remporte le pli". Il ne s'agit pas d'une relation
     d'ordre, car elle n'est pas antisymétrique. Par exemple, un deux
     de pique l'emporte sur un trois de coeur et vice versa.
     @param carte : le numéro de la carte à comparer.
     @return vrai dans le cas de l'exemple précédent.
   */
  bool operator>(unsigned int carte) const;
  /**
     @brief Comparaison des cartes (2).

     @param carte : l'objet carte à comparer à "this".
     @see Carte::operator>(unsigned int) const
  */
  bool operator>(const Carte & carte) const;
  /**
     @brief Test d'égalité entre les cartes.

     Puisque les cartes sont copiables, on veut tester l'égalité.
     @param carte : le numéro de la carte à comparer à "this".
     @return vrai ssi la carte a ce numéro.
   */
  bool operator==(unsigned int carte) const;
  /**
     @brief Test d'égalité entre les cartes (2).

     @param carte : l'objet Carte à comparer à "this".
     @see Carte::operator==(unsigned int) const
  */
  bool operator==(const Carte & carte) const;
  /**
     @brief Test de différence entre 2 cartes.

     @param carte : le numéro de la carte à comparer à "this".
     @return vrai ssi les cartes sont différentes.
     @see Carte::operator==(unsigned int) const
  */
  bool operator!=(unsigned int carte) const;
  /**
     @brief Test de différence entre 2 cartes (2).

     @param carte : l'objet carte à comparer à "this".
     @see Carte::operator==(const Carte &) const
     @see Carte::operator!=(unsigned int) const
  */
  bool operator!=(const Carte & carte) const;
  /**
     @brief Retourne le numéro de la carte.

     Le numéro de la carte est un entier entre 0 et 78.
     @return le numéro de la carte.
     @see Carte::Carte(unsigned int)
   */
  unsigned int numero() const;
  /**
     @brief Retourne la couleur de la carte.

     La couleur de la carte est un nombre entre 0 et 4. La dernière
     couleur est la couleur atout. L'excuse est considérée comme un
     atout. Avant de faire des tests, il peut être utile de vérifier
     si la carte est l'excuse : 
     
     ~~~~~~~~~~~~~~~~~~~{.cpp}
     if(carte == EXCUSE || carte == DETTE_EXCUSE)
     {
     traiterCommeExcuse(carte);
     }
     else if(carte1.couleur() == carte2.couleur())
     {
     traiter(carte);
     }
     ~~~~~~~~~~~~~~~~~~~
     
     @return la couleur de la carte.
     @see Carte::operator==(unsigned int) const
   */
  Couleur couleur() const;
  /**
     @brief Indique si la carte est de l'atout.

     Peut être utile pour écarter des cartes ou pour déclarer des
     poignées. L'excuse est considérée comme un atout.
     @return vrai ssi la carte est l'excuse ou un atout.
     @see Carte::couleur() const
     @see Carte::bout() const
     @see Carte::roi() const
     @see Carte::tete() const
   */
  bool atout() const;
  /**
     @brief Indique si la carte est une tête.

     Les têtes sont les valets, cavaliers, dames et rois.
     Peut être utile pour appeler une carte.
     @return vrai ssi la carte est une tête.
     @see Carte::atout() const
     @see Carte::roi() const
     @see Carte::couleur() const
     @see Carte::bout() const
   */
  bool tete() const;
  /**
     @brief Indique si la carte est un roi.

     @return vrai ssi la carte est un roi.
     @see Carte::atout() const
     @see Carte::tete() const
     @see Carte::couleur() const
     @see Carte::bout() const     
   */
  bool roi() const;
  /**
     @brief Indique si la carte est un bout.

     @return vrai ssi la carte est un bout.
     @see Carte::atout() const
     @see Carte::tete() const
     @see Carte::couleur() const
     @see Carte::roi() const     
   */
  bool bout() const;
  /**
     @brief Retourne la valeur d'une carte.

     La valeur d'une carte est son ordre dans le classement par ordre
     croissant des cartes d'une même couleur. Par exemple, la valeur
     d'une Dame est 12. La valeur des Excuses est 0, celle du Petit
     est 1, celle du Vingt et un est 21. Utilisé pour déterminer la
     carte la plus forte d'un pli. N'a rien à voir avec le nombre de
     points que rapporte cette carte.
     @return la valeur de la carte.
     @see Carte::couleur() const
     @see Carte::demipoints() const
   */
  Valeur valeur() const;
  /**
     @brief Retourne le nombre de demi-points que cette carte
     rapporte.

     Le nombre de demi-points d'une carte basse et de la dette
     d'excuse est 1. Le nombre de demi-points de l'excuse est 8 (4
     points). Le nombre de demi-points d'un autre bout est 9 (4.5
     points). Le nombre de demi-points d'une tête est déterminé par sa
     valeur : Valet 3, Cavalier 5, Dame 7, Roi 9.
     @return le nombre de demi-points de cette carte.
     @see Carte::bout() const
     @see Carte::tete() const
     @see Carte::valeur() const
   */
  unsigned int demipoints() const;
  /**
     @brief Indique si la carte est écartable.

     Lors de la constitution d'un écart, le joueur peut ou ne peut pas
     mettre certaines cartes dans le chien. Les bouts et les rois sont
     interdits, et les atouts sont tolérés dans une certaine mesure :
     si le joueur ne peut pas écarter autre chose, et en montrant ce
     ou ces atouts aux autres joueurs.
     @return de quelle manière on peut écarter cette carte.
     @see Carte::bout() const
     @see Carte::roi() const
     @see Carte::atout() const
   */
  ModaliteEcart ecartable() const;
  /**
     @brief Affecte la carte (1).

     Modifie le numéro de la carte pour qu'il corresponde à celui
     d'une autre carte.
     @param carte : la carte à copier.
     @return la copie créée.
     @see Carte::Carte(const Carte &)
     @see Carte::numero() const
   */
  Carte const & operator=(const Carte & carte);
  /**
     @brief Affecte la carte (2).

     Change le numéro de la carte. S'il n'est pas inférieur à 79, le
     résultat modulo 79 est utilisé.
     @param carte : le numéro à utiliser. 
     @return la copie créée.
     @see Carte::operator=(const Carte &)
     @see Carte::Carte(unsigned int)
     @see Carte::numero() const
  */
  Carte const & operator=(unsigned int carte);

  /**
     @brief Donne le nom de la Carte.
     
     Donne son nom sous la forme :
     « Valeur de Couleur »
     
     @return Le nom.
  */
  std::string nom() const;

  /**
     @brief Donne une version simplifiée (sans accent) de la couleur.
     
     Utilisable dans une feuille de style css...

     @return Une version simple de la couleur : "pique", "carreau",
     "trefle", "coeur"
   */
  std::string couleur_simple() const;
private:
  /**
     @brief Le numéro interne de la carte.
   */
  unsigned int m_numero;
};

/**
   @brief Affiche le nom d'une Carte.
   
   @see Carte::nom
   
   @param[out] out Le flux à utiliser.
   @param carte La carte à écrire.
   @return La suite du flux.
 */
std::ostream & operator<<(std::ostream & out, const Carte & carte);

/**
   @brief Affiche le nom d'une Couleur.
   
   @param[out] out Le flux à utiliser.
   @param couleur La couleur à écrire.
   @return La suite du flux.
 */
std::ostream & operator<<(std::ostream & out, Carte::Couleur couleur);

/**
   @brief Affiche le nom d'une Valeur.
   
   @param[out] out Le flux à utiliser.
   @param valeur La valeur à écrire.
   @return La suite du flux.
 */
std::ostream & operator<<(std::ostream & out, Carte::Valeur valeur);

/**
   @brief Affiche le nom d'une modalité d'écart.
   
   @param[out] out Le flux à utiliser.
   @param mod La modalité à écrire.
   @return La suite du flux.
 */
std::ostream & operator<<(std::ostream & out, Carte::ModaliteEcart mod);

/**
   @brief Affiche le nom d'une force d'excuse.
   
   @param[out] out Le flux à utiliser.
   @param exc La force d'excuse à écrire.
   @return La suite du flux.
 */
std::ostream & operator<<(std::ostream & out, Carte::ForceExcuse exc);

#endif
