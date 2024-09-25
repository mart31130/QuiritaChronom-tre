// Type de carte : "Nucleo64"
// Board part number : "Nucleo F401RE" ou "Nucleo L152RE"
//note STM32 Nucleo: Pour la liaison série USB on utilisera Serial2 sous l'alias cout
// Tous les objets de l'application sont regouprés dans la classe CQuirita et le loop anime la machine à état
// Librairies à télécharger dans l'environnement : X-NUCLEO-IKS01A2 pour les capteurs accéléromètre

#include "CQuirita.h"
#include "EVManager.h"
#include "SimpleSerialShell.h" //l'objet shell est déclaré en externe dans le .h de SimpleSerialShell 
#include <STM32TimerInterrupt.h>  //https://github.com/khoih-prog/STM32_TimerInterrupt

#include <STM32LowPower.h>  //https://github.com/stm32duino/STM32LowPower
#include "CInteraction.h"
#include "SPI.h"
#include "QuiriPinDefinition.h"
#include "db.h"

#define SYSTIC100ms 100000

CQuirita quirita;
STM32Timer ITimer0(TIM1);
// l'interface globale SPI doit exister avant toute utilisation
SPIClass IDB05A1_spi(PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCK);

// Horloge de séquencement du système
void SysticHandler(void) {
  static uint16_t nb100Ms = 0;
  Push(EV::Systic_100MS, 0);  //Systic principal à 100MS
  if (++nb100Ms > 60/*00*/) {nb100Ms = 0;
  Push(EV::Systic_10MIN, 0);  //decompte de 10MIN avant enregistrement sur carte sd
  }
}  //SysticHandler()

HardwareSerial Serial6(PA12,PA11); // BT HC05 connecté sur Serial6 en (PA12 et PA11)
#define HC05 Serial6               // HC05 est un nom plus explicite pour communiquer avec le modude HC05 


void setup() {
  delay(5000);  // 5s pour garder la main en cas de satellisation
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  LowPower.begin();
  HC05.begin(9600); // la liaison BT vers l'appli soignant
  cout.begin(230400); // la liaison série USB pour le technicien
  cout << "\n----------------------------\n";
  if (!db_begin()) {cout << "EEPROM non trouvée\n"; while (true);}  // et on bloque ici
  //shell.attach(HC05);// par défaut le shell est connecté à la liaison USB du technicien.
  shell.attach(cout); // par défaut le shell est connecté à la liaison USB du technicien.
  Shell_AddCommands();
  quirita.addListenerAll();
  quirita.initialisation();
  SendAll();  // un premier sendAll() pour permettre les initialisations (EV::ColdReset) qui prennent environ 12 secondes
  ITimer0.attachInterruptInterval(1 * SYSTIC100ms, SysticHandler);  // ce démarrage doit être différé après les initialisations
}  //setup()

void loop() {
  if (HC05.available()) {shell.attach(HC05);} // choix du flux entre le hc05 ou le terminal
  if (cout.available()){shell.attach(cout);}
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  shell.executeIfInput();
  {  // début section monitorée
    uint32_t deb = HAL_GetTick();
    SendAll();
    uint32_t elaps = HAL_GetTick() - deb;
    if (elaps >= SYSTIC100ms) cout << "SendAll overflow : " << elaps << " ms\n";
  }  //fin section monitorée
  if (!quirita.interaction.GetFlagInteraction()) { /*cout.println("mise en veille.")*/;LowPower.sleep(); }
}  //loop()
