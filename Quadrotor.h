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
#include "QuadTimer.h"
#include "Timeline.h"
#include <cstdio>
#include <vector>
#include <GL/glut.h>
#include <string>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace std;
#ifndef Pi

#define Pi 3.1415926535897932384626433832795
#endif

namespace QuadOrientationMode{
    enum Enum{FREE,ANOTHERQUAD,UPRIGHT};
}

// Virtual Class for use by all quadrotors.
// Houses common functions that govern motion and orientation of the quadrotors
class Quadrotor
{
protected:
    QuadOrientationMode::Enum orientationMode;
    QuadTimer quadTime;
    Timeline *timeline;
    
    char command[100];
    bool isExecuting;
    bool isMoving;
    bool readTimeline;
    double comTime;
    double nextTime;
    double comOrientationTime;
    double comAngle;
    double moveTime;
    char delayedCommand[250];
    Quadrotor *otherQuad;
    virtual string getName()=0;
    

    
    //double com_posx,com_posy,com_posz;
    glm::vec3 comVect;
    glm::vec3 comDirection;
    glm::vec3 comRotationAxis;
    
    glm::mat4 Model;
    float axisLength;
    unsigned int rotationSpeed;

    GLUquadric *quadricObj;
    void drawAxes();
    GLfloat pos_x,pos_y,pos_z;

public:
    
    static vector<Quadrotor*>quads;
    static Quadrotor* getQuadFromName(string quadname);


    //Contructor
    Quadrotor();
    //Applies Roll relative to the current orientation
    void rollQuad(float angle);
    //Applies Pitch relative to the current orientation
    void pitchQuad(float angle);
    //Applies Yaw relative to the current orientation
    void yawQuad(float angle);
    
    float getRoll();
    float getPitch();
    float getYaw();
    //void moveAbs(GLfloat x,GLfloat y,GLfloat z);
    void moveAbs(glm::vec3 position);
    void moveRel(glm::vec3 position);
    glm::vec3 getQuadPosition();
    void getOrientation(glm::vec3  &axisVector, glm::vec3  &upVector,glm::vec3  distanceVector);
    
    void draw();
    void executeTimeLineCommand();
    void lookAtQuad();
    virtual void drawQuad() = 0;
    virtual void collided() = 0;
    virtual glm::vec3 getBarrelPosition() = 0; 
    virtual void customCommandParser(string commandstring) = 0;
    ~Quadrotor();
};

#endif
