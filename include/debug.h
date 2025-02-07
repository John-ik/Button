#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "main.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

uint8_t debug_uart_buf[64];


#define printf(format, data) \
    sprintf(debug_uart_buf, format, data); \
    HAL_UART_Transmit(&huart1, debug_uart_buf, strlen(debug_uart_buf), 100) 


#define print(str) printf("%s", str)

#define println(str) printf("%s\n", str)

#endif
