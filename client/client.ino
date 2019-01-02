#include <stdlib.h>
#include <string.h>
#include <Keypad.h>
#include "PIN.hpp"
#include "LED.hpp"
#include "Button.hpp"
#include "Speaker.hpp"
#include "SonicSensor.hpp"
#include "SonicMotionSensor.hpp"
#include "packet.h"

LED ledRed(A0, false);
LED ledYellow(A1, false);
LED ledGreen(A2, false);
Button button(2);
Button resetButton(4);
Speaker speaker(3);
SonicMotionSensor motionSensor(A3, A4, 100.0);

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
const char modes[] = "ABCD";

PIN pinCode;

bool alarmActive = false;
bool outerSensorsActive = true;
bool innerSensorsActive = true;

void SoundAlarm(void)
{
    speaker.Siren(512, 1024, 5000);
    ledRed.SetState(true);
    delay(250);
    ledRed.SetState(false);
    delay(250);
}

void OnMotionDetection(bool hasMotion)
{
    if(alarmActive)
        return;

    if(hasMotion)
    {
        if(innerSensorsActive)
            alarmActive = true; //
        else
        {
            ledRed.SetState(true);
            delay(100UL);
        }
    }
    else
    {
        ledRed.SetState(false);
    }
}

void ActivateOuterAlarm()
{
    outerSensorsActive = true;

    speaker.Beep(1024, 3000);
    ledRed.Blink(3, 500);
}

void ActivateAlarm()
{
    innerSensorsActive = true;
    outerSensorsActive = true;

    speaker.Beep(1024, 3000);
    ledRed.Blink(3, 500);
}

void DeactivateAlarm()
{
    innerSensorsActive = false;
    outerSensorsActive = false;

    speaker.Beep(1024, 100);
    delay(100);
    speaker.Beep(1024, 100);
    ledGreen.SetState(false);
}

char data[256];
unsigned long int delayTime = 100UL;

void setup()
{
    button.SetReleaseCallback(SoundAlarm); Serial.begin(9600);
    motionSensor.SetTriggerCallback(OnMotionDetection);
    motionSensor.SetFailureCallback(SoundAlarm);

    do
    {
        motionSensor.SetMonitorDistance();
    } while(motionSensor.GetMonitorDistance() == 0.0);

    ledGreen.SetState(true);

    //Serial.begin(9600);
}

void handleKeypad()
{
    char key = keypad.getKey();
    if(key != '\0')
    {
        speaker.Beep(768, 100);

        if(strchr(digits, key) != NULL)
        {
            pinCode.Append(key);
        }
        else if(strchr(modes, key) != NULL)
        {
            pinCode.Clear();
            pinCode.SetMode(key);
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
                packet_mkpincode(&pp, (const uint8_t *const)pinCode.GetContent(), pinCode.GetMode());

                Serial.write((const char *)&pp, sizeof(pp));
                Serial.flush();
                speaker.Beep(1024, 100);

                size_t size = Serial.readBytes(data, sizeof(data));
                if(size < (int)sizeof(packet_header_t))
                {
                    if(size != 0)
                    {
                        ledYellow.SetState(true);
                    }
                }
                else
                {
                    packet_header_t *pkt = (packet_header_t *)data;
                    if(packet_verify(pkt, size, pkt->checksum))
                    {
                        if(pkt->type == PT_SUCCESS)
                        {
                            DeactivateAlarm();
                            switch(pinCode.GetMode())
                            {
                                case 'A':
                                    break;
                                case 'B':
                                    break;
                                case 'C':
                                    break;
                                case 'D':
                                    break;
                                default:
                            };
                        }
                        else if(pkt->type == PT_ERROR)
                        {
                            speaker.Beep(256, 250);
                            ledRed.SetState(true);
                            delay(250);
                            ledRed.SetState(false);
                        }
                    }
                }
            }
            else
            {
                speaker.Beep(256, 100);
            }

            pinCode.Clear();
        }

        delay(delayTime);
    }
}

unsigned long bla = 0;

void loop()
{
    if(alarmActive)
    {
        if(resetButton.GetState())
        {
            alarmActive = false;
            return;
        }

        SoundAlarm();
    }
    else
    {
        alarmActive = outerSensorsActive && !digitalRead(2);

        //button.Update();
        //motionSensor.Update();

        handleKeypad();
    }
}
