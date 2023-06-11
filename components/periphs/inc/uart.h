#ifndef __UART__
#define __UART__

/* ----------------------------------------------------------------------- */
//                              INCLUDE FILES                              //
/* ----------------------------------------------------------------------- */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


/* ----------------------------------------------------------------------- */
//                               DEFINES                                   //
/* ----------------------------------------------------------------------- */

#define UART_BUFFER_SIZE    1024

#define UART_PORT_1         0
#define UART_PORT_2         1
#define UART_PORT_3         2

// UART PIN SETTINGS
#define UART_PORT_1_TX_PIN  35
#define UART_PORT_1_RX_PIN  34
#define UART_PORT_1_RTS_PIN -1
#define UART_PORT_1_CTS_PIN -1

#define UART_PORT_2_TX_PIN  15
#define UART_PORT_2_RX_PIN  16
#define UART_PORT_2_RTS_PIN -1
#define UART_PORT_2_CTS_PIN -1

#define UART_PORT_3_TX_PIN  14
#define UART_PORT_3_RX_PIN  34
#define UART_PORT_3_RTS_PIN -1
#define UART_PORT_3_CTS_PIN -1

/* ----------------------------------------------------------------------- */
//                           TYPE DEFINATIONS                              //
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
//                          FUNCTION PROTOTYPE                             //
/* ----------------------------------------------------------------------- */

bool uart_init(int port_num);

int  uart_write(int port_num, char *data, size_t data_len);

int  uart_read(int port_num, char* buffer, size_t buffer_len);

bool uart_setBaud(int port_num, uint32_t baudrate);

bool uart_setParity(int port_num, uint8_t parity);

bool uart_setStopBit(int port_num, uint8_t stop_bit);

bool uart_setDataBit(int port_num, uint8_t data_bit);

bool uart_destroy(int port_num);


#endif /* __UART__ */

/* ----------------------------------------------------------------------- */
//                               END OF CODE                               //
/* ----------------------------------------------------------------------- */