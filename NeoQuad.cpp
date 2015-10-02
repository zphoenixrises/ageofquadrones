/******************************************************************
 * Name   : NeoQuad.cpp
 * Purpose: NeoQuad member functions defined here
 * 
 * Author: Akkas Uddin Haque
 * Email : akkasuddin@live.com
 *
 * Creation Date: 09/19/2015
 *******************************************************************/

#include "NeoQuad.h"
#include <GL/glut.h>
#include <cmath>
using namespace std;

//#define SOLID_RENDERING 

#ifndef Pi

#define Pi 3.1415926535897932384626433832795
#endif
NeoQuad::NeoQuad()
{
    
    propAngle = 0.0;
    propSpeed = 2;
    animate = true;
    gettimeofday(&startTime,NULL);
    currentState = NEUTRAL;
}
void NeoQuad::drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC)
{
    float tStep = (Pi) / (float)uiSlices;
    float sStep = (Pi) / (float)uiStacks;
    #ifndef SOLID_RENDERING
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    #endif
    for (float t = -Pi / 2; t <= (Pi / 2) + .0001; t += tStep)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (float s = -Pi; s <= Pi + .0001; s += sStep)
        {
            glNormal3f(2 * fA * cos(t) * cos(s), 2 * fB * cos(t) * sin(s), 2 * fC * sin(t));
            glVertex3f(fA * cos(t) * cos(s), fB * cos(t) * sin(s), fC * sin(t));
            glNormal3f(2 * fA * cos(t + tStep) * cos(s), 2 * fB * cos(t + tStep) * sin(s), 2 * fC * sin(t + tStep));
            glVertex3f(fA * cos(t + tStep) * cos(s), fB * cos(t + tStep) * sin(s), fC * sin(t + tStep));
        }
        glEnd();
    }
}

void NeoQuad::drawBlade()
{
    //Draw blade
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    glVertex3f(0.5f, 10.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glEnd();
}

void NeoQuad::drawPropellers(int rotorDirection)
{
    glPushMatrix();
    static float propScale = 1;
    
    if(currentState == POWERINGUP)
    {
       propScale +=0.015;
    }
    
    if(currentState == POWERINGDOWN)
    {
        propScale -=0.015;
    }
    glScalef(propScale,propScale,propScale);
    glColor4f(.75f, .75f, .75f, 0.25f);
    gluCylinder(quadricObj, 0.25f, 0.25f, 3.0f, 10.0f, 10.0f);
    glTranslatef(0.0f, 0.0f, 3.0f);
    #ifndef SOLID_RENDERING
    glutWireTorus(.5f, 10.0f, 10.0f, 30.0f);
    #else
    glutSolidTorus(.5f, 10.0f, 10.0f, 30.0f);    
    #endif
    
    if (rotorDirection)
        glRotatef(propAngle, 0.0f, 0.0f, 1.0f);
    else
        glRotatef(-propAngle, 0.0f, 0.0f, 1.0f);
    
    drawBlade();
    glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
    drawBlade();
    glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
    drawBlade();
    
    glPopMatrix();
    
}

void NeoQuad::drawGuns()
{
    static float gunScale = 0;
    
    if(currentState == POWERINGUP)
    {
        gunScale +=0.1;
        if((int)gunScale == 1)
            currentState = POWEREDUP;
    }
    
    if(currentState == POWERINGDOWN)
    {
        gunScale -=0.1;
        if((int)(gunScale*10) == 0)
            currentState = NEUTRAL;
    }
    
    //drawGuns
    glPushMatrix();
    glColor3f(0.5,0.5,0.5);
    glTranslatef(-7.0f,0.0f,6.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    
    gluDisk(quadricObj,0.0f,2.0f,10,3);
    gluCylinder(quadricObj, 2.0f, 2.0f, 15.0f, 10.0f, 10.0f);
    glTranslatef(0.0f,0.0f,15.0f);
    
    glScalef(1.0f,1.0f,gunScale);
    gluDisk(quadricObj,1.0f,2.0f,10,3);
    gluCylinder(quadricObj, 1.0f, 1.0f, 10.0f, 10.0f, 10.0f);
    glTranslatef(0.0f,0.0f,10.0f);
    gluDisk(quadricObj,.5f,1.0f,10,3);
    gluCylinder(quadricObj, .5f, .5f, 10.0f, 10.0f, 10.0f);
    glPopMatrix();
    
    //drawAxes();
    glColor3f(0.5,0.5,0.5);
    glTranslatef(-7.0f,0.0f,-6.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluDisk(quadricObj,0.0f,2.0f,10,3);
    gluCylinder(quadricObj, 2.0f, 2.0f, 15.0f, 10.0f, 10.0f);
    glTranslatef(0.0f,0.0f,15.0f);
    
    glScalef(1.0f,1.0f,gunScale);
    gluDisk(quadricObj,1.0f,2.0f,10,3);
    gluCylinder(quadricObj, 1.0f, 1.0f, 10.0f, 10.0f, 10.0f);
    glTranslatef(0.0f,0.0f,10.0f);
    gluDisk(quadricObj,.5f,1.0f,10,3);
    gluCylinder(quadricObj, .5f, .5f, 10.0f, 10.0f, 10.0f);
}


void NeoQuad::drawQuad()
{
    rotateProps();
    glPushMatrix();//push current matrix to stack
    glRotatef(yawAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(rollAngle, 1.0f, 0.0f, 0.0f);
    glRotatef(pitchAngle, 0.0f, 0.0f, 1.0f);
    
    
    //Draw Axes
    //drawAxes();
    
    glColor4f(.75f, .75f, .75f, 0.25f);
    
    //draw body
    drawEllipsoid(10, 10, 15, 5, 5);
    glTranslatef(15.0f, 0.0f, 0.0f);
    gluSphere(quadricObj, 5.0f, 20, 20); //draw head
    glTranslatef(-15.0f, 0.0f, 0.0f);
    
    //draw Arms
    glPushMatrix();    
    glTranslatef(20.0f, 5.0f, 20.0f);
    glRotatef(-135.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadricObj, 0.25f, 0.25f, 40.0f*sqrt2, 10.0f, 10.0f);
    
    
    glTranslatef(20.0f*sqrt2,0.0, 20.0f*sqrt2);
    glRotatef(90.0f, 0.0, -1.0f, 0.0f);
    //drawAxes();
    gluCylinder(quadricObj, 0.25f, 0.25f, 40.0f*sqrt2, 10.0f, 10.0f);
    //Draw 
    
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    //drawAxes();
    for(int i=0;i<4;i++)
    {
        drawPropellers(i%2);
        glTranslatef(20.0f*sqrt2, -20.0f*sqrt2, 0.0f);
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    }
    
    glPopMatrix();
    
    if(currentState != NEUTRAL)
    {
        drawGuns();
 
    }
    
    
    
    glPopMatrix();
}


void NeoQuad::rotateProps()
{
    if (animate)
    {
        double elapsedTime;
        timeval t2;
        gettimeofday(&t2,NULL);
        
        elapsedTime = (t2.tv_sec - startTime.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (t2.tv_usec - startTime.tv_usec) / 1000.0;   // us to ms
        
        propAngle += propSpeed*elapsedTime;
    }
    gettimeofday(&startTime,NULL);
    
}

void NeoQuad::toggleAnimate()
{
    animate = !animate;
}

void NeoQuad::changePropSpeed(float increment)
{
    propSpeed+=increment;
}


void NeoQuad::powerToggle()
{
    if(currentState == NEUTRAL)
        currentState = POWERINGUP;
    else if(currentState == POWEREDUP)
        currentState = POWERINGDOWN;
}
