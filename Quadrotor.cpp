/******************************************************************
 * Name   : Quadrotor.cpp
 * Purpose: Quadrotor member functions defined here
 * 
 * Author: Akkas Uddin Haque
 * Email : akkasuddin@live.com
 *
 * Creation Date: 09/19/2015
 *******************************************************************/

#include "Settings.h"

#include<GL/glut.h>
#include"Quadrotor.h"
#include <glm/gtx/vector_angle.hpp>

#include "raygl/raygl.h"
#include "raygl/raygldefs.h"
vector<Quadrotor*>Quadrotor::quads;

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
    quads.push_back(this);
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
    //glLoadIdentity();
    executeTimeLineCommand();
    glTranslatef(pos_x, pos_y, pos_z);
    
    drawQuad();
    glPopMatrix();
}

glm::mat4 getOrientationMatrix(glm::vec3 &eye,glm::vec3 &center,glm::vec3 &up)
{
    glm::mat4 mm =  glm::lookAt(eye,center,up);
    mm = glm::inverse(mm);
    mm[2][0] = -mm[2][0];
    mm[2][1] = -mm[2][1];
    mm[2][2] = -mm[2][2];
    
    
    mm[0][0] = -mm[0][0];
    mm[0][1] = -mm[0][1];
    mm[0][2] = -mm[0][2]; 
    return mm;
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

            sscanf(delayedCommand,"%lf %lf %s",&nextTime,&comTime,command);
            //printf("\n%s",command);
            //printf("\n%s",delayedCommand);
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
                {  }
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
            
            else if(!strcmp(command,"LOOKATQUAD"))
            {
                char quadname[10];
                sscanf(delayedCommand,"%lf %lf %s %s",&nextTime,&comTime,command, quadname);
                int i;
                for(i=0;i<quads.size();i++)
                {
                    if(quads[i]->getName() == string(quadname))
                        break;
                }
                otherQuad = quads[i];
                orientationMode = QuadOrientationMode::ANOTHERQUAD;
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
            if(timediff>delta_time) timediff = delta_time;
            float axisAngle = glm::angle(current_axis,comDirection);
        //    printf("\nX: %f",angle_axis);
            if(axisAngle>Pi)
            {
                printf("\n%f",axisAngle);
                axisAngle = (2*Pi-axisAngle);
                
            }
            axisAngle = axisAngle*(float) (timediff/ delta_time);
            Model = glm::rotate(Model,axisAngle,comRotationAxis);
            //*
            glm::vec3 eye = glm::vec3(0,0,0), 
            center = glm::vec3( Model*glm::vec4(1.0f,0.0f,0.0f,1.0f)),
            up = glm::vec3(0,1,0); 
             
            glm::mat4 mm = getOrientationMatrix(eye,center,up);
            Model = mm;
            Model = glm::rotate(Model,-glm::half_pi<float>(),glm::vec3(0,1,0));
            //*/

        }
        else if(current_time>=nextTime)
        { isExecuting = false;isMoving=false;}
        else if(current_time>=comTime)
        {
            if(!strcmp(command,"MOV"))
            {
                glm::vec3 distance = glm::vec3(comVect.x-pos_x,comVect.y-pos_y,comVect.z-pos_z);
                
                if(orientationMode == QuadOrientationMode::ANOTHERQUAD)
                {  
                    glm::vec3 otherQuadPosition = otherQuad->getQuadPosition();
                    glm::vec3 otherDirectionRelative = otherQuadPosition - getQuadPosition();
                      
 
                    glm::vec3 upVector,tempVector; 
                    //getOrientation();
                    glm::vec3 eye = glm::vec3(0,0,0),center = otherDirectionRelative,up = glm::vec3(0,1,0);
                    
                    glm::mat4 mm = getOrientationMatrix(eye,center,up);
                    Model = mm;
                    Model = glm::rotate(Model,-glm::half_pi<float>(),glm::vec3(0,1,0));
                    
                    
                }
                else 
                {
                 //*     
                   glm::vec3 eye = glm::vec3(0,0,0),
                    center = comDirection,
                    up = glm::vec3(0,1,0);
                    
                    glm::mat4 mm = getOrientationMatrix(eye,center,up);
                    Model = mm;
                    Model = glm::rotate(Model,-glm::half_pi<float>(),glm::vec3(0,1,0));
                  //*/  
                }
                 double delta_time = nextTime-current_time;
                 double timediff = quadTime.getTimeDiffSec();
                 if(delta_time < timediff)
                     timediff=delta_time;
                glm::vec3 dist2 = distance *(float) (timediff/ delta_time);
                moveRel(dist2);
                //     printf("\ndelta pos:%f %f %f",dist2.x,dist2.y,dist2.z);
            }
        }
         
         
          
        //   printf("\ncurrent pos:%f %f %f",pos_x, pos_y,pos_z);
    }
    //*/
}

Quadrotor* Quadrotor::getQuadFromName(string quadname)
{
    for(vector<Quadrotor*>::iterator it = quads.begin(); it!=quads.end(); ++it)
    {
        if((*it)->getName() == quadname)
            return *it;
    }

}


Quadrotor::~Quadrotor()
{
    gluDeleteQuadric(quadricObj);
    delete timeline;
}