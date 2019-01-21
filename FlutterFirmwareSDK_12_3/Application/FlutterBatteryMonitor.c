#include "stdint.h"
#include "stdlib.h"
#include "app_uart.h"
#include "app_timer.h"
#include "configGlobal.h"

/*
extern bool broadcastFlag;
extern bool lowBatteryCheckTimeOut;

APP_TIMER_DEF(batteryTimerID);


void batteryTimerHandler()
{
	lowBatteryCheckTimeOut = true;
}


void initBatteryTimer(void)
{
	 volatile uint32_t err_code = 0;
	 err_code = app_timer_create(&batteryTimerID,APP_TIMER_MODE_REPEATED,batteryTimerHandler);
   APP_ERROR_CHECK(err_code);
	 
}


void broadcast_start()
{
	volatile uint32_t err_code = 0;
	err_code = app_timer_start(batteryTimerID, APP_TIMER_TICKS(1000, 0), NULL);
	APP_ERROR_CHECK(err_code);
}


void broadcast_stop()
{
	volatile uint32_t err_code = 0;
	err_code = app_timer_stop(batteryTimerID);
	APP_ERROR_CHECK(err_code);
}


void batteryMonitorInit()
{
	
}


//Check every 1000 millisecond
void  checkLowBattery()
{
	static uint8_t reactCount = 0;
	
	//Check if time has elapsed
	if(lowBatteryCheckTimeOut == true)
	{
		lowBatteryCheckTimeOut = false;
		if(broadcastFlag == true)
		{	
				//Read the stored battery value
			  
		}
		else
		{
		    //Send a command and read the battery  value 
				
			
		}
	}
	
	///React every 8 seconds
	if(reactCount > REACT_COUNT_THRESHOLD)
	{
			reactCount++;
	}
	
	checkLEDStatus();
	
}
*/