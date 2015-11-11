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
    readTimeline = true;
    isExecuting = false;
    isMoving = false;
    orientationMode = QuadOrientationMode::FREE;
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
    Model = glm::rotate(Model, xx, glm::vec3(1, 0, 0)); 
}

void Quadrotor::pitchQuad(float angle)
{
    
    float xx = angle * Pi/180;
    Model = glm::rotate(Model, xx, glm::vec3(0,0, 1)); 
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

void Quadrotor::moveAbs(glm::vec3 position)
{
    pos_x = position.x;
    pos_y = position.y;
    pos_z = position.z;

}

void Quadrotor::moveRel(glm::vec3 position)
{
    pos_x += position.x;
    pos_y += position.y;
    pos_z += position.z;
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
    executeTimeLineCommand();
    glTranslatef(pos_x, pos_y, pos_z);
    
    drawQuad();
    glPopMatrix();
}

void Quadrotor::executeTimeLineCommand()
{
    //*
   //return;
    if(readTimeline)
    {
        double current_time = QuadTimer::GetProcessTime();
        

        
        if(!isExecuting)
        {
            char command[100];
            char* commandstr;
            double peektime = -1;
            //double com_time;
            commandstr = timeline->readNextCommand(peektime);
            //read the command type
            if(sscanf(commandstr,"%lf %s",&comTime,command)==EOF)
            {readTimeline = false; return;}
            
            sprintf(delayedCommand,"%lf %s",peektime,commandstr);
            isExecuting = true;
        }
        else if(current_time>=comTime && !isMoving)
        {
            //command to set quad position
            char command[100];
            sscanf(delayedCommand,"%lf %lf %s",&nextTime,&comTime,command);
            printf("\n%s",command);
            printf("\n%s",delayedCommand);
            comOrientationTime = 0;
            if(!strcmp(command,"POS"))
            {
                sscanf(delayedCommand,"%lf %lf %s %f %f %f",&nextTime,&comTime, command,&comVect.x,&comVect.y,&comVect.z);
                moveAbs(comVect)  ;
                
            }
            else if(!strcmp(command,"ROT"))
            {
                
                sscanf(delayedCommand,"%lf %lf %s %f %f %f",&nextTime,&comTime, command,&comVect.x,&comVect.y,&comVect.z); 
                
            }
            
            else if(!strcmp(command,"MOV"))
            {
                
                sscanf(delayedCommand,"%lf %lf %s %lf %f %f %f",&nextTime,&comTime, command,&comOrientationTime,&comVect.x,&comVect.y,&comVect.z); 
                isExecuting = true;
                quadTime.getTimeDiffSec();
                comOrientationTime = current_time + comOrientationTime;
                if(orientationMode == QuadOrientationMode::FREE)
                {}
                else if(orientationMode == QuadOrientationMode::ANOTHERQUAD)
                {
                    
                }
                else if(orientationMode == QuadOrientationMode::UPRIGHT)
                {}
                //Operations to align the quad with the direction of motion 
                
                comDirection = glm::vec3(comVect.x-pos_x,comVect.y-pos_y,comVect.z-pos_z);
                comDirection = glm::normalize(comDirection);
                
                
                /*glm::vec3 x_axis(1.0f,0.0f,0.0f);
                 *        glm::vec3 rotation_axis = glm::cross(comDirection,x_axis);
                 *        
                 *        
                 *        rotation_axis = glm::normalize(rotation_axis);
                 *        float angle = glm::angle(x_axis,comDirection);
                 *        Model = glm::rotate(glm::mat4(1.0f), -angle, rotation_axis);
                 */
            }    
            //Operations to align the quad with the direction of motion
            isMoving = true;
            //   printf("\nGot command:%f %f %f %f",com_time,com_posx,com_posy,com_posz);
            
        }
         
        else if(current_time<=comOrientationTime) //dont have to worry about modes other than MOV
        {
            
            glm::vec3 current_axis, current_up;
            double delta_time = comOrientationTime-current_time;
            getOrientation(current_axis,current_up,glm::vec3(1.0,0.0,0.0));
            
            current_axis = glm::normalize(current_axis);
            comRotationAxis = glm::cross(current_axis,comDirection);
             
            double timediff =   quadTime.getTimeDiffSec();
            float angle_axis = glm::angle(current_axis,comDirection);
        //    printf("\nX: %f",angle_axis);
            if(angle_axis>Pi)
            {
                printf("\n%f",angle_axis);
                angle_axis = (2*Pi-angle_axis);
                
            }
            angle_axis = angle_axis*(float) (timediff/ delta_time);
            Model = glm::rotate(Model,angle_axis,comRotationAxis);
            
          //  Model = glm::rotate(Model,float(1*Pi/180),glm::vec3(0, 1, 0));
            
            /* 
            getOrientation(current_axis,current_up,glm::vec3(1.0,0.0,0.0));
            current_axis = glm::normalize(current_axis);
            glm::vec3 worldy(0,1,0);
            
            glm::vec3 perpxWy = glm::normalize( glm::cross(current_axis,worldy));
            
            //current_up = glm::normalize(current_up);
            glm::vec3 desiredUp = glm::normalize( glm::cross(perpxWy,current_axis));
            //comRotationAxis = glm::cross(current_up, desiredUp);
            
            angle_axis = glm::angle(current_up,desiredUp);
            if(angle_axis<=0.01)
                return;
            printf("  Y:  %f\n",angle_axis);
            if(angle_axis>Pi)
            {
                printf("\n%f",angle_axis);
                angle_axis = (2*Pi-angle_axis);
                
            }
            angle_axis = angle_axis*(float) (timediff/ delta_time);
            Model = glm::rotate(Model,angle_axis,glm::vec3(1.0,0,0));
           // */
        }
        else if(current_time>=nextTime)
        { isExecuting = false;isMoving=false;}
        else if(current_time>=comTime)
        {
             
           
            glm::vec3 distance = glm::vec3(comVect.x-pos_x,comVect.y-pos_y,comVect.z-pos_z);
            double delta_time = nextTime-current_time;
            glm::vec3 dist2 = distance *(float) (quadTime.getTimeDiffSec()/ delta_time);
            moveRel(dist2);
            //     printf("\ndelta pos:%f %f %f",dist2.x,dist2.y,dist2.z);
            
        }
        
        
        
        //   printf("\ncurrent pos:%f %f %f",pos_x, pos_y,pos_z);
    }
    //*/
}



Quadrotor::~Quadrotor()
{
    gluDeleteQuadric(quadricObj);
    delete timeline;
}