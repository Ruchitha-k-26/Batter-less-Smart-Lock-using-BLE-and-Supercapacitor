#ifndef UNLOCK_H
#define UNLOCK_H
#include <ti/drivers/NVS.h>
void unLockInit(void);
void performUnlockAction(NVS_Handle handle);

void Sdap_BLETimeoutInd(void);
void Unlock_handleTimeout(NVS_Handle nvsHandle);
void Unlock_resetTimeout(void);

#endif
