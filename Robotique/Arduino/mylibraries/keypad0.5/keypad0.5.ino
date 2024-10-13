#include <Keypad.h>
#include <LiquidCrystal.h>

#define DLENGTH 5  // Longueur maximale du mot de passe

const byte ROWS = 4;  // 4 rangées
const byte COLS = 4;  // 4 colonnes
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Connexion des rangées du keypad aux broches 2, 3, 4, 5
byte rowPins[ROWS] = {2, 3, 4, 5};     
// Connexion des colonnes du keypad aux broches A0, A1, A2, A3
byte colPins[COLS] = {A0, A1, A2, A3}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // Brochage du LCD1602

String password = "7458A";           // Mot de passe utilisateur
String unlockPassword = "5555*";     // Mot de passe administrateur pour déblocage
String input = "";                   // Stocke l'entrée utilisateur
int attempts = 0;
int maxAttempts = 5;                // Limite de 5 essais
bool isLocked = false;              // Variable de verrouillage après 5 essais
bool adminMode = false;             // Mode administrateur activé avec '*'

void setup() {
  lcd.begin(16, 2);
  lcd.print("Entrez le code:");
}

void loop() {
  char key = keypad.getKey();

  // Si le système est bloqué, on peut entrer en mode administrateur avec '*'
  if (isLocked && key == '*') {
    adminMode = true;
    lcd.clear();
    lcd.print("Admin: entrez code");
    input = ""; // Réinitialise l'entrée pour l'administrateur
  }

  if (key && !isLocked) {
    // Si l'utilisateur appuie sur '#' pour soumettre
    if (key == '#') {  
      if (input == password) {
        lcd.clear();
        lcd.print("Mot de passe OK");
        delay(2000);
        attempts = 0;  // Réinitialise les essais après succès
        input = "";    // Réinitialise l'entrée
      } else {
        attempts++;
        lcd.clear();
        lcd.print("Erreur!");
        tone(8, 500, 1000);  // Son pour mauvais mot de passe
        delay(1000);
        if (attempts >= maxAttempts) {
          lcd.clear();
          lcd.print("Trop d'erreurs!");
          lockSystem();  // Verrouille le système après trop d'essais
        }
        input = "";  // Réinitialise l'entrée après échec
      }
    } 
    // Ajouter les touches uniquement si la longueur de l'entrée est inférieure à DLENGTH
    else if (input.length() < DLENGTH) {
      input += key;  // Ajoute la touche au mot de passe
      lcd.clear();
      lcd.print("Code: ");
      lcd.setCursor(0, 1);
      lcd.print(input);
    } else {
      lcd.clear();
      lcd.print("Limite de 5 car.");
      delay(1000);  // Affiche un message temporaire
      lcd.clear();
      lcd.print("Entrez le code:");
      lcd.setCursor(0, 1);
      lcd.print(input);  // Réaffiche l'entrée existante
    }
  }

  // Mode administrateur après que '*' a été pressé
  if (adminMode && key) {
    if (key == '#') {
      if (input == unlockPassword) {
        lcd.clear();
        lcd.print("Debloque par admin");
        tone(8, 1500, 500);  // Son pour réinitialisation admin
        delay(2000);
        resetSystem();  // Appelle la fonction de déblocage admin
      } else {
        lcd.clear();
        lcd.print("Erreur admin!");
        delay(1000);
        input = "";  // Réinitialise l'entrée
      }
    } 
    // Ajouter les touches pour l'admin uniquement si la longueur de l'entrée est inférieure à DLENGTH
    else if (input.length() < DLENGTH) {
      input += key;
      lcd.clear();
      lcd.print("Admin: ");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
  }
}

void lockSystem() {
  isLocked = true;  // Verrouille le système après 5 essais incorrects
}

void resetSystem() {
  attempts = 0;     // Réinitialise le nombre d'essais
  isLocked = false; // Déverrouille le système
  adminMode = false; // Désactive le mode administrateur
  input = "";       // Réinitialise l'entrée utilisateur
  lcd.clear();
  lcd.print("Systeme reset");
  delay(2000);
  lcd.clear();
  lcd.print("Entrez le code:");
}
