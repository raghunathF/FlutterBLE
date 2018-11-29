#include <stdint.h>
#include "app_timer.h"

#include "configGlobal.h"
#include "FlutterUART.h"


APP_TIMER_DEF(UARTReceiveTimer);

extern bool BLEReceiveDataReset;

void UARTReceiveTimerHandler()
{
		//Reset UART data
	  BLEReceiveDataReset = true;
}

/************************************************************************/
void ReceiveTimerInit()
{
  	volatile ret_code_t err_code;
	  err_code = app_timer_create(&UARTReceiveTimer, APP_TIMER_MODE_SINGLE_SHOT, UARTReceiveTimerHandler);
    APP_ERROR_CHECK(err_code);
}

void startUARTReceiveTimer()
{
		app_timer_start( UARTReceiveTimer, APP_TIMER_TICKS(MAX_RECEIVE_MILLI_THRES, 0), NULL);
}	

void stopUARTReceiveTimer()
{
		app_timer_stop( UARTReceiveTimer);
}