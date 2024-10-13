#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Définir les broches pour l'écran TFT
#define TFT_CS     10
#define TFT_RST    4
#define TFT_DC     6

// Initialisation de l'écran TFT
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Variables pour l'heure
unsigned long previousMillis = 0;
const long interval = 1000;  // Intervalle d'une seconde
byte seconds = 0;
byte minutes = 0;
byte hours = 0;

void setup() {
  // Initialisation de l'écran
  tft.initR(INITR_BLACKTAB);  // Initialisation spécifique pour ST7735R
  tft.setRotation(1);         // Orientation horizontale
  tft.fillScreen(ST77XX_BLACK);

  // Configuration du texte sur l'écran
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);

  // Afficher une heure de départ à 00:00:00
  displayTime();
}

void loop() {
  // Obtenir le temps actuel en millisecondes
  unsigned long currentMillis = millis();

  // Si une seconde s'est écoulée, mettre à jour l'heure
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    updateClock();
    displayTime();
  }
}

// Mettre à jour les variables de l'horloge
void updateClock() {
  seconds++;
  if (seconds > 59) {
    seconds = 0;
    minutes++;
    if (minutes > 59) {
      minutes = 0;
      hours++;
      if (hours > 23) {
        hours = 0;
      }
    }
  }
}

// Afficher l'heure sur l'écran TFT
void displayTime() {
  // Effacer l'écran
  tft.fillScreen(ST77XX_BLACK);

  // Afficher les heures
  tft.setCursor(20, 30);
  if (hours < 10) {
    tft.print("0");  // Ajouter un zéro devant les chiffres uniques
  }
  tft.print(hours);
  tft.print(":");

  // Afficher les minutes
  if (minutes < 10) {
    tft.print("0");
  }
  tft.print(minutes);
  tft.print(":");

  // Afficher les secondes
  if (seconds < 10) {
    tft.print("0");
  }
  tft.print(seconds);
}

Connexions :

    GND de l'écran → GND de l'Arduino.
    VCC de l'écran → 3.3V de l'Arduino.
    SCK de l'écran → D13 de l'Arduino (SPI SCK).
    SDA (MOSI) de l'écran → D11 de l'Arduino (SPI MOSI).
    RES de l'écran → D4 de l'Arduino.
    RS (DC) de l'écran → D6 de l'Arduino.
    CS de l'écran → D10 de l'Arduino.
