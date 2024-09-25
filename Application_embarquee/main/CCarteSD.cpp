#include "EVManager.h"
/*!
* @file <CCarteSD.cpp>
* @version : <5>
* @brief <This file contains the code of the functions of CCarteSD, which are used for handling SD card operations.>
* @author <Evann POMMIER, Yohan Davion>
* @date <25/04/2024>
* 20240511 DK : this->spyOnOff trop bavard. TODO ajouter this->InitOK + n'envoyer les erreurs qu'une seule fois
*/
#include <CCarteSD.h>

SPIClass SPI_2(PB15, PB14, PB13);
SdFat sd;
SdSpiConfig spiConfig(PB12, DEDICATED_SPI, SD_SCK_MHZ(50), &SPI_2);
/*!
* @fn <Event(EV event, int32_t param)>
* @brief <This function allows you to do what you want to do depending on the current event>
* @param [in]  <event is of type EV which is an enumeration of the EVentList.h file, which allows you to use events>
* @param [in]  <param is an int32_t which allows you to retrieve its data for each event.>
* @return <bool>
*/
bool CCarteSD::Event(EV event, int32_t param) {
  switch (event) {
    case EV::ColdReset:
      this->init(this->m_l, "m_l.csv");
      this->init(this->m_r, "m_r.csv");
      this->init(this->config, "config.txt");
      break;
    case EV::Spy_ON_OFF:
      //this->spyOnOff = param; DK : trop bavard + évènement utilisé au sens de debug
      this->spyOnOff = false;
      break;
    case EV::BatterieLevel_MV:
      this->battLvlValue = param;
      break;
    case EV::Temperature:
      this->tempValue = param;
      break;
    case EV::Humidite:
      this->humidValue = param;
      break;
    case EV::AccelerometreX:
      this->acceValue_X = param;
      break;
    case EV::AccelerometreY:
      this->acceValue_Y = param;
      break;
    case EV::AccelerometreZ:
      this->acceValue_Z = param;
      break;
    case EV::Lux:
      this->lightValue = param;
      break;
    case EV::Micro:
      this->sonorValue = param;
      break;
    case EV::Systic_100MS:
      if (InitNOK) break;
      if (param == 0) {
        this->m_r = sd.open("m_r.csv", FILE_WRITE);
        if (this->m_r.size() == 0) { this->m_r.print("Accelerometre_X;Accelerometre_Y;Accelerometre_Z;Micro\n"); }
        if (this->m_r) {
          enregistrer(this->acceValue_X, this->m_r);
          enregistrer(this->acceValue_Y, this->m_r);
          enregistrer(this->acceValue_Z, this->m_r);
          enregistrer(this->sonorValue, this->m_r);
          sauterLigne(this->m_r);
        } else {
          if (this->spyOnOff) {
            cout.print("Erreur : Impossible d'ouvrir le fichier de sortie mesures rapides\n");
            cout.print("Verifiez la carte SD et appuyez sur le bouton RESET\n");
          }
        }
        this->m_r.close();
      }
      break;
    case EV::Systic_10MIN:
      this->sdLvlValue = this->GetSdValuePC();
      cout.println(this->sdLvlValue);
      Push(EV::SDLvl, this->sdLvlValue);
      if (InitNOK) break;
      if (param == 0) {
        this->m_l = sd.open("m_l.csv", FILE_WRITE);
        if (this->m_l.size() == 0) { this->m_l.print("Temperature en C°;BatterieLvl %;Luminosite en lux;Humiditee en %\n"); }
        if (this->m_l) {
          enregistrer(this->tempValue, this->m_l);
          enregistrer(this->battLvlValue, this->m_l);
          enregistrer(this->lightValue, this->m_l);
          enregistrer(this->humidValue, this->m_l);
          sauterLigne(this->m_l);
        } else {
          if (this->spyOnOff) {
            cout.print("Erreur : Impossible d'ouvrir le fichier de sortie mesures lentes\n");
            cout.print("Verifiez la carte SD et appuyez sur le bouton RESET\n");
          }
        }
        this->m_l.close();
      }
      break;
    default:
      break;
  }
  return ContinuerEVDistribution;
}
/*!
* @fn <init(File &file, String file_name)>
* @brief <This function initializes a file and prepares it for writing data.>
* @param [out]  <file> <The file object to be initialized and written to.>
* @param [in]  <file_name> <The name of the file to be opened and written to.>
* @return <void>
*/
void CCarteSD::init(File &file, String file_name) {
  SPI_2.begin();

  if (this->spyOnOff) {
    cout.print("Initialisation de ");
    cout.print(file_name);
    cout.print("\n");
  }
  if ((InitNOK = !sd.begin(spiConfig))) {
    if (this->spyOnOff) {
      cout.print("Erreur : Impossible d'initialiser la carte SD\n");
      cout.print("Verifiez la carte SD et appuyez sur le bouton RESET\n");
    }
  }
  if (InitNOK) return;
  file = sd.open(file_name, FILE_WRITE);
  if ((InitNOK = !file)) {
    if (this->spyOnOff) {
      cout.print("Erreur : Impossible d'ouvrir le fichier\n");
      cout.print("Verifiez la carte SD et appuyez sur le bouton RESET\n");
    }
  }
  file.close();
}  //END CCarteSD::init()
/*!
* @fn <enregistrer(int32_t param, File file)>
* @brief <This function records data to a file.>
* @param [in]  <param> <The data to be recorded.>
* @param [in]  <file> <The file object to which the data will be written.>
* @return <void>
*/
void CCarteSD::enregistrer(int32_t param, File &file) {
  if (!InitNOK) {
    file.print(param);
    file.print("; ");
  }
}
/*!
* @fn <sauterLigne(File file)>
* @brief <This function writes a newline character to the specified file.>
* @param [in]  <file> <The file object to which the newline character will be written.>
* @return <void>
*/
void CCarteSD::sauterLigne(File &file) {
  if (!InitNOK) {
    file.print("\n");
  }
}

int32_t CCarteSD::GetSdValuePC() {
  if (!InitNOK) {
  double totalSize = 0;
  // Parcourir le répertoire racine et calculer la taille totale des fichiers
  File file = sd.open("/");
  File entry;
  if (!file) {
    if (this->spyOnOff) {
      cout.println("Failed to open root directory");
    }
    return -1;
    }
  while ((entry = file.openNextFile())) {
    if (!entry.isDirectory()) {
      totalSize += entry.size();
    }
    entry.close();
  }
  file.close();
  // Convertir la taille en pourcentage de la capacité totale
  return (int32_t)((totalSize / 15703441408) * 100);
  }
}