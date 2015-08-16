//
//  SaveMap.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-10-16.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#include "SaveMap.h"
#include "LazyVars.h"
#include "GlobalDebugger.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "Tiles.h"
#include "Player.h"
#include <algorithm>    // std::sort

std::vector<std::vector<Sprite*> > SaveMap::spriteHolder;

cpVect SaveMap::findCords(int position){
    
    int xValue = position%Tiles::width;
    int yValue = position/Tiles::width;
    
    return findCords(cpv(xValue,yValue));
}


cpVect SaveMap::findCords(cpVect position){

    double multi=10.0;
    
    cpVect returnValue;
    returnValue.x = (-multi*position.y) + (multi*position.x);
    returnValue.y = T_V*multi*position.y + T_V* multi*position.x;


    return returnValue;
    
    
}

cpVect SaveMap::findCordsTile(cpVect position){


    cpVect newValue1;
    
    {
        //-100,15,-14,-28
        cpVect unitVector = position;
        unitVector.y=0;
        
        double zrot = (M_PI/2.0) * ((25.6+1.2-0.07-0.006)/90.0) +M_PI ;
        newValue1.x = cos(zrot)*unitVector.x - sin(zrot)*unitVector.y;
        newValue1.y = sin(zrot)*unitVector.x + cos(zrot)*unitVector.y;
    }
    
    
    
    //Y Value
    cpVect newValue2 = cpvzero;
    
    {
        cpVect unitVector = position;
        unitVector.x=0;
        double zrot = (M_PI/2.0) * (63.28/90.0) +M_PI ;
        newValue2.x = cos(zrot)*unitVector.x - sin(zrot)*unitVector.y;
        newValue2.y = sin(zrot)*unitVector.x + cos(zrot)*unitVector.y;
    }
    
    
    
    
    
    
    
    return cpvadd(newValue1, newValue2);
    
    
    
    
}

cpVect SaveMap::findCordsReverse(cpVect position){


    
    
    
    
    double zrot = (63.3/180.0)*-M_PI;
    
    cpVect pointA = cpv(cos(zrot)*position.x - sin(zrot)*position.y,
                        sin(zrot)*position.x+cos(zrot)*position.y );
    
    zrot = (26.7/180.0)*-M_PI;
    cpVect pointB = cpv(cos(zrot)*position.x - sin(zrot)*position.y,
                        sin(zrot)*position.x+cos(zrot)*position.y );
    
    double mod = 0.11118;
    
    pointA.x += 4.764258;
    pointB.y += 4.764258;
    
    return cpv(pointA.x*mod,pointB.y*mod);
    

    
    /*
     cpVect newValue1;
     
     {
     
     double zrot = (M_PI/2.0) * ((25.6+1.2-0.07-0.006)/90.0) ;
     
     newValue1.x = cos(zrot)*position.x;
     newValue1.y = sin(zrot)*position.x;
     }
     
     
     
     //Y Value
     cpVect newValue2 = cpvzero;
     
     {
     double zrot = (M_PI/2.0) * (63.28/90.0) ;
     
     newValue2.x = - sin(zrot)*position.y;
     newValue2.y = cos(zrot)*position.y;
     }
     
     
     
     
     
     
     
     return cpvadd(newValue1, newValue2);
     
     */
    
    

}

void SaveMap::test(){
  
    printf("Test Should Not Run");
/*    cpVect vB1 = findCords(cpv(51,73));
    cpVect vB2 = findCords(cpv(21,58));
    cpVect vB3 = findCords(cpv(3,9));
    
    cpVect vC1 = findCordsReverse(vB1);
    cpVect vC2 = findCordsReverse(vB2);
    cpVect vC3 = findCordsReverse(vB3);

*/
}


bool sortHorizontal(Sprite*s1, Sprite*s2){
    
    cpVect pos1, pos2;
    pos1 = cpv(s1->boundingBox.l, s1->boundingBox.b);//s1->grid;
    pos2 = cpv(s2->boundingBox.l, s2->boundingBox.b);//s2->grid;
    
    return  (pos1.y>pos2.y);//?true:(pos1.x>pos2.x);
  
}


void SaveMap::update(std::vector<Sprite*> * data){

    
    std::vector<std::vector<Sprite*> > info;
    
    info.assign(Tiles::width,  std::vector<Sprite*>());
    
    for (std::vector<Sprite*>::iterator IT= data->begin();
         IT != data->end();
         IT++){
        if (*IT==NULL){
            continue;
        }
        
        if (!(*IT)->_vertexArray){
            continue;
        }

        
        (*IT)->grid = findCordsReverse(cpv((*IT)->boundingBox.r,(*IT)->boundingBox.b));;
        
        size_t count = (*IT)->grid.x;
        
        info[count].push_back(*IT);
    }
    
    
    
    
    
    
    
    for ( std::vector<std::vector<Sprite*> >::iterator IT = info.begin();
         IT != info.end();
         IT++){
        
        
        std::sort((*IT).begin(),
                  (*IT).end(),
                  sortHorizontal);
        
    }
    
    
    spriteHolder = info;
    
    


}


void SaveMap::update(){
    
   
    std::vector<Sprite*> data;
    
    for (std::map<size_t, Sprite*>::iterator IT = Sprite::spriteByID.begin();
         IT != Sprite::spriteByID.end();
         IT++){
        
        Sprite * aSprite = IT->second;
        aSprite->updateNoGL();
        
        if (
        Camera::isBoundBoxVisible(aSprite->boundingBox.l, aSprite->boundingBox.b, aSprite->boundingBox.r, aSprite->boundingBox.t)
            ){
            
            data.push_back(aSprite);
        
        }
    
    }
    
    {
        
        
        Sprite * aSprite   = Player::mainPlayer;
        
        aSprite->updateNoGL();
        
        if (
            Camera::isBoundBoxVisible(aSprite->boundingBox.l, aSprite->boundingBox.b, aSprite->boundingBox.r, aSprite->boundingBox.t)
            ){
            
            data.push_back(aSprite);
            
        }

    }
    
    {
        Sprite * aSprite = Sprite::testSprite;
        
        aSprite->updateNoGL();
        
        if (
            Camera::isBoundBoxVisible(aSprite->boundingBox.l, aSprite->boundingBox.b, aSprite->boundingBox.r, aSprite->boundingBox.t)
            ){
            
            data.push_back(aSprite);
            
        }
        
        
        
        
        
    }
    
    
    
    
    
    std::vector<std::vector<Sprite*> > info;
    
    info.assign(Tiles::width,  std::vector<Sprite*>());
    
    
   
    
    
    
    for (std::vector<Sprite*>::iterator IT= data.begin();
         IT != data.end();
         IT++){
        
        
        
        
        (*IT)->grid = findCordsReverse(cpv((*IT)->boundingBox.r,(*IT)->boundingBox.b));;
        
        size_t count = (*IT)->grid.x;
        
        
      
        
        info[count].push_back(*IT);
        
        
        (*IT)->updateGL();
    
    }
    
    
    
    
    
    
    
    
    for ( std::vector<std::vector<Sprite*> >::iterator IT = info.begin();
         IT != info.end();
         IT++){
        
        
        std::sort((*IT).begin(),
                  (*IT).end(),
                  sortHorizontal);
        
    }
    
    
    spriteHolder = info;
   
    
    }

void SaveMap::render(){

     ShaderManager::enableTextureShader();
    
    

    /*
    
    
    
    //Find Camera Position
    int value[2];
    
    std::pair<int,int> tilePos = Tiles::findTile(cpv(Camera::posX, Camera::posY));
    
    
    value[0]=-1*tilePos.first;
    value[1]=-1*tilePos.second;
    
    //START
    
    
    
    //Draw Tiles
    int maxStep = 16*Camera::zoom;
    
    
    
    if (Tiles::_vertexArrayTiles!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(Tiles::_vertexArrayTiles);
        
        
        for (int counterY =value[1]+maxStep;
             counterY >value[1]-maxStep;
             counterY--){
            if (counterY>=0){
                int Yvalue = ((Tiles::width)*(counterY));
                //-12,16
                for (int counterX =value[0]+maxStep;
                     counterX > value[0]-maxStep;
                     counterX--) {
                    
                    int Xvalue = counterX;
                    if (Xvalue<0||Yvalue<0||(Yvalue+Xvalue)>=Tiles::mapValues.size()){
                        continue;
                    }
                    
                    
                    Tiles::mapValues[Yvalue+Xvalue].renderTile(Yvalue+Xvalue);
                    
                    
                }
            }
        }
    }
    */
    
    /*
    
    int counterOne=0;
    int counterTwo=0;
    
    
    if (Tiles::_vertexArrayWalls!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(Tiles::_vertexArrayWalls);
        
        if (true){
            glEnable(GL_STENCIL_TEST);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.05);
            glClear(GL_STENCIL_BUFFER_BIT);
        }
        
        
        int xLow =  -tilePos.first+-maxStep;
        int xHigh = -tilePos.first+maxStep;
        int yLow = -tilePos.second+-maxStep;
        int yHigh = -tilePos.second+maxStep;
        
        for (int counterX = xHigh; counterX>xLow; counterX--){
            counterTwo = 0;
            for (int counterY = yHigh; counterY>yLow; counterY--){
                
                
                
                glStencilFunc(GL_GEQUAL, counterTwo+counterOne, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                Tiles::renderWallR(counterX, counterY);
                counterTwo++;
            }
            
            counterTwo = 0;
            for (int counterY = yHigh; counterY>yLow; counterY--){
                
                
                
                glStencilFunc(GL_GEQUAL, counterTwo+counterOne, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                Tiles::renderWallL(counterX, counterY);
                
                //Tiles::renderSprites(counterX, counterY);
                
                if (counterX+(counterY*Tiles::width)==Sprite::indexOfTestSprite){
                    
                    Sprite::testSpriteRender();
                }
                counterTwo++;
            }
            
            counterOne+=5;
        }
        
        
        if (true){
            glDisable(GL_STENCIL_TEST);
            glDisable(GL_ALPHA_TEST);
        }
    }
    */
   
    
    int value[2];
    
    std::pair<int,int> tilePos = Tiles::findTile(cpv(Camera::posX, Camera::posY));
    
    
    value[0]=-1*tilePos.first;
    value[1]=-1*tilePos.second;
    
    int maxStep = 16*Camera::zoom;
    
    
    int maxY=value[1]+maxStep;
    int minY=(value[1]<maxStep)?0:value[1]-maxStep;
    int maxX=value[0]+maxStep;
    int minX=(value[0]<maxStep)?0:value[0]-maxStep;
    
    glClear(GL_STENCIL_BUFFER_BIT);
    
    for ( int counter = maxX;
         counter > minX;
         counter --){
        
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_ALPHA_TEST);
       
       
        
        std::vector<Sprite*> * IT = &spriteHolder.at(counter);
        
        
       
        
        for (int counterY = maxY; counterY>minY; counterY--){
            Tiles::renderWallR(counter, counterY);
        }
        
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.05);
        
       
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        
        
        for (int counterY = maxY; counterY>minY; counterY--){
            glStencilFunc(GL_GEQUAL, 129-counterY, 0xFF);
            Tiles::renderWallL(counter, counterY);
        }
        
        
        
        
        for (std::vector<Sprite*>::iterator IT2 = IT->begin(); IT2 != IT->end(); IT2++ )
        {
        
            cpVect gridRef = SaveMap::findCordsReverse((*IT2)->position);
        
            int index = gridRef.y;
            
            glStencilFunc(GL_GEQUAL, 129-index, 0xFF);
           
                (*IT2)->render();
            
        }
        
        
       
        
    }
    

}
    
    
    
    
    
    
    


