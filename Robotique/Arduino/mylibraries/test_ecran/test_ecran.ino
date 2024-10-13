#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Définir les broches pour l'écran TFT
#define TFT_CS     10
#define TFT_RST    4
#define TFT_DC     6

// Initialisation de l'écran TFT
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  // Initialisation de l'écran
  tft.initR(INITR_BLACKTAB);  // Initialisation pour ST7735R
  tft.setRotation(1);         // Orientation horizontale
  tft.fillScreen(ST77XX_BLACK);

  // Affichage de texte simple
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Ecran TFT OK");

  // Dessin de formes simples
  tft.drawRect(10, 30, 100, 50, ST77XX_RED);   // Dessiner un rectangle rouge
  tft.fillCircle(60, 100, 20, ST77XX_GREEN);   // Dessiner un cercle vert rempli
}

void loop() {
  // Rien dans la boucle principale
}

Broche de l'écran TFT	Fonction	Broche Arduino
GND	                  Masse	     GND
VCC	                  Alimentation	3.3V
SCK	                  Horloge SPI (SCK)	D13
SDA	                  Données SPI (MOSI)	D11
RES	                  Reset	D4
RS (ou DC)	          Data/Command	D6
CS	                  Chip Select (CS)	D10