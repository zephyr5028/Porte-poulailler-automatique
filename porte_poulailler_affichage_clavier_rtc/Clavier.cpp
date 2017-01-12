/* Clavier.cpp
  définitions de la classe Clavier
*/

#include "Clavier.h"

Clavier::Clavier() : m_oldKey (-1), m_MenuManuel(14), m_NumKeys(6), m_pinBp(9), m_pinBoitier(6), m_relacheBp(true), m_debounce(350),
  m_AdcKeyVal( {
  50, 200, 400, 600, 800
})
{

}
/* sucharge du constructeur avec le nombre de lignes du menu */
Clavier::Clavier(byte nbMenu, const byte pinBp, const byte pinBoitier, const int debounce, const boolean debug) : m_oldKey (-1), m_MenuManuel(nbMenu), m_debug(debug), m_NumKeys(6),
  m_pinBp(pinBp), m_pinBoitier(pinBoitier), m_relacheBp(true), m_debounce(debounce), m_AdcKeyVal( {
  50, 200, 400, 600, 800
})
{

}

Clavier::~Clavier() {

}

//------routine get_key : Convert ADC value to key number------
int Clavier::get_key(int &input) {
  int k;
  for (k = 1; k < m_NumKeys; k++ ) {
    if (input < m_AdcKeyVal [k - 1]) {
      return k;
    }
  }
  if (k >= m_NumKeys) {
    k = -1; // No valid key pressed
    return k;
  }
}

//-----routine read_key : lecture du clavier-----
int Clavier::read_key(const int &sensor) {
  int  adc_key_in = analogRead(sensor); // read the value from the sensor A1
  byte key = get_key(adc_key_in); // convert into key press
  if (key != m_oldKey) { // if keypress is detected
    if (m_debug) {
      Serial.println(adc_key_in);
    }
    delay(100); // wait for debounce time
    adc_key_in = analogRead(sensor); // read the value from the sensor
    key = get_key(adc_key_in); // convert into key press
    if (key != m_oldKey)  {
      return  key;
    }
  }
  return m_oldKey;
}

//-----position du menu pour l'affichage - touches 2 et 3 -----
void Clavier::positionMenu(byte &positionMenu, const byte &toucheClavier) {
  if (toucheClavier == 3 ) {
    if (positionMenu < m_MenuManuel) {
      positionMenu++; // +1
    } else {
      positionMenu = 1;
    }
  }
  if (toucheClavier == 2 ) {
    if (positionMenu > 1) {
      positionMenu-- ; // -1
    } else {
      positionMenu = m_MenuManuel;
    }
  }
}

//-----relache touche-----
void Clavier::relacheTouche(const int &touche, bool &relache) {
  if (touche == m_oldKey ) { // si touche == -1, donc  relache=true
    relache = true;
  }
}

//-----deplacement dans le menu-----
bool Clavier::deplacementDansMenu(const &touche, bool &relache, const bool &reglage) {
  if ((touche == 2 or touche == 3) and relache and !reglage) { // si appui sur les touches 2 ou 3 , la touche relache et le mode reglage
    relache = false;
    return true;
  } else {
    return false;
  }
}

//-----test touche 5-----
bool Clavier::testTouche5(const &touche, bool &relache) {
  if (touche == 5 and relache == true ) { // retro eclairage si appuis sur la touche 5
    relache = false;
    return true;
  } else {
    return false;
  }
}

//-----test touche Bp-----
bool Clavier::testToucheBp(unsigned long  &tempoDebounce) {
  if (((millis() - tempoDebounce) > m_debounce)  and  m_relacheBp == true and !digitalRead(m_pinBp) ) {
    m_relacheBp = false;
    return true;
  } else {
    return false;
  }
}

//-----test relache Bp-----
void Clavier::testRelacheBp (volatile bool &interruptBp) {
  if (m_relacheBp == false and digitalRead(m_pinBp) ) {
    interruptBp = false; // autorisation de la prise en compte de l'IT
    m_relacheBp = true;
  }
}

//-----test IT Bp-----
void Clavier::testInterruptionBp (volatile bool &interruptBp, unsigned long  &tempoDebounce) {
  if (!digitalRead(m_pinBp) and !interruptBp) { // entree 9 pour interruption BP
    interruptBp = true;
    tempoDebounce = millis();
  }
}

//-----test IT ouverture boitier-----
void Clavier::testInterruptionBoitier (volatile bool &interruptOuvBoi) {
  if (!digitalRead(m_pinBoitier) and !interruptOuvBoi) { // entree 9 pour interruption BP
    interruptOuvBoi = true;
  }
}

//-----test boitier ouvert------
bool  Clavier::testBoitierOuvert(const volatile bool &interruptOuvBoi, const bool &boitierOuvert) {
  if ( interruptOuvBoi and !digitalRead(m_pinBoitier) and !boitierOuvert) {    //  interruption ouverture boitier
    return true;
  } else {
    return false;
  }
}

//-----test boitier ferme------
bool  Clavier::testBoitierFerme(const volatile bool &interruptOuvBoi, const bool &boitierOuvert) {
  if (digitalRead(m_pinBoitier) and boitierOuvert) { //  fermeture boitier
    return true;
  } else {
    return false;
  }
}

