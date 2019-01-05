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
#include "shared.h"

LED redLED(A0, false);
LED yellowLED(A1, false);
LED greenLED(A2, false);
SpeakerNB speaker(3);

Sensor outerSensor(101, 2, INPUT, false); // Outer sensor
SonicMotionSensor motionSensor(102, A3, A4, 50.0, false); // Inner sensor

const unsigned long int MS_SETDISTANCE_MAXDELAY = 5000UL;
const unsigned long int MS_FAILUREDELAY = 1000UL;
unsigned long int motionSensorFailurePoint = 0UL;

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

char data[256];
unsigned long int delayTime = 100UL;

unsigned int loginAttempts = 0U;
PIN pinCode;

const unsigned long AUTH_TIMEOUT = (1UL * 60UL) * 1000UL;
unsigned long int authTimeout = 0UL;
bool isAuthenticated = false;
bool alarmTriggered = false;
unsigned int sirenStartFreq = 1024U;
unsigned int sirenEndFreq = 512U;

void resetLEDs(void)
{
    redLED.SetState(false);
    yellowLED.SetState(false);
    greenLED.SetState(false);
}

void SendSensorStatus(const signed char id, const SensorStatus value)
{
    packet_sensorstatus_t pss;
    packet_mksensorstatus(&pss, id, value);
    Serial.write((const char *)&pss, sizeof(pss));
    Serial.flush();
    delay(delayTime);
}

void SendPINCode(const char *const value)
{
    packet_pincode_t pp;
    packet_mkpincode(&pp, (const uint8_t *const)value, pinCode.GetMode());
    Serial.write((const char *)&pp, sizeof(pp));
    Serial.flush();
    delay(delayTime);
}

void ActivateOuterSensor(void)
{
    outerSensor.SetActive(true);
    SendSensorStatus(outerSensor.GetID(), SensorStatus::SS_CLOSED);
}

void ActivateInnerSensor(void)
{
    unsigned long int endTime = millis() + MS_SETDISTANCE_MAXDELAY;
    do
    {
        if(millis() >= endTime)
        {
            packet_sensorstatus_t pss;
            packet_mksensorstatus(&pss, motionSensor.GetID(), SensorStatus::SS_FAILURE);
            Serial.write((const char *)&pss, sizeof(pss));
            Serial.flush();
            return;
        }

        motionSensor.SetMonitorDistance();

    } while(motionSensor.GetMonitorDistance() == 0.0);
    motionSensor.SetActive(true);

    packet_sensorstatus_t pss;
    packet_mksensorstatus(&pss, motionSensor.GetID(), SensorStatus::SS_CLOSED);
    Serial.write((const char *)&pss, sizeof(pss));
    Serial.flush();
}

void ActivateOuterAlarm(void)
{
    if(!isAuthenticated)
        return;

    isAuthenticated = false;
    authTimeout = 0;

    ActivateOuterSensor();

    resetLEDs();
    speaker.Beep(1024, 250);
    delay(500);
    speaker.Beep(1024, 250);
    delay(500);

    if(outerSensor.GetActive())
    {
        /*redLED.TimedBlink(3000, 750);
        unsigned long int t = millis() + 3000;
        while(millis() < t)
        {
            redLED.Update();
        }*/

        yellowLED.SetState(true);
    }
}

void ActivateAlarm(void)
{
    if(!isAuthenticated)
        return;

    isAuthenticated = false;
    authTimeout = 0;

    ActivateOuterSensor();
    ActivateInnerSensor();

    resetLEDs();
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
    if(isAuthenticated)
        return;

    isAuthenticated = true;
    authTimeout = millis() + AUTH_TIMEOUT;

    outerSensor.SetActive(false);
    motionSensor.SetActive(false);

    resetLEDs();
    speaker.Beep(1024, 250);
    delay(500);
    speaker.Beep(1024, 250);
    delay(500);
    greenLED.SetState(false);
}

void TriggerAlarm(void)
{
    isAuthenticated = false;
    authTimeout = 0;

    alarmTriggered = true;

    redLED.Blink(250);
    yellowLED.Blink(250);
    greenLED.Blink(250);
}

void ResetAlarm(void)
{
    alarmTriggered = false;
    loginAttempts = 0;

    redLED.Stop();
    yellowLED.Stop();
    greenLED.Stop();
    speaker.Stop();
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
                return true;

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
        SendSensorStatus(outerSensor.GetID(), SensorStatus::SS_OPEN);
        TriggerAlarm();
    }

    if(motionSensor.GetActive() && motionSensor.GetMonitorDistance() > 0.0)
    {
        MotionSensorState mss = motionSensor.GetState();
        switch(mss)
        {
            case MotionSensorState::MSS_FAILURE:
                if(!motionSensor.GetActive() || alarmTriggered) //
                    return;

                if(motionSensorFailurePoint == 0U)
                {
                    motionSensorFailurePoint = millis() + MS_FAILUREDELAY;
                }
                else if(millis() >= motionSensorFailurePoint)
                {
                    SendSensorStatus(motionSensor.GetID(), SensorStatus::SS_FAILURE);
                    TriggerAlarm();
                }
                break;
            case MotionSensorState::MSS_TRIGGERED:
                SendSensorStatus(motionSensor.GetID(), SensorStatus::SS_OPEN);
                TriggerAlarm();
                motionSensorFailurePoint = 0U;
                break;
            case MotionSensorState::MSS_IDLE:
                motionSensorFailurePoint = 0U;
                break;
        }
    }
}

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if(authTimeout > 0 && millis() > authTimeout)
    {
        isAuthenticated = false;
    }

    if(alarmTriggered)
    {
        if(!speaker.Active())
        {
            unsigned int tmp = sirenStartFreq;
            sirenStartFreq = sirenEndFreq;
            sirenEndFreq = tmp;
            speaker.LerpTone(sirenStartFreq, sirenEndFreq, 2500);
        }
    }
    else
    {
        checkSensors();

        if(keypadAuthentication())
        {
            // Send PIN code to server
            SendPINCode(pinCode.GetContent());

            // Receive response
            size_t size = Serial.readBytes(data, sizeof(data));
            if(size >= (int)sizeof(packet_header_t))
            {
                packet_header_t *pkt = (packet_header_t *)data;
                if(packet_verify(pkt, size, pkt->checksum)) // Verify packet itegrity
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

            pinCode.Clear();
        }
    }

    speaker.Update();
    redLED.Update();
    yellowLED.Update();
    greenLED.Update();
}
