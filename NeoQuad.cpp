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
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <glm/gtc/type_ptr.hpp>
using namespace std;

#define SOLID_RENDERING 

NeoQuad::NeoQuad()
{
    propAngle = 0.0;
    propSpeed = 2;
    animate = true;
    currentState = NEUTRAL;
}
void NeoQuad::drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC)
{
    glPushMatrix();
    glScalef(fA,fB,fC);
    
    gluSphere(quadricObj,1.0f,uiSlices,uiStacks);
    glPopMatrix();
  
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
    //Draw blade
    
    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    glVertex3f(-0.5f, 10.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
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
    //drawAxes();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quadricObj,0.0f,2.0f,10,3);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    
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
    
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quadricObj,0.0f,2.0f,10,3);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    
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
  
    glMultMatrixf(glm::value_ptr(Model)); //load Model matrix
    
    //Draw Axes
    //drawAxes();
    
    glColor4f(.75f, .75f, .75f, 0.25f);
    
    //draw body
    drawEllipsoid(20, 20, 15, 5, 5);
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
    
    
    
}


void NeoQuad::rotateProps()
{
    if (animate)
    {
        
        propAngle += propSpeed*time.getTimeDiffSec()*1000;
    }
    time.getTimeDiffSec();
    
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
