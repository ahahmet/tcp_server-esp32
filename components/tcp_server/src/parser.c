/* ----------------------------------------------------------------------- */
//                              INCLUDE FILES                              //
/* ----------------------------------------------------------------------- */
#include <stdint.h>
#include "cJSON.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "uart.h"
#include "common.h"

/* ----------------------------------------------------------------------- */
//                               DEFINES                                   //
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
//                           GLOBAL VARIABLE                               //
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
//                          PRIVATE FUNCTIONS                              //
/* ----------------------------------------------------------------------- */

static bool check_parity(const char parity)
{
    switch (parity) {
        case 'E':
        case 'e':
            uart_setParity(UART_PORT_2, UART_PARITY_EVEN);
            break;
        case 'O':
        case 'o':
            uart_setParity(UART_PORT_2, UART_PARITY_ODD);
            break;
        case 'N':
        case 'n':
            uart_setParity(UART_PORT_2, UART_PARITY_DISABLE);
            break;
        default:
            DEBUGE("Invalid character");
            return false;
    }

    return true;
}

static bool check_data_bit(const char data_bit)
{
    switch (data_bit)
    {
        case '7':
            uart_setDataBit(UART_PORT_2, UART_DATA_7_BITS);
            break;
        case '8':
            uart_setDataBit(UART_PORT_2, UART_DATA_8_BITS);
            break;
        case '9':
            uart_setDataBit(UART_PORT_2, UART_DATA_BITS_MAX);
            break;
        default:
            DEBUGE("Invalid character");
            return false;
    }
    return true;
}

static bool check_stop_bit(const char stop_bit)
{
    switch (stop_bit) 
    {
        case '1':
            uart_setStopBit(UART_PORT_2, UART_STOP_BITS_1);
            break;
        case '2':
            uart_setStopBit(UART_PORT_2, UART_STOP_BITS_2);
            break;
        default:
            DEBUGE("Invalid character");
            return false;
    }
    return true;
}

static void parse_gpio(cJSON* obj)
{
    cJSON* open_pins = cJSON_GetObjectItem(obj, "open");

    int size_open_pins = cJSON_GetArraySize("open");
    for(int i=0; i<size_open_pins; ++i)
    {
        cJSON* open_pin = cJSON_GetArrayItem(open_pins, i);
        if(!open_pin) break;
        gpio_set_direction(open_pin->valueint, GPIO_MODE_OUTPUT);
        gpio_set_level(open_pin->valueint, 1);
    }


    cJSON* close_pins = cJSON_GetObjectItem(obj, "close");

    int size_close_pins = cJSON_GetArraySize("close");
    for(int i=0; i<size_close_pins; ++i)
    {
        cJSON* close_pin = cJSON_GetArrayItem(close_pins, i);
        if(!close_pin) break;
        gpio_set_direction(close_pin->valueint, GPIO_MODE_OUTPUT);
        gpio_set_level(close_pin->valueint, 0);
    }

}


static bool parse_uart(cJSON* obj)
{
    cJSON* baudrate = cJSON_GetObjectItem(obj, "baudrate");
    if(!baudrate){
        DEBUGE("Baudrate parse err");
        return false;
    }
    uart_setBaud(UART_PORT_2, baudrate->valueint);

    cJSON* parity = cJSON_GetObjectItem(obj, "parity");
    if(!parity){
        DEBUGE("Parity parse err");
        return false;
    }
    check_parity(parity->valuestring[0]);

    cJSON* stop_bit = cJSON_GetObjectItem(obj, "stop_bit");
    if(!stop_bit){
        DEBUGE("Stop bit parse err");
        return false;
    }
    check_stop_bit(stop_bit->valuestring[0]);

    cJSON* data_bit = cJSON_GetObjectItem(obj, "data_bit");
    if(!stop_bit){
        DEBUGE("Data bit parse err");
        return false;
    }
    check_data_bit(data_bit->valuestring[0]);

    return true;
}

/* ----------------------------------------------------------------------- */
//                           GLOBAL FUNCTIONS                              //
/* ----------------------------------------------------------------------- */

/**
 * @brief This function parse the json file according to example json file(gpio.json) 
 * 
 * @param content Json file content
 * @param len Json file content length
 * @return true , if is succeed
 * @return false , if is failed
 */
bool parse_settings(char* content, int len)
{
    if(!content){
        DEBUGE("Content is null");
        return false;
    }

    cJSON *root = cJSON_Parse(content);
    if(!root){
        DEBUGE("Root is null");
        return false;
    }

    cJSON *gpio = cJSON_GetObjectItem(root, "gpio");
    if(!gpio){
        DEBUGE("Gpio is null");
        return false;
    }
    parse_gpio(gpio);

    cJSON *uart = cJSON_GetObjectItem(root, "uart");
    if(!uart){
        DEBUGE("Uart is null");
        return false;
    }
    parse_uart(uart);


    cJSON_Delete(root);

    return true;
}

/* ----------------------------------------------------------------------- */
//                               END OF CODE                               //
/* ----------------------------------------------------------------------- */