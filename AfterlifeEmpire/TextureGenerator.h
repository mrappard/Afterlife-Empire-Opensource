//
//  TextureGenerator.h
//  ThanatosIpad
//
//  Created by Autobotika on 13-09-21.
//  Copyright (c) 2013 Autobotika. All rights reserved.
//

#ifndef ThanatosIpad_TextureGenerator_h
#define ThanatosIpad_TextureGenerator_h

#include "VecAndBB.h"
#include "OpenGLImports.h"
#include "Sprite.h"
#include "Door.h"

#include <string>

GLuint * generateTexture(int type);
GLuint * generateTextureFill(int type);
GLuint * generateTextureBrush(int type);
GLuint * generateTextureMesh(int type);



GLuint dontStore(std::string theString, int * value);


GLuint * generateTextureR(std::string type);
GLuint * generateTexture(DoorDict * dict);
GLuint * generateTexture(SpriteDict * dict);
GLuint * generateTexture(std::string type);
GLuint * generateTextureFill(std::string type);
GLuint * generateTextureBrush(std::string type);


GLuint * replaceTexture(DoorDict * dict);
GLuint * replaceTexture(SpriteDict * dict);
GLuint * replaceTexture(std::string type);

void dumpAllTextures();

void removeTexture(GLuint * aText);
void removeTexture(GLuint aText);


#endif
