#ifndef NVS_HELPER_H
#define NVS_HELPER_H

#include <stddef.h>
#include <ti/drivers/NVS.h>
#include <stdbool.h>

/* Signature string and size */
//#define SIGNATURE "SimpleLink SDK NVS Example Signature"
//#define SIGNATURE_SIZE (sizeof(SIGNATURE))


#define PIN_ADDR 30
#define PIN_LENGTH 4
/* Initialize NVS module and return handle */
NVS_Handle nvs_init(void);
bool nvs_verify_pin(const char *pin);
void nvs_save_pin(const char *pin);
/* Read signature from NVS into buffer
 * Returns 0 on success, negative on failure */
//int nvs_read_signature(char *buffer, size_t bufSize);

/* Write signature to NVS
 * Returns 0 on success, negative on failure */
//int nvs_write_signature(const char *data);

/* Erase entire NVS sector
 * Returns 0 on success, negative on failure */
//int nvs_erase_sector(void);

/* Close NVS handle */
//void nvs_close(void);

#endif /* NVS_HELPER_H */
