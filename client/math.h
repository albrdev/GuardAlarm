#ifndef _MATH_H_
#define _MATH_H_

#include <stdint.h> /* uint8_t, uint16_t */
#include <string.h> /* size_t, memset(), memcpy(), strlen() */
#include <math.h>
#ifdef __AVR__
#include <Arduino.h>
#endif

#define diff(a, b) (abs((a) - (b)))

#ifdef __cplusplus
extern "C"
{
#endif
    int lerp(int start, int end, float percent);
#ifdef __cplusplus
} // extern "C"
#endif

#endif // _MATH_H_
