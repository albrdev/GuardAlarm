#include <stdlib.h>
#include <string.h>
#include <Keypad.h>
#include "PIN.hpp"
#include "LED.hpp"
#include "Button.hpp"
#include "Speaker.hpp"
#include "packet.h"

LED ledRed(A0, false);
LED ledYellow(A1, false);
LED ledGreen(A2, false);
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
const char modes[] = {keys[0][3], keys[1][3], keys[2][3], keys[3][3], '\0'};

PIN pinCode;

void alarm(void)
{
    speaker.Siren(512, 1024, 5000);
}

char data[256];
unsigned long int delayTime = 100UL;

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
        else if(strchr(modes, key) != NULL)
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

                size_t size = Serial.readBytes(data, sizeof(data));
                if(size < (int)sizeof(packet_header_t))
                {
                    if(size != 0)
                    {
                        speaker.Beep(1024, 100);
                    }
                }
                else
                {
                    packet_header_t *pkt = (packet_header_t *)data;
                    if(packet_verify(pkt, size, pkt->checksum))
                    {
                        if(pkt->type == PT_SUCCESS)
                        {
                            ledYellow.SetState(false);
                            ledRed.SetState(false);
                            ledGreen.SetState(true);
                        }
                        else if(pkt->type == PT_ERROR)
                        {
                            ledYellow.SetState(false);
                            ledGreen.SetState(false);
                            ledRed.SetState(true);
                        }
                    }
                    else
                    {
                        ledRed.SetState(false);
                        ledGreen.SetState(false);
                        ledYellow.SetState(true);
                    }
                }
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
