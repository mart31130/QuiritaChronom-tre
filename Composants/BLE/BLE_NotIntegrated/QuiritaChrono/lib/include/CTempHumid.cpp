#include "CTempHumid.h"

HTS221Sensor HumTemp(&Wire);

CTempHumid::CTempHumid(int x){
}

 bool CTempHumid::Event( EV event, int32_t param ){
  switch(event){   // passer l'évènement à l'état actuel pour traitement
    case EV::ColdReset: 
    initialisation(); 
    break;
    case EV::Systic:
      captureHumidity();
      captureTemperature();
      PushHumidity();
      PushTemperature();
    break;
    }
  return ContinuerEVDistribution;
 }

void CTempHumid::initialisation(){
  HumTemp.begin();
  HumTemp.Enable();
}

void CTempHumid::captureHumidity(){
  HumTemp.GetHumidity(&(this->humidValue));
}

void CTempHumid::captureTemperature(){
  HumTemp.GetTemperature(&(this->humidValue));
}

void CTempHumid::PushHumidity(){
  Push(EV::Humidite,this->humidValue);
}
void CTempHumid::PushTemperature(){
  Push(EV::Temperature,this->tempValue);
}
