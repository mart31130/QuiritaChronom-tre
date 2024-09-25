/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CLightMeter.h>
* @version : <1>
* @brief <contient la declaration de la class CLightMeter.>
* @author <Evann POMMIER> 
* @date < 28/02/2024> 
*/

#include <BH1750.h>
#include <Wire.h>

class CLightMeter {
public:
  CLightMeter(){};
  void init();
  int32_t GetLightValue();
private:
  BH1750 lightMeter;
};