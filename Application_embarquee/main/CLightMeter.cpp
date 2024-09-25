/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CLightMeter.cpp>
* @version : <2>
* @brief <This file contains the code of the functions of CLightMeter, which is used for handling light sensor operations.>
* @author <Evann POMMIER>
* @date <07/03/2024>
*/
#include "CLightMeter.h"

BH1750 lightMeter;

/*!
* @fn <init()>
* @brief <This function initializes the light sensor.>
* @return <void>
*/
void CLightMeter::init() {
  Wire.begin();
  lightMeter.begin();
}

/*!
* @fn <Event(EV event, int32_t param)>
* @brief <This function handles events related to the light sensor.>
* @param [in]  <event> <The event to be handled.>
* @param [in]  <param> <The parameter associated with the event.>
* @return <bool>
*/
bool CLightMeter::Event(EV event, int32_t param) {
  switch (event) {
    case EV::ColdReset:
      this->init();
      break;
    case EV::Systic_10MIN:
      Push(EV::Lux,(int32_t)lightMeter.readLightLevel());
      break;
    default: break;
  }
  return ContinuerEVDistribution;
}