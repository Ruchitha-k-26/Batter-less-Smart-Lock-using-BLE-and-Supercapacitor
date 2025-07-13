#include "checksum.h"

bool validateChecksum(uint8_t *data, uint8_t length) {
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < length - 1; i++) {
        checksum += data[i];
    }
    return (checksum == data[length - 1]);
}
