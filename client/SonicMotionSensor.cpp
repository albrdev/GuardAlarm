#include "SonicMotionSensor.hpp"
#include <Arduino.h>

#define diff(a, b) (abs((a) - (b)))

double SonicMotionSensor::GetMonitorDistance(void)
{
    return m_MonitorDistance;
}

void SonicMotionSensor::SetMonitorDistance(const double value)
{
    m_MonitorDistance = value;
}

void SonicMotionSensor::SetMonitorDistance(void)
{
    m_MonitorDistance = GetDistance();
}

double SonicMotionSensor::GetTriggerDifference(void)
{
    return m_TriggerDifference;
}

void SonicMotionSensor::SetTriggerDifference(const double value)
{
    m_TriggerDifference = value;
}

MotionSensorState SonicMotionSensor::GetState(void)
{
    double distance = GetDistance();
    if(distance <= 0.0)
        return MotionSensorState::MSS_FAILURE;

    double difference = diff(distance, m_MonitorDistance);
    return difference >= m_TriggerDifference ? MotionSensorState::MSS_TRIGGERED : MotionSensorState::MSS_IDLE;
}

SonicMotionSensor::SonicMotionSensor(const int id, const uint8_t triggerPin, const uint8_t echoPin, const double triggerDifference, const bool active) : SonicSensor(id, triggerPin, echoPin, active)
{
    m_TriggerDifference = triggerDifference;
}
