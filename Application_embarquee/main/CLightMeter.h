/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CLightMeter.h>
* @version : <2>
* @brief <This file contains the declaration of the CLightMeter class, which is used for handling light sensor operations.>
* @author <Evann POMMIER>
* @date <07/03/2024>
*/
#pragma once
#include "EVManager.h"
#include <BH1750.h>

class CLightMeter : public EVClasse {
public:
  CLightMeter(){};
  bool Event(EV event, int32_t param);
  void init();
private:
  int32_t lightValue;
};