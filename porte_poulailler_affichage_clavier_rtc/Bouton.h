/* Bouton.h
  prototype de la Bouton
*/

#ifndef Bouton_h
#define Bouton_h

#include "Arduino.h"

class Bouton
{
  public:

    Bouton(); // constructeur
    Bouton( const byte pinBp, const byte pinBoitier, const int debounce = 350, const boolean debug = false ); // surcharge du constructeur
    ~Bouton(); // destructeur

    bool testToucheBp(unsigned long  &tempoDebounce);  //test touche Bp
    void testRelacheBp (volatile bool &interruptBp);//test relache Bp
    void testInterruptionBp (volatile bool &interruptBp, unsigned long  &tempoDebounce); //test Bp suite interruption
    void testInterruptionBoitier (volatile bool &interruptOuvBoi); //test IT ouverture boitier
    bool  testBoitierOuvert(const volatile bool &interruptOuvBoi, const bool &boitierOuvert);//test boitier ouvert
    bool  testBoitierFerme(const volatile bool &interruptOuvBoi, const  bool &boitierOuvert); //test boitier ferme

  protected:

    const boolean m_debug ; // pour affichage console si nécessaire
    const byte m_pinBp; // pin D9 bouton poussoir ouverture / fermeture
    const byte m_pinBoitier; //pin D6 interrupteur ouverture boitier
    const int m_debounce; // debounce latency in ms (350)
    bool m_relacheBp; // relache du Bp

};

#endif

