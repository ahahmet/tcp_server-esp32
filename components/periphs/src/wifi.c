/* ----------------------------------------------------------------------- */
//                              INCLUDE FILES                              //
/* ----------------------------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include <esp_err.h>
#include <wifi.h>

#include "common.h"

/* ----------------------------------------------------------------------- */
//                               DEFINES                                   //
/* ----------------------------------------------------------------------- */

#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

/* ----------------------------------------------------------------------- */
//                           GLOBAL VARIABLE                               //
/* ----------------------------------------------------------------------- */

static EventGroupHandle_t s_wifi_event_group;

/* ----------------------------------------------------------------------- */
//                          PRIVATE FUNCTIONS                              //
/* ----------------------------------------------------------------------- */

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    DEBUGI("Wifi event id = %d", event_id);
    if((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_START)){
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
        while (esp_wifi_connect() == ESP_FAIL){
            DEBUGE("retry to connect to the AP");
            SLEEP(1000);
        }
        

        DEBUGI("connect to the AP fail");
    }
    else if((event_base == IP_EVENT) && (event_id == IP_EVENT_STA_GOT_IP)){
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        DEBUGI("got ip: %d.%d.%d.%d", IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }else if((event_base == IP_EVENT) && (event_id == IP_EVENT_STA_LOST_IP)){
        DEBUGE("Losted device wifi sta ip");
    }else{
        DEBUGE("Unknown event from wifi");
    }
}

static bool set_wifiEvent(void)
{
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    s_wifi_event_group = xEventGroupCreate();

    ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                                    &event_handler, NULL,
                                                    &instance_any_id),
                                                    "event register error");

    ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                                    &event_handler, NULL,
                                                    &instance_got_ip),
                                                    "event register error");
    return true;
}

static bool wifi_initSTA(void)
{
    esp_netif_t *wifi_netif = esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_init = WIFI_INIT_CONFIG_DEFAULT();
    ERROR_CHECK(esp_wifi_init(&wifi_init), "WIFI init error");

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PWD,
            .threshold.authmode = WIFI_AUTH_OPEN,
        },
    }; 

    ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA), "WIFI set mode error");
    ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config), "WIFI set error");

    if(esp_netif_dhcps_start(wifi_netif) != ESP_OK){
        DEBUGE("DHCP start err");
    }

    ERROR_CHECK(esp_wifi_start(), "WIFI start error");
    
    return true;
}

static bool wifi_initAP(void)
{
    esp_netif_t *wifi_netif =esp_netif_create_default_wifi_ap();
    wifi_init_config_t wifi_init = WIFI_INIT_CONFIG_DEFAULT();

    ERROR_CHECK(esp_wifi_init(&wifi_init), "WIFI init error");

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .password = WIFI_PWD,
            .max_connection = MAX_STA_CONNECTION,
            .authmode = WIFI_AUTH_OPEN,
        },
    };

    ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP), "WIFI set mode error");
    ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config), "WIFI set error");
    
    if(esp_netif_dhcps_start(wifi_netif) != ESP_OK){
        DEBUGE("DHCP start err");
    }

    ERROR_CHECK(esp_wifi_start(), "WIFI start error");

    return true;
}

/* ----------------------------------------------------------------------- */
//                           GLOBAL FUNCTIONS                              //
/* ----------------------------------------------------------------------- */

/**
 * @brief This function initialize the wifi 
 * 
 * @param mod Wifi working mod (sta or ap), wifi_sta or wifi_ap enums
 * @return true , if is succeed
 * @return false , if is failed
 */
bool wifi_init(int mod)
{
    set_wifiEvent();

    if(mod == wifi_ap){
        wifi_initAP();
        DEBUGI("wifi works on ap mode.");
    }
    else if(mod == wifi_sta){
        wifi_initSTA();
        DEBUGI("wifi works on sta mode.");
    }
    else{
        DEBUGE("WIFI mode is invalid");
        return false;
    }

    DEBUGI("wifi_init finished.");

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT){
        DEBUGI("WIFI Connection is successed");
    }
    else if (bits & WIFI_FAIL_BIT){
        DEBUGE("WIFI Connection is failed");
        return false;
    }
    else{
        DEBUGE("UNEXPECTED EVENT");
        return false;
    }
    DEBUGI("successed");

    return true;
}

/* ----------------------------------------------------------------------- */
//                               END OF CODE                               //
/* ----------------------------------------------------------------------- */