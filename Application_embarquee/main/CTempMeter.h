/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CTempMeter.h>
* @version : <2>
* @brief <This file contains the declaration of the CTempMeter class, which is used for handling temperature sensor operations.>
* @author <Evann POMMIER>
* @date <07/03/2024>
*/
#pragma once
#include "EVManager.h"
#include "HTS221Sensor.h"

class CTempMeter : public EVClasse {
public:
  CTempMeter(){};
  bool Event(EV event, int32_t param);
  void init();
  void GetTempValue();
private:
  int32_t tempValue;
};