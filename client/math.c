#include "math.h"

int lerp(int start, int end, float percent)
{
    return (start + percent * (end - start));
}
