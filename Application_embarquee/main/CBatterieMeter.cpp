/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CBatterieMeter.cpp>
* @version : <2>
* @brief <This file contains the code of the Event function of CBatterieMeter, which is used for handling battery level measurements.>
* @author <Evann POMMIER>
* @date <20/03/2024>
*/
#include <CBatterieMeter.h>

void CBatterieMeter::init() {
  batterieValuePC = 0;   // charge batterie en %
  batterieSeuil = 3000;  // seuil de décharge à 3000 mV (le minimun avant de tuer une 18650 est 2500 mV)
  mesureCalibree = 0;    // tension batterie en mV
  dbreadint(batterieSeuil, 3000);
}  //CBatterieMeter::init()
/*!
* @fn <Event(EV event, int32_t param)>
* @brief <This function handles events related to battery level measurements.>
* @param [in]  <event> <The event to be handled.>
* @param [in]  <param> <The parameter associated with the event.>
* @return <bool>
*/
bool CBatterieMeter::Event(EV event, int32_t param) {
  static int16_t multiplicateur = 2000;
  static int16_t decalage = 0;
  const uint32_t Vref = 3300;
  const uint32_t resolution = 1024;
  uint32_t adc_mv = 0;
  switch (event) {
    case EV::ColdReset: this->init(); break;
    case EV::Calibrage:
      decalage = param & 0x0000FFFF;
      multiplicateur = param >> 16;
      break;
    case EV::Systic_10MIN:
      // mesure batterie
      adc_mv = (analogRead(PIN_18650) * Vref) / resolution;
      this->mesureCalibree = (int32_t)(((adc_mv + decalage) * multiplicateur) / 1000);
      Push(EV::BatterieLevel_MV, this->mesureCalibree);
      if (this->mesureCalibree >= this->batterieSeuil) Push(EV::LowBatt, this->mesureCalibree);
      // estimation du % charge
      this->batterieValuePC = this->CalculPourcentage(this->mesureCalibree);
      Push(EV::BatterieLevel_PC, this->batterieValuePC);
      break;
    default: break;
  }
  return ContinuerEVDistribution;
}  //CBatterieMeter::Event()


uint32_t CBatterieMeter::CalculPourcentage(int32_t T) {
  // [2500..3600] => [1%..20%]
  // [3600..3800] => [20%..100%]
  // >= 3800 mv => 100%
  if (T > 3800) return 100;
  if (T > 3600) return map(T, 3600, 3800, 20, 100);
  if (T > 2500) return map(T, 2500, 3600, 1, 20);
  return 0;
}  //CalculPourCentage()

int32_t CBatterieMeter::GetBatterieValuePC() {
  return this->batterieValuePC;
}