// ----------------------------------------------
// Inclusion des bibliothèques
// ----------------------------------------------

#include <Ethernet.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>


// ----------------------------------------------
// Configuration du réseau
// ----------------------------------------------

// const byte MAC[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x24, 0x7A };  // Shield #1
// const byte MAC[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x24, 0x8B };  // Shield #2
const byte MAC[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x37, 0xBF };  // Shield #3

// IPAddress IP_SHIELD(192, 168, 50, 101);  // IP Shield #1
// IPAddress IP_SHIELD(192, 168, 50, 102);  // IP Shield #2
IPAddress IP_SHIELD(192, 168, 50, 103);  // IP Shield #3
IPAddress IP_SERVER(192, 168, 50, 100);  // IP Serveur MQTT

EthernetClient shield;
PubSubClient MQTT(shield);


// ----------------------------------------------
// Configuration de l'horloge
// ----------------------------------------------

#define HORLOGE_SERVO 8
#define HORLOGE_PAUSE 60000

unsigned long horlogeTimer = 0;
byte horlogeStart = 0;

Servo horlogeServo;


// ----------------------------------------------
// Configuration du servo bougeant le cadre
// ----------------------------------------------

#define CADRE_SERVO 9

Servo cadreServo;

byte cadreOpen = 0;
byte cadreClose = 90;


// ----------------------------------------------
// Configuration des lives audio
// ----------------------------------------------

#define LIVRE_1 2
#define LIVRE_2 3
#define LIVRE_3 4
#define LIVRE_4 5
#define LIVRE_5 6
#define LIVRE_6 7

#define LIVRES_PAUSE 2000

unsigned long livresTimer = 0;


// -------------------------------------
// Imprimante
// -------------------------------------

// Ecran LCD 2x16
#define LCD_EN 51
#define LCD_RS 50
#define LCD_D4 49
#define LCD_D5 48
#define LCD_D6 47
#define LCD_D7 46

#define LCD_PAUSE 2000

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

String lcdLigne1 = "";
String lcdLigne2 = "";

unsigned long lcdTimer = 0;

// Clavier 4x4
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

String codeInput = "";
String codeValid = "1234";

bool codeUnlock = false;
bool printEnded = false;


// ----------------------------------------------
// Déclarations des flags
// ----------------------------------------------

bool isVideoIntro = false;
bool isHorloge = false;
bool isCadre = false;
bool isLivres = false;
bool isImprimante = true;  // <------------- à remettre sur "false" après les tests


// ----------------------------------------------
// Déclarations des fonctions
// ----------------------------------------------
void mqttReconnect();
void mqttCallback(char* topic, byte* payload, uint16_t length);

void handleHorloge();
void handleCadre();
void handleLivres();
void handleImprimante();

void setLCD(String ligne1, String ligne2);


// ==============================================
// Initialisation
// ==============================================
void setup() {
  Serial.begin(9600);
  Serial.println();

  Ethernet.begin(MAC, IP_SHIELD);

  MQTT.setServer(IP_SERVER, 1883);
  MQTT.setCallback(mqttCallback);

  horlogeServo.attach(HORLOGE_SERVO);
  horlogeServo.write(horlogeStart);

  cadreServo.attach(CADRE_SERVO);
  cadreServo.write(cadreClose);

  pinMode(LIVRE_1, INPUT_PULLUP);
  pinMode(LIVRE_2, INPUT_PULLUP);
  pinMode(LIVRE_3, INPUT_PULLUP);
  pinMode(LIVRE_4, INPUT_PULLUP);
  pinMode(LIVRE_5, INPUT_PULLUP);
  pinMode(LIVRE_6, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.cursor();

  Serial.println("Setup terminé.");
}


// ==============================================
// Boucle principale
// ==============================================
void loop() {
  if (!MQTT.connected()) { mqttReconnect(); }

  MQTT.loop();  // Gestion de MQTT

  handleHorloge();     // Gestion de l'horloge
  handleCadre();       // Gestion du cadre
  handleLivres();      // Gestion des livres audio
  handleImprimante();  // Gestion de l'imprimante
}


// ----------------------------------------------
// Autres fonctions
// ----------------------------------------------
void mqttReconnect() {
  while (!MQTT.connected()) {
    // Tentative de connexion
    if (MQTT.connect("shield01", "cesep", "cesepasbl")) {
      Serial.println("connecté");

      // S'abonne aux topics
      MQTT.subscribe("arduino/connexions");

      // Publie un 'tit message
      MQTT.publish("arduino/connexions/shield01", "oui");
    }

    else {
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

  Serial.print("MQTT: Message reçu : (");
  Serial.print(topic);
  Serial.print(") ");
  Serial.println(message);

  if (String(topic) == "arduino/connexions") {
    Serial.println("Ca marche !");
  }
}

void handleHorloge() {
  if (!isHorloge) { return; }

  if (horlogeTimer + HORLOGE_PAUSE <= millis()) {
    horlogeTimer = millis();

    byte horlogePosition = horlogeServo.read();
    horlogeServo.write(horlogePosition + 6);
  }
}

// Gestion du cadre
void handleCadre() {
  if (!isVideoIntro) { return; }

  if (isCadre) {
    cadreServo.write(cadreOpen);
  } else {
    cadreServo.write(cadreClose);
  }
}

// Gestion des livres audio
void handleLivres() {
  if (!isLivres) return;

  if (livresTimer + LIVRES_PAUSE <= millis()) {

    bool stateLivre1 = !digitalRead(LIVRE_1);
    bool stateLivre2 = !digitalRead(LIVRE_2);
    bool stateLivre3 = !digitalRead(LIVRE_3);
    bool stateLivre4 = !digitalRead(LIVRE_4);
    bool stateLivre5 = !digitalRead(LIVRE_5);
    bool stateLivre6 = !digitalRead(LIVRE_6);

    if (stateLivre1 || stateLivre2 || stateLivre3 || stateLivre4 || stateLivre5 || stateLivre6) {
      livresTimer = millis();

      if (stateLivre1) {
        // jouer le son du livre 1
      } else if (stateLivre2) {
        // jouer le son du livre 2
      } else if (stateLivre3) {
        // jouer le son du livre 3
      } else if (stateLivre4) {
        // jouer le son du livre 4
      } else if (stateLivre5) {
        // jouer le son du livre 5
      } else if (stateLivre6) {
        // jouer le son du livre 6
      }
    }
  }
}


// Gestion de l'imprimante
void handleImprimante() {
  if (!isImprimante) { return; }

  if (printEnded) {
    setLCD("Printing Done", "love <3");
    // Ouvre la trappe pour récupérer la pièce
    return;
  }

  // Imprimante dévérouillée
  if (codeUnlock && (lcdTimer + LCD_PAUSE <= millis())) {
    setLCD("Printing:", "chess_queen.3d");
    // Lance la vidéo d'impression
    return;
  }

  // Imprimante vérouillée
  if (lcdTimer + LCD_PAUSE <= millis()) {
    setLCD("Taper le code:", codeInput);

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

      if (codeInput.length() >= codeValid.length()) {
        if (codeInput == codeValid) {
          setLCD("Code Valide !", "");
          lcdTimer = millis();
          codeUnlock = true;
        }

        else {
          setLCD("Code Incorrect !", "");
          lcdTimer = millis();
          // Indique le nombre d'erreurs
        }

        codeInput = "";
      }
    }
  }
}

// Gestion de l'écran LCD de l'imprimante
void setLCD(String ligne1, String ligne2) {
  if (ligne1 != lcdLigne1 || ligne2 != lcdLigne2) {
    lcdLigne1 = ligne1;
    lcdLigne2 = ligne2;
    lcd.clear();
    lcd.print(ligne1);
    lcd.setCursor(0, 1);
    lcd.print(ligne2);
  }
}
