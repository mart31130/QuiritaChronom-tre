// configuration dans Arduino/Outils/
// Type de carte : "Nucleo64"
// Board part number : "Nucleo F401RE" ou "Nucleo L152RE"
// Usart support: "Enable (generic serial)"
// USB Support (if available): "rien"
//
//note STM32 Nucleo: Pour la liaison série USB on utilisera Serial2 sous l'alias cout

#include <EVManager.h>
#include <ButtonIHM.h>
#include <Spy.h>
#include <CTempHumid.h>
#include <CAccelGyro.h>
#include <CLux.h>
#include "CBluetooth.h"
#include "CEeprom.h"
#include "DB.h"

//---- Déclaration des objets utilisés au niveau main
ButtonIHM BoutonPoussoir(USER_BTN);
Spy AfficheLesEvenementsQuiPassent(1);
CTempHumid CapteurTempHumid(1);
CAccelGyro CapteurAccelGyro(1);
CLux CapteurLux(1);
CBluetooth BleExtension(1);

//CEeprom Eeprom(1);
int newState;



void setup(){
    delay(1000);
    pinMode(LED_BUILTIN,OUTPUT);
    Wire.begin();
    cout.begin(230400);
    cout << "\n----------------------------\n";
    AddListerner(&BoutonPoussoir);
    AddListerner(&AfficheLesEvenementsQuiPassent);
    AddListerner(&CapteurTempHumid);
    AddListerner(&CapteurAccelGyro);
    AddListerner(&CapteurLux);
    AddListerner(&BleExtension);
    //AddListerner(&Eeprom);
    Push(EV::ColdReset,HAL_GetTick());
    Push(EV::Spy_ON_OFF,true);
    Push(EV::SpyIgnoreEven,(uint32_t)EV::Dummy);
    Push(EV::Dummy,0xABCD0123); // pour vérifier le filtre dans Spy
    Push(EV::SpyIgnoreEven,(uint32_t)EV::NoMore); // quand on en a marre de voir l'EV 0x7FFF
}//END setup()

void loop(){
    static uint32_t next = 20;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN) ); // juste pour que ça bouge un peu
    Push(EV::Systic,next=next>0?next-1:20); // le paramètre décompte avant enregistrement sur carte SD
    SendAll();
    
    BleExtension.btleUpdate();
    
    if (BLEService.isConnected() == TRUE) {
        cout.println("THIS DEVICE IS CONNECTED");
    }
    else {
        BleExtension.initConnection();
    }
}//END loop()
