/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CHumidMeter.h>
* @version : <1>
* @brief <contient la declaration de la class CHumidMeter.>
* @author <Evann POMMIER> 
* @date < 07/03/2024> 
*/
#pragma once
#include "EVManager.h"
#include "HTS221Sensor.h"

class CHumidMeter : public EVClasse {
public:
  CHumidMeter(){};
  bool Event(EV event, int32_t param);
  void init();
  void GetHumidValue();
private:
  int32_t humidValue;
};