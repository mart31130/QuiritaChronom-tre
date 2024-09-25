#pragma once
#include "arduino.h"
#include "EVManager.h"
#include <BH1750.h>

class CLux : public EVClasse {
  public :
  CLux(int x);
  bool Event( EV event, int32_t param );
  void initialisation();
  void captureLux();
  void PushLux();
  private :
  int32_t luxValue; //POUR LE MOMENT C'EST UN FLOAT !!! A CHANGER DANS LA LIB
};
