/*!
* Project Name: QUIRITACHRONOMETRE
* @file <CInteraction.h>
* @version : <1>
* @brief <contient la declaration de la class CInteraction.>
* @author <Evann POMMIER> 
* @date < 30/04/2024> 
*/
#pragma once
#include "EVManager.h"

class CInteraction : public EVClasse {
public:
  CInteraction(){};
  bool Event(EV event, int32_t param);
  void init();
  bool GetFlagInteraction();
  void SetFlagInteraction(bool flag);
private:
  bool flagInteraction;
};