#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>
#include <stdbool.h>

bool validateChecksum(uint8_t *data, uint8_t length);

#endif // CHECKSUM_H
