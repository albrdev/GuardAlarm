#include "Speaker.hpp"
#include <Arduino.h>    /* pinMode(), digitalWrite() */

static int Lerp(int start, int end, float percent)
{
    return (start + percent * (end - start));
}

void Speaker::LerpTone(unsigned int a, unsigned int b, unsigned long int t) const
{
    unsigned long int elapsedTime = 0UL;
    unsigned long prevTime = millis();
    while(elapsedTime < t)
    {
        unsigned int value = Lerp((int)a, (int)b, (float)elapsedTime / (float)t);
        tone(m_Pin, value);

        unsigned long int currentTime = millis();
        elapsedTime += currentTime - prevTime;
        prevTime = currentTime;
    }

    noTone(m_Pin);
}

void Speaker::Beep(const int freq, const unsigned long int duration) const
{
    tone(m_Pin, freq, duration);
}

void Speaker::Siren(const int fromFreq, const unsigned int toFreq, const unsigned long int duration) const
{
    unsigned long int dur = duration / 2;
    LerpTone(fromFreq, toFreq, dur);
    LerpTone(toFreq, fromFreq, dur);
}

Speaker::Speaker(const uint8_t pin)
{
    m_Pin = pin;

    pinMode(m_Pin, OUTPUT);
}