
#ifndef INC_ButtonIHM
#define INC_ButtonIHM

//--- WARNING : cette classe ne devrait jamais être instanciée en plusieurs exemplaires à cause de l'ISR interne
//--> TODO en faire un singleton

#include "EVManager.h"

class ButtonIHM : public EVClasse {
public:
  ButtonIHM();  //You can use USER_BUTTON PC_13 on STM32 Nucleo
  bool Event(EV event, int32_t param);
};

#endif
