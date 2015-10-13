/******************************************************************
 * Name   : NeoQuad.h
 * Purpose: Header File for NeoQuad
 * 
 * Author: Akkas Uddin Haque
 * Email : akkasuddin@live.com
 *
 * Creation Date: 09/19/2015
 *******************************************************************/

#ifndef NEOQUAD_H
#define NEOQUAD_H
#include "QuadTimer.h"
#include "Quadrotor.h"
using namespace std;

#define sqrt2 1.41421356237
enum State{NEUTRAL=1,POWERINGUP,POWEREDUP,POWERINGDOWN};

class NeoQuad:public Quadrotor
{
private:
    QuadTimer time;
    float propAngle;
    float propSpeed;
    bool animate;
    State currentState;
    
    void drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC);
    void drawBlade();
    void drawPropellers(int rotorDirection);
    void drawGuns();
    void rotateProps();
    
public:
  
    NeoQuad();
    void toggleAnimate();
    void drawQuad();
    void changePropSpeed(float increment);
    void powerToggle();
};

#endif