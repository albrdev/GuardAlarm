#ifndef _SONICMOTIONSENSOR_HPP_
#define _SONICMOTIONSENSOR_HPP_

#include "SonicSensor.hpp"

class SonicMotionSensor : public SonicSensor
{
private:
    double m_MonitorDistance = 0.0;
    double m_TriggerDifference;
    long unsigned int m_FailureTriggerDelay = 1000UL;
    long unsigned int m_FailurePoint = 0UL;
    bool m_HasMotion = false;

    void(*m_TriggerCallback)(const bool) = nullptr;
    void(*m_FailureCallback)(void) = nullptr;

public:
    double GetMonitorDistance(void);
    void SetMonitorDistance(const double value);
    void SetMonitorDistance(void);

    double GetTriggerDifference(void);
    void SetTriggerDifference(const double value);

    long unsigned int GetFailureTriggerDelay(void);
    void SetFailureTriggerDelay(const long unsigned int value);

    void SetTriggerCallback(void(*value)(const bool));
    void SetFailureCallback(void(*value)(void));

    bool HasMotion(void);
    void Update(void);
    bool Failure(void);

    SonicMotionSensor(const uint8_t triggerPin, const uint8_t echoPin, const double triggerDifference);
};

#endif // _SONICMOTIONSENSOR_HPP_
