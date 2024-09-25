/*!
* @file <CBluetooth.h>
* @version : <1>
* @brief <This file contains the declaration of the CBluetooth class, which is used for handling Bluetooth operations.>
* @author <Samy RAFA, Evann POMMIER>
* @date <25/04/2024>
*/
#pragma once
#include "EVManager.h"

class CBluetooth : public EVClasse {
public:
  CBluetooth() {}
  bool Event(EV event, int32_t param);
  void init();
  int32_t battLvlValue;
  int32_t sdLvlValue;
  int32_t seuilAcce;
  int32_t seuilMicro;
};
