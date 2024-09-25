/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CTempMeter.ino>
* @version : <1>
* @brief <contient le code qui instancie CTempMeter et qui affiche le retour de la fonction GetTempValue>
* @author <Evann POMMIER> 
* @date < 28/02/2024> 
*/

#include "CTempMeter.h"

CTempMeter tempMeter;

void setup() {
  Serial.begin(115200);
  tempMeter.init();
}

void loop() {
  float temp = tempMeter.GetTempValue();

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" °C");
  delay(1000);
}