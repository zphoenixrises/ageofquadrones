#include "Settings.h"
#include "Ammo.h"
   
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "raygl/raygl.h"
#include "raygl/raygldefs.h"
vector<Ammo*> Ammo::bullets;

GLuint Ammo::EarthMap;
namespace AMMOTYPE{
    
ENUM getAmmotypeFromString(string ammoType)
{
    if(ammoType == "CANNONBALL")
        return CANNONBALL;
    if(ammoType == "LASER")
        return LASER;
    if(ammoType == "BLASTER")
        return BLASTER;

}
}

Ammo::Ammo(glm::vec3 startPosition, glm::vec3 direction, glm::vec4 color/*,void (*collidedEvent)(string)*/,float lifetime,Quadrotor*owner,AMMOTYPE::ENUM ammoType)
{
    position = startPosition;
    this->owner = owner;
    //quadCollisionEvent = collidedEvent; 
    this->color = color;
    this->startTime = time.GetProcessTime();
    this->lifetime = lifetime+startTime;
    quadricObj = gluNewQuadric();
    gluQuadricDrawStyle(quadricObj, GLU_FILL);
    gluQuadricTexture(quadricObj, GL_TRUE);
    gluQuadricNormals(quadricObj, GLU_SMOOTH);
    
    direction = glm::normalize(direction);
    glm::vec3 current_axis(0,0,1);
    glm::vec3 comRotationAxis = glm::cross(current_axis,direction);
    float axisAngle = glm::angle(current_axis,direction);
    orientationMatrix = glm::mat4(1.0f);
    orientationMatrix = glm::rotate(orientationMatrix,axisAngle,comRotationAxis);
    this->ammoType = ammoType;
    //  
    this->direction = direction;
    
    bullets.push_back(this);
}
void Ammo::LoadGLTextures()
{
    int w, h, c;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    #if RAYGL == 1
    Image *image[1];
    for (c = 0; c < 1; c++) {
        image[c] = (Image *) malloc(sizeof(Image));
        if (image[c] == NULL) exit(0);
    }
    #else
    uchar*data;

    #endif

    #if RAYGL == 1
    if (!imageLoad("Data/red.ppm", image[0])) exit(0);
    #else
    data = PGM_FILE_READ("Data/red.ppm", &w, &h, &c); 
    #endif 
    
    // Load texture map for sphere.
    //data = PGM_FILE_READ("Data/image-1.ppm", &w, &h, &c); 
    /*BITMAPINFO *BitmapInfo; 
     * data = LoadDIBitmap("Data/perf_white.bmp",&BitmapInfo);
     * w = BitmapInfo->bmiHeader.biWidth; 
     * h = BitmapInfo->bmiHeader.biHeight;
     */
    glGenTextures(1, &EarthMap);
    glBindTexture(GL_TEXTURE_2D, EarthMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );   
    #if RAYGL == 1
    gluBuild2DMipmaps(GL_TEXTURE_2D,3, image[0]->sizeX, image[0]->sizeY,GL_RGB,GL_UNSIGNED_BYTE,image[0]->data);
    #else
    
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data );
    free(data);
    #endif
    

}


void Ammo::drawAll()
{
    int i=0,del=-1;
    for(vector<Ammo*>::iterator it = bullets.begin();it!=bullets.end();++it,++i)
    {
        if(NULL!=*it)
        {
            Quadrotor* collidedQuad = (*it)->hasCollided();
            if(NULL!=collidedQuad && (*it)->owner!=collidedQuad)
            {
                collidedQuad->collided();
                delete *it;
                *it=NULL;
                continue;
            }
            (*it)->draw();
            if((*it)->time.GetProcessTime()> (*it)->lifetime)
            {   
                del = i;
              //  bullets.erase(bullets.begin()+i);
                //delete this;
            }
        }
    }
    if(del!=-1)
        bullets.erase(bullets.begin()+del);
    

}

void Ammo::fire(glm::vec3 startPosition, glm::vec3 position,glm::vec4 color/*,void (*collidedEvent)(string)*/,float lifetime,Quadrotor* owner,AMMOTYPE::ENUM ammoType)
{
    glm::vec3 direction = position-startPosition;
    
    Ammo *ammo = new Ammo(startPosition, direction,color,lifetime,owner,ammoType);
    
}

void Ammo::fire(glm::vec3 startPosition, string quadname, glm::vec4 color, float lifetime,Quadrotor* owner,AMMOTYPE::ENUM ammoType)
{
    glm::vec3 direction = Quadrotor::getQuadFromName(quadname)->getQuadPosition()-startPosition;
    
    Ammo *ammo = new Ammo(startPosition, direction,color,lifetime,owner,ammoType);
    

}

void Ammo::draw()
{
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(position.x,position.y,position.z);
    glMultMatrixf(glm::value_ptr(orientationMatrix));
    glColor3f(color.x,color.y,color.z);
    glBindTexture(GL_TEXTURE_2D, EarthMap);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
    

    if(ammoType == AMMOTYPE::CANNONBALL)
    {
        gluSphere(quadricObj, 2.0f, 5, 5); //draw head
    }   
    else if(ammoType == AMMOTYPE::LASER)
    {
        #if RAYGL == 1
        rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
        rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
        rayglTextureType(2);                       //undefined/ Set texture type for PovRAY.
        #endif
        gluCylinder(quadricObj,1.0,1.0,30.0,5,5);
        glRotatef(180,1,0,0);
        
        #if RAYGL == 1
        rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
        rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
        rayglTextureType(0);                       //undefined/ Set texture type for PovRAY.
        #endif
        gluDisk(quadricObj,0.0f,1.0f,5,3);
        //glRotatef(90,1,0,0);
        glTranslatef(0,0,-30);
        glRotatef(180,1,0,0);
        gluDisk(quadricObj,0.0f,1.0f,5,3);
    }
    else if(ammoType == AMMOTYPE::BLASTER)
    {
        glTranslatef(0,0,10);
        glScalef(1,1,10);
        
        #if RAYGL == 1
        rayglScaleTexture(1, 1, 1);                // Scale texture for PovRAY.
        rayglTranslateTexture(0, 0, 0);            // Translate texture for PovRAY.
        rayglTextureType(1);                       //undefined/ Set texture type for PovRAY.
        #endif
        gluSphere(quadricObj, 1.0f, 5, 5); //draw head
        
    }
    glPopMatrix();
    
    position += direction*(float)(500*time.getTimeDiffSec());


}

Quadrotor* Ammo::hasCollided()
{
    for(vector<Quadrotor*>::iterator it = Quadrotor::quads.begin(); it!=Quadrotor::quads.end(); ++it)
    {
        if(glm::length((*it)->getQuadPosition()-position)< COLLISIONRADIUS)
            return *it;
    }
    return NULL;

}


Ammo::~Ammo() 
{
    gluDeleteQuadric( quadricObj);

}
