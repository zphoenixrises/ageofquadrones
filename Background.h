
#ifndef Background_H
#define Background_H
#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
#include <unistd.h>     // Header file for sleeping.
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include <math.h>       // Header file for trigonometric functions.

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
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
} Image;

class Background
{
private:
    SECTOR sector1;
    
    GLuint loop;             // general loop variable
    GLuint texture[3];       // storage for 3 textures;
    GLuint filter;       // texture filtering method to use (nearest, linear, linear + mipmaps)
    
public:
    Background();
    // loads the world from a text file.
    void SetupWorld();
    // helper for SetupWorld.  reads a file into a string until a nonblank, non-comment line
    // is found ("/" at the start indicating a comment); assumes lines < 255 characters long.
    void readstr(FILE *f, char *string);

    // quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
    // See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
    int ImageLoad(char *filename, Image *image);
    // Load Bitmaps And Convert To Textures
    GLvoid LoadGLTextures();
    
    
    /* The main drawing function. */
    GLvoid DrawGLScene();
};
#endif