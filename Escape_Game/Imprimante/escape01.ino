#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <PubSubClient.h>
#include <Ethernet.h>

// ----------------------------------------------
// Configuration Address IP
// ----------------------------------------------
const byte MAC[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x24, 0x8B };
IPAddress IP_SHIELD(192, 168, 50, 102);
IPAddress IP_SERVER(192, 168, 50, 100);

EthernetClient shield;
PubSubClient MQTT(shield);

// ----------------------------------------------
// Configuration des Servos et du LCD
// ----------------------------------------------
#define HORLOGE_SERVO 8
#define CADRE_SERVO 9
#define SERVO_BOITE 30
Servo horlogeServo;
Servo cadreServo;
Servo boiteServo;

#define LCD_EN 51
#define LCD_RS 50
#define LCD_D4 49
#define LCD_D5 48
#define LCD_D6 47
#define LCD_D7 46
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// ----------------------------------------------
// Configuration du Keypad
// ----------------------------------------------
#define KEY_ROWS 4
#define KEY_COLS 4
const byte PIN_ROWS[KEY_ROWS] = { A0, A1, A2, A3 };
const byte PIN_COLS[KEY_COLS] = { A4, A5, A6, A7 };
const char TOUCHES[KEY_ROWS][KEY_COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
Keypad clavier = Keypad(makeKeymap(TOUCHES), PIN_ROWS, PIN_COLS, KEY_ROWS, KEY_COLS);

// ----------------------------------------------
// Variables pour la gestion du code
// ----------------------------------------------
String codeInput = "";
String codeValid = "1234";
String resetCode = "5555*";
int attemptCount = 0;
const int MAX_ATTEMPTS = 3;
bool lockedOut = false;
bool correctCodeEntered = false;
unsigned long lcdTimer = 0;
#define LCD_PAUSE 2000

// ----------------------------------------------
// Déclarations des flags et des timers
// ----------------------------------------------
#define HORLOGE_PAUSE 60000
#define LIVRES_PAUSE 2000
unsigned long horlogeTimer = 0;
unsigned long livresTimer = 0;

#define LIVRE_1 2
#define LIVRE_2 3
#define LIVRE_3 4
#define LIVRE_4 5
#define LIVRE_5 6
#define LIVRE_6 7

bool isVideoIntro = false;
bool isHorloge = false;
bool isCadre = false;
bool isLivres = false;
bool isImprimante = true;

// ----------------------------------------------
// Déclarations des fonctions
// ----------------------------------------------
void handleHorloge();
void handleCadre();
void handleLivres();
void handleImprimante();
void setLCD(String ligne1, String ligne2);
void mqttReconnect();
void mqttCallback(char* topic, byte* payload, uint16_t length);
void sendStatusMessage(const char* message);

// ==============================================
// Initialisation
// ==============================================
void setup() {
  Serial.begin(9600);
  Serial.println();

  // Initialisation Ethernet et MQTT
  Ethernet.begin(MAC, IP_SHIELD);
  MQTT.setServer(IP_SERVER, 1883);
  MQTT.setCallback(mqttCallback);

  // Initialisation des servos
  horlogeServo.attach(HORLOGE_SERVO);
  horlogeServo.write(0);

  cadreServo.attach(CADRE_SERVO);
  cadreServo.write(90); // Fermer le cadre par défaut

  boiteServo.attach(SERVO_BOITE);
  boiteServo.write(90); // Fermer la boîte par défaut

  // Initialisation des livres
  pinMode(LIVRE_1, INPUT_PULLUP);
  pinMode(LIVRE_2, INPUT_PULLUP);
  pinMode(LIVRE_3, INPUT_PULLUP);
  pinMode(LIVRE_4, INPUT_PULLUP);
  pinMode(LIVRE_5, INPUT_PULLUP);
  pinMode(LIVRE_6, INPUT_PULLUP);

  // Initialisation du LCD
  lcd.begin(16, 2);
  lcd.cursor();
  setLCD("Enter code:", "");
}

// ==============================================
// Boucle principale
// ==============================================
void loop() {
  // Gestion MQTT (boucle)
  if (!MQTT.connected()) { mqttReconnect(); }
  MQTT.loop();

  // Gestion des fonctionnalités
  handleHorloge();
  handleCadre();
  handleLivres();

  // Vérification du code
  if (!lockedOut && !correctCodeEntered && (lcdTimer + LCD_PAUSE <= millis())) {
    char touche = clavier.getKey();

    if (touche) {
      // Si la touche est un chiffre
      if (touche >= '0' && touche <= '9') {
        codeInput += touche;
      }
      // Si la touche est '*'
      else if (touche == '*') {
        codeInput = "";
      }

      setLCD("Enter code:", codeInput);

      if (codeInput.length() >= codeValid.length()) {
        if (codeInput == codeValid) {
          setLCD("Code Valide!", "");
          lcdTimer = millis();
          correctCodeEntered = true;
          handleImprimante();
          sendStatusMessage("Code valide, imprimante activée.");
        } else {
          setLCD("Code Incorrect!", "");
          lcdTimer = millis();
          attemptCount++;
          if (attemptCount >= MAX_ATTEMPTS) {
            lockedOut = true;
            setLCD("Locked Out", "Reset needed");
            sendStatusMessage("Système verrouillé après trop de tentatives.");
          }
        }
        codeInput = "";
      }
    }
  }

  // Vérification du code de réinitialisation
  if (lockedOut) {
    char resetKey = clavier.getKey();
    if (resetKey) {
      codeInput += resetKey;
      setLCD("Reset code:", codeInput);

      if (codeInput.length() >= resetCode.length()) {
        if (codeInput == resetCode) {
          setLCD("Reset OK", "");
          delay(2000);
          lockedOut = false;
          attemptCount = 0;
          correctCodeEntered = false;
          setLCD("Enter code:", "");
          sendStatusMessage("Système réinitialisé avec succès.");
        } else {
          setLCD("Reset Failed", "");
          delay(2000);
          sendStatusMessage("Code de réinitialisation incorrect.");
        }
        codeInput = "";
      }
    }
  }
}

// ----------------------------------------------
// Gestion de l'horloge
// ----------------------------------------------
void handleHorloge() {
  if (isHorloge) {
    if (horlogeTimer + HORLOGE_PAUSE <= millis()) {
      horlogeTimer = millis();
      byte horlogePosition = horlogeServo.read();
      horlogeServo.write(horlogePosition + 6);
      sendStatusMessage("Horloge avancée de 6 degrés.");
    }
  }
}

// Gestion du cadre
void handleCadre() {
  if (isVideoIntro) {
    if (isCadre) {
      cadreServo.write(0);  // Ouvre le cadre
      sendStatusMessage("Cadre ouvert.");
    } else {
      cadreServo.write(90);  // Ferme le cadre
      sendStatusMessage("Cadre fermé.");
    }
  }
}

// Gestion des livres audio
void handleLivres() {
  if (isLivres) {
    if (livresTimer + LIVRES_PAUSE <= millis()) {
      bool stateLivre1 = !digitalRead(LIVRE_1);
      bool stateLivre2 = !digitalRead(LIVRE_2);
      bool stateLivre3 = !digitalRead(LIVRE_3);
      bool stateLivre4 = !digitalRead(LIVRE_4);
      bool stateLivre5 = !digitalRead(LIVRE_5);
      bool stateLivre6 = !digitalRead(LIVRE_6);

      if (stateLivre1 || stateLivre2 || stateLivre3 || stateLivre4 || stateLivre5 || stateLivre6) {
        livresTimer = millis();
        // Actions à réaliser selon le livre appuyé
        if (stateLivre1) {
          sendStatusMessage("Livre 1 activé.");
        } else if (stateLivre2) {
          sendStatusMessage("Livre 2 activé.");
        } else if (stateLivre3) {
          sendStatusMessage("Livre 3 activé.");
        } else if (stateLivre4) {
          sendStatusMessage("Livre 4 activé.");
        } else if (stateLivre5) {
          sendStatusMessage("Livre 5 activé.");
        } else if (stateLivre6) {
          sendStatusMessage("Livre 6 activé.");
        }
      }
    }
  }
}

// ----------------------------------------------
// Gestion de l'imprimante
// ----------------------------------------------
void handleImprimante() {
  if (isImprimante) {
    setLCD("Printing:", "chess_queen.3d");
    delay(5000);  // Simulation de l'impression pendant 5 secondes
    setLCD("Printing Done", "love <3");
    boiteServo.write(0);  // Ouvre la boîte pour récupérer la pièce
    sendStatusMessage("Impression terminée, boîte ouverte.");
  }
}

// ----------------------------------------------
// Gestion de l'écran LCD
// ----------------------------------------------
void setLCD(String ligne1, String ligne2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ligne1);
  lcd.setCursor(0, 1);
  lcd.print(ligne2);
}

// ----------------------------------------------
// Connexion MQTT
// ----------------------------------------------
void mqttReconnect() {
  while (!MQTT.connected()) {
    if (MQTT.connect("Shield02", "cesep", "cesepasbl")) {
      Serial.println("Connecté");
      MQTT.subscribe("arduino/connexions");
      MQTT.publish("arduino/connexions/test", "Anthony");
      sendStatusMessage("Connexion MQTT établie.");
    } else {
      Serial.print("échoué. rc = ");
      Serial.print(MQTT.state());
      Serial.println("MQTT: Nouvel essai dans 3 secondes");
      delay(3000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, uint16_t length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("MQTT: message reçu: (");
  Serial.print(topic);
  Serial.print(") ");
  Serial.println(message);
}

// ----------------------------------------------
// Envoi d'un message de statut au serveur MQTT
// ----------------------------------------------
void sendStatusMessage(const char* message) {
  MQTT.publish("test/anthony2", message);
}