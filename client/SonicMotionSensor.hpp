#ifndef _SONICMOTIONSENSOR_HPP_
#define _SONICMOTIONSENSOR_HPP_

#include "SonicSensor.hpp"

enum MotionSensorState
{
    MSS_FAILURE = -1,
    MSS_IDLE = 0,
    MSS_TRIGGERED = 1
};

class SonicMotionSensor : public SonicSensor
{
private:
    double m_MonitorDistance = 0.0;
    double m_TriggerDifference;

public:
    double GetMonitorDistance(void);
    void SetMonitorDistance(const double value);
    void SetMonitorDistance(void);

    double GetTriggerDifference(void);
    void SetTriggerDifference(const double value);

    MotionSensorState GetState(void);

    SonicMotionSensor(const int id, const uint8_t triggerPin, const uint8_t echoPin, const double triggerDifference, const bool active = false);
};

#endif // _SONICMOTIONSENSOR_HPP_
