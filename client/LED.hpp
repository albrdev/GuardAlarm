#ifndef _LED_HPP_
#define _LED_HPP_

#include <stdint.h> /* uint8_t */

class LED
{
private:
    uint8_t m_Pin;
    bool m_State;

public:
    bool GetState(void) const;
    void SetState(const bool value);

    void Blink(const int count, const unsigned long int duration);
    void Blink(const int count, const unsigned long int duration1, const unsigned long int duration2);

    LED(uint8_t pin, bool state = false);
};

#endif // _LED_HPP_
