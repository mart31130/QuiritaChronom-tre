#pragma once
#include "arduino.h"
#include "EVManager.h"
#include "HTS221Sensor.h"
class CTempHumid : public EVClasse {
  public :
  CTempHumid(int x);
  bool Event( EV event, int32_t param );
  void initialisation();
  void captureHumidity();
  void captureTemperature();
  void PushHumidity();
  void PushTemperature();
  private :
  uint16_t humidValue, tempValue;
};
