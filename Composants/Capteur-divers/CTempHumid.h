#include <HTS221Sensor.h>
class CTempHumid{
public :

CTempHumid();

void setStatus(int i);
float getTempValue();
float getHumidValue();
void CTempHumidloop();

private :
int status ;
void initialisation();
float tempValue, humidValue;

};
