
#include "QuadTimer.h"


double QuadTimer::StartTime = GetCurrentSystemTime();
int QuadTimer::fps = 20;
double QuadTimer::ProcessTime = 0;



QuadTimer::QuadTimer()
{
    //time = GetCurrentSystemTime();
    time = 0;
    //fps = framesPerSec;
}

double QuadTimer::GetProcessTime()
{
    //return GetCurrentSystemTime() - StartTime;
    return ProcessTime;
}

double QuadTimer::getTimeDiffSec()
{

    //double time2 = GetCurrentSystemTime();
    //double diffTime = time2 - time;
    //time = time2;
    //return diffTime;
    //time+=60.0/framesPerSec;
    return 1.0/fps;
}

void QuadTimer::updateProcessTime()
{
    ProcessTime+=1.0/fps;
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

void QuadTimer::initializeTimer(double seconds)
{
   // StartTime = GetCurrentSystemTime()-seconds;
   // time = GetCurrentSystemTime();
    //StartTime = 
    ProcessTime = seconds;
    

}
