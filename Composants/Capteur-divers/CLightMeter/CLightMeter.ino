/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CLightMeter.ino>
* @version : <1>
* @brief <contient le code qui instancie CLightMeter et qui affiche le retour de la fonction GetLightValue>
* @author <Evann POMMIER> 
* @date < 28/02/2024> 
*/

#include "CLightMeter.h"
CLightMeter lightMeter;

void setup() {
  Serial.begin(115200);
  lightMeter.init();
}
void loop() {
  Serial.print("Light: ");
  Serial.print(lightMeter.GetLightValue());
  Serial.println(" lx");
  delay(1000);
}