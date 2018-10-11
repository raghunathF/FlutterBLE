#include <stdint.h>
#include <stdlib.h>
#include "ble_gap.h"


#define FLUTTER_INITIAL_1 'F' 
#define FLUTTER_INITIAL_2 'L'

static char convert_ascii(char input)
{
	char output;
	if(input >= 0 & input <=9)
	{
		output = input + 0x30;
	}
	else
	{
		output = input + 0x37;
	}
	return output;
}


uint8_t* getDeviceName()
{
	static uint8_t deviceName[20];
	ble_gap_addr_t 				mac;
	uint8_t addr_byte 		= 0;
	
	sd_ble_gap_addr_get(&mac);
	
	deviceName[0] = FLUTTER_INITIAL_1;
	deviceName[1] = FLUTTER_INITIAL_2;
	deviceName[2] = convert_ascii((mac.addr[2]&0x0F));
	
	deviceName[3] = (mac.addr[1]&0xF0);
	deviceName[3] = convert_ascii(deviceName[3]>>4);
	
	deviceName[4] = convert_ascii(mac.addr[1]&0x0F);
	deviceName[5] = (mac.addr[0]& 0xF0);
	deviceName[5] = convert_ascii(deviceName[5]>>4);
	
	deviceName[6] = convert_ascii(mac.addr[0]&0x0F);
	deviceName[7] = '\0';
	
	return deviceName; 
}
