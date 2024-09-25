/*!
* @file <CCarteSD.h>
* @version : <4>
* @brief <This file contains the declaration of the CCarteSD class, which is used for handling SD card operations.>
* @author <Yohan Davion, Evann POMMIER>
* @date <25/04/2024>
*/
#ifndef CCARTESD_H
#define CCARTESD_H

#include "EVManager.h"
#include <SPI.h>
#include <SdFat.h>

class CCarteSD : public EVClasse {
public:
  CCarteSD() {}
  bool Event(EV event, int32_t param);
  void init(File &file, String file_name);
  int32_t GetSdValuePC();
private:
  void enregistrer(int32_t param, File &file);
  void sauterLigne(File &file);
  File m_l;
  File m_r;
  File config;
  bool InitNOK = true;
  int32_t tempValue,
    humidValue,
    acceValue_X,
    acceValue_Y,
    acceValue_Z,
    gyroValue_X,
    gyroValue_Y,
    gyroValue_Z,
    lightValue,
    sonorValue,
    battLvlValue,
    sdLvlValue,
    spyOnOff = false;
};
#endif