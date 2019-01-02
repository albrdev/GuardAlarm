#include "SonicSensor.hpp"
#include <Arduino.h>

double SonicSensor::GetDistance(void)
{
    double duration, distance;

    digitalWrite(m_TriggerPin, LOW); // Clear
    delayMicroseconds(2);
    digitalWrite(m_TriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(m_TriggerPin, LOW);

    duration = pulseIn(m_EchoPin, HIGH, m_Timeout);
    distance = (duration / 2.0) * SPEED_OF_SOUND;

    return distance;
}

SonicSensor::SonicSensor(const uint8_t triggerPin, const uint8_t echoPin)
{
    m_TriggerPin = triggerPin;
    m_EchoPin = echoPin;

    pinMode(m_TriggerPin, OUTPUT);
    pinMode(m_EchoPin, INPUT);
}
