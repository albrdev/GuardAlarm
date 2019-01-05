#include "packet.h"
#include "crc.h"

int packet_verify(const struct _packet_header *const pkt, const size_t size, const uint16_t checksum)
{
    return mkcrc16((uint8_t *)pkt + sizeof(pkt->checksum), size - sizeof(pkt->checksum)) == checksum;
}

void packet_mkheader(struct _packet_header *const pkt, const size_t size, const uint8_t type)
{
    pkt->type = type;

    // Checmsum has to be calculated last
    // Checmsum can't be taken into account for the checksum itself, therefore start at an offset to avoid it
    // The length of the data to be calculated over is now total size minus the checksum size to avoid going outside allowed memory

    // Packet in bits: C = checksum, T = type, D = data (size may vary)
    // CCCCCCCCCCCCCCCCTTTTTTTTDDDDDDDDDDDDDDDD
    //                 ^---------------------->| size - sizeof(checksum)

    pkt->checksum = mkcrc16((uint8_t *)pkt + sizeof(pkt->checksum), size - sizeof(pkt->checksum));
}

void packet_mksensorstatus(struct _packet_sensorstatus *const pkt, const uint8_t id, const signed char status)
{
    pkt->id = id;
    pkt->status = status;

    packet_mkheader(&pkt->header, sizeof(*pkt), PT_SENSORSTATUS);
}

void packet_mkpincode(struct _packet_pincode *const pkt, const uint8_t *const pin, const uint8_t mode)
{
    size_t pinLen = strlen((const char *)pin);
    memcpy(pkt->pin, pin, pinLen);
    memset(pkt->pin + pinLen, 0, sizeof(pkt->pin) - pinLen);
    pkt->mode = mode;

    packet_mkheader(&pkt->header, sizeof(*pkt), PT_PINCODE);
}
