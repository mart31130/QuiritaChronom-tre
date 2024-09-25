/**
  ******************************************************************************
  * @file    sensor_service.h
  * @author  CL
  * @version V1.0.0
  * @date    04-July-2014
  * @brief
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BLE_SERVICE_H_
#define _BLE_SERVICE_H_

/* Includes ------------------------------------------------------------------*/
#include "hal_types.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap.h"
#include "string.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "hci_const.h"
#include "gp_timer.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"
#include "hci.h"
#include "hci_le.h"
#include "hal.h"
#include "sm.h"
#include "debug.h"
#include "Arduino.h"
#include <SPBTLE_RF.h>
#include <stdlib.h>



/** @addtogroup X-CUBE-BLE1_Applications
 *  @{
 */

/** @addtogroup SensorDemo
 *  @{
 */

/** @addtogroup SENSOR_SERVICE
 * @{
 */

/** @addtogroup SENSOR_SERVICE_Exported_Defines
 * @{
 */
/* Exported defines ----------------------------------------------------------*/
// Default Name
#define DEFAULT_DEVICE_NAME 'B','l','u','e','N','R','G'
/**
 * @}
 */

/** @addtogroup SENSOR_SERVICE_Exported_Types
 *  @{
 */
typedef int i32_t;
//typedef struct 
#pragma pack(push, 1)
struct int24{
    uint32_t data : 24;
};
#pragma pack(pop)

int24 getUInt24Val(uint32_t val);
/**
 * @brief Structure containing acceleration value (in mg) of each axis.
 */
typedef struct {
  i32_t AXIS_X;
  i32_t AXIS_Y;
  i32_t AXIS_Z;
} AxesRaw_t;

/**
 * @}
 */

/** @addtogroup SENSOR_SERVICE_Exported_Functions
 *  @{
 */
class BLEServiceClass
{
  public:
    tBleStatus begin(const char *name, uint8_t addr[BDADDR_SIZE]);
    tBleStatus end(uint16_t connhandle);

    void       setConnectable(void);
    void       setUnconnectable(void);
    int        isConnected(void);
    int       getTryConnectState();
    void       tryToConnect(short int x);

    tBleStatus Add_Acc_Service(void);
    tBleStatus Free_Fall_Notify(void);
    tBleStatus Acc_Update(AxesRaw_t *data);

    tBleStatus Add_Environmental_Sensor_Service(void);
    tBleStatus Temp_Update(int16_t temp);
    tBleStatus Press_Update(uint32_t press);
    tBleStatus Humidity_Update(uint16_t humidity);

    tBleStatus Add_Time_Service(void);
    void       Update_Time_Characteristics(void);
    tBleStatus Seconds_Update(void);
    tBleStatus Minutes_Notify(void);

    tBleStatus Add_Battery_Service(void);
    tBleStatus BatteryLevel_Update(uint8_t batterylvl);

    tBleStatus Add_LED_Service(void);
    bool       LED_State(void) {return ledState;}

    tBleStatus Add_Illuminance_Service(void);
    tBleStatus Illuminance_Update(uint32_t illuminanceValue);

    /*tBleStatus Add_Illuminance_Service(void);
    tBleStatus IlluminanceValue_Update(unsigned int24 illuminanceValue);*/

    //tBleStatus Add_Microphone_Service(void);
    //tBleStatus Microphone_Update(uint32_t microphoneValue);

    void       GAP_ConnectionComplete_CB(uint8_t addr[BDADDR_SIZE], uint16_t handle);
    void       GAP_DisconnectionComplete_CB();
    void       Read_Request_CB(uint16_t handle);
    void       Attribute_Modified_CB(uint16_t handle, uint8_t data_length,
                                     uint8_t *att_data);

    uint16_t sampleServHandle, TXCharHandle, RXCharHandle;
    uint16_t accServHandle, freeFallCharHandle, accCharHandle;
    uint16_t envSensServHandle, tempCharHandle, pressCharHandle, humidityCharHandle;

    uint16_t timeServHandle, secondsCharHandle, minuteCharHandle;
    uint16_t ledServHandle, ledButtonCharHandle;
    uint16_t batteryServHandle, batteryLvlCharHandle;
    uint16_t illuminanceServHandle, illuminannceCharHandle;
    //uint16_t microphoneServHandle, microphoneCharHandle;

  private:
    volatile uint16_t connection_handle = 0;
    volatile bool connected = FALSE;
    volatile bool set_connectable = TRUE;
    volatile bool notification_enabled = FALSE;
    volatile bool tryingToConnectState = FALSE;
    volatile short int initStatus = 0;
    volatile AxesRaw_t  axes_data;
    volatile int16_t    temp_data;
    volatile uint32_t    press_data;
    volatile uint16_t   hum_data;
    volatile uint8_t    batterylvl_data;
    volatile int24      lux_data;
    char dev_name[8] =  {AD_TYPE_COMPLETE_LOCAL_NAME, DEFAULT_DEVICE_NAME};
    uint8_t dev_nameLen;
    bool ledState = false;
    uint32_t previousMinuteValue = 0;
    
};
extern BLEServiceClass BLEService;
#endif /* _SENSOR_SERVICE_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
