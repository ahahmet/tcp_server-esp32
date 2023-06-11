/*

    Author: Ahmet Aktaş
    Date: 11.06.2023

    Project name: TCP Server Demo 

*/

/* ----------------------------------------------------------------------- */
//                              INCLUDE FILES                              //
/* ----------------------------------------------------------------------- */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <esp_event.h>
#include <esp_netif.h>
#include <esp_err.h>
#include <nvs_flash.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "tcp_server.h"
#include "wifi.h"
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

static bool board_init()
{
    if(nvs_flash_init() != ESP_OK){
        DEBUGE("Flash init err");
    }

    if(esp_netif_init() != ESP_OK){
        DEBUGE("Netif init err");
    }

    if(esp_event_loop_create_default() != ESP_OK){
        DEBUGE("Event loop init err");
    }

    if(!wifi_init(wifi_sta)){
        DEBUGE("Wifi init err");
        return false;
    }

    if(!uart_init(UART_PORT_2)){
        DEBUGE("UART init err");
        return false;
    } 

    return true;
}

/* ----------------------------------------------------------------------- */
//                           GLOBAL FUNCTIONS                              //
/* ----------------------------------------------------------------------- */

void app_main(void)
{

    if(!board_init()){
        exit(0);
    }

    start_tcpServer();

    while(1)
    {
        SLEEP(1000);
    }

}

/* ----------------------------------------------------------------------- */
//                               END OF CODE                               //
/* ----------------------------------------------------------------------- */