/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CAcceGyroMeter.h>
* @version : <1>
* @brief <contient la declaration de la class CAcceGyroMeter.>
* @author <Evann POMMIER> 
* @date < 05/03/2024> 
*/

#include <Wire.h>
#include <LSM6DSLSensor.h>

class CAcceGyroMeter
{
  public :
    CAcceGyroMeter() : accegyro(&wirePort, LSM6DSL_ACC_GYRO_I2C_ADDRESS_HIGH) {}
    void init();
    void GetAcceGyroValues(int32_t* acce, int32_t* gyro);
  private:
    TwoWire wirePort;
    LSM6DSLSensor accegyro;
};