/******************************************************************
 * Name   : Quadrotor.h
 * Purpose: Header File for Quadrotor
 * 
 * Author: Akkas Uddin Haque
 * Email : akkasuddin@live.com
 *
 * Creation Date: 09/19/2015
 *******************************************************************/

#ifndef QUADROTOR_H
# define QUADROTOR_H


# include<GL/glu.h>
class Quadrotor
{
protected:

    float axisLength;
    unsigned int rotationSpeed;
    float rollAngle;
    float pitchAngle;
    float yawAngle;
    

    GLUquadric *quadricObj;
    void drawAxes();

public:


    Quadrotor();
    void rollQuad(float angle);
    void pitchQuad(float angle);
    void yawQuad(float angle);
    float getRoll();
    float getPitch();
    float getYaw();
    virtual void drawQuad() = 0;
    ~Quadrotor();
};
#endif
