#include <Keypad.h>
#include <util/crc16.h>
#include "LED.hpp"
#include "Button.hpp"
#include "Speaker.hpp"

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

const int PINCODE_MINLENGTH = 4;
const int PINCODE_MAXLENGTH = 6;

const char options[] = "ABCD";

void alarm(void)
{
    speaker.Siren(512, 1024, 5000);
}

enum TransmitType
{
    TT_ERROR = 0,
    TT_SUCCESS = 1,
    TT_EMERGENCY = 1,
    TT_PIN = 3
};

typedef struct __attribute__((packed)) packet
{
    uint16_t checksum;
    uint8_t type;
} packet;

typedef struct __attribute__((packed)) packet_pincode
{
    uint16_t checksum;
    uint8_t type;

    uint16_t size;
    uint8_t data[6];
} packet_pincode_t;

typedef struct __attribute__((packed)) packet_i32
{
    uint16_t checksum;
    uint8_t type;

    uint32_t value;
} packet_i32_t;

packet p;

uint16_t calcCRC(const uint8_t *s, const uint16_t size)
{
    uint16_t crc = 0; // starting value as you like, must be the same before each calculation
    for(uint16_t i = 0U; i < size; i++) // for each character in the string
    {
        crc = _crc16_update(crc, s[i]); // update the crc value
    }
    return crc;
}

uint16_t checksum(const uint8_t *s, const uint16_t size)
{
    uint16_t c = 0;
    for(uint16_t i = 0U; i < size; i++)
    {
        c ^= *s++;
    }

    return c;
}

String pinInput;
char pinOption;
char pin[] = "0123";
char dataa[255];
unsigned long int delayTime = 100UL;

void setPacket(void *const pkt, const uint16_t size, const uint8_t type)
{
    packet *pp = (packet *)pkt;
    pp->type = type;

    // Checmsum has to be calculated last
    // Checmsum can't be taken into account for the checksum itself, therefore start at an offset to avoid it
    // The length of the data to be calculated over is now total size minus the checksum size to avoid going outside allowed memory

    // Packet in bits: C = checksum, T = type, D = data (size may vary)
    // CCCCCCCCCCCCCCCCTTTTTTTTDDDDDDDDDDDDDDDD
    //                 ^---------------------->| size - sizeof(checksum)

    pp->checksum = calcCRC((uint8_t *)pp + sizeof(pp->checksum), size - sizeof(pp->checksum));
}

void setup()
{
    pinInput.reserve(PINCODE_MAXLENGTH);
    button.SetPushCallback(alarm);

    Serial.begin(9600);

    setPacket(&p, sizeof(p), 1);
}

void loop()
{
    button.Poll();

    //ledRed.Blink(true, 100);
    //ledYellow.Blink(true, 100);
    //ledGreen.Blink(true, 100);

    char key = keypad.getKey();
    if(key != '\0')
    {
        if(strchr(options, key) != NULL)
        {
            pinOption = key;
            pinInput = "";
        }
        else if(key == '#')
        {
            if(pinInput.length() >= PINCODE_MINLENGTH && pinInput.length() <= PINCODE_MAXLENGTH)
            {
                packet_pincode_t pp;
                memcpy(pp.data, pinInput.c_str(), pinInput.length());
                pp.size = pinInput.length();
                setPacket(&pp, sizeof(pp), TT_PIN);

                Serial.write((const char *)&pp, sizeof(pp));
                Serial.flush();
                speaker.Beep(512, 100);
            }
            else
            {
                speaker.Beep(1024, 100);
            }

            pinOption = '\0';
            pinInput = "";
        }
        else
        {
            while(pinInput.length() >= PINCODE_MAXLENGTH)
            {
                pinInput.remove(0, 1);
            }

            pinInput.concat(key);
        }

        delay(delayTime);
    }
}
