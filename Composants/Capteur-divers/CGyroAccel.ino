#define DEV_I2C Wire
//enum {COLD, WARM, RUN, DEAD}; // COLD = 1er démarage | WARM = Veille | RUN = Fonctionnement 

LSM6DSLSensor AccGyr(&DEV_I2C);

void CGyroAccelsetup() {
  Serial.begin(115200);
}

void CGyroAccel::CGyroAccelloop(){
  while(this->status == 2){
  this->initialisation();
  AccGyr.Get_X_Axes(acceValue);
  AccGyr.Get_G_Axes(gyroValue);
  this->status = 1;
  // FAIRE ENDORMIR LES CAPTEURS ICI
  }
}

void CGyroAccel::initialisation(){
  AccGyr.begin();
  AccGyr.Enable_X();
  AccGyr.Enable_G();
}

void CGyroAccel::setStatus(int i){
  this->status = i;
}

int32_t * CGyroAccel::getAcceValue(){
  return this->acceValue;
}
int32_t * CGyroAccel::getGyroValue(){
  return this->gyroValue;
}
