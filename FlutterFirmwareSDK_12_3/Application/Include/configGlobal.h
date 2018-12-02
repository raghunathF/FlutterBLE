/**
*	Declare #defines which are used to configure different modules
* Kepp all the global variables in one place.
*/
#include <stdio.h>
#include <stdlib.h>




//UART
#define BAUD_RATE					    UART_BAUDRATE_BAUDRATE_Baud115200

#define UART_TX_BUF_SIZE                256                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                256                                         /**< UART RX buffer size. */

#define SIZE_DATA_BUFFER								255

//APP_TIMER
#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */

#define MAX_RECEIVE_MILLI_THRES         15