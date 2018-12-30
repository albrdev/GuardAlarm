#ifndef _CRC_H_
#define _CRC_H_

#include <stdint.h> /* uint8_t, uint16_t */

#ifdef __cplusplus
extern "C"
{
#endif

uint16_t mkcrc16(const uint8_t *data, const uint16_t size);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _CRC_H_
