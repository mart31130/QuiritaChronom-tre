/**
 * Exemple de code Arduino pour un datalogger basique avec stockage sur carte SD.
 */

/* Dépendances */
#include <SPI.h> // Pour la communication SPI
#include <SD.h>  // Pour la communication avec la carte SD


/** Broche CS de la carte SD */
const byte SDCARD_CS_PIN = 10; // A remplacer suivant votre shield SD

/** Nom du fichier de sortie */
const char* OUTPUT_FILENAME = "demo.csv";

/** Delai entre deux prise de mesures */
const unsigned long DELAY_BETWEEN_MEASURES = 5000;


/** Fichier de sortie avec les mesures */
File file;


/** Fonction setup() */
void setup() {

  /* Initialisation du port série (debug) */
  Serial.begin(115200);

  /* Initialisation du port SPI */
  pinMode(10, OUTPUT); // Arduino UNO
  //pinMode(53, OUTPUT); // Arduino Mega

  /* Initialisation de la carte SD */
  Serial.println(F("Initialisation de la carte SD ... "));
  if (!SD.begin(SDCARD_CS_PIN)) {
    Serial.println(F("Erreur : Impossible d'initialiser la carte SD"));
    Serial.println(F("Verifiez la carte SD et appuyez sur le bouton RESET"));
    for (;;); // Attend appui sur bouton RESET
  }

  /* Ouvre le fichier de sortie en écriture */
  Serial.println(F("Ouverture du fichier de sortie ... "));
  file = SD.open(OUTPUT_FILENAME, FILE_WRITE);
  if (!file) {
    Serial.println(F("Erreur : Impossible d'ouvrir le fichier de sortie"));
    Serial.println(F("Verifiez la carte SD et appuyez sur le bouton RESET"));
    for (;;); // Attend appui sur bouton RESET
  }
 
  /* Ajoute l'entête CSV si le fichier est vide */
  if (file.size() == 0) {
    Serial.println(F("Ecriture de l'entete CSV ..."));
    file.println(F("TempHumid; Mems; Lux; Micro; Battery"));
    file.flush();
  }
}


/** Fonction loop() */
void loop() {
  // Temps de la précédente mesure et actuel
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  /* Réalise une prise de mesure toutes les DELAY_BETWEEN_MEASURES millisecondes */
  if (currentMillis - previousMillis >= DELAY_BETWEEN_MEASURES) {
    previousMillis = currentMillis;
    measure();
  }
}


/** Fonction de mesure - à personnaliser selon ses besoins */
void measure() {

  /* Réalise la mesure */
  float v_1 = analogRead(A0) * 5.0 / 1023; 
  float v_2 = analogRead(A1) * 5.0 / 1023;
  float v_3 = analogRead(A2) * 5.0 / 1023;
  float v_4 = analogRead(A3) * 5.0 / 1023;
  float v_5 = 7;
  
  /* Affiche les données sur le port série pour debug */ 
  Serial.print(v_1);
  Serial.print(F("; "));
  Serial.print(v_2);
  Serial.print(F("; "));
  Serial.print(v_3);
  Serial.print(F("; "));
  Serial.print(v_4);
  Serial.print(F("; "));
  Serial.println(v_5);
  
  /* Enregistre les données sur la carte SD */
  file.print(v_1);
  file.print(F("; "));
  file.print(v_2);
  file.print(F("; "));
  file.print(v_3);
  file.print(F("; "));
  file.print(v_4);
  file.print(F("; "));
  file.println(v_5);
  file.flush();
}
