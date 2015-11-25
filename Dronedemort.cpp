  /**************************************************/
  /* Name: Ian Nordeng                              */
  /*                                                */
  /* Creates and Draws Dronedemort                  */
  /*                                                */
  /* CSci 446 / fall 2013                           */
  /**************************************************/
#include "Settings.h"
#include "Dronedemort.h"
#include <GL/glut.h>
#include <cmath>
#include <stdio.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <glm/gtc/type_ptr.hpp>
//#include "pgmIO.h"
using namespace std;

#ifndef Pi

#define Pi 3.1415926535897932384626433832795
#endif

/***********************************************************/
/* INCLUDE RAYGL HEADERS, IF DESIRED.                      */
/***********************************************************/
//#define RAYGL 0
//#if RAYGL == 1
 #include "raygl/raygl.h"
 #include "raygl/raygldefs.h"
//#else
// #include "./pgmIO.h"
//#endif
#define noTextures 3
GLuint texture[noTextures];

#define ENABLETEXTURES

Dronedemort::Dronedemort()
	{
	    gunPitch = 60.0;
	    gunAngle=0.0;
	    propAngle = 0.0;
	    propSpeed = 2;
	    animate = true;
            
            timeline = new Timeline("DRO");
           LoadGLTextures();
	}
	

GLvoid Dronedemort::LoadGLTextures()
{
    int w, h, c;
    #if RAYGL == 1
     Image *image[noTextures];
     for (c = 0; c < noTextures; c++) {
         image[c] = (Image *) malloc(sizeof(Image));
         if (image[c] == NULL) exit(0);
     }
    #endif
    
     #if RAYGL == 1
     if (!imageLoad("Data/perfblack.ppm", image[0])) exit(0);
    #else
     data = PGM_FILE_READ("Data/perfblack.ppm", &w, &h, &c); 
    #endif 
    // Load texture map for sphere.

    glGenTextures(1, &body);
    glBindTexture(GL_TEXTURE_2D, body);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

//     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); 
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );   
    
     #if RAYGL == 1
//      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[0]->sizeX, image[0]->sizeY, 
//                   0, GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
     gluBuild2DMipmaps(GL_TEXTURE_2D,3, image[0]->sizeX, image[0]->sizeY,GL_RGB,GL_UNSIGNED_BYTE,image[0]->data);
    #else
//      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
     
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data );
     free(data);
    #endif
    

       #if RAYGL == 1
     if (!imageLoad("Data/eye.ppm", image[1])) exit(0);
    #else
      data = PGM_FILE_READ("Data/eye.ppm", &w, &h, &c);  
    #endif 
    
   

    glGenTextures(1, &eye);
    glBindTexture(GL_TEXTURE_2D, eye);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

//     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );   
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
      #if RAYGL == 1
//       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[1]->sizeX, image[1]->sizeY, 
//                    0, GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);
    
     gluBuild2DMipmaps(GL_TEXTURE_2D,3, image[1]->sizeX, image[1]->sizeY,GL_RGB,GL_UNSIGNED_BYTE,image[1]->data);
    #else
//       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data );
     free(data);
    #endif
  

    
    
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
        
	glColor3f(0.12f,0.12f, 0.12f);
        
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
        
	glColor3f(0.12f, 0.12f, 0.12f);
        
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
//        glRotatef(270,0.0f,1.0f,0.0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	glPushMatrix(); //Main push

// 	glDisable(GL_LIGHTING);
//         //*
// 	output(-49.0,-48.5,"Prop-");
// 	drawButton(-50,-50,-40,-45);
// 	output(-34.0,-48.5,"Prop+");
// 	drawButton(-35,-50,-25,-45);
// 	output(33.0,-48.5,"Gun<");
// 	drawButton(32,-50,42,-45);
// 	output(49.0,-48.5,"Gun>");
// 	//drawButton(40,-50,50,-45);
// 	drawButton(48,-50,58,-45);
// 	output(41.0,-38.5,"Gun^");
// 	drawButton(40,-40,50,-35);
// 	output(41.0,-58.5,"Gun");
// 	glRotatef(180,1.0,0.0,0.0);
// 	
// 	glColor3f(1.0,1.0,1.0);
// 	output(46.5,55.7,"^");
// 	glRotatef(180,1.0,0.0,0.0);
// 	drawButton(40,-60,50,-55);
// 	glEnable(GL_LIGHTING);
// 	
	glMultMatrixf(glm::value_ptr(Model)); //load Model glLoadMatrixd
	//drawAxes();
// 	gluQuadricDrawStyle(quadricObj,GLU_FILL);
// 	
// 	gluQuadricNormals(quadricObj, GLU_SMOOTH);
        
	glRotatef(90.0f,0.0f,1.0f,0.0f);
        glPushMatrix();//Push for main body and eye
        
	glColor3f(1.0f, 1.0f, 1.0f);
	
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//Draw main body
	
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, body);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
        #if RAYGL == 1
        rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
        rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
        rayglTextureType(1);                       // Set texture type for PovRAY.
        #endif
        
        
//         gluQuadricDrawStyle(quadricObj, GLU_FILL);
//         gluQuadricTexture(quadricObj, GL_TRUE);
//         gluQuadricNormals(quadricObj, GLU_SMOOTH);
	gluSphere(quadricObj, 25.0f, 20.0f, 20.0f);
	
       glDisable(GL_TEXTURE_2D);
        
// 	//Draw eye
        
 	glColor3f(0.75f, 0.0f, 0.0f);
        
 	glTranslatef(0.0f,-6.4f,23.5f);
	glRotatef(270,1.0f,0.0f,0.0);
        
//         glEnable(GL_TEXTURE_2D);
//         glBindTexture(GL_TEXTURE_2D, eye);
//         glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
//         #if RAYGL == 1
//         rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
//         rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
//         rayglTextureType(1);                       // Set texture type for PovRAY.
//         #endif
        
 	drawEllipsoid(20.0f,20.0f,10.0f,3.0f,3.0f);
// 	gluQuadricNormals(quadricObj, GLU_SMOOTH);

// 	glDisable(GL_TEXTURE_2D);

 	glPopMatrix();//Pop for main body and eye
 	
 	//Draw laser gun
 
	glColor3f(0.2f, 0.2f, 0.2f);
        
 	glPushMatrix();//Push for gun
 	glRotatef(180.0f,0.0f,0.0f,1.0f);
 	glTranslatef(0.0f,24.5f,0.0f);
 	glRotatef(90.0f,-1.0f,0.0f,0.0f);
        
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, body);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        #if RAYGL == 1
        rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
        rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
        rayglTextureType(1);                       // Set texture type for PovRAY.
        #endif
        
 	gluCylinder(quadricObj,2.0f,2.0f,10.0f,10.0f,10.0f);
 	glTranslatef(0.0f,0.5f,8.0f);
 	
 	//rotateGun(angleInc);
	glRotatef(gunAngle,0.0f,0.0f,1.0f);
	gluSphere(quadricObj, 5.0f, 10.0f, 10.0f);
	

	//
	
	glRotatef(gunPitch,1.0f,0.0f,0.0f);
	gluCylinder(quadricObj,2.0f,2.0f,15.0f,15.0f,10.0f);
	glTranslatef(0.0f,0.0f,15.0f);
        
	glDisable(GL_TEXTURE_2D);
        
	glColor3f(1.0f, 0.0f, 0.0f);
        
	gluDisk(quadricObj,0,1.5,15,5);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, body);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        #if RAYGL == 1
        rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
        rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
        rayglTextureType(1);                       // Set texture type for PovRAY.
        #endif
        
	glColor3f(0.2f, 0.2f, 0.2f);
        
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
        
       glDisable(GL_TEXTURE_2D);

	glPopMatrix();//pop arm 4
	
	glPopMatrix(); //Main pop
}
	
void Dronedemort::rotateProps()
{
	if (animate)
	{
		propAngle += propSpeed*time.getTimeDiffSec()*1000;
	}
	time.getTimeDiffSec();
 
}

void Dronedemort::rotateGun(float angleinc)
{
  gunAngle+=angleinc;
}

void Dronedemort::pitchGun(float pitchinc)
{
  
  gunPitch += pitchinc;
  
  if(gunPitch>120) gunPitch = 120;
  if(gunPitch<-120) gunPitch = -120;
  
  
}


void Dronedemort::toggleAnimate()
{
	animate = !animate;
}

void Dronedemort::changePropSpeed(float increment)
{
  propSpeed+=increment;
}

// void Dronedemort::drawButton(float x2,float y2,float x1, float y1)
// {
//   glBegin(GL_QUADS);
//   glColor4f(0.0f, 0.0f, 0.0f, 0.25f);
//   glVertex3f(x1,y1,0.0);
//   glVertex3f(x2,y1,0.0);
//   glVertex3f(x2,y2,0.0);
//   glVertex3f(x1,y2,0.0);
//   glEnd();
//   }

  void Dronedemort::output(GLfloat x, GLfloat y, char const* text)
{
    glPushMatrix();
    
    glColor3f(1.0,1.0,1.0);
    
    glTranslatef(x, y, 0);
    glScalef(1/46.0, 1/46.0, 1/46.0);
    for( char const* p = text; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
}
  
