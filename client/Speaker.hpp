#ifndef _SPEAKER_HPP_
#define _SPEAKER_HPP_

#include <stdint.h> /* uint8_t */

class Speaker
{
private:
    uint8_t m_Pin;

    void LerpTone(unsigned int a, unsigned int b, unsigned long int t) const;

public:
    void Beep(const int freq, const unsigned long int duration) const;
    void Siren(const int fromFreq, const unsigned int toFreq, const unsigned long int duration) const;

    Speaker(const uint8_t pin);
};

#endif // _SPEAKER_HPP_
