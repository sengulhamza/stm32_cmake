#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "app/app_tasks.h"
#include "app/user_io.h"
#include "drv/io.h"

static TaskHandle_t s_user_io_task;

static void user_io_task(void *params)
{
    (void)(params);
    leds_e led_num = LED_1;
    while (pdTRUE) {
        io_toggle_leds(led_num);
        if (led_num < LED_MAX - 1) {
            led_num++;
        } else {
            led_num = LED_1;
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void user_io_process_start(void)
{
    xTaskCreate(user_io_task,
                APP_USER_IO_TASK_NAME,
                APP_USER_IO_TASK_STACK,
                NULL,
                APP_USER_IO_TASK_PRIO,
                &s_user_io_task);
}

void user_io_task_suspend()
{
    if (s_user_io_task) {
        vTaskSuspend(s_user_io_task);
    }
}
