/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CTempMeter.cpp>
* @version : <1>
* @brief <This file contains the code of the functions of CTempMeter, which is used for handling temperature sensor operations.>
* @author <Evann POMMIER>
* @date <07/03/2024>
*/
#include <CTempMeter.h>

HTS221Sensor tempMeter(&Wire);

/*!
* @fn <init()>
* @brief <This function initializes the temperature sensor.>
* @return <void>
*/
void CTempMeter::init() {
  tempMeter.begin();
  tempMeter.Enable();
}
/*!
* @fn <Event(EV event, int32_t param)>
* @brief <This function handles events related to the temperature sensor.>
* @param [in]  <event> <The event to be handled.>
* @param [in]  <param> <The parameter associated with the event.>
* @return <bool>
*/
bool CTempMeter::Event(EV event, int32_t param) {
  switch (event) {
    case EV::ColdReset:
      this->init();
      break;
    case EV::Systic_10MIN:
      this->GetTempValue();
      Push(EV::Temperature, this->tempValue);
      break;
    default: break;
  }
  return ContinuerEVDistribution;
}
/*!
* @fn <GetTempValue()>
* @brief <This function retrieves the temperature value from the sensor.>
* @return <void>
*/
void CTempMeter::GetTempValue() {
  float temperature;
  tempMeter.GetTemperature(&(temperature));
  this->tempValue = (int32_t)(temperature * 10);
}