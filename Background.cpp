/**************************************************************
 * Name   : Background.cpp
 * Purpose: Background functions are defined here
 * 
 * Author: Ahmad Hasan
 * Email : jarjishasan@gmail.com
 * 
 * CSci 446 / fall 2015
 *
 * Creation Date: 09/16/2015
 * ***********************************************************/


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
    limits = 4;
    X=0, Y=0;
    currentParticle = 100;
    radius= 0.2;
 
}

// loads the world from a text file.
void Background::SetupWorld()
{
    SetupItem((const char*)"Data/world.txt",sector1);//world
    //SetupItem((const char*)"Data/grass.txt",sector2);//grass
    //SetupItem((const char*)"Data/floor.txt",sector3);//floor
    
    
}

void Background::SetupItem(char* path,SECTOR &sector)
{
    float x, y, z, u, v;
    int vert;
    int numQuads;
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
    sscanf(oneline, "NUMPOLLIES %d\n", &numQuads);                             
      
    sector.numquads = numQuads;
    sector.quad = (QUADS *) malloc(sizeof(QUADS)*numQuads);
    
    for (loop = 0; loop < numQuads; loop++) {
        readstr(filein,oneline);
        char ch;
        sscanf(oneline, "%c", &ch);
        sector.quad[loop].normal.x = 0;
        sector.quad[loop].normal.y = 0;
        sector.quad[loop].normal.z = 0;
        if(ch=='X')
            sector.quad[loop].normal.x = 1;
        else if(ch=='Y')
            sector.quad[loop].normal.y = 1;
        else if(ch=='Z')
            sector.quad[loop].normal.z = 1;
        
        for (vert = 0; vert < 4; vert++) {
            readstr(filein,oneline);
            sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
            sector.quad[loop].vertex[vert].x = x;
            sector.quad[loop].vertex[vert].y = y;
            sector.quad[loop].vertex[vert].z = z;
            sector.quad[loop].vertex[vert].u = u;
            sector.quad[loop].vertex[vert].v = v;
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
    
    
    
    
    char image_paths[][50]={
        "Data/flame.ppm",
        "Data/download.ppm",
        "Data/grass.ppm",
        "Data/sky.ppm",
        "Data/images.ppm",
        "Data/security.ppm",
        "Data/comm.ppm",
        "Data/red.ppm",
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
        
        //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); 
        //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE); 
        //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE); 
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image[i]->sizeX, image[i]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image[i]->data);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[i]->sizeX, image[i]->sizeY, 
        //             0, GL_RGB, GL_UNSIGNED_BYTE, image[i]->data);
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
   
        
 
    glPushMatrix(); //Main push
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // This Will Clear The Background Color To Black
    // glClearDepth(1.0);                          // Enables Clearing Of The Depth Buffer
    //glColor3f(1.0)

    glScalef(200.0f,200.0f,200.0f);
    //glTranslatef(pos_x, pos_y, pos_z);
    //   glMultMatrixf(glm::value_ptr(Model)); //load Model matrix
    
    GLfloat x_m, y_m, z_m, u_m, v_m;
    //GLfloat sceneroty;
    int numquads;
    // Draw textured cube.
   /* glBindTexture(GL_TEXTURE_2D, texture[1]);   
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    #if RAYGL == 1
    rayglScaleTexture(2, 2, 1);                // Scale texture for PovRAY.
    rayglTranslateTexture(0.5, 0.5, 0);        // Translate texture for PovRAY.
    rayglTextureType(0);                       // Set texture type for PovRAY.
    #endif
    glTranslatef(-4,0,3);
    drawcube();
    //*/
    //* 
    
    /////EXTRA BOX
    glPushMatrix();
    glTranslatef(30.0,0.0,0.0);                 
    
    glBindTexture(GL_TEXTURE_2D, texture[1]); // pick the texture.
    glutSolidCube (4.0);
    glNormal3f(0,1,0);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                //undefined Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(0);                       // Set texture type for PovRAY.
    #endif
    glPopMatrix();
    
    
    glPushMatrix();
    glTranslatef(-30.0,0.0,30.0); 
    
    glBindTexture(GL_TEXTURE_2D, texture[1]); // pick the texture.
    glutSolidCube (4.0);
    glNormal3f(0,1,0);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                //undefined Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(0);                       // Set texture type for PovRAY.
    #endif
    glPopMatrix();
    ///////////////////
    
    glBindTexture(GL_TEXTURE_2D, texture[1]); // pick the texture.
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                //undefined Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(0);                       // Set texture type for PovRAY.
    #endif
    
    numquads = sector1.numquads;   
            
    for (loop=0; loop<numquads; loop++) {        // loop through all the quads
        glBegin(GL_QUADS);          
        glNormal3f( sector1.quad[loop].normal.x ,sector1.quad[loop].normal.y ,sector1.quad[loop].normal.z );
        
        x_m = sector1.quad[loop].vertex[0].x;
        y_m = sector1.quad[loop].vertex[0].y;
        z_m = sector1.quad[loop].vertex[0].z;
        u_m = sector1.quad[loop].vertex[0].u;
        v_m = sector1.quad[loop].vertex[0].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);
            
        x_m = sector1.quad[loop].vertex[1].x;
        y_m = sector1.quad[loop].vertex[1].y;
        z_m = sector1.quad[loop].vertex[1].z;
        u_m = sector1.quad[loop].vertex[1].u;
        v_m = sector1.quad[loop].vertex[1].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);
        
        x_m = sector1.quad[loop].vertex[2].x;
        y_m = sector1.quad[loop].vertex[2].y;
        z_m = sector1.quad[loop].vertex[2].z;
        u_m = sector1.quad[loop].vertex[2].u;
        v_m = sector1.quad[loop].vertex[2].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);  
        
        x_m = sector1.quad[loop].vertex[3].x;
        y_m = sector1.quad[loop].vertex[3].y;
        z_m = sector1.quad[loop].vertex[3].z;
        u_m = sector1.quad[loop].vertex[3].u;
        v_m = sector1.quad[loop].vertex[3].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);  
        
        
        
        glEnd();        
        
    }
    //*/
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(0);                       //undefined/ Set texture type for PovRAY.
    #endif
    //numquads = sector2.numquads;
    
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex3f(-80.0,  -0.01, -80.0);
    
    glTexCoord2f(80,0);
    glVertex3f(-80.0,  -0.01, 80.0);
    
    glTexCoord2f(80,80);
    glVertex3f(80.0,  -0.01, 80.0);
    
    glTexCoord2f(0,80);
    glNormal3f(0,1,0);
    glVertex3f(80.0,  -0.01, -80.0);
                              
    
    glEnd();
            

    //*
    
    sky();
    
    Draw_smoke();
    power();    
    double processtime = QuadTimer::GetProcessTime();
    if((processtime >= 147 && processtime <= 186 ) ||(processtime >= 0 && processtime <= 71 )  ){
    security();
    }
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
   
    #if RAYGL == 1
    rayglScaleTexture(1.0, 1.0, 1.0);                // Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(1);                       // Set texture type for PovRAY.
    //  rayglRotateTexture(0.0,0.0,90.0);
    //rayglRotateTexture(0.0,0.0,45.0);
    #endif
    
    glPushMatrix();
    glTranslatef(0.0,0.0,0.0);
    gluSphere(sky,80.0,100,100);
    glPopMatrix();
    
    gluDeleteQuadric(sky);      
    
}



void Background::power(){
    
    
    GLUquadric *power = gluNewQuadric(); 
    
    gluQuadricTexture(power,GL_TRUE); 
    
    
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(1);                       //undefined/ Set texture type for PovRAY.
    #endif
    
    glPushMatrix();
    glTranslatef(-20.0,1.2,3.75);
  
     if(QuadTimer::GetProcessTime() >= 243.5){radius+=0.5;};
    gluSphere(power,radius,100,100);
    glPopMatrix();
    
    gluDeleteQuadric(power);
    
    
    GLUquadric *power_tower = gluNewQuadric(); 
    
    gluQuadricTexture(power_tower,GL_TRUE); 
    
    
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(2);                       //undefined/ Set texture type for PovRAY.
    #endif
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
   
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(2);                       //undefined/ Set texture type for PovRAY.
    #endif
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
   
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(2);                       //undefined/ Set texture type for PovRAY.
    #endif
    
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
    
    if(QuadTimer::GetProcessTime() >= 0 && QuadTimer::GetProcessTime() <= 118 ){
        
    glBindTexture(GL_TEXTURE_2D,texture[6]);
    
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(0);                       //undefined/ Set texture type for PovRAY.
    #endif
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
}



void Background::security() {
    
    GLUquadric *security = gluNewQuadric(); 
    
    gluQuadricTexture(security,GL_TRUE); 
    
    
    glBindTexture(GL_TEXTURE_2D,texture[7]);
    
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(2);                       //undefined/ Set texture type for PovRAY.
    #endif
    glPushMatrix();
    
    glTranslatef(2.0,0.0,2.0);
    glRotatef(-90,0,1,0);
    gluCylinder(    security,
                    0.005,
                    0.005,
                    4.0,
                    50,
                    50);
 int i=0;   
for(i=0;i<9;i++){
    
    glTranslatef(0.0,0.1,0.0);
    
    gluCylinder(    security,
                    0.005,
                    0.005,
                    4.0,
                    50,
                    50);
    
    }
    glPopMatrix();
    
    gluDeleteQuadric(security); 
    
    
    
    glBindTexture(GL_TEXTURE_2D,texture[6]);
    
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(0);                       //undefined/ Set texture type for PovRAY.
    #endif
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
    
    
   
     
} 


GLvoid Background:: Draw_smoke(void) {
    
    bool check = false;
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(-20.0,5.5,5.75);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    
    #if RAYGL == 1
    rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
    rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
    rayglTextureType(0);                       //undefined/ Set texture type for PovRAY.
    #endif
    
    /*
     *       
     *       glColor3d(.3, .1, 0);
     *       
     *       glutSolidCube(.3);
     *       glPopMa trix();
     */
    if (check == false) {
        float R, G, B;
        glPushMatrix();
        for (int i = 0; i < currentParticle; i++) {
           // R = rand() % 100 + 1;
           // G = rand() % 100 + 1;
           // B = rand() % 100 + 1;
           // glColor3d(R*.01, G*.01, B*.01);
            
            glBegin(GL_QUADS);
            glColor4d(1.0, 0.0, 0.0, 1);
            //glColor3d(10, 10, 0)
            //glColor3d(0, 0+posY[i], 0);
            glVertex3f(X-.1, Y, -2);
            glVertex3f(X+.1, Y, -2);
            glVertex3f(X, Y+.2, -2);
            glVertex3f(X, Y+.2, -2);
            
            glEnd();
            X = posX[i];
            Y = posY[i];
        }
        glPopMatrix();
        check = true;
    }

    moveParticles(currentParticle);
    if (currentParticle < MAX_PARTICLES) {
        currentParticle++;
    }
    

    glPopMatrix();
}


void Background::moveParticles(int amount_of_particles) {
    srand (time (NULL) );
    float myX, myY;
    
    for (int i = 0; i < amount_of_particles; i++) {         
        myX = rand() % 3 + 1;
        if(myX==1 && posX[i]<=limits ){
            int mytemp = rand() % 100 + 1;
            int temp = rand() % 5 + 1;
            if(posX[i]<=2)
                posX[i]+=temp*.015;
            else posX[i]=0;
            posY[i]+=mytemp*.01;
        }
        if(myX==2){posX[i]+=.00;posY[i]+=.01;}
        if(myX==3 && posX[i]>=-limits){
            int temp = rand() % 5 + 1;
            int mytemp = rand() % 100 + 1;
            if(posX[i]>=-2)
                posX[i]-=temp*.015;
            else posX[i]=0;
            posY[i]+=mytemp*.01;
        }
        ///////////////////////////////////////////
        if(posY[i]>=limits){    
            posY[i]=0;
            posX[i]=0;
        }
    }
}