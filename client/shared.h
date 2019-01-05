#ifndef _SHARED_H_
#define _SHARED_H_

enum SensorStatus
{
    SS_FAILURE = -1,
    SS_CLOSED = 0,
    SS_OPEN = 1
};

const unsigned int LOGINATTEMPTS_MAX = 3;

#endif // _SHARED_H_
