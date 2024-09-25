#pragma once
#include "arduino.h"
#include "EVManager.h"
#include <LSM6DSLSensor.h>
class CAccelGyro : public EVClasse {
  public :
  CAccelGyro(int x);
  bool Event( EV event, int32_t param );
  void initialisation();
  void captureAccelerometre();
  void captureGyroscope();
  void PushAccelerometre();
  void PushGyroscope();
  private :
  int32_t accelValue[3], gyroValue[3];
};
