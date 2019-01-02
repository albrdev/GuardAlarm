#ifndef _PACKET_H_
#define _PACKET_H_

#include <stdint.h> /* uint8_t, uint16_t */
#include <string.h> /* size_t, memset(), memcpy(), strlen() */

typedef struct _packet_header packet_header_t;
typedef struct _packet_pincode packet_pincode_t;

enum PacketType
{
    PT_ERROR = 0,
    PT_SUCCESS = 1,
    PT_EMERGENCY = 2,
    PT_SENSORSTATUS = 3,
    PT_PIN = 4,
};

struct __attribute__((packed)) _packet_header
{
    uint16_t checksum;
    uint8_t type;
};

struct __attribute__((packed)) _packet_sensorstatus
{
    packet_header_t header;

    uint8_t status;
};

struct __attribute__((packed)) _packet_pincode
{
    packet_header_t header;

    uint8_t pin[6 + 1];
    uint8_t mode;
};

#ifdef __cplusplus
extern "C"
{
#endif
    int packet_verify(struct _packet_header *const pkt, const size_t size, const uint16_t checksum);
    void packet_mkheader(struct _packet_header *const pkt, const size_t size, const uint8_t type);
    void packet_mkpincode(struct _packet_pincode *const pkt, const uint8_t *const pin, const uint8_t mode);
#ifdef __cplusplus
} // extern "C"
#endif

#endif // _PACKET_H_
