//
//  FrameBufferTemp.cpp
//  HelloSDL
//
//  Created by Autobotika on 12-07-19.
//  Copyright (c) 2012 Student. All rights reserved.
//

#include "FrameBuffer.h"



bool operator<(const theFrameBufferKey lhs, const theFrameBufferKey rhs){
    return (lhs.TEXTURE_WIDTH<rhs.TEXTURE_WIDTH)?
    true:(lhs.TEXTURE_HEIGHT<rhs.TEXTURE_HEIGHT);}

std::map<theFrameBufferKey,frameBufferStruct > FrameBufferTemp::bufferKeys;


FrameBufferTemp::FrameBufferTemp(){
    renderBuffer=-1;
    _vertexArray=0;
    _vertexBuffer=0;
    
}
FrameBufferTemp::~FrameBufferTemp(){
    if (renderBuffer!=-1){
        deleteFrameBuffer();
    }
}




void FrameBufferTemp::update(GLfloat width, GLfloat height){


    
    
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)-width,
            (GLfloat)-height,
            (GLfloat) 0,
            (GLfloat) 0,
            
            (GLfloat)-width,
            (GLfloat) height,
            (GLfloat) 0,
            (GLfloat) 1.0,
            
            (GLfloat) width,
            (GLfloat)-height,
            (GLfloat) 1.0,
            (GLfloat) 0,
            
            (GLfloat) width,
            (GLfloat) height,
            (GLfloat) 1.0,
            (GLfloat) 1.0,
        };
        
        if (!_vertexArray){
            glGenVertexArraysCon(1, &_vertexArray);
        }
        glBindVertexArrayCon(_vertexArray);
        
        if(!_vertexBuffer){
            glGenBuffers(1, &_vertexBuffer);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gBoxVertexData), gBoxVertexData, GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
        
        
        
        glBindVertexArrayCon(0);

}



void FrameBufferTemp::render(){
    
    if (_vertexArray!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindVertexArrayCon(_vertexArray);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
    }
    
    
    
}


bool FrameBufferTemp::initateFrameBuffer16(unsigned int WIDTH, unsigned int HEIGHT){
    return true;
    /*
    if (renderBuffer!=-1){return false;}
    TEXTURE_WIDTH=WIDTH;
    TEXTURE_HEIGHT =HEIGHT;
    
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_HALF_FLOAT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glGenFramebuffersEXT(1, &frameBuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
    
    // create a renderbuffer object to store depth info
    // NOTE: A depth renderable image should be attached the FBO for depth test.
    // If we don't attach a depth renderable image to the FBO, then
    // the rendering output will be corrupted because of missing depth test.
    // If you also need stencil test for your rendering, then you must
    // attach additional image to the stencil attachement point, too.
    glGenRenderbuffersEXT(1, &renderBuffer);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    
    // attach a texture to FBO color attachement point
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureId, 0);
    
    // attach a renderbuffer to depth attachment point
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderBuffer);
    
    //@ disable color buffer if you don't attach any color buffer image,
    //@ for example, rendering depth buffer only to a texture.
    //@ Otherwise, glCheckFramebufferStatusEXT will not be complete.
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);
    
    
    //bool test = checkFramebufferStatus();
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDisable(GL_TEXTURE_2D);
    glClearDepth(1.0f);
    
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREENWIDTH, SCREENHEIGHT,0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    
    
    
    
    return true;
    
    */
    
    
    
    
    
}



bool FrameBufferTemp::initateFrameBufferNonIndependent(unsigned int WIDTH, unsigned int HEIGHT){
    
    if (renderBuffer!=-1){return false;}
    theFrameBufferKey newKey;
    newKey.TEXTURE_HEIGHT=HEIGHT;
    newKey.TEXTURE_HEIGHT=WIDTH;
    if (bufferKeys.find(newKey)==bufferKeys.end()){
        
        TEXTURE_WIDTH=WIDTH;
        TEXTURE_HEIGHT =HEIGHT;
        
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
        
        
        
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glGenFramebuffersEXT(1, &frameBuffer);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
        
        // create a renderbuffer object to store depth info
        // NOTE: A depth renderable image should be attached the FBO for depth test.
        // If we don't attach a depth renderable image to the FBO, then
        // the rendering output will be corrupted because of missing depth test.
        // If you also need stencil test for your rendering, then you must
        // attach additional image to the stencil attachement point, too.
        glGenRenderbuffersEXT(1, &renderBuffer);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBuffer);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
        
        // attach a texture to FBO color attachement point
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureId, 0);
        
        // attach a renderbuffer to depth attachment point
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderBuffer);
        
        //@ disable color buffer if you don't attach any color buffer image,
        //@ for example, rendering depth buffer only to a texture.
        //@ Otherwise, glCheckFramebufferStatusEXT will not be complete.
        //glDrawBuffer(GL_NONE);
        //glReadBuffer(GL_NONE);
        
        
        //bool test = checkFramebufferStatus();
        
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDisable(GL_TEXTURE_2D);
        glClearDepth(1.0f);
        
        
        frameBufferStruct theStruct;
        
        theStruct.frameBuffer =frameBuffer;
        theStruct.renderBuffer=renderBuffer;
        theStruct.textureId =textureId;
        
        bufferKeys[newKey] = theStruct;
        
        
        
        
        return true;
        
        
        
        
        
        
        
    } else {
        
        frameBufferStruct theStruct = bufferKeys[newKey];
        
        frameBuffer=	theStruct.frameBuffer;
        renderBuffer= theStruct.renderBuffer;
        textureId = theStruct.textureId;
        
        TEXTURE_WIDTH=WIDTH;
        TEXTURE_HEIGHT =HEIGHT;
        
    }
    
    return true;
    
    
    
    
    
}


bool FrameBufferTemp::initateFrameBuffer(unsigned int WIDTH, unsigned int HEIGHT){
    if (renderBuffer!=-1){return false;}
    TEXTURE_WIDTH=WIDTH;
    TEXTURE_HEIGHT =HEIGHT;
    
 
    
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    

    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glGenFramebuffersEXT(1, &frameBuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
    
    // create a renderbuffer object to store depth info
    // NOTE: A depth renderable image should be attached the FBO for depth test.
    // If we don't attach a depth renderable image to the FBO, then
    // the rendering output will be corrupted because of missing depth test.
    // If you also need stencil test for your rendering, then you must
    // attach additional image to the stencil attachement point, too.
    glGenRenderbuffersEXT(1, &renderBuffer);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    
    // attach a texture to FBO color attachement point
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureId, 0);
    
    // attach a renderbuffer to depth attachment point
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderBuffer);
    
    //@ disable color buffer if you don't attach any color buffer image,
    //@ for example, rendering depth buffer only to a texture.
    //@ Otherwise, glCheckFramebufferStatusEXT will not be complete.
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);
    
    
    //bool test = checkFramebufferStatus();
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDisable(GL_TEXTURE_2D);
    glClearDepth(1.0f);
    
    
    return true;
    
    
    
    
    
    
    
}


void FrameBufferTemp::clearForBackground(){
    
    if (renderBuffer!=-1){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    
}


void FrameBufferTemp::enableFrameBufferNoClear(){
    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
    glDisable(GL_TEXTURE_2D);
    

    
}

void FrameBufferTemp::enableFrameBuffer(){
    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    
    
    
}
void FrameBufferTemp::disableFrameBuffer(){
    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind
    
    /*
    glClearColor(.0, .0, .0, 1);
    glClearDepth(1.0f);
    
    glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(0, SCREENWIDTH, SCREENHEIGHT, 0, 1, -1);
    
    glMatrixMode(GL_MODELVIEW);
    */
    
    
    
    
}

void FrameBufferTemp::drawFrameBufferAlpha(GLfloat alpha){
    
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glColor4f(1.0, 1.0, 1.0, alpha);
    
    
    /*
    
    glBegin( GL_QUADS);
    glTexCoord2d(0,1); glVertex3f(0, 0, 0);
    glTexCoord2d(1,1);glVertex3f(SCREENWIDTH, 0, 0);
    glTexCoord2d(1,0);glVertex3f(SCREENWIDTH, SCREENHEIGHT, 0);
    glTexCoord2d(0,0);glVertex3f(0, SCREENHEIGHT, 0);
    glEnd();
    
    */
    
    
    
    
}

void FrameBufferTemp::drawFrameBuffer(GLfloat X, GLfloat Y){
  /*  glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    
    
    switch (orthMode) {
        case NORMAL_ORTHO:
            
            glBegin( GL_QUADS);
            glTexCoord2d(0,1); glVertex3f(0+X, 0+Y, 0);
            glTexCoord2d(1,1);glVertex3f(SCREENWIDTH+X, 0+Y, 0);
            glTexCoord2d(1,0);glVertex3f(SCREENWIDTH+X, SCREENHEIGHT+Y, 0);
            glTexCoord2d(0,0);glVertex3f(0+X, SCREENHEIGHT+Y, 0);
            glEnd();
            break;
        case CENTER_CUT:
            glBegin( GL_QUADS);
            
            
            
            glTexCoord2d(0,1); glVertex3f(-(SCREENWIDTH/4)+X,
                                          -(SCREENHEIGHT/4)+Y,
                                          0);
            glTexCoord2d(1,1);glVertex3f(SCREENWIDTH+(SCREENWIDTH/4)+X,
                                         -(SCREENHEIGHT/4)+Y
                                         , 0);
            glTexCoord2d(1,0);glVertex3f(SCREENWIDTH+(SCREENWIDTH/4)+X,
                                         SCREENHEIGHT+(SCREENHEIGHT/4)+Y
                                         , 0);
            glTexCoord2d(0,0);glVertex3f(-(SCREENWIDTH/4)+X,
                                         SCREENHEIGHT+(SCREENHEIGHT/4)+Y
                                         , 0);
            glEnd();
            
            break;
            
            
            
        default:
            glBegin( GL_QUADS);
            glTexCoord2d(0,1); glVertex3f(0+X, 0+Y, 0);
            glTexCoord2d(1,1);glVertex3f(SCREENWIDTH+X, 0+Y, 0);
            glTexCoord2d(1,0);glVertex3f(SCREENWIDTH+X, SCREENHEIGHT+Y, 0);
            glTexCoord2d(0,0);glVertex3f(0+X, SCREENHEIGHT+Y, 0);
            glEnd();
            
            
            break;
    }
    
    
    
    glEnd();
   */
    
}






void FrameBufferTemp::drawFrameBuffer(){
    
    
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    
    /*
    switch (orthMode) {
        case NORMAL_ORTHO:
            
            glBegin( GL_QUADS);
            glTexCoord2d(0,1); glVertex3f(0, 0, 0);
            glTexCoord2d(1,1);glVertex3f(SCREENWIDTH, 0, 0);
            glTexCoord2d(1,0);glVertex3f(SCREENWIDTH, SCREENHEIGHT, 0);
            glTexCoord2d(0,0);glVertex3f(0, SCREENHEIGHT, 0);
            glEnd();
            break;
        case CENTER_CUT:
            glBegin( GL_QUADS);
            
            
            
            glTexCoord2d(0,1); glVertex3f(-(SCREENWIDTH/4),
                                          -(SCREENHEIGHT/4), 0);
            glTexCoord2d(1,1);glVertex3f(SCREENWIDTH+(SCREENWIDTH/4),
                                         -(SCREENHEIGHT/4), 0);
            glTexCoord2d(1,0);glVertex3f(SCREENWIDTH+(SCREENWIDTH/4),
                                         SCREENHEIGHT+(SCREENHEIGHT/4), 0);
            glTexCoord2d(0,0);glVertex3f(-(SCREENWIDTH/4),
                                         SCREENHEIGHT+(SCREENHEIGHT/4), 0);
            glEnd();
            
            break;
            
            
            
        default:
            glBegin( GL_QUADS);
            glTexCoord2d(0,1); glVertex3f(0, 0, 0);
            glTexCoord2d(1,1);glVertex3f(SCREENWIDTH, 0, 0);
            glTexCoord2d(1,0);glVertex3f(SCREENWIDTH, SCREENHEIGHT, 0);
            glTexCoord2d(0,0);glVertex3f(0, SCREENHEIGHT, 0);
            glEnd();
            
            
            break;
    }
    
    
    
    glEnd();
    */
}


void FrameBufferTemp::deleteFrameBuffer(){
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDeleteTextures( 1, &textureId );
    glDisable(GL_TEXTURE_2D);
    
    glDeleteFramebuffersEXT(1, &frameBuffer);
    glDeleteRenderbuffersEXT(1, &renderBuffer);
    renderBuffer=frameBuffer=textureId=0;
    
    
}


std::vector<char> FrameBufferTemp::save(){
    
    
    //GLbyte * data = new GLbyte(TEXTURE_WIDTH*TEXTURE_HEIGHT*4);
    //glReadPixels(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    std::vector<char> info;//
    info.assign(TEXTURE_WIDTH*TEXTURE_HEIGHT*4,0 );
    glReadPixels(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, &info[0]);
    

    
    return info;
}


FrameBufferTemp FrameBufferMask::theBuffer;

void FrameBufferMask::enableBuffer(){


    theBuffer.initateFrameBuffer(1024, 1024);
    //theBuffer.enableFrameBuffer();
    theBuffer.enableFrameBufferNoClear();
    
    


}

void FrameBufferMask::disableBuffer(){

    theBuffer.disableFrameBuffer();

}

void FrameBufferMask::clearBuffer(){

    theBuffer.clearForBackground();

}


void FrameBufferMask::renderBuffer(){

    theBuffer.render();
    
    


}

void FrameBufferMask::updateBuffer(GLfloat width, GLfloat height){

    theBuffer.update(width, height);


}

