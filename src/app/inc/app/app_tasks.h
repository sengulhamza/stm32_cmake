#ifndef _APP_TASK_H_
#define _APP_TASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"

#define APP_RTOS_HEAP_SIZE          (configTOTAL_HEAP_SIZE)
#define APP_TASK_PRIO_MAX           (configMAX_PRIORITIES)
#define APP_TASK_PRIO_MIN           (0)
#define APP_TASK_MIN_STACK          (configMINIMAL_STACK_SIZE)
#define APP_TASK_STACK_TYPE         portSTACK_TYPE

/* freertos internal tasks */
#define APP_RTOS_IDLE_TASK_PRIO     (APP_TASK_PRIO_MIN)
#define APP_RTOS_IDLE_TASK_STACK    (APP_TASK_MIN_STACK)

/* sem application tasks */
#define APP_USER_IO_TASK_PRIO        (APP_TASK_PRIO_MIN + 1)
#define APP_USER_IO_TASK_STACK       (APP_TASK_MIN_STACK)
#define APP_USER_IO_TASK_NAME        "userio"

#define APP_TASK_SIZE(a)            (sizeof(APP_TASK_STACK_TYPE)*(a))

#endif
