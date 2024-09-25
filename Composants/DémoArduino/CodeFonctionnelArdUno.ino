#include <SoftwareSerial.h>

// Définition des broches pour la communication série avec le module Bluetooth HC-05
#define rxPin 2
#define txPin 3
#define baudrate 9600

String msg; // Variable pour stocker les messages

SoftwareSerial hc05(rxPin, txPin); // Création d'un objet SoftwareSerial pour la communication avec le HC-05

void setup() {
  // Configuration des broches
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  // Initialisation de la communication série avec le moniteur série et le module HC-05
  Serial.begin(9600);
  Serial.println("ENTREZ les commandes AT :");
  hc05.begin(baudrate);
}

void loop() {
  // Fonction pour lire les données depuis le moniteur série
  readSerialPort();
  
  // Si un message est disponible, l'envoyer via le module HC-05
  if (msg != "") hc05.println(msg);
  
  // Lire les données reçues depuis le module HC-05 et les transmettre au moniteur série
  if (hc05.available() > 0) {
    Serial.write(hc05.read());
  }
}

void readSerialPort() {
  msg = ""; // Réinitialisation du message
  
  // Lire les données du moniteur série
  while (Serial.available()) {
    delay(10);  // Attendre un peu pour permettre la réception complète des données
    if (Serial.available() > 0) {
      char c = Serial.read();  // Lire un octet depuis le tampon série
      msg += c; // Ajouter l'octet à la chaîne msg
    }
  }
}
