#ifndef __COMMON__
#define __COMMON__

/* ----------------------------------------------------------------------- */
//                              INCLUDE FILES                              //
/* ----------------------------------------------------------------------- */

#include <stdbool.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* ----------------------------------------------------------------------- */
//                               DEFINES                                   //
/* ----------------------------------------------------------------------- */

#define DEBUG 1

#define DEBUGE(fmt, ...)\
        do{ if(DEBUG){ printf("\033[0;31m" "ERROR: %s:%d:%s(): " #fmt "\r\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__);} }while (false);

#define DEBUGI(fmt, ...)\
        do{ if(DEBUG) {printf("\033[0;37m" "INFO: %s:%d:%s(): " #fmt "\r\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__);} }while (false);

#define ERROR_CHECK(x, error_msg)    \
{                                    \
    int error = (x);                 \
    if(error == -1){                 \
        printf("%s\r\n", error_msg); \
        return false;                \
    }                                \
}

#define SLEEP(ms)                   (vTaskDelay(ms / portTICK_PERIOD_MS))

/* ----------------------------------------------------------------------- */
//                           TYPE DEFINATIONS                              //
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
//                          FUNCTION PROTOTYPE                             //
/* ----------------------------------------------------------------------- */

#endif /* __COMMON__ */

/* ----------------------------------------------------------------------- */
//                               END OF CODE                               //
/* ----------------------------------------------------------------------- */