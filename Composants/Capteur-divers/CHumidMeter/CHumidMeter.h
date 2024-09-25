/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CHumidMeter.h>
* @version : <1>
* @brief <contient la declaration de la class CHumidMeter.>
* @author <Evann POMMIER> 
* @date < 28/02/2024> 
*/

#include <Arduino_HTS221.h>

class CHumidMeter
{
  public :
    CHumidMeter(){};
    void init();
    int32_t GetHumidValue();
};