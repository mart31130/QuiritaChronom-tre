/*!
* Project Name: QUIRITACHRONOMETER
* @file <CInteraction.cpp>
* @version : <2>
* @brief <contains the code for the functions of the CInteraction class.>
* @author <Evann POMMIER> 
* @date <30/04/2024> 
*
*/
#include "CInteraction.h"

/*!
* @fn <Event(EV event, int32_t param)>
* @brief <This function handles events related to interactions.>
* @param [in]  <event> <The event to be handled.>
* @param [in]  <param> <The parameter associated with the event.>
* @return <bool>
*/
bool CInteraction::Event(EV event, int32_t param) {
  switch (event) {
    case EV::ColdReset:
      this->init();
      break;
    case EV::ShellActif: [[fallthrough]];
    case EV::ShortPress:
      this->SetFlagInteraction(true);
      break;
    case EV::Systic_100MS:
      static uint16_t nb_100Ms_temps_interaction = 0;
      if (this->GetFlagInteraction()) {
        if (++nb_100Ms_temps_interaction > 60) {nb_100Ms_temps_interaction = 0;
          this->SetFlagInteraction(false);
        }
      }
      break;
    default: break;
  }
  return ContinuerEVDistribution;
}

/*!
* @fn <init()>
* @brief <This function initializes the interaction flag.>
* @return <void>
*/
void CInteraction::init() {
  this->SetFlagInteraction(false);
}

/*!
* @fn <GetFlagInteraction()>
* @brief <This function returns the current state of the interaction flag.>
* @return <boolean>
*/
bool CInteraction::GetFlagInteraction() {
  return this->flagInteraction;
}

/*!
* @fn <SetFlagInteraction(int32_t flag)>
* @brief <This function sets the value of the interaction flag.>
* @param [in]  <flag> <The value to set for the interaction flag.>
* @return <void>
*/
void CInteraction::SetFlagInteraction(bool flag) {
  this->flagInteraction = flag;
}