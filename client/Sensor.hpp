#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include <stdint.h> /* uint8_t */

class Sensor
{
private:
    int m_ID;
    bool m_Active;

    uint8_t m_Pin;
    bool m_State;

public:
    int GetID(void) const;
    bool GetActive(void) const;
    void SetActive(const bool value);
    bool GetState(void) const;

    Sensor(const int id, const uint8_t pin, const uint8_t mode, const bool active = false);
};

#endif // _SENSOR_HPP_
