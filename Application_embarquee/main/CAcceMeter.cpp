/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CAcceMeter.cpp>
* @version : <2>
* @brief <This file contains the code of the functions of CAcceMeter, which is used for handling accelerometer operations.>
* @author <Evann POMMIER>
* @date <20/03/2024>
*/
#include <CAcceMeter.h>

LSM6DSLSensor acceMeter(&Wire);


/*!
* @fn <init()>
* @brief <This function initializes the accelerometer sensor.>
* @return <void>
*/
void CAcceMeter::init() {
  InitNOK = !acceMeter.begin();
  acceMeter.Enable_X();
    dbreadint(seuilAcce, 0x200);  //valeur seuil par défaut 200, paramétréedans DB.
}
/*!
* @fn <Event(EV event, int32_t param)>
* @brief <This function handles events related to the accelerometer.>
* @param [in]  <event> <The event to be handled.>
* @param [in]  <param> <The parameter associated with the event.>
* @return <bool>
*/
bool CAcceMeter::Event(EV event, int32_t param) {
  switch (event) {  // Pass the event to the current state for processing
    case EV::ColdReset:
      this->init();
      break;
    case EV::Systic_100MS:
      if (!InitNOK) break;
      acceMeter.Get_X_Axes(this->acceValue);
      this->CheckSeuil(this->acceValue, this->seuilAcce);
      Push(EV::AccelerometreX, this->acceValue[0]);

      Push(EV::AccelerometreY, this->acceValue[1]);
      Push(EV::AccelerometreZ, this->acceValue[2]);
      break;
    case EV::BesoinSeuilAcce:
      Push(EV::ValeurSeuilAcce, this->seuilAcce);
      break;
    case EV::ModifierSeuilAcce:
      this->seuilAcce = param;
      dbset(seuilAcce);
      break;
    default: break;
  }
  return ContinuerEVDistribution;
}  //CAcceGyroMeter::Event()
/*!
* @fn <CheckSeuil(int32_t value[3], int32_t seuil, EV event, uint32_t module)>
* @brief <This function checks if the accelerometer values exceed a certain threshold.>
* @param [in]  <value> <The accelerometer values to be checked.>
* @param [in]  <seuil> <The threshold value.>
* @param [in]  <event> <The event to be triggered if the threshold is exceeded.>
* @return <void>
*/
void CAcceMeter::CheckSeuil(int32_t value[3], int32_t seuil) {
  this->module_Acce = (int32_t)sqrt(value[0] * value[0] + value[1] * value[1] + value[2] * value[2]);
  if (this->module_Acce >= seuil) Push(EV::SeuilAtteintAccelerometre, this->module_Acce);
}  //CAcceGyroMeter::CheckSeuil()