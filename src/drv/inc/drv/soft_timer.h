#ifndef SOFT_TIMER_H
#define SOFT_TIMER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TIMER_A,
    TIMER_B,
    TIMER_MAX
} timer_num_e;

#define TIMER_COUNT         TIMER_MAX

void SoftTimer_Init(void);
void SoftTimer_ISR(void);
void SoftTimer_SetTimer(uint8_t TimerNo, uint32_t SetTimeMs);
bool SoftTimer_GetTimerStatus(uint8_t TimerNo);
void SoftTimer_ResetTimer(uint8_t TimerNo);

#endif
