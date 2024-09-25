/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CHumidMeter.cpp>
* @version : <1>
* @brief <This file contains the code of the functions of CHumidMeter, which is used for handling humidity sensor operations.>
* @author <Evann POMMIER>
* @date <07/03/2024>
*/
#include "CHumidMeter.h"

HTS221Sensor humidMeter(&Wire);

/*!
* @fn <init()>
* @brief <This function initializes the humidity sensor.>
* @return <void>
*/
void CHumidMeter::init() {
  humidMeter.begin();
  humidMeter.Enable();
}
/*!
* @fn <Event(EV event, int32_t param)>
* @brief <This function handles events related to the humidity sensor.>
* @param [in]  <event> <The event to be handled.>
* @param [in]  <param> <The parameter associated with the event.>
* @return <bool>
*/
bool CHumidMeter::Event(EV event, int32_t param) {
  switch (event) {  // Pass the event to the current state for processing
    case EV::ColdReset:
      this->init();
      break;
    case EV::Systic_10MIN:
      this->GetHumidValue();
      Push(EV::Humidite, this->humidValue);
      break;
    default: break;
  }
  return ContinuerEVDistribution;
}
/*!
* @fn <GetHumidValue()>
* @brief <This function retrieves the humidity value from the sensor.>
* @return <void>
*/
void CHumidMeter::GetHumidValue() {
  float humidite;
  humidMeter.GetHumidity(&(humidite));
  this->humidValue = (int32_t)(humidite * 10);
}