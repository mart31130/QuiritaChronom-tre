

/* Includes ------------------------------------------------------------------*/

#include "M95640R_emulation.h"
#include <EEPROM.h>

/* Class Implementation ------------------------------------------------------*/
// M95640R simulée dans un l'eeprom d'un Arduino MEGA (4Ko eeprom)

/** Constructor
 * @param spi object of the instance of the spi peripheral
 * @param ns the spi chip select pin
 */
M95640R::M95640R(){ }

/**
* @brief  Initialize the M95640R library.
* @param  None.
* @retval None.
*/
void M95640R::begin(void){}

/**
* @brief  Read a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
* @param  nAddress the address of the page
* @param  cNbBytes the size in bytes of the data to be read
* @param  pcBuffer pointer to store the read data
* @retval None
*/
void M95640R::EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer){
	// l'eeprom ne rapportera pas ces erreurs, mais elle ne doivent pas se produire dans notre gestionnaire
	if(cNbBytes>32) return; // transfert supérieur à une page
	if( nAddress&0x1F > 0 ) return; // n'est pas une adresse de page
	uint8_t tampon[32];
  EEPROM.get(nAddress,tampon);
  memcpy(pcBuffer,tampon,cNbBytes);
	return;
}

/**
* @brief  Write a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
*         It is allowed to write only a page for each operation. If the bytes
*         exceed the single page location, the other bytes are written at the
*         beginning.
* @param  nAddress the address of the page
* @param  cNbBytes the size in bytes of the data to be written
* @param  pcBuffer pointer to the data to be written
* @retval None
*/
void M95640R::EepromWrite(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer){
  uint8_t tampon[32];
	// l'eeprom ne rapportera pas ces erreurs, mais elle ne doivent pas se produire dans notre gestionnaire
	if(cNbBytes>32) return; // transfert supérieur à une page
	if( nAddress&0x1F > 0 ) return; // n'est pas une adresse de page
	memcpy(tampon,pcBuffer,cNbBytes);
  EEPROM.put(nAddress,tampon);
	return;
}
