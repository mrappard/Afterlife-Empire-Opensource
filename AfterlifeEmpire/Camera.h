//
//  Camera.h
//  After Life Empire
//
//  Created by Autobotika on 2014-09-30.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__Camera__
#define __After_Life_Empire__Camera__

#include <stdio.h>
#include <vector>
#include "openglimports.h"

class Camera
{
public:
    
    static float projectionMatrixS[16];
    static float modelMatrixS[16];
    
    static void setWindow(float l, float r, float b, float t, float n, float f);
    static void setWindow(float width ,float height ,float l, float r, float b, float t, float n, float f);
    static void setModelMatrix(float x, float y, float r);
    static void setModelMatrixZoom(float x, float y, float r);
    
    static double posX;
    static double posY;
    static double zoom;
    
    static void increaseZoom();
    static void decreaseZoom();
    static void neutralZoom();
    static void changeZoom(GLfloat newValue);
    
    static void grabEntireMap();
    static void returnToNormal();
    
    static void OneVOne();
    
    static bool isPointVisible(int index);
    static bool isPointVisible(double xPos, double yPos);
    static bool isPointVisibleExact(double xPos, double yPos);
    static bool isBoundBoxVisible(double l, double b, double r, double t);
    static bool isEntireRectVisible(int left, int top, int right, int down);
    
  
    

};



#endif /* defined(__After_Life_Empire__Camera__) */
