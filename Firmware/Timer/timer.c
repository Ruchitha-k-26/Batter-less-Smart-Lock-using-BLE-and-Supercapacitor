// timer.c

#include "timer.h"
#include "ti/drivers/dpl/ClockP.h"

static ClockP_Struct periodicTimer;
static ClockP_Params clockParams;

static ClockP_Fxn g_isrCallback = NULL;
static uintptr_t g_callbackArg = 0;
static uint32_t g_periodTicks = 0;

// ISR that gets called by ClockP
static void timerISR(uintptr_t arg)
{
    if (g_isrCallback)
    {
        g_isrCallback(g_callbackArg);
    }

    // Restart timer for periodic behavior
    ClockP_setTimeout(ClockP_handle(&periodicTimer), g_periodTicks);
    ClockP_start(ClockP_handle(&periodicTimer));
}

void Timer_initPeriodic(uint32_t periodMicroseconds, ClockP_Fxn isrCallback, uintptr_t arg)
{
    ClockP_Params_init(&clockParams);
    clockParams.period = 0; // one-shot, we restart manually
    clockParams.startFlag = false;
    clockParams.arg = (uintptr_t)NULL;

    g_isrCallback = isrCallback;
    g_callbackArg = arg;

    uint32_t tickPeriod = ClockP_getSystemTickPeriod(); // in microseconds
    g_periodTicks = periodMicroseconds / tickPeriod;

    ClockP_construct(&periodicTimer, timerISR, g_periodTicks, &clockParams);
}

void Timer_start(void)
{
    ClockP_start(ClockP_handle(&periodicTimer));
}

void Timer_stop(void)
{
    ClockP_stop(ClockP_handle(&periodicTimer));
}

void Timer_restart(void)
{
    ClockP_stop(ClockP_handle(&periodicTimer));
    ClockP_setTimeout(ClockP_handle(&periodicTimer), g_periodTicks);
    ClockP_start(ClockP_handle(&periodicTimer));
}
