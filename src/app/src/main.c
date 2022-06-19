#include <stdlib.h>
#include "stm32f4xx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "drv/io.h"
#include "app/user_io.h"

void vApplicationTickHook(void)
{

}

void vApplicationIdleHook(void)
{

}

int main(void)
{
    HAL_Init();
    io_init();
    user_io_process_start();
    vTaskStartScheduler();

    while (pdTRUE) {
    }
}
