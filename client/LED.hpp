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

    LED(uint8_t pin, bool state = false);
};

#endif // _LED_HPP_
