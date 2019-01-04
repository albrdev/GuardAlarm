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

LED redLED(A0, false);
LED yellowLED(A1, false);
LED greenLED(A2, false);
SpeakerNB speaker(3);

Sensor outerSensor(101, 2, INPUT, true); // Outer sensor
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

    speaker.Beep(1024, 250);
    delay(500);
    speaker.Beep(1024, 250);
    delay(500);
    

    if(outerSensor.GetActive())
    {
        redLED.TimedBlink(3000, 750);
        unsigned long int t = millis() + 3000;
        while(millis() < t)
        {
            redLED.Update();
        }

        yellowLED.SetState(true);
    }
}

void ActivateAlarm(void)
{
    outerSensor.SetActive(true);

    do
    {
        motionSensor.SetMonitorDistance();
    } while(motionSensor.GetMonitorDistance() == 0.0);
    motionSensor.SetActive(true);

    //speaker.Beep(1024, 3000);
    speaker.Beep(1024, 500);
    delay(750);
    speaker.Beep(1024, 500);
    delay(750);
    speaker.Beep(1024, 500);
    delay(750);
    greenLED.SetState(true);

    if(outerSensor.GetActive() || motionSensor.GetActive())
    {
        yellowLED.SetState(true);
    }
}

void DeactivateAlarm(void)
{
    outerSensor.SetActive(false);
    motionSensor.SetActive(false);

    speaker.Beep(1024, 250);
    delay(500);
    speaker.Beep(1024, 250);
    delay(500);
    greenLED.SetState(false);
}

void TriggerAlarm(void)
{
    Serial.println("activate");
    alarmTriggered = true;

    redLED.Blink(250);
    yellowLED.Blink(250);
    greenLED.Blink(250);
}

void ResetAlarm(void)
{
    Serial.println("deactivate");
    alarmTriggered = false;
    loginAttempts = 0;

    redLED.Stop();
    yellowLED.Stop();
    greenLED.SetState(true);
    speaker.Stop();
}

char data[256];
unsigned long int delayTime = 100UL;

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
    if(outerSensor.GetActive() && !outerSensor.GetState())
    {
        TriggerAlarm();
    }

    if(motionSensor.GetActive() && motionSensor.GetMonitorDistance() > 0.0)
    {

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
                TriggerAlarm();
                motionSensorFailurePoint = 0U;
                break;
            case MotionSensorState::MSS_IDLE:
                motionSensorFailurePoint = 0U;
                break;
        }
    }
}

bool sensorFailure = false;

unsigned int fromFreq = 1024;
unsigned int toFreq = 512;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if(alarmTriggered)
    {
        if(!speaker.Active())
        {
            unsigned int tmp = fromFreq;
            fromFreq = toFreq;
            toFreq = tmp;
            speaker.LerpTone(fromFreq, toFreq, 2500);
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

            size_t size = Serial.readBytes(data, sizeof(data));
            if(size >= (int)sizeof(packet_header_t))
            {
                packet_header_t *pkt = (packet_header_t *)data;
                if(packet_verify(pkt, size, pkt->checksum))
                {
                    if(pkt->type == PT_SUCCESS)
                    {
                        loginAttempts = 0;
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
                        }
                    }
                    else if(pkt->type == PT_FAILURE)
                    {
                        if(++loginAttempts >= LOGINATTEMPTS_MAX)
                        {
                            TriggerAlarm();
                        }
                        else
                        {
                            speaker.Beep(256, 250);
                            redLED.CountedBlink(1, 250);
                        }
                    }
                }
            }
            else
            {
                if(size != 0)
                    yellowLED.SetState(true);
            }

            pinCode.Clear();
        }
    }

    speaker.Update();
    redLED.Update();
    yellowLED.Update();
    greenLED.Update();
}
