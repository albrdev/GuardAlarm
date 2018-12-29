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

void LED::Blink(const int count, const unsigned long int duration)
{
    Blink(count, duration, duration);
}

void LED::Blink(const int count, const unsigned long int duration1, const unsigned long int duration2)
{
    bool initialState = m_State;

    for(int i = 0; i < count; i++)
    {
        SetState(!m_State);
        delay(duration1);
        SetState(!m_State);
        delay(duration2);
    }

    SetState(initialState);
}

void LED::Blink(const bool blink, const unsigned long int duration)
{
    if(!blink)
    {
        nextState = 0UL;
        return;
    }

    unsigned long int now = millis();
    if(now >= nextState)
    {
        SetState(!m_State);
        nextState = now + duration;
    }
}

LED::LED(const uint8_t pin, const bool state)
{
    m_Pin = pin;

    pinMode(m_Pin, OUTPUT);
    SetState(state);
}
