#include <LSM6DSLSensor.h>
class CGyroAccel{
public :

void setStatus(int i);
int32_t * getAcceValue();
int32_t * getGyroValue();
void CGyroAccelloop();

private :

int status ;
void initialisation();
int32_t acceValue[3];
int32_t gyroValue[3];

};
