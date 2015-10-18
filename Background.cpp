#include "Background.h"


/*
 * getint and getshort are help functions to load the bitmap byte by byte on 
 * SPARC platform (actually, just makes the thing work on platforms of either
 * endianness, not just Intel's little endian)
 */
static unsigned int getint(FILE *fp)
{
    int c, c1, c2, c3;
    
    // get 4 bytes
    c = getc(fp);  
    c1 = getc(fp);  
    c2 = getc(fp);  
    c3 = getc(fp);
    
    return ((unsigned int) c) +   
    (((unsigned int) c1) << 8) + 
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp)
{
    int c, c1;
    
    //get 2 bytes
    c = getc(fp);  
    c1 = getc(fp);
    
    return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

Background::Background()
{
    filter = 0; 

}

// loads the world from a text file.
void Background::SetupWorld()
{
    float x, y, z, u, v;
    int vert;
    int numtriangles;
    FILE *filein;        // file to load the world from
    char oneline[255];
    
    filein = fopen("Data/world.txt", "rt");
    
    readstr(filein, oneline);
    sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);
    
    sector1.numtriangles = numtriangles;
    sector1.triangle = (TRIANGLE *) malloc(sizeof(TRIANGLE)*numtriangles);
    
    for (loop = 0; loop < numtriangles; loop++) {
        for (vert = 0; vert < 3; vert++) {
            readstr(filein,oneline);
            sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
            sector1.triangle[loop].vertex[vert].x = x;
            sector1.triangle[loop].vertex[vert].y = y;
            sector1.triangle[loop].vertex[vert].z = z;
            sector1.triangle[loop].vertex[vert].u = u;
            sector1.triangle[loop].vertex[vert].v = v;
        }
    }
    
    fclose(filein);
    return;
}

// helper for SetupWorld.  reads a file into a string until a nonblank, non-comment line
// is found ("/" at the start indicating a comment); assumes lines < 255 characters long.
void Background::readstr(FILE *f, char *string)
{
    do {
        fgets(string, 255, f); // read the line
    } while ((string[0] == '/') || (string[0] == '\n'));
    return;
}



// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
int Background::ImageLoad(char *filename, Image *image) 
{
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // used to convert bgr to rgb color.
    
    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL) {
        printf("File Not Found : %s\n",filename);
        return 0;
    }
    
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);
    
    // No 100% errorchecking anymore!!!
    
    // read the width
    image->sizeX = getint (file);
    printf("Width of %s: %lu\n", filename, image->sizeX);
    
    // read the height 
    image->sizeY = getint (file);
    printf("Height of %s: %lu\n", filename, image->sizeY);
    
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;
    
    // read the planes
    planes = getshort(file);
    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }
    
    // read the bpp
    bpp = getshort(file);
    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }
    
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);
    
    // read the data. 
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;       
    }
    
    if ((i = fread(image->data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }
    
    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = image->data[i];
        image->data[i] = image->data[i+2];
        image->data[i+2] = temp;
    }
    
    // we're done.
    return 1;
}

// Load Bitmaps And Convert To Textures
GLvoid Background::LoadGLTextures() 
{       
    // Load Texture
    Image *image1;
    
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    
    if (!ImageLoad((char*)"Data/mud.bmp", image1)) {
        exit(1);
    }        
    
    // Create Textures  
    glGenTextures(3, &texture[0]);
    
    // nearest filtered texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale cheaply when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale cheaply when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    
    // linear filtered texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    
    // mipmapped texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
};



/* The main drawing function. */
GLvoid Background::DrawGLScene()
{
    glEnable(GL_TEXTURE_2D);                    // Enable texture mapping.
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); //Main push
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // This Will Clear The Background Color To Black
   // glClearDepth(1.0);                          // Enables Clearing Of The Depth Buffer
    //glColor3f(1.0)
    glLoadIdentity();
    glScalef(200.0f,200.0f,200.0f);
    //glTranslatef(pos_x, pos_y, pos_z);
 //   glMultMatrixf(glm::value_ptr(Model)); //load Model matrix
    
    GLfloat x_m, y_m, z_m, u_m, v_m;
    GLfloat xtrans, ztrans, ytrans;
    GLfloat sceneroty;
    int numtriangles;

    glBindTexture(GL_TEXTURE_2D, texture[filter]);    // pick the texture.
    
    numtriangles = sector1.numtriangles;
    
    for (loop=0; loop<numtriangles; loop++) {        // loop through all the triangles
        glBegin(GL_TRIANGLES);          
        glNormal3f( 0.0f, 0.0f, 1.0f);
        
        x_m = sector1.triangle[loop].vertex[0].x;
        y_m = sector1.triangle[loop].vertex[0].y;
        z_m = sector1.triangle[loop].vertex[0].z;
        u_m = sector1.triangle[loop].vertex[0].u;
        v_m = sector1.triangle[loop].vertex[0].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);
        
        x_m = sector1.triangle[loop].vertex[1].x;
        y_m = sector1.triangle[loop].vertex[1].y;
        z_m = sector1.triangle[loop].vertex[1].z;
        u_m = sector1.triangle[loop].vertex[1].u;
        v_m = sector1.triangle[loop].vertex[1].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);
        
        x_m = sector1.triangle[loop].vertex[2].x;
        y_m = sector1.triangle[loop].vertex[2].y;
        z_m = sector1.triangle[loop].vertex[2].z;
        u_m = sector1.triangle[loop].vertex[2].u;
        v_m = sector1.triangle[loop].vertex[2].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);        
        
        glEnd();        
    }
    glDisable(GL_TEXTURE_2D);                    // Enable texture mapping.
    
    glPopMatrix();
    // since this is double buffered, swap the buffers to display what just got drawn.
   // glutSwapBuffers();
}
