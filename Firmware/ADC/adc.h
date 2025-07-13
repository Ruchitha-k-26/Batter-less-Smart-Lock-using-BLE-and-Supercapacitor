#ifndef ADC_H
#define ADC_H

#include <stdint.h>

extern uint16_t adcValue;
extern uint32_t adcValueMicroVolt;

void ADC(void);
void adc_init(void);

// New function for voltage-based control
void ADC_processVoltageAndControlLock(void);

#endif
