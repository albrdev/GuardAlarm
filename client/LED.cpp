#include "LED.hpp"
#include <Arduino.h>    /* pinMode(), digitalWrite() */

bool LED::GetState(void) const
{
    return m_State;
}

void LED::_SetState(const bool value)
{
    m_State = value;
    digitalWrite(m_Pin, m_State ? HIGH : LOW);
}

void LED::SetState(const bool value)
{
    Stop();
    _SetState(value);
}

void LED::InitBlink(const bool initialState)
{
    Stop();
    SetState(initialState);

    m_Count = 0U;
    m_ElapsedTime = 0U;
    m_StateChangePoint = 0U;
    m_InitialState = m_State;
    m_Active = true;
}

void LED::CountedBlink(const unsigned int count, const unsigned long int blinkDuration, bool initialState)
{
    m_Mode = BlinkMode::BM_COUNT;
    m_MaxCount = count >= 0 ? count * 2 : -1;
    m_Duration = blinkDuration;
    InitBlink(initialState);
}

void LED::TimedBlink(const unsigned long int totalDuration, const unsigned long int blinkDuration, bool initialState)
{
    m_Mode = BlinkMode::BM_TIME;
    m_EndTime = totalDuration;
    m_Duration = blinkDuration;
    InitBlink(initialState);
}

void LED::Blink(const unsigned int count, const unsigned long int totalDuration, const unsigned long int blinkDuration, bool initialState)
{
    m_Mode = BlinkMode::BM_COUNT | BlinkMode::BM_TIME;
    m_MaxCount = count * 2;
    m_EndTime = totalDuration;
    m_Duration = blinkDuration;
    InitBlink(initialState);
}

void LED::Blink(const unsigned long int blinkDuration, bool initialState)
{
    m_Mode = BlinkMode::BM_NONE;
    m_Duration = blinkDuration;
    InitBlink(initialState);
}

void LED::Stop(void)
{
    if(!m_Active) return;

    SetState(m_InitialState);
    m_Active = false;
}

void LED::Update(void)
{
    unsigned long int time = millis();
    if((m_Mode & BlinkMode::BM_COUNT && m_Count >= m_MaxCount) || (m_Mode & BlinkMode::BM_TIME && time >= m_EndTime))
    {
        Stop();
        return;
    }

    if(time >= m_StateChangePoint)
    {
        _SetState(!GetState());
        m_StateChangePoint = time + m_Duration;
        m_Count++;
    }
}

LED::LED(const uint8_t pin, const bool state)
{
    m_Pin = pin;

    pinMode(m_Pin, OUTPUT);
    SetState(state);
}
