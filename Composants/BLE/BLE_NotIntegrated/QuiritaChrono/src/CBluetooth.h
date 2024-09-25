//Avoid Recursive Includes
#ifndef INC_CBluetooth_H
#define INC_CBluetooth_H
//Includes
#include "BLE_service.h"
#include "bluenrg_hal_aci.h"
#include "hci.h"
#include "hci_le.h"
#include "bluenrg_utils.h"
#include "stm32_bluenrg_ble.h"
#include "osal.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "gp_timer.h"
#include "Arduino.h"
#include "EVManager.h"
#include <SPI.h>
#include <SPBTLE_RF.h>

class CBluetooth : public EVClasse {
public :
CBluetooth(int x);
~CBluetooth();
void initBLE();
bool Event( EV event, int32_t param );
void initConnection();
int getInitState();
int getTryConnectState();
int isConnected();
void disableConnection();
void btleUpdate();

private :
short int initServices = 0;
BLEServiceClass BLEService;

};
extern CBluetooth BleExtension;
#endif /* INC_CBluetooth_H */