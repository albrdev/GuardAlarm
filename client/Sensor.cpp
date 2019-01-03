#include "Sensor.hpp"
#include <Arduino.h>    /* pinMode(), digitalRead() */

int Sensor::GetID(void) const
{
    return m_ID;
}

bool Sensor::GetActive(void) const
{
    return m_Active;
}

void Sensor::SetActive(const bool value)
{
    m_Active = value;
}

bool Sensor::GetState(void) const
{
    return digitalRead(m_Pin);
}

bool Sensor::IsTriggered(void) const
{
    return GetActive() && !GetState();
}

Sensor::Sensor(const int id, const uint8_t pin, const uint8_t mode, const bool active)
{
    m_ID = id;
    m_Active = active;

    m_Pin = pin;
    pinMode(m_Pin, mode);
}
