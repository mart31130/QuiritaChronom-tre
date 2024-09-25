/*-----------------------------------------------------------------------------
V1.0 par DaK le 23 fev 2024 (adapté d'un exemple de la librairie)

Démonstration de l'utilisation de l'EEPROM de la carte X-NUCLEO IDB05A1 (modem BLE)
  L'EEPROM utilise le même périphérique SPI que le modem BLE = arduino pin 3, 11, et 12
  Pour le modem BLE la broche de sélection est PIN_BLE_SPI_nCS = arduino pin A1
  Pour l'eeprom 8Ko la broche de sélection est M95640R_nCS = arduino pin 8
  le basculement est fait automatiquement par les fonctions des librairies.

Montage à faire : STM32F401RE + IDB05A1

Librairie M95640R
EepromRead() et EepromWrite() travaillent sur des pages de 32 octets
On peut lire ou écrite 1 à 32 octets maximum
Il y a 256 pages.
  Page 0 address: 0x0000
  Page 1 address: 0x0020
  ...
  Page 255 address: 0x1FE0
Il faut donc donner l'adresse du premier octet de la page qui nous intéresse.

Extrait de la datasheet M95640R
  M95640 64-Kbit serial SPI bus EEPROM with high-speed clock
  64 Kb (8 Kbytes) of EEPROM
  Page size: 32 bytes
  Page Write within 5 ms
  More than 4 million Write cycles 
  More than 200-year data retention

Ca ne marche pas ?
  Vérifier que la IDB05A1 ne soit pas décalée d'une broche sur la F401RE.
  Vérifiez que vous êtes en 115200 Baud dans le moniteur série.
  Vérifiez que les broches {A0, A1, 3, 7, 8, 11, 12} ne sont pas connectée ailleur.
-----------------------------------------------------------------------------*/

#include "util_debug.h"
#include "SPI.h"
#include "M95640R.h"

// déclarations du bus SPI telles qu'utilisées pour le modem BLE carte dans la démo du BLE
#define PIN_BLE_SPI_MOSI   (11)  //PA7 = arduino pin 11
#define PIN_BLE_SPI_MISO   (12)  //PA6 = arduino pin 12
#define PIN_BLE_SPI_SCK    (3)   //PB3 = arduino pin 3
#define PIN_BLE_SPI_nCS    (A1)  //PA1 = arduino pin A1
#define PIN_BLE_SPI_RESET  (7)   //PA8 = arduino pin 7
#define PIN_BLE_SPI_IRQ    (A0)  //PA0 = arduino pin A0
// les correspondances sont tirées du fichier "X-NUCLEO-IDB05A1 schematic.pdf" 

// déclarations du bus SPI pour adresser l'EEPROM
#define M95640R_nCS        (8)  //PA9 = arduino pin 8
#define SPI_MOSI   PIN_BLE_SPI_MOSI
#define SPI_MISO   PIN_BLE_SPI_MISO
#define SPI_SCK    PIN_BLE_SPI_SCK

// Il faut créer l'interface SPI avant d'utiliser l'EEPROM ou le BLE de la carte IDB05A1
SPIClass IDB05A1_spi(SPI_MOSI, SPI_MISO, SPI_SCK);
// Création de l'objet EEPROM
M95640R  IDB05A1_eeprom(&IDB05A1_spi, M95640R_nCS);

//----- EepromValide() renvoit true si l'eeprom répond
uint8_t EepromValide(void);


void setup(){
  delay(2000);
  cout.begin(230400);
  for(int i=9;i>=0;i--,delay(8<<7)) cout << i << " " ;
  cout << "\nEEPROM test d'existence\n";
  IDB05A1_spi.begin(); 
  IDB05A1_eeprom.begin();
  if(!EepromValide()){
    cout << "EEPROM pas trouvée\n" ; while(true); // et on bloque ici
  } else{
    cout << "EEPROM presente\n" ;
  }// ifEepromExist()
  
  pinMode(LED_BUILTIN, OUTPUT);
}//setup()

void loop(){
  delay(100); digitalWrite(13,!digitalRead(13));
}// loop()


uint8_t EepromValide(void){
  uint8_t status=0;
  status = IDB05A1_eeprom.EepromStatus();
  debug(status);
  if((status&0xF0) == EEPROM_STATUS_SRWD) {
    /* If it is EEPROM_STATUS_SRWD => OK, the EEPROM is present and ready to work */
    status=1;
  }else{
    /* Else the bit may be not set (first time we see this EEPROM), try to set it*/
    IDB05A1_eeprom.EepromWriteEnable();
    delay(10); // on lui laisse gentiment un peu plus de 5ms pour faire le job ...
    status = IDB05A1_eeprom.EepromSetSrwd();
    debug(status);
    delay(10);
    /*check again*/
    status = IDB05A1_eeprom.EepromStatus();
    debug(status);
    if((status&0xF0) == EEPROM_STATUS_SRWD) { // 0xF0 mask [SRWD 0 0 0]
      /* If it is EEPROM_STATUS_SRWD => OK, the EEPROM is present and ready to work */
      status=1;
    }else{
      /* Else no EEPROM is present */
      status = 0;
    }// check again
  }
  return status;
}//EepromExist()


