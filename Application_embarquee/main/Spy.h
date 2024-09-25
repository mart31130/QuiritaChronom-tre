
#ifndef INC_SPY
#define INC_SPY

#include "arduino.h"
#include "EVManager.h"


class Spy : public EVClasse {
public:
  Spy();
  bool Event(EV event, int32_t param);
};

#endif
