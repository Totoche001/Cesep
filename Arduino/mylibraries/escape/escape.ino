// ********************
// Importation des bibliothèques nécessaires
// ********************

#include "Servo.h"
#include <Keypad.h>
#include <Key.h>
#include <LiquidCrystal.h>
#include <PubSubClient.h>  // Bibliothèque pour MQTT
#include <Ethernet.h>      // Bibliothèque pour la connexion Ethernet (ou WiFi si tu utilises un module WiFi)

// ********************
// Variables et constantes globales
// ********************

// Horloge (Servo utilisé pour simuler le mouvement d'une horloge)
#define SERVO_CLOCK 22      // Broche du servo de l'horloge
#define BREAK_CLOCK 60000   // Temps entre chaque mouvement de l'horloge (60 secondes)
unsigned long clockTimer = 0; // Timer pour l'horloge
Servo clockServo;           // Servo pour l'horloge
byte clockStart = 0;        // Position initiale du servo de l'horloge

// Cadre (Servo utilisé pour ouvrir/fermer un cadre)
Servo cadreServo;     // Initialisation du servo pour le cadre
byte cadreOpen = 0;   // Position ouverte du cadre
byte cadreClose = 90; // Position fermée du cadre

// Boîte (Servo utilisé pour ouvrir/fermer une boîte) - imprimante
Servo boiteServo;           
#define Servo_boite 30      // Servo pour la boîte

// Livres (Boutons associés à des livres dans l'escape game)
#define buttonLivrePause 2000  // Délai pour la lecture des boutons de livre (2 secondes)
#define buttonLivre1 23        // Bouton livre 1
#define buttonLivre2 24        // Bouton livre 2
#define buttonLivre3 25        // Bouton livre 3
#define buttonLivre4 26        // Bouton livre 4
#define buttonLivre5 27        // Bouton livre 5
#define buttonLivre6 28        // Bouton livre 6
unsigned long buttonLivreTimer = 0;  // Timer pour les boutons de livre

// Clavier et écran LCD (pour entrer et afficher un code)
#define ROWS 4          // Nombre de lignes du clavier
#define COLS 4          // Nombre de colonnes du clavier
#define DLENGTH 5       // Longueur du code
#define MAX_ATTEMPTS 5  // Nombre maximum d'essais avant verrouillage

// Clavier matriciel 4x4
const char kp4x4Keys[ROWS][COLS] = { 
  { '1', '2', '3', 'A' }, 
  { '4', '5', '6', 'B' }, 
  { '7', '8', '9', 'C' }, 
  { '*', '0', '#', 'D' } 
};

// Déclaration des broches du clavier
byte rowKp4x4Pin[ROWS] = { 30, 31, 32, 33 };  // Broches pour les lignes
byte colKp4x4Pin[COLS] = { A0, A1, A2, A3 };  // Broches pour les colonnes

char digits[DLENGTH + 1];  // Tableau pour stocker l'entrée utilisateur
char code[] = "7854A";     // Code correct
char resetCode[] = "5555*"; // Code de réinitialisation
int attemptCount = 0;      // Compteur de tentatives
int index = 0;             // Index pour l'entrée des chiffres
bool lockedOut = false;    // Système bloqué après trop de tentatives
bool maxAttemptsMessageShown = false;  // Message de verrouillage affiché
String input = "";         // Variable pour stocker l'entrée de l'utilisateur
bool keypadEnabled = false; // Activer ou désactiver le clavier
bool correctCodeEntered = false;  // Flag pour savoir si le code correct a été entré


// Initialisation du clavier et de l'écran LCD
Keypad kp4x4 = Keypad(makeKeymap(kp4x4Keys), rowKp4x4Pin, colKp4x4Pin, ROWS, COLS);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);  // Connexion de l'écran LCD 1602

// ********************
// Connexion MQTT (pour la communication entre Arduino et un serveur via réseau)
// ********************
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

// Paramètres du serveur MQTT
const char* mqttServer = "X.X.X.X";  // Adresse IP du serveur MQTT
const int mqttPort = 1883;                 // Port du serveur MQTT
const char* mqttUser = "mqtt_user";        // Nom d'utilisateur MQTT
const char* mqttPassword = "mqtt_password"; // Mot de passe MQTT

// Variables d'état pour les différents éléments
bool isVideoIntro = false;  // État de l'intro vidéo
bool isHorloge = false;     // État de l'horloge
bool isCadre = false;       // État du cadre
bool isLivre = false;       // État des livres
bool isImprimante = false;  // État de l'imprimante

// Prototypes des fonctions
void handleHorloge();
void handleCadre();
void handleLivres();
void handleImprimante();
void connectToMqtt();
void sendMqttMessage(const char* topic, const char* payload);

void setup() {
  // Initialisation de l'écran LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Initialize");
  delay(2000);
  lcd.clear();
  lcd.print("Enter code:");
  input = "";  // Réinitialise l'entrée utilisateur

  // Initialisation du serveur MQTT
  mqttClient.setServer(mqttServer, mqttPort);

  // Attacher les servos aux broches
  cadreServo.attach(Servo_cadre);
  cadreServo.write(cadreClose);  // Fermer le cadre par défaut

  clockServo.attach(SERVO_CLOCK); // Attacher le servo de l'horloge
  clockServo.write(clockStart);   // Positionner l'horloge au départ

  boiteServo.attach(Servo_boite); // Attacher le servo de la boîte
  boiteServo.write(90);           // Position de la boîte fermée

  // Initialisation des broches pour les boutons des livres
  pinMode(buttonLivre1, INPUT_PULLUP);
  pinMode(buttonLivre2, INPUT_PULLUP);
  pinMode(buttonLivre3, INPUT_PULLUP);
  pinMode(buttonLivre4, INPUT_PULLUP);
  pinMode(buttonLivre5, INPUT_PULLUP);
  pinMode(buttonLivre6, INPUT_PULLUP);

  // Connexion au serveur MQTT
  connectToMqtt();
}

void loop() {
  // Gestion des différents éléments
  handleHorloge();    // Gestion du servo de l'horloge
  handleCadre();      // Gestion du cadre
  handleLivres();     // Gestion des boutons associés aux livres
  // Appeler handleImprimante uniquement si le bon code a été entré
  if (correctCodeEntered) {
    handleImprimante();
  }

  // Reconnecter au serveur MQTT si déconnecté
  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  mqttClient.loop();  // Maintenir la connexion MQTT active
}

// ********************
// Fonctions pour la gestion des éléments du jeu
// ********************

// Gestion de l'horloge
void handleHorloge() {
  if (!isHorloge) {return;}
    if (millis() - clockTimer >= BREAK_CLOCK) {
      clockTimer = millis();
      byte clockPosition = clockServo.read();
      clockServo.write(clockPosition + 6);  // Avancer l'horloge de 6 degrés
    
  }
}

// Gestion du cadre
void handleCadre() {
  if (!isVideoIntro) {return;}
    if (isCadre) {
      cadreServo.write(cadreOpen);  // Ouvre le cadre
    } else {
      cadreServo.write(cadreClose);  // Ferme le cadre
    
  }
}

// Gestion des livres (vérifie si un bouton de livre a été appuyé)
void handleLivres() {
  if (isLivre) {return;}
    if (millis() - buttonLivreTimer >= buttonLivrePause) {
      // Lire l'état de chaque bouton
      bool statebuttonLivre1 = !digitalRead(buttonLivre1);
      bool statebuttonLivre2 = !digitalRead(buttonLivre2);
      bool statebuttonLivre3 = !digitalRead(buttonLivre3);
      bool statebuttonLivre4 = !digitalRead(buttonLivre4);
      bool statebuttonLivre5 = !digitalRead(buttonLivre5);
      bool statebuttonLivre6 = !digitalRead(buttonLivre6);

      if (statebuttonLivre1 || statebuttonLivre2 || statebuttonLivre3 || statebuttonLivre4 || statebuttonLivre5 || statebuttonLivre6) {
        buttonLivreTimer = millis();  // Remet à jour le timer
        // Actions à réaliser selon le livre appuyé
      }
    }
  
}

// Gestion de l'imprimante (vérifie le code et gère l'ouverture de la boîte)
void handleImprimante() {
  /*if (isImprimante) {
    if (millis() - buttonImprimanteTimer >= buttonImprimantePause) {
      // Lire l'état du bouton
      bool statebuttonImprimante = !digitalRead(buttonImprimante);
    }}*/
  
  // Simuler un compte à rebours pour l'impression
  sendMqttMessage("impression/start", "start_video");
  lcd.clear();
  lcd.print("Impression en cours");

  for (int i = 5; i >= 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("Temps restant: ");
    lcd.print(i);
    lcd.print(" sec");
    delay(1000);
  }

  // Ouvrir la boîte à la fin de l'impression
  sendMqttMessage("boite/open", "open_boite");
  lcd.clear();
  lcd.print("Impression terminee");
  delay(2000);
  boiteServo.write(0);  // Ouvre la boîte
}

// ********************
// Fonctions pour la gestion du clavier et du code d'accès
// ********************

// Lecture des entrées du clavier
void readKp4x4() {
  if (keypadEnabled && !correctCodeEntered) {  // Activer seulement si le clavier est "allumé" et que le code n'a pas encore été validé
    char customKey = kp4x4.getKey();  // Lire une touche du clavier
    if (customKey) {
      // Ajouter la touche entrée au tableau
      digits[index] = customKey;
      index++;
      lcd.setCursor(index - 1, 1);  // Afficher la touche sur l'écran LCD
      lcd.print(customKey);

      // Lorsque 5 caractères sont entrés
      if (index == DLENGTH) {
        digits[DLENGTH] = '\0';  // Ajouter le caractère de fin de chaîne
        lcd.setCursor(0, 1);
        lcd.print(digits);  // Afficher le code saisi
        delay(1000);        // Pause avant la vérification

        attemptCount++;  // Incrémenter le compteur de tentatives

        if (strcmp(digits, code) == 0) {  // Vérification du code
          lcd.clear();
          lcd.print("Code correct");
          correctCodeEntered = true;  // Mettre à jour le flag si le code est correct
          delay(2000);
          resetSystem();  // Réinitialiser le système après un code correct
        } else {
          lcd.clear();
          lcd.print("Code incorrect");
          delay(1000);
          if (attemptCount >= MAX_ATTEMPTS) {
            lockedOut = true;  // Verrouiller le système après trop d'échecs
          }
        }

        index = 0;    // Réinitialise l'index pour une nouvelle entrée
        lcd.clear();  // Effacer l'écran après chaque tentative complète
        lcd.print("Enter code:");
      }
    }
  }
}


// Lecture du code de réinitialisation
void readResetCode() {
  char resetKey = kp4x4.getKey();  // Lire une touche du clavier
  if (resetKey) {
    digits[index] = resetKey;
    index++;
    lcd.setCursor(index - 1, 1);  // Afficher la touche sur l'écran
    lcd.print(resetKey);

    if (index == DLENGTH) {
      digits[DLENGTH] = '\0';  // Fin de chaîne
      lcd.setCursor(0, 1);
      lcd.print(digits);
      delay(1000);

      if (strcmp(digits, resetCode) == 0) {  // Vérification du code admin
        lcd.clear();
        lcd.print("Reset correct");
        delay(2000);
        resetSystem();  // Réinitialiser le système
      } else {
        lcd.clear();
        lcd.print("Reset incorrect");
        delay(1000);
        index = 0;  // Réinitialise l'index pour retenter
      }
    }
  }
}

// ********************
// Fonctions pour la gestion de la connexion MQTT
// ********************

// Connexion au serveur MQTT
void connectToMqtt() {
  while (!mqttClient.connected()) {
    lcd.setCursor(0, 0);
    lcd.print("Connecting MQTT");
    if (mqttClient.connect("arduinoClient", mqttUser, mqttPassword)) {
      lcd.clear();
      lcd.print("MQTT Connected");
    } else {
      delay(5000);  // Attendre 5 secondes avant de réessayer
    }
  }
}

// Envoi d'un message via MQTT
void sendMqttMessage(const char* topic, const char* payload) {
  mqttClient.publish(topic, payload);
}
