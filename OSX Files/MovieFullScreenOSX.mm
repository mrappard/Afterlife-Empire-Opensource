//
//  MovieFullScreenOSX.m
//  After Life Empire
//
//  Created by Autobotika on 2015-02-13.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#import "MovieFullScreen.h"
#import "OpenGLMovieLayer.h"
#import "OpenGLImports.h"

static GLuint _vertexArray;
static GLuint _vertexBuffer;



void initVideo(){
    
    NSString * thePath = @"OpenVideo.mp4";
    
    NSURL * theUrl = [[NSBundle mainBundle] URLForResource:[[thePath lastPathComponent] stringByDeletingPathExtension] withExtension:[thePath pathExtension]];

    [OpenGLMovieLayer setUpReader:theUrl];




}

static bool halfSpeed = false;

bool updateVideo(){
    
    halfSpeed=!halfSpeed;
    
    if (halfSpeed==false){
    
        return true;
    }
    
    
    
    
    if(
    [OpenGLMovieLayer grabNextFrame]
       ){
    
    
    
    
    GLfloat gBoxVertexData[] = {
        
        (GLfloat)(-100),
        (GLfloat)(56.25),
                0,  0,
        
        
        (GLfloat)(-100),
        (GLfloat)(-56.25),
              0,  1,
        
        (GLfloat)(100),
        (GLfloat)(56.25),
               1,  0,
        
        (GLfloat)(100),
        (GLfloat)(-56.25),
             1,  1
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
    
    
    
        return true;
    
    glBindVertexArrayCon(0);

    } else {
        return false;
    
    }
  
}
void renderVideo(){

    ;
    
    
    if (_vertexArray!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, [OpenGLMovieLayer grabCurrentFrame]);
        glBindVertexArrayCon(_vertexArray);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
    }

}

void closeVideo(){
    [OpenGLMovieLayer closeVideo];
}
