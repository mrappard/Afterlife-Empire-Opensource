//
//  Tiles.h
//  After Life Empire
//
//  Created by Autobotika on 2014-09-30.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__Circle__
#define __After_Life_Empire__Circle__

#include <stdio.h>
#include <vector>
#include "OpenGLImports.h"
#include "VecAndBB.h"
#include <string>
#include <map>



class Circle
{
public:
    Circle(cpVect _center, GLfloat _radius, GLfloat * _color);
    Circle();
    ~Circle();
    
    static std::vector<Circle*> theCircles;
    static std::map<std::string, Circle*> getByName;
    
    static void addCircle(cpVect center, GLfloat radius, GLfloat * _color);
    static void addCircle(cpVect center, GLfloat radius, GLfloat * _color, std::string key);
    static void setCirclePosition(cpVect center, std::string key);
    static void releaseCircle(std::string key);
    static void updateAll();
    static void renderAll();
    static void tickAll();
    
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    GLuint _vertexSize;
    
    cpVect  center;
    GLfloat radius;
    GLfloat tween;
    bool inOut;
    bool hold;
    GLfloat color[4];
    
    
    std::vector<GLfloat> createCircle();
    std::string theName;
    
    
    
    void update();
    void render();
    

    
    


 };



#endif /* defined(__After_Life_Empire__Tiles__) */
