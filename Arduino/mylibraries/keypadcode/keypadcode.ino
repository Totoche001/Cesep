// Libraries
#include <Keypad.h>  // https://github.com/Chris--A/Keypad
#include <Key.h>
#include <LiquidCrystal.h>

// Constants
#define ROWS 4
#define COLS 4
#define DLENGTH 5
#define MAX_ATTEMPTS 5

// Parameters
const char kp4x4Keys[ROWS][COLS] = { { '1', '2', '3', 'A' }, { '4', '5', '6', 'B' }, { '7', '8', '9', 'C' }, { '*', '0', '#', 'D' } };
byte rowKp4x4Pin[ROWS] = { 2, 3, 4, 5 };      // Connexion des rangées, pour l'instant c'est inversé
byte colKp4x4Pin[COLS] = { A0, A1, A2, A3 };  // Connexion des colonnes, pour l'instant c'est inversé
//byte rowKp4x4Pin [4] = {11, 10, 9, 8};
//byte colKp4x4Pin [4] = {7, 6, 5, 4};
char digits[DLENGTH + 1];  // Tableau de caractères pour stocker l'entrée utilisateur
char code[] = "7854A";     // Code correct
char resetCode[] = "5555*";
int attemptCount = 0;                  // Compteur de tentatives
int index = 0;                         // Index pour l'entrée des chiffres
bool lockedOut = false;                // Système bloqué après MAX_ATTEMPTS
bool maxAttemptsMessageShown = false;  // Message de blocage montré
String input = "";                     // Inscription du code dans l'écran
// Variables
Keypad kp4x4 = Keypad(makeKeymap(kp4x4Keys), rowKp4x4Pin, colKp4x4Pin, ROWS, COLS);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);  // Brochage du LCD1602

void resetSystem() {
  attemptCount = 0;
  index = 0;
  lockedOut = false;
  maxAttemptsMessageShown = false;
  input = "";
  lcd.clear();
  lcd.print("System reset!");
  delay(2000);  // Pause avant de redémarrer
  lcd.clear();
  lcd.print("Enter code:");
}

// Initialisation du système
void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Initialize");
  delay(2000);
  lcd.clear();
  lcd.print("Enter code:");
  input = "";  // Réinitialise l'entrée utilisateur
}

// Boucle principale
void loop() {
  if (!lockedOut) {
    readKp4x4();  // Lecture des touches si le système n'est pas bloqué
  } else {
    if (!maxAttemptsMessageShown) {
      lcd.clear();
      lcd.print("System locked");
      maxAttemptsMessageShown = true;  // Affiche une seule fois
    }
    // Lecture des touches pour le code de réinitialisation
    readResetCode();
  }
}

// Fonction pour lire les entrées du clavier
void readKp4x4() {
  char customKey = kp4x4.getKey();  // Lire une touche du clavier

  if (customKey) {
    // Ajouter la touche entrée au tableau
    digits[index] = customKey;
    index++;

    // Afficher les chiffres/lettres à mesure qu'ils sont entrés
    lcd.setCursor(index - 1, 1);  // Afficher à la position de l'index
    lcd.print(customKey);

    // Lorsque 5 caractères ont été saisis
    if (index == DLENGTH) {
      digits[DLENGTH] = '\0';  // Ajouter le caractère de fin de chaîne
      lcd.setCursor(0, 1);
      lcd.print(digits);  // Afficher le code saisi
      delay(1000);        // Pause avant la vérification

      attemptCount++;  // Incrémenter le compteur de tentatives

      // Vérifier si le code est correct
      if (strcmp(digits, code) == 0) {
        lcd.clear();
        lcd.print("Code correct");
        delay(2000);
        resetSystem();  // Réinitialiser le système après un code correct
      } else {
        lcd.clear();
        lcd.print("Code incorrect");
        delay(1000);

        // Si le nombre maximum de tentatives est atteint
        if (attemptCount >= MAX_ATTEMPTS) {
          lockedOut = true;  // Verrouille le système
        }
      }

      index = 0;    // Réinitialise l'index pour une nouvelle entrée
      lcd.clear();  // Efface l'écran après chaque tentative complète
      lcd.print("Enter code:");
    }
  }
}

// Fonction pour lire le code de réinitialisation
void readResetCode() {
  char resetKey = kp4x4.getKey();  // Lire une touche du clavier

  if (resetKey) {
    // Ajouter la touche entrée au tableau
    digits[index] = resetKey;
    index++;

    // Afficher les chiffres/lettres à mesure qu'ils sont entrés
    lcd.setCursor(index - 1, 1);  // Afficher à la position de l'index
    lcd.print(resetKey);

    // Lorsque 5 caractères ont été saisis pour le code de réinitialisation
    if (index == DLENGTH) {
      digits[DLENGTH] = '\0';  // Ajouter le caractère de fin de chaîne
      lcd.setCursor(0, 1);
      lcd.print(digits);  // Afficher le code saisi
      delay(1000);        // Pause avant la vérification

      // Vérifier si le code de réinitialisation est correct
      if (strcmp(digits, resetCode) == 0) {
        lcd.clear();
        lcd.print("Reset correct");
        delay(2000);
        resetSystem();  // Réinitialiser le système après le bon code admin
      } else {
        lcd.clear();
        lcd.print("Reset incorrect");
        delay(1000);
        index = 0;  // Réinitialise l'index si le code est incorrect
      }
    }
  }
}