//
//  ShaderManager.h
//  After Life Empire
//
//  Created by Autobotika on 2014-09-30.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__ShaderManager__
#define __After_Life_Empire__ShaderManager__

#include <stdio.h>
#include "OpenGLImports.h"
#include <string>


enum SelectedShader {
    
    

    
    
    SHADOW_N,
    TEXTURE_N,
    TEXTURE_UI,
    TEXTURE_FULL_MAP,
    TEXTURE_MAKE_SMALLER,

    SELECT_SHADE,
    SELECT_SHADE_UI,

    ENABLE_HIGHLIGHT_SHADER_N,


    ENABLE_UI,
    ENABLE_UI_M,

    
    HALFTEXT_N,
    HALFTEXT_UI,
    
    HIGHLIGHT_N,
    HIGHLIGHT_UI,
    
   
    TEXTURE_SOLID_COLOR_N,
    TEXTURE_SOLID_COLOR_UI,

    TEXTURE_ALPHA_N,
    TEXTURE_ALPHA_UI,
    DESELECT_N,
    DESELECT_UI,
    REVERSE_ALPHA,
    COLOR_SHIFT,
    NOSHADER
    
};

class Shader
{
public:
    
    Shader();
    ~Shader();
    
    std::string vertexPath;
    std::string fragPath;
    GLuint _program;
    GLboolean compiled;
    int theTypeOfShader;
    bool load();
    bool compileShader(GLuint * shader, GLenum type, std::string * file);
    bool linkProgram(GLuint prog);
    bool setMatrix(float * projectionMatrix, float * modelMatrix);
    void enable();
    void setVarOne(float value);
    void setVarOne(float * value);
    void setVarOne4F(float * value);
    void setVarTwo4f(float * value);
    GLint _projectionUniform;
    GLint  _modelViewUniform;
    
    GLint _varOne;
    GLint _varTwo;
    GLint _varOneType;
    
    
};

class ShaderManager
{
public:
    
    static SelectedShader currentSelect;
    
    static Shader textureShaderShadow;
    static void enableTextureShaderShadow(GLfloat alpha);
    
    
    static Shader textureShader;
    static void enableTextureShader();
    static void enableTextureShaderNoMove();
    static void enableTextureShaderFullMap();
    static void enableTextureShaderMakeSmaller();
    
    
    static Shader selectShader;
    static void enableSelectShader(bool redOrGreen);
    static void enableSelectShaderUI(bool redOrGreen);
    
    static Shader highLightColorShader;
    static void enableHighLightShaderColor(float value, float * color);
    
    static Shader UIShader;
    static void enableUIShader();
    static void enableUIShaderMove(GLfloat X, GLfloat Y);
    
    static Shader halfTextureShader;
    static void enableHalfTextureShader();
    static void enableHalfTextureShaderUI();
    
    
    static Shader highLightShader;
    static void enableHighLightShader(float value);
    static void enableHighLightShaderUI(float value);
    
    static Shader solidColor;
    static void enableSolidColor(float value[4]);
    static void enableSolidColorUI(float value[4]);

    
    
    static Shader alphaTexture;
    static void enableTextureAlpha(GLfloat alpha);
    static void enableTextureAlphaUI(GLfloat alpha);
    
    
    static Shader deselectTextureShader;
    static void  deselectedShader();
    static void  deselectShaderUI();
    
    static Shader reverseAlphaShader;
    static void enableReverseAlphaShader();

    
    static Shader colorShift;
    static void enableColorShift(GLfloat * colorValue);
    static void enableColorShiftUI(GLfloat * colorValue);
    
    static GLfloat colorTest[4];
    
    
};




#endif /* defined(__After_Life_Empire__ShaderManager__) */
