#include "DHT.h"
DHT dht(2, DHT11);

#include "OLED_I2C.h"
OLED myOLED(SDA, SCL, 8);

extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];

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
  myOLED.begin();
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

    myOLED.clrScr();
    myOLED.setFont(BigNumbers);

    // Construction de la chaîne de l'heure
    String timeStr;
    if (HOUR < 10) {
      timeStr += F("0");
    }
    timeStr += HOUR;
    timeStr += F("-");
    if (MIN < 10) {
      timeStr += F("0");
    }
    timeStr += MIN;
    timeStr += F("-");
    if (SEC < 10) {
      timeStr += F("0");
    }
    timeStr += SEC;

    myOLED.print(timeStr, 8, 20);

    myOLED.setFont(SmallFont);
    myOLED.print(alarm ? F("on ") : F("off"), RIGHT, 3);

    // Construction de la chaîne température/humidité
    String tempHumStr;
    tempHumStr += t;
    tempHumStr += F("' / ");
    tempHumStr += h;
    tempHumStr += F("% ");
    myOLED.print(tempHumStr, RIGHT, 55);

    myOLED.update();
    noTone(5);
  }

  // Boucle de réglage de l'heure
  while (w == 1) {
    myOLED.clrScr();
    myOLED.setFont(BigNumbers);
    if (HOUR > 23) {
      HOUR = 0;
    }

    String timeStr;
    if (HOUR < 10) {
      timeStr += F("0");
    }
    timeStr += HOUR;
    timeStr += F("-");
    if (MIN < 10) {
      timeStr += F("0");
    }
    timeStr += MIN;
    timeStr += F("-");
    timeStr += F("00");
    myOLED.print(timeStr, 8, 20);

    myOLED.setFont(SmallFont);
    myOLED.print(alarm ? F("on ") : F("off"), RIGHT, 3);
    myOLED.print(F("HOUR SETTING"), CENTER, 55);
    myOLED.update();

    button1IsUp = digitalRead(9);
    button2IsUp = digitalRead(8);
    if (button1WasUp && !button1IsUp) {
      delay(10);
      button1IsUp = digitalRead(9);
      if (!button1IsUp) {
        w = 2;
      }
    }
    button1WasUp = button1IsUp;
    if (button2WasUp && !button2IsUp) {
      delay(10);
      button2IsUp = digitalRead(8);
      if (!button2IsUp) {
        HOUR++;
      }
    }
    button2WasUp = button2IsUp;
  }

  // Boucle de réglage des minutes
  while (w == 2) {
    myOLED.clrScr();
    myOLED.setFont(BigNumbers);
    if (MIN > 59) {
      MIN = 0;
    }

    String timeStr;
    if (HOUR < 10) {
      timeStr += F("0");
    }
    timeStr += HOUR;
    timeStr += F("-");
    if (MIN < 10) {
      timeStr += F("0");
    }
    timeStr += MIN;
    timeStr += F("-");
    timeStr += F("00");
    myOLED.print(timeStr, 8, 20);

    myOLED.setFont(SmallFont);
    myOLED.print(alarm ? F("on ") : F("off"), RIGHT, 3);
    myOLED.print(F("MINUTE SETTING"), CENTER, 55);
    myOLED.update();

    button1IsUp = digitalRead(9);
    button2IsUp = digitalRead(8);
    if (button1WasUp && !button1IsUp) {
      delay(10);
      button1IsUp = digitalRead(9);
      if (!button1IsUp) {
        w = 3;
      }
    }
    button1WasUp = button1IsUp;
    if (button2WasUp && !button2IsUp) {
      delay(10);
      button2IsUp = digitalRead(8);
      if (!button2IsUp) {
        MIN++;
      }
    }
    button2WasUp = button2IsUp;
  }

  // Boucle de réglage de l'heure de l'alarme
  while (w == 3) {
    myOLED.clrScr();
    myOLED.setFont(BigNumbers);
    if (MIN_A > 59) {
      MIN_A = 0;
      HOUR_A++;
    }
    if (HOUR_A > 23) {
      HOUR_A = 0;
    }

    String timeStr;
    if (HOUR_A < 10) {
      timeStr += F("0");
    }
    timeStr += HOUR_A;
    timeStr += F("-");
    if (MIN_A < 10) {
      timeStr += F("0");
    }
    timeStr += MIN_A;
    timeStr += F("-");
    timeStr += F("00");
    myOLED.print(timeStr, 8, 20);

    myOLED.setFont(SmallFont);
    myOLED.print(alarm ? F("on ") : F("off"), RIGHT, 3);
    myOLED.print(F("ALARM SET HOUR"), CENTER, 55);
    myOLED.update();

    button1IsUp = digitalRead(9);
    button2IsUp = digitalRead(8);
    if (button1WasUp && !button1IsUp) {
      delay(10);
      button1IsUp = digitalRead(9);
      if (!button1IsUp) {
        w = 4;
      }
    }
    button1WasUp = button1IsUp;
    if (button2WasUp && !button2IsUp) {
      delay(10);
      button2IsUp = digitalRead(8);
      if (!button2IsUp) {
        HOUR_A++;
      }
    }
    button2WasUp = button2IsUp;
  }

  // Boucle de réglage des minutes de l'alarme
  while (w == 4) {
    myOLED.clrScr();
    myOLED.setFont(BigNumbers);
    if (MIN_A > 59) {
      MIN_A = 0;
      HOUR_A++;
    }
    if (HOUR_A > 23) {
      HOUR_A = 0;
    }

    String timeStr;
    if (HOUR_A < 10) {
      timeStr += F("0");
    }
    timeStr += HOUR_A;
    timeStr += F("-");
    if (MIN_A < 10) {
      timeStr += F("0");
    }
    timeStr += MIN_A;
    timeStr += F("-");
    timeStr += F("00");
    myOLED.print(timeStr, 8, 20);

    myOLED.setFont(SmallFont);
    myOLED.print(alarm ? F("on ") : F("off"), RIGHT, 3);
    myOLED.print(F("ALARM SET MINUTE"), CENTER, 55);
    myOLED.update();

    button1IsUp = digitalRead(9);
    button2IsUp = digitalRead(8);
    if (button1WasUp && !button1IsUp) {
      delay(10);
      button1IsUp = digitalRead(9);
      if (!button1IsUp) {
        w = 0;
      }
    }
    button1WasUp = button1IsUp;
    if (button2WasUp && !button2IsUp) {
      delay(10);
      button2IsUp = digitalRead(8);
      if (!button2IsUp) {
        MIN_A++;
      }
    }
    button2WasUp = button2IsUp;
  }
}
