#ifndef _LED_HPP_
#define _LED_HPP_

#include <stdint.h> /* uint8_t */

class LED
{
private:
    uint8_t m_Pin;
    bool m_State;

    bool initialState;
    unsigned long int nextState = 0UL;

public:
    bool GetState(void) const;
    void SetState(const bool value);

    void Blink(const int count, const unsigned long int duration);
    void Blink(const int count, const unsigned long int duration1, const unsigned long int duration2);

    void Blink(const bool blink, const unsigned long int duration);

    LED(const uint8_t pin, const bool state = false);
};

#endif // _LED_HPP_
