/******************************************************************
 * Name: main.cpp
 * Purpose: main routines are defined here
 * 
 * Author: Akkas Uddin Haque
 * Email : akkasuddin@live.com
 *
 * Creation Date: 09/19/2015
 *******************************************************************/
 
#include<iostream>
#include<cmath>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<QuadTimer.h>
//#include<glm/glm/glm.hpp>

using namespace std;

void animatefunc(int value);

void rotateDisplayfunc(int value);

#include "NeoQuad.h"


void initializeRendering()
{
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
    glEnable(GL_LIGHT1); //Enable light #1
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glEnable(GL_CULL_FACE);

}



NeoQuad *neoQuad;

void output(GLfloat x, GLfloat y, char* text)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(1/15.38, 1/15.38, 1/15.38);
    for( char* p = text; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
}

void drawHandler()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(-0, -0, -50);
    //Add ambient light
    GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    //Add positioned light
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    
    //Add directed light
    GLfloat lightColor1[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.2, 0.2)
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    neoQuad->drawQuad();
    //glLoadIdentity();
   // glColor3ub(255,0,0);
   // output(20,20,neoQuad->difftime);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glutSwapBuffers();
    
}


void resizeHandler(int w, int h)//Called when the window is resized
{
    //Tell OpenGL how to convert from coordinates to pixel values
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
    
    //Set the camera perspective
    glLoadIdentity(); //Reset the camera
    gluPerspective(80,                  //The camera angle
                   (double)w / (double)h, //The width-to-height ratio
                   1.0,                   //The near z clipping coordinate
                   200.0);                //The far z clipping coordinate
}


void keypressHandler(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'd':
            neoQuad->yawQuad(2);
            break;
        case 'a':
            neoQuad->yawQuad(-2);
            break;
        case 's':
            neoQuad->rollQuad(2);
            break;
        case 'w':
            neoQuad->rollQuad(-2);
            break;
        case 'f':
            neoQuad->pitchQuad(2);
            break;
        case 'g':
            neoQuad->pitchQuad(-2);
            break;
        case 'k':
            neoQuad->changePropSpeed(-0.1);
            break;
        case 'l':
            neoQuad->changePropSpeed(0.1);
            break;
        case 'p':
            neoQuad->powerToggle();
            break;
        case 'm':
            neoQuad->toggleAnimate();
            break;
        case 13:
            exit(0);
            
    };
    //glutPostRedisplay();
}

//void mouseHandler()


void animatefunc(int value)
{
    glutTimerFunc(20, animatefunc, 0);
    glutPostRedisplay();
    //neoQuad->rotateProps(0);
}

void rotateDisplayfunc(int value)
{
   
    //      neoQuad->rotateDisplay(0);
    
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv); //initialize glut 
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); //initialize display mode
    glutInitWindowSize(500, 500); //window size
    glutCreateWindow("Akkas's 3D QuadCopter Model"); //create window
    
    initializeRendering();
    
    glutDisplayFunc(drawHandler);
    glutKeyboardFunc(keypressHandler);
    glutReshapeFunc(resizeHandler);
    neoQuad = new NeoQuad();
    //glutMouseFunc()
    
    glutTimerFunc(10, animatefunc, 0);
    //glutTimerFunc(rotationSpeed,rotateDisplay,0);

    glutMainLoop();
    
    return 0;
    
    
}

