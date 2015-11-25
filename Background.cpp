#include "Settings.h"

#include "Background.h"


#include <stdio.h>      // Header file for standard file i/o.

#include "raygl/raygl.h"
#include "raygl/raygldefs.h"

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



GLvoid Background::LoadGLTextures()
{
    
    int w[noTextures], h[noTextures], c;
    unsigned char *data[noTextures];
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Allocate space for RayGL textures.
    #ifdef RAYGL_ENABLE
    Image *image[noTextures];
    #else
    MyImage *image[noTextures];
    #endif
    for (c = 0; c < noTextures; c++) {
        
        image[c] = 
        #ifdef RAYGL_ENABLE
        (Image *) malloc(sizeof(Image));
        #else
        (MyImage *) malloc(sizeof(MyImage));
        
        #endif
        if (image[c] == NULL) exit(0);
    }
    
    
    
    
    char image_paths[][50]={"Data/download.ppm",
        "Data/floor.ppm",
        "Data/grass.ppm",
        "Data/sky.ppm",
        "Data/images.ppm",
        "Data/security.ppm",
        "Data/comm.ppm",
        "Data/laser_gun.ppm",
        "Data/tower.ppm",
    };
    // Create Textures  
    
    glGenTextures(9, &texture[0]);
    // Create a texture (OpenGL and RayGL variants).
    for(int i=0;i<noTextures;i++)
    {
        
        #ifdef RAYGL_ENABLE
        
        if (!imageLoad(image_paths[i], image[i])) exit(0);
        #else
        image[i]->data = PGM_FILE_READ(image_paths[i], &image[i]->sizeX, &image[i]->sizeY, &c); 
        #endif
        glBindTexture(GL_TEXTURE_2D, texture[i]);   // 2d texture (x and y size)
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image[i]->sizeX, image[i]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image[i]->data);
        
        #ifndef RAYGL_ENABLE
        free(image[i]->data);
        free(image[i]);
        #endif
    }
    
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
   // glLoadIdentity();
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
    
    /*
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
  
  */
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