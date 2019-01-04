#include "SpeakerNB.hpp"
#include "math.h"
#include <Arduino.h>    /* pinMode(), digitalWrite() */

bool SpeakerNB::Active(void) const
{
    return m_Active;
}

void SpeakerNB::Beep(const int freq, const unsigned long int duration)
{
    Stop();
    tone(m_Pin, freq, duration);
}

void SpeakerNB::Stop(void)
{
    if(!m_Active)
        return;

    noTone(m_Pin);
    m_Active = false;
}

void SpeakerNB::LerpTone(const unsigned int fromFreq, const unsigned int toFreq, const unsigned long int duration)
{
    m_Duration = duration;
    m_FromFrequency = fromFreq;
    m_ToFrequency = toFreq;
    m_StartTime = millis();
    m_EndTime = m_StartTime + m_Duration;
    m_Active = true;
}

void SpeakerNB::Update(void)
{
    if(!m_Active)
        return;

    unsigned long int currentTime = millis();
    if(currentTime < m_EndTime)
    {
        double percentage = (double)(currentTime - m_StartTime) / (double)m_Duration;
        unsigned int value = lerp((int)m_FromFrequency, (int)m_ToFrequency, percentage);
        tone(m_Pin, value);
    }
    else
    {
        Stop();
    }
}

SpeakerNB::SpeakerNB(const uint8_t pin)
{
    m_Pin = pin;

    pinMode(m_Pin, OUTPUT);
}
