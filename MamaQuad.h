/******************************************************************
 * Name: MamaQuad.h
 * Purpose: MamaQuad class is defined here
 * 
 * Author: Steve Beuttner
 * Email : 
 *
 * Creation Date: 
 *******************************************************************/


#ifndef MamaQuad_H
#define MamaQuad_H
#include "QuadTimer.h"
#include "Quadrotor.h"

using namespace std;

class MamaQuad:public Quadrotor
{
private:
    
    QuadTimer time;
    float propAngle;
    float propSpeed;
    bool animate;
    
    void drawBlade();
    void drawProparms(int rotorDirection);
    void rotateProps();
    
    
    
    int rotPropeller;
    int rotPropellerLeft;
    GLUquadric *quad;
    void mamaBody(void);
    void motor();
    void upRightPropeller();
    void upLeftPropeller();
    void lowLeftPropeller();
    void lowRightPropeller();
    void arm();
    
public:
    MamaQuad();
    void toggleAnimate();
    //TODO: Steve-> Please solve issues with the drawing of mamaquad
    void drawQuad();
    //TODO: Steve-> Please implement this function to change your propeller speed
    void changePropSpeed(float increment);
    ~MamaQuad();
 
};

#endif
