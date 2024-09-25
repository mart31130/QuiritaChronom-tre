/*!
* Project Name: QUIRITACHRONOMETRE
* @file <CQuirita.h>
* @version : <2>
* @brief <contient la declaration de la class CQuirita.>
* @author <Yohan Davion , Evann POMMIER> 
* @date < 07/03/2024> 
*/
#pragma once
#include "EVManager.h"
#include "ButtonIHM.h"
#include "Spy.h"
#include "CCarteSD.h"
#include "CLightMeter.h"
#include "CHumidMeter.h"
#include "CAcceMeter.h"
#include "CSonorMeter.h"
#include "CTempMeter.h"
#include "CBatterieMeter.h"
#include "CBluetooth.h"
#include "CInteraction.h"
#include "db.h"

class CQuirita {
public:
  CQuirita();
  void addListenerAll();
  void initialisation();
  CInteraction interaction;
  //---- identification du Quirita
  char QuiritaIdentifiant[MAXSIZEOF];  //paramètre en EEPROM
  int32_t GetBatterieValuePC();
  int32_t GetSdValuePC();
private:
  ButtonIHM BoutonPoussoir;
  Spy AfficheLesEvenementsQuiPassent;
  CHumidMeter humidMeter;
  CLightMeter lightMeter;
  CTempMeter tempMeter;
  CSonorMeter sonorMeter;
  CAcceMeter acceMeter;
  CBatterieMeter batterieMeter;
  CCarteSD csd;
  CBluetooth bluetooth;
};