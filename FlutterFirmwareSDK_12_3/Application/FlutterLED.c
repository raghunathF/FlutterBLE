#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_drv_spi.h"


#include "nrf_gpio.h"
#include "nrf_error.h"
#include "sdk_config.h"


#include "ble_gap.h"

#include "FlutterLED.h"

/************************************************************************/
/******************     Define         **********************************/
/************************************************************************/
#define SPI_INSTANCE  0 /**< SPI instance index. */

#define NOLEDS     3

#define NoBytesLED 12
#define STARTZEROS 12*5

#define LED_BRIGHTNESS_THRESHOLD 50

#define TWOBITSPIVALUES {0x88,0x8E,0xE8,0xEE}
#define TOTAL_SPI_LENGTH  NOLEDS*NoBytesLED+2*STARTZEROS

enum colors{NOCOLOR,RED,GREEN,BLUE,YELLOW,PINK,SKYBLUE,WHITE};


const	uint8_t SPIDataConertConsArray[] = TWOBITSPIVALUES;
uint8_t SPISendData[TOTAL_SPI_LENGTH];

typedef struct
{
	uint8_t RBrightness ;
	uint8_t GBrightness ;
	uint8_t BBrightness ;
}colorInfo;

static  colorInfo colorInitial[3];
static  colorInfo colorZero[3];




//Prototyoes
void SPI_init();



/************************************************************************/
/******************    Variables       **********************************/
/************************************************************************/
nrf_drv_spi_t    spi	= 	NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
extern  volatile bool 		spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */
static  uint8_t       		m_rx_buf[20];           /**< RX buffer. */


//SPI transfer
void transfer_data(uint8_t transmit_length,uint8_t* data_send)
{
	spi_xfer_done = false;
	memset(m_rx_buf, 0, transmit_length);
	nrf_drv_spi_transfer(&spi, data_send, transmit_length, m_rx_buf, transmit_length);
}
/************************************************************************/


void sendDataRegisterInit()
{
	uint8_t i =0;
	for(i=0;i<STARTZEROS;i++)
	{
		SPISendData[i]	=	0x00;
	}
	for(i=STARTZEROS;i<STARTZEROS + NOLEDS*NoBytesLED ;i++)
	{
		SPISendData[i]	=	0x88;
	}
	for(i=NOLEDS*NoBytesLED;i<STARTZEROS;i++)
	{
		SPISendData[i]    =   0x00;
	}
	
}

static uint8_t* convertColorSPIData(colorInfo* colorData )
{
	static uint8_t SPIData[NoBytesLED];
	volatile uint8_t i =0;
	uint8_t countSPIData = 0;
	volatile uint8_t temp =0;
	volatile uint8_t twoBitValue = 0;
	for(i=0;i<4;i++)
	{
		twoBitValue = (colorData->GBrightness & (0xC0>>i*2))>>(6-2*i);
		SPIData[countSPIData] = SPIDataConertConsArray[twoBitValue];
		countSPIData++;
	}
	for(i=0;i<4;i++)
	{
		//temp = (0x30>>(i*2));
		twoBitValue = (colorData->RBrightness & (0xC0>>i*2))>>(6-2*i);
		SPIData[countSPIData] = SPIDataConertConsArray[twoBitValue];
		countSPIData++;
	}
	for(i=0;i<4;i++)
	{
		twoBitValue = (colorData->BBrightness & (0xC0>>i*2))>>(6-2*i);
		SPIData[countSPIData] = SPIDataConertConsArray[twoBitValue];
		countSPIData++;
	}
	return SPIData;
}



static void  SetAllLEDs(colorInfo* colorLED)
{
	uint8_t* LEDSPIData = NULL;
	static bool transferOngoing = false;
	uint16_t startArrayAddress = 0;
	static uint8_t counter = 31;
	uint8_t i,j =0;
	
	for(j = 0 ; j < NOLEDS ; j++)
	{
		LEDSPIData			= convertColorSPIData(&colorLED[j]);
		startArrayAddress   = j*NoBytesLED + STARTZEROS ;
		for(i = 0 ; i < NoBytesLED ; i++)
		{
			SPISendData[i+startArrayAddress]   = LEDSPIData[i] ;
		}
	}
	//
	
	transfer_data(TOTAL_SPI_LENGTH,SPISendData);
	while(spi_xfer_done == false);
	spi_xfer_done = false;
	
	
}



//SPI callback 
void spi_event_handler(nrf_drv_spi_evt_t const * p_event)
{
		spi_xfer_done = true;
}


//
void updateColors(colorInfo* RGBInitial,uint8_t LEDNumber,uint8_t RContent , uint8_t GContent , uint8_t BContent)
{
	uint8_t i = 0;
	
	//RContent
	if(RContent == 1)
	{
		RGBInitial[LEDNumber].RBrightness = LED_BRIGHTNESS_THRESHOLD;
	}
	else
	{
		RGBInitial[LEDNumber].RBrightness = 0;
	}
	
	//GContent
	if(GContent == 1)
	{
		RGBInitial[LEDNumber].GBrightness = LED_BRIGHTNESS_THRESHOLD;
	}
	else
	{
		RGBInitial[LEDNumber].GBrightness = 0;
	}
	
	//BContent
	if(BContent == 1)
	{
		RGBInitial[LEDNumber].BBrightness = LED_BRIGHTNESS_THRESHOLD;
	}
	else
	{
		RGBInitial[LEDNumber].BBrightness = 0;
	}


}

uint8_t* setLED2Address()
{
	static uint8_t LEDColors[3];
	ble_gap_addr_t 				mac;
	sd_ble_gap_addr_get(&mac);
	
	LEDColors[0] = mac.addr[2] % 7;
	LEDColors[1] = mac.addr[1] % 7;
	LEDColors[2] = mac.addr[0] % 7;
	return LEDColors;
	
}


//
void initLEDS()
{
	
	uint8_t i =0;
	
	uint8_t RContent = 0;
	uint8_t GContent = 0;
	uint8_t BContent = 0;
	
	
	uint8_t* LEDColor = NULL;
	LEDColor = setLED2Address();
	for(i=0;i<NOLEDS;i++)
	{
		switch(LEDColor[i])
		{
			case RED :
				RContent = 1;
			  GContent = 0;
			  BContent = 0;
				
				break;
			case GREEN :
				RContent = 0;
			  GContent = 1;
			  BContent = 0;
				
				break;
			case BLUE:
				RContent = 0;
			  GContent = 0;
			  BContent = 1;
				
				break;
			case YELLOW:
				RContent = 1;
			  GContent = 1;
			  BContent = 0;
				
				break;
			case PINK:
				RContent = 1;
			  GContent = 0;
			  BContent = 1;
				
				break;
			case SKYBLUE:
				RContent = 0;
			  GContent = 1;
			  BContent = 1;
				
				break;
			case WHITE:
				RContent = 1;
			  GContent = 1;
			  BContent = 1;
				
				break;
			default:
				break;
		}
		updateColors(colorInitial,i,RContent,GContent,BContent);
	}
	SetAllLEDs(colorInitial);
	
}

void fade()
{
	static  colorInfo RGBInitial[3];
	uint8_t i,j = 0;
	for(j=0;j<255;j++)
	{
		for(i=0;i<3;i++)
		{
			RGBInitial[i].RBrightness = j;
			RGBInitial[i].GBrightness = j;
			RGBInitial[i].BBrightness = j;
		}
		SetAllLEDs(RGBInitial);
		nrf_delay_ms(1);
	}
	
}


void initLEDZero()
{
	static  colorInfo colorZero[3];
	uint8_t i =0;

	for(i=0;i<3;i++)
	{
		colorZero[i].RBrightness = 0;
		colorZero[i].GBrightness = 0;
		colorZero[i].BBrightness = 0;
	}
	
}



void checkLEDStatus( uint8_t batteryVolatge)
{
	static bool toggleState = false;
	
	if(batteryVolatge < BATTERY_THRESHOLD)
	{
		//Flash the colors
		if(toggleState == true)
		{
			 SetAllLEDs(colorInitial);
		}
		else
		{
			 SetAllLEDs(colorZero);
		}
	  
	}
}




/************************************************************************/
void LEDSPIInit()
{		
	 
	  nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	  spi_config.frequency            = NRF_DRV_SPI_FREQ_4M ;
    spi_config.ss_pin               = NRF_DRV_SPI_PIN_NOT_USED;
    spi_config.miso_pin             = NRF_DRV_SPI_PIN_NOT_USED;
    spi_config.mosi_pin             = SPI_MOSI_PIN;
    spi_config.sck_pin              = NRF_DRV_SPI_PIN_NOT_USED;
	  spi_config.mode                 = NRF_DRV_SPI_MODE_1;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler));
	  sendDataRegisterInit();
	  initLEDS();
	  initLEDZero();
	  //nrf_drv_spi_uninit(&spi);
	  //fade();
	
}
/************************************************************************/


