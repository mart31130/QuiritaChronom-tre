#include "CAccelGyro.h"

LSM6DSLSensor AccGyr(&Wire);

CAccelGyro::CAccelGyro(int x){
}

 bool CAccelGyro::Event( EV event, int32_t param ){
  switch(event){   // passer l'évènement à l'état actuel pour traitement
    case EV::ColdReset: 
    initialisation(); 
    break;
    case EV::Systic:
      captureAccelerometre();
      captureGyroscope();
      PushAccelerometre();
      PushGyroscope();
    break;
    }
  return ContinuerEVDistribution;
 }

void CAccelGyro::initialisation(){
  AccGyr.begin();
  AccGyr.Enable_X();
  AccGyr.Enable_G();
}

void CAccelGyro::captureAccelerometre(){
  AccGyr.Get_X_Axes(this->accelValue);
}

void CAccelGyro::captureGyroscope(){
  AccGyr.Get_G_Axes(this->gyroValue);
}

void CAccelGyro::PushAccelerometre(){
  Push(EV::Accelerometre,*(this->accelValue));
}
void CAccelGyro::PushGyroscope(){
  Push(EV::Gyroscope,*(this->gyroValue));
}
