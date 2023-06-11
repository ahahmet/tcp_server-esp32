/* ----------------------------------------------------------------------- */
//                              INCLUDE FILES                              //
/* ----------------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include "driver/uart.h"
#include "freertos/queue.h"

#include "common.h"
#include "uart.h"

/* ----------------------------------------------------------------------- */
//                               DEFINES                                   //
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
//                           GLOBAL VARIABLE                               //
/* ----------------------------------------------------------------------- */

static QueueHandle_t uart_queue[3];

/* ----------------------------------------------------------------------- */
//                          PRIVATE FUNCTIONS                              //
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
//                           GLOBAL FUNCTIONS                              //
/* ----------------------------------------------------------------------- */

/**
 * @brief This function initialize the uart 
 * 
 * @param port_num Uart handle
 * @return true , if is succeed
 * @return false , if is failed
 */
bool uart_init(int port_num)
{
    int uart_port_num;
    int uart_rx_pin;
    int uart_tx_pin;
    int uart_rts_pin;
    int uart_cts_pin;
    int index;

    if(port_num == UART_PORT_1){
        uart_port_num = UART_NUM_0;
        uart_tx_pin  = UART_PORT_1_TX_PIN;
        uart_rx_pin  = UART_PORT_1_RX_PIN;
        uart_rts_pin = UART_PORT_1_RTS_PIN;
        uart_cts_pin = UART_PORT_1_CTS_PIN;
        index = 0;

    }else if(port_num == UART_PORT_2){
        uart_port_num = UART_NUM_1;
        uart_tx_pin = UART_PORT_2_TX_PIN;
        uart_rx_pin = UART_PORT_2_RX_PIN;
        uart_rts_pin = UART_PORT_2_RTS_PIN;
        uart_cts_pin = UART_PORT_2_CTS_PIN;
        index = 1;

    }else if(port_num == UART_PORT_3){
        uart_port_num = UART_NUM_2;
        uart_tx_pin = UART_PORT_3_TX_PIN;
        uart_rx_pin = UART_PORT_3_RX_PIN;
        uart_rts_pin = UART_PORT_3_RTS_PIN;
        uart_cts_pin = UART_PORT_3_CTS_PIN;
        index = 2;
    }else{
        DEBUGE("Invalid UART port");
        return false;
    }

    uart_config_t uart_conf = 
    {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_EVEN,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    ERROR_CHECK(uart_driver_install(uart_port_num, UART_BUFFER_SIZE,
                  UART_BUFFER_SIZE, 10, &(uart_queue[index]), 0),
                  "UART driver install error");

    ERROR_CHECK(uart_param_config(uart_port_num, &uart_conf),
                  "UART config error");

    ERROR_CHECK(uart_set_pin(uart_port_num, uart_tx_pin, uart_rx_pin, 
                  uart_rts_pin, uart_cts_pin),
                  "UART set pin error");

    return true;
}

/**
 * @brief This function send the data over the uart port
 * 
 * @param port_num Uart handle
 * @param data sended data
 * @param data_len sended data length
 * @return int sended data length, if is failed returns -1
 */
int uart_write(int port_num, char *data, size_t data_len)
{
    if(data_len > UART_BUFFER_SIZE){
        DEBUGE("UART transmit size error");
        return -1;
    }
    return uart_write_bytes(port_num, data, data_len);
}

/**
 * @brief This function recieve the data over the uart port
 * 
 * @param port_num Uart handle
 * @param buffer Recieve buffer
 * @param buffer_len Recieve buffer length
 * @return int int recieved data length, if is failed returns -1
 */
int uart_read(int port_num, char* buffer, size_t buffer_len)
{
    if(buffer_len > UART_BUFFER_SIZE){
        DEBUGE("UART recieve size error");
        return -1;
    }
    return uart_read_bytes(port_num, buffer, buffer_len, 0);
}

/**
 * @brief This function changes the uart baudrate value
 * 
 * @param port_num Uart handle
 * @param baudrate Uart baudrate value
 * @return true , if is succeed
 * @return false , if is failed
 */
bool uart_setBaud(int port_num, uint32_t baudrate)
{
    ERROR_CHECK(uart_set_baudrate(port_num, baudrate), "Baudrate set error");   
    return true;
}

/**
 * @brief This function changes the uart parity value
 * 
 * @param port_num Uart handle
 * @param parity Uart parity value
 * @return true , if is succeed
 * @return false , if is failed
 */
bool uart_setParity(int port_num, uint8_t parity)
{
    ERROR_CHECK(uart_set_parity(port_num, parity), "Baudrate set error");   
    return true;
}

/**
 * @brief This function changes the uart stop bit value
 * 
 * @param port_num Uart handle
 * @param stop_bit Uart stop bit value
 * @return true , if is succeed
 * @return false , if is failed
 */
bool uart_setStopBit(int port_num, uint8_t stop_bit)
{
    if((stop_bit != UART_STOP_BITS_1) && (stop_bit != UART_STOP_BITS_2)){
        DEBUGE("Stop bit chose error");
    }
    ERROR_CHECK(uart_set_stop_bits(port_num, stop_bit), "Stop bit set error");   
    return true;

}

/**
 * @brief This function changes the uart data bit value
 * 
 * @param port_num Uart handle
 * @param data_bit Uart data bit value
 * @return true , if is succeed
 * @return false , if is failed
 */
bool uart_setDataBit(int port_num, uint8_t data_bit)
{
    ERROR_CHECK(uart_set_word_length(port_num, data_bit), "UART driver delete error");   
    return true;
}

/**
 * @brief This function uninitialize the uart and then close the port
 * 
 * @param port_num Uart handle
 * @return true , if is succeed
 * @return false , if is failed
 */
bool uart_destroy(int port_num)
{
    ERROR_CHECK(uart_driver_delete(port_num), "UART driver delete error");   
    return true;
}

/* ----------------------------------------------------------------------- */
//                               END OF CODE                               //
/* ----------------------------------------------------------------------- */