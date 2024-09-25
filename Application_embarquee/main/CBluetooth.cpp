/*!
* @file <CBluetooth.cpp>
* @version : <1>
* @brief <This file contains the code of the functions of CBluetooth, which are used for handling Bluetooth operations.>
* @author <Samy RAFA, Evann POMMIER>
* @date <25/04/2024>
*/
#include <CBluetooth.h>

void CBluetooth::init() {
}

bool CBluetooth::Event(EV event, int32_t param) {
  switch (event) {
    case EV::ColdReset:
      this->init();
      break;
    case EV::BesoinSeuilAcce:
      this->seuilAcce = param;
      break;
    case EV::BesoinSeuilMic:
      this->seuilMicro = param;
      break;
    case EV::BatterieLevel_PC:
      this->battLvlValue = param;
      break;
    case EV::SDLvl:
      this->sdLvlValue = param;
      break;
    default:
      break;
  }
  return ContinuerEVDistribution;
}

