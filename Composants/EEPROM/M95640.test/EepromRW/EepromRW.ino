/*
V1.0 par DaK le 24 fev 2024 (adapté d'un exemple de la librairie)

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
  Vérifier que la IDB05A1 ne soit pas décalée d'une broche.
  Vérifiez que vous êtes en 115200 Baud dans le moniteur série.
  Vérifiez que les broches {A0, A1, 3, 7, 8, 11, 12} ne sont pas connectée ailleur.
  
Précaution importante :
  Le bus SPI est bidirectionnel. Les opérations de lecture et d'écriture sont identiques.
  Lecture  : Buffer -> MOSI -> M95640R -> MISO -> Buffer
  Ecriture : Buffer -> MOSI -> M95640R -> MISO -> Buffer
  En cas d'écriture, le contenu du buffer d'origine sera détruit,
  du moins dans cette version de la librairie M95640R.cpp
  C'est un effet de bort de cette techno non pris en compte par cette librairie.
  => il faut toujours écrire une copie de la donnée sinon celle-ci sera écrasée.
*/

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

// Il faut créer l'interface SPI avant d'utiliser l'EEPROM ou le BLE 
SPIClass IDB05A1_spi(SPI_MOSI, SPI_MISO, SPI_SCK);
// Création de l'objet EEPROM
M95640R  IDB05A1_eeprom(&IDB05A1_spi, M95640R_nCS);

//----- EepromValide() renvoit true si l'eeprom répond
uint8_t EepromValide(void);

//----- EepromRW() lecture/ecriture d'une page au hazard
uint8_t EepromRW(void);

void setup(){
  delay(2000);
  cout.begin(230400);
  cout << "EEPROM test d'existence\n";
  IDB05A1_spi.begin(); 
  IDB05A1_eeprom.begin();
  if(!EepromValide()){
    cout << "EEPROM pas trouvée\n" ; while(true); // et on bloque ici
  } else{
    cout << "EEPROM presente\n" ;
  }// ifEepromExist()
  
  EepromRW();

  pinMode(LED_BUILTIN, OUTPUT);
}//setup()

void loop(){
  delay(100); digitalWrite(13,!digitalRead(13));
}// loop()

//---- Hex(data) --------- imprime data en hexadécimal sur le moniteur série ---------
// template = truc de paresseux. En regardant ses usages effectifs le compilateur va créer 
// autant de versions que nécessaire avec le même code source :
// une version 16 bits et une version 32bits.
template <typename T> void Hex(T data){
    int i=sizeof(data)*2; // le nombre de demi-octets dans data
    char tmp[1+i]; //<- n'est pas un tableau dynamique car à cet instant précis le compilateur connait la valeur exacte de i
    tmp[i]=0; // le zéro de fin de chaine ASCII en C
    while(i>0){
      char b=data&0x0F; // un demi-octet
      tmp[--i]=(b<10)?('0'+b):('A'+b-10); // conversion demi-octet vers ASCII
      data>>=4; // prochain demi-octet
    }
    cout.print(tmp);
}//END Hex()


void PrintHexa(uint8_t* var, size_t varsize ){
  cout << "0x" ;
  for(size_t i=0,j=1; i<varsize; i++,j++){
    Hex(var[i]);
    if(j==4){ j=0; cout << ' ';}
  }
  cout << '\n';
}//PrintHexa()

uint8_t EepromRW(void){
  uint16_t  Adresse = 0x1FE0; // début de la 255 ième et dernière page
  uint8_t BufferOrg[32]={'U'};
  IDB05A1_eeprom.EepromRead(Adresse, sizeof(BufferOrg), BufferOrg); cout << "R.BufferOrg[]=" ; PrintHexa(BufferOrg,sizeof(BufferOrg));

  uint8_t BufferUUU[32];
  cout << "\nécriture UUUUUUUU puis lecture\n" ;
  for(uint8_t i=0; i<32; i++ ) BufferUUU[i] = 'U';   cout << " .BufferUUU[]=" ; PrintHexa(BufferUUU,sizeof(BufferUUU));
  IDB05A1_eeprom.EepromWrite(Adresse, sizeof(BufferUUU), BufferUUU); cout << "W.BufferUUU[]=" ; PrintHexa(BufferUUU,sizeof(BufferUUU));
  IDB05A1_eeprom.EepromRead (Adresse, sizeof(BufferUUU), BufferUUU); cout << "R.BufferUUU[]=" ; PrintHexa(BufferUUU,sizeof(BufferUUU));
  cout << "Si les 3 lignes ci-dessus sont différentes, alors EepromWrite() écrase le buffer ! \n" ;

  uint8_t BufferNew[32];
  cout << "\ninversion puis écriture puis lecture\n" ;
  for(uint8_t i=0; i<32; i++ ) BufferNew[i] = BufferOrg[i] ^ 0xFF;   cout << " .BufferNew[]=" ; PrintHexa(BufferNew,sizeof(BufferNew));
  IDB05A1_eeprom.EepromWrite(Adresse, sizeof(BufferNew), BufferNew); cout << "W.BufferNew[]=" ; PrintHexa(BufferNew,sizeof(BufferNew));
  IDB05A1_eeprom.EepromRead (Adresse, sizeof(BufferNew), BufferNew); cout << "R.BufferNew[]=" ; PrintHexa(BufferNew,sizeof(BufferNew));
  cout << "Si les 3 lignes ci-dessus sont différentes, alors EepromWrite() écrase le buffer ! \n" ;

  cout << "\nre-écriture du contenu original \n" ;                    cout << " .BufferOrg[]=" ; PrintHexa(BufferOrg,sizeof(BufferOrg));
  IDB05A1_eeprom.EepromWrite(Adresse, sizeof(BufferOrg), BufferOrg);  cout << "W.BufferOrg[]=" ; PrintHexa(BufferOrg,sizeof(BufferOrg));
  IDB05A1_eeprom.EepromRead (Adresse, sizeof(BufferOrg), BufferOrg);  cout << "R.BufferOrg[]=" ; PrintHexa(BufferOrg,sizeof(BufferOrg));
  cout << "Si les 3 lignes ci-dessus sont différentes, alors EepromWrite() écrase le buffer ! \n" ;

  cout << "\n Conclusion\nManipuler M95640R.EepromWrite() en sachant qu'on récupère un buffer modifié.\n" \
       << "https://www.arduino.cc/reference/en/language/functions/communication/spi/transfer/ \n" \
       << "Send given byte to device. Returns byte received from device.\n" ;

  return 0;
}


uint8_t EepromValide(void){
  uint8_t status=IDB05A1_eeprom.EepromStatus();
  if((status&0xF0)==EEPROM_STATUS_SRWD) return true; // EEPROM présente et prète à travailler
  // peut-être que le bit d'écriture est encore vierge ?
  IDB05A1_eeprom.EepromWriteEnable(); delay(10); // on lui laisse gentiment un peu plus de 5ms pour faire le job ...
  IDB05A1_eeprom.EepromSetSrwd(); delay(10);
  status = IDB05A1_eeprom.EepromStatus(); // on relance le test d'accès à l'EEPROM
  return ((status&0xF0)==EEPROM_STATUS_SRWD);
}//EepromValide()


