#include <PubSubClient.h>
#include <Ethernet.h>

EthernetClient shield;
PubSubClient MQTT(shield);

const char* TOPIC_VIDEO = "escape_game/video/imprimante";
const char* TOPIC_ETAT = "escape_game/etat";

void setup() {
  // Initialisation Ethernet et MQTT
  Ethernet.begin(MAC, IP_SHIELD);
  MQTT.setServer(IP_SERVER, 1883);
}

void checkCodeInput() {
  if (codeInput == codeValid) {
    // Code correct, envoyer un message MQTT pour lancer la vidéo
    sendStatusMessage(TOPIC_VIDEO, "play");
  }
}

void sendStatusMessage(const char* topic, const char* message) {
  MQTT.publish(topic, message);
}
