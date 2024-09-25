/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <>
* @version : <1>
* @brief <.>
* @author <Samy Rafa> 
* @date < 25/04/2024> 
*/





#include <SoftwareSerial.h>
#include <ArduinoJson.h> 

SoftwareSerial hc05(2, 3);

int seuilBruit;
int seuilAgitation;

void setup() {
    Serial.begin(9600);
    hc05.begin(9600);
}

void loop() {
    // Lecture des valeurs des capteurs (simulée pour cet exemple)
    int valeurCapteur1 = lireValeurCapteur1();
    int valeurCapteur2 = lireValeurCapteur2();

    
    StaticJsonDocument<64> doc;
    doc["valeurCapteur1"] = valeurCapteur1;
    doc["valeurCapteur2"] = valeurCapteur2;

    // Conversion de l'objet JSON en chaîne JSON.
    char json[64];
    serializeJson(doc, json);

    // Envoi des valeurs des capteurs via Bluetooth
    hc05.println(json);

    // Lecture des valeurs seuils depuis le moniteur série (via le moniteur série pour cet exemple)
    if (Serial.available() > 0) {
        String seuils = Serial.readStringUntil('\n');
        StaticJsonDocument<64> seuilsDoc;
        deserializeJson(seuilsDoc, seuils);
        seuilBruit = seuilsDoc["seuilBruit"];
        seuilAgitation = seuilsDoc["seuilAgitation"];
    }

    delay(1000); 
}

int lireValeurCapteur1() {
    
    return random(20); // Exemple : valeur aléatoire entre 0 et 20
}

int lireValeurCapteur2() {
    
    return random(30); // Exemple : valeur aléatoire entre 0 et 30
}
