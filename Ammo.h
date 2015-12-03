/******************************************************************
 * Name   : Ammo.h
 * Purpose: Header File for Ammo
 * 
 * Author: Akkas Uddin Haque
 * Email : akkasuddin@live.com
 *
 * Creation Date: 11/30/2015
 *******************************************************************/
#ifndef AMMO_H
#define AMMO_H
#include <vector>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "Quadrotor.h"
#include <string>

using namespace std;
#define COLLISIONRADIUS 20

namespace AMMOTYPE{
    enum ENUM{
        CANNONBALL,LASER,BLASTER
    };
    ENUM getAmmotypeFromString(string ammoType);

}

class Ammo
{
private:
    static vector<Ammo*> bullets;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec4 color;    
    glm::mat4 orientationMatrix;
    float lifetime;
    float startTime;
    GLUquadric *quadricObj;
    QuadTimer time;
    Quadrotor*owner;
    AMMOTYPE::ENUM ammoType;
    
public:
    Ammo(glm::vec3 startPosition, glm::vec3 direction,glm::vec4 color/*,void (*collidedEvent)(string)*/,float lifetime,Quadrotor*owner,AMMOTYPE::ENUM ammoType);
  //  Ammo(glm::vec3 &startPosition, string &quadname,glm::vec4 &color/*,void (*collidedEvent)(string)*/,float lifetime);
    
    static void fire(glm::vec3 startPosition, glm::vec3 direction,glm::vec4 color/*,void (*collidedEvent)(string)*/,float lifetime,Quadrotor* owner, AMMOTYPE::ENUM ammoType);
    static void fire(glm::vec3 startPosition, string quadname,glm::vec4 color/*,void (*collidedEvent)(string)*/,float lifetime,Quadrotor* owner, AMMOTYPE::ENUM ammoType);
    
    void draw();
    static void drawAll();
    
    //button click event callee
    Quadrotor* hasCollided();
    ~Ammo();

    
};
#endif