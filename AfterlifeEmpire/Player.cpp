//
//  Player.cpp
//  After Life Empire
//
//  Created by Autobotika on 2015-01-27.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#include "Player.h"
#include "TextureGenerator.h"



#include "FileManager.h"
#include "GlobalDebugger.h"
#include "Camera.h"

#undef check
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "GameClock.h"

#include "Circle.h"
#include "Tiles.h"

#include "ShaderManager.h"
#include "GameLogic.h"
#include "TranslationMatrix.h"

#include "AudioController.h"

#include "BobZombie.h"
#include "SetupVars.h"


#define UpLeft          0
#define UpRight         1
#define DownRight       2
#define DownLeft        3
#define Laugh_G         4
#define Posses_G        5
#define Scare_G         6
#define Twirl_G         7
#define Replicate_G     8
#define Replicate_Out_G 9

#define Idle 0
#define Flying -1


bool Player::scareRadiusCheck = false;
bool Player::laughRadiusCheck = false;


std::vector<ScareDict> Player::scareDict;

int Player::playerPosition=-33;


bool Player::loadDict(std::string * path){

    return false;
}



std::string Player::getDescription(int index){
    
    if (index<scareDict.size()){
        return  scareDict[index].bobText;
    }else {
    return "No Text";
    }

}

void Player::movePlayer(cpVect thePos){

    std::pair<int, int> checkThis = Tiles::findTile(thePos);
    
    if (checkThis.first<0||checkThis.first>126){
        return;
        
    }
    
    if (checkThis.second<0||checkThis.second>126){
        return;
        
    }
    
    /*
     int index = Tiles::findTile(Tiles::findTile(cpvadd(control,cpv(-Camera::posX,-Camera::posY))));
     
     
     std::vector<Sprite*> * theGrid = &Tiles::mapValues[index].renderGrid;
     
     
     
     
     for (std::vector<Sprite*>::iterator IT = theGrid->begin();
     IT!=theGrid->end();
     IT++){
     Sprite * theSprite = *IT;
     
     
     if (theSprite->dict->prop){
     if (theSprite->dict->animates){
     
     
     if (theSprite->frame>=theSprite->dict->frames[1]&&theSprite->frame<theSprite->dict->frames[2]){
     return true;
     
     } else {
     
     cpVect thePos = cpBodyGetPos(theSprite->theBody);
     Sprite::scareAllSprites(thePos, 3, theSprite->dict);
     theSprite->setAnimationState(1);
     return true;
     
     
     }
     
     
     
     }
     
     }
     
     
     }
     
     
     Sprite * target = Tiles::mapValues[index].occupiedSprite;
     
     if (target){
     
     if (target->dict->prop){
     
     if (target->dict->animates){
     
     
     cpVect thePos = cpBodyGetPos(target->theBody);
     Sprite::scareAllSprites(thePos, 3, target->dict);
     target->setAnimationState(1);
     return true;
     
     
     }
     
     }
     
     
     }
     */
    
    
    
    
    
    
    
    
    
    
    startKey = position;
    
    // cpVect theCameraPos = cpv(-Camera::posX,-Camera::posY);
    // std::pair<int,int> cameraPos = Tiles::findTile(theCameraPos);
    // std::pair<int, int> playerPos = Tiles::findTile(startKey);
    
    // int ghostDetection = 13*Camera::zoom;
    
    
    
    /*
     if (abs(cameraPos.first-playerPos.first)>ghostDetection||abs(cameraPos.second-playerPos.second)>ghostDetection){
     
     if (playerPos.first<cameraPos.first){
     if (playerPos.second<cameraPos.second){
     
     cameraPos.first+=-ghostDetection;
     cameraPos.second+=-ghostDetection;
     }   else {
     cameraPos.first+=-ghostDetection;
     cameraPos.second+=ghostDetection;
     }
     
     
     } else {
     if (playerPos.second<cameraPos.second){
     
     cameraPos.first+=ghostDetection;
     cameraPos.second+=-ghostDetection;
     }   else {
     cameraPos.first+=ghostDetection;
     cameraPos.second+=ghostDetection;
     }
     
     }
     
     
     startKey = Tiles::findCords(cameraPos);
     position = startKey;
     
     
     
     }
     */
    
    
    
    endKey = Tiles::defTile(thePos);
    
    
    if (startKey.y>endKey.y){
        //Down
        if (startKey.x>endKey.x){
            frame=dict->frames[3];
            direction =3;
            
            
        } else {
            frame=dict->frames[2];
            direction =2;
            
        }
        
    } else {
        if (startKey.x>endKey.x){
            frame=dict->frames[0];
            direction =0;
            
            
        } else {
            frame=dict->frames[1];
            direction =1;
            
        }
        
    }
    
    
    tween=0;
    playerState=Flying;
    




}

bool Player::mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
    if (theTouchNumber==MOUSE_L){
        if (input==MOUSE_STATE_UP){
            

            cpVect thePoint = cpvadd(control,cpv(-Camera::posX,-Camera::posY));
            movePlayer(thePoint);

            
        }
    }
    return false;
}

Player::Player(){
    dict=0;
    timeTileDead=1000;

}
Player::~Player(){

}

Player * Player::mainPlayer = 0;

void Player::init(){
    mainPlayer = this;
    
    
    dict = Sprite::theSpriteAssets[-1];
    
    UID = Sprite::generateUID();
    
    frame=0;
    goto_AnimationState=INT_MAX;
    direction=0;
    _texture=0;
  
    

    tween =0;

    


    position = cpv(10.0,10.0);
    
    

    
    startKey=cpv(10.0,10.0);
    endKey=cpvzero;
    
    _vertexArray = 0;
    _vertexBuffer = 0;
    
    //graveYardAble=graveYardAble;
    //summonPosition=anPosition;




}

void Player::scareRadius(bool onOrOff){

    if (onOrOff){
    cpVect thePos = mainPlayer->position;
    GLfloat color[] = {1.0,0,0,0.33};
    Circle::addCircle(thePos, 30, color, "ScareRadius");
        
    scareRadiusCheck = true;
    
        
    } else {
    Circle::releaseCircle("ScareRadius");
    scareRadiusCheck = false;
    }

}

void Player::laughRadius(bool onOrOff){
    
    if (onOrOff){
    cpVect thePos = mainPlayer->position;
    GLfloat color[] = {1.0,0,0,0.33};
    Circle::addCircle(thePos, 60, color, "LaughRadius");
    laughRadiusCheck = true;
    } else {
        Circle::releaseCircle("LaughRadius");
    laughRadiusCheck = false;
    }
}






void Player::possession(){

    
    frame=dict->frames[Posses_G];
    playerState=Posses_G;
     direction = Posses_G;

}




void Player::replicate(){

    frame=dict->frames[Replicate_G];
    playerState=Replicate_G;
    direction = Replicate_G;
}


void Player::twirl(){

    frame=dict->frames[Twirl_G];
    playerState=Twirl_G;
    direction = Twirl_G;


}



void Player::laugh(){


    frame=dict->frames[Laugh_G];
    playerState=Laugh_G;
    direction = Laugh_G;
    
    Sprite::summonAllSprites(position, 60.0);

}

void Player::scare(){
    
    
    scareAbility(Scare_G-4);
    

}


void Player::scareAbilityDrawRadiusButton(int index){
    mainPlayer->scareAbilityDrawRadius(index);

}
void Player::scareAbilityDrawRadius(int index){
    
    if (scareDict.size()<=index){
    
        
    
    } else {
    
       int tile =  Tiles::findTile(Tiles::findTile(position));
    
        
        if (tile >0){
            
            
            ScareDict * target = &scareDict[index];

            
            Tiles::tileShowRadius(tile,target->radius);
        
        
        }
    
    }


}



void Player::scareAbilityButton(int index){

    mainPlayer->scareAbility(index);
}

#include "GameClock.h"

void Player::scareAbility(int index){
    
    
    
    
    
    if (scareDict.size()<=index){
    
    
    }else {
        
        
        if (frame>=dict->frames[4+index]&&frame<dict->frames[5+index]){
            return;
        }
        
    
        ScareDict * target = &scareDict[index];
        
        
        if (GameClock::checkTime(target->lastClick, target->reload)){
            TranslationMatrix::warning(RELOAD_TIME);
            return;
        }
        
        
        
        if (GameLogic::energy<target->energy){
            TranslationMatrix::warning(NOT_ENOUGH_ENERGY);
            return;
        
        }
        
        if (GameLogic::getCurrentLevel()<target->repLevelRequired){
            
            TranslationMatrix::warning(LEVEL_TO_LOW);
            return;
        }
        
        if (target->specialFactor==TWOXTWO){
            if (
                Tiles::checkTileOffestForPlacement(renderGridIndex,0,0)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,1,0)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,0,1)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,1,1)){
            
            
                
            
            } else {
            
                TranslationMatrix::warning(NEAR_WALL);
                return;
            }
            
        } else if (target->specialFactor==THREEXTHREE){
            if (
                Tiles::checkTileOffestForPlacement(renderGridIndex,-1,0)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,0,0)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,1,0)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,-1,-1)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,0,-1)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,1,-1)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,-1,-2)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,0,-2)&&
                Tiles::checkTileOffestForPlacement(renderGridIndex,1,-2)){
                
        
                
                
            } else {
                
     
                
                TranslationMatrix::warning(NEAR_WALL);
                return;
            }
        
        
        }
        
        
        
        
        if (!BobZombie::scaresAreFree()){
            GameLogic::energy -=target->energy;
            target->lastClick = GameClock::time;
        } else {
            
            target->lastClick = GameClock::time-target->reload-15;
            
        }
        
        frame=dict->frames[4+index];
        playerState = 4+index;
        direction = 4+index;
        
        
        
        cpVect thePos = position;
        
        int index = Tiles::findTile(Tiles::findTile(thePos));
   
        std::vector<Sprite*> theFoundSprites = Tiles::grabAllSpriteLos(index, target->radius);
    
        
        for (std::vector<Sprite*>::iterator IT = theFoundSprites.begin();
             IT!=theFoundSprites.end();
             IT++){
        //std::pair<double, double> amount =
            
            
            
             (*IT)->scareFromPlayer(thePos, target);
            
            
        
        }
        
        
        AudioController::playAudioLua(-1, target->name);
     
        
        
        
    }



}



void Player::changeBackToNormalMode(){
    switch (direction) {
        case  UpLeft :
            frame=dict->frames[UpLeft];
            direction = UpLeft;
            break;
        case UpRight:
            frame=dict->frames[UpRight];
            direction= UpRight;
            break;
        case DownRight:
            frame=dict->frames[DownRight];
            direction = DownRight;
            break;
        case DownLeft:
            frame=dict->frames[DownLeft];
            direction= DownLeft;
            break;
            
        case Laugh_G:
        case Posses_G:
        case Scare_G:
        case Twirl_G:
        case Replicate_G:
        case Replicate_Out_G:
            frame=dict->frames[DownLeft];
            direction=DownLeft;
            break;
        
            
            
        default:
            frame=dict->frames[DownLeft];
            direction=DownLeft;
            break;
    }

    
    frame=dict->frames[DownLeft];
}


void Player::think(){
    
    
    animationStateHack++;
    
    if (animationStateHack>2){
        frame++;
        
        if (dict->frames[direction+1]<=frame){
            
            if (direction==Replicate_G){
                frame=dict->frames[Replicate_Out_G];
                direction = Replicate_Out_G;
                
            
            } else {
            changeBackToNormalMode();
                
            }
            frame=dict->frames[direction];
        }
        animationStateHack=0;
    }

    
    reputationStruct testStruct = GameLogic::reputationLevels[GameLogic::getCurrentLevel()];
   
    
    if (playerState==Flying){
        
        if (cpvdist(endKey, position)>1.0){
            cpVect direction_ = cpvnormalize(cpvsub(endKey, startKey));
            direction_ = cpvnormalize(direction_);
            cpvmult(direction_, testStruct.speed);
            position = cpvadd(position, direction_);
        } else {
            position = endKey;

            playerState=Idle;
        
        }
     
        /*
        tween+=1.0/120.0;
        if (tween>=1.0){
            flying=false;
            return;
        }
        cpBodySetPos(theBody, cpvlerp(startKey, endKey, tween));
        cpSpaceReindexShape(theBody->space_private, theShape);
         */
    }
    
    cpVect bodyPos = position;
    
    if (laughRadiusCheck){
        Circle::setCirclePosition(bodyPos, "LaughRadius");
    }
    
    if (scareRadiusCheck){
        Circle::setCirclePosition(bodyPos, "ScareRadius");
    
    }
    
    
}



void Player::update(){
    if (dict){
    cpVect pos ;
    double rotation;
    
    pos = position;
    rotation = 0;
    update(pos,rotation);
    }
}



void Player::update(cpVect pos, double rotation){
    
    if (_texture==0){
        if (dict->texture==0){
            generateTexture(dict);
        }
        _texture = dict->texture;
    }
    
    if ( dict->animates   ){
        GLfloat text[12];
        dumpTextureCord(frame, text);
               
        GLfloat gBoxVertexData[] = {
            (GLfloat)(text[4]+pos.x+dict->sludgeX),
            (GLfloat)(text[5]+pos.y+dict->sludgeY),0,        text[0],  text[1],
            (GLfloat)(text[6]+pos.x+dict->sludgeX),
            (GLfloat)(text[7]+pos.y+dict->sludgeY),0,        text[0],  text[3],
            (GLfloat)(text[8]+pos.x+dict->sludgeX),
            (GLfloat)(text[9]+pos.y+dict->sludgeY),0,        text[2],  text[1],
            (GLfloat)(text[10]+pos.x+dict->sludgeX),
            (GLfloat)(text[11]+pos.y+dict->sludgeY),0,      text[2],  text[3]
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
        glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 4*5, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*5, BUFFER_OFFSET(12));

        glBindVertexArrayCon(0);
        
    } else {
        
        
        double minX = -dict->width/2;
        double maxX = dict->width/2;
        double minY = -dict->height/2;
        double maxY = dict->height/2;
        
        cpVect theRotation = cpvforangle(rotation);
        
        cpVect X1Y1 = cpv(minX,minY);
        cpVect X2Y1 = cpv(maxX,minY);
        cpVect X1Y2 = cpv(minX,maxY);
        cpVect X2Y2 = cpv(maxX,maxY);
        
        X1Y1 = cpvrotate(X1Y1, theRotation);
        X2Y1 = cpvrotate(X2Y1, theRotation);
        X1Y2 = cpvrotate(X1Y2, theRotation);
        X2Y2 = cpvrotate(X2Y2, theRotation);
        
        GLfloat textX=0,textY = 0;
        GLfloat textH=1.0,textW = 1.0;
        
        
        //_textureArray->at(frame);
        
        GLfloat gBoxVertexData[] = {
            (GLfloat)(X1Y2.x+pos.x), (GLfloat)(X1Y2.y+pos.y),0,        textX,  textY,
            (GLfloat)(X1Y1.x+pos.x), (GLfloat)(X1Y1.y+pos.y),0,        textX,  textH,
            (GLfloat)(X2Y2.x+pos.x), (GLfloat)(X2Y2.y+pos.y),0,        textW,  textY,
            (GLfloat)(X2Y1.x+pos.x), (GLfloat)(X2Y1.y+pos.y),0,        textW,  textH
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
        glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 4*5, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*5, BUFFER_OFFSET(12));
        
        glBindVertexArrayCon(0);
        
    }
    
    

    int index = Tiles::findTile(Tiles::findTile(pos));
    playerPosition = index;
    

    
    
    
    
    
    /*
    if (index>0&&!(Tiles::mapValues.size()<index)){
       
        Tiles::mapValues[index].renderGrid.push_back(this);

        Sprite::gridsLastTurn.insert(index);
        
    } else {
        
    }
    */
    
    
}

void Player::render(){
    
    if (_vertexArray!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *_texture);
        glBindVertexArrayCon(_vertexArray);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
    }
    
    
    
}
 
void Player::renderAlpha(){

    if (_vertexArray!=0){
    
        
        ShaderManager::enableReverseAlphaShader();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *_texture);
        glBindVertexArrayCon(_vertexArray);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
    }
    
    

}



bool Player::UItiles(int startIndex, int numberOfTiles,
             std::vector<PlayIcons> * dict, bool propOrNot, bool animate){
    
    
    int level = GameLogic::getCurrentLevel();
    
    for (std::vector<ScareDict>::iterator IT = scareDict.begin();
         IT != scareDict.end();
         IT++){
        if (level<(*IT).repLevelRequired){
            continue;
        }
        
        
        size_t counter = IT - scareDict.begin();
    
        PlayIcons playIcon;
        playIcon.texture=mainPlayer->dict->texture;
    
        GLfloat texture[12];
        int frame = 0;
        frame = mainPlayer->dict->frames[4+counter];
        frame += (*IT).iconOffset;
        playIcon.cost = (*IT).energy;
        
        Sprite::dumpTextureCord(mainPlayer->dict->index, frame, texture, mainPlayer->dict);
        
        playIcon.vertex[0]=texture[4];
        playIcon.vertex[1]=texture[5];
        playIcon.vertex[2]=texture[6];
        playIcon.vertex[3]=texture[7];
        
        playIcon.vertex[4]=texture[8];
        playIcon.vertex[5]=texture[9];
        playIcon.vertex[6]=texture[10];
        playIcon.vertex[7]=texture[11];
        
        playIcon.cords[0]=texture[0];
        playIcon.cords[1]=texture[1];
        playIcon.cords[2]=texture[2];
        playIcon.cords[3]=texture[3];
        
        
        
        
        
        
        dict->push_back(playIcon);
        
    }
    
    
    return true;


}

void Player::showPath(int path){

    mainPlayer->findPath(path);

    /*
    
    for (std::vector<PathTile>::iterator IT = mainPlayer->path.begin();
         IT != mainPlayer->path.end();
         IT++){
        
        
    
        IT->index;
        
    }
    */


}

std::vector<int> Player::returnPath(){
    
    std::vector<int> newPath;
    newPath.reserve(mainPlayer->path.size());
    
    
    for (std::vector<PathTile>::iterator IT = mainPlayer->path.begin();
         IT != mainPlayer->path.end();
         IT++){
        
        newPath.push_back((*IT).index);

    }
    
    
    return newPath;


}





