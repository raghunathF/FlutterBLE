
/************************************************************************/
/******************     Includes       **********************************/
/************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "FlutterBLEPinout.h"
#include "configGlobal.h"
/************************************************************************//************************************************************************/
/******************     Defines        **********************************/
/************************************************************************/
#define SPI_SS_PIN				PIN_UNUSED      //P
#define SPI_MISO_PIN			PIN_UNUSED
#define SPI_MOSI_PIN			LED_IN_PIN
#define SPI_SCK_PIN				PIN_UNUSED
//#define INITIAL_LENGTH    19
/************************************************************************/

void LEDSPIInit();
void fade();
void LEDAdvInit();

void checkLEDStatus( uint8_t batteryVolatge);
void LEDControlLoop();