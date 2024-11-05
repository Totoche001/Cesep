#include <AccelStepper.h>

#define STEP_PIN 22
#define DIR_PIN 23
#define LIMIT_SWITCH_PIN 8  // Capteur de fin de course pour détecter la position de la figurine
#define ELECTROAIMANT_PIN 11

AccelStepper figurineStepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

bool figurineAtTarget = false;

void setup() {
  Serial.begin(9600);
  
  // Configuration du moteur pas à pas
  figurineStepper.setMaxSpeed(500.0);
  figurineStepper.setAcceleration(100.0);
  
  // Configuration de la broche de l'électroaimant
  pinMode(ELECTROAIMANT_PIN, OUTPUT);
  
  // Configuration du capteur de fin de course
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
  
  // Déplacer la figurine vers la position cible
  figurineStepper.moveTo(1000);  // Par exemple, avancer de 1000 pas
}

void loop() {
  // Déplacer la figurine vers la position définie
  if (!figurineAtTarget) {
    figurineStepper.run();
    
    // Vérifier si la figurine a atteint la position cible
    if (digitalRead(LIMIT_SWITCH_PIN) == LOW) {
      figurineAtTarget = true;
      Serial.println("Figurine a atteint la position cible!");
      
      // Déclencher la séquence (son ou vidéo)
      playSequence();
      
      // Ouvrir la porte de sortie
      openExitDoor();
    }
  }
}

// Fonction pour déclencher la séquence (son ou vidéo)
void playSequence() {
  Serial.println("Lecture de la séquence vidéo/son...");
  // Ajouter ici le code pour déclencher une vidéo ou un son
  delay(5000);  // Simulation de la durée de la séquence (5 secondes)
}

// Fonction pour ouvrir la porte de sortie
void openExitDoor() {
  Serial.println("Ouverture de la porte de sortie...");
  digitalWrite(ELECTROAIMANT_PIN, HIGH);  // Activer l'électroaimant pour déverrouiller la porte
  delay(5000);  // Maintenir l'électroaimant activé pendant 5 secondes
  digitalWrite(ELECTROAIMANT_PIN, LOW);  // Désactiver l'électroaimant
}
