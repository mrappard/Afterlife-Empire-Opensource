//
//  FrameBuffer.h
//  After Life Empire
//
//  Created by Autobotika on 2014-09-29.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__FrameBuffer__
#define __After_Life_Empire__FrameBuffer__

#include <stdio.h>

//
//  FrameBufferTemp.h
//  HelloSDL
//
//  Created by Autobotika on 12-07-19.
//  Copyright (c) 2012 Student. All rights reserved.
//


#include "OpenGLImports.h"
#include <map>
#include <vector>
#include <string>

struct frameBufferStruct{
    
    GLuint frameBuffer;
    GLuint textureId;
    GLuint renderBuffer;
};

struct theFrameBufferKey{
    unsigned int TEXTURE_WIDTH;
    unsigned int TEXTURE_HEIGHT;
};




#define NORMAL_ORTHO 1
#define CENTER_CUT 2


class FrameBufferTemp
{
public:
    
    
    static std::map<theFrameBufferKey,frameBufferStruct > bufferKeys;
    GLuint frameBuffer;
    GLuint textureId;
    GLuint renderBuffer;
    unsigned int TEXTURE_WIDTH;
    unsigned int TEXTURE_HEIGHT;
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    
   
    void update(GLfloat width, GLfloat height);
    void render();
    
    FrameBufferTemp();
    ~FrameBufferTemp();
    bool initateFrameBufferNonIndependent(unsigned int WIDTH, unsigned int HEIGHT);
    bool initateFrameBuffer16(unsigned int WIDTH, unsigned int HEIGHT);
    bool initateFrameBuffer(unsigned int WIDTH, unsigned int HEIGHT);
    void clearForBackground();
    void enableFrameBuffer();
    void enableFrameBufferNoClear();
    void disableFrameBuffer();
    void drawFrameBuffer();
    void drawFrameBufferAlpha(GLfloat alpha);
    void deleteFrameBuffer();
    void drawFrameBuffer(GLfloat X, GLfloat Y);
    
    std::vector<char> save();
};





class FrameBuffer
{
public:
    std::map<std::string, FrameBufferTemp> frameBufferMap;
    
    static void enableBuffer(std::string name, int widthX, int heightY);
    
    
};


class FrameBufferMask
{
public:
    static FrameBufferTemp theBuffer;
    static void enableBuffer();
    static void disableBuffer();
    static void clearBuffer();
    static void renderBuffer();
    static void updateBuffer(GLfloat width, GLfloat height);
    
};





























#endif /* defined(__After_Life_Empire__FrameBuffer__) */
