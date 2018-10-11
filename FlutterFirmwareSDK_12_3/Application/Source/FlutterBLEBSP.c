/**
* Bord Support Package
* 1.Configure all the pins 
*	2.Configure all the modules
*/

#include <stddef.h>
#include <stdio.h>
#include "nrf_gpio.h"
#include "boards.h"
#include "FlutterBLEPinout.h"
#include "configGlobal.h"
#include "app_uart.h"
#include "FlutterBLEBSP.h"
#include "FlutterCallbacks.h"



/**
*Initialize the Advertsing indicaion pin and BLE test pin as outputs
*/

void	initializeIO()
{
		nrf_gpio_cfg_output(BLE_ADV_PIN);
		nrf_gpio_pin_clear(BLE_ADV_PIN);
		nrf_gpio_cfg_output(BLE_TEST_PIN);
		nrf_gpio_pin_clear(BLE_TEST_PIN);
}


void UARTInit()
{
	uint32_t    err_code;
  const app_uart_comm_params_t comm_params =
   {
        UART_RX_PIN,
        UART_TX_PIN,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        UART_BAUDRATE_BAUDRATE_Baud115200
   };

    APP_UART_FIFO_INIT( &comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_event_handle,
                       APP_IRQ_PRIORITY_LOWEST,
                       err_code);
    APP_ERROR_CHECK(err_code);
}

