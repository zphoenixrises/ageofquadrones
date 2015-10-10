
#include <QuadTimer.h>


double QuadTimer::StartTime = GetCurrentSystemTime();




QuadTimer::QuadTimer()
{
    time = GetCurrentSystemTime();
}

double QuadTimer::GetProcessTime()
{
    return GetCurrentSystemTime() - StartTime;
}

double QuadTimer::getTimeDiffSec()
{

    double time2 = GetCurrentSystemTime();
    double diffTime = time2 - time;
    time = time2;
    return diffTime;
}

double QuadTimer::GetCurrentSystemTime()
{
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

