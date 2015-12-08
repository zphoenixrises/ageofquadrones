#include "Ammo.h"
   
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

vector<Ammo*> Ammo::bullets;

namespace AMMOTYPE{
    
ENUM getAmmotypeFromString(string ammoType)
{
    if(ammoType == "CANNONBALL")
        return CANNONBALL;
    if(ammoType == "LASER")
        return LASER;
    if(ammoType == "BLASTER")
        return BLASTER;

}
}

Ammo::Ammo(glm::vec3 startPosition, glm::vec3 direction, glm::vec4 color/*,void (*collidedEvent)(string)*/,float lifetime,Quadrotor*owner,AMMOTYPE::ENUM ammoType)
{
    position = startPosition;
    this->owner = owner;
    //quadCollisionEvent = collidedEvent; 
    this->color = color;
    this->startTime = time.GetProcessTime();
    this->lifetime = lifetime+startTime;
    quadricObj = gluNewQuadric();
    gluQuadricNormals(quadricObj, GLU_SMOOTH);   // Create Smooth Normals ( NEW )
    gluQuadricTexture(quadricObj, GL_TRUE);      // Create Texture Coords ( NEW )
    
    
    direction = glm::normalize(direction);
    glm::vec3 current_axis(0,0,1);
    glm::vec3 comRotationAxis = glm::cross(current_axis,direction);
    float axisAngle = glm::angle(current_axis,direction);
    orientationMatrix = glm::mat4(1.0f);
    orientationMatrix = glm::rotate(orientationMatrix,axisAngle,comRotationAxis);
    this->ammoType = ammoType;
    //  
    this->direction = direction;
    
    bullets.push_back(this);
}


void Ammo::drawAll()
{
    int i=0,del=-1;
    for(vector<Ammo*>::iterator it = bullets.begin();it!=bullets.end();++it,++i)
    {
        if(NULL!=*it)
        {
            Quadrotor* collidedQuad = (*it)->hasCollided();
            if(NULL!=collidedQuad && (*it)->owner!=collidedQuad)
            {
                collidedQuad->collided();
                delete *it;
                *it=NULL;
                continue;
            }
            (*it)->draw();
            if((*it)->time.GetProcessTime()> (*it)->lifetime)
            {   
                del = i;
              //  bullets.erase(bullets.begin()+i);
                //delete this;
            }
        }
    }
    if(del!=-1)
        bullets.erase(bullets.begin()+del);
    

}

void Ammo::fire(glm::vec3 startPosition, glm::vec3 direction,glm::vec4 color/*,void (*collidedEvent)(string)*/,float lifetime,Quadrotor* owner,AMMOTYPE::ENUM ammoType)
{
    Ammo *ammo = new Ammo(startPosition, direction,color,lifetime,owner,ammoType);
    
}

void Ammo::fire(glm::vec3 startPosition, string quadname, glm::vec4 color, float lifetime,Quadrotor* owner,AMMOTYPE::ENUM ammoType)
{
    glm::vec3 direction = Quadrotor::getQuadFromName(quadname)->getQuadPosition()-startPosition;
    
    Ammo *ammo = new Ammo(startPosition, direction,color,lifetime,owner,ammoType);
    

}

void Ammo::draw()
{
    
    glPushMatrix();
    glTranslatef(position.x,position.y,position.z);
    glMultMatrixf(glm::value_ptr(orientationMatrix));
    glColor4f(color.x,color.y,color.z,color.w);
    
    if(ammoType == AMMOTYPE::CANNONBALL)
    {
        gluSphere(quadricObj, 2.0f, 5, 5); //draw head
    }   
    else if(ammoType == AMMOTYPE::LASER)
    {
        gluCylinder(quadricObj,1.0,1.0,30.0,5,5);
        glRotatef(90,1,0,0);
        gluDisk(quadricObj,0.0f,1.0f,5,3);
        glRotatef(90,1,0,0);
        glTranslatef(0,0,30);
        
        gluDisk(quadricObj,0.0f,1.0f,5,3);
    }
    else if(ammoType == AMMOTYPE::BLASTER)
    {
        glTranslatef(0,0,10);
        glScalef(1,1,10);
        gluSphere(quadricObj, 1.0f, 5, 5); //draw head
        
    }
    glPopMatrix();
    
    position += direction*(float)(500*time.getTimeDiffSec());


}

Quadrotor* Ammo::hasCollided()
{
    for(vector<Quadrotor*>::iterator it = Quadrotor::quads.begin(); it!=Quadrotor::quads.end(); ++it)
    {
        if(glm::length((*it)->getQuadPosition()-position)< COLLISIONRADIUS)
            return *it;
    }
    return NULL;

}


Ammo::~Ammo() 
{
    gluDeleteQuadric( quadricObj);

}
