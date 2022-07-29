#include "softTimer.h"

//
// soft timers array
//
typedef struct {
    bool            IsRunning;      // timer calisma durumu
    volatile bool   Timeout;        // timer tamamlanma durumu
    uint32_t        SetTime;        // timer suresi
    uint32_t        Time;           // aktif sure
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

    //
    // !! !! !!
    //
    // burada islemcinin herhangi bir timer arabirimi 1ms'de bir kesme �retecek sekilde init edilecek
    // bu kesme fonksiyonu (ISR) icerisinde SoftTimer_ISR fonskiyonu cagirilacak
    //
    // !! !! !!
    //
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
