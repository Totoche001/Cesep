#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// Définir les broches pour l'écran TFT
#define TFT_CS     10
#define TFT_RST    8  // Connecté à reset ou à une broche Arduino
#define TFT_DC     9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello! ST7735 TFT Test");

  // Ajouter une réinitialisation manuelle (si nécessaire)
  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, LOW);
  delay(100);
  digitalWrite(TFT_RST, HIGH);
  delay(500);  // Laisser du temps à l'écran de se stabiliser

  // Initialiser l'écran avec un ST7735 avec une puce "Black tab"
  tft.initR(INITR_BLACKTAB);
  tft.setSPISpeed(2000000);  // Réduire la vitesse SPI à 2 MHz

  // Effacer l'écran
  tft.fillScreen(ST7735_BLACK);
  delay(500);

  // Test d'affichage de texte rapide pour voir la fréquence
  Serial.println("Testing text refresh rates...");
  testTextRefreshRate();
}

void loop() {
  // Inverser l'affichage pour l'effet visuel
  tft.invertDisplay(true);
  delay(500);
  tft.invertDisplay(false);
  delay(500);
}

// Fonction pour tester la vitesse de rafraîchissement de texte
void testTextRefreshRate() {
  // Variables pour mesurer le temps de rafraîchissement
  unsigned long startTime, elapsedTime;

  // Test d'affichage de texte à différentes vitesses
  for (int i = 0; i < 10; i++) {
    // Mesurer le temps pour chaque affichage
    startTime = millis();
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(2);
    tft.println("Test de frequence:");
    tft.setTextSize(1);
    tft.println("Iteration:");
    tft.setTextSize(3);
    tft.println(i);
    elapsedTime = millis() - startTime;

    // Afficher les résultats dans la console série
    Serial.print("Iteration ");
    Serial.print(i);
    Serial.print(" refresh time: ");
    Serial.print(elapsedTime);
    Serial.println(" ms");

    delay(1000);  // Attendre avant de changer l'affichage
  }
}
