/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CLightMeter.cpp>
* @version : <1>
* @brief <contient le code des fonctions de CLightMeter, init() et GetLightValue.>
* @author <Evann POMMIER> 
* @date < 28/02/2024> 
*/

#include <CLightMeter.h>

void CLightMeter::init() {
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Ready"));
}

int32_t CLightMeter::GetLightValue() {
  return (int32_t)lightMeter.readLightLevel();
}