/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CAcceMeter.h>
* @version : <1>
* @brief <This file contains the declaration of the CAcceMeter class, which is used for handling accelerometer operations.>
* @author <Evann POMMIER>
* @date <20/03/2024>
*/
#pragma once
#include <LSM6DSLSensor.h>
#include "EVManager.h"
#include "DB.h"

class CAcceMeter : public EVClasse {
public:
  CAcceMeter(){};
  bool Event(EV event, int32_t param);
  void init();
  void CheckSeuil(int32_t value[3], int32_t seuil);
private:
  int32_t acceValue[3];
  int32_t seuilAcce = 200;
  uint32_t module_Acce;
  bool InitNOK = true;
};