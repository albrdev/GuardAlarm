#include <stdlib.h>
#include <string.h>
#include <Keypad.h>
#include "PIN.hpp"
#include "LED.hpp"
#include "Button.hpp"
#include "Speaker.hpp"
#include "crc.h"

LED ledRed(A0, true);
LED ledYellow(A1, true);
LED ledGreen(A2, true);
Button button(2);
Speaker speaker(3);

const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] =
{
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
};

byte rowPins[rows] = { 10, 11, 12, 13 };
byte colPins[cols] = { 6, 7, 8, 9 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

const char digits[] = "0123456789";
const char options[] = {keys[0][3], keys[1][3], keys[2][3], keys[3][3], '\0'};

PIN pinCode;

void alarm(void)
{
    speaker.Siren(512, 1024, 5000);
}

enum PacketType
{
    PT_ERROR = 0,
    PT_SUCCESS = 1,
    PT_EMERGENCY = 1,
    PT_PIN = 3
};

typedef struct _packet_header packet_header_t;
typedef struct _packet_pincode packet_pincode_t;
typedef struct _packet_i32 packet_i32_t;

struct __attribute__((packed)) _packet_header
{
    uint16_t checksum;
    uint8_t type;
};

struct __attribute__((packed)) _packet_pincode
{
    packet_header_t header;

    uint8_t pin[6 + 1];
    uint8_t option;
};

struct __attribute__((packed)) _packet_i32
{
    packet_header_t header;

    uint32_t value;
};

char data[256];
unsigned long int delayTime = 100UL;

void packet_mkheader(struct _packet_header *const pkt, const uint16_t size, const uint8_t type)
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

void packet_mkpincode(struct _packet_pincode *const pkt, const uint8_t *const pin, const uint8_t option)
{
    size_t pinLen = strlen((const char *)pin);
    memcpy(pkt->pin, pin, pinLen);
    memset(pkt->pin + pinLen, 0, sizeof(pkt->pin) - pinLen);
    pkt->option = option;

    packet_mkheader(&pkt->header, sizeof(*pkt), PT_PIN);
}

void setup()
{
    button.SetPushCallback(alarm);

    Serial.begin(9600);
}

void loop()
{
    button.Update();

    char key = keypad.getKey();
    if(key != '\0')
    {
        if(strchr(digits, key) != NULL)
        {
            pinCode.Append(key);
        }
        else if(strchr(options, key) != NULL)
        {
            pinCode.Clear();
            pinCode.SetOption(key);
        }
        else if(key == '*')
        {
            pinCode.Clear();
            speaker.Beep(1024, 100);
        }
        else if(key == '#')
        {
            if(pinCode.IsValid())
            {
                packet_pincode_t pp;
                packet_mkpincode(&pp, (const uint8_t *const)pinCode.GetContent(), pinCode.GetOption());

                Serial.write((const char *)&pp, sizeof(pp));
                Serial.flush();
                speaker.Beep(512, 100);
            }
            else
            {
                speaker.Beep(1024, 100);
            }

            pinCode.Clear();
        }

        delay(delayTime);
    }
}
