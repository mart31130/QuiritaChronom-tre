/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CAcceGyroMeter.cpp>
* @version : <1>
* @brief <contient le code des fonctions de CAcceGyroMeter, init() et GetAcceGyroValues.>
* @author <Evann POMMIER> 
* @date < 05/03/2024> 
*/

#include <CAcceGyroMeter.h>

void CAcceGyroMeter::init()
{
  Serial.println(F("IKS01A2 Ready"));
  wirePort.begin();
  accegyro.Enable_X();
  accegyro.Enable_G();
}
void CAcceGyroMeter::GetAcceGyroValues(int32_t* acce,int32_t* gyro)
{
	accegyro.Get_X_Axes(acce);
	accegyro.Get_G_Axes(gyro);
}