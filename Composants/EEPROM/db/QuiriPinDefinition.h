#ifndef QuiriPinDefinition_INC
#define QuiriPinDefinition_INC
// Broches utilisées par la carte IDB05A1 BLE+EEPROM
// Source "X-NUCLEO-IDB05A1 schematic.pdf" 
#undef PIN_SPI_MISO
#define PIN_SPI_MISO      (12)  //PA6 = arduino pin 12
#undef PIN_SPI_MOSI
#define PIN_SPI_MOSI      (11)  //PA7 = arduino pin 11
#define PIN_SPI_RESET     (7)   //PA8 = arduino pin 7
#undef PIN_SPI_SCK
#define PIN_SPI_SCK       (3)   //PB3 = arduino pin 3
#define PIN_SPI_IRQ       (A0)  //PA0 = arduino pin A0
#define PIN_SPI_nCSBLE    (A1)  //PA1 = arduino pin A1
#define PIN_SPI_nCSEEPROM (8)   //PA9 = arduino pin 8

// Broches utilisées par la carte IKS01A2 MEMS
#define PIN_I2C_SCL         //PB_8
#define PIN_I2C_SDA         //PB_9
//à compléter


/* Arduino connecteur CN6 
---------------------------
NC
IOREF
NRST
+3V3
+5V
GND
GND
VIN
*/

/* Arduino connecteur CN8
---------------------------
A0 IDB05A1 PIN_SPI_IRQ
A1 IDB05A1 PIN_SPI_nCSBLE
A2 IKS01A2 LSM303AGR_DRDY
A3 IKS01A2 LSM303AGR_INT
A4 IKS01A2 INT2(DIL24)
A5 IKS01A2 INT1(DIL24)
*/

/* Arduino connecteur CN5
---------------------------
D15 IKS01A2 I2C_SCL             PA_8
D14 IKS01A2 I2C_SDA             PA_9
AVDD
GND
D13 NUCLEO  LED_BUILTIN         PA_5
D12 IDB05A1 SPI_MISO            PA_6
D11 IDB05A1 SPI_MOSI            PA_7
D10                             PB_6
D9                              PC_7
D8  IDB05A1 PIN_SPI_nCSEEPROM   PA_9
*/

/* Arduino connecteur CN9
---------------------------
D7                      PA_8
D6 IKS01A2 LPS22HB_INT1 PB_10
D5 IKS01A2 LSM6DSL_INT2 PB_4
D4 IKS01A2 LSM6DSL_INT1 PB_5
D3                      PB_3
D2 IKS01A2 USER_INT     PA_10
D1         SERIAL2TX    PA_2
D0         SERIAL2TX    PA_3
*/

#endif