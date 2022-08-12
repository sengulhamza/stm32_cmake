#include "soft_timer.h"

typedef struct {
    bool            IsRunning;
    volatile bool   Timeout;
    uint32_t        SetTime;
    uint32_t        Time;
} STimerType;

static STimerType SoftTimers[TIMER_COUNT];

//
// Server functions
//
void SoftTimer_Init(void)
{
    uint8_t i;

    for (i = 0; i < TIMER_COUNT; i++) {
        SoftTimers[i].IsRunning = false;
        SoftTimers[i].Time = 0;
        SoftTimers[i].Timeout = false;
    }
}

void SoftTimer_SetTimer(uint8_t TimerNo, uint32_t SetTimeMs)
{
    if (TimerNo < TIMER_COUNT) {
        SoftTimers[TimerNo].Time        = 0;
        SoftTimers[TimerNo].SetTime     = SetTimeMs;
        SoftTimers[TimerNo].IsRunning   = true;
        SoftTimers[TimerNo].Timeout     = false;
    }
}

bool SoftTimer_GetTimerStatus(uint8_t TimerNo)
{
    return (SoftTimers[TimerNo].Timeout);
}

void SoftTimer_ResetTimer(uint8_t TimerNo)
{
    SoftTimers[TimerNo].IsRunning = false;
    SoftTimers[TimerNo].Time = 0;
    SoftTimers[TimerNo].Timeout = false;
}

void SoftTimer_ISR(void)
{
    for (int i = 0; i < TIMER_COUNT ; i++) {
        if (SoftTimers[i].IsRunning) {
            if (SoftTimers[i].SetTime <= SoftTimers[i].Time) {
                SoftTimers[i].Timeout = true;
            } else {
                SoftTimers[i].Time++;
            }
        } else {
            SoftTimers[i].Timeout = false;
            SoftTimers[i].Time = 0;
        }
    }
}
