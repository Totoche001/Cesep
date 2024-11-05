#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal.h>  // Pour l'écran LCD1602
#include <SD.h>
#include <SPI.h>
#include <Servo.h>

// Paramètres du clavier matriciel
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String password = "1234";
String input = "";
int attempts = 0;
bool isLocked = true;
Servo myServo;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // Brochage du LCD1602

const int buzzerPin = 8;
const int sdPin = 10;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  // Initialise l'écran LCD
  lcd.clear();
  lcd.print("Entrez le mot de");
  lcd.setCursor(0, 1);
  lcd.print("passe:");

  pinMode(buzzerPin, OUTPUT);
  myServo.attach(6);  // Connecte le servo sur la pin 6
  myServo.write(0);    // Ferme la boîte par défaut

  if (!SD.begin(sdPin)) {
    lcd.clear();
    lcd.print("Erreur SD!");
    while (1);  // Arrêt si la carte SD ne fonctionne pas
  }
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == '#') {  // Si l'utilisateur termine la saisie
      if (input == password) {
        lcd.clear();
        lcd.print("Mot de passe OK");
        tone(buzzerPin, 1000, 500);  // Son pour bon mot de passe
        delay(1000);
        playVideo();  // Démarre la simulation d'impression avec vidéo
        openBox();    // Ouvre la boîte
      } else {
        attempts++;
        lcd.clear();
        lcd.print("Mauvais code!");
        tone(buzzerPin, 500, 1000);  // Son pour mauvais mot de passe
        delay(1000);
        if (attempts >= 3) {
          lcd.clear();
          lcd.print("Trop d'erreurs!");
          lockSystem();  // Verrouille après trois erreurs
        }
      }
      input = "";  // Réinitialise l'entrée
    } else if (key == '*') {  // Reset manuel
      resetSystem();
    } else {
      input += key;  // Ajoute la touche au mot de passe
      lcd.clear();
      lcd.print("Mot de passe: ");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
  }
}

void playVideo() {
  // Simulation de l’impression via vidéo
  lcd.clear();
  lcd.print("Impression...");
  delay(5000);  // Ici, vous devrez jouer la vidéo réelle
}

void openBox() {
  lcd.clear();
  lcd.print("Ouverture...");
  myServo.write(90);  // Ouvre la boîte
  tone(buzzerPin, 1500, 1000);  // Son pour la boîte qui s'ouvre
  delay(2000);
  myServo.write(0);  // Referme la boîte après un moment
}

void lockSystem() {
  isLocked = true;
  lcd.clear();
  lcd.print("Verrouille");
  tone(buzzerPin, 200, 1000);
  delay(5000);
}

void resetSystem() {
  attempts = 0;
  isLocked = false;
  lcd.clear();
  lcd.print("Systeme reset");
  delay(2000);
  lcd.clear();
  lcd.print("Entrez le mot de");
  lcd.setCursor(0, 1);
  lcd.print("passe:");
}
