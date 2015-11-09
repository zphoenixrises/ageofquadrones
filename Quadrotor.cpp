/******************************************************************
 * Name   : Quadrotor.cpp
 * Purpose: Quadrotor member functions defined here
 * 
 * Author: Akkas Uddin Haque
 * Email : akkasuddin@live.com
 *
 * Creation Date: 09/19/2015
 *******************************************************************/


#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include"Quadrotor.h"
#include <glm/gtx/vector_angle.hpp>


Quadrotor::Quadrotor()
{
    Model = glm::mat4(1.0f);
    axisLength = 20;
    rotationSpeed = 5;
    quadricObj = gluNewQuadric();
    gluQuadricNormals(quadricObj, GLU_SMOOTH);   // Create Smooth Normals ( NEW )
    gluQuadricTexture(quadricObj, GL_TRUE);      // Create Texture Coords ( NEW )
    pos_x = pos_y = pos_z = 0.0f;
    turnoff = false;
    isInMotion = false;
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
    
    float xx = angle * Pi/180;
    Model = glm::rotate(Model, xx, glm::vec3(0, 0, 1)); 
}

void Quadrotor::pitchQuad(float angle)
{
    
    float xx = angle * Pi/180;
    Model = glm::rotate(Model, xx, glm::vec3(1,0, 0)); 
}

void Quadrotor::yawQuad(float angle)
{
    
    float xx = angle * Pi/180;
    Model = glm::rotate(Model, xx, glm::vec3(0, 1, 0));   
}

float Quadrotor::getPitch()
{
    return 0;
}

float Quadrotor::getRoll()
{
    return 0;
}

float Quadrotor::getYaw()
{
    return 0;
}

void Quadrotor::moveAbs(GLfloat x, GLfloat y, GLfloat z)
{
    pos_x = x;
    pos_y = y;
    pos_z = z;

}

void Quadrotor::moveRel(GLfloat x, GLfloat y, GLfloat z)
{
    pos_x += x;
    pos_y += y;
    pos_z += z;
}

glm::vec3 Quadrotor::getQuadPosition()
{
    //get position
    return glm::vec3(pos_x,pos_y,pos_z);
    
}

void Quadrotor::getOrientation(glm::vec3 &axisVector, glm::vec3 &upVector,glm::vec3  distanceVector)
{
    //get the camera distance vector relative to the position of the quadcopter
    axisVector = glm::vec3( Model*glm::vec4(distanceVector,1.0f));
    //get the up vector relative to the orientation of the quadcopter
    upVector = glm::vec3( Model*glm::vec4(0.0f,1.0f,0.0f,1.0f));//up is in +y for the quadcopter
    

}




void Quadrotor::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); //Main push
    glLoadIdentity();
    move();
    glTranslatef(pos_x, pos_y, pos_z);
    
    drawQuad();
    glPopMatrix();
}

void Quadrotor::move()
{
    //*
   //return;
    if(!turnoff)
    {
        double current_time = QuadTimer::GetProcessTime();

        
        if(!isInMotion)
        {
            
            if(sscanf(timeline->readNextCommand(),"%lf %lf %lf %lf",&com_time,&com_posx,&com_posy,&com_posz)==EOF)
            {turnoff = true; return;}
            isInMotion = true;
            quadTime.getTimeDiffSec();
            
            //Operations to align the quad with the direction of motion 
            glm::vec3 direction(com_posx-pos_x,com_posy-pos_y,com_posz-pos_z);
            glm::vec3 x_axis(1.0f,0.0f,0.0f);
            direction = glm::normalize(direction);
            glm::vec3 rotation_axis = glm::cross(direction,x_axis);
            
            
            rotation_axis = glm::normalize(rotation_axis);
            float angle = glm::angle(x_axis,direction);
            Model = glm::rotate(glm::mat4(1.0f), -angle, rotation_axis);
            //Operations to align the quad with the direction of motion
            
            //   printf("\nGot command:%f %f %f %f",com_time,com_posx,com_posy,com_posz);
        }
        else if(current_time<=com_time)
        {
            
           
            glm::vec3 distance = glm::vec3(com_posx-pos_x,com_posy-pos_y,com_posz-pos_z);
            double delta_time = com_time-current_time;
            glm::vec3 dist2 = distance *(float) (quadTime.getTimeDiffSec()/ delta_time);
            moveRel(dist2.x,dist2.y,dist2.z);
            //     printf("\ndelta pos:%f %f %f",dist2.x,dist2.y,dist2.z);
            
        }
        else
            isInMotion = false;
        
        
        
        //   printf("\ncurrent pos:%f %f %f",pos_x, pos_y,pos_z);
    }
    //*/
}



Quadrotor::~Quadrotor()
{
    gluDeleteQuadric(quadricObj);
    delete timeline;
}