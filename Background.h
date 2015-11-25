/**************************************************************
 * Name   : Background.h
 * Purpose: Header File for Background
 * 
 * Author: Ahmad Hasan
 * Email : jarjishasan@gmail.com
 * 
 * CSci 446 / fall 2015
 *
 * Creation Date: 09/16/2015
 * ***********************************************************/
#ifndef Background_H
#define Background_H

#include <GL/glut.h>    // Header File For The GLUT Library 

#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include "QuadTimer.h"  // Header file for the timer

typedef struct {         // vertex coordinates - 3d and texture
    GLfloat x, y, z;     // 3d coords.
    GLfloat u, v;        // texture coords.
} VERTEX;

typedef struct {         // triangle
    VERTEX vertex[3];    // 3 vertices array
} TRIANGLE;
 
typedef struct {         // sector of a 3d environment
    int numtriangles;    // number of triangles in the sector
    TRIANGLE* triangle;  // pointer to array of triangles.
} SECTOR;

/* Image type - contains height, width, and data */
typedef struct {
    int sizeX;
    int sizeY;
    unsigned char *data;
} MyImage;

const  int MAX_PARTICLES = 1000;
const  int MIN_PARTICLES = 10;
#define  noTextures 9

class Background
{
private:
    
    GLuint texture[noTextures];
    SECTOR sector1;
    SECTOR sector2;
    SECTOR sector3;
    
    GLuint loop;             // general loop variable
    GLuint filter;       // texture filtering method to use (nearest, linear, linear + mipmaps)
    
    ////////for smoke
    float limits;
    float X, Y;
    int currentParticle;
    float posX[MAX_PARTICLES], posY[MAX_PARTICLES];
    
    ////////for power release
    float radius= 0.2;
    
    
public:
    Background();
    // loads the world from a text file.
    void SetupWorld();
    void SetupItem(char* path,SECTOR &sector);
    // helper for SetupWorld.  reads a file into a string until a nonblank, non-comment line
    // is found ("/" at the start indicating a comment); assumes lines < 255 characters long.
    void readstr(FILE *f, char *string);
    
    // quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
    // See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
    //int ImageLoad(char *filename, Image *image);
    // Load Bitmaps And Convert To Textures
    GLvoid LoadGLTextures();
    void sky();
    void power();
    void security();
    
    GLvoid Draw_smoke();
    
    /* The main drawing function. */
    GLvoid DrawGLScene();
    void moveParticles(int amount_of_particles);
};
#endif