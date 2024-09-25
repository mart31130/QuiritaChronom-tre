#include "CLux.h"

BH1750 lightMeter;

CLux::CLux(int x){
}

 bool CLux::Event( EV event, int32_t param ){
  switch(event){   // passer l'évènement à l'état actuel pour traitement
    case EV::ColdReset: 
    initialisation(); 
    break;
    
    case EV::Systic:
      //captureLux();
      PushLux();
    break;
    }
  return ContinuerEVDistribution;
 }

void CLux::initialisation(){
  lightMeter.begin();
}

/*void CLux::captureLux(){
  lightMeter.readLightLevel(&(this->luxValue));
}*/

void CLux::PushLux(){
  Push(EV::Lux,this->luxValue);
}
