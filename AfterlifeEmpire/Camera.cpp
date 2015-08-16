//
//  Camera.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-09-30.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//
#include "OpenGLImports.h"
#include "Camera.h"
#include <algorithm>
#include "GlobalDebugger.h"
#include "GameLogic.h"
#include "Tiles.h"
#include "Player.h"
#include "SaveMap.h"

float Camera::projectionMatrixS[16];
float Camera::modelMatrixS[16];


static GLfloat rightWindow;
static GLfloat topWindow;


double Camera::zoom=1;


void Camera::setWindow(float l, float r, float b, float t, float n, float f){
    
    l=l*zoom;
    r=r*zoom;
    b=b*zoom;
    t=t*zoom;
    
    
    std::fill(std::begin(projectionMatrixS), std::end(projectionMatrixS), 0);
   
    projectionMatrixS[0+0]  = 2 / (r - l);

    projectionMatrixS[4+1]  = 2 / (t - b);
  
    projectionMatrixS[8+2]  = -1 / (f - n);

    projectionMatrixS[12+0] = -(r + l) / (r - l);
    projectionMatrixS[12+1] = -(t + b) / (t - b);
    projectionMatrixS[12+2] = -n / (f - n);
    projectionMatrixS[12+3] = 1;
    
    
    glViewport(0, 0, r*2.0, t*2.0);
    rightWindow=r*2.0;
    topWindow=t*2.0;
    
    #ifdef __IPHONE_7_0
    
    #else
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(l, r, b,t, 1, -1);
    #endif
}




double Camera::posX =0;
double Camera::posY =0;



void Camera::setWindow(float width ,float height ,float l, float r, float b, float t, float n, float f){
    
    l=l*zoom;
    r=r*zoom;
    b=b*zoom;
    t=t*zoom;
    
    
    std::fill(std::begin(projectionMatrixS), std::end(projectionMatrixS), 0);

    projectionMatrixS[0+0]  = 2 / (r - l);

    projectionMatrixS[4+1]  = 2 / (t - b);
    
    projectionMatrixS[8+2]  = -1 / (f - n);
    
    projectionMatrixS[12+0] = -(r + l) / (r - l);
    projectionMatrixS[12+1] = -(t + b) / (t - b);
    projectionMatrixS[12+2] = -n / (f - n);
    projectionMatrixS[12+3] = 1;
    
    rightWindow = width;
    topWindow = height;
    
#ifdef __IPHONE_7_0
    
#else
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	
    glOrtho(l, r, b,t, 100, -100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
     #endif

}




void Camera::setModelMatrixZoom(float x, float y, float r){

    
    float l=-GameLogic::width*zoom;
    r=GameLogic::width*zoom;
    float b=-GameLogic::height*zoom;
    float t=GameLogic::height*zoom;
    float n = 1;
    float f = -1;
    
    
    std::fill(std::begin(projectionMatrixS), std::end(projectionMatrixS), 0);
    
    projectionMatrixS[0+0]  = 2 / (r - l);
    
    projectionMatrixS[4+1]  = 2 / (t - b);
    
    projectionMatrixS[8+2]  = -1 / (f - n);
    
    projectionMatrixS[12+0] = -(r + l) / (r - l);
    projectionMatrixS[12+1] = -(t + b) / (t - b);
    projectionMatrixS[12+2] = -n / (f - n);
    projectionMatrixS[12+3] = 1;
    
    std::fill(std::begin(modelMatrixS), std::end(modelMatrixS), 0);
    modelMatrixS[0]     = 1;
    modelMatrixS[5]     = 1;
    modelMatrixS[10]    = 1;
    modelMatrixS[15]    = 1;
    
    
    modelMatrixS[12]    = x;
    modelMatrixS[13]    = y;


}


void Camera::changeZoom(GLfloat newValue){


    if (newValue>0){
        zoom +=0.025;
        if (zoom>2){
            zoom = 2;
        }
    
    } else if (newValue<0){
        zoom -=0.025;
        if (zoom<0.75){
            zoom = 0.75;
        }
        
    }

}

void Camera::increaseZoom(){

    if (zoom<2.0){
        zoom +=0.1;
    
    } else {
        zoom = 2;
    }

}

    
void Camera::decreaseZoom(){
    
    zoom -= 0.1;

    if (zoom<0.75){
    
        
        zoom = 0.75;

    
    }
    
    
}

void Camera::neutralZoom(){

    zoom = 1.0;

}




bool Camera::isEntireRectVisible(int left, int top, int right, int down){

    
        int posX1 = left;
        int posX2 = right;
        int posY1 = down*Tiles::width;
        int posY2 = top*Tiles::width;
    
        return (isPointVisible(posX1+posY1)||isPointVisible(posX1+posY2)||isPointVisible(posX2+posY1)||isPointVisible(posX2+posY2));
}


bool Camera::isBoundBoxVisible(double l, double b, double r, double t){

  
    cpBB theTarget = cpBBNew(l, b, r, t);

    cpBB screen = cpBBNew(-GameLogic::width*zoom-posX, -GameLogic::height*zoom-posY, GameLogic::width*zoom-posX, GameLogic::height*zoom-posY);

    return cpBBIntersects(screen, theTarget);


}




bool Camera::isPointVisible(int index){
    
    
    cpVect returnValue =  SaveMap::findCords(index);

    
    return isPointVisible(returnValue.x, returnValue.y);
    
}



bool Camera::isPointVisibleExact(double xPos, double yPos){

    if (-posX+GameLogic::width*(zoom)>xPos&&-posX-GameLogic::width*(zoom)<xPos){
        
        if (-posY+GameLogic::height*(zoom)>yPos&&-posY-GameLogic::width*(zoom)<yPos){
            
            return true;
        }
        
    }
    
    return false;
    
    
}

bool Camera::isPointVisible(double xPos, double yPos){
    
    if (-posX+GameLogic::width*(zoom*1.10)>xPos&&-posX-GameLogic::width*(zoom*1.10)<xPos){
    
        if (-posY+GameLogic::height*(zoom*1.10)>yPos&&-posY-GameLogic::width*(zoom*1.10)<yPos){
        
            return true;
        }
    
    }
    
    return false;
    
}



void Camera::setModelMatrix(float x, float y, float r){
    float l=-GameLogic::width;
    r=GameLogic::width;
    float b=-GameLogic::height;
    float t=GameLogic::height;
    float n = 1;
    float f = -1;
    
    
    std::fill(std::begin(projectionMatrixS), std::end(projectionMatrixS), 0);
    
    projectionMatrixS[0+0]  = 2 / (r - l);
    
    projectionMatrixS[4+1]  = 2 / (t - b);
    
    projectionMatrixS[8+2]  = -1 / (f - n);
    
    projectionMatrixS[12+0] = -(r + l) / (r - l);
    projectionMatrixS[12+1] = -(t + b) / (t - b);
    projectionMatrixS[12+2] = -n / (f - n);
    projectionMatrixS[12+3] = 1;
    
    std::fill(std::begin(modelMatrixS), std::end(modelMatrixS), 0);
    modelMatrixS[0]     = 1;
    modelMatrixS[5]     = 1;
    modelMatrixS[10]    = 1;
    modelMatrixS[15]    = 1;
    
    
    modelMatrixS[12]    = x;
    modelMatrixS[13]    = y;





}


static float projectionMatrixSB[16];
static float modelMatrixSB[16];

void Camera::grabEntireMap(){
    
    for (int counter = 0; counter<16;counter++){
        projectionMatrixSB[counter]=projectionMatrixS[counter];
        modelMatrixSB[counter]=modelMatrixS[counter];
    }
    
    
    GLfloat r,l,t,b,f,n;

    l = -185*10;
    r = 72*10;
    t = 92*10;
    b = -38*10;
    f = -1.0;
    n = 1.0;
    
    
    
    
    
    
    projectionMatrixS[0+0]  = 2 / (r - l);

    projectionMatrixS[4+1]  = 2 / (t - b);
    
    projectionMatrixS[8+2]  = -1 / (f - n);
    
    projectionMatrixS[12+0] = -(r + l) / (r - l);
    projectionMatrixS[12+1] = -(t + b) / (t - b);
    projectionMatrixS[12+2] = -n / (f - n);
    projectionMatrixS[12+3] = 1;
    
    
    modelMatrixS[0] = 1;
    modelMatrixS[5] = 1;
    modelMatrixS[10] = 1;
    modelMatrixS[15] = 1;
    
    
    modelMatrixS[12] = 0;
    modelMatrixS[13] = 0;
    
    


}

void Camera::OneVOne(){
    
    GLfloat r,l,t,b,f,n;
    
    
    l = -100;
    r = 100;
    t = 100;
    b = -100;
    f = -1.0;
    n = 1.0;
    
    
    
    
    
    
    projectionMatrixS[0+0]  = 2 / (r - l);


    projectionMatrixS[4+1]  = 2 / (t - b);

    projectionMatrixS[8+2]  = -1 / (f - n);
    
    projectionMatrixS[12+0] = -(r + l) / (r - l);
    projectionMatrixS[12+1] = -(t + b) / (t - b);
    projectionMatrixS[12+2] = -n / (f - n);
    projectionMatrixS[12+3] = 1;
    
    
    modelMatrixS[0] = 1;
    modelMatrixS[5] = 1;
    modelMatrixS[10] = 1;
    modelMatrixS[15] = 1;
    
    
    modelMatrixS[12] = 0;
    modelMatrixS[13] = 0;

}


void Camera::returnToNormal(){
    /*
    for (int counter = 0; counter<16;counter++){
        projectionMatrixS[counter]=projectionMatrixSB[counter];
        modelMatrixS[counter]=modelMatrixSB[counter];
    }
     */
    
    glViewport(0, 0, rightWindow, topWindow);
}





