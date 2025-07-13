#include <stdint.h>
#include <stddef.h>
#include "ti/drivers/ADC.h"
#include "ti_drivers_config.h"
#include "ti/drivers/GPIO.h"

#include "adc.h"

uint16_t adcValue;
uint32_t adcValueMicroVolt;

static ADC_Handle adc;
static ADC_Params params;

int_fast16_t res;

void adc_init(void)
{
    ADC_init();
}

void ADC(void)
{
    GPIO_write(Battery_enable, 1);
    ADC_Params_init(&params);
    adc = ADC_open(CONFIG_ADC_0, &params);

    if (adc == NULL)
    {
        GPIO_write(Battery_enable, 0);
        return;
    }

    res = ADC_convert(adc, &adcValue);

    if (res == ADC_STATUS_SUCCESS)
    {
        adcValueMicroVolt = ADC_convertRawToMicroVolts(adc, adcValue);
    }

    ADC_close(adc);
    GPIO_write(Battery_enable, 0);
}

// New function to control lock based on voltage level
void ADC_processVoltageAndControlLock(void)
{
    ADC();  // Get latest voltage
    uint16_t voltage = adcValueMicroVolt / 1000;  // in millivolts

    if (voltage < 5500)
    {
        GPIO_write(Lock_Drive1, 0);  // Turn OFF the lock
    }
    else if (voltage >= 6000)
    {
        GPIO_write(Lock_Drive1, 1);  // Turn ON the lock
    }
}
