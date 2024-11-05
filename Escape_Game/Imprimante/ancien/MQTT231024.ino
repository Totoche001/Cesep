/*
--------------------------------
Bibliothèque
----------------------------------
*/
#include <PubSubClient.h>
#include <Ethernet.h>

/*
--------------------------------
declaration des fonctions
----------------------------------
*/
const byte MAC[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x24, 0x8B };

IPAddress IP_SHIELD(192, 168, 50, 102);  // IP shield #3
IPAddress IP_SERVER(192, 168, 50, 100);  //IP serveur MQTT
const char* MQTT_ARDUINO = "Anthony";
const char* MQTT_USER = "cesep";
const char* MQTT_PASS = "cesepasbl";

EthernetClient shield;
PubSubClient MQTT(shield);

/*
--------------------------------
boutons et led
----------------------------------
*/

#define PIN_BTN 2
#define PIN_LED 3


bool isled = false;
bool etatBouton = false;
bool etatPrecedent = false;

/*
--------------------------------
declaration des fonctions
----------------------------------
*/
void mqttReconnect();
void mqttCallback(char* topic, byte* payload, uint16_t length);
void handlebouton();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Ethernet.begin(MAC, IP_SHIELD);

  MQTT.setServer(IP_SERVER, 1883);
  MQTT.setCallback(mqttCallback);

  pinMode(PIN_BTN, INPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.println("Setup Terminé");
}
/*
--------------------------------
Boucle principale 
----------------------------------
*/
void loop() {
  if (!MQTT.connected()) { mqttReconnect(); }
  MQTT.loop();


  etatBouton = digitalRead(PIN_BTN);

  handlebouton();
}

void mqttReconnect() {
  while (!MQTT.connected()) {
    if (MQTT.connect(MQTT_ARDUINO, MQTT_USER, MQTT_PASS)) {
      Serial.println("Connecté");

      MQTT.subscribe("arduino/leds");
      //MQTT.subscribe("arduino/intro/video");
      String timestamp = String(millis());
      MQTT.publish("arduino/connexions/Anthony", timestamp.c_str());

      //MQTT.publish("connexions/Anthony", "être ou ne pas être, ne pas être");
    } else {
      Serial.print("échoué. rc = ");
      Serial.print(MQTT.state());
      Serial.println("MQTT: Nouvel essai dans 3 seconde");
      delay(3000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, uint16_t lenght) {
  String message;


  for (int i = 0; i < lenght; i++) {
    message += (char)payload[i];
  }
  Serial.print("MQTT: message reçu: (");
  Serial.print(topic);
  Serial.print(")");
  Serial.println(message);
  /*if (String(topic) == "arduino/connexions") {
    Serial.println("Ca marche");
  }*/
  if (String(topic) == "arduino/leds") {
    if (message == "true") {
      Serial.println("Allumer Led");
      digitalWrite(PIN_LED, HIGH);
    } else {
      Serial.println("Eteindre Led");
      digitalWrite(PIN_LED, LOW);
    }
  }
  /*if (message.equalsIgnoreCase("Allumé")) {
    digitalWrite(10, HIGH);
    ledPin = true;
    Serial.println("LED ON for MQTT");

  } else if (message.equalsIgnoreCase("eteint")) {
    digitalWrite(10, LOW);
    ledPin = false;
    Serial.println("LED off for MQTT");
  }*/
}
void handlebouton(){
  if(!isled) return
;  if (etatBouton != etatPrecedent) {
    if (etatBouton) {
      
      MQTT.publish("arduino/boutons/Anthony", "1");
      Serial.println("Bouton enfoncé");
    } else {
      MQTT.publish("arduino/boutons/Anthony", "0");
      Serial.println("Bouton relaché");
    }

    etatPrecedent = etatBouton;
    delay(50);
  }
}
