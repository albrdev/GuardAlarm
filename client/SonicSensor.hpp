#ifndef _SONICSENSOR_HPP_
#define _SONICSENSOR_HPP_

#include <stdint.h> /* uint8_t */

class SonicSensor
{
protected:
    static constexpr double SPEED_OF_SOUND = 0.0343; // km/h / 10

    uint8_t m_TriggerPin;
    uint8_t m_EchoPin;

    long unsigned int m_Timeout = 50000UL;

public:
    double GetDistance(void);

    SonicSensor(const uint8_t triggerPin, const uint8_t echoPin);
};

#endif // _SONICSENSOR_HPP_
