/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CSonorMeter.ino>
* @version : <1>
* @brief <contient le code qui instancie CSonorMeter et qui affiche le retour de la fonction GetSonorValue>
* @author <Evann POMMIER> 
* @date < 05/03/2024> 
*/

#include "CSonorMeter.h"

CSonorMeter sonorMeter;

int32_t inline CalculMoyenneFlottante(int16_t mesure){
    static int32_t mesures[16]={0},total=0;
    static uint8_t i=0;
    total-=mesures[i];          // on retire du total la plus ancienne mesure
    total+=(mesures[i]=mesure); // ajoute la nouvelle mesure au total, et on l'enregistre aussi dans l'anneau 
    i=(i+1)&0x0F;               // on se positionne sur la case suivante. En forçant à zéro les bits de poids fort on reboucle après 15.
    return total>>4;            // la moyenne, c'est le total divisé par 16. décaler de 4 bits vers la droite est équivalent et plus rapide.
}

void setup() {
  Serial.begin(115200);
  sonorMeter.init();
}

void loop() {
  int16_t sonor = sonorMeter.GetSonorValue();

  Serial.println(sonor);
  delay(100);
}