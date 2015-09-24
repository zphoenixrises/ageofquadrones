#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include"Quadrotor.h"


Quadrotor::Quadrotor()
{
    axisLength = 20;
    rotationSpeed = 5;
    rollAngle = 0;
    pitchAngle = 0;
    yawAngle = 0;
    quadricObj = gluNewQuadric();
}
void Quadrotor::drawAxes()
{
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);   glVertex3f(axisLength, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);   glVertex3f(0.0f, axisLength, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);   glVertex3f(0.0f, 0.0f, axisLength);
    glEnd();
    glPopMatrix();
}

void Quadrotor::rollQuad(float angle)
{
    rollAngle += angle;
    rollAngle += (rollAngle > 360) ? -360 : ((rollAngle < 0)? 360 : 0);
}

void Quadrotor::pitchQuad(float angle)
{
    pitchAngle += angle;
    pitchAngle += (pitchAngle > 360) ? -360 : ((pitchAngle < 0)? 360 : 0);
}

void Quadrotor::yawQuad(float angle)
{
    yawAngle += angle;
    yawAngle += (yawAngle > 360) ? -360 : ((yawAngle < 0)? 360 : 0);
}

float Quadrotor::getPitch()
{
    return pitchAngle;
}

float Quadrotor::getRoll()
{
   return rollAngle;
}

float Quadrotor::getYaw()
{
   return yawAngle;
}



Quadrotor::~Quadrotor()
{
    gluDeleteQuadric(quadricObj);
}