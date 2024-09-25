/*!
* Project Name: QUIRITACHRONOMETRE
* @file <CQuirita.cpp>
* @version : <2>
* @brief <This file contains the code of the functions of CQuirita, which is the main class for handling Quirita operations.>
* @author <Evann POMMIER, Yohan Davion>
* @date <19/03/2024>
*/
#include "CQuirita.h"

CQuirita::CQuirita()
  : BoutonPoussoir(),
    AfficheLesEvenementsQuiPassent(),
    csd(),
    humidMeter(),
    lightMeter(),
    tempMeter(),
    sonorMeter(),
    acceMeter(),
    batterieMeter(),
    bluetooth() {
}
/*!
* @fn <initialisation()>
* @brief <This function initializes the Quirita system by pushing initial events and adding listeners.>
* @return <void>
*/
void CQuirita::initialisation() {
  Push(EV::ColdReset, 0);
  Push(EV::Spy_ON_OFF, true);
  Push(EV::SpyIgnoreEven, (uint32_t)EV::Dummy);
  Push(EV::SpyIgnoreEven, (uint32_t)EV::Systic_100MS);
  Push(EV::SpyIgnoreEven, (uint32_t)EV::Micro);
  Push(EV::SpyIgnoreEven, (uint32_t)EV::ShellActif);
  Push(EV::SpyIgnoreEven, (uint32_t)EV::AccelerometreX);
  Push(EV::SpyIgnoreEven, (uint32_t)EV::AccelerometreY);
  Push(EV::SpyIgnoreEven, (uint32_t)EV::AccelerometreZ);
  Push(EV::SpyIgnoreEven, (uint32_t)EV::SeuilAtteintAccelerometre);
  Push(EV::SpyIgnoreEven, (uint32_t)EV::SeuilAtteintMicro);
  Push(EV::SpyIgnoreEven, (uint32_t)EV::NoMore);
  Push(EV::Dummy, 0xABCD0123);  // pour vérifier le filtre dans Spy
  //---- relecture des paramètrages en EEPROM
  {                                               // CBatterieMeter. TODO : logiquement, cela devrait plutôt être codé dans CBatterieMeter->EV::ColdInit
    uint32_t batterie_mul = 0, batterie_dec = 0;  // ((Vmesurée+Batterie_dec)*Batterie_mul)/1000
    dbreadint(batterie_mul, 2000);                // pont diviseur de tension 10K + 10K => 3,7V mesure 1,95V
    dbreadint(batterie_dec, 0);
    Push(EV::Calibrage, (batterie_mul << 16) | batterie_dec);
  }
  // valeur par défaut       "013456789ABCDEF0123456789ABCDE" // max 31 caratères + zéro terminal
  dbreadstr(QuiritaIdentifiant, "I am not personalized");
  //voir aussi DB.QuiriUID[6] @MAC du dispositif = 4 octets MCUuid + 2 octets arbitaires
}  //CQuirita::initialisation()

/*!
* @fn <addListenerAll()>
* @brief <This function adds all listeners to the Quirita system.>
* @return <void>
*/
void CQuirita::addListenerAll() {
  AddListerner(&BoutonPoussoir);
  AddListerner(&AfficheLesEvenementsQuiPassent);
  AddListerner(&csd);
  AddListerner(&humidMeter);
  AddListerner(&lightMeter);
  AddListerner(&tempMeter);
  AddListerner(&sonorMeter);
  AddListerner(&acceMeter);
  AddListerner(&batterieMeter);
  AddListerner(&bluetooth);
  AddListerner(&interaction);
}

int32_t CQuirita::GetBatterieValuePC() {
  return this->batterieMeter.GetBatterieValuePC();
}
  int32_t CQuirita::GetSdValuePC() {
    return this->csd.GetSdValuePC();
  }