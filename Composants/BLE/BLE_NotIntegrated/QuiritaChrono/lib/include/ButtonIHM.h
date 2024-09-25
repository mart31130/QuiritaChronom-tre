
#ifndef INC_ButtonIHM
#define INC_ButtonIHM

//--- WARNING : cette classe ne devrait jamais être instanciée en plusieurs exemplaires à cause de l'ISR interne
//--> TODO en faire un singleton

#include "arduino.h"
#include "EVManager.h"

class ButtonIHM : public EVClasse {
    public:
    ButtonIHM(int pin); //You can use USER_BUTTON PC_13 on STM32 Nucleo
    bool Event( EV event, int32_t param );
};

#endif
