/*
  Ce code permet d'ajouter des services codés dans service au Bluetooth afin d'y communiquer des valeurs.
  Chaque service possède des caractérisitques. Les caractéristiques peuvent être lu, écrites, 
  en fonction des permissions.
  Ce code a été pensé pour la arte STM32 Nucleo F401RE équipé de la carte
  d'extension de capteur X_NUCLEO_IKS01A2 et la carte extension BLE X_NUCLEO_IDB05A1


  - X_NUCLEO_IDB05A1 (BlueNRG-MS expansion board) on top of an STM32 Nucleo board

  Les services sont lancés dans le main.  
 For the Sensor Service sketch, 3 services are started : Acc, Environnemental and Time.
 For testing the sketch, you can download on the playstore the "BlueNRG"
 application provided by STMICROELECTRONICS.
 Launch the application and enable Bluetooth on your smartphone. Connect it to
 the BLueNRG device. You will see all the services, you can click on each one.

 You can choose the bluetooth MAC address of the device by configuring SERVER_BDADDR.

 Accelerometer values are updated on user action (press user button).
 Environnemental values (Temperature, humidity and pressure) are updated each seconds.
 Each minute a notification is sent to the user and seconds can be read.

 */

#include "CBluetooth.h"


#define PIN_BLE_SPI_MOSI   (11)
#define PIN_BLE_SPI_MISO   (12)
#define PIN_BLE_SPI_SCK    (3)

#define PIN_BLE_SPI_nCS    (A1)
#define PIN_BLE_SPI_RESET  (7)
#define PIN_BLE_SPI_IRQ    (A0)
  
#define PIN_BLE_LED    (0xFF)

#define SerialPort Serial

#define I2C2_SCL    PB10
#define I2C2_SDA    PB3 // Use PB3 on F401RE cf : Alternate Function Mapping

/*TwoWire dev_i2c(I2C2_SDA, I2C2_SCL);
HTS221Sensor  HumTemp(&dev_i2c);*/

// Configure BTLE_SPI
SPIClass BTLE_SPI(PIN_BLE_SPI_MOSI, PIN_BLE_SPI_MISO, PIN_BLE_SPI_SCK);

// Configure BTLE pins
SPBTLERFClass BTLE(&BTLE_SPI, PIN_BLE_SPI_nCS, PIN_BLE_SPI_IRQ, PIN_BLE_SPI_RESET, PIN_BLE_LED);

/* Public variables ----------------------------------------------------------*/


const char *name = "Boitier";
uint8_t SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x03};
uint32_t previousSecond = 0;

CBluetooth::CBluetooth(int x) {
  
}
CBluetooth::~CBluetooth() {}

bool CBluetooth::Event(EV event, int32_t param ){
  
  switch(event){
    case EV::LongPress:
      cout.println("Event Longpress in Switch Case");
      this->initBLE();
      BLEService.tryToConnect(TRUE);
      //boucle if delay, deconnecte
      
      break;
    case EV::ShortPress:
      cout.println("Case ShortPress");
      BLEService.tryToConnect(FALSE);
      break;
    case EV::ColdReset:
      
      break;
    case EV::Temperature:
      //cout << "temp = : " << param;
      if((millis() - previousSecond) >= 1000 && initServices == 1) {
        previousSecond = millis();
        BLEService.Temp_Update(random(-100,400));
      }
      break;
    case EV::Accelerometre:
      //cout << "acc = " << param;

      break;
  }
  return ContinuerEVDistribution;
}

//Retourne l'état d'initialisation
int CBluetooth::getInitState() {
  return this->initServices;
}

/*Retourne si l'utilisateur essaie de se connecter au boitier
bool CBluetooth::getConnectState() {
  return this->tryingToConnectState;
}*/
int CBluetooth::getTryConnectState() {
  return BLEService.getTryConnectState();
}

void CBluetooth::initConnection() {
  if (getTryConnectState() == TRUE) {
    BLEService.setConnectable();
    cout.println("DISCOVERING ...");
    cout << '\n' << BLEService.getTryConnectState();
    if (!isConnected()) { this->BLEService.tryToConnect(FALSE); }
  }
  else if (getTryConnectState() == FALSE) {
    BLEService.setUnconnectable();
    cout << '\n' << BLEService.getTryConnectState();
    cout.println("DISCOVER MODE DISABLED");
  }
  
    //Faire un délai Compter les secondes, attendre 10 seconde et repasser l'état a false. 
    //Utiliser méthode HalGetTick, comparaison Time Elapsed gérer incrément. 
}

void CBluetooth::btleUpdate() {
  if (initServices == 1) {
    BTLE.update();
  }
}

int CBluetooth::isConnected() {
  return this->BLEService.isConnected();
}

//Initialise la configuration du module, des services et caractéristiques
void CBluetooth::initBLE() {
  if (!initServices) {
    int ret;

    //SerialPort.begin(115200);

    if(BTLE.begin() == SPBTLERF_ERROR)
    {
      SerialPort.println("Bluetooth module configuration error!");
      while(1);
    }

    if(BLEService.begin(name, SERVER_BDADDR))
    {
      SerialPort.println("Sensor service configuration error!");
      while(1);
    }

    /* Configure the User Button in GPIO Mode */
    //pinMode(USER_BTN, INPUT);

    ret = BLEService.Add_Acc_Service();

    if(ret == BLE_STATUS_SUCCESS)
      SerialPort.println("Acc service added successfully.");
    else
      SerialPort.println("Error while adding Acc service.");

    ret = BLEService.Add_Environmental_Sensor_Service();

    if(ret == BLE_STATUS_SUCCESS)
      SerialPort.println("Environmental Sensor service added successfully.");
    else
      SerialPort.println("Error while adding Environmental Sensor service.");

    randomSeed(analogRead(A0));

    /* Instantiate Timer Service with two characteristics:
    * - seconds characteristic (Readable only)
    * - minutes characteristics (Readable and Notifiable )
    */
    ret = BLEService.Add_Time_Service();

    if(ret == BLE_STATUS_SUCCESS)
      SerialPort.println("Time service added successfully.");
    else
      SerialPort.println("Error while adding Time service.");

    ret = BLEService.Add_Battery_Service();

    if(ret == BLE_STATUS_SUCCESS)
      SerialPort.println("Battery service added successfully.");
    else
      SerialPort.println("Error while adding Battery service.");

    if (ret == BLE_STATUS_SUCCESS)
      SerialPort.println("BLE Servies have been initialized");
    else {
      SerialPort.println("BLE Services not initialized. Call Support"); 
    }
    this->initServices = 1;
  }
  else
    cout << "BLE already initialized\n";
}
