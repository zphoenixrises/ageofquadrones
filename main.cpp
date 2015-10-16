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
#include<GL/glut.h>
#include<QuadTimer.h>
#include "camera.h"

#include "NeoQuad.h"
#include "Dronedemort.h"

using namespace std;
using namespace glm;

//Create the Camera
Camera camera;

class Window {
public:
    Window() {
        this->interval = 1000 / 60;             //60 FPS
        this->window_handle = -1;
    }
    int window_handle, interval;
    ivec2 size;
    float window_aspect;
} window;

//Invalidate the window handle when window is closed
void CloseFunc() {
    window.window_handle = -1;
}
//Resize the window and properly update the camera viewport
void ReshapeFunc(int w, int h) {
    if (h > 0) {
        window.size = ivec2(w, h);
        window.window_aspect = float(w) / float(h);
    }
    camera.SetViewport(0, 0, window.size.x, window.size.y);
}

//Keyboard input for camera, also handles exit case
void KeyboardFunc(unsigned char c, int x, int y) {
    switch (c) {
        case 'i':
            camera.Move(FORWARD);
            break;
        case 'j':
            camera.Move(LEFT);
            break;
        case 'k':
            camera.Move(BACK);
            break;
        case 'l':
            camera.Move(RIGHT);
            break;
        case 'u':
            camera.Move(DOWN);
            break;
        case 'o':
            camera.Move(UP);
            break;
        case 'x':
        case 27:
            exit(0);
            return;
        default:
            break;
    }
}

void SpecialFunc(int c, int x, int y) {}
void CallBackPassiveFunc(int x, int y) {}
//Used when person clicks mouse
void CallBackMouseFunc(int button, int state, int x, int y) {
    camera.SetPos(button, state, x, y);
}
//Used when person drags mouse around
void CallBackMotionFunc(int x, int y) {
    camera.Move2D(x, y);
}
//Draw a wire cube! (nothing fancy here)
void DisplayFunc() {
    //glEnable(GL_CULL_FACE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, window.size.x, window.size.y);
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
    glm::mat4 model, view, projection;
    camera.Update();
    camera.GetMatricies(projection, view, model);
    
    glm::mat4 mvp = projection* view * model;       //Compute the mvp matrix
    glLoadMatrixf(glm::value_ptr(mvp));
    glColor3f(1.0f,1.0f,1.0f);
    glutSolidTeapot(10);
    glutSwapBuffers();
}
//Redraw based on fps set for window
void TimerFunc(int value) {
    if (window.window_handle != -1) {
        glutTimerFunc(window.interval, TimerFunc, value);
        glutPostRedisplay();
    }
}


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
Dronedemort *dronedemort;
Quadrotor *quadrotor;

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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   // glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, window.size.x, window.size.y);
   
    
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
    //glLoadIdentity();
    glm::mat4 model, view, projection;
    camera.Update();
    camera.GetMatricies(projection, view, model);
    
    glm::mat4 mvp = projection* view * model;       //Compute the mvp matrix
    glLoadMatrixf(glm::value_ptr(mvp));
    neoQuad->moveAbs(10,0,0);
    neoQuad->draw();
    
    
    
    //glMatrixMode(GL_MODELVIEW);
    dronedemort->moveAbs(-50, 0 ,0);
    dronedemort->draw();
    
    


    //glLoadIdentity();
   // glColor3ub(255,0,0);
   // output(20,20,neoQuad->difftime);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glutSwapBuffers();
    
}


void keypressHandler(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'd':
            quadrotor->yawQuad(2);
            break;
        case 'a':
            quadrotor->yawQuad(-2);
            break;
        case 's':
            quadrotor->rollQuad(2);
            break;
        case 'w':
            quadrotor->rollQuad(-2);
            break;
        case 'f':
            quadrotor->pitchQuad(2);
            break;
        case 'g':
            quadrotor->pitchQuad(-2);
            break;
        case ';':
            neoQuad->changePropSpeed(-0.1);
            break;
        case '\'':
            neoQuad->changePropSpeed(0.1);
            break;
        case 'p':
            neoQuad->powerToggle();
            break;
        case 'm':
            neoQuad->toggleAnimate();
            break;
        case ' ':
            if(quadrotor == neoQuad)
                quadrotor = dronedemort;
            else
                quadrotor = neoQuad;
            break;
        case 13:
            exit(0);
            
    };
    KeyboardFunc(key,x,y);
    //glutPostRedisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv); //initialize glut 
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //initialize display mode
    
    glutInitWindowSize(1024, 512);
    glutInitWindowPosition(0, 0);
    window.window_handle = glutCreateWindow("Akkas's 3D QuadCopter Model"); //create window
    

    //*
    glutReshapeFunc(ReshapeFunc);
    glutDisplayFunc(drawHandler);
    glutKeyboardFunc(keypressHandler);
    glutSpecialFunc(SpecialFunc);
    glutMouseFunc(CallBackMouseFunc);
    glutMotionFunc(CallBackMotionFunc);
    glutPassiveMotionFunc(CallBackPassiveFunc);
    glutTimerFunc(window.interval, TimerFunc, 0);
    
    neoQuad = new NeoQuad();
    dronedemort = new Dronedemort();
    quadrotor = dronedemort;
    initializeRendering();//Setup camera
    camera.SetMode(FREE);
    camera.SetPosition(glm::vec3(0, 0, 100));
    camera.SetLookAt(glm::vec3(0, 0, 0));
    camera.SetClipping(.1, 1000);
    camera.SetFOV(45);
    //Start the glut loop!
    //*/
    glutMainLoop();
    
    return 0;
    
    
}

