/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <CSonorMeter.h>
* @version : <1>
* @brief <contient la declaration de la class CSonorMeter.>
* @author <Evann POMMIER> 
* @date < 05/03/2024> 
*/

#include <Arduino_HTS221.h>
#define PIN A2

class CSonorMeter
{
  public :
    CSonorMeter(){};
    void init();
    int16_t GetSonorValue();
};