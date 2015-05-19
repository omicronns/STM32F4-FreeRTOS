/*
 * stdout_uart.c
 *
 *  Created on: May 19, 2015
 *      Author: omicronns
 */

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f4xx_usart.h"
#include "stdout_uart.h"

static QueueHandle_t g_oStdoutQueue;

int uart_init(void) {
    g_oStdoutQueue = xQueueCreate(UART_QUEUE_SIZE, sizeof(char));
    return (int)g_oStdoutQueue;
}

int uart_schedule(char c) {
    return xQueueSend(g_oStdoutQueue, &c, 0) == pdTRUE ? 1 : 0;
}

void stdout_uart_task(void *p) {
    for(;g_oStdoutQueue;) {
        char c;
        if(xQueueReceive(g_oStdoutQueue, &c, 1) == pdTRUE) {
            while (!(USART3->SR & 0x00000040)) {
                vTaskDelay(1);
            }
            USART_SendData(USART3, c);
        }
    }
    vTaskDelete(NULL);
}
