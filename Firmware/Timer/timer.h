#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <ti/drivers/dpl/ClockP.h>

void Timer_initPeriodic(uint32_t periodTicks, ClockP_Fxn cb, uintptr_t arg);
void Timer_start(void);
void Timer_stop(void);
void Timer_restart(void);

#endif // TIMER_H
