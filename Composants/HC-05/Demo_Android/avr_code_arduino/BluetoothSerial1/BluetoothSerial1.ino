#include <HardwareSerial.h>

#define HC05 Serial1
#define cout Serial2
String msg; // Variable pour stocker les messages

//PA10,PA9
HardwareSerial Serial1(PA10,PA9); 


void setup() {
  // Initialisation de la communication série avec le moniteur série et le module HC-05
  cout.begin(230400);
  HC05.begin(230400);
}

void loop() {
  
  readcoutPort(); // lire les données depuis le moniteur série
  
  // Si un message est disponible, l'envoyer via le module HC-05
  if (msg != "") HC05.println(msg);
  
  // Lire les données reçues depuis le module HC-05 et les transmettre au moniteur série
  if (HC05.available() > 0) {
    cout.write(HC05.read());
  }
}

void readcoutPort() {
  msg = ""; // Réinitialisation du message
  // Lire les données du moniteur série
  while (cout.available()) {
      char c = cout.read();  // Lire un octet depuis le tampon série
      msg += c; // Ajouter l'octet à la chaîne msg
  }
}
