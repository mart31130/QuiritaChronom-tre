#define DEV_I2C Wire
#define ssCOLD 0
#define ssWARM 1
#define ssRUN 2
#define ssDEAD 3

enum {sCOLD, sWARM, sRUN, sDEAD}; // COLD = 1er démarage | WARM = Veille | RUN = Fonctionnement

HTS221Sensor HumTemp(&DEV_I2C);

void CTempHumidsetup() {
  Serial.begin(115200);
}

CTempHumid::CTempHumid(){
  status = sCOLD;
}

void CTempHumid::CTempHumidloop(){
  while(this->status == sRUN){
  this->initialisation();
  HumTemp.GetHumidity(&humidValue);
  HumTemp.GetTemperature(&tempValue);
  this->setStatus(sWARM);
  // FAIRE ENDORMIR LE CAPTEUR ICI
  }
}

void CTempHumid::initialisation(){
  HumTemp.begin();
  HumTemp.Enable();
}

void CTempHumid::setStatus(int i){
  this->status = i;
}

float CTempHumid::getTempValue(){
  return this->tempValue;
}
float CTempHumid::getHumidValue(){
  return this->humidValue;
}
