# Description

This project creates a tcp sever on esp32 and this server push the recieved data from tcp client over the uart port. Also if recieved data include in the configuration json file, server performs this settings. This settings includes the some gpio pin level changes or uart setting. Json file attached the below as an example.

json file :
{

    "gpio":{
        "open":{
            "pin_num":[5,6,7]
        },
        "close":{
            "pin_num":[8,9,10]
        } 
    },
    "uart":{
        "baudrate": 115200,
        "parity":"none",
        "stop_bit":1,
        "data_bit":8
    }}

#### open object : This object includes the will set the gpio level high <br/>
#### close object : This object includes the will set the gpio level low <br/>
#### uart object : This object includes the will perform uart settings <br/>

## How to use example
To connect wifi : You should go to componenets/periphs/inc/wifi.h and the changes the WIFI_SSID WIFI_PWD defines according to your wifi network ssid and password.

You can use the telnet to connect and command is attached the below.

Telnet "ip address" "port number"
For example : Telnet 192.168.1.102 3333

Note: If you don't want to see debug message you should changes the DEBUG define in the /componenets/periphs/inc/common.h .

## Project folder contents.

Below is short explanation of remaining files in the project folder.

```
├── Components
│   ├──Periphs
│      ├──src
│      ├──inc
│   ├──CMakeLists.txt
│   ├──tcp_server
│      ├──src
│      ├──inc
│   ├──CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
├── CMakeLists.txt
└── README.md                  This is the file you are currently reading
```
