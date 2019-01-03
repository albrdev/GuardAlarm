#ifndef _SONICSENSOR_HPP_
#define _SONICSENSOR_HPP_

#include <stdint.h> /* uint8_t */

class SonicSensor
{
protected:
    static constexpr double SPEED_OF_SOUND = 0.0343; // km/h / 10

    int m_ID;
    bool m_Active;

    uint8_t m_TriggerPin;
    uint8_t m_EchoPin;

    long unsigned int m_Timeout = 50000UL;

public:
    int GetID(void) const;
    bool GetActive(void) const;
    void SetActive(const bool value);

    double GetDistance(void);

    SonicSensor(const int id, const uint8_t triggerPin, const uint8_t echoPin, const bool active = false);
};

#endif // _SONICSENSOR_HPP_
