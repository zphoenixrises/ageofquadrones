
#include "MamaQuad.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <glm/gtc/type_ptr.hpp>

/*
 void MamaQuad::mamaBody(void) {
     
     //start of front panelopengl double buffer vbo
     
     glLineWidth(1);
   //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glColor3f(0.0,1.0,0.0);
     glScalef(0.05,0.05,0.05);
     glPolygonMode(GL_FRONT,GL_FILL);
     
     
     glBegin(GL_TRIANGLE_FAN);  // draw front body of Mama
     glScalef(0.1f,0.1f,0.1f);
     glVertex3f(0.0f,0.0f,0.0f);  // start center of screen
     
     glVertex3f(5.0f,-5.0f,0.0f);  // bottom right corner
     glVertex3f(5.0f,5.0f,0.0f);    //bottom left corner
     glVertex3f(-5.0f,5.0f,0.0f); //top left corner
     glVertex3f(-5.0f,-5.0f,0.0f);  // top right corner
     glVertex3f(5.0f,-5.0f,0.0f);
     glEnd();
     
     
     
     glBegin(GL_TRIANGLES);  // draw sides of Mama body
     glColor3f(1.0,1.0,0.0);
     //draw front side
     glVertex3f(5.0f,5.0f,0.0f);
     glVertex3f(-5.0f,5.0f,0.0f); 
     glVertex3f(-5.0f,5.0f,-1.0f);
     
     // glColor3f(1.0f,0.0f,0.0f);
     glVertex3f(5.0f,5.0f,0.0f);
     glVertex3f(-5.0f,5.0f,-1.0f); 
     glVertex3f(5.0f,5.0f,-1.0f);
     
     //draw right side top
     glVertex3f(5.0f,5.0f,-1.0f);
     glVertex3f(5.0f,5.0f,0.0f);
     glVertex3f(5.0f,-5.0f,0.0f);
     //right side bottom
     glVertex3f(5.0f,5.0f,-1.0f);
     glVertex3f(5.0f,-5.0f,0.0f);
     glVertex3f(5.0f,-5.0f,-1.0f);
     
     
     //draw bottom top side
     glVertex3f(5.0f,-5.0f,0.0f);
     glVertex3f(-5.0f,-5.0f,0.0f);
     glVertex3f(-5.0f,-5.0f,-1.0f);
     //draw bottom bottom side
     glVertex3f(5.0f,-5.0f,0.0f);
     glVertex3f(-5.0f,-5.0f,-1.0f);
     glVertex3f(5.0f,-5.0f,-1.0f);
     
     //draw left top side
     glVertex3f(-5.0f,5.0f,-1.0f);
     glVertex3f(-5.0f,5.0f,0.0f);
     glVertex3f(-5.0f,-5.0f,-1.0f);
     //draw left bottom side
     glVertex3f(-5.0f,-5.0f,-1.0f);
     glVertex3f(-5.0f,-5.0f,0.0f);
     glVertex3f(-5.0f,5.0f,0.0f);
     glEnd();
     
     //draw back of Mama
     
     glBegin(GL_TRIANGLE_FAN);  // draw back side body of quadrotor
     //      glScalef(0.1f,0.0f,0.0f);
     glColor3f(1.0f,0.0f,0.0f);
     glVertex3f(0.0f,0.0f,-1.0f);  // start center of screen
     glVertex3f(5.0f,-5.0f,-1.0f); //top right corner
     glVertex3f(-5.0f,-5.0f,-1.0f);  // top left corner
     glVertex3f(-5.0f,5.0f,-1.0f);    //bottom left corner
     glVertex3f(5.0f,5.0f,-1.0f); //bottom right corner
     glVertex3f(5.0f,-5.0f,-1.0f);  // top left corner
     glEnd();
     //  glTranslatef(-5.0f,5.0f,1.0f);
     glFlush();      
     //glutSwapBuffers();
     
     
     //      glPopMatrix();
     
 }
 void MamaQuad::drawQuad(void){
     
     //      glLoadIdentity();
     //      rotation();
     //glPushMatrix();
     
     glMultMatrixf(glm::value_ptr(Model)); //load Model glLoadMatrixd
     glScalef(30.0f,30.0f,30.0f);
     glPushMatrix();
     mamaBody();
     //*
     //move arm to lower right left corner
     glColor3f(0.0f,0.0f,1.0f);
     glTranslatef(0.0f,0.0f,-0.50f);
     glRotatef(90,1.0f,1.0f,0.0f);
     glTranslatef(0.0f,0.0f,6.5f);   
     gluCylinder(quad,0.5f , 0.5f ,10.0f,5.0f,5.0f);
     glPopMatrix();
     
     
     glPushMatrix();
     //lower right motor
     glColor3f(1.0,1.0,0.0);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(12.0f,-12.0f,-2.0f);
     motor();
     glPopMatrix();
     
     
     //lower right propeller
     glPushMatrix(); 
     glColor3f(1.0,0.0,0.0);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(12.0f,-12.0f,-2.0f);
     glTranslatef(0.0f,3.5f,0.0f);
     glRotatef(90,1.0f,0.0f,0.0f);
     lowRightPropeller();
     glPopMatrix();
     
     //move arm to lower right corner
     glPushMatrix(); 
     glTranslatef(0.0f,0.0f,-0.25f);
     glRotatef(90,1.0f,1.0f,0.0f);
     glTranslatef(0.0f,0.0f,6.5f);
     
     gluCylinder(quad,0.5f , 0.5f ,10.0f,5.0f,5.0f);
     glPopMatrix();
     //      gluCylinder(quad,0.5f , 0.5f ,10.0f,5.0f,5.0f);
     //      glPopMatrix();
     //lower right arm
     
     glPushMatrix(); 
     glColor3f(0.0f,0.0f,1.0f);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(0.0f,0.0f,-0.25f);
     glRotatef(90,1.0f,1.0f,0.0f);
     glTranslatef(0.0f,0.0f,6.5f);
     arm();
     glPopMatrix();
     
     glPushMatrix();
     //lower right motor
     glColor3f(1.0,1.0,0.0);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(12.0f,-12.0f,-2.0f);
     motor();
     glPopMatrix();
     
     //lower right propeller
     glPushMatrix(); 
     glColor3f(1.0,0.0,0.0);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(12.0f,-12.0f,-2.0f);
     glTranslatef(0.0f,3.5f,0.0f);
     glRotatef(90,1.0f,0.0f,0.0f);
     lowRightPropeller();
     glPopMatrix();
     
     glPushMatrix();
     //lower left arm        
     glColor3f(1.0f,0.0f,0.0f);
     glScalef(0.05,0.05,0.05);
     
     glTranslatef(0.0f,0.0f,-0.65);
     glRotatef(90,1.0f,-1.0f,0.0f);
     glTranslatef(0.0f,0.0f,6.5f);
     gluCylinder(quad,0.5f , 0.5f ,10.0f,5.0f,5.0f);
     glPopMatrix();
     
     //lower Left motor
     glPushMatrix();
     glColor3f(1.0,1.0,0.0);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(-12.0f,-12.0f,-2.0f);
     gluCylinder(quad,1.0f , 1.0f ,3.0f,5.0f,5.0f);
     glPopMatrix();
     
     //low Left propeller
     glPushMatrix();
     
     glColor3f(1.0,0.0,0.0);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(-12.0f,-12.0f,-2.0f);
     glRotatef(90,1.0f,0.0f,0.0f);
     lowLeftPropeller();
     glPopMatrix();
     
     
     //upper left arm
     glPushMatrix();
     glColor3f(1.0f,1.0f,0.0f);
     glScalef(0.05,0.05,0.05);
     glTranslatef(0.0f,0.0f,-0.65);
     glRotatef(90,-1.0,-1.0f,0.0f);
     glTranslatef(0.0f,0.0f,6.5);
     gluCylinder(quad,0.5f , 0.5f ,10.0f,5.0f,5.0f);
     glPopMatrix();
     
     // upper left motor
     glPushMatrix();
     glColor3f(1.0,1.0,0.0);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(-12.0f,12.0f,-2.0f);
     gluCylinder(quad,1.0f , 1.0f ,3.0f,5.0f,5.0f);
     glPopMatrix();
     
     //upper left propeller
     glPushMatrix();
     
     glColor3f(1.0,1.0,0.0);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(-12.0f,12.0f,-2.0f);
     upLeftPropeller();
     
     glPopMatrix();
     
     //upper right arm
     glPushMatrix();
     glColor3f(0.0f,1.0f,0.0f);
     glScalef(0.05,0.05,0.05);
     glTranslatef(0.0f,0.0f,-0.65);
     glRotatef(90,-1.0,1.0f,0.0f);
     glTranslatef(0.0f,0.0f,6.5);
     gluCylinder(quad,0.5f , 0.5f ,10.0f,5.0f,5.0f);
     glPopMatrix();
     
     //upper right motor
     glPushMatrix();
     glColor3f(1.0,1.0,0.0);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(12.0f,12.0f,-2.0f);
     gluCylinder(quad,1.0f , 1.0f ,3.0f,5.0f,5.0f);
     glPopMatrix();
     
     //upper right propeller
     glPushMatrix();
     
     glColor3f(1.0,1.0,0.0);
     glScalef(0.05f,0.05f,0.05f);
     glTranslatef(12.0f,12.0f,-2.0f);
     upRightPropeller();
     glPopMatrix();
     //*
     glPopMatrix();
     
     //glFlush();      
     //glutSwapBuffers();
 }
 
 void MamaQuad::motor() {
     
     gluCylinder(quad,1.0f , 1.0f ,3.0f,5.0f,5.0f);
 }
 
 void MamaQuad::upRightPropeller() {
     
     glPushMatrix();
     //draw sphere
     glColor3f(0.0f,1.0f,0.0f);
     // glTranslatef(0.0,0.0,3.5);
     gluSphere(quad,1.0,5,5);       
     glPopMatrix();
     //draw propeller
     
     glPushMatrix();
     glColor3f(1.0,0.0,0.0);
     glTranslatef(0.0f,0.0f,0.3f);
     glRotatef(90,0.0f,1.0f,0.0f);
     glRotatef(rotPropellerLeft,1.0,0.0,0.0);
     gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);                  
     glTranslatef(0.0f,0.0f,-6.5f);          
     gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);
     glPopMatrix();
 }
 void MamaQuad::upLeftPropeller() {
     
     glPushMatrix();
     //draw sphere
     glColor3f(0.0f,1.0f,0.0f);
     // glTranslatef(0.0,0.0,3.5);
     gluSphere(quad,1.0,5,5);       
     glPopMatrix();
     //draw propeller
     glPushMatrix();               
     glColor3f(1.0,0.0,0.0);
     glTranslatef(0.0f,0.0f,0.3f);
     glRotatef(90,0.0f,1.0f,0.0f);
     glRotatef(rotPropellerLeft,1.0,0.0,0.0);
     gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);                  
     glTranslatef(0.0f,0.0f,-6.5f);          
     gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);
     //      rotPropellerLeft+=3.5;
     glPopMatrix();
 }
 void MamaQuad::lowLeftPropeller() {
     
     glPushMatrix();
     //draw sphere
     glColor3f(0.0f,1.0f,0.0f);
     // glTranslatef(0.0,0.0,3.5);
     gluSphere(quad,1.0,5,5);       
     glPopMatrix();
     //draw propeller
     glPushMatrix();
     glColor3f(1.0,0.0,0.0);
     glTranslatef(0.0f,0.0f,0.3f);
     glRotatef(rotPropellerLeft,0.0,1.0,0.0);
     gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);                  
     glTranslatef(0.0f,0.0f,-6.5f);          
     gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);
     rotPropellerLeft+=3.5;
     glPopMatrix();
     
 }
 void MamaQuad::lowRightPropeller() {
     
     glPushMatrix();
     //draw sphere
     glColor3f(0.0,1.0,0.0);
     glTranslatef(0.0,0.0,3.5);
     gluSphere(quad,1.0,5,5);        
     glPopMatrix();
     //draw propeller
     glPushMatrix();
     glColor3f(1.0,0.0,0.0);
     glTranslatef(0.0f,0.0f,3.5f);
     glRotatef(rotPropeller,0.0,1.0,0.0);
     gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);                  
     glTranslatef(0.0f,0.0f,-6.5f);          
     gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);
     rotPropeller+= 3.5f;
     glPopMatrix();
     /*
      *       glPushMatrix();
      *               glTranslatef(0.0f,0.0f,-3.5f);
      *       //      glTranslatef(0.0f,0.0f,3.5f);
      *       
      *               glRotatef(rotPropellerOp,0.0,1.0,0.0);
      *               gluCylinder(quad,0.1f , 0.1f ,6.0f,5.0f,5.0f);
      *       //      rotPropellerOp+= 1.0f;
      *       glPopMatrix();
      *
     
 }
 void MamaQuad::arm() {
     
     glLineWidth(1);
     
     /*              glColor3f(0.0f,0.0f,1.0f);
                    glScalef(0.05f,0.05f,0.05f);
      glTranslatef(0.0f,0.0f,-0.50f);
      glRotatef(90,1.0f,1.0f,0.0f);
      glTranslatef(0.0f,0.0f,6.5f);
      //*
     gluCylinder(quad,0.5f , 0.5f ,10.0f,5.0f,5.0f);
     glFlush();
     
 }
 
 //*/
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
 
 
 
 