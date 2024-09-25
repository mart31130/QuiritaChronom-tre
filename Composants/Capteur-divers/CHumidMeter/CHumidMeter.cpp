/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CHumidMeter.cpp>
* @version : <1>
* @brief <contient le code des fonctions de CHumidMeter, init() et GetHumidValue.>
* @author <Evann POMMIER> 
* @date < 28/02/2024> 
*/

#include <CHumidMeter.h>

void CHumidMeter::init()
{
  HTS.begin();
  Serial.println(F("HTS Ready"));
}

int32_t CHumidMeter::GetHumidValue()
{
  return (int32_t)HTS.readHumidity();
}