
/* Prevent recursive inclusion -----------------------------------------------*/

#ifndef __M95640R_emulation_H__
#define __M95640R_emulation_H__

/* Includes ------------------------------------------------------------------*/
#include "arduino.h"
   
/**
 * Abstract class of a M95640-R EEPROM simulée en RAM
 */
class M95640R{
  public:
    M95640R();
    void begin(void);
    void EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
    void EepromWrite(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
};

#endif /* __M95640R_emulation_H__ */
