/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CHumidMeter.ino>
* @version : <1>
* @brief <contient le code qui instancie CHumidMeter et qui affiche le retour de la fonction GetHumidValue>
* @author <Evann POMMIER> 
* @date < 28/02/2024> 
*/

#include "CHumidMeter.h"

CHumidMeter humidMeter;

void setup() {
  Serial.begin(115200);
  humidMeter.init();
}
void loop() {
  Serial.print("Humid: ");
  Serial.print(humidMeter.GetHumidValue());
  Serial.println(" %");
  delay(1000);
}