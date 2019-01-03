#include "Button.hpp"
#include <Arduino.h>    /* pinMode(), digitalRead() */

bool Button::GetState(void) const
{
    return digitalRead(m_Pin);
}

void Button::SetPushCallback(void(*value)(void))
{
    m_PushCallback = value;
}

void Button::SetReleaseCallback(void(*value)(void))
{
    m_ReleaseCallback = value;
}

void Button::SetHoldCallback(void(*value)(void))
{
    m_HoldCallback = value;
}

void Button::Update(void)
{
    bool state = digitalRead(m_Pin);
    if(state != m_State)
    {
        m_State = state;

        if(state)
        {
            if(m_PushCallback != nullptr) m_PushCallback();
        }
        else
        {
            if(m_ReleaseCallback != nullptr) m_ReleaseCallback();
        }
    }
    else
    {
        if(state)
        {
            if(m_HoldCallback != nullptr) m_HoldCallback();
        }
    }
}

Button::Button(uint8_t pin)
{
    m_Pin = pin;

    pinMode(m_Pin, INPUT);
    m_State = digitalRead(m_Pin);
}
