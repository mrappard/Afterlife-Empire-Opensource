//
//  UnitFinder.cpp
//  After Life Empire
//
//  Created by Autobotika on 2015-07-02.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#include "UnitFinder.h"

#include "TextureGenerator.h"
#include "Gamelogic.h"
#include "FileManager.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Player.h"
#include "Tiles.h"

std::map<long, UnitFinder> UnitFinder::allFinders;


GLuint UnitFinder::_texture_Circle = 0;

UnitFinder::UnitFinder(){

    aSprite = 0;
    _vertexArray = 0;
    _vertexBuffer = 0;
    danger = false;

}
UnitFinder::~UnitFinder(){

    if (_vertexArray){
        glDeleteVertexArraysCon(1, &_vertexArray);
    }
    
    if (_vertexBuffer){
        glDeleteBuffers(1, &_vertexBuffer);
    }
}

void UnitFinder::thinkAll(){
    for (std::map<long, UnitFinder>::iterator IT = allFinders.begin();
         IT != allFinders.end();
         IT++){
        IT->second.think();
    }
}

void UnitFinder::updateAll(){
    
    for (std::map<long, UnitFinder>::iterator IT = allFinders.begin();
         IT != allFinders.end();
         IT++){
    
        IT->second.update();
    
    }

}

void UnitFinder::renderAll(){
    
    ShaderManager::enableUIShader();

    
    for (std::map<long, UnitFinder>::iterator IT = allFinders.begin();
         IT != allFinders.end();
         IT++){
        
        IT->second.render();
        
    }


}

double fixAngle(double angle){

    
    while (angle<0) {
        angle+=M_PI+M_PI;
    }
    
    while (angle>M_PI+M_PI) {
        angle-=M_PI+M_PI;
    }

    return angle;
}

void UnitFinder::think(){
    
    
    if (aSprite){
       
        if ( Camera::isBoundBoxVisible(aSprite->boundingBox.l,aSprite->boundingBox.b,aSprite->boundingBox.r,aSprite->boundingBox.t)){
            renderIt = false;
        } else {
            renderIt = true;
        }
        
        cpVect targetPoint = cpv(-Camera::posX, -Camera::posY);
        double radius = GameLogic::width/10.0;
    
        cpVect modPoint;
        position.x = (aSprite->position.x-targetPoint.x-radius)/Camera::zoom;
        modPoint.x= position.x;
        if (position.x>=GameLogic::width-radius){
            position.x=GameLogic::width-radius;
        }
        
        if (position.x<=-GameLogic::width+radius){
            position.x = -GameLogic::width+radius;
        }
        
        
        position.y = (aSprite->position.y-targetPoint.y+radius)/Camera::zoom;
        modPoint.y = position.y;
        if (position.y>=GameLogic::height-radius){
            position.y=GameLogic::height-radius;
        }
        
        if (position.y<= -GameLogic::height+radius){
            position.y = -GameLogic::height+radius;
        }
        
        cpVect direction = cpvsub(aSprite->position, cpvadd(position,cpv(-Camera::posX,-Camera::posY)));
        rotation = cpvtoangle(direction);
        
        return;
    }
    
    cpVect theValue = GameLogic::lastTouchPosition;
    theValue = cpvadd(theValue, cpv(Camera::posX, Camera::posY));
    
    rotation = cpvtoangle(theValue);
}

void UnitFinder::update(){
   
    if (!aSprite){
        return;
    }
        
    if (!_texture_Circle){
        int dimension[2];
        _texture_Circle = dontStore(findFile("CircleToFindThings.png"),dimension);
    }
    
    double radius = GameLogic::width/10.0;
    cpVect rot = cpvforangle(rotation);
    
    cpVect x1y1 = cpv(-radius, -radius);
    cpVect x2y1 = cpv(+radius, -radius);
    cpVect x1y2 = cpv(-radius, +radius);
    cpVect x2y2 = cpv(+radius, +radius);
    
    x1y1 = cpvrotate(x1y1, rot);
    x1y2 = cpvrotate(x1y2, rot);
    
    x2y1 = cpvrotate(x2y1, rot);
    x2y2 = cpvrotate(x2y2, rot);
    
    cpBB aBox = cpBBNew(0, 0, 0, 0);
    
    GLfloat widthSprite = 0;
    GLfloat heightSprite =0;
    
    if (aSprite){
        aSprite->updateNoGL();
        aBox = aSprite->boundingBox;
        
        widthSprite = aBox.r-aBox.l;
        heightSprite = aBox.t-aBox.b;
        
        if (widthSprite>heightSprite){
            heightSprite = radius*heightSprite/widthSprite/2.0;
            widthSprite=radius/2.0;
        } else {
            widthSprite = radius*widthSprite/heightSprite/2.0;
            heightSprite = radius/2.0;
        }
    }

    GLfloat vertex[] = {
        static_cast<GLfloat>(x1y1.x+position.x), static_cast<GLfloat>(x1y1.y+position.y),0,1,
        static_cast<GLfloat>(x1y2.x+position.x), static_cast<GLfloat>(x1y2.y+position.y),0,0,
        static_cast<GLfloat>(x2y1.x+position.x), static_cast<GLfloat>(x2y1.y+position.y),1,1,
        static_cast<GLfloat>(x2y2.x+position.x), static_cast<GLfloat>(x2y2.y+position.y),1,0,
        
        
        static_cast<GLfloat>(position.x-widthSprite),
        static_cast<GLfloat>(position.y+heightSprite),
        static_cast<GLfloat>(aSprite->textureBox.l),
        static_cast<GLfloat>(aSprite->textureBox.t),
        
        
        static_cast<GLfloat>(position.x-widthSprite),
        static_cast<GLfloat>(position.y-heightSprite),
        static_cast<GLfloat>(aSprite->textureBox.l),
        static_cast<GLfloat>(aSprite->textureBox.b),
        
        static_cast<GLfloat>(position.x+widthSprite),
        static_cast<GLfloat>(position.y+heightSprite),
        static_cast<GLfloat>(aSprite->textureBox.r),
        static_cast<GLfloat>(aSprite->textureBox.t),
        
        static_cast<GLfloat>(position.x+widthSprite),
        static_cast<GLfloat>(position.y-heightSprite),
        static_cast<GLfloat>(aSprite->textureBox.r),
        static_cast<GLfloat>(aSprite->textureBox.b)
    
    };

    if (!_vertexArray){
        glGenVertexArraysCon(1, &_vertexArray);
    }
    glBindVertexArrayCon(_vertexArray);
    
    if(!_vertexBuffer){
        glGenBuffers(1, &_vertexBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
}

void UnitFinder::render(){
    
    if (aSprite){
        if (aSprite->isScared){
            return;
        }
    
    }
    
    if (renderIt){
        if (_vertexArray!=0){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _texture_Circle);
            glBindVertexArrayCon(_vertexArray);
            
   
            if (danger){
                
                    ShaderManager::enableSelectShaderUI(true);
                 
                
                
            }
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            if (danger){
                ShaderManager::enableUIShader();
            }
            
            if (aSprite){
                glBindTexture(GL_TEXTURE_2D, *aSprite->dict->texture);
                glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
            }
        }
    }
}


void UnitFinder::findAndSetToActive(long UID){

    std::map<long, UnitFinder>::iterator IT = allFinders.find(UID);
    
    if (IT != allFinders.end()){
    
        
        IT->second.danger = true;
    
    }


}


void UnitFinder::test(){

    
   

}


