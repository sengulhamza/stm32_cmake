#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "app/app_tasks.h"
#include "app/user_io.h"
#include "drv/io.h"
#include "drv/soft_timer.h"

#define BUTTON_DEBOUNCE_DELAY       100
#define BUTTON_MODE_1_INTERVAL      BUTTON_DEBOUNCE_DELAY
#define BUTTON_MODE_2_INTERVAL      3000
#define LED_TIMER_A_INTERVAL        1000
#define LED_TIMER_B_INTERVAL        2000

typedef enum {
    ON_1,
    OFF_1,
    ON_2,
    OFF_2
} user_leds_cases_e;

typedef enum {
    LED_MODE_1 = 2,
    LED_MODE_2 = 4
} user_led_mode_e;

typedef struct {
    user_leds_cases_e led_case;
    user_led_mode_e led_mode;
    uint8_t shot_cnt;
} user_led_case_control_t;

typedef enum {
    BUTTON_FSM_IDLE = 0,
    BUTTON_FSM_GET_START_TS,
    BUTTON_FSM_WAIT_RELEASE,
    BUTTON_FSM_GET_END_TS,
    BUTTON_FSM_DO_HANDLE,
} button_fsm_steps_t;

static user_led_case_control_t s_case_control = {.led_case = ON_1, .led_mode = LED_MODE_1, .shot_cnt = 0};
static TaskHandle_t s_user_io_task, s_user_io_button_task;
static TimerHandle_t s_user_io_timer;

static void user_io_update_case_params(user_led_mode_e next_mode)
{
    if (next_mode == s_case_control.led_mode) {
        return;
    }
    s_case_control.led_mode = next_mode;
    s_case_control.led_case = ON_1;
    s_case_control.shot_cnt = 0;
    SoftTimer_ResetTimer(TIMER_A);
    SoftTimer_ResetTimer(TIMER_B);
    SoftTimer_SetTimer(TIMER_A, LED_TIMER_A_INTERVAL);
    SoftTimer_SetTimer(TIMER_B, LED_TIMER_B_INTERVAL);
    io_toggle_leds(LED_2);                      //TODO Remova later. For observing while switching mode. - led2 will toggle -
}

static void user_io_task(void *params)
{
    (void)(params);
    while (pdTRUE) {
        switch (s_case_control.led_case) {
        case ON_1:
            if (SoftTimer_GetTimerStatus(TIMER_A)) {
                io_set_led(LED_1);
                SoftTimer_SetTimer(TIMER_A, LED_TIMER_A_INTERVAL);
                s_case_control.led_case = OFF_1;
            }
            break;
        case OFF_1:
            if (SoftTimer_GetTimerStatus(TIMER_A)) {
                io_reset_led(LED_1);
                SoftTimer_SetTimer(TIMER_A, LED_TIMER_A_INTERVAL);
                if (++s_case_control.shot_cnt == s_case_control.led_mode) {
                    SoftTimer_SetTimer(TIMER_B, LED_TIMER_A_INTERVAL);
                    s_case_control.led_case = ON_2;
                    s_case_control.shot_cnt = 0;
                    break;
                }
                s_case_control.led_case = ON_1;
            }
            break;
        case ON_2:
            if (SoftTimer_GetTimerStatus(TIMER_B)) {
                io_set_led(LED_1);
                SoftTimer_SetTimer(TIMER_B, LED_TIMER_B_INTERVAL);
                s_case_control.led_case = OFF_2;
            }
            break;
        case OFF_2:
            if (SoftTimer_GetTimerStatus(TIMER_B)) {
                io_reset_led(LED_1);
                SoftTimer_SetTimer(TIMER_B, LED_TIMER_B_INTERVAL);
                if (++s_case_control.shot_cnt == s_case_control.led_mode) {
                    SoftTimer_SetTimer(TIMER_A, LED_TIMER_B_INTERVAL);
                    s_case_control.led_case = ON_1;
                    s_case_control.shot_cnt = 0;
                    break;
                }
                s_case_control.led_case = ON_2;
            }
            break;
        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

static void user_io_button_task(void *params)
{
    (void)(params);
    uint32_t start_ts = 0, end_ts = 0;
    uint8_t  fsm_step = BUTTON_FSM_IDLE;
    while (pdTRUE) {
        uint8_t buton_pos = io_read_input(BUTTON_1);
        switch (fsm_step) {
        case BUTTON_FSM_IDLE:
            if (buton_pos) {
                fsm_step = BUTTON_FSM_GET_START_TS;
            }
            break;
        case BUTTON_FSM_GET_START_TS:
            start_ts = xTaskGetTickCount() * portTICK_RATE_MS;
            fsm_step = BUTTON_FSM_WAIT_RELEASE;
            break;
        case BUTTON_FSM_WAIT_RELEASE:
            if (!buton_pos) {
                fsm_step = BUTTON_FSM_GET_END_TS;
            }
            break;
        case BUTTON_FSM_GET_END_TS:
            end_ts = xTaskGetTickCount() * portTICK_RATE_MS;
            fsm_step = BUTTON_FSM_DO_HANDLE;
            break;
        case BUTTON_FSM_DO_HANDLE:
            if (end_ts - start_ts > BUTTON_MODE_2_INTERVAL) {
                user_io_update_case_params(LED_MODE_2);
            } else if (end_ts - start_ts > BUTTON_MODE_1_INTERVAL) {
                user_io_update_case_params(LED_MODE_1);
            }
            fsm_step = BUTTON_FSM_IDLE;
            break;
        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_DELAY));
    }
}

static void user_io_timer_cb(TimerHandle_t xTimer)
{
    (void)xTimer;
    SoftTimer_ISR();
}

void user_io_timer_init(void)
{
    s_user_io_timer = xTimerCreate(
                          "s_user_io_timer",
                          pdMS_TO_TICKS(1),                                // Timer period
                          pdTRUE,                                          // Auto-reload
                          NULL,                                            // Timer ID
                          user_io_timer_cb);                               // Callback function
    xTimerStart(s_user_io_timer, portMAX_DELAY);
}

void user_io_process_start(void)
{
    io_init();
    user_io_timer_init();
    SoftTimer_Init();
    SoftTimer_SetTimer(TIMER_A, LED_TIMER_A_INTERVAL);
    SoftTimer_SetTimer(TIMER_B, LED_TIMER_B_INTERVAL);

    xTaskCreate(user_io_task,
                APP_USER_IO_TASK_NAME,
                APP_USER_IO_TASK_STACK,
                NULL,
                APP_USER_IO_TASK_PRIO,
                &s_user_io_task);
    xTaskCreate(user_io_button_task,
                APP_USER_IO_TASK_NAME,
                APP_USER_IO_TASK_STACK,
                NULL,
                APP_USER_IO_TASK_PRIO,
                &s_user_io_button_task);
}
