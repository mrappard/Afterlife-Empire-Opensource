//
//  OpenGLImports.h
//  ThanatosIpad
//
//  Created by Autobotika on 13-09-22.
//  Copyright (c) 2013 Autobotika. All rights reserved.
//

#ifndef ThanatosIpad_OpenGLImports_h
#define ThanatosIpad_OpenGLImports_h



#if defined(_WIN32)
#include "glew.h"
#include <gl/gl.h>
#include <gl/glu.h>

#define glGenVertexArraysCon glGenVertexArrays
#define glBindVertexArrayCon glBindVertexArray
#define glDeleteVertexArraysCon glDeleteVertexArrays


#else

/*

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>



#define glGenVertexArraysCon  glGenVertexArraysOES
#define glBindVertexArrayCon  glBindVertexArrayOES
#define glDeleteVertexArraysCon glDeleteVertexArraysOES


*/


#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#define glGenVertexArraysCon glGenVertexArraysAPPLE
#define glBindVertexArrayCon glBindVertexArrayAPPLE
#define glDeleteVertexArraysCon glDeleteVertexArraysAPPLE

#endif



#define BUFFER_OFFSET(i) ((char *)NULL + (i))

typedef enum {
    GLKVertexAttribPosition,
    GLKVertexAttribNormal,
    GLKVertexAttribColor,
    GLKVertexAttribTexCoord0,
    GLKVertexAttribTexCoord1,
} GLKVertexAttrib;

/*

#if defined(_WIN32)
#include "glew.h"
#include <gl/gl.h>
#include <gl/glu.h>

#define glGenVertexArraysCon glGenVertexArrays
#define glBindVertexArrayCon glBindVertexArray
#define glDeleteVertexArraysCon glDeleteVertexArrays

#endif

#if defined(TARGET_OS_MAC )

    #if defined(TARGET_OS_IPHONE)

    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>

 

    #define glGenVertexArraysCon  glGenVertexArraysOES
    #define glBindVertexArrayCon  glBindVertexArrayOES
    #define glDeleteVertexArraysCon glDeleteVertexArraysOES

    #else

    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>

    #define glGenVertexArraysCon glGenVertexArraysAPPLE
    #define glBindVertexArrayCon glBindVertexArrayAPPLE
    #define glDeleteVertexArraysCon glDeleteVertexArraysAPPLE
    #endif

#endif



*/




inline GLenum returnBlendTypeDebug(int value){
    
    switch (value) {
        case 0:
            return GL_ZERO;
            break;
        case 1:
            return GL_ONE;
            break;
        case 2:
            return GL_SRC_COLOR;
            break;
        case 3:
            return GL_ONE_MINUS_SRC_COLOR;
            break;
        case 4:
            return GL_SRC_ALPHA;
            break;
        case 5:
            return GL_ONE_MINUS_SRC_COLOR;
            break;
        case 6:
            return GL_DST_ALPHA;
            break;
        case 7:
            return GL_ONE_MINUS_DST_ALPHA;
            break;
            
        default:
            return GL_ZERO;
            break;
    }
}








#endif
