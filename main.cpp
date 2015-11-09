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
#include "MamaQuad.h"
#include "Background.h"

using namespace std;
using namespace glm;

//Create the Camera
Camera camera;

NeoQuad *neoQuad;
Dronedemort *dronedemort;
MamaQuad *mamaQuad;
Quadrotor *quadrotor;
Background *background;

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
int z;
//Keyboard input for camera, also handles exit case
void CameraKeyboardFunc(unsigned char c, int x, int y) {
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
            /*
            z+=20;
            camera.SetPosition(glm::vec3(0, 8000, z+2));
            camera.SetLookAt(glm::vec3(0, 60, z));//*/
            camera.Move(UP);
            break;
        case 'c':    
            camera.SetCameraModeFollow(*neoQuad,-glm::vec3 (-100.0f,10.0f,0));
            break;
        case 'v':    
            camera.SetCameraModeFollow(*mamaQuad,-glm::vec3 (-100.0f,10.0f,0));
            break;
        case 'b':    
            camera.SetCameraModeFollow(*dronedemort,-glm::vec3 (-100.0f,10.0f,0));
            break;
        case 'n':
            camera.SetCameraModeWorld();
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

//Redraw based on fps set for window
void TimerFunc(int value) {
    if (window.window_handle != -1) {
        glutTimerFunc(window.interval, TimerFunc, value);
        glutPostRedisplay();
    }
}



void initializeRendering()
{
    background-> LoadGLTextures();                           // load the textures.
    glEnable(GL_TEXTURE_2D);                    // Enable texture mapping.
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);          // Set the blending function for translucency (note off at init time)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       // This Will Clear The Background Color To Black
    glClearDepth(1.0);                          // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);                       // type of depth test to do.
    // glEnable(GL_DEPTH_TEST);                    // enables depth testing.
    glShadeModel(GL_SMOOTH);                    // Enables Smooth Color Shading
    
    glEnable(GL_LIGHTING); //Enable lighting
    //   glMatrixMode(GL_PROJECTION);
    //  glLoadIdentity();                           // Reset The Projection Matrix
    
    //   gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);   // Calculate The Aspect Ratio Of The Window
    
    glMatrixMode(GL_MODELVIEW);
    
    /*****************************************For Background***********************/ 
    GLfloat LightAmbient[]  = {0.5f, 0.5f, 0.5f, 1.0f}; 
    GLfloat LightDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
    GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};
    
    //For smoke
    
    const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 0.0f, 0.0f, 3.0f, 1.0f };
    //const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    //const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    //const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    //const GLfloat high_shininess[] = { 100.0f };
    //////////////////////////
    
    // set up lights.
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT1);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    
    
    /*****************************************For Background***********************/
    
    
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_LIGHT0); //Enable light #0
    // glEnable(GL_LIGHT1); //Enable light #1
    glEnable(GL_NORMALIZE); //Automatically normalize normals
       glEnable(GL_CULL_FACE);
    
}


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


void drawGrid()
{
    glPushMatrix();
    glTranslatef(0.0f,50.0f,0.0f);
    glBegin(GL_LINES);
    int gridsize  = 7000;
    for(int i =-gridsize;i<=gridsize;i+=100)
    {
        if(i==0)
            glColor3f(0.0f,1.0f,0.0f);
        else if(i%1000==0)
            glColor3f(1.0f,0.0f,0.0f);
        else
            glColor3f(1.0f,1.0f,1.0f);
        glVertex3d(-gridsize,0.0f,i);
        glVertex3d(gridsize,0.0f,i);
        
        glVertex3d(i,0.0f,-gridsize);
        glVertex3d(i,0.0f,gridsize);
        
    }
    glEnd();
    glColor3f(1.0f,1.0f,1.0f);
    glPopMatrix();
}
bool gridEnabled = false; 
void drawHandler()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0);                          // Enables Clearing Of The Depth Buffer
    
    // glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, window.size.x, window.size.y);
    
    
    //*   //Add ambient light
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
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);//*/
    //glLoadIdentity();
    glm::mat4 model, view, projection;
    camera.Update();
    camera.GetMatricies(projection, view, model);
    
    glm::mat4 mvp = projection* view * model;       //Compute the mvp matrix
    glLoadMatrixf(glm::value_ptr(mvp));
    glColor3f(1.0f,1.0f,1.0f);
    glDisable(GL_CULL_FACE); //had to disable culling as it is not done in background
    //
    if(gridEnabled)
        drawGrid();
    background->DrawGLScene();
 
    glEnable(GL_CULL_FACE);

    neoQuad->draw();
    dronedemort->draw();
    mamaQuad->draw();

     glCullFace(GL_BACK);
     glFrontFace(GL_CCW);
    glutSwapBuffers();
    
}


void keypressHandler(unsigned char key, int x, int y)
{
    switch (key)
    {
        
        case 'd':
            quadrotor->yawQuad(-2);
            break;
        case 'a':
            quadrotor->yawQuad(2);
            break;
        case 's':
            quadrotor->pitchQuad(2);
            break;
        case 'w':
            quadrotor->pitchQuad(-2);
            break;
        case 'q':
            quadrotor->rollQuad(2);
            break;
        case 'e':
            quadrotor->rollQuad(-2);
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
            else if(quadrotor == dronedemort)
                quadrotor = mamaQuad;
            else
                quadrotor = neoQuad;
            break;
        case 'g':
            gridEnabled = !gridEnabled;
            break;
        case 13:
            exit(0);
            
    };
    CameraKeyboardFunc(key,x,y);
    //glutPostRedisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv); //initialize glut 
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //initialize display mode
    
    glutInitWindowSize(1024, 512);
    glutInitWindowPosition(0, 0);
    window.window_handle = glutCreateWindow("Age of Quadrones"); //create window
    
    
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
    mamaQuad = new MamaQuad();
    quadrotor = dronedemort;
  //  neoQuad->moveAbs(10,60,0);
  //  dronedemort->moveAbs(-50, 60 ,0);
    
  //  mamaQuad-> moveAbs(60,60,0);
    
    background = new Background();
    background->SetupWorld();
    initializeRendering();//Setup camera
    camera.SetCameraType(CameraType::FREE);
    camera.SetPosition(glm::vec3(0, 7000, 2502));
    camera.SetLookAt(glm::vec3(0, 60, 2500));
    camera.SetClipping(.1, 80000);
    camera.SetFOV(45);
   // camera.SetToFollow(*neoQuad,-glm::vec3 (100.0f,10.0f,0));
    
    camera.SetCameraModePoint(vec3(0,60,0),vec3(0,60,300));
    
    //tempcode
    z = 2500;
   // neoQuad->yawQuad(90);
    //quadrotor->rollQuad(20);
//    quadrotor->pitchQuad(20);
    
    //Start the glut loop!
    //*/
    glutMainLoop();
    
    return 0;
    
    
}

