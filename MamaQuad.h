/******************************************************************
 * Name: MamaQuad.h
 * Purpose: MamaQuad class is defined here
 * 
 * Author: Steve Beuttner
 * Email : 
 *
 * Creation Date: 
 *******************************************************************/


#ifndef MamaQuad_H
#define MamaQuad_H
#include "QuadTimer.h"
#include "Quadrotor.h"
#include <string>

using namespace std;

class MamaQuad:public Quadrotor
{
private:
    
    QuadTimer time;
    bool animate;
  
    
    int rotPropeller;
    GLUquadric *quad;
    void mamaBody(void);
    void motor();
    void arm();
    void propeller();
    void newBody();
    
public:
    MamaQuad();
    string getName();
    void toggleAnimate();
    //TODO: Steve-> Please solve issues with the drawing of mamaquad
    void drawQuad();
    //TODO: Steve-> Please implement this function to change your propeller speed
    void changePropSpeed(float increment);
    
    void collided();
    glm::vec3 getBarrelPosition();
    void customCommandParser(string commandString);
    
    ~MamaQuad();
 
};

#endif
