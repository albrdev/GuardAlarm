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

long unsigned int SonicMotionSensor::GetFailureTriggerDelay(void)
{
    return m_FailureTriggerDelay;
}

void SonicMotionSensor::SetFailureTriggerDelay(const long unsigned int value)
{
    m_FailureTriggerDelay = value;
}

void SonicMotionSensor::SetTriggerCallback(void(*value)(const bool))
{
    m_TriggerCallback = value;
}

void SonicMotionSensor::SetFailureCallback(void(*value)(void))
{
    m_FailureCallback = value;
}

bool SonicMotionSensor::HasMotion(void)
{
    double distance = GetDistance();
    if(distance <= 0.0 || distance > 400.0)
    {
        if(m_FailurePoint == 0U)
        {
            m_FailurePoint = millis() + m_FailureTriggerDelay;
        }

        if(Failure() && m_FailureCallback != nullptr)
        {
            m_FailureCallback();
        }

        return false;
    }
    else
    {
        m_FailurePoint = 0UL;
    }

    double difference = diff(distance, m_MonitorDistance);
    return difference >= m_TriggerDifference;
}

void SonicMotionSensor::Update(void)
{
    bool hasMotion = HasMotion();
    if(hasMotion != m_HasMotion)
    {
        m_HasMotion = hasMotion;

        if(m_TriggerCallback != nullptr)
        {
            m_TriggerCallback(m_HasMotion);
        }
    }
}

bool SonicMotionSensor::Failure(void)
{
    return m_FailurePoint > 0UL && millis() >= m_FailurePoint;
}

SonicMotionSensor::SonicMotionSensor(const uint8_t triggerPin, const uint8_t echoPin, const double triggerDifference) : SonicSensor(triggerPin, echoPin)
{
    m_TriggerDifference = triggerDifference;
}
