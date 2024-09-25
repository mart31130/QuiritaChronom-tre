/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CSonorMeter.cpp>
* @version : <2>
* @brief <This file contains the code of the functions of CSonorMeter, which is used for handling sound sensor operations.>
* @author <Evann POMMIER>
* @date <07/03/2024>
*/
#include <CSonorMeter.h>

/*!
* @fn <init()>
* @brief <This function initializes the sound sensor.>
* @return <void>
*/
void CSonorMeter::init() {
  dbreadint(sonorSeuil, 0x200);  //valeur seuil par défaut 200, paramétréedans DB.
}
/*!
* @fn <Event(EV event, int32_t param)>
* @brief <This function handles events related to the sound sensor.>
* @param [in]  <event> <The event to be handled.>
* @param [in]  <param> <The parameter associated with the event.>
* @return <bool>
*/
bool CSonorMeter::Event(EV event, int32_t param) {
  switch (event) {  // Pass the event to the current state for processing
    case EV::ColdReset:
      this->init();
      break;
    case EV::Systic_100MS:
      this->sonorValue = (int32_t)analogRead(PIN_MIKE);
      this->CheckSeuil();
      Push(EV::Micro, this->sonorValue);
      break;
    case EV::BesoinSeuilMic:
      Push(EV::ValeurSeuilMic, this->sonorSeuil);
      break;
    case EV::ModifierSeuilMic:
      this->sonorSeuil = param;
      dbset(sonorSeuil);
      break;
    default: break;
  }
  return ContinuerEVDistribution;
}  //END bool CSonorMeter::Event()

/*!
* @fn <CheckSeuil()>
* @brief <This function checks if the sound value exceeds a certain threshold.>
* @return <void>
*/
void CSonorMeter::CheckSeuil() {
  if (this->sonorValue > this->sonorSeuil) {
    Push(EV::SeuilAtteintMicro, this->sonorValue);
  }
}