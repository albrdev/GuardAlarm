#ifndef _SPEAKERNB_HPP_
#define _SPEAKERNB_HPP_

#include <stdint.h> /* uint8_t */

/*  SpeakerNB - Speaker Non-blocking
    A non-blocking spekaer class to handle a passive piezo speaker
*/
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
    // Beep - Make a beeping sound with the frequency 'freq' under a duration of 'duration'
    void Beep(const int freq, const unsigned long int duration);
    // Stop - Silence the speaker
    void Stop(void);
    // LerpTone - Sets the values to lerp between the frequencies 'fromFreq' and 'toFreq' during a time of 'duration', the effect of this methos will need to run in Update (for non-blocking functionality)
    void LerpTone(const unsigned int fromFreq, const unsigned int toFreq, const unsigned long int duration);
    // Update - Updates the LerpTone effect
    void Update(void);

    SpeakerNB(const uint8_t pin);
};

#endif // _SPEAKERNB_HPP_
