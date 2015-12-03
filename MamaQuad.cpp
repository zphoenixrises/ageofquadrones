#include "Settings.h"
#include "MamaQuad.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <glm/gtc/type_ptr.hpp>
#include "raygl/raygl.h"
#include "raygl/raygldefs.h"

 MamaQuad::MamaQuad() {
        
     /* rotUp=20;
      * rotDown=0;
      * rotLeft=0;
      * rotRight=0;
      * rotIn=0;
      * rotOut=0;
      */
     //speed=1.0;
     //red = true;
    // grey = false;
     rotPropeller=0;
     rotPropellerLeft=0;
     quad = gluNewQuadric();
     gluQuadricDrawStyle(quad,GLU_FILL);
     gluQuadricNormals(quad,GLU_SMOOTH);
     
     timeline = new Timeline("MAM");
 }
 
string MamaQuad::getName()
{
    return "MAM";

}

 
 MamaQuad::~MamaQuad() {
     gluDeleteQuadric(quad);
 }
 /****************************************************/
 // written by Steve Buettner
 void MamaQuad::propeller() {
     
     glPushMatrix();
     glColor3f(1.0f,0.0f,0.0f);
     glRotatef(90.0f,0.0f,1.0f,0.0f);
     glRotatef(rotPropeller,1.0,0.0,0.0);          
     gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);                  
     glTranslatef(0.0f,0.0f,-6.5f);  
     gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);
     glPopMatrix();
     
 }
 //written by Steve Buettner
 void MamaQuad::motor() {
     //motor
     gluCylinder(quad,1.0f , 1.0f ,3.0f,5.0f,5.0f);
     //draw sphere for propeller blades
     gluSphere(quad,1.0,5,5);
 }
 
 //written by Steve Buettner
 void MamaQuad::newBody() {
     //      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glScalef(0.5f,0.5f,0.5f);
     //body
     gluSphere(quad,5.0f,25,25);
     
     //head
     glTranslatef(-6.9,0.0f,0.0f);
     GLUquadricObj *sphere = gluNewQuadric();
     gluQuadricDrawStyle(sphere, GLU_FILL);
     gluQuadricTexture(sphere, GL_TRUE);
     gluQuadricNormals(sphere,GL_SMOOTH);
     gluSphere(sphere,2.0f,25,25);
     gluDeleteQuadric(sphere);
     
     
     //pickup for chip of destiny
     glTranslatef(8.5f,0.0f,4.5f);
     gluCylinder(quad,1.0f,1.0f,5.0f,5.0f,5.0f);   
     
 }
 
 
 //written by Steve Buettner
 void MamaQuad::arm()  {
     
     glScalef(0.5f,0.5f,0.5f);
     //drawing arm
     gluCylinder(quad,0.5f , 0.5f ,10.0f,5.0f,5.0f);
 }
 
 //written by Steve Buettner 
 void MamaQuad::drawQuad(void){
     
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glMultMatrixf(glm::value_ptr(Model)); //load Model glLoadMatrixd
     
     glScalef(4.0f,4.0f,4.0f);
   //  glPushMatrix();
     
     glRotated(180,0,1,0);
     glRotated(90,1,0,0);
     
     
     glPushMatrix();
     glTranslatef(0.0f,0.0f,-2.0f);
     //     mySmoke();
     glPopMatrix();                  
     glPushMatrix();
     glColor3f(0.5f,0.5f,0.0f);
     newBody();
     glPopMatrix();
     
     glPushMatrix();
     //lower right arm               
     glColor3f(0.0f,1.0f,0.0f);
     glTranslatef(0.25f,-0.25f,-0.01f);
     glRotatef(90.0f,1.0f,1.0f,0.0f);
     arm();
     glTranslatef(0.75f,-0.75f,9.5f);
     glRotatef(-90.0f,1.0f,1.0f,0.0f);
     motor();
     propeller();
     glPopMatrix();
     
     glPushMatrix();
     //lower left arm
     
     glColor3f(0.0f,1.0f,0.0f);
     glTranslatef(-0.25f,-0.25f,-0.01f);     
     glRotatef(90.0f,1.0f,-1.0f,0.0f);
     arm();
     
     //set up motor for lower left arm               
     glTranslatef(-1.25f,-1.25f,9.5f);
     glRotatef(-90.0f,1.0f,-1.0f,0.0f);
     motor();
     propeller();
     glPopMatrix();
     
     glPushMatrix();
     //upper left arm        
     
     glColor3f(0.0f,1.0f,0.0f);
     glTranslatef(-0.25f,0.25f,-0.01f);      
     glRotatef(90.0f,-1.0f,-1.0f,0.0f);
     arm();
     //upper left motor
     glTranslatef(-0.75f,0.75f,9.5f);
     glRotatef(90.0f,1.0f,1.0f,0.0f);
     motor();
     propeller();
     glPopMatrix();
     
     glPushMatrix();
     //upper right arm
     
     glColor3f(0.0f,1.0f,0.0f);
     glTranslatef(0.25f,0.25f,-0.01f);       
     glRotatef(90.0f,-1.0f,1.0f,0.0f);
     arm();
     //set up upper right arm        motor
     glTranslatef(0.75f,0.75f,9.5f);                 
     glRotatef(90.0f,1.0f,-1.0f,0.0f);
     motor();
     propeller();
     
     glPopMatrix(); 
     
     rotPropeller+=15.0; 
  //   glPopMatrix();
     
    
     
 }
 
  
void MamaQuad::collided()
{

} 

glm::vec3 MamaQuad::getBarrelPosition()
{

}

void MamaQuad::customCommandParser(string commandString)
{

}
