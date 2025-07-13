#include "unlock.h"
#include <ti/drivers/GPIO.h>
#include <ti/drivers/NVS.h>
#include <ti/drivers/dpl/ClockP.h>
#include "ti_drivers_config.h"
#include "nvs.h"  // For NVS handle

#include "simple_gatt_profile.h"   // For SimpleProfile_setParameter
//#include "bleapputil_api.h"        // Optional: only if you're using BLEAppUtil_invokeFunctionNoData
#include <ti/bleapp/ble_app_util/inc/bleapputil_api.h>

const uint_least8_t lockDrivePins[] = {
    Lock_Drive1,
    Lock_Drive2
};

const uint_least8_t lockFeedbackPins[] = {
    Feedback_Drive1,
    Feedback_Drive2
};

// Timer object for delayed unlock
static ClockP_Struct unlockDelayClock;
static ClockP_Handle unlockDelayHandle;

static uint8_t timeoutCounter = 0;

// Forward declaration
static void unlockDelayedCallback(uintptr_t arg);

// Timer counter for timeout logic
//static uint8_t timeoutCounter = 0;

void unLockInit(void)
{
    GPIO_init();

    // Create a one-shot timer for 500ms (500,000 µs)
    ClockP_Params clkParams;
    ClockP_Params_init(&clkParams);
    clkParams.period = 0;
    clkParams.startFlag = false;
    clkParams.arg = 0;

    unlockDelayHandle = ClockP_construct(&unlockDelayClock, unlockDelayedCallback,
                                         500000, &clkParams);
}

void performUnlockAction(NVS_Handle handle)
{
    // Set the handle as argument for callback
        ClockP_setFunc(unlockDelayHandle, unlockDelayedCallback, (uintptr_t)handle);
        ClockP_start(unlockDelayHandle);
}

// === Unlock delay expired: perform real unlock ===
static void unlockDelayedCallback(uintptr_t arg)
{
    NVS_Handle handle = (NVS_Handle)arg;

    GPIO_write(lockDrivePins[0], 1);

    //uint8_t response[8] = {'u', 'n', 'l', 'o', 'c', 'k', 'e', 'd'};
    //SimpleProfile_setParameter(SIMPLEPROFILE_CHAR4, sizeof(response), response);

    if (handle != NULL)
    {
        const char unlockMsg[] = "Unlock";
        NVS_write(handle, 10, (void *)unlockMsg, sizeof(unlockMsg), NVS_WRITE_POST_VERIFY);
    }
}

// === External manual lock (e.g., on timeout) ===
void Sdap_BLETimeoutInd(void)
{
    GPIO_write(lockDrivePins[0], 0);
}

// === Call this repeatedly on BLE timeout ticks ===
void Unlock_handleTimeout(NVS_Handle nvsHandleParam)
{
    timeoutCounter++;

    if (timeoutCounter >= 5) // 5 seconds timeout
    {
        timeoutCounter = 0;
        Sdap_BLETimeoutInd();

        // Send BLE failure message
        uint8_t failedMsg[8] = {'f', 'a', 'i', 'l', 'e', 'd', '!', '!'};
        SimpleProfile_setParameter(SIMPLEPROFILE_CHAR4, sizeof(failedMsg), failedMsg);

        // Log to flash
        if (nvsHandleParam != NULL)
        {
            const char failMsg[] = "FailLog";
            NVS_write(nvsHandleParam, 20, (void *)failMsg, sizeof(failMsg), NVS_WRITE_POST_VERIFY);
        }
    }
}

void Unlock_resetTimeout(void)
{
    timeoutCounter = 0;
}
