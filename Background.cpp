#include "Background.h"


#include <stdio.h>      // Header file for standard file i/o.
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
    limits = .8;
    X=0, Y=0;
    currentParticle = 1;
}

// loads the world from a text file.
void Background::SetupWorld()
{
    SetupItem((const char*)"Data/world.txt",sector1);//world
    SetupItem((const char*)"Data/grass.txt",sector2);//grass
    SetupItem((const char*)"Data/floor.txt",sector3);//floor
    
    
}

void Background::SetupItem(char* path,SECTOR &sector)
{
    float x, y, z, u, v;
    int vert;
    int numtriangles;
    FILE *filein;        // file to load the world from
    char oneline[255];
    
    filein = fopen(path, "rt");
    if (filein==NULL)
    {
        printf("file not found");
        fclose (filein);
    }
    //printf("Successfully read:%s\n",path);
    
    readstr(filein, oneline);
    sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);
    
    sector.numtriangles = numtriangles;
    sector.triangle = (TRIANGLE *) malloc(sizeof(TRIANGLE)*numtriangles);
    
    for (loop = 0; loop < numtriangles; loop++) {
        for (vert = 0; vert < 3; vert++) {
            readstr(filein,oneline);
            sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
            sector.triangle[loop].vertex[vert].x = x;
            sector.triangle[loop].vertex[vert].y = y;
            sector.triangle[loop].vertex[vert].z = z;
            sector.triangle[loop].vertex[vert].u = u;
            sector.triangle[loop].vertex[vert].v = v;
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
    //printf("Width of %s: %lu\n", filename, image->sizeX);
    
    // read the height 
    image->sizeY = getint (file);
    //printf("Height of %s: %lu\n", filename, image->sizeY);
    
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
    Image *image1 , *image2 , *image3, *image4, *image5, *image6, *image7, *image8, *image9;
/*  
   Image image[9];
    char imagePaths[][40]={"Data/download.bmp","Data/floor.bmp","Data/grass.bmp","Data/sky.bmp",
        "Data/images.bmp","Data/security.bmp","Data/comm.bmp","Data/laser_gun.bmp","Data/tower.bmp"};
    
    for(int i=0;i<9;i++)
        if (!ImageLoad(imagePaths, image[i])) 
            exit(1);
     //*/   
    
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    
    if (!ImageLoad("Data/download.bmp", image1)) {
        exit(1);
        
    }
    
    image2 = (Image *) malloc(sizeof(Image));
    if (image2 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    
    if (!ImageLoad("Data/floor.bmp", image2)) {
        exit(1);
    }    
    
    image3 = (Image *) malloc(sizeof(Image));
    if (image3 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    
    if (!ImageLoad("Data/grass.bmp", image3)) {
        exit(1);  
    }
    
    image4 = (Image *) malloc(sizeof(Image));
    if (image4 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    
    if (!ImageLoad("Data/sky.bmp", image4)) {
        exit(1);  
    }
    
    image5 = (Image *) malloc(sizeof(Image));
    if (image5 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    
    if (!ImageLoad("Data/images.bmp", image5)) {
        exit(1);  
    }
    
    image6 = (Image *) malloc(sizeof(Image));
    if (image6 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    
    if (!ImageLoad("Data/security.bmp", image6)) {
        exit(1);  
    }
    
    image7 = (Image *) malloc(sizeof(Image));
    if (image7 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    
    if (!ImageLoad("Data/comm.bmp", image7)) {
        exit(1);  
    }
    
    image8 = (Image *) malloc(sizeof(Image));
    if (image8 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    
    if (!ImageLoad("Data/laser_gun.bmp", image8)) {
        exit(1);  
    }
    
    image9 = (Image *) malloc(sizeof(Image));
    if (image9 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    
    if (!ImageLoad("Data/tower.bmp", image9)) {
        exit(1);  
    }
    
    // Create Textures  
    glGenTextures(8, &texture[0]);
    
    // nearest filtered texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale cheaply when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale cheaply when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image3->sizeX, image3->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image3->data);
    
    free(image3);
    
    // linear filtered texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    
    free(image1);
    
    // mipmapped texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image2->sizeX, image2->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image2->data);
    
    free(image2); 
    
    // mipmapped texture
    glBindTexture(GL_TEXTURE_2D, texture[3]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image4->sizeX, image4->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image4->data);
    
    free(image4); 
    
    // mipmapped texture
    glBindTexture(GL_TEXTURE_2D, texture[4]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image5->sizeX, image5->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image5->data);
    
    free(image5); 
    
    // mipmapped texture
    glBindTexture(GL_TEXTURE_2D, texture[5]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image6->sizeX, image6->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image6->data);
    
    free(image6); 
    
    // mipmapped texture
    glBindTexture(GL_TEXTURE_2D, texture[6]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image7->sizeX, image7->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image7->data);
    
    free(image7); 
    
    // mipmapped texture
    glBindTexture(GL_TEXTURE_2D, texture[7]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image8->sizeX, image8->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image8->data);
    
    free(image8); 
    
    // mipmapped texture
    glBindTexture(GL_TEXTURE_2D, texture[8]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image9->sizeX, image9->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image9->data);
    
    free(image9); 
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
    //GLfloat sceneroty;
    int numtriangles;
    
    glBindTexture(GL_TEXTURE_2D, texture[1]); // pick the texture.
    
    
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
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    
    numtriangles = sector2.numtriangles;
    
    for (loop=0; loop<numtriangles; loop++) {        // loop through all the triangles
        glBegin(GL_TRIANGLES);          
        glNormal3f( 0.0f, 0.0f, 1.0f);
        
        x_m = sector2.triangle[loop].vertex[0].x;
        y_m = sector2.triangle[loop].vertex[0].y;
        z_m = sector2.triangle[loop].vertex[0].z;
        u_m = sector2.triangle[loop].vertex[0].u;
        v_m = sector2.triangle[loop].vertex[0].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);
        
        x_m = sector2.triangle[loop].vertex[1].x;
        y_m = sector2.triangle[loop].vertex[1].y;
        z_m = sector2.triangle[loop].vertex[1].z;
        u_m = sector2.triangle[loop].vertex[1].u;
        v_m = sector2.triangle[loop].vertex[1].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);
        
        x_m = sector2.triangle[loop].vertex[2].x;
        y_m = sector2.triangle[loop].vertex[2].y;
        z_m = sector2.triangle[loop].vertex[2].z;
        u_m = sector2.triangle[loop].vertex[2].u;
        v_m = sector2.triangle[loop].vertex[2].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);        
        
        glEnd();        
    }
    
    
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    numtriangles = sector3.numtriangles;
    
    for (loop=0; loop<numtriangles; loop++) {        // loop through all the triangles
        glBegin(GL_TRIANGLES);          
        glNormal3f( 0.0f, 0.0f, 1.0f);
        
        x_m = sector3.triangle[loop].vertex[0].x;
        y_m = sector3.triangle[loop].vertex[0].y;
        z_m = sector3.triangle[loop].vertex[0].z;
        u_m = sector3.triangle[loop].vertex[0].u;
        v_m = sector3.triangle[loop].vertex[0].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);
        
        x_m = sector3.triangle[loop].vertex[1].x;
        y_m = sector3.triangle[loop].vertex[1].y;
        z_m = sector3.triangle[loop].vertex[1].z;
        u_m = sector3.triangle[loop].vertex[1].u;
        v_m = sector3.triangle[loop].vertex[1].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);
        
        x_m = sector3.triangle[loop].vertex[2].x;
        y_m = sector3.triangle[loop].vertex[2].y;
        z_m = sector3.triangle[loop].vertex[2].z;
        u_m = sector3.triangle[loop].vertex[2].u;
        v_m = sector3.triangle[loop].vertex[2].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);        
        
        glEnd();
        
        
    }
    //*
    sky();
    power();
    security();
    Draw_smoke();
    //*/
    glDisable(GL_TEXTURE_2D);                    // Enable texture mapping.
    
    glPopMatrix();
    // since this is double buffered, swap the buffers to display what just got drawn.
    // glutSwapBuffers();
}



void Background::sky(){
    
    
    GLUquadric *sky = gluNewQuadric(); 
    
    gluQuadricTexture(sky,GL_TRUE); 
    
    
    glBindTexture(GL_TEXTURE_2D,texture[3]);
    
    glPushMatrix();
    glTranslatef(0.0,0.0,0.0);
    gluSphere(sky,40.0,100,100);
    glPopMatrix();
    
    gluDeleteQuadric(sky);      
    
}



void Background::power(){
    
    
    GLUquadric *power = gluNewQuadric(); 
    
    gluQuadricTexture(power,GL_TRUE); 
    
    
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    
    glPushMatrix();
    glTranslatef(-20.0,1.2,3.75);
    gluSphere(power,0.2,100,100);
    glPopMatrix();
    
    gluDeleteQuadric(power);
    
    
    GLUquadric *power_tower = gluNewQuadric(); 
    
    gluQuadricTexture(power_tower,GL_TRUE); 
    
    
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    
    glPushMatrix();
    
    glTranslatef(-20.0,0.0,3.75);
    glRotatef(-90,1,0,0);
    gluCylinder(    power_tower,
                    0.08,
                    0.08,
                    1.0,
                    100,
                    100);
    
    glPopMatrix();
    
    gluDeleteQuadric(power_tower); 
    
    
    GLUquadric *power_outlet = gluNewQuadric(); 
    
    gluQuadricTexture(power_outlet,GL_TRUE); 
    
    
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    
    glPushMatrix();
    
    glTranslatef(-20.0,2.0,3.75);
    glRotatef(-90,1,0,0);
    gluCylinder(    power_outlet,
                    0.2,
                    0.2,
                    3.5,
                    100,
                    100);
    
    glPopMatrix();
    
    gluDeleteQuadric(power_outlet); 
    
    
    GLUquadric *chip_tower = gluNewQuadric(); 
    
    gluQuadricTexture(chip_tower,GL_TRUE); 
    
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    
    glPushMatrix();
    
    glTranslatef(-20.0,0.0,3.0);
    glRotatef(-90,1,0,0);
    gluCylinder(    chip_tower,
                    0.08,
                    0.08,
                    0.7,
                    100,
                    100);
    
    glPopMatrix();
    
    gluDeleteQuadric(chip_tower); 
    
    
    glBindTexture(GL_TEXTURE_2D,texture[6]);
    
    glPushMatrix();
    
    //glTranslatef(-20.0,0.0,3.0);
    //glRotatef(-90,1,0,0);
    glBegin(GL_QUADS);
    //glNormal3f( 0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0,0.0);
    glVertex3f(-20.0,0.7,2.975);
    glTexCoord2f(0.0,0.0);
    glVertex3f(-20.0,0.8,2.975);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-20.0,0.8,3.025);
    glTexCoord2f(0.0,0.0);
    glVertex3f(-20.0,0.7,3.025);
    glEnd();
    
    glPopMatrix();
    
    
}



void Background::security() {
    
    GLUquadric *security = gluNewQuadric(); 
    
    gluQuadricTexture(security,GL_TRUE); 
    
    
    glBindTexture(GL_TEXTURE_2D,texture[5]);
    
    glPushMatrix();
    
    glTranslatef(0.0,0.0,2.0);
    glRotatef(-90,1,0,0);
    gluCylinder(    security,
                    0.05,
                    0.05,
                    0.5,
                    50,
                    50);
    
    glPopMatrix();
    
    gluDeleteQuadric(security); 
    
    glBindTexture(GL_TEXTURE_2D,texture[6]);
    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0,0.0);
    glVertex3f(0.12,0.4,2.1);
    glTexCoord2f(0.0,0.0);
    glVertex3f(0.12,0.3,2.1);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-0.12,0.3,2.1);
    glTexCoord2f(0.0,0.0);
    glVertex3f(-0.12,0.4,2.1);
    glEnd();
    glPopMatrix();  
    
    
    GLUquadric *laser_gun = gluNewQuadric(); 
    
    gluQuadricTexture(laser_gun,GL_TRUE); 
    
    glBindTexture(GL_TEXTURE_2D,texture[7]);
    
    glPushMatrix();
    glTranslatef(0.0,0.6,2.0);
    gluSphere(laser_gun,0.08,100,100);
    glPopMatrix();
    
    gluDeleteQuadric(laser_gun);
    
}


GLvoid Background:: Draw_smoke(void) {
    
    int thingy = 1;
    bool check = false;
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(-20.0,5.5,5.75);
    
    /*
     *       
     *       glColor3d(.3, .1, 0);
     *       
     *       glutSolidCube(.3);
     *       glPopMatrix();
     */
    if (check == false) {
        float R, G, B;
        glPushMatrix();
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < MAX_PARTICLES; i++) {
            //R = rand() % 100 + 1;
            //G = rand() % 100 + 1;
            //B = rand() % 100 + 1;
            //glColor3d(R*.01, G*.01, B*.01);
            glColor3f(1.0, 1.0, 1.0);
            //glColor3d(10, 10, 0);
            //glColor3d(0, 0+posY[i], 0);
            glVertex3f(X-.01, Y, -2);
            glVertex3f(X+.01, Y, -2);
            glVertex3f(X, Y+.02, -2);
            X = posX[i];
            Y = posY[i];
        }
        glEnd();
        glPopMatrix();
        check = true;
    }
    switch(thingy){
        case 1:
            //Sleep(1);
            moveParticles(currentParticle);
            if (currentParticle != MAX_PARTICLES) {
                currentParticle++;
            }
            
             // glutPostRedisplay();
            break;
            
            
            
    }
    
    //glutSwapBuffers();
}


void Background::moveParticles(int amount_of_particles) {
    srand (time (NULL) );
    float myX, myY;
    
    for (int i = 0; i < amount_of_particles; i++) {         
        myX = rand() % 3 + 1;
        if(myX==1 && posX[i]<=limits ){
            int mytemp = rand() % 100 + 1;
            int temp = rand() % 5 + 1;
            posX[i]+=temp*.001;
            posY[i]+=mytemp*.0004;
        }
        if(myX==2){posX[i]+=.00;posY[i]+=.01;}
        if(myX==3 && posX[i]>=-limits){
            int temp = rand() % 5 + 1;
            int mytemp = rand() % 100 + 1;
            posX[i]-=temp*.001;
            posY[i]+=mytemp*.0004;
        }
        ///////////////////////////////////////////
        if(posY[i]>=limits){    
            posY[i]=0;
            posX[i]=0;
        }
    }
}