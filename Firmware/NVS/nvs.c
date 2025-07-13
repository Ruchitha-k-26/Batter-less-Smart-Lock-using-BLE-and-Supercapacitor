#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ti/drivers/NVS.h>
#include "ti_drivers_config.h"

#define PIN_ADDR 30
#define PIN_LENGTH 4

static const char signature[] = { "SimpleLink SDK NVS Example Signature" };
//#define SIGNATURE_SIZE (sizeof(SIGNATURE))

static char buffer[64];

static NVS_Handle nvsHandle;
static NVS_Params nvsParams;
static NVS_Attrs regionAttrs;

/* Initialize NVS and return handle */
NVS_Handle nvs_init(void)
{
    NVS_init();
    NVS_Params_init(&nvsParams);
    nvsHandle = NVS_open(CONFIG_NVSINTERNAL, &nvsParams);

    if (nvsHandle == NULL)
    {
        /* Handle error - system halt or safe fallback */
        return NULL;
    }

    NVS_getAttrs(nvsHandle, &regionAttrs);

    NVS_read(nvsHandle, 0, (void *)buffer, sizeof(signature));

    if (strcmp((char *)buffer, (char *)signature) == 0)
    {
        NVS_erase(nvsHandle, 0, regionAttrs.sectorSize);
    }
    else
    {
        NVS_write(nvsHandle, 0, (void *)signature, sizeof(signature), NVS_WRITE_ERASE | NVS_WRITE_POST_VERIFY);
    }

    NVS_close(nvsHandle);

    return nvsHandle;
}


void nvs_save_pin(const char *pin)
{
    if(nvsHandle != NULL)
    {
        NVS_write(nvsHandle, PIN_ADDR, (void *)pin, PIN_LENGTH, NVS_WRITE_POST_VERIFY);
    }
}

bool nvs_verify_pin(const char *pin)
{
    char storedPin[PIN_LENGTH];

    if (nvsHandle == NULL)
    {
        return false;
    }

    NVS_read(nvsHandle, PIN_ADDR, storedPin, PIN_LENGTH);

    return (memcmp(storedPin, pin, PIN_LENGTH) == 0);
}


/* Read signature from NVS */
/*int nvs_read_signature(char *buffer, size_t bufSize)
{
    if (bufSize < SIGNATURE_SIZE)
        return -1;  // Buffer too small

    int status = NVS_read(nvsHandle, 0, buffer, SIGNATURE_SIZE);
    return status;  // 0 on success, negative on failure
}*/

/* Write signature to NVS */
/*int nvs_write_signature(const char *data)
{
    int status = NVS_write(nvsHandle, 0, (void *)data, SIGNATURE_SIZE,
                           NVS_WRITE_ERASE | NVS_WRITE_POST_VERIFY);
    return status;
}*/

/* Erase NVS sector */
/*int nvs_erase_sector(void)
{
    NVS_Attrs regionAttrs;
    NVS_getAttrs(nvsHandle, &regionAttrs);

    int status = NVS_erase(nvsHandle, 0, regionAttrs.sectorSize);
    return status;
}*/

/* Close NVS handle */
//void nvs_close(void)
//{
  //  if (nvsHandle != NULL)
    //{
      //  NVS_close(nvsHandle);
      //  nvsHandle = NULL;
    //}
//}

