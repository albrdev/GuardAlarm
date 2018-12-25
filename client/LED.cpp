#include "LED.hpp"
#include <Arduino.h>    /* pinMode(), digitalWrite() */

bool LED::GetState(void) const
{
    return m_State;
}

void LED::SetState(const bool value)
{
    m_State = value;
    digitalWrite(m_Pin, m_State ? HIGH : LOW);
}

LED::LED(uint8_t pin, bool state)
{
    m_Pin = pin;

    pinMode(m_Pin, OUTPUT);
    SetState(state);
}
