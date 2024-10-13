#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "DHT.h"

// Définir les broches pour l'écran TFT
#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8

// Initialisation de l'écran TFT
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Initialisation du capteur DHT
DHT dht(2, DHT11);

byte SEC = 0;
byte MIN = 59;
byte HOUR = 7;
byte MIN_A = 0;
byte HOUR_A = 8;
unsigned long timer;

boolean button1WasUp = true;
boolean button2WasUp = true;
boolean button1IsUp;
boolean button2IsUp;

boolean alarm = true;
byte w, i;

void setup() {
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  dht.begin();
  tft.initR(INITR_BLACKTAB);   // Initialisation pour l'écran spécifique (ST7735R)
  tft.setRotation(1);          // Orientation de l'écran
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  timer = millis();
}

void loop() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  button1IsUp = digitalRead(9);
  button2IsUp = digitalRead(8);

  if (button1WasUp && !button1IsUp) {
    delay(10);
    button1IsUp = digitalRead(9);
    if (!button1IsUp) {
      SEC = 0;
      w = 1;
    }
  }
  button1WasUp = button1IsUp;

  if (button2WasUp && !button2IsUp) {
    delay(10);
    button2IsUp = digitalRead(8);
    if (!button2IsUp) {
      alarm = !alarm;
    }
  }
  button2WasUp = button2IsUp;

  if (millis() - timer > 1000) {
    timer = millis();
    SEC++;
    if (SEC > 59) {
      SEC = 0;
      MIN++;
    }
    if (MIN > 59) {
      MIN = 0;
      HOUR++;
    }
    if (HOUR > 23) {
      HOUR = 0;
    }

    if (alarm && HOUR == HOUR_A && MIN == MIN_A && i < 60) {
      tone(5, 50);
      i++;
    }
    if (i >= 60) {
      noTone(5);
      alarm = false;
      i = 0;
    }

    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(10, 10);

    // Affichage de l'heure et des minutes
    if (HOUR < 10) {
      tft.print("0");
    }
    tft.print(HOUR);
    tft.print(":");
    if (MIN < 10) {
      tft.print("0");
    }
    tft.print(MIN);
    tft.print(":");
    if (SEC < 10) {
      tft.print("0");
    }
    tft.print(SEC);

    // Affichage de l'état de l'alarme
    tft.setCursor(10, 50);
    if (alarm) {
      tft.print("Alarm ON");
    } else {
      tft.print("Alarm OFF");
    }

    // Affichage de la température et de l'humidité
    tft.setCursor(10, 90);
    tft.print("Temp: ");
    tft.print(t);
    tft.print("C ");
    tft.print("Hum: ");
    tft.print(h);
    tft.print("%");
    
    noTone(5);
  }
  
  // Boucles de réglages similaires à l'écran OLED...
  // Vous pouvez les adapter en suivant la même structure d'affichage avec tft.print()
}


