/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CSonorMeter.cpp>
* @version : <1>
* @brief <contient le code des fonctions de CSonorMeter, init() et GetSonorValue.>
* @author <Evann POMMIER> 
* @date < 05/03/2024> 
*/

#include <CSonorMeter.h>

void CSonorMeter::init()
{
  Serial.println(F("MAX9814 Ready"));
}

int16_t CSonorMeter::GetSonorValue()
{
  return (int16_t)analogRead(PIN);
}