#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "math.h"
#include "stdio.h"
#include "stm32f4xx_usart.h"
#include "stdout_uart.h"

void init_USART3(void);
void test_FPU_test(void* p);

int main(void) {
    uint8_t ret0, ret1;

    SystemInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    init_USART3();
    uart_init();

    // Create a task
    ret0 = xTaskCreate(test_FPU_test, "FPU", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL);
    ret1 = xTaskCreate(stdout_uart_task, "stdout", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL);

    if(ret0 == pdTRUE && ret1 == pdTRUE) {
        printf("System Started!\n");
        vTaskStartScheduler();  // should never return
    }
    else {
        printf("System Error!\n");
        // --TODO blink some LEDs to indicate fatal system error
    }

    for(;;);
}

void test_FPU_test(void* p) {
    int iter = 0;
    printf("Start FPU test task.\n");
    for(;;) {
        volatile float flt = sinf((float)iter);
        printf("Iteration:%d sin:%f\n", iter++, flt);
        vTaskDelay(1000);
    }
    vTaskDelete(NULL);
}

/*
 * Configure USART3(PB10, PB11) to redirect printf data to host PC.
 */
void init_USART3(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART3, &USART_InitStruct);
    USART_Cmd(USART3, ENABLE);
}

