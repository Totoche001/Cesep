#include "ClassLed.h"

#define PIN_ROUGE 3
#define PIN_VERTE 5
#define PIN_BLEUE 6
#define PAUSE 2000

ClassLed rouge = ClassLed(PIN_ROUGE);
ClassLed verte = ClassLed(PIN_VERTE);
ClassLed bleue = ClassLed(PIN_BLEUE);

void setup() {
  Serial.begin(9600);

  // Initialisation des LEDs à différentes intensités
  bleue.fadeTo(255);  // Bleue maximale
  rouge.fadeTo(170);  // Rouge moyenne
  verte.fadeTo(85);   // Verte faible

  Serial.println("Setup terminé");
}

bool modeIn = false;
uint32_t resetTimer = 0;

void loop() {
  // Animation de chaque LED
  bleue.animation();
  rouge.animation();
  verte.animation();

  // Changement de mode toutes les 6 secondes
  if (millis() - resetTimer >= 6000) {
    resetTimer = millis();  // Met à jour le timer

    if (modeIn) {
      // Si modeIn est activé, réinitialiser les LEDs à des intensités différentes
      modeIn = false;
      bleue.fadeTo(255);  // Intensité maximale
      rouge.fadeTo(170);  // Intensité moyenne
      verte.fadeTo(85);   // Intensité faible
    } else {
      // Si modeIn est désactivé, toutes les LEDs s'éteignent progressivement
      modeIn = true;
      bleue.fadeTo(0);    // Éteindre complètement
      rouge.fadeTo(0);    // Éteindre complètement
      verte.fadeTo(0);    // Éteindre complètement
    }
  }
}
