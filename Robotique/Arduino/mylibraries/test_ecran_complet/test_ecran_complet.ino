#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// Définir les broches pour l'écran TFT
#define TFT_CS     10
#define TFT_RST    8  // Vous pouvez aussi connecter ceci au reset de l'Arduino (et définir à 0 dans ce cas)
#define TFT_DC     9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello! ST7735 TFT Test");

  // Initialiser l'écran avec un ST7735 avec une puce "Black tab"
  tft.initR(INITR_BLACKTAB);
  Serial.println("Initialized");

  // Effacer l'écran
  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;
  Serial.print("Cleared screen in: ");
  Serial.print(time);
  Serial.println(" ms");
  delay(500);

  // Afficher un large bloc de texte
  tft.fillScreen(ST7735_BLACK);
  testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit.", ST7735_WHITE);
  delay(1000);

  // Test de l'impression
  tftPrintTest();
  delay(4000);

  // Dessin de pixel unique
  tft.drawPixel(tft.width() / 2, tft.height() / 2, ST7735_GREEN);
  delay(500);

  // Tests des lignes et formes
  testlines(ST7735_YELLOW);
  delay(500);

  testfastlines(ST7735_RED, ST7735_BLUE);
  delay(500);

  testdrawrects(ST7735_GREEN);
  delay(500);

  testfillrects(ST7735_YELLOW, ST7735_MAGENTA);
  delay(500);

  tft.fillScreen(ST7735_BLACK);
  testfillcircles(10, ST7735_BLUE);
  testdrawcircles(10, ST7735_WHITE);
  delay(500);

  testroundrects();
  delay(500);

  testtriangles();
  delay(500);

  mediabuttons();
  delay(500);

  Serial.println("done");
  delay(1000);
}

void loop() {
  // Inverser l'affichage pour l'effet visuel
  tft.invertDisplay(true);
  delay(500);
  tft.invertDisplay(false);
  delay(500);
}

// Fonction pour dessiner du texte
void testdrawtext(const char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

// Fonction pour tester l'impression de texte sur l'écran
void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(3);
  tft.print("0x");
  tft.println(0xDEADBEEF, HEX);
}

// Fonction pour dessiner des lignes
void testlines(uint16_t color) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawLine(0, 0, x, tft.height() - 1, color);
  }
  for (int16_t y = 0; y < tft.height(); y += 6) {
    tft.drawLine(0, 0, tft.width() - 1, y, color);
  }
}

// Dessiner des lignes optimisées
void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t y = 0; y < tft.height(); y += 5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x = 0; x < tft.width(); x += 5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

// Fonction pour dessiner des rectangles
void testdrawrects(uint16_t color) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawRect(x, x, tft.width() - 2 * x, tft.height() - 2 * x, color);
  }
}

// Fonction pour dessiner des rectangles remplis
void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x = tft.width() - 1; x > 6; x -= 6) {
    tft.fillRect(x, x, tft.width() - 2 * x, tft.height() - 2 * x, color1);
    tft.drawRect(x, x, tft.width() - 2 * x, tft.height() - 2 * x, color2);
  }
}

// Fonction pour dessiner des cercles
void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x = 0; x < tft.width(); x += radius * 2) {
    for (int16_t y = 0; y < tft.height(); y += radius * 2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

// Fonction pour dessiner des cercles remplis
void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x = radius; x < tft.width(); x += radius * 2) {
    for (int16_t y = radius; y < tft.height(); y += radius * 2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

// Fonction pour dessiner des rectangles arrondis
void testroundrects() {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawRoundRect(x, x, tft.width() - 2 * x, tft.height() - 2 * x, tft.height() / 4, ST7735_WHITE);
  }
}

// Fonction pour dessiner des triangles
void testtriangles() {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x = 0; x < tft.width() / 2; x += 5) {
    tft.drawTriangle(tft.width() / 2, tft.height() / 2 - x, tft.width() / 2 - x, tft.height() / 2 + x, tft.width() / 2 + x, tft.height() / 2 + x, ST7735_RED);
  }
}

// Fonction pour simuler des boutons multimédias
void mediabuttons() {
  tft.fillScreen(ST7735_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, ST7735_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST7735_RED);
  delay(500);
  tft.fillRoundRect(25, 90, 78, 60, 8, ST7735_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, ST7735_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST7735_GREEN);
}
