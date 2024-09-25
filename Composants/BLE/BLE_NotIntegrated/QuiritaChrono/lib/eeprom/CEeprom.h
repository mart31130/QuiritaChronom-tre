#ifndef INC_CEEPROM
#define INC_CEEPROM
#include "DB.h"
//#include "EVManager.h"

class CEeprom /*: public EVClasse*/ {
    public :
    CEeprom(); // (int x)
    ~CEeprom();
    void AfficheDB();
    void AfficheQuiriUID();
    //bool Event( EV event, int32_t param );
    
    private :
    //M95640R eeprom;
    CDb db;
};
#endif