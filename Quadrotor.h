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

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#ifndef Pi

#define Pi 3.1415926535897932384626433832795
#endif
// Virtual Class for use by all quadrotors.
// Houses common functions that govern motion and orientation of the quadrotors
class Quadrotor
{
protected:
    
    glm::mat4 Model;
    float axisLength;
    unsigned int rotationSpeed;

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
