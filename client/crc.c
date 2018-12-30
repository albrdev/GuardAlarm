#include "crc.h"
#ifdef __AVR__
#include <util/crc16.h>
#endif

#ifndef __AVR__
static uint16_t _crc16_update(uint16_t crc, uint8_t a)
{
    uint8_t i;

    crc = (uint16_t)(crc ^ a);
    for(i = 0U; i < 8U; i++)
    {
        crc = crc & 1 ? (uint16_t)((crc >> 1) ^ 0xA001) : (uint16_t)(crc >> 1);
    }

    return crc;
}
#endif

uint16_t mkcrc16(const uint8_t *data, const size_t size)
{
    uint16_t result = 0U;
    for(size_t i = 0U; i < size; i++)
    {
        result = _crc16_update(result, data[i]);
    }

    return result;
}
