
/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CBatterieMeter.h>
* @version : <1>
* @brief <This file contains the declaration of the CBatterieMeter class, which is used for handling battery level measurements.>
*@author<Evann POMMIER>
* @date <19/03/2024>
*/
#pragma once
#define PIN_18650 A3
#include "EVManager.h"
#include "DB.h"

class CBatterieMeter : public EVClasse {
public:
  CBatterieMeter() {}
  void init();
  bool Event(EV event, int32_t param);
  int32_t GetBatterieValuePC();
private:
  uint32_t CalculPourcentage(int32_t T);
  int32_t batterieValuePC;  // charge batterie en % TODO calculé mais ne sert à rien. à passer en public ?
  int32_t batterieSeuil;    // seuil de décharge en mV
  int32_t battPin;
  int32_t mesureCalibree;  // tension batterie en mV
};
