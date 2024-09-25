/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CAcceGyroMeter.ino>
* @version : <1>
* @brief <contient le code qui instancie CAcceGyroMeter et qui affiche le retour de la fonction GetAcceGyroValues>
* @author <Evann POMMIER> 
* @date < 05/03/2024> 
*/

#include "CAcceGyroMeter.h"

CAcceGyroMeter accegyroMeter;

void setup() {
  Serial.begin(115200);
  accegyroMeter.init();
}

void loop() {
  int32_t acce[3],gyro[3];
  accegyroMeter.GetAcceGyroValues(acce,gyro);
  Serial.print("Accelerometre-X:");
  Serial.print(acce[0]);
  Serial.print(" Y:");
  Serial.print(acce[1]);
  Serial.print(" Z:");
  Serial.print(acce[2]);
  Serial.print("  Gyroscope-X:");
  Serial.print(acce[0]);
  Serial.print(" Y:");
  Serial.print(acce[1]);
  Serial.print(" Z:");
  Serial.println(acce[2]);
  delay(100);
}