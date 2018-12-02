#include "stdint.h"
#include "stdlib.h"
#include "app_uart.h"
#include "configGlobal.h"

extern volatile uint8_t BLEReceiveBuffer[SIZE_DATA_BUFFER];
extern volatile uint8_t headPointerReceiveBuffer;
extern volatile uint8_t tailPointerReceiveBuffer;

#define LEDMATRIX  'L'
#define LEDMATRIX_LENGTH 19
#define CURRENT_PACKET_MASK		0x1C
#define TOTAL_PACKET_MASK			0xE0
#define COLOR_MASK 						0x03

#define RED					1
#define GREEN 			2
#define BLUE				3

void updateRGBBuffer(uint8_t* tempRGB, uint8_t* tempReceive , uint8_t color)
{
	uint8_t i = 0;
	
	switch(color)
	{
		case RED:
			for(i=1; i<19; i++)
			{
				tempRGB[i]   =	tempReceive[i];  
			}
			break;
		case GREEN:
			for(i=1; i<19; i++)
			{
				tempRGB[18+i] =	tempReceive[i];  
			}
			break;
		case BLUE:
			for(i=1; i<19; i++)
			{
				tempRGB[36+i] =	tempReceive[i];  
			}
			break;
		default:
			break;
	}
	
}

void readRemainingBytes(uint8_t* temp , uint8_t length)
{
	  uint8_t i =0;
		for(i=0;	i<length; i++)
		{
				temp[i] = BLEReceiveBuffer[tailPointerReceiveBuffer];
				tailPointerReceiveBuffer++;
		}
}

void refreshRGBLED(uint8_t* RGBtemp)
{
	uint8_t i =0;
	for(i=0;i<55;i++)
	{
		RGBtemp[i] = 0;
	}
	
}

void BLEControlLoop()
{
	uint8_t i =0;
	uint8_t length = 0;
	uint8_t color = 0 ;
  static uint8_t tempReceiveBuffer[19]; 
	static uint8_t RGBValues[54];
	RGBValues[0] = 'L';
	bool newPacketProgress = false;
	if(headPointerReceiveBuffer != tailPointerReceiveBuffer)
	{
		length = BLEReceiveBuffer[tailPointerReceiveBuffer];
		tailPointerReceiveBuffer++;
		switch(BLEReceiveBuffer[tailPointerReceiveBuffer])
		{
			case LEDMATRIX:
					tailPointerReceiveBuffer++;
					readRemainingBytes(tempReceiveBuffer,LEDMATRIX_LENGTH);
					color  = (tempReceiveBuffer[0]&COLOR_MASK);
					if( ( ((tempReceiveBuffer[0]&TOTAL_PACKET_MASK)>>5) - ((tempReceiveBuffer[0]&CURRENT_PACKET_MASK)>>2) ) > 0  )
					{		
							//update the respective RGB Buffer
							updateRGBBuffer( RGBValues ,tempReceiveBuffer , color);
					}
					else
					{
							//update the RGB Buffer
							updateRGBBuffer( RGBValues ,tempReceiveBuffer , color);
							//Send the 55 bytes
							for(i=0;i<55;i++)
							{
								while (app_uart_put(RGBValues[i]) != NRF_SUCCESS);
							}
							//Refresh RGB LED
							refreshRGBLED(RGBValues);
					}
					break;
			case 'b':
					readRemainingBytes(tempReceiveBuffer,length);
					for(i=0;i<length;i++)
					{
							while (app_uart_put(tempReceiveBuffer[i]) != NRF_SUCCESS);
					}
					break;
			default:
					headPointerReceiveBuffer = 0;
					tailPointerReceiveBuffer = 0;
					break;
		}
	}
	else
	{
		headPointerReceiveBuffer = 0;
		tailPointerReceiveBuffer = 0;
	}
	
		
	
}