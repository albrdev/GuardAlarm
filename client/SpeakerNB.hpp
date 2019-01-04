#ifndef _SPEAKERNB_HPP_
#define _SPEAKERNB_HPP_

#include <stdint.h> /* uint8_t */

class SpeakerNB
{
private:
    uint8_t m_Pin;

    unsigned long int m_EndTime = 0UL;
    unsigned long int m_StartTime = 0UL;
    unsigned long int m_Duration = 0UL;
    unsigned int m_FromFrequency = 0U;
    unsigned int m_ToFrequency = 0U;
    bool m_Active;

public:
    bool Active(void) const;
    void Beep(const int freq, const unsigned long int duration);
    void Stop(void);
    void LerpTone(const unsigned int fromFreq, const unsigned int toFreq, const unsigned long int duration);
    void Update(void);

    SpeakerNB(const uint8_t pin);
};

#endif // _SPEAKERNB_HPP_
