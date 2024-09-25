/* dbdriver est la partie de la librairie db.cpp qui code l'adaptation à une eeprom précise.
   dbdriver est appelé en tête de db.cpp et ne doit être appelé nulle part ailleur.
DB utilise uniquement les 3 fonctions suivantes pour gérer une eeprom :
    uint8_t eeInit();  true si l'init se passe bien
    void eeWrite(uint16_t addr, uint8_t size, uint8_t* buffer); 
    void eeRead(uint16_t addr, uint8_t size, uint8_t* buffer);
    et aussi eeMAXKEYS qui dépend de la taille de l'eeprom
    note importante:
    Un périphérique de communication (SPI ou I2C selon le type d'eeprom)   doit exister et être initialisé dans le main du programme. Lire détails demandés dans l'implémentation.
*/

// ---- implémentation du driver pour l'eeprom M95640R de la carte fille NUCLEO.IDB05A1
#include "M95640R.h"
#include "QuiriPinDefinition.h"
#define eeMAXKEYS (8192/64)
//*** Une interface SPI doit exister avant d'utiliser l'EEPROM ***
//*** il faut cet objet global dans le programme principal
//*** Exemple :
//*** SPIClass IDB05A1_spi(PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCK);
extern SPIClass IDB05A1_spi;
// Création de l'objet EEPROM
M95640R  IDB05A1_eeprom(&IDB05A1_spi, PIN_SPI_nCSEEPROM);

void eeWrite(uint16_t addr, uint8_t size, uint8_t* buffer){ 
  static uint8_t tmp[32]; 
  memcpy(tmp,buffer,32); //contourne un effet de bord de SPI : la source est modifiée
  IDB05A1_eeprom.EepromWrite(addr, size, tmp); 
  /* le buffer SPI est circulaire WRITE = Buffer -> MOSI -> EEPROM -> MISO -> Buffer 
  avec pour conséquence qu'en écriture, les octets écrits sont remplacés par ce que retourne le composant.
  Ce qui a pour effet de bord de modifier la donnée source !!!
  Ca m'a pris 2 jours pour comprendre ça !
  On pourrait aussi modifier la librairie M95640R originale.
  */
}//eeWrite()

void eeRead(uint16_t addr, uint8_t size, uint8_t* buffer){ 
  /* le buffer SPI est circulaire WRITE = Buffer -> MOSI -> EEPROM -> MISO -> Buffer */
  IDB05A1_eeprom.EepromRead(addr, size, buffer); 
}//eeRead()


uint8_t EepromValide(void){
    uint8_t status=IDB05A1_eeprom.EepromStatus();
    /* EEPROM_STATUS_SRWD => OK, the EEPROM is present and ready to work */
    /* 0xF0 mask [SRWD 0 0 0] */
    if((status&0xF0) == EEPROM_STATUS_SRWD) return true;
    /* The bit may be not set (first time we see this EEPROM), try to set it*/
    IDB05A1_eeprom.EepromWriteEnable();
    delay(10); // on laisse gentiment un peu plus de 5ms pour faire le job ...  
    status = IDB05A1_eeprom.EepromStatus(); /*check again*/
    if((status&0xF0) == EEPROM_STATUS_SRWD) return true;
    return false;
}//EepromValide()

uint8_t eeInit(){
  IDB05A1_spi.begin(); 
  IDB05A1_eeprom.begin();
  return EepromValide();
}
    
