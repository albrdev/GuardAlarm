#ifndef _LED_HPP_
#define _LED_HPP_

#include <stdint.h> /* uint8_t */
#include <Arduino.h> /* uint8_t */

class LED
{
private:
    enum BlinkMode
    {
        BM_NONE = 0,
        BM_COUNT = 1,
        BM_TIME = 2
    };

    uint8_t m_Pin;
    bool m_State;

    unsigned int m_MaxCount;
    unsigned long int m_Duration;
    unsigned long int m_EndTime;

    unsigned int m_Count;
    unsigned long int m_StateChangePoint;
    unsigned long int m_ElapsedTime;

    bool m_InitialState;
    bool m_Active = false;
    uint8_t m_Mode = BlinkMode::BM_COUNT | BlinkMode::BM_TIME;

    void _SetState(const bool value);
    void InitBlink(const bool initialState);

public:
    bool GetState(void) const;
    void SetState(const bool value);

    void CountedBlink(const unsigned int count, const unsigned long int blinkDuration, bool initialState = false);
    void TimedBlink(const unsigned long int totalDuration, const unsigned long int blinkDuration, bool initialState = false);
    void Blink(const unsigned int count, const unsigned long int totalDuration, const unsigned long int blinkDuration, bool initialState = false);
    void Blink(const unsigned long int blinkDuration, bool initialState = false);

    void Stop(void);
    void Update(void);

    LED(const uint8_t pin, const bool state = false);
};

#endif // _LED_HPP_
