/*
 * stdout_uart.h
 *
 *  Created on: May 19, 2015
 *      Author: omicronns
 */

#ifndef stdout_uart_h
#define stdout_uart_h

#define UART_QUEUE_SIZE     1024

#ifdef __cplusplus
extern "C" {
#endif

int uart_init(void);
int uart_schedule(char c);
void stdout_uart_task(void *p);

#ifdef __cplusplus
}
#endif

#endif /* stdout_uart_h */
