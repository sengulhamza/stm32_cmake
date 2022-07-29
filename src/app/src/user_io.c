#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "app/app_tasks.h"
#include "app/user_io.h"
#include "drv/io.h"
#include "drv/softTimer.h"

static TaskHandle_t s_user_io_task;
static TimerHandle_t s_user_io_timer[TIMER_COUNT];

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

static void user_io_timer_cb(TimerHandle_t xTimer)
{
    uint32_t timer_id = (uint32_t)pvTimerGetTimerID(xTimer);
    timer_id = timer_id;
    SoftTimer_ISR();
}

void user_io_timer_init(uint8_t timer_no)
{
    s_user_io_timer[timer_no] = xTimerCreate(
                                    "s_user_io_timer",
                                    pdMS_TO_TICKS(1),
                                    pdTRUE,                                          // Auto-reload
                                    (void *)(int)timer_no,                             // Timer ID
                                    user_io_timer_cb);                                // Callback function
    xTimerStart(s_user_io_timer[timer_no], portMAX_DELAY);
}

void user_io_process_start(void)
{
    user_io_timer_init(TIMER_A);
    user_io_timer_init(TIMER_B);
    SoftTimer_Init();
    SoftTimer_SetTimer(TIMER_A, 10);
    SoftTimer_SetTimer(TIMER_B, 20);

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
