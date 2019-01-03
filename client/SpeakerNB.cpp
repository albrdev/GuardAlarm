#include "SpeakerNB.hpp"
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
    m_FromFreq = fromFreq;
    m_ToFreq = toFreq;
    m_ElapsedTime = 0U;
    m_PrevTime = millis();
    m_Active = true;
}

void SpeakerNB::Update(void)
{
    if(m_ElapsedTime < m_Duration)
    {
        unsigned int value = Lerp((int)m_FromFreq, (int)m_ToFreq, (float)m_ElapsedTime / (float)m_Duration);
        tone(m_Pin, value);

        unsigned long int currentTime = millis();
        m_ElapsedTime += currentTime - m_PrevTime;
        m_PrevTime = currentTime;
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
