/* Author- Ian Nordeng
 * Header File for Dronedemort
   Last Updated: 10/10/2015
   Code created as a homework for CSci 446
   */


#ifndef Drondemort_H
#define Drondemort_H
#include "QuadTimer.h"
#include "Quadrotor.h"

using namespace std;

class Dronedemort:public Quadrotor
{
private:
 
	QuadTimer time;
	float propAngle;
	float propSpeed;
	bool animate;
 
	void drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC);
	void drawBlade();
	void drawProparms(int rotorDirection);
	void rotateProps();

public:
	Dronedemort();
	void toggleAnimate();
	void drawQuad();
	void changePropSpeed(float increment);
	
};

#endif
