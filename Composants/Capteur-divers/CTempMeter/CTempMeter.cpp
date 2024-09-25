/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CTempMeter.cpp>
* @version : <1>
* @brief <contient le code des fonctions de CTempMeter, init() et GetTempValue.>
* @author <Evann POMMIER> 
* @date < 28/02/2024> 
*/

#include <CTempMeter.h>

void CTempMeter::init()
{
  HTS.begin();
  Serial.println(F("HTS Ready"));
}

float CTempMeter::GetTempValue()
{
  return HTS.readTemperature();
}