//camera.cpp
#include "Settings.h"
#include <string.h>
#include "camera.h"
#include "raygl/raygl.h"
#include "raygl/raygldefs.h"

using namespace std;

Camera::Camera() {
    camera_type = CameraType:: ORTHO;
    camera_up = glm::vec3(0, 1, 0);
    field_of_view = 45;
    rotation_quaternion = glm::quat(1, 0, 0, 0);
    camera_position_delta = glm::vec3(0, 0, 0);
    camera_scale = 5.0f;
    max_pitch_rate = 5;
    max_heading_rate = 5;
    move_camera = false;
    cameraMode = CameraModes::WORLD;
    timeline = new Timeline("CAM");
    readTimeline = true;
    isExecuting = false;
    isMoving = false;
}
Camera::~Camera() {
}

void Camera::Reset() {
    camera_up = glm::vec3(0, 1, 0);
}

void Camera::Update() {
    executeTimelineCommands();
    if(cameraMode == CameraModes::FOLLOW_QUAD)
    {
        SetLookAt(quadrotor->getQuadPosition());
        glm::vec3 axisVector,upVector;
        quadrotor->getOrientation(axisVector,upVector,distance);
        SetPosition(quadrotor->getQuadPosition()-axisVector);
        camera_up = upVector;
    }
    else if(cameraMode == CameraModes::FOLLOW_QUAD_UPRIGHT)
    {
        SetLookAt(quadrotor->getQuadPosition());
        glm::vec3 axisVector,upVector; 
        quadrotor->getOrientation(axisVector,upVector,distance);
        SetPosition(quadrotor->getQuadPosition()-axisVector);
        camera_up = glm::vec3(0.0f,1.0f,0.0f);
    }
    else if(cameraMode == CameraModes::WORLD)
    {
        
        
    }
    else if(cameraMode == CameraModes::CIRCLEMOTION)
    {
        
    }
    
    camera_direction = glm::normalize(camera_look_at - camera_position);
    //need to set the matrix state. this is only important because lighting doesn't work if this isn't done
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    glViewport(viewport_x, viewport_y, window_width, window_height);
    
    if (camera_type == CameraType::ORTHO) {
        //our projection matrix will be an orthogonal one in this case
        //if the values are not floating point, this command does not work properly
        //need to multiply by aspect!!! (otherise will not scale properly)
        projection = glm::ortho(-1.5f * float(aspect), 1.5f * float(aspect), -1.5f, 1.5f, -10.0f, 10.f);
    } else if (camera_type == CameraType::FREE) {
        projection = glm::perspective(field_of_view, aspect, near_clip, far_clip);
         //detmine axis for pitch rotation
        glm::vec3 axis = glm::cross(camera_direction, camera_up);
        //compute quaternion for pitch based on the camera pitch angle
        glm::quat pitch_quat = glm::angleAxis(camera_pitch, axis);
        //determine heading quaternion from the camera up vector and the heading angle
        glm::quat heading_quat = glm::angleAxis(camera_heading, camera_up);
        //add the two quaternions
        glm::quat temp = glm::cross(pitch_quat, heading_quat);
        temp = glm::normalize(temp);
        //update the direction from the quaternion
        camera_direction = glm::rotate(temp, camera_direction);
        //add the camera delta
        camera_position += camera_position_delta;
        //set the look at to be infront of the camera
        camera_look_at = camera_position + camera_direction * 1.0f;
        //damping for smooth camera
        camera_heading *= .5;
        camera_pitch *= .5;
        camera_position_delta = camera_position_delta * .8f;
    }
    //compute the MVP
    //     view = glm::lookAt(camera_position, camera_look_at, camera_up);
    //     model = glm::mat4(1.0f);
    //     MVP = projection * view * model;
    //     glLoadMatrixf(glm::value_ptr(MVP));
    gluPerspective(field_of_view, aspect, near_clip, far_clip);
    
    gluLookAt(camera_position.x,camera_position.y,camera_position.z,
              camera_look_at.x,camera_look_at.y,camera_look_at.z,
              camera_up.x,camera_up.y,camera_up.z
    );
    
    
    
    
}

//Setting Functions
void Camera::SetCameraType(CameraType::Enum cam_mode) {
    camera_type = cam_mode;
    camera_up = glm::vec3(0, 1, 0);
    rotation_quaternion = glm::quat(1, 0, 0, 0);
}

void Camera::SetCameraModeFollow(Quadrotor& quad, glm::vec3 distance)
{
    this->quadrotor = &quad;
    this->distance = distance;
    cameraMode = CameraModes::FOLLOW_QUAD;
    
}

void Camera::SetCameraModeFollowUpright(Quadrotor& quad, glm::vec3 distance)
{
    this->quadrotor = &quad;
    this->distance = distance;
    cameraMode = CameraModes::FOLLOW_QUAD_UPRIGHT;
    
}

void Camera::SetCameraModeWorld()
{
    
    SetPosition(glm::vec3(0, 3000, 1100));
    SetLookAt(glm::vec3(0, 60, 1000));
    cameraMode = CameraModes::WORLD;
    
}

void Camera::SetCameraModeCircleMotion(glm::vec3 point, glm::vec3 position)
{
    SetLookAt(point);
    if(position.y != -1000.0f)
        SetPosition(position);
    
    
}


void Camera::SetPosition(glm::vec3 pos) {
    camera_position = pos;
}

void Camera::SetLookAt(glm::vec3 pos) {
    camera_look_at = pos;
}
void Camera::SetFOV(double fov) {
    field_of_view = fov;
}
void Camera::SetViewport(int loc_x, int loc_y, int width, int height) {
    viewport_x = loc_x;
    viewport_y = loc_y;
    window_width = width;
    window_height = height;
    //need to use doubles division here, it will not work otherwise and it is possible to get a zero aspect ratio with integer rounding
    aspect = double(width) / double(height);
    ;
}
void Camera::SetClipping(double near_clip_distance, double far_clip_distance) {
    near_clip = near_clip_distance;
    far_clip = far_clip_distance;
}

void Camera::Move(CameraDirection dir) {
    if (camera_type == CameraType::FREE) {
        switch (dir) {
            case UP:
                camera_position_delta += camera_up * camera_scale;
                break;
            case DOWN:
                camera_position_delta -= camera_up * camera_scale;
                break;
            case LEFT:
                camera_position_delta -= glm::cross(camera_direction, camera_up) * camera_scale;
                break;
            case RIGHT:
                camera_position_delta += glm::cross(camera_direction, camera_up) * camera_scale;
                break;
            case FORWARD:
                camera_position_delta += camera_direction * camera_scale;
                break;
            case BACK:
                camera_position_delta -= camera_direction * camera_scale;
                break;
        }
    }
}
void Camera::ChangePitch(float degrees) {
    //Check bounds with the max pitch rate so that we aren't moving too fast
    if (degrees < -max_pitch_rate) {
        degrees = -max_pitch_rate;
    } else if (degrees > max_pitch_rate) {
        degrees = max_pitch_rate;
    }
    camera_pitch += degrees;
    
    //Check bounds for the camera pitch
    if (camera_pitch > 360.0f) {
        camera_pitch -= 360.0f;
    } else if (camera_pitch < -360.0f) {
        camera_pitch += 360.0f;
    }
}
void Camera::ChangeHeading(float degrees) {
    //Check bounds with the max heading rate so that we aren't moving too fast
    if (degrees < -max_heading_rate) {
        degrees = -max_heading_rate;
    } else if (degrees > max_heading_rate) {
        degrees = max_heading_rate;
    }
    //This controls how the heading is changed if the camera is pointed straight up or down
    //The heading delta direction changes
    if ((camera_pitch > 90 && camera_pitch < 270) || (camera_pitch < -90 && camera_pitch > -270)) {
        camera_heading -= degrees;
    } else {
        camera_heading += degrees;
    }
    //Check bounds for the camera heading
    if (camera_heading > 360.0f) {
        camera_heading -= 360.0f;
    } else if (camera_heading < -360.0f) {
        camera_heading += 360.0f;
    }
}
void Camera::Move2D(int x, int y) {
    //compute the mouse delta from the previous mouse position
    glm::vec3 mouse_delta = mouse_position - glm::vec3(x, y, 0);
    //if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
    if (move_camera) {
        ChangeHeading(.008f * mouse_delta.x);
        ChangePitch(.008f * mouse_delta.y);
    }
    mouse_position = glm::vec3(x, y, 0);
}

void Camera::SetPos(int button, int state, int x, int y) {
    if (button == 3 && state == GLUT_DOWN) {
        camera_position_delta += camera_up * .05f;
    } else if (button == 4 && state == GLUT_DOWN) {
        camera_position_delta -= camera_up * .05f;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        move_camera = true;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        move_camera = false;
    }
    mouse_position = glm::vec3(x, y, 0);
}

CameraType::Enum Camera::GetMode() {
    return camera_type;
}

void Camera::GetViewport(int &loc_x, int &loc_y, int &width, int &height) {
    loc_x = viewport_x;
    loc_y = viewport_y;
    width = window_width;
    height = window_height;
}

void Camera::GetMatricies(glm::mat4 &P, glm::mat4 &V, glm::mat4 &M) {
    P = projection;
    V = view;
    M = model;
}


void Camera::executeTimelineCommands()
{
    if(readTimeline)
    {
        double current_time = QuadTimer::GetProcessTime();
        
        if(!isExecuting)
        {
            char command[250];
            char* commandstr;
            double peektime = -1;
            commandstr = timeline->readNextCommand(peektime);
            if(sscanf(commandstr,"%lf %s",&comTime,command)==EOF)
            {readTimeline = false; return;}
            //strcpy(delayedCommand,commandstr);
            
            
            sprintf(delayedCommand,"%lf %s",peektime,commandstr);
            printf(delayedCommand);
            //printf("\n%s",commandstr);
            isExecuting = true;
        }
        
        else if(current_time>=comTime && !isMoving)
        {
            
            sscanf(delayedCommand,"%lf %lf %s",&nextTime,&comTime,command);
            // printf("\n%s",command);
            //printf("\n%s",delayedCommand);
            if(!strcmp(command,"FOLLOW"))
            {
                char whichQuad[10];
                glm::vec3 position;
                sscanf(delayedCommand,"%lf %lf %s %s %f %f %f",&nextTime,&comTime,command, whichQuad, &position.x, &position.y, &position.z);
                if(!strcmp(whichQuad,"NEO"))
                    SetCameraModeFollow(*neoQuad,-position);
                else if(!strcmp(whichQuad,"DRO"))
                    SetCameraModeFollow(*dronedemort,-position);
                else if(!strcmp(whichQuad,"MAM"))
                    SetCameraModeFollow(*mamaQuad,-position);
                
                
            }
            else if(!strcmp(command,"FOLLOWVERTICAL"))
            {
                char whichQuad[10];
                glm::vec3 position;
                sscanf(delayedCommand,"%lf %lf %s %s %f %f %f",&nextTime,&comTime,command, whichQuad, &position.x, &position.y, &position.z);
                if(!strcmp(whichQuad,"NEO"))
                    SetCameraModeFollowUpright(*neoQuad,-position);
                else if(!strcmp(whichQuad,"DRO"))
                    SetCameraModeFollowUpright(*dronedemort,-position);
                else if(!strcmp(whichQuad,"MAM"))
                    SetCameraModeFollowUpright(*mamaQuad,-position);
                
            }
            else if(!strcmp(command,"WORLD"))
            {
                SetCameraModeWorld();  
            } 
            else if(!strcmp(command,"CIRCLEMOTION"))
            {
                glm::vec3 center;
                sscanf(delayedCommand,"%lf %lf %s %f %f %f %lf",&nextTime,&comTime,command, &center.x, &center.y, &center.z,&comAngle);
                SetCameraModeCircleMotion(center,camera_position);
                
            }
            else if(!strcmp(command,"LOOKAT"))
            {
                glm::vec3 center;
                sscanf(delayedCommand,"%lf %lf %s %f %f %f",&nextTime,&comTime,command, &center.x, &center.y, &center.z);
                SetLookAt(center);
                
            }
            else if(!strcmp(command,"POS"))
            {
                glm::vec3 center;
                sscanf(delayedCommand,"%lf %lf %s %f %f %f",&nextTime,&comTime,command, &center.x, &center.y, &center.z);
                SetPosition(center);
                
            }
            else if(!strcmp(command,"MOV"))
            {
                
                sscanf(delayedCommand,"%lf %lf %s %lf %f %f %f",&nextTime,&comTime, command,&comOrientationTime,&comVect.x,&comVect.y,&comVect.z); 
                isExecuting = true;
                cameraTime.getTimeDiffSec();
                comOrientationTime = current_time + comOrientationTime;
                /*
                 *                if(orientationMode == QuadOrientationMode::FREE)
                 *                {}
                 *                else if(orientationMode == QuadOrientationMode::ANOTHERQUAD)
                 *                { 
                 *                     
            }
            else if(orientationMode == QuadOrientationMode::UPRIGHT)
            {}
            //*/
            //Operations to align the quad with the direction of motion 
            
            comDirection = comVect-camera_position; // glm::vec3(comVect.x-camera_position.x,comVect.y-camera_position.y,comVect.z-camera_position.z);
            //  comDirection = glm::normalize(comDirection);
            
            
            }
            //isExecuting = false;
            isMoving = true;
        }
        else if(current_time<=comOrientationTime) //dont have to worry about modes other than MOV
        { 
            
            glm::vec3 cam_direction, current_up;
            double delta_time = comOrientationTime-current_time;
            
            cam_direction = glm::normalize( camera_look_at-camera_position);
            
            comRotationAxis = glm::cross(cam_direction,glm::normalize(comDirection));
            
            //if(comRotationAxis.length()>0)
            //    camera_up = -comRotationAxis;
            //else 
            
            
            //camera_up = glm::vec3(0,1,0);
            double timediff =   cameraTime.getTimeDiffSec();
            float angle_axis = glm::angle(cam_direction,glm::normalize(comDirection));
            //  printf("Angle: %f\n",angle_axis);
            
            angle_axis = angle_axis*(float) (timediff/ delta_time);
            if(glm::angle(glm::normalize(comRotationAxis),glm::vec3(0,1,0))>glm::half_pi<float>())
            {camera_up = -comRotationAxis; angle_axis = -angle_axis;}
            // angle_axis = .005;
            //ChangeHeading(abs(float(angle_axis*180/glm::pi<float>())));
            ChangeHeading(angle_axis); 
        }
        else if(current_time>=nextTime)
        { isExecuting = false;isMoving=false;}
        else if(current_time>=comTime)
        {
            
            if(!strcmp(command,"MOV"))
            {
                //SetLookAt(comVect);
                glm::vec3 distance = comVect-camera_position;
                SetLookAt(comVect+comDirection);
                double delta_time = nextTime-current_time;
                glm::vec3 dist2 = distance *(float) (cameraTime.getTimeDiffSec()/ delta_time);
                camera_position+=dist2;
            }
            //     printf("\ndelta pos:%f %f %f",dist2.x,dist2.y,dist2.z);
            
        }
        // else
        //   isExecuting = false;
    }
    
}

void Camera::loadQuadrotors(NeoQuad* neoQuad, Dronedemort* dronedemort, MamaQuad* mamaQuad)
{
    this->neoQuad = neoQuad;
    this->dronedemort = dronedemort;
    this->mamaQuad = mamaQuad;
    
}

