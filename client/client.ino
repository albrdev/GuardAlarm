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
bool motionSensorFailure = false;
bool outerSensorFailure = false;

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

// Commonly used constants
const unsigned int VALUE_LOW = 250U;
const unsigned int VALUE_MEDIUM = 500U;
const unsigned int VALUE_HIGH = 750U;
const unsigned int VALUE_HIGHEST = 1000U;

const unsigned long int POLL_DELAY = 100UL;
char data[256];

unsigned int loginAttempts = 0U;
PIN pinCode;

enum AlarmMode
{
    AM_NONE = 0,
    AM_OFF = 1,
    AM_A = 2,
    AM_B = 3
};

const unsigned long AUTH_TIMEOUT = (1UL * 60UL) * 1000UL;
unsigned long int authTimeout = 0UL;
bool isAuthenticated = false;
AlarmMode alarmMode = AM_NONE;
bool alarmTriggered = false;
unsigned int sirenStartFreq = VALUE_HIGHEST;
unsigned int sirenEndFreq = VALUE_MEDIUM;

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
    delay(POLL_DELAY);
}

void SendPINCode(const char *const value)
{
    packet_pincode_t pp;
    packet_mkpincode(&pp, (const uint8_t *const)value, pinCode.GetMode());
    Serial.write((const char *)&pp, sizeof(pp));
    Serial.flush();
    delay(POLL_DELAY);
}

void beep(const unsigned int count, const unsigned freq, const unsigned long int duration)
{
    for(unsigned int i = 0U; i < count; i++)
    {
        speaker.Beep(freq, duration);
        delay(duration * 2U);
    }
}

void ActivateOuterSensor(void)
{
    outerSensor.SetActive(true);

    outerSensorFailure = !outerSensor.GetState();
    SendSensorStatus(outerSensor.GetID(), outerSensorFailure ? SensorStatus::SS_FAILURE : SensorStatus::SS_CLOSED);
}

void ActivateInnerSensor(void)
{
    unsigned long int endTime = millis() + MS_SETDISTANCE_MAXDELAY;
    do
    {
        if(millis() >= endTime)
        {
            motionSensorFailure = true;
            SendSensorStatus(motionSensor.GetID(), SensorStatus::SS_FAILURE);
            return;
        }

        motionSensor.SetMonitorDistance();

    } while(motionSensor.GetMonitorDistance() == 0.0);
    motionSensor.SetActive(true);

    SendSensorStatus(motionSensor.GetID(), SensorStatus::SS_CLOSED);
}

void ActivateOuterAlarm(void)
{
    if(!isAuthenticated)
        return;

    isAuthenticated = false;
    authTimeout = 0;
    alarmMode = AlarmMode::AM_B;

    ActivateOuterSensor();

    resetLEDs();
    beep(2, VALUE_HIGHEST, VALUE_LOW);

    if(!outerSensorFailure)
    {
        //redLED.TimedBlink(3000UL, VALUE_HIGH);
        //unsigned long int endTime = millis() + 3000UL;
        //while(millis() < endTime)
        //{
        //    redLED.Update();
        //    //speaker.Beep(VALUE_MEDIUM, 500);
        //    //delay(1000);
        //}

        yellowLED.SetState(true);
        greenLED.Blink(VALUE_HIGH);
    }
}

void ActivateAlarm(void)
{
    if(!isAuthenticated)
        return;

    isAuthenticated = false;
    authTimeout = 0;
    alarmMode = AlarmMode::AM_A;

    ActivateOuterSensor();
    ActivateInnerSensor();

    resetLEDs();
    beep(3, VALUE_HIGHEST, VALUE_MEDIUM);
    greenLED.SetState(true);

    if(!outerSensorFailure || !motionSensorFailure)
    {
        //speaker.Beep(VALUE_MEDIUM, 3000);
        //redLED.TimedBlink(3000, VALUE_HIGH);

        yellowLED.SetState(true);
    }
}

void DeactivateAlarm(void)
{
    if(isAuthenticated)
        return;

    isAuthenticated = true;
    authTimeout = millis() + AUTH_TIMEOUT;
    alarmMode = AlarmMode::AM_OFF;

    outerSensor.SetActive(false);
    motionSensor.SetActive(false);

    beep(2, VALUE_HIGHEST, VALUE_LOW);
    resetLEDs();
    greenLED.SetState(false);
}

void TriggerAlarm(void)
{
    isAuthenticated = false;
    authTimeout = 0;

    alarmTriggered = true;

    redLED.Blink(VALUE_LOW);
    yellowLED.Blink(VALUE_LOW);
    greenLED.Blink(VALUE_LOW);
}

void ResetAlarm(void)
{
    alarmTriggered = false;
    loginAttempts = 0;

    redLED.Stop();
    yellowLED.Stop();
    greenLED.Stop();
    speaker.Stop();

    sirenStartFreq = VALUE_HIGHEST;
    sirenEndFreq = VALUE_MEDIUM;
}

bool keypadAuthentication()
{
    char key = keypad.getKey();
    if(key != NO_KEY)
    {
        speaker.Beep(VALUE_HIGH, 100);

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
                if((alarmMode != AlarmMode::AM_OFF && pinCode.GetMode() == '\0') || (alarmMode == AlarmMode::AM_OFF && pinCode.GetMode() == 'A') || (alarmMode == AlarmMode::AM_OFF && pinCode.GetMode() == 'B'))
                {
                    return true;
                }
            }

            pinCode.Clear();
        }
        else
        {
            pinCode.Append(key);
        }
    }

    return false;
}

void checkSensors()
{
    if(!outerSensorFailure)
    {
        if(outerSensor.GetActive() && !outerSensor.GetState())
        {
            SendSensorStatus(outerSensor.GetID(), SensorStatus::SS_OPEN);
            TriggerAlarm();
        }
    }

    if(!motionSensorFailure)
    {
        if(motionSensor.GetActive() && motionSensor.GetMonitorDistance() > 0.0)
        {
            MotionSensorState mss = motionSensor.GetState();
            switch(mss)
            {
                case MotionSensorState::MSS_FAILURE:
                    if(!motionSensor.GetActive() || alarmTriggered)
                        return;

                    if(motionSensorFailurePoint == 0U)
                    {
                        motionSensorFailurePoint = millis() + MS_FAILUREDELAY;
                    }
                    else if(millis() >= motionSensorFailurePoint)
                    {
                        motionSensorFailure = true;
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
                            speaker.Beep(VALUE_LOW, VALUE_LOW);
                            redLED.CountedBlink(1, VALUE_LOW);
                        }
                    }
                }

                delay(POLL_DELAY);
            }

            pinCode.Clear();
        }
    }

    speaker.Update();
    redLED.Update();
    yellowLED.Update();
    greenLED.Update();
}
