//
//  ShaderManager.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-09-30.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#include "ShaderManager.h"
#include "FileManager.h"
#include <stdlib.h>
#include "Camera.h"
#include "Window_OSX_Linux_fix.h"

#define doubleValue 1


Shader ShaderManager::textureShader;
Shader ShaderManager::selectShader;
Shader ShaderManager::UIShader;
Shader ShaderManager::highLightShader;
Shader ShaderManager::halfTextureShader;
Shader ShaderManager::deselectTextureShader;
Shader ShaderManager::solidColor;
Shader ShaderManager::reverseAlphaShader;
Shader ShaderManager::alphaTexture;
Shader ShaderManager::highLightColorShader;
Shader ShaderManager::colorShift;

SelectedShader ShaderManager::currentSelect=NOSHADER;
GLfloat ShaderManager::colorTest[4]={1.0,1.0,1.0,1.0};

void ShaderManager::enableColorShiftUI(GLfloat * colorValue){

    
    if (currentSelect==COLOR_SHIFT){
        
        if (colorTest[0]==colorValue[0]&&
            colorTest[1]==colorValue[1]&&
            colorTest[2]==colorValue[2]&&
            colorTest[3]==colorValue[3]){
            return;
            
        }
    }
    currentSelect=COLOR_SHIFT;
    colorTest[0]=colorValue[0];
    colorTest[1]=colorValue[1];
    colorTest[2]=colorValue[2];
    colorTest[3]=colorValue[3];
    
    if (!colorShift.compiled){
        
        
        colorShift.vertexPath="NormalFastColorShift.vsh";
        colorShift.fragPath="NormalFastColorShift.fsh";
        
        
        
        colorShift.theTypeOfShader= 12;
        colorShift.load();
        
    }
    Camera::setModelMatrix(0, 0, 0);
    colorShift.enable();
    colorShift.setVarOne4F(colorTest);
    colorShift.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);



}

void ShaderManager::enableColorShift(GLfloat * colorValue){

    if (currentSelect==COLOR_SHIFT){
        
        if (colorTest[0]==colorTest[0]&&
            colorTest[1]==colorTest[1]&&
            colorTest[2]==colorTest[2]&&
            colorTest[3]==colorTest[3]){
            return;
            
        }
    }
    currentSelect=COLOR_SHIFT;
    colorTest[0]=colorValue[0];
    colorTest[1]=colorValue[1];
    colorTest[2]=colorValue[2];
    colorTest[3]=colorValue[3];
    
    if (!colorShift.compiled){
        
 
        colorShift.vertexPath="NormalFastColorShift.vsh";
        colorShift.fragPath="NormalFastColorShift.fsh";
        
  
        
        colorShift.theTypeOfShader= 12;
        colorShift.load();
        
    }
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    colorShift.enable();
    colorShift.setVarOne4F(colorTest);
    colorShift.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    
    


    
    
}






void ShaderManager::enableReverseAlphaShader(){
    
    
    if (currentSelect==REVERSE_ALPHA){
        return;
    }
    
    currentSelect=REVERSE_ALPHA;

    if (!reverseAlphaShader.compiled){
        reverseAlphaShader.vertexPath="NormalFastReverseAlpha.vsh";
        reverseAlphaShader.fragPath="NormalFastReverseAlpha.fsh";
        reverseAlphaShader.theTypeOfShader= 3;
        reverseAlphaShader.load();
        
    }
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    reverseAlphaShader.enable();
    reverseAlphaShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    
    




}

void ShaderManager::deselectShaderUI(){
    
    if (currentSelect==DESELECT_UI){
        return;
    }
    
    currentSelect=DESELECT_UI;
    

    if (!deselectTextureShader.compiled){
        deselectTextureShader.vertexPath="deselectTextureShader.vsh";
        deselectTextureShader.fragPath="deselectTextureShader.fsh";
        deselectTextureShader.theTypeOfShader= 3;
        deselectTextureShader.load();
        
    }
    Camera::setModelMatrix(0, 0, 0);
    deselectTextureShader.enable();
    deselectTextureShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);

}

void ShaderManager::deselectedShader(){
    
    
    if (currentSelect==DESELECT_N){
        return;
    }
    
    currentSelect=DESELECT_N;
    
    if (!deselectTextureShader.compiled){
        deselectTextureShader.vertexPath="deselectTextureShader.vsh";
        deselectTextureShader.fragPath="deselectTextureShader.fsh";
        deselectTextureShader.theTypeOfShader= 3;
        deselectTextureShader.load();
        
    }
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    deselectTextureShader.enable();
    deselectTextureShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    
    
}

void ShaderManager::enableTextureAlpha(GLfloat alpha){

    if (currentSelect==TEXTURE_ALPHA_N){
        return;
    }
    
    currentSelect=TEXTURE_ALPHA_N;
    
    
    if (!alphaTexture.compiled){
        alphaTexture.vertexPath="NormalFastAlpha.vsh";
        alphaTexture.fragPath="NormalFastAlpha.fsh";
        alphaTexture.theTypeOfShader= 8;
        alphaTexture.load();
        
    }
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    
    alphaTexture.enable();
    alphaTexture.setVarOne(alpha);
    alphaTexture.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);


}

void ShaderManager::enableTextureAlphaUI(GLfloat alpha){
    
    if (currentSelect==TEXTURE_ALPHA_UI){
        return;
    }
    
    currentSelect=TEXTURE_ALPHA_UI;
    
    if (!alphaTexture.compiled){
        alphaTexture.vertexPath="NormalFastAlpha.vsh";
        alphaTexture.fragPath="NormalFastAlpha.fsh";
        alphaTexture.theTypeOfShader= 8;
        alphaTexture.load();
        
    }
    Camera::setModelMatrix(0, 0, 0);
    alphaTexture.enable();
    alphaTexture.setVarOne(alpha);
    alphaTexture.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);



}

void ShaderManager::enableHalfTextureShader(){

    if (currentSelect==HALFTEXT_N){
        return;
    }
    
    currentSelect=HALFTEXT_N;
    
    
    if (!halfTextureShader.compiled){
        halfTextureShader.vertexPath="NormalFastHalfAlpha.vsh";
        halfTextureShader.fragPath="NormalFastHalfAlpha.fsh";
        halfTextureShader.theTypeOfShader= 3;
        halfTextureShader.load();
        
    }
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    halfTextureShader.enable();
    halfTextureShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    

}

void ShaderManager::enableHalfTextureShaderUI(){
    
    if (currentSelect==HALFTEXT_UI){
        return;
    }
    
    currentSelect=HALFTEXT_UI;

    if (!halfTextureShader.compiled){
        halfTextureShader.vertexPath="NormalFastHalfAlpha.vsh";
        halfTextureShader.fragPath="NormalFastHalfAlpha.fsh";
        halfTextureShader.theTypeOfShader= 3;
        halfTextureShader.load();
        
    }
    Camera::setModelMatrix(0, 0, 0);
    halfTextureShader.enable();
    halfTextureShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    
    
}

void ShaderManager::enableHighLightShaderColor(float value, float * color){

    if (currentSelect==HIGHLIGHT_N){
        return;
    }
    
    currentSelect=HIGHLIGHT_N;
    
    
    if (!highLightColorShader.compiled){
        highLightColorShader.vertexPath = "HighLightShader.vsh";
        highLightColorShader.fragPath = "HighLightShader_Color.fsh";
        highLightColorShader.theTypeOfShader = 11;
        highLightColorShader.load();
    }

    
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    highLightColorShader.enable();
    
    float array[] = {value,value};
    highLightColorShader.setVarOne(array);
    highLightColorShader.setVarTwo4f(color);
    highLightColorShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    
    

    

}

void ShaderManager::enableSelectShader(bool redOrGreen){

   /* if (currentSelect==SELECT_SHADE){
        return;
    }
    */
    currentSelect=SELECT_SHADE;
    
    
    if (!selectShader.compiled){
        selectShader.vertexPath="Select.vsh";
        selectShader.fragPath="Select.fsh";
        selectShader.theTypeOfShader= 4;
        selectShader.load();
        
        
    }
    
    GLfloat value = 0;
    
    if (redOrGreen){
        value = 0.0;
    } else {
        value = 1.0;
    }
    

    
    
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    selectShader.enable();
    selectShader.setVarOne(value);
    selectShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    




}



void ShaderManager::enableSelectShaderUI(bool redOrGreen){
    
    /* if (currentSelect==SELECT_SHADE){
     return;
     }
     */
    currentSelect=SELECT_SHADE_UI;
    
    
    if (!selectShader.compiled){
        selectShader.vertexPath="Select.vsh";
        selectShader.fragPath="Select.fsh";
        selectShader.theTypeOfShader= 4;
        selectShader.load();
        
        
    }
    
    GLfloat value = 0;
    
    if (redOrGreen){
        value = 0.0;
    } else {
        value = 1.0;
    }
    
    
    
    
    Camera::setModelMatrix(0, 0, 0);
    selectShader.enable();
    selectShader.setVarOne(value);
    selectShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    
    
    
    
    
}


void ShaderManager::enableUIShader()
{
    
    if (currentSelect==ENABLE_UI){
        return;
    }
    
    currentSelect=ENABLE_UI;
    
    
    if (!UIShader.compiled){
        UIShader.vertexPath="NormalFast.vsh";
        UIShader.fragPath="NormalFast.fsh";
        UIShader.theTypeOfShader= 3;
        UIShader.load();
        UIShader.enable();
        
        
    }
    Camera::setModelMatrix(0, 0, 0);
    UIShader.enable();
    UIShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    glEnable(GL_TEXTURE_2D);
    
    
};

void ShaderManager::enableUIShaderMove(GLfloat X, GLfloat Y){

    if (currentSelect==ENABLE_UI_M){
        return;
    }
    
    currentSelect=ENABLE_UI_M;

    if (!UIShader.compiled){
        UIShader.vertexPath="NormalFast.vsh";
        UIShader.fragPath="NormalFast.fsh";
        UIShader.theTypeOfShader= 3;
        UIShader.load();
        UIShader.enable();
        
        
    }
    Camera::setModelMatrix(X, Y, 0);
    UIShader.enable();
    UIShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    glEnable(GL_TEXTURE_2D);


}

void ShaderManager::enableHighLightShader(float value){
    
      
    currentSelect=HIGHLIGHT_N;
    
    
    if (!highLightShader.compiled){
        highLightShader.vertexPath="HighLightShader.vsh";
        highLightShader.fragPath="HighLightShader.fsh";
        highLightShader.theTypeOfShader= 5;
        highLightShader.load();
        
        
    }
    
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    highLightShader.enable();
    float array[] = {value,value};
    highLightShader.setVarOne(array);
    highLightShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    
    
    
}

void ShaderManager::enableHighLightShaderUI(float value){
    
    if (currentSelect==HIGHLIGHT_UI){
        return;
    }
    
    currentSelect=HIGHLIGHT_UI;
    

    if (!highLightShader.compiled){
        highLightShader.vertexPath="HighLightShader.vsh";
        highLightShader.fragPath="HighLightShader.fsh";
        highLightShader.theTypeOfShader= 5;
        highLightShader.load();
        
        
    }
    
    Camera::setModelMatrix(0, 0, 0);
    highLightShader.enable();
    float array[] = {value,value};
    highLightShader.setVarOne(array);
    highLightShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    
    

}

void ShaderManager::enableSolidColorUI(float value[4]){
    
   
    
    currentSelect=TEXTURE_SOLID_COLOR_UI;
    
    
    if (!solidColor.compiled){
        solidColor.vertexPath="DrawColor.vsh";
        solidColor.fragPath="DrawColor.fsh";
        solidColor.theTypeOfShader= 7;
        solidColor.load();
        
        
    }
    
    Camera::setModelMatrix(0, 0, 0);
    solidColor.enable();
    solidColor.setVarOne4F(value);
    solidColor.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    
    
}

void ShaderManager::enableSolidColor(float value[4]){
    
    if (currentSelect==TEXTURE_SOLID_COLOR_N){
        return;
    }
    
    currentSelect=TEXTURE_SOLID_COLOR_N;
    
    if (!solidColor.compiled){
        solidColor.vertexPath="DrawColor.vsh";
        solidColor.fragPath="DrawColor.fsh";
        solidColor.theTypeOfShader= 7;
        solidColor.load();
        
        
    }
    
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    solidColor.enable();
    solidColor.setVarOne4F(value);
    solidColor.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    

}

Shader ShaderManager::textureShaderShadow;

void ShaderManager::enableTextureShaderShadow(GLfloat alpha){
    if (currentSelect==SHADOW_N){
        return;
    }
    
    currentSelect=SHADOW_N;
    
    
    
    if (!textureShaderShadow.compiled){
        textureShaderShadow.vertexPath="NormalFastShadow.vsh";
        textureShaderShadow.fragPath="NormalFastShadow.fsh";
        textureShaderShadow.theTypeOfShader= 8;
        textureShaderShadow.load();
        
    }
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    textureShaderShadow.enable();
    textureShaderShadow.setVarOne(alpha);
    textureShaderShadow.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    glEnable(GL_TEXTURE_2D);
    
    
};

void ShaderManager::enableTextureShader(){
    if (currentSelect==TEXTURE_N){
        return;
    }
    
    currentSelect=TEXTURE_N;
    
    
    if (!textureShader.compiled){
    textureShader.vertexPath="NormalFast.vsh";
    textureShader.fragPath="NormalFast.fsh";
    textureShader.theTypeOfShader= 3;
    textureShader.load();
        
    }
    Camera::setModelMatrixZoom(Camera::posX, Camera::posY, 0);
    textureShader.enable();
    textureShader.setMatrix(Camera::projectionMatrixS, Camera::modelMatrixS);
    glEnable(GL_TEXTURE_2D);
    
    
};

void ShaderManager::enableTextureShaderFullMap(){
    if (currentSelect==TEXTURE_FULL_MAP){
        return;
    }
    
    currentSelect=TEXTURE_FULL_MAP;
    
    if (!textureShader.compiled){
        textureShader.vertexPath="NormalFast.vsh";
        textureShader.fragPath="NormalFast.fsh";
        textureShader.theTypeOfShader= 3;
        textureShader.load();
        
    }
    
    
    GLfloat r,l,t,b,f,n;
    l = -135*10;
    r = 135*10;
    t = 130*10;
    b = -5*10;
    f = -1.0;
    n = 1.0;
    
    float projectionMatrixS[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    float modelMatrixS[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    
    
    
    
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
    
    
    
    
    //Camera::setModelMatrix(Camera::posX, Camera::posY, 0);
    textureShader.enable();
    textureShader.setMatrix(projectionMatrixS, modelMatrixS);

    
    

    

}

void ShaderManager::enableTextureShaderMakeSmaller(){

    if (currentSelect==TEXTURE_MAKE_SMALLER){
        return;
    }
    
    currentSelect=TEXTURE_MAKE_SMALLER;
    
    

    GLfloat r,l,t,b,f,n;
    
    
    l = -100;
    r = 100;
    t = 100;
    b = -100;
    f = -1.0;
    n = 1.0;
    
    float projectionMatrixS[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    float modelMatrixS[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    
    
    
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

    textureShader.enable();
    textureShader.setMatrix(projectionMatrixS, modelMatrixS);
    
}

void ShaderManager::enableTextureShaderNoMove(){
    if (currentSelect==TEXTURE_UI){
        return;
    }
    
    currentSelect=TEXTURE_UI;
    
    
    if (!textureShader.compiled){
        textureShader.vertexPath="NormalFast.vsh";
        textureShader.fragPath="NormalFast.fsh";
        textureShader.theTypeOfShader= 3;
        textureShader.load();
        
    }
    
    
};





Shader::Shader(){
    compiled=false;
}
Shader::~Shader(){

}


static char *
shaderLoadSource(const char *filePath)
{
    const size_t blockSize = 512;
    FILE *fp;
    char buf[blockSize];
    char *source = NULL;
    size_t tmp, sourceLength = 0;
    
    /* open file */
	#if defined(_WIN32)

	fopen_s(&fp,filePath, "r");
	#else
	fp = fopen(filePath, "r");
	#endif

   
    if(!fp) {
        fprintf(stderr, "shaderLoadSource(): Unable to open %s for reading\n", filePath);
        return NULL;
    }
    
    /* read the entire file into a string */
    while((tmp = fread(buf, 1, blockSize, fp)) > 0) {
        char * newSource ;
        newSource = new char[sourceLength + tmp + 1];
        
        if(!newSource) {
            fprintf(stderr, "shaderLoadSource(): malloc failed\n");
            if(source)
                delete [] source;
            return NULL;
        }
        
        if(source) {
            memcpy(newSource, source, sourceLength);
            delete [] source;
        }
        memcpy(newSource + sourceLength, buf, tmp);
        
        source = newSource;
        sourceLength += tmp;
    }
    
    /* close the file and null terminate the string */
    fclose(fp);
    if(source)
        source[sourceLength] = '\0';
    
    return source;
}


void Shader::setVarOne(float value){
    
    if (_varOneType==doubleValue){
        glUniform1f(_varOne, value);
    }

}


void Shader::setVarOne(float *  value){
    
    glUniform2f(_varOne, value[0], value[1]);

}

void Shader::setVarOne4F(float * value){
    glUniform4f(_varOne, value[0], value[1], value[2], value[3]);
}

void Shader::setVarTwo4f(float * value){
    glUniform4f(_varTwo, value[0], value[1], value[2], value[3]);

}





bool Shader::load(){
    GLuint vertShader =0, fragShader =0;
    
    _program = glCreateProgram();
    
    std::string locationVertex = findFile(vertexPath);
    std::string locationFrag = findFile(fragPath);
    if (!compileShader(&vertShader, GL_VERTEX_SHADER, &locationVertex)){
        printf("Failed to compile vertex shader");
        return false;
    };
    if(!compileShader(&fragShader, GL_FRAGMENT_SHADER, &locationFrag)){
        printf("Failed to compile fragment shader");
        return false;
    }

 
    // Attach vertex shader to program.
    glAttachShader(_program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(_program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    
    
    switch (theTypeOfShader) {
        case 0:
            glBindAttribLocation(_program, GLKVertexAttribPosition, "Position");
            
            glBindAttribLocation(_program, GLKVertexAttribTexCoord0, "TexCoordIn");
            glBindAttribLocation(_program, GLKVertexAttribColor, "SourceColor");
            
            break;
        case 1:
            glBindAttribLocation(_program, GLKVertexAttribPosition, "Position");
            glBindAttribLocation(_program, GLKVertexAttribColor, "SourceColor");
            break;
        case 2:
            glBindAttribLocation(_program, GLKVertexAttribPosition, "Position");
            glBindAttribLocation(_program, GLKVertexAttribTexCoord0, "TexCoordIn");
            glBindAttribLocation(_program, GLKVertexAttribTexCoord1, "LightAmount");
            glBindAttribLocation(_program, GLKVertexAttribColor, "SourceColor");
            break;
 
       
       
            break;
        case 7:
            glBindAttribLocation(_program, GLKVertexAttribPosition, "Position");
            
            break;
        case 3:
        case 4:
        case 5:
        case 8:
        case 11:
        case 12:
            glBindAttribLocation(_program, GLKVertexAttribPosition, "Position");
            glBindAttribLocation(_program, GLKVertexAttribTexCoord0, "TexCoordIn");
            break;
        /*case POINT_SHADER:
            glBindAttribLocation(_program, GLKVertexAttribPosition, "Position");
            break;
        case LINE_SHADER:
            glBindAttribLocation(_program, GLKVertexAttribPosition, "Position");
            glBindAttribLocation(_program, GLKVertexAttribColor, "SourceColor");
            break;
        case FILL_SHADER:
            glBindAttribLocation(_program, GLKVertexAttribPosition, "Position");
            break;
        case COLORFILL_SHADER:
            glBindAttribLocation(_program, GLKVertexAttribPosition, "Position");
            break;
          */
            
            
            break;
        default:
            break;
    }
    
    
    
    
    
    
    
    // Link program.
    if (!linkProgram(_program)) {
        printf("Failed to link program: %d", _program);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (_program) {
            glDeleteProgram(_program);
            _program = 0;
        }
        
        return 0;
    }
    
    // Get uniform locations.
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(_program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(_program, fragShader);
        glDeleteShader(fragShader);
    }
    _projectionUniform = glGetUniformLocation(_program, "Projection");
    _modelViewUniform = glGetUniformLocation(_program, "Modelview");
    
    switch (theTypeOfShader) {

        case 4:
            
            _varOne = glGetUniformLocation(_program, "GlowAmount");
            _varOneType = doubleValue;
            break;
        case 5:
            _varOne = glGetUniformLocation(_program, "imageSize");
            break;
        case 12:
        case 7:
            _varOne = glGetUniformLocation(_program, "SourceColor");
            break;
        case 8:
            _varOne = glGetUniformLocation(_program, "Alpha");
            _varOneType = doubleValue;
            break;
        case 11:
            _varOne = glGetUniformLocation(_program, "imageSize");
            _varTwo = glGetUniformLocation(_program, "colorValue");
        
            
        default:
            break;
    }
    compiled=true;
    return true;


}

void Shader::enable(){
    glUseProgram(_program);
}

bool Shader::compileShader(GLuint * shader, GLenum type, std::string * file){

    GLint status;
    const GLchar *source;

    
    
    
    source = (GLchar *) shaderLoadSource(file->c_str());
    if (!source) {
        printf("Failed to load vertex shader");
        return false;
    }
    
    *shader = glCreateShader(type);
    
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        printf("Shader compile log:\n%s", log);
        free(log);
    }
#endif
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        delete [] source;
        return false;
    }
    
    delete [] source;
    return true;

}

bool Shader::linkProgram(GLuint prog)
{
    GLint status;
    glLinkProgram(prog);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        printf("Program link log:\n%s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return false;
    }
    
    return true;
}

bool Shader::setMatrix(float * projectionMatrix, float * modelMatrix){

    //float modelMatrix[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    glUniformMatrix4fv(_modelViewUniform, 1, 0, modelMatrix);
    glUniformMatrix4fv(_projectionUniform, 1, 0, projectionMatrix);
    
    return true;
    

};

