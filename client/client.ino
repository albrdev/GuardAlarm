#include <stdlib.h>
#include <string.h>
#include <Keypad.h>
#include "PIN.hpp"
#include "LED.hpp"
#include "Button.hpp"
#include "Speaker.hpp"
#include "SpeakerNB.hpp"
#include "Sensor.hpp"
#include "SonicMotionSensor.hpp"
#include "packet.h"

LED ledRed(A0, false);
LED ledYellow(A1, false);
LED ledGreen(A2, false);
Button resetButton(4);
SpeakerNB speaker(3);
Sensor outerSensor(101, 2, INPUT, false); // Outer sensor
SonicMotionSensor motionSensor(102, A3, A4, 50.0, false); // Inner sensor
unsigned int motionSensorFailurePoint = 0U;
const unsigned int MOTIONSENSOR_FAILUREDELAY = 1000U;

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

const int LOGINATTEMPTS_MAX = 3;
PIN pinCode;
int loginAttempts = 0;

bool alarmTriggered = false;

void ActivateOuterAlarm()
{
    outerSensor.SetActive(true);

    //speaker.Beep(1024, 3000);
}

void ActivateAlarm()
{
    outerSensor.SetActive(true);
    motionSensor.SetActive(true);

    //speaker.Beep(1024, 3000);
    tone(3, 1024, 250);
    delay(250);
    tone(3, 1024, 250);
    delay(250);
    tone(3, 1024, 250);
    delay(250);
    tone(3, 1024, 250);
    delay(250);
    ledGreen.SetState(true);
}

void DeactivateAlarm()
{
    outerSensor.SetActive(false);
    motionSensor.SetActive(false);

    tone(3, 1024, 100);
    delay(100);
    tone(3, 1024, 100);
    delay(100);
    ledGreen.SetState(false);
}

void TriggerAlarm()
{
    alarmTriggered = true;

    ledRed.Blink(250);
    ledYellow.Blink(250);
    ledGreen.Blink(250);
}

char data[256];
unsigned long int delayTime = 100UL;

void Bla(int count, unsigned int freq, unsigned long dur)
{
    int i;
    for(i = 0; i < count; i++)
    {
        tone(freq, freq, dur);
    }
}

void setup()
{
    Serial.begin(9600);

    do
    {
        motionSensor.SetMonitorDistance();
    } while(motionSensor.GetMonitorDistance() == 0.0);

    ledGreen.SetState(true);
}

bool keypadAuthentication()
{
    char key = keypad.getKey();
    if(key != '\0')
    {
        speaker.Beep(768, 100);

        if(strchr(modes, key) != NULL)
        {
            pinCode.Clear();
            pinCode.SetMode(key);
        }
        else if(key == '*')
        {
            pinCode.Clear();
        }
        else if(key == '#')
        {
            if(pinCode.IsValid())
            {
                return true;
            }

            pinCode.Clear();
        }
        else
        {
            pinCode.Append(key);
        }

        //delay(delayTime);
    }

    return false;
}

void checkSensors()
{
    if(outerSensor.IsTriggered())
    {
        TriggerAlarm();
    }

    MotionSensorState mss = motionSensor.GetState();
    switch(mss)
    {
        case MotionSensorState::MSS_FAILURE:
            if(!motionSensor.GetActive()) //
                return;

            if(motionSensorFailurePoint == 0U)
            {
                motionSensorFailurePoint = millis() + MOTIONSENSOR_FAILUREDELAY;
            }
            else if(millis() >= motionSensorFailurePoint)
            {
                packet_sensorstatus_t pss;
                packet_mksensorstatus(&pss, motionSensor.GetID(), (uint8_t)-1);
                Serial.write((const char *)&pss, sizeof(pss));
                Serial.flush();

                TriggerAlarm();
            }
            break;
        case MotionSensorState::MSS_TRIGGERED:
            if(motionSensor.GetActive())
                TriggerAlarm();
            else
                ledRed.SetState(true);

            motionSensorFailurePoint = 0U;
            break;
        case MotionSensorState::MSS_IDLE:
            motionSensorFailurePoint = 0U;

            if(!motionSensor.GetActive())
                ledRed.SetState(false);
            break;
    }
}

bool sensorFailure = false;

unsigned int fromFreq = 1024;
unsigned int toFreq = 512;

void loop()
{
    if(alarmTriggered)
    {
        if(resetButton.GetState())
        {
            alarmTriggered = false;
            loginAttempts = 0;

            return;
        }

        if(!speaker.Active())
        {
            unsigned int tmp = fromFreq;
            fromFreq = toFreq;
            toFreq = tmp;
            speaker.LerpTone(fromFreq, toFreq, 2500U);
        }
    }
    else
    {
        checkSensors();

        if(keypadAuthentication())
        {
            packet_pincode_t pp;
            packet_mkpincode(&pp, (const uint8_t *const)pinCode.GetContent(), pinCode.GetMode());

            Serial.write((const char *)&pp, sizeof(pp));
            Serial.flush();
            //speaker.Beep(1024, 100);

            size_t size = Serial.readBytes(data, sizeof(data));
            if(size >= (int)sizeof(packet_header_t))
            {
                packet_header_t *pkt = (packet_header_t *)data;
                if(packet_verify(pkt, size, pkt->checksum))
                {
                    if(pkt->type == PT_SUCCESS)
                    {
                        switch(pinCode.GetMode())
                        {
                            case 'A':
                                ActivateAlarm();
                                break;
                            case 'B':
                                ActivateOuterAlarm();
                                break;
                            case 'C':
                                break;
                            case 'D':
                                break;
                            default:
                                DeactivateAlarm();
                        };
                    }
                    else if(pkt->type == PT_FAILURE)
                    {
                        loginAttempts++;
                        if(loginAttempts >= LOGINATTEMPTS_MAX)
                        {
                            TriggerAlarm();
                        }

                        speaker.Beep(256, 500);
                        ledRed.CountedBlink(1, 500);
                    }
                }
            }
            else
            {
                if(size != 0)
                    ledYellow.SetState(true);
            }

            pinCode.Clear();
        }
    }

    speaker.Update();
    ledRed.Update();
    ledYellow.Update();
    ledGreen.Update();
}
