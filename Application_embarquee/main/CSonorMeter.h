/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CSonorMeter.h>
* @version : <2>
* @brief <This file contains the declaration of the CSonorMeter class, which is used for handling sound sensor operations.>
* @author <Evann POMMIER>
* @date <07/03/2024>
*/
#pragma once
#define PIN_MIKE A2
#include "EVManager.h"
#include "db.h"

class CSonorMeter : public EVClasse {
public:
  CSonorMeter(){};
  bool Event(EV event, int32_t param);
  void init();
  void CheckSeuil();
private:
  int32_t sonorValue;
  int32_t sonorSeuil;
};