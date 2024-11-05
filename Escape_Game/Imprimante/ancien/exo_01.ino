// ----------------------------------------------
// Inclusion des bibliothèques
// ----------------------------------------------
#include <Ethernet.h>
#include <PubSubClient.h>


// ----------------------------------------------
// Configuration du réseau
// ----------------------------------------------
const byte MAC[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x24, 0x7A };  // Shield #1

IPAddress IP_SHIELD(192, 168, 50, 101);  // IP Shield #1
IPAddress IP_SERVER(192, 168, 50, 100);  // IP Serveur MQTT

const char* MQTT_ARDUINO = "VOTRE_PRENOM";
const char* MQTT_USER = "cesep";
const char* MQTT_PASS = "cesepasbl";

EthernetClient shield;
PubSubClient MQTT(shield);


// ----------------------------------------------
// Bouton et Led
// ----------------------------------------------
#define PIN_BTN 2
#define PIN_LED 3

bool etatBouton = false;
bool etatPrecedent = false;

bool isLed = false;


// ----------------------------------------------
// Déclarations des fonctions
// ----------------------------------------------
void mqttReconnect();
void mqttCallback(char* topic, byte* payload, uint16_t length);
void handleBouton();


// ----------------------------------------------
// Initialisation
// ----------------------------------------------
void setup() {
  Serial.begin(9600);

  Ethernet.begin(MAC, IP_SHIELD);

  MQTT.setServer(IP_SERVER, 1883);
  MQTT.setCallback(mqttCallback);

  pinMode(PIN_BTN, INPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.println("Setup terminé.");
}


// ----------------------------------------------
// Boucle Principale
// ----------------------------------------------
void loop() {
  if (!MQTT.connected()) { mqttReconnect(); }
  MQTT.loop();  // Gestion de MQTT

  handleBouton();
}


// ----------------------------------------------
// Autres fonctions
// ----------------------------------------------
void mqttReconnect() {
  while (!MQTT.connected()) {
    // Tentative de connexion
    if (MQTT.connect(MQTT_ARDUINO, MQTT_USER, MQTT_PASS)) {
      Serial.println("connecté");

      // S'abonne aux topics
      MQTT.subscribe("arduino/leds");

      // Publie un 'tit message
      String timestamp = String(millis());
      MQTT.publish("arduino/connexions/VOTRE_PRENOM", timestamp.c_str());
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

  // Ci-dessous, ce que doit faire le callback
  if (String(topic) == "arduino/leds") {
    if (message == "true") {
      isLed = true;
      digitalWrite(PIN_LED, HIGH);
      Serial.println("Allume la LED");
    } else {
      isLed = false;
      digitalWrite(PIN_LED, LOW);
      Serial.println("Eteind la LED");
    }
  }
}

void handleBouton() {
  if (!isLed) return;

  etatBouton = digitalRead(PIN_BTN);

  if (etatBouton != etatPrecedent) {
    if (etatBouton) {
      MQTT.publish("arduino/boutons/VOTRE_PRENOM", "1");
      Serial.println("Bouton enfoncé");
    } else {
      MQTT.publish("arduino/boutons/VOTRE_PRENOM", "0");
      Serial.println("Bouton relâché");
    }

    etatPrecedent = etatBouton;
    delay(50);
  }
}