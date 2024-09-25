/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CTempMeter.h>
* @version : <1>
* @brief <contient la declaration de la class CTempMeter.>
* @author <Evann POMMIER> 
* @date < 28/02/2024> 
*/

#include <Arduino_HTS221.h>

class CTempMeter
{
  public :
    CTempMeter(){};
    void init();
    float GetTempValue();
};