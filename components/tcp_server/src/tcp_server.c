/* ----------------------------------------------------------------------- */
//                              INCLUDE FILES                              //
/* ----------------------------------------------------------------------- */

#include <string.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/errno.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "uart.h"
#include "common.h"

/* ----------------------------------------------------------------------- */
//                               DEFINES                                   //
/* ----------------------------------------------------------------------- */

#define KEEPALIVE_IDLE              5
#define KEEPALIVE_INTERVAL          10
#define KEEPALIVE_COUNT             3

#define SOCKET_TCP                  SOCK_STREAM
#define RX_BUFFER_SIZE              512

#define MAX_CLIENT_COUNT            2
#define TIMEOUT_VAL                 1000 * 1

/* ----------------------------------------------------------------------- */
//                           GLOBAL VARIABLE                               //
/* ----------------------------------------------------------------------- */

int tcp_port = 3333;

/* ----------------------------------------------------------------------- */
//                          PRIVATE FUNCTIONS                              //
/* ----------------------------------------------------------------------- */

static bool socket_close(int sock_fd)
{
    if(close(sock_fd) != 0){
        DEBUGE("Failed to close socket");
        return false;
    }
        
    DEBUGI("Socket close is succeed");

    return true;
}

static void recieve_data(const int sock, int uart_fd)
{
    int  len, rc;
    char rx_buffer[RX_BUFFER_SIZE];
    struct timeval timeout;
    fd_set read_fds;

    while(1)
    {
        FD_ZERO(&read_fds);
        FD_SET(sock, &read_fds);

        timeout.tv_usec = 100*1000;
        timeout.tv_sec = 0;

        rc = select(sock + 1, &read_fds, NULL, NULL, &timeout);
        if(rc == -1){
            //TODO
            //DEBUGI("RC = %d", rc);
            DEBUGE("RC errno =%d", errno);
        }else if(rc == 0){
            //TODO
            //DEBUGI("RC = %d", rc);
        }else{
            DEBUGI("MAX FD = %d", sock);
            //      Socket Activity
            if(FD_ISSET(sock, &read_fds))
            {
                len = recv(sock, rx_buffer, sizeof(rx_buffer)-1, 0);
                DEBUGI("Socket read length = %d", len);
                if (len <= 0) {
                    DEBUGE("Error occurred during receiving: errno %d", errno);
                    //socket_close(sock);
                    break;
                }

                if(parse_settings(rx_buffer, len) != true){
                    DEBUGE("Rx buffer parse err or irrelevant content");
                }

                rx_buffer[len] = '\0';
                uart_write(uart_fd, rx_buffer, len);
                printf("RX buffer = %s\r\n", rx_buffer);
            }

        }

    }
    
}

void tcp_serverTask(void* args)
{
    int socket_port = tcp_port;
    int keepAlive = 1;
    int keepIdle = KEEPALIVE_IDLE;
    int keepInterval = KEEPALIVE_INTERVAL;
    int keepCount = KEEPALIVE_COUNT;
    int client_sock;

    struct sockaddr_storage dest_addr;

    struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
    dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr_ip4->sin_family = AF_INET;
    dest_addr_ip4->sin_port = htons(socket_port);

    // Craete a socket
    int listen_sock = socket(AF_INET, SOCKET_TCP, IPPROTO_IP);
    if (listen_sock < 0) {
        DEBUGE("Unable to create socket: errno %d", errno);
        goto err;
    }
    
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind socket
    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        DEBUGE("Socket unable to bind: errno %d", errno);
        goto err;
    }

    // Listen socket
    err = listen(listen_sock, MAX_CLIENT_COUNT);
    if (err < 0) {
        DEBUGE("Error occurred during listen: errno %d", errno);
        goto err;
    }

    while(1)
    {
        // Accept connection request
        struct sockaddr_storage source_addr;
        socklen_t addr_len = sizeof(source_addr);
        client_sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (client_sock < 0) {
            DEBUGE("Unable to accept connection: errno %d", errno);
            goto err;
        }else{
            DEBUGI("Socket listening || client sock = %d", client_sock);
            setsockopt(client_sock,  SOL_SOCKET,  SO_KEEPALIVE,  &keepAlive,    sizeof(int));
            setsockopt(client_sock,  IPPROTO_TCP, TCP_KEEPIDLE,  &keepIdle,     sizeof(int));
            setsockopt(client_sock,  IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
            setsockopt(client_sock,  IPPROTO_TCP, TCP_KEEPCNT,   &keepCount,    sizeof(int));
            
            // Recieve data data proccess
            recieve_data(client_sock, UART_PORT_2);
            socket_close(client_sock);
        }  
    }
err:
    DEBUGE("Socket is closed");
    close(listen_sock);
}

/* ----------------------------------------------------------------------- */
//                           GLOBAL FUNCTIONS                              //
/* ----------------------------------------------------------------------- */

/**
 * @brief This function provide to start tcp server task
 * 
 */
void start_tcpServer(void)
{
    xTaskCreate(tcp_serverTask, "tcp_server", 1024*5, NULL, 5, NULL);

}

/* ----------------------------------------------------------------------- */
//                               END OF CODE                               //
/* ----------------------------------------------------------------------- */