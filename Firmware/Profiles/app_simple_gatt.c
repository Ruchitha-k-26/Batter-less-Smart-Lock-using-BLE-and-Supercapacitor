/******************************************************************************

@file  app_simple_gatt.c

@brief This file contains the Simple GATT application functionality

Group: WCS, BTS
Target Device: cc23xx

******************************************************************************/

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <ti/bleapp/menu_module/menu_module.h>
#include <app_main.h>
#include <simple_gatt_profile.h>
#include "unlock.h"
#include "checksum.h"
#include "adc.h"
#include "nvs.h"
#include <ti/drivers/NVS.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include "timer.h"
#include <ti/bleapp/ble_app_util/inc/bleapputil_api.h>
//#include "bleapputil_api.h"
#define UNLOCK_CODE 0x09 // 4-bit unlock code

static NVS_Handle nvsHandle;
static void SimpleGatt_changeCB(uint8_t paramId);
static void SimpleGatt_processVoltage(void);

static simpleProfileCBs_t simple_profileCBs = {
    SimpleGatt_changeCB
};

static void SimpleGatt_timerISR(uintptr_t arg)
{
    BLEAppUtil_invokeFunctionNoData((void*)SimpleGatt_processVoltage);
}

static void SimpleGatt_processVoltage(void)
{
    ADC_processVoltageAndControlLock();
    Unlock_handleTimeout(nvsHandle);
    Timer_restart();
}

static void SimpleGatt_changeCB(uint8_t paramId)
{
    uint8_t newValue = {0};

    switch (paramId)
    {
    case SIMPLEPROFILE_CHAR1:
        SimpleProfile_getParameter(SIMPLEPROFILE_CHAR1, &newValue);
        SimpleProfile_setParameter(SIMPLEPROFILE_CHAR4, sizeof(uint8_t), &newValue);
        break;

    case SIMPLEPROFILE_CHAR3:
        SimpleProfile_getParameter(SIMPLEPROFILE_CHAR3, &newValue);
        SimpleProfile_setParameter(SIMPLEPROFILE_CHAR4, sizeof(uint8_t), &newValue);
        break;

    case SIMPLEPROFILE_CHAR5:
    /*{
        uint8_t receivedData[SIMPLEPROFILE_CHAR5_LEN];
        SimpleProfile_getParameter(SIMPLEPROFILE_CHAR5, &receivedData);

        if (validateChecksum(receivedData, SIMPLEPROFILE_CHAR5_LEN))
              {
                  performUnlockAction(nvsHandle);
                  uint8_t msg[] = "unlocked";
                  SimpleProfile_setParameter(SIMPLEPROFILE_CHAR4, sizeof(msg), msg);
                  Unlock_resetTimeout();
              }
              else
              {
                  uint8_t failMsg[] = "FAILED!!";
                  SimpleProfile_setParameter(SIMPLEPROFILE_CHAR4, sizeof(failMsg), failMsg);
              }
          }*/
    {
           uint8_t pinBuf[PIN_LENGTH];
           SimpleProfile_getParameter(SIMPLEPROFILE_CHAR5, pinBuf);

           if (nvs_verify_pin((char *)pinBuf))
           {
               performUnlockAction(nvsHandle);
               uint8_t msg[] = "unlocked";
               SimpleProfile_setParameter(SIMPLEPROFILE_CHAR4, sizeof(msg), msg);
               Unlock_resetTimeout();
           }
           else
           {
               uint8_t msg[] = "PIN FAIL";
               SimpleProfile_setParameter(SIMPLEPROFILE_CHAR4, sizeof(msg), msg);
           }
       }




    break;
    }
}

bStatus_t SimpleGatt_start(void)
{
    bStatus_t status = SUCCESS;

    status = SimpleProfile_AddService();
    if (status != SUCCESS)
    {
        return status;
    }

    uint8_t charValue1 = 1;
    uint8_t charValue2 = 2;
    uint8_t charValue3 = 3;
    uint8_t charValue4 = 4;
    uint8_t charValue5[SIMPLEPROFILE_CHAR5_LEN] = {1, 2, 3, 4, 5};

    SimpleProfile_setParameter(SIMPLEPROFILE_CHAR1, sizeof(uint8_t), &charValue1);
    SimpleProfile_setParameter(SIMPLEPROFILE_CHAR2, sizeof(uint8_t), &charValue2);
    SimpleProfile_setParameter(SIMPLEPROFILE_CHAR3, sizeof(uint8_t), &charValue3);
    SimpleProfile_setParameter(SIMPLEPROFILE_CHAR4, sizeof(uint8_t), &charValue4);
    SimpleProfile_setParameter(SIMPLEPROFILE_CHAR5, SIMPLEPROFILE_CHAR5_LEN, charValue5);

    status = SimpleProfile_RegisterAppCBs(&simple_profileCBs);

    adc_init();
    unLockInit();
    nvsHandle = nvs_init();

   // BLEAppUtil_registerFunction(SimpleGatt_processVoltage);

    Timer_initPeriodic(1000000, SimpleGatt_timerISR, 0);
    Timer_start();

    return status;
}

void SimpleGatt_notifyChar4()
{
    uint8_t value;
    if (SimpleProfile_getParameter(SIMPLEPROFILE_CHAR3, &value) == SUCCESS)
    {
        SimpleProfile_setParameter(SIMPLEPROFILE_CHAR4, sizeof(uint8_t), &value);
    }
}
