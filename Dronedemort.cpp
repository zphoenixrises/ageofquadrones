/* Author- Ian Nordeng
   Last Updated: 10/8/2015
   Code created as a homework for CSci 446
   */

#include "Dronedemort.h"
#include <GL/glut.h>
#include <cmath>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <glm/gtc/type_ptr.hpp>
using namespace std;
#ifndef Pi

#define Pi 3.1415926535897932384626433832795
#endif

Dronedemort::Dronedemort()
	{
	      
	    propAngle = 0.0;
	    propSpeed = 2;
	    animate = true;
	}

void Dronedemort::drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC)
{
    glPushMatrix();
    glScalef(fA,fB,fC);
    gluSphere(quadricObj,1.0f,uiSlices,uiStacks);
    glPopMatrix();
}


	

void Dronedemort::drawBlade()
{
	//Draw blade
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.12f,0.12f, 0.12f, 0.25f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 15.0f, 0.0f);
	glVertex3f(0.0f, 15.0f, 0.0f);
	glVertex3f(0.5f, 15.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glEnd();
	
	glBegin(GL_TRIANGLE_FAN);
	
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 15.0f, 0.0f);
	glVertex3f(0.0f, 15.0f, 0.0f);
	glVertex3f(-0.5f, 15.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
}

void Dronedemort::drawProparms(int rotorDirection)
{
	//Draw rotor arm
	glPushMatrix();
	glColor4f(0.12f, 0.12f, 0.12f, 0.25f);
	glRotatef(45.0f,-1.0f,0.0f,0.0f);
	gluCylinder(quadricObj,1.5f,1.5f,20.0f,10.0f,10.0f);
	glTranslatef(0.0f,0.0f,19.5f);
	gluSphere(quadricObj, 2.0f, 15.0f, 10.0f);
	glRotatef(45.0f,-1.0f,0.0f,0.0f);
	glTranslatef(0.0f,0.0f,-0.5f);
	gluCylinder(quadricObj,1.5f,1.5f,5.0f,10.0f,10.0f);
	glTranslatef(0.0f,0.0f,5.0f);
	gluDisk(quadricObj,0,1.5,10,5);
	glTranslatef(0.0f,0.0f,-1.f);
	
	if (rotorDirection)
	   glRotatef(propAngle,0.0f,0.0f,1.0f);
	else
	  glRotatef(-propAngle,0.0f,0.0f,1.0f);
	
	drawBlade();
	glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	drawBlade();
	glPopMatrix();
	
	
}


void Dronedemort::drawQuad()
{
	rotateProps();
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glMultMatrixf(glm::value_ptr(Model)); //load Model glLoadMatrixd
	
	//drawAxes();
	glRotatef(90.0f,0.0f,1.0f,0.0f);
	glPushMatrix();//Push for main body and eye
	glColor4f(0.12f, 0.12f, 0.12f, 0.25f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//Draw main body
	gluSphere(quadricObj, 25.0f, 25.0f, 25.0f);
	
	//Draw eye
	glColor4f(0.6f, 0.0f, 0.0f, 0.25f);
	glTranslatef(0.0f,-6.4f,23.5f);
	drawEllipsoid(20.0f,20.0f,10.0f,3.0f,3.0f);
	glPopMatrix();//Pop for main body and eye
	
	//Draw laser gun
	glColor4f(0.12f, 0.12f, 0.12f, 0.25f);
	glPushMatrix();//Push for gun
	glRotatef(180.0f,0.0f,0.0f,1.0f);
	glTranslatef(0.0f,24.5f,0.0f);
	glRotatef(90.0f,-1.0f,0.0f,0.0f);
	gluCylinder(quadricObj,2.0f,2.0f,10.0f,10.0f,10.0f);
	glTranslatef(0.0f,0.5f,8.0f);
	
	//
	
	gluSphere(quadricObj, 5.0f, 15.0f, 10.0f);
	
	//
	
	glRotatef(60.0f,1.0f,0.0f,0.0f);
	gluCylinder(quadricObj,2.0f,2.0f,15.0f,15.0f,10.0f);
	glTranslatef(0.0f,0.0f,15.0f);
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
	gluDisk(quadricObj,0,1.5,15,5);
	glColor4f(0.12f, 0.12f, 0.12f, 0.25f);
	gluDisk(quadricObj,1.5,2.0,15,5);
	glPopMatrix();//pop for gun
	
	//Draw rotor arms
	glPushMatrix(); //Push arm1
	glRotatef(45.0f,0.0f,1.0f,0.0f);
	glTranslatef(0.0f,0.0f,23.5f);
	drawProparms(0);
	glPopMatrix(); //Pop arm1
	glPushMatrix();//push arm2
	glRotatef(135.0f,0.0f,1.0f,0.0f);
	glTranslatef(0.0f,0.0f,23.5f);
	drawProparms(1);
	glPopMatrix(); //pop arm2
	glPushMatrix();//push arm3
	glRotatef(225.0f,0.0f,1.0f,0.0f);
	glTranslatef(0.0f,0.0f,23.5f);
	drawProparms(0);
	glPopMatrix();//pop arm3
	glPushMatrix();//push arm 4
	glRotatef(315.0f,0.0f,1.0f,0.0f);
	glTranslatef(0.0f,0.0f,23.5f);
	drawProparms(1);
	glPopMatrix();//pop arm 4
	
	//glPopMatrix(); //Main pop
}
	
void Dronedemort::rotateProps()
{
	if (animate)
	{
		propAngle += propSpeed*time.getTimeDiffSec()*1000;
	}
	time.getTimeDiffSec();
 
}

void Dronedemort::toggleAnimate()
{
	animate = !animate;
}

void Dronedemort::changePropSpeed(float increment)
{
  propSpeed+=increment;
}
	
