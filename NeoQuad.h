#ifndef NEOQUAD_H
#define NEOQUAD_H

#include <ctime>
#include <sys/time.h>
#include "Quadrotor.h"

using namespace std;

class NeoQuad:public Quadrotor
{
private:
    
    timeval startTime;
    float propAngle;
    unsigned int propSpeed;
    bool animate;
    
    void drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC);
    void drawBlade();
    void drawPropellers(int rotorDirection);
    void rotateProps();
    
public:
    char difftime[6];
    NeoQuad();
    void toggleAnimate();
    void drawQuad();
};

#endif