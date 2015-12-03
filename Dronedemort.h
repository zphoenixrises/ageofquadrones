  /**************************************************/
  /* Name: Ian Nordeng                              */
  /*                                                */
  /* Header file for Dronedemort                    */
  /*                                                */
  /* CSci 446 / fall 2013                           */
  /**************************************************/


#ifndef Drondemort_H
#define Drondemort_H
#include "QuadTimer.h"
#include "Quadrotor.h"
#include <string>

using namespace std;

class Dronedemort:public Quadrotor
{
  

private:
 
	QuadTimer time;
	float propAngle;  
	float propSpeed;
	float ga;
	float gunAngle;
	float gunPitch;
	bool animate;
	void drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC);
	void drawBlade();
	void drawProparms(int rotorDirection);
	void rotateProps();
	void animateGun(unsigned char key, int x, int y);
	void drawButton(float x1, float y1, float x2, float y2);
        GLuint body;
	GLuint eye;
        unsigned char * data;
	unsigned char * data1;
        glm::mat4  barrelOrientationMatrix;
        int pointBarrelFlag;
        string whichQuad;
public:
	Dronedemort(); 
        string getName();
	void toggleAnimate();
	GLvoid LoadGLTextures();
	void drawQuad();
	void changePropSpeed(float increment);
	void rotateGun(float ga);
	void pitchGun(float gunPitch);
	void output(GLfloat x, GLfloat y, char const* text);
        
        void collided();
        glm::vec3 getBarrelPosition();
        void pointBarrel();
        void customCommandParser(string commandString);
        

};

#endif
