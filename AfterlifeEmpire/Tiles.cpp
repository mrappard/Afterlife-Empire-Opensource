//
//  Tiles.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-09-30.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#include "Tiles.h"

#include "TextureGenerator.h"
#include "LazyVars.h"
#include "ScreenTranslator.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "GlobalDebugger.h"

#include "GameLogic.h"


#include "Directions.h"

#include <vector>

#include "Pathfinding.h"
#include "GameClock.h"

//Hack For Cocoa to get Boost to Work
#undef check

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "FrameBuffer.h"

#include "FileManager.h"
#include "Directions.h"
#include "Circle.h"
#include "Bulldozer.h"
#include "Player.h"

#include "TranslationMatrix.h"
#include "DebugTools.h"
#include "Userinterface.h"
#include <unordered_set>

#include "SaveMap.h"
#include <unordered_set>
#include "SetupVars.h"
#include "AudioController.h"

//std::vector<short> Tiles::tiles;
std::vector<TileObject> Tiles::mapValues;
//std::vector<short> Tiles::walls;
int Tiles::width = 128;
int Tiles::height = 128;


//std::vector<size_t> Tiles::hoverTiles

std::set<int> Tiles::hoverTileSelected;
std::set<int> Tiles::trapRadius;

std::map<int,TileDict*> Tiles::tileDictionary;

int Tiles::hoverTile=-1;
int Tiles::hoverWall=-1;
int Tiles::hoverWallSide;

static cpBB tileButtons[9];
size_t Tiles::changeTileType =1;

int Tiles::startIndex=-1;
int Tiles::lastIndex=-1;

bool Tiles::halfWalls=false;



// Debug

int debugS=0;
int debugE=0;
std::vector<PathTile> debugPath;

std::vector<int> Tiles::usedTiles;

GLuint Tiles::_vertexArrayTiles=0;
GLuint Tiles::_vertexBufferTiles=0;

GLuint Tiles::_vertexArrayWalls=0;
GLuint Tiles::_vertexBufferWalls=0;


GLuint Tiles::_vertexArraySelection=0;
GLuint Tiles::_vertexBufferSelection=0;

static bool overInfluence = false;


int Tiles::houseDimension[4];

int counterOne = 0;
int counterTwo = 0;


static Door * testDoor;


std::vector<TileDict*> Tiles::tileDictVector;

std::map<int,std::vector< FastTileObject> > Tiles::fastTileSort;


TileObject::TileObject(){

    type = 0;
    window = false;
    textureWallL =0;
    textureWallR =0;
    theDoorL = 0;
    theDoorR = 0;
    block = false;
    thinkValue = 0;
   
    theDoor = 0;
    renderGrid.clear();
    occupiedSprite = 0;
    
    
    
    
    
    
}

void TileObject::renderTile(){
    
    


}

void TileObject::renderTile(size_t index){

    

    
    if (Tiles::tileDictVector[type]->checker){
    
        int valueEvenOrOddY = (index/Tiles::width)%(2);
        int valueEvenOrOddX = (index%2);
    
        if ((valueEvenOrOddX + valueEvenOrOddY)%2){
        
            glBindTexture(GL_TEXTURE_2D,*Tiles::tileDictVector[type]->texture);
            glDrawArrays(GL_TRIANGLES, GLuint(index*6), 6);
        } else {
        
            glBindTexture(GL_TEXTURE_2D,*Tiles::tileDictVector[type]->textureC);
            glDrawArrays(GL_TRIANGLES, GLuint(index*6), 6);
        }
    
    } else {
    
        
        glBindTexture(GL_TEXTURE_2D, *Tiles::tileDictVector[type]->texture);
        glDrawArrays(GL_TRIANGLES, GLuint(index*6), 6);
    
    }
    
}


void TileObject::renderTile(size_t index, int _type){
    
    if (Tiles::tileDictVector[_type]->checker){
    
        
        int valueEvenOrOddY = (index/Tiles::width)%(2);
        int valueEvenOrOddX = (index%2);
        
        if ((valueEvenOrOddX + valueEvenOrOddY)%2){
            
            glBindTexture(GL_TEXTURE_2D,*Tiles::tileDictVector[_type]->texture);
            glDrawArrays(GL_TRIANGLES, GLuint(index*6), 6);
        } else {
            
            glBindTexture(GL_TEXTURE_2D,*Tiles::tileDictVector[_type]->textureC);
            glDrawArrays(GL_TRIANGLES, GLuint(index*6), 6);
        }
    
    }else{

    glBindTexture(GL_TEXTURE_2D, *Tiles::tileDictVector[_type]->texture);
    glDrawArrays(GL_TRIANGLES, GLuint(index*6), 6);
    }

}





bool Tiles::UItiles(int startIndex, int numberOfTiles,
                    std::vector<TileDict*> * dict){
    
    
    int currentLevel = GameLogic::getCurrentLevel();
    
    int counter= 0;
    
    std::map<int,TileDict*>::iterator IT = tileDictionary.begin();
    
    for (int i = 0; i<startIndex; i++){
        IT++;
        
        if (IT ==tileDictionary.end()){
            break;
        }
    }
    
    
    for (std::map<int,TileDict*>::iterator IT2 = IT;
         IT2 != tileDictionary.end(); IT2++){
    
        if (numberOfTiles<=counter){
            return true;
            
        }
        
        if (IT2->second->requiredLevel<=currentLevel){
        
            dict->push_back(IT2->second);
             counter++;
        }
        
        
    }
    
    return false;

}


void Tiles::createTileTextures(){
    
    
    for (std::map<int,TileDict*>::iterator IT = tileDictionary.begin(); IT != tileDictionary.end(); IT++){
    
        if (IT->second->texture==0){
            IT->second->texture = generateTexture(findFile(IT->second->image));
        }
        
        if (IT->second->textureL==0){
            IT->second->textureL =  generateTexture(findFile(IT->second->imageL));
        }
        
        if (IT->second->textureR==0){
            IT->second->textureR = generateTexture(findFile(IT->second->imageR));
        }
    }

}

int Tiles::moveDirection(int start ,int end){

    /*
    int startX = start%width;
    int startY = start/width;
    
    int endX = end%width;
    int endY = end/width;
    
    
    if (startX>endX){
        //Go Left
        if (startY<endY){
        // Go Up
            
            
        
        
        } else {
         //  Go Down
        
        }
    
    } else {
        // Go Right
        
        if (startY<endY){
            // Go Up
            
            
            
        } else {
            //  Go Down
            
        }
    
    
    }
    */
    
    
    
    if (start>end){
    
        if (start-end>1){
            
            
            
            return 2;//Down
        } else {
            return 3;//Left
        }
    } else {

        if (end-start>1){
            return 0;
        } else {
                    return 1;
        }
    }

}

void Tiles::saveDict(){

    /*
    
    char buff[1000];
    std::map<cpShape*, Sprite*>::iterator IT;
    
    
    theOutput->append("\"Sprites\":[");
    
    for (IT=levelSprites.begin();IT!=levelSprites.end();IT++){
        
        std::string enterData;
        cpBodySetPos(
                     IT->second->theBody,
                     IT->second->summonPosition
                     );
        
        cpBodySetVel(
                     IT->second->theBody,
                     cpvzero
                     );
        
        
        enterData.append("{");
        
        sprintf(buff, " \"position\":[%f,%f],\t",
                IT->second->summonPosition.x,
                IT->second->summonPosition.y);
        enterData.append(buff);
        
        
        sprintf(buff, " \"type\":%i",
                IT->second->type
                );
        enterData.append(buff);
        
        enterData.append("},");
        
        theOutput->append(enterData);
        
    }
    
    if (levelSprites.size()!=0){
        theOutput->erase(theOutput->begin()+(theOutput->size()-1));
    }
    theOutput->append("]},");
*/


}

bool Tiles::purge(){


    for (std::vector<TileObject>::iterator IT = Tiles::mapValues.begin();
         IT != Tiles::mapValues.end();
         IT++){
    
        *IT = TileObject();
    
    }

    
    
    

    
    return true;
}


void Tiles::createBlankMap(){

    
   

    fillTiles(0, 0, width, width, 0);
     Sprite::spawnTrees();
    
    
    fillTiles(0, 0,             3, 3, 0);
    
    fillTiles(0, width-4,       3, width-1, 0);
    
    fillTiles(width-4, 0,       width-1, 3, 0);
    
    fillTiles(width-4, width-4, width-1, width-1, 0);
    
    //Create Path
    
    
    
    fillTiles(58, 0, 60, 4, 1);
    fillTiles(68, 0, 70, 4, 1);
    fillTiles(58, 5, 70, 7, 1);
    
    fillTiles(61, 0, 67, 3, 0);
    
    
    
    
    Door::createThis  = (Door::theDoorAssets.begin()->second);
    
    
    fillTilesMakeWalls(61, 10, 67, 16, 4);
    

    clearWall(1344,true);
    
    clearWall(1725,true);
    
    clearWall(1732,true);
    
    clearWall(2240,true);
    
    hoverWall = 0;
    hoverWallSide = 0;
    
}

static std::vector<int> quickSearch;
static int t_check = 0;

void quickSearchSetup(){
    
    if (quickSearch.size()!=Tiles::width*Tiles::width){
        
        quickSearch.clear();
        
        quickSearch.assign(Tiles::width*Tiles::width,0);
        
        
    }
    
    t_check++;
    
    
}



void Tiles::initialize(int widthI, int heightI){
    
    width=widthI;
    height=heightI;
    

    TileObject newObject = TileObject();
    
    Tiles::mapValues.assign(widthI*heightI, newObject);
    
    
    Door::removeAllDoors();
    
    
    createBlankMap();
    

    
    
    checkWalkAbility();
    
    
    Camera::posX = -567.776442;
    Camera::posY = -370.506811;
    
    
}

cpVect RotateVector2d(cpVect v, double radian)
{
    cpVect result;
    result.x = v.x * cos(radian) - v.y * sin(radian);
    result.y = v.x * sin(radian) + v.y * cos(radian);
    return result;
}


bool Tiles::checkTileOffestForPlacement(int index, int offsetX, int offsetY){

    if (index>=0&&index<mapValues.size()){
    
        int xPos = index%width;
        int yPos = index/width;
        
        if (xPos+offsetX<0||xPos+offsetX>=width-2){
            return false;
        }
        
        if (yPos+offsetY<0||yPos+offsetY>=width-2){
            return false;
        }
        
        xPos+=offsetX;
        yPos+=offsetY;
        
        TileObject * theObject = &Tiles::mapValues[xPos+yPos*width];
        
        
        
        
        TileObject * theObjectX = &Tiles::mapValues[xPos+yPos*width+1];
        TileObject * theObjectY = &Tiles::mapValues[xPos+yPos*width+width];
        
        
        if (theObject->block||theObject->lw||theObject->rw||theObject->theDoorL||theObject->theDoorR||theObjectX->theDoorR||theObjectY->theDoorL){
           
            
            return false;
         
        
        }
    
        return true;
    
    
    }


    return false;
}

int Tiles::goDirect(int index, int direction){

    if (index==L){
        if (index%width==0){
            return INT_MAX;
        }
        return index-1;
    
    }
    
    if (index==U){
        if (index>=mapValues.size()-width){
            return INT_MAX;
        }
        return index+width;
    
    }
    
    if (index==R){
        if (index%width==width-1){
            return INT_MAX;
        }
        return index+1;
    
    }
    
    if (index==D){
        if (index<width){
            return INT_MAX;
        }
        return index-width;
    }
    
    return INT_MAX;

}

bool Tiles::canMove(int index, int x, int type ){

    if (GDebug::value4==index){
        if (x==R){
        int x =1;
        }
        
    
    }
    
    if (x==L){

        

        if (index%width==0){
            return false;
        }
        
        if (mapValues[index].rw){
            return false;
        }
        
        if (type%2==1){
            if (mapValues[index].theDoorR){
                return false;
            }
        }
        
        
        if (mapValues.size()>index-1+width){
            if (mapValues[index-1+width].theDoorL){
                return false;
            }
        }
        
        if (type<2){
            if (index>0){
                if (mapValues[index-1].block){
                return false;
                }
            }
        }
        
        /*
        if (mapValues.size()>index+width){
            if (mapValues[index+width].lw){
                return false;
            }
        }
        */
        
        return true;
    } else if(x==U){
        
        
        if (index>=mapValues.size()-width-1){
            return false;
        }
        
        
        if (type%2==1){
            if (mapValues[index+width].theDoorL){
                return false;
            }
        }
        
        if (type<2){
            if (mapValues[index+width].block){
                return false;
            }
        }
        
        if (type<2){
            if (mapValues[index+1].theDoorR){
                return false;
            
            }
        }
        
        if (mapValues.size()>index+width){
            if (mapValues[index+width].lw){
                return false;
            }
        }
        
        return true;
    } else if (x==R){
        if (index%width==width-1){
            return false;
        }
        if (mapValues.size()>index-1){
            if (mapValues[index+1].rw){
              return false;
            }
            
            if (type%2==1){
                if (mapValues[index+1].theDoorR){
                    return false;
                }
            }
            
            
        }
        
        if (type<2){
            if (mapValues[index+1].block){
                return false;
            }
        }
        
        if (type<2){
        if (index+width<mapValues.size()){
            if (mapValues[index+width].theDoorL){
                return false;
            }
        }
        }
        /*if (type%2==1){
        if (mapValues.size()>index+width){
            if (mapValues[index+width].lw){
                return false;
            }
        }
        }
        */
        return true;
    } else if (x==D){
        if (index<width){
            return false;
        }
        if (mapValues[index].lw){
            return false;
        }
        
        
        if (type%2==1){
            if (mapValues[index].theDoorL){
                return false;
            }
        }
        
        if (index-width+1>=0){
        
            if (mapValues[index-width+1].theDoorR){
                return false;
            
            }
        }
        
        if (type<2){
            if (mapValues[index+-width].block){
                return false;
            }
        }
        if (mapValues.size()>index+width){
            
        }
        
        return true;
        
    }
    return false;
    
    
    
    
    
}



bool Tiles::canMove(int index, int x){
    return canMove(index, x, 2);

}


cpVect Tiles::findCords(std::pair<int,int> control){

    
    
    
    double multi=10.0;
    
    
    double leftMost = 0;
    double rightMove = leftMost;
    double topMost =0;
    
    for (int counter = 0; counter<control.second; counter++) {
        rightMove=leftMost-=multi;
    
        topMost +=T_V*multi;
        }
    
    for (int counter = 0; counter<control.first; counter++) {
        rightMove+=multi;
        topMost+=T_V*multi;
    }
    
    return cpv(rightMove,topMost);
}


int Tiles::findTile(std::pair<int,int> path){


    return path.first + (path.second*width);
    


}


int Tiles::findTile(int valueOne, int valueTwo){

    return valueOne + valueTwo*width;

}


std::pair<int,int> Tiles::findTile(cpVect control){


    
    cpVect value = translater(control);
    
  
    
    int Yval = (value.y-20)/(40+(21*0.01));
    int Xval = (value.x-20)/(40+(12*0.01));
    
    return std::pair<int, int>(-Xval,-Yval);
    
    
}

std::pair<int,int> Tiles::findTile(int path){
    return std::pair<int,int>(path%width,path/width);
}


void Tiles::findPath(int start, int end){

    PathFinder test;
    
    
    test.findPath(start, end);
    debugPath =test.path;

}

bool Tiles::doesThisTileMakeWalls (int type){
    
    
    return tileDictionary[type]->walls;
    
}




int Tiles::checkWall(int index, bool leftOrRight){

    if (index<0||index>=width*width){
        return false;
    }
    
    int xPos = index%width;
    int yPos = index/width;
    
 

    if (Tiles::mapValues.size()<5){
    
        return false;
    }
    
    if (index<Tiles::width*Tiles::width-Tiles::width-2&&index>Tiles::width+1){
        
        if (mapValues[index].theDoorR||mapValues[index].theDoorL||
            mapValues[index+1].theDoorR||mapValues[index+1].theDoorL||
            mapValues[index-1].theDoorR||mapValues[index-1].theDoorL||
            mapValues[index+width].theDoorR||
            mapValues[index+width].theDoorL||
            mapValues[index+1+width].theDoorR||
            mapValues[index+1+width].theDoorL||
            mapValues[index-1+width].theDoorR||
            mapValues[index-1+width].theDoorL||
            mapValues[index+width*2].theDoorR||
            mapValues[index+width*2].theDoorL||
            mapValues[index+1+width*2].theDoorR||
            mapValues[index+1+width*2].theDoorL||
            mapValues[index-1+width*2].theDoorR||
            mapValues[index-1+width*2].theDoorL
            ){
            
            
            
            return 2;
            
        }
        
        
        
        
        
        
        
    }

    
    
    
    if (xPos>Tiles::width-2){
        return false;
    }
    if (yPos>Tiles::width-2){
        return false;
    }
    
    //TileObject * theTile = &mapValues[index];
    
    if (leftOrRight){
    //right
        int yPosTest = yPos;
        
        while (yPosTest<width-1) {
            yPosTest++;
            
            TileObject * theTile = &mapValues[xPos+yPosTest*width];
            TileObject * theTileO = &mapValues[xPos-1+yPosTest*width];
            
            if (theTile->lw||theTileO->lw){
                break;
            }
       
            
            if (theTile->theDoorR){
                return false;
            
            }
            
            if (!theTile->rw){
                break;
            }
            
      
        }
        
        yPosTest = yPos;
        while (yPosTest>0) {
            yPosTest--;
            
            TileObject * theTile = &mapValues[xPos+yPosTest*width];
            TileObject * theTileO = &mapValues[xPos-1+yPosTest*width];
            
            if (theTile->lw||theTileO->lw){
                break;
            }
            
            if (theTile->theDoorR){
                return false;
                
            }

        
            if (!theTile->rw){
                break;
            }
        
      
            
        }
        
    } else {
    // left
        
        
        int xPosTest = xPos;
        
        while (xPosTest<width-1) {
            xPosTest++;
            
            TileObject * theTile = &mapValues[xPosTest+yPos*width];
            TileObject * theTileO = &mapValues[xPosTest+yPos*width-width];
            

            if (theTile->rw||theTileO->rw){
                break;
            }
         
            if (theTile->theDoorL){

			
                return false;
                
            }
            
            if (!theTile->lw){
                break;
            }
            
         
        }
        
        xPosTest = xPos;
        while (xPosTest>1) {
            xPosTest--;
            
            TileObject * theTile = &mapValues[xPosTest+yPos*width];
            TileObject * theTileO = &mapValues[xPosTest+yPos*width-width];
            
            if (theTile->rw||theTileO->rw){
                break;
            }
            
            if (theTile->theDoorL){
                return false;
                
            }
            
            
            if (!theTile->lw){
                break;
            }

            
        }

        
        
    
    }
    
    
    
    
    
    return 1;
    

}


int Tiles::clearWall(int index, bool create){
	int returnValue = 0;


    {
    TileObject * theTile = &mapValues[index];
    
        if (theTile->rw){
        
            if (create){
            
                if (Door::createThis){
                    
                    int tester = checkWall(index, true);
                    
                    if (tester==1){
                    
                    Door * createdDoor = Door::addDoor(index, Door::createThis->index, false);
                    if (createdDoor){
                    
                        theTile->theDoorR = createdDoor;
                        theTile->rw = false;
						returnValue = 2;


                    }
                    } else if (tester==0) {
                        returnValue=99;
                   
                    
                    } else if (tester==2){
                    
                        returnValue=199;
                    
                    }
                }
            }
            
            hoverWall = (index);
            hoverWallSide = 2;
            
			return returnValue;
        }else if (theTile->theDoorR){
            if (create){
                theTile->rw = true;
                
                
                if (index>1){
					TileObject * checkTile = &mapValues[index - 1];
                    
                    theTile->textureWallR = Tiles::tileDictVector[checkTile->type]->textureR;
                }
                
                Door::removeDoor(index, false);
                theTile->theDoorR = 0;
                theTile->thinkValue = Sprite::generateUID();
   
                hoverWallSide = 0;
				returnValue = 1;
				return returnValue;
            
            
            }
            hoverWall = (index);
            hoverWallSide = 4;
        
			return returnValue;
        } else {
        
            
        }
    
    
    }
    {
    
        TileObject * theTile = &mapValues[index];
        
        if (theTile->lw){
            
            if (create){
                
                if (Door::createThis){
                    int tester = checkWall(index, false);
                    if (tester==1){
                    Door * createdDoor = Door::addDoor(index, Door::createThis->index, true);
                    if (createdDoor){
                        
                        theTile->theDoorL = createdDoor;
                        theTile->lw = false;
						returnValue = 2;
                    }
                    } else if (tester==0) {
                        returnValue=99;
                        
                        
                    } else if (tester==2){
                        
                        returnValue=199;
                        
                    }
                }
            }
            
            hoverWall = (index);
            hoverWallSide = 1;
            
			return returnValue;
        }else if (theTile->theDoorL){
            
            if (create){
                theTile->lw = true;
                
                if (index>width){
                    
                TileObject * checkTile = &mapValues[index-width];
                theTile->textureWallL = Tiles::tileDictVector[checkTile->type]->textureL;
                
                }
                
                
                Door::removeDoor(index, true);
                theTile->thinkValue = Sprite::generateUID();
				theTile->theDoorL = 0;
                hoverWallSide = 0;
				returnValue = 1;
				return returnValue;
                
            }
            
            hoverWall = (index);
            hoverWallSide = 3;

			return returnValue;
        } else {
            
            
        }
    
    }
    
    
    
    
    

    hoverWall = (index);
    hoverWallSide = 0;
    
  
    
    
	return returnValue;
}


int Tiles::clearWall(int index, MOUSE_STATES input){
    
	if (input == MOUSE_STATE_UP){
		int a = 1;
	}
    
    if (mapValues.size()>index+width){
    TileObject * theTile = &mapValues[index+width];
        if (theTile->lw){
            
            if (input==MOUSE_STATE_UP){
                theTile->lw=false;
                if (Door::createThis){
                    Door * createdDoor = Door::addDoor(index+width, Door::createThis->index, true);
                
                    if (createdDoor){
                
                        theTile->theDoorL = createdDoor;
                        
                    }
                }
            }
            
   
            hoverWall = (index+width);
            hoverWallSide = 1;
      
            
            return (index+width)*2;
        } else {
            if (theTile->theDoorL){
                if (input==MOUSE_STATE_UP){
             
                
                
                    Door::removeDoor(index+width, true);
                    theTile->theDoorL = 0;
                    theTile->lw= true;
                    mapValues[index].thinkValue = Sprite::generateUID();
                    
                    
                    
                    TileObject * aTile =  &mapValues[index];
                    
         
                    if(
                       doesThisTileMakeWalls(aTile->type)
                       ){
                        
                        theTile->textureWallL = Tiles::tileDictVector[aTile->type]->textureL;
                        
                    }else {
                        
                        theTile->textureWallL = Tiles::tileDictVector[1]->textureL;
                    }
                    
                    hoverWall = (index+width);
                    hoverWallSide = 3;
               
                    return (index+width)*2;
                
                }
                
                hoverWall = (index);
                hoverWallSide = -1;
                //NOT WORKING
                return INT_MAX;
                
            }
        
        }
        
    }
    
    if (mapValues.size()>index){
        TileObject * theTile = &mapValues[index];
        if (theTile->lw){
            if (input==MOUSE_STATE_UP){
                theTile->lw=false;
                if (Door::createThis){
                    Door * createdDoor = Door::addDoor(index, Door::createThis->index, true);
                
                if (createdDoor){
                    
                    theTile->theDoorL = createdDoor;
                }
                }
            }
            
            hoverWall = (index);
            hoverWallSide = 1;
    //               printf("ROAR");
    
            return (index)*2;
        } else {
            if (theTile->theDoorL){
                if (input==MOUSE_STATE_UP){
                
                
                    
                    Door::removeDoor(index, true);
                    theTile->theDoorL = 0;
                    theTile->lw= true;
                    mapValues[index].thinkValue = Sprite::generateUID();
                    
                    if (mapValues.size()>index-width){
                    
                        TileObject * aTile =  &mapValues[index-width];
                        
                        if(
                        doesThisTileMakeWalls(aTile->type)
                           ){
                        
                            theTile->textureWallL = Tiles::tileDictVector[aTile->type]->textureL;
                            
                        }else {
                        
                              theTile->textureWallL = Tiles::tileDictVector[1]->textureL;
                        }
                        
                        
                        
                    }else {
                  
                    theTile->textureWallL = Tiles::tileDictVector[1]->textureL;
                    }
                    
                    hoverWall = (index+width);
                    hoverWallSide = 3;
                    return (index+width)*2;
                    
                }
                hoverWall = (index+width);
                hoverWallSide = 3;
           
                return INT_MAX;
                
            }
            
        }

    
    }
    
    if (mapValues.size()>index){
        TileObject * theTile = &mapValues[index];
        if (theTile->rw){
            if (input==MOUSE_STATE_UP){
                theTile->rw=false;
                Door * createdDoor=0;
                if (Door::createThis){
                createdDoor =   Door::addDoor(index, Door::createThis->index, false);
                }
                if (createdDoor){
                    theTile->theDoorR= createdDoor;
                
                }
            }
            hoverWall = (index);
            hoverWallSide = 2;
            
            return ((index)*2)+1;
        } else {
            if (theTile->theDoorR){
                if (input==MOUSE_STATE_UP){
                
                
                    
                    Door::removeDoor(index, false);
                    theTile->theDoorR = 0;
                    theTile->rw= true;
                    mapValues[index].thinkValue = Sprite::generateUID();
            
                    
                     TileObject * aTile =  &mapValues[index-1];
                    if (doesThisTileMakeWalls(aTile->type)){
                    
                      theTile->textureWallR = Tiles::tileDictVector[aTile->type]->textureR;
                    } else {
                        theTile->textureWallR = Tiles::tileDictVector[1]->textureR;
                    }
                    
                    hoverWall = (index+width);
                    hoverWallSide = 4;
                    return (index)*2+1;
                    
                }
                hoverWall = (index+width);
                hoverWallSide = 4;
                
            }
        
        
        
        }
        
    }
    
    if (mapValues.size()>index+1){
        TileObject * theTile = &mapValues[index+1];
        if (theTile->rw){
            if (input==MOUSE_STATE_UP){
                theTile->rw=false;
                
                if (Door::createThis){
                    theTile->theDoorR = Door::addDoor(index+1, Door::createThis->index, false);
                }
            }
            
            hoverWall = (index+1);
            hoverWallSide = 2;
            
            return ((index+1)*2)+1;
        } else {
            if (theTile->theDoorR){
                if (input==MOUSE_STATE_UP){
                
                
                    
                    Door::removeDoor(index+1, false);
                    theTile->theDoorR = 0;
                    theTile->rw= true;
                    mapValues[index+1].thinkValue = Sprite::generateUID();
                    
                    
                    {
                        if (mapValues.size()>index-1){
                            
                            TileObject * aTile =  &mapValues[index-1];
                            
                            if(
                               doesThisTileMakeWalls(aTile->type)
                               ){
                                
                                theTile->textureWallR = Tiles::tileDictVector[aTile->type]->textureR;
                                
                            }else {
                                
                                theTile->textureWallR = Tiles::tileDictVector[1]->textureR;
                            }
                            
                            
                            
                        } else {
                            
                            theTile->textureWallR = Tiles::tileDictVector[1]->textureR;
                            
                        }
                    
                    }
                    
                    hoverWall = (index+1);
                    hoverWallSide = 4;
          
                    return (index+1)*2+1;
                    
                }
                hoverWall = (index+1);
                hoverWallSide = 4;
                
            }
        
        
        
        }
        
    }
    
    
   
    hoverWallSide = -1;
    return INT_MAX;
}


void Tiles::fillTilesDebug(int leftStart, int upStart, int rightStart, int downStart, size_t type){

    for (int counterY =upStart; counterY <downStart+1; counterY++){
        if (counterY>=0){
            int Yvalue = ((width)*(counterY));
            for (int counterX =leftStart; counterX < rightStart+1; counterX++) {
                
                int Xvalue = counterX;
                if (Xvalue<0||Yvalue<0){
                    continue;
                }
                
                Tiles::mapValues[Yvalue+Xvalue].type =type;
                Tiles::mapValues[Yvalue+Xvalue].lw = true;
                Tiles::mapValues[Yvalue+Xvalue].textureWallL = Tiles::tileDictVector[type]->textureL;
                
                Tiles::mapValues[Yvalue+Xvalue].rw = true;
                Tiles::mapValues[Yvalue+Xvalue].textureWallR = Tiles::tileDictVector[type]->textureR;
                
            }
        }
    }



}


void Tiles::changeTile(int index, size_t type, long idValue){

    std::cout << "Don't Use This";

    
  }



void Tiles::fillTiles(int leftStart, int upStart, int rightStart, int downStart, size_t type){
    
    long ID = Sprite::generateUID();
    
    
    if (leftStart<0){
        leftStart = 0;
    
    }
    
    if (upStart<0){
        upStart = 0;
    }
    
    if (rightStart>width-1){
        rightStart=width-1;
    }
    
    if (downStart>height-1){
        downStart=height-1;
    }
    
    
    
    std::set<Sprite*> killThese;
    
    for (int counterY =upStart; counterY <downStart+1; counterY++){
        if (counterY>=0){
            int Yvalue = ((width)*(counterY));
            for (int counterX =leftStart; counterX < rightStart+1; counterX++) {
                
                int Xvalue = counterX;
                if (Xvalue<0||Yvalue<0){
                    continue;
                }
                
                TileObject * theObject = &Tiles::mapValues[Yvalue+Xvalue];
                
                theObject->thinkValue = ID;
                theObject->type =type;
                theObject->canWalkThere = false;
                
                
                if (theObject->lw){
                
                    if (counterY==upStart){
                        theObject->lw = true;
                    } else {
                
                    theObject->lw = false;
                
                    }
                }
                
                if (theObject->rw){
                    if (counterX==leftStart){
                        theObject->rw = true;
                    } else {

                        theObject->rw = false;
                        
                    }
                }
                
                
                
                Door * doorL = theObject->theDoorL;
                Door * doorR = theObject->theDoorR;
                
                
                
                
                
                
                
                if (doorL){
                    if (counterY==upStart){
                        
                    }else {
                        theObject->theDoorL = 0;
                        Door::removeDoor(Yvalue+Xvalue, true);
                    }
                }
                
                if (doorR){
                    if (counterX==leftStart){
                    
                    }else {
                        theObject->theDoorR = 0;
                        Door::removeDoor(Yvalue+Xvalue, false);
                    }
                    
                }
    
                
                
                if (theObject->occupiedSprite){
                    if (Sprite::trees.find(theObject->occupiedSprite->dict->index)!=Sprite::trees.end()){
                    
                        killThese.insert(theObject->occupiedSprite);
                        theObject->occupiedSprite=0;
                    
                    } else if (theObject->occupiedSprite->dict->placementType!=OneXOne&&
                               theObject->occupiedSprite->dict->placementType!=OneXOneLeftWall){
                        Sprite * aSprite = theObject->occupiedSprite;
                        
                        std::pair<int, int> dim = Sprite::dimensionsForBlockType(aSprite->dict->placementType);
                        
                        int xi = aSprite->renderGridIndex%Tiles::width;
                        int yi = aSprite->renderGridIndex/Tiles::width;
                        
                        if (xi<leftStart||xi+dim.first>rightStart||yi+dim.second>downStart||yi<upStart){
                            killThese.insert(theObject->occupiedSprite);
                            theObject->occupiedSprite=0;
                        }
                    }
                }
                theObject->block=false;
                
                
         
                
                if (type ==0 ||type ==1){
                    
                    if (Tiles::mapValues.size()>Yvalue+Xvalue+1){
                        TileObject * rightTile = &Tiles::mapValues[Yvalue+Xvalue+1];
                        if (rightTile->type==0||rightTile->type==1){
                            rightTile->rw = false;
                            
                            if (rightTile->theDoorR){
                                Door::removeDoor(Yvalue+Xvalue+1, true);
                                rightTile->theDoorR=0;
                                
                            }
                        }
                        
                    }
                    if (Tiles::mapValues.size()>Yvalue+Xvalue+width){
                        TileObject * topTile = &Tiles::mapValues[Yvalue+Xvalue+width];
                        if (topTile->type==0||topTile->type==1){
                            topTile->lw=false;
                            
                            if (topTile->theDoorL){
                                Door::removeDoor(Yvalue+Xvalue+width, false);
                                topTile->theDoorL=0;
                            }
                        }
                    }
                }
            }
        }
    }
    
    /*
    
    if (type<3){
        if (leftStart!=0){
            int xPos = leftStart-1;
            
            for (int counterY =upStart; counterY <downStart+1; counterY++){
                int Yvalue = ((width)*(counterY));
                TileObject * theTile = &Tiles::mapValues[xPos+Yvalue];
                //theTile->type=7;
                if (theTile->type<3){
                    
                    TileObject * change =&Tiles::mapValues[xPos+Yvalue+1];
                    
                    
                    if (change->rw){
                        change->rw = false;
                    }
                    if (change->theDoorR){
                        
                        change->theDoorR = 0;
                        Door::removeDoor(xPos+Yvalue+1, true);
                        
                    }
                    
                    
                    
                }
                
                
            }
        }
        if (downStart!=0){
            int Yvalue = ((width)*(upStart));
            for (int counterX =leftStart; counterX < rightStart+1; counterX++) {
        
                TileObject * theTile = &Tiles::mapValues[counterX+Yvalue];
                if (theTile->type<3){
                    
                    
               
                    if (theTile->lw){
                        theTile->lw = false;
                        //theTile->type=7;
                    }
                    if (theTile->theDoorL){
                        
                        theTile->theDoorL = 0;
                        Door::removeDoor(counterX+Yvalue, false);
                        
                    }
                    
                }
            }
        }
    }
   
     */
    
    
    for (std::set<Sprite*>::iterator IT = killThese.begin();
         IT != killThese.end();
         IT++){
        
     
        
        if ((*IT)->dict->prop){
        
            GameLogic::energy +=(*IT)->dict->cost/2.0;;
            
        
        }
        
  
        Sprite::removeSprite(*IT);
    
    }
    
    
    
    hasBeenCheckedForWalkability=false;
    
    
   
    
    
    
}


std::vector<int> Tiles::getAllTilesInRoom(int index){

    std::cout << "Don't Run too slow";
    return std::vector<int>();
    
    
}


int pastValue = -1;


std::vector<int> Tiles::getSizeOrRoom(int _index){

    if (_index<0||_index>=mapValues.size()){

        return std::vector<int>();
    }
    

    quickSearchSetup();
    
    
    short testType = mapValues[_index].type;
    std::vector<int> testedTiles;
    std::vector<int> testThese;
    
    testThese.push_back(_index);

    
    int left=999999, right=-1, up=-1,down=999999;
    
    int numberOfTrap = 0;
    
    
    //Player::mainPlayer->path.clear();
    
    while (testThese.size()) {
        
        int index = testThese.back();
        testThese.pop_back();
        
        int xPos = index%width;
        int yPos = index/width;
        
        if (left>xPos){
            left=xPos;
        }
        
        if (right<xPos){
            right = xPos;
        }
        
        if (up<yPos){
            up=yPos;
        }
        
        if (down>yPos){
            down=yPos;
        }
        
      
        std::vector<Sprite*> *checkThis = &mapValues[index].renderGrid;
        if (checkThis){
            for (std::vector<Sprite*>::iterator IT = checkThis->begin();
                 IT != checkThis->end();
                 IT++){
             
                if(
                (*IT)->dict->prop&&(*IT)->dict->animates
                   ){
                 numberOfTrap++;
                }
            
            
            }
            
            
            
        }
        
        
        
        if (Tiles::canMove(index, L, 3)){
            
            if (mapValues[index-1].type==testType){
                if (quickSearch[index-1]!=t_check){
                    quickSearch[index-1]=t_check;
                    testThese.push_back(index-1);
                }
            }
        }
        if (Tiles::canMove(index, U,3)){
            if (mapValues[index+width].type==testType){
                if (quickSearch[index+width]!=t_check){
                    quickSearch[index+width]=t_check;
                    testThese.push_back(index+width);
                }
            }
            
        }
        if (Tiles::canMove(index, R,3)){
            if (mapValues[index+1].type==testType){
                if (quickSearch[index+1]!=t_check){
                    quickSearch[index+1]=t_check;
                    testThese.push_back(index+1);
                }
            }
            
        }
        if(Tiles::canMove(index, D,3)){
            if (mapValues[index-width].type==testType){
                if (quickSearch[index-width]!=t_check){
                    quickSearch[index-width]=t_check;
                    testThese.push_back(index-width);
                }
            }
        }
        quickSearch[index]=t_check;
        testedTiles.push_back(index);
   
        //Player::mainPlayer->path.push_back(index);
        
    }
    
    std::sort(testedTiles.begin(), testedTiles.end());
    testedTiles.erase(std::unique(testedTiles.begin(), testedTiles.end()), testedTiles.end());
    
   

    
    std::vector<int> sizes;
    
    sizes.push_back((int)testedTiles.size());
    sizes.push_back(right-left+1);
    sizes.push_back(up-down+1);
    sizes.push_back(left);
    sizes.push_back(up);
    sizes.push_back(right);
    sizes.push_back(down);
    sizes.push_back(numberOfTrap);
    
    
    pastValue = _index;
    
    
    
    return sizes;
    
    
}


void Tiles::fillTilesMakeWalls(int leftStart, int upStart, int rightStart, int downStart, size_t tileType){
    
    fillTiles(leftStart, upStart, rightStart, downStart, tileType);
    
    std::vector<std::pair<size_t, int>> wallsHold;
  
    
    if (tileType==0||tileType==1||tileType==2){
        
        int high = (upStart>downStart)?upStart:downStart;
        int low = (upStart<downStart)?upStart:downStart;
        
        int left = (leftStart<rightStart)?leftStart:rightStart;
        int right = (leftStart>rightStart)?leftStart:rightStart;

        
        if (tileType==2){
            for (int counterX = left; counterX<=right; counterX++){
                for (int counterY = low; counterY<=high; counterY++){
                
                    TileObject * tile = &mapValues[counterX+counterY*Tiles::width];
                    if (tile->occupiedSprite){
                        GameLogic::energy +=tile->occupiedSprite->dict->cost/2;
                    Sprite::removeSprite(tile->occupiedSprite);
                    }
                    
                
                }
            }
        }
        
        
        {
            
            
            
            int xPos1 = left-1;
            int xPos2 = right+1;
            int xPos3 = left;
            int xPos4 = right;
            
            
         
        
            
            
            for (int counter = low; counter<high+1; counter++){
                int yPos1 = counter*width;
                
                TileObject * theLeftC = &mapValues[xPos1+yPos1];
                TileObject * theLeft = &mapValues[xPos3+yPos1];
                TileObject * theRightC = &mapValues[xPos2+yPos1];
                TileObject * theRight = &mapValues[xPos4+yPos1];
                
                if (doesThisTileMakeWalls(theLeftC->type)){
                    if (!theLeft->theDoorR){
                    theLeft->rw = true;
                    
                    theLeft->textureWallR = Tiles::tileDictVector[theLeftC->type]->textureR;;
                    }
                }
                
                
                if (doesThisTileMakeWalls(theRightC->type)){
                    if (doesThisTileMakeWalls(theRight->type)){
                        if (!theRight->theDoorR){
                        theRightC->rw = true;
                        theRightC->textureWallR = Tiles::tileDictVector[theRightC->type]->textureR;;
                        }
                    } else {
                        if (!theRight->theDoorR){
                        theRightC->rw = true;
                        theRightC->textureWallR = Tiles::tileDictVector[1]->textureR;
                        }

                    
                    }
                }
            }
            
        }
        
     
        
        
        
        {
            int yPos1 = high*width;
            int yPos2 = (low-1)*width;
            
            
            for (int counter = left; counter<right+1; counter++){
                int xPos1 = counter;
            
                
                TileObject * theTop = &mapValues[xPos1+yPos1+width];
                TileObject * theTopC = &mapValues[xPos1+yPos1];
                TileObject * theBot = &mapValues[xPos1+yPos2+width];
                TileObject * theBotC = &mapValues[xPos1+yPos2];
                
                if (doesThisTileMakeWalls(theTop->type)){
                    
                    if (doesThisTileMakeWalls(theTopC->type)){
                        if (!theTop->theDoorL){
                        theTop->lw = true;
                        theTop->textureWallL = Tiles::tileDictVector[theTop->type]->textureL;
                        }
     
                    } else {
                        if (!theTop->theDoorL){
                        theTop->lw = true;
                        theTop->textureWallL = Tiles::tileDictVector[1]->textureL;;
                        }

                    }
                
                }
                
                if (doesThisTileMakeWalls(theBotC->type)){
                    if (!theBot->theDoorL){
                    theBot->lw = true;
                    theBot->textureWallL = Tiles::tileDictVector[theBotC->type]->textureL;
                    }
                }
            }
        }
        
        killLostWalls(leftStart,upStart,rightStart,downStart);
        return;
    }
    

	if (true){
	
		
	
			for (int counter = upStart; counter <= downStart; counter++){
			
				int rightCheck = counter*Tiles::width + rightStart;
			
				while (true){
					if (mapValues[rightCheck].type == tileType){
						if (mapValues[rightCheck + 1].rw || mapValues[rightCheck + 1].theDoorR){
							mapValues[rightCheck + 1].textureWallR = tileDictVector[tileType]->textureR;
							break;
					
						}
						rightCheck++;
				}
				else {
					mapValues[rightCheck].rw = true;		
					mapValues[rightCheck].textureWallR = tileDictVector[tileType]->textureR;					
					break;
				}

			
			}
				int leftCheck = counter*Tiles::width + leftStart;
			
			while (true){
				if (mapValues[leftCheck].type == tileType){				
					if (mapValues[leftCheck].rw || mapValues[leftCheck].theDoorR){

						if (doesThisTileMakeWalls(mapValues[leftCheck - 1].type)){

							mapValues[leftCheck].textureWallL = tileDictVector[mapValues[leftCheck - 1].type]->textureL;

						}
						else {

							mapValues[leftCheck].textureWallL = tileDictVector[1]->textureL;

						}
			
				
						break;
					}				
					leftCheck--;
				}
				else {

					short checkThis = mapValues[leftCheck].type;

					if (doesThisTileMakeWalls(checkThis)){

						mapValues[leftCheck + 1].rw = true;
						mapValues[leftCheck + 1].textureWallR = tileDictVector[tileType]->textureR;


					}
					else {
					
						mapValues[leftCheck + 1].rw = true;
						mapValues[leftCheck + 1].textureWallR = tileDictVector[1]->textureR;
					
					
					}
					break;
				}
			}
		

			
			for (int counter = leftStart; counter <= rightStart; counter++){


				int downCheck = counter + upStart*Tiles::width;

				while (true){
					if (mapValues[downCheck].type == tileType){
						if (mapValues[downCheck].lw || mapValues[downCheck].theDoorL){

							if (doesThisTileMakeWalls(mapValues[downCheck-Tiles::width].type)){
							
								mapValues[downCheck].textureWallL = tileDictVector[mapValues[downCheck - Tiles::width].type]->textureL;
							
							}
							else {
							
								mapValues[downCheck].textureWallL = tileDictVector[1]->textureL;
							
							}


					
							break;

						}
						downCheck -= Tiles::width;
					}
					else {

						short checkThis = mapValues[downCheck].type;

						if (doesThisTileMakeWalls(checkThis)){
						
							mapValues[downCheck + Tiles::width].lw = true;
							mapValues[downCheck + Tiles::width].textureWallL = tileDictVector[checkThis]->textureL;
						}
						else {

							mapValues[downCheck + Tiles::width].lw = true;
							mapValues[downCheck + Tiles::width].textureWallL = tileDictVector[1]->textureL;
						
						}


						break;
					}


				}
				int upCheck = counter + downStart*Tiles::width;

				while (true){
					if (mapValues[upCheck].type == tileType){
						if (mapValues[upCheck].lw || mapValues[upCheck].theDoorL){
						
								mapValues[upCheck].textureWallL = tileDictVector[tileType]->textureL;
							
							break;
						}
						upCheck += Tiles::width;
					}
					else {
						if (!mapValues[upCheck].theDoorL){
							mapValues[upCheck].lw = true;
							mapValues[upCheck].textureWallL = tileDictVector[tileType]->textureL;
						}
						break;
					}
				}



			}

	}

	

			killLostWalls(leftStart, upStart, rightStart, downStart);
		return;
	}

/*
	findEdge(
		&wallsHold,
		width,
		tileType,
		leftStart + (upStart*(width))
		);
*/
	



    for (std::vector<std::pair<size_t, int>>::iterator IT = wallsHold.begin(); IT != wallsHold.end(); IT++){
        
        if(doesThisTileMakeWalls((int)tileType)){
            
            switch (IT->second) {
                case L:
                    if (mapValues[IT->first+1].theDoorR){
                        continue;
                    
                    }
                    
                    
                    if(mapValues[IT->first].type==0){
                        mapValues[IT->first+1].rw = true;
                        mapValues[IT->first+1].textureWallR= Tiles::tileDictVector[1]->textureR;
                        
                    } else {
                        mapValues[IT->first+1].rw = true;
                        mapValues[IT->first+1].textureWallR= tileDictVector[mapValues[IT->first].type]->textureR;

                    }
                    
                    break;
                case U:
                    
                    if (mapValues[IT->first].theDoorL){
                        continue;
                        
                    }
                    
                    
                    mapValues[IT->first].lw = true;
                    mapValues[IT->first].textureWallL = tileDictVector[tileType]->textureL;
                    
                    
                    
                    
                    
                    
                    break;
                case R:
                    
                    if (mapValues[IT->first].theDoorR){
                        continue;
                        
                    }
                    
                    
                    mapValues[IT->first].rw = true;
                    mapValues[IT->first].textureWallR = tileDictVector[tileType]->textureR;
                    
                    break;
                case D:
                
                    if (mapValues[IT->first+width].theDoorL){
                        continue;
                        
                    }
                    
                    
                    if (mapValues[IT->first].type==0){
                        mapValues[IT->first+width].lw = true;
                        mapValues[IT->first+width].textureWallL=tileDictVector[1]->textureL;
                    } else {
                        mapValues[IT->first+width].lw = true;
                        mapValues[IT->first+width].textureWallL= tileDictVector[mapValues[IT->first].type]->textureL;
                    }
                    
                    break;
                default:
                    break;
            }
        }
    }

	if (!(rightStart + 1 >= Tiles::width)){
	for (int counter = upStart; counter <= downStart; counter++){
		int value = counter*Tiles::width + rightStart;
		
		
		if (mapValues[counter*Tiles::width + rightStart + 1].rw){

			mapValues[counter*Tiles::width + rightStart + 1].textureWallR = tileDictVector[tileType]->textureR;

		}
	}
	}

	if (!(downStart + 1 >= Tiles::width)){
	for (int counter = leftStart; counter <= rightStart; counter++){
		int value = downStart*Tiles::width + counter;

		
		if (mapValues[downStart*Tiles::width + counter + Tiles::width].lw){

			mapValues[downStart*Tiles::width + counter + Tiles::width].textureWallL = tileDictVector[tileType]->textureL;

			}
		}
	}

	



    
    
    std::set<Sprite*> killThese;
    
        if (downStart!=Tiles::width-1){
            
            for (int counterY =downStart+1; counterY <downStart+2; counterY++){
                if (counterY>=0){
                    int Yvalue = ((width)*(counterY));
                    for (int counterX =leftStart; counterX < rightStart+1; counterX++) {
                        
                        int Xvalue = counterX;
                        
                        TileObject * theObject = &Tiles::mapValues[Yvalue+Xvalue];
                        
                        if (theObject->occupiedSprite){
                            if (theObject->occupiedSprite->checkIfLocationIsInvalid()){
                                killThese.insert(theObject->occupiedSprite);
                                
                            }
                        }
                        
                    }
                }
            }
        }
    for (std::set<Sprite*>::iterator IT = killThese.begin();
         IT != killThese.end();
         IT++){
        
        
        
        if ((*IT)->dict->prop){
            
            GameLogic::energy +=(*IT)->dict->cost/2.0;;
            
            
        }
        
        
        Sprite::removeSprite(*IT);
        
    }

    
    
     killLostWalls(leftStart,upStart,rightStart,downStart);
    
 
}

int Tiles::checkIfEdge(int edge, bool * returner){

    bool saySomething = false;
    if (edge%Tiles::width == 0){
        edge += 1;
        saySomething = true;
    }
    if (edge%Tiles::width == Tiles::width - 1){
        edge -= 1;
        saySomething = true;
    }
    
    
    
    if (edge / Tiles::width == 0){
        
        edge += Tiles::width;
        saySomething = true;
    }
    if (edge / Tiles::width == Tiles::width - 1){
        edge -= Tiles::width;
        saySomething = true;
    }
    
    if (saySomething == true){
        *returner=saySomething;
        UserInterface::spawnTextBox(TranslationMatrix::translate("YOU_CAN'T_PLACE_TILES_HERE"), TEXTBOX_STAY_UP);
        
    }
    
    return edge;
    

}

int Tiles::checkIfEdge(int edge){
    bool useless;
    
    return checkIfEdge(edge,&useless);
	

}


bool Tiles::doorMouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber){

    std::pair<int,int> tilePos = findTile(cpvadd(control,cpv(-Camera::posX,-Camera::posY)));
    int index = findTile(tilePos);
    
    //Odd Values are for Right Walls
    
    if (index<0||index>=Tiles::width*Tiles::width){
    
        return false;
    }
    
    int value;
    
    int xPos = index%Tiles::width;
    int yPos = index/Tiles::width;
    
    if (xPos==0||xPos>=Tiles::width-2){
        if (yPos==0||yPos>=Tiles::width-2){
        
            return false;
        }
    
    }
    
    
    if (input==MOUSE_STATE_UP){
        value = Tiles::clearWall(index, true);
    } else {
        value = Tiles::clearWall(index, false);
    }
   
    
    
    
    
    //int value = clearWall(index, input);
    
    if (value>0){
        
        
        
		if (value == 99){
			UserInterface::spawnTextBox(TranslationMatrix::translate("YOU_MAY_ONLY_PLACE_ONE_DOOR"), TEXTBOX_STAY_UP);
		
        } else if (value==199){
        
            UserInterface::spawnTextBox(TranslationMatrix::translate("PLEASE_DON'T_PLACE_DOORS"), TEXTBOX_STAY_UP);
        
        }
         AudioController::playAudio("Interface_Accept.wav");
        Tiles::hasBeenCheckedForWalkability=false;
        Tiles::checkWalkAbility();
       
    }




    return true;
}


int Tiles::mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
    std::pair<int,int> tilePos = findTile(cpvadd(control,cpv(-Camera::posX,-Camera::posY)));

	tilePos.first = (tilePos.first < 0) ? 0 : tilePos.first;
	tilePos.second = (tilePos.second < 0) ? 0 : tilePos.second;

	tilePos.first = (tilePos.first > Tiles::width - 1) ? Tiles::width - 1 : tilePos.first;
	tilePos.second = (tilePos.second > Tiles::width - 1) ? Tiles::width - 1 : tilePos.second;

    int index = findTile(tilePos);
    
    hoverTile = index;
    
    
    if (index<0){
        // Don't Display the Hover Tile
        hoverTile=-1;
        return 0;
    }
    

    
    if (/* DISABLES CODE */ (false)){
        
        
        bool goL = (canMove(hoverTile, L))?true:false;
        bool goR = (canMove(hoverTile, R))?true:false;
        bool goU = (canMove(hoverTile, U))?true:false;
        bool goD = (canMove(hoverTile, D))?true:false;
       
        std::string theText;
        
        if (goL){
            theText.append("Left=True,");
        } else {
            theText.append("Left=False,");
        }
        
        if (goR){
            theText.append("Right=True,");
        } else {
            theText.append("Right=False,");
        }
        
        if (goU){
            theText.append("Up=True,");
        } else {
            theText.append("Up=False,");
        }
        
        if (goD){
            theText.append("Down=True,");
        } else {
            theText.append("Down=False,");
        }
        
    
    }
    
    
    if (input==MOUSE_STATE_TOUCH){
       
        index = checkIfEdge(index);

		
        
        if (changeTileType == 0 || changeTileType ==1){
            startIndex=index;
            lastIndex=index;
			UserInterface::spawnTextBox(TranslationMatrix::translate("GRASS_AND_PATH_ARE_FREE"), TEXTBOX_STAY_UP);

            return 0;
        
        }
        
        if (GameLogic::numberOfTilesInPlay!=0){
            if (mapValues[index-1].type>2||
                mapValues[index+1].type>2||
                mapValues[index+Tiles::width].type>2||
                mapValues[index-Tiles::width].type>2){
                
                startIndex=index;
                lastIndex=index;
                
                if (GameLogic::numberOfTilesInPlay!=0){
                    UserInterface::spawnTextBox(TranslationMatrix::translate("YOU_CAN_MAKE_A_NEW_HOUSE"), TEXTBOX_STAY_UP);
                }
                
                return 0;
            }
        }
        
        UserInterface::spawnTextBox(TranslationMatrix::translate("ONLY_PLACE_TILES_NEXT_TO_A_HOUSE"), TEXTBOX_STAY_UP);
        

        
        
        startIndex=-1;
        lastIndex=-1;
        
    }else if (input==MOUSE_STATE_DRAG){
        
        bool value = false;
        index = checkIfEdge(index,&value);
        
        lastIndex=index;
        
        if (startIndex!=-1){
            
			

			
        
            int whatIsAlreadyUsed = energyUsed(startIndex, lastIndex);
            
            int x1 = startIndex%width;
            int x2 = lastIndex%width;
            
            int y1 = startIndex/width;
            int y2 = lastIndex/width;



            
            if (changeTileType == 0 || changeTileType ==1) {
                    overInfluence=false;
                if (!value){
                UserInterface::spawnTextBox(TranslationMatrix::translate("GRASS_AND_PATH_ARE_FREE"), TEXTBOX_STAY_UP);
                } else {
                UserInterface::spawnTextBox(TranslationMatrix::translate("YOU_CAN'T_PLACE_TILES_HERE"), TEXTBOX_STAY_UP);
                }
                    
                return 0;
            } if (abs(x1-x2)<1||abs(y1-y2)<1){
                if (!(changeTileType==0||changeTileType==1)){
                    overInfluence=true;
                    UserInterface::spawnTextBox(TranslationMatrix::translate("THAT_ROOM_IS_TO_SMALL"), TEXTBOX_STAY_UP);
                    return 0;
                }
            
            }else {
                
                if (whatIsAlreadyUsed>GameLogic::energy){
                    UserInterface::spawnTextBox(TranslationMatrix::translate("NOT_ENOUGH_ENERGY"), TEXTBOX_STAY_UP);
                    overInfluence = true;
                } else {
            
                    std::string theRoom = "The Room is ";
                    char dumpText[400];
                    if (!value){
                    sprintf(dumpText, TranslationMatrix::translate("THE_ROOM_IS_X_BY_X").c_str(), abs(x1-x2)+1,abs(y1-y2)+1, whatIsAlreadyUsed);
                    UserInterface::spawnTextBox(dumpText, TEXTBOX_STAY_UP);
                    } else {
                    UserInterface::spawnTextBox(TranslationMatrix::translate("YOU_CAN'T_PLACE_TILES_HERE"), TEXTBOX_STAY_UP);
                    }
                    overInfluence = false;
                    return 0;
                }
            }
        }
        
        
        
    } else if (input==MOUSE_STATE_UP){
        index = checkIfEdge(index);
        
        
        // Fill the area
        if (startIndex!=-1&&lastIndex!=-1){
            
            
            
            int whatIsAlreadyUsed = energyUsed(startIndex, lastIndex);
            
            int x1 = startIndex%width;
            int x2 = lastIndex%width;
            
            int y1 = startIndex/width;
            int y2 = lastIndex/width;

		

            
            // Check if the room is to small
            if (Tiles::doesThisTileMakeWalls((int)changeTileType)){
                if (abs(x1-x2)<1||abs(y1-y2)<1){
                
                    startIndex=-1;
                    lastIndex=-1;
                    
                    TranslationMatrix::warning(THAT_ROOM_IS_TO_SMALL);
                
                    return true;

                
                }
            }
            
            // Check if the player has enough energy
            if (whatIsAlreadyUsed>GameLogic::energy){
                
                startIndex=-1;
                lastIndex=-1;
                
                TranslationMatrix::warning(NOT_ENOUGH_ENERGY);
                    return true;
            }
            
            
        
            
            
            
            
            int leftStart = startIndex%(width);
            int rightStart = lastIndex%(width);
            
            if (leftStart>rightStart){
                int holder = leftStart;
                leftStart = rightStart;
                rightStart=holder;
                
            }
            
            int downStart = startIndex/(width);
            int upStart = lastIndex/(width);
            
            if (downStart<upStart){
                int holder = downStart;
                downStart = upStart;
                upStart=holder;
                
            }
            
            fillTilesMakeWalls(leftStart, upStart, rightStart, downStart, changeTileType);
            GameLogic::energy = GameLogic::energy - whatIsAlreadyUsed; 
            
            
            UserInterface::despawnTextBox();
            Tiles::checkWalkAbility();
            
            startIndex=-1;
            lastIndex=-1;
            
            return 2;
            
            
        }
        startIndex=-1;
        lastIndex=-1;
    }
    return 0;
}


bool Tiles::updated = false;


void Tiles::update(){
    
    
    if (hoverWall>0&&hoverWallSide==1){
   
        if (!testDoor){
            
            if (Door::createThis){
                testDoor= new Door(hoverWall,Door::createThis->index ,false);
                testDoor->LeftOrRight = true;
                testDoor->setFirstFrame(false);
                testDoor->update();
            }
        
        } else {
        
            
            if (Door::createThis){
                testDoor->destroy();
                free (testDoor);
                testDoor= new Door(hoverWall,Door::createThis->index ,false);
                testDoor->LeftOrRight = true;
                testDoor->setFirstFrame(false);
                testDoor->update();
            }
        
        }
        
        
        
    
    } else if (hoverWall>0&&hoverWallSide==2){
    
        
        if (!testDoor){
            
            if (Door::createThis){
                testDoor= new Door(hoverWall,Door::createThis->index ,true);
                testDoor->setFirstFrame(true);
                testDoor->LeftOrRight = false;
                testDoor->update();
            }
            
        } else {
            
            
            if (Door::createThis){
                testDoor->destroy();
                free(testDoor);
                testDoor= new Door(hoverWall,Door::createThis->index ,true);
                testDoor->setFirstFrame(true);
                testDoor->LeftOrRight = false;
                testDoor->update();
            }
            
        }

    
    } else {
    
        if (testDoor){
          testDoor->destroy();
            testDoor=0;
        }
    
    
    }
    
    
    int value[2];
    
    std::pair<int,int> tilePos = findTile(cpv(Camera::posX, Camera::posY));
    
    
    value[0]=-1*tilePos.first;
    value[1]=-1*tilePos.second;
    
    //START
    
    
    
    //Draw Tiles
    int maxStep = 16*Camera::zoom;

    
    std::vector<std::vector<GLuint> > tilesHolders;
    
    for (int counterY =value[1]-maxStep;
         counterY <=value[1]+maxStep;
         counterY++){
        if (counterY>=0){
            int Yvalue = ((width)*(counterY));
            //-12,16
            for (int counterX =value[0]-maxStep;
                 counterX <= value[0]+maxStep;
                 counterX++) {
                
                int Xvalue = counterX;
                if (Xvalue<0||Yvalue<0||(Yvalue+Xvalue)>=mapValues.size()){
                    continue;
                }
                
                TileObject * tile = &mapValues[Xvalue+Yvalue];
                
                while (tile->type+2> tilesHolders.size()){
                    tilesHolders.push_back(std::vector<GLuint>());
                }
                
                tilesHolders[tile->type].push_back(Xvalue+Yvalue);
                

            }
        }
    }
    
   
    fastTileSort.clear();
    
    for (std::vector<std::vector<GLuint> >::iterator IT = tilesHolders.begin();
         IT != tilesHolders.end();
         IT++){
        
        if ((*IT).begin()==(*IT).end()){
            continue;
        }
        
        size_t counter = IT - tilesHolders.begin();
        
        FastTileObject newObject;
        
        newObject.type = counter;
        newObject.start = (*IT).at(0);
        newObject.end = newObject.start-1;
        

        
        for (std::vector<GLuint>::iterator IT2 = (*IT).begin();
             IT2 != (*IT).end();
             IT2++){
        
            if ((*IT2)-1==newObject.end){
            
                newObject.end = *IT2;
            
            } else {
                fastTileSort[newObject.type].push_back(newObject);
                newObject.start = *IT2;
                newObject.end = *IT2;
            
            }
            
            if (IT2-(*IT).begin() == (*IT).size()-1){
                
                fastTileSort[newObject.type].push_back(newObject);
            
            }
            
            
            
        }
    }
    
    

    
    
    
    
    
    if (updated){
        return;
    }
    
    

    
    
    
    {
    std::vector<float> vertex;
    
    double multi=10.0;
    
    
    double leftMost = 0;
    double rightMove = leftMost;
    double topMost =0;
    double topMove = topMost;
    
   
    
    int lineCount = 0;
    int lengthCount = 0;
    for (int counter =0; counter<width*height; counter++) {
        
        if (lengthCount >= width){
            lengthCount=0;
            lineCount++;
            rightMove=leftMost-=multi;
            
            topMost +=T_V*multi;
            topMove = topMost;
            
        }
        lengthCount++;
        

        
        
    
    // Create Tile
    vertex.push_back(-1.0*multi+rightMove);
    vertex.push_back(-T_V*multi+topMove);
    vertex.push_back(0);
    vertex.push_back(1);
    
    vertex.push_back(-1*multi+rightMove);
    vertex.push_back(T_V*multi+topMove);
    vertex.push_back(0);
    vertex.push_back(0);
    
    
    
    vertex.push_back(1*multi+rightMove);
    vertex.push_back(-T_V*multi+topMove);
    vertex.push_back(1);
    vertex.push_back(1);
    
    
    
    
    vertex.push_back(-1*multi+rightMove);
    vertex.push_back(T_V*multi+topMove);
    vertex.push_back(0);
    vertex.push_back(0);
    
    
    vertex.push_back(1*multi+rightMove);
    vertex.push_back(T_V*multi+topMove);
    vertex.push_back(1);
    vertex.push_back(0);
    
    vertex.push_back(1*multi+rightMove);
    vertex.push_back(-T_V*multi+topMove);
    vertex.push_back(1);
    vertex.push_back(1);
        
        rightMove+=multi;
        topMove+=T_V*multi;
    }
    
    if (!_vertexArrayTiles){
        glGenVertexArraysCon(1, &_vertexArrayTiles);
    }
    glBindVertexArrayCon(_vertexArrayTiles);
    
    if(!_vertexBufferTiles){
        glGenBuffers(1, &_vertexBufferTiles);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferTiles);
    glBufferData(GL_ARRAY_BUFFER, vertex.size()*sizeof(float), &vertex[0], GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    
    }
    
    
    if (!halfWalls){
        std::vector<float> vertex;
        
        double multi=10.0;
        
        
        double leftMost = 0;
        double rightMove = leftMost;
        double topMost =0;
        double topMove = topMost;
        
        
        double spacerY1 = 7.35;
        double spacerX1 = -10.14-0.06;
        
        double spacerY2 = 7.5;
        double spacerX2 = -20.2;
        
        int lineCount = 0;
        int lengthCount = 0;
        
        
        
        
        
        
        
        
        for (int counter =0; counter<width*height; counter++) {
            
            if (lengthCount >= width){
                lengthCount=0;
                lineCount++;
                rightMove=leftMost-=multi;
                
                topMost +=T_V*multi;
                topMove = topMost;
                
            }
            lengthCount++;
            
            
           
            
            // Create Wall Right Going Down
            vertex.push_back(1.0*multi+rightMove+spacerX1);
            vertex.push_back(-W_V*multi+topMove+spacerY1);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(1.0*multi+rightMove+spacerX1);
            vertex.push_back(W_V*multi+topMove+spacerY1);
            vertex.push_back(0);
            vertex.push_back(0);
            
            
            
            vertex.push_back(2.0*multi+rightMove+spacerX1);
            vertex.push_back(-W_V*multi+topMove+spacerY1);
            vertex.push_back(1);
            vertex.push_back(1);
            
            
            
            
            vertex.push_back(1.0*multi+rightMove+spacerX1);
            vertex.push_back(W_V*multi+topMove+spacerY1);
            vertex.push_back(0);
            vertex.push_back(0);
            
            
            vertex.push_back(2.0*multi+rightMove+spacerX1);
            vertex.push_back(W_V*multi+topMove+spacerY1);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(2.0*multi+rightMove+spacerX1);
            vertex.push_back(-W_V*multi+topMove+spacerY1);
            vertex.push_back(1);
            vertex.push_back(1);
            
            
            
            // Create Wall Left Going Down
            
            vertex.push_back(1.0*multi+rightMove+spacerX2);
            vertex.push_back(-W_V*multi+topMove+spacerY2);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(1.0*multi+rightMove+spacerX2);
            vertex.push_back(W_V*multi+topMove+spacerY2);
            vertex.push_back(0);
            vertex.push_back(0);
            
            
            
            vertex.push_back(2.0*multi+rightMove+spacerX2);
            vertex.push_back(-W_V*multi+topMove+spacerY2);
            vertex.push_back(1);
            vertex.push_back(1);
            
            
            
            
            vertex.push_back(1.0*multi+rightMove+spacerX2);
            vertex.push_back(W_V*multi+topMove+spacerY2);
            vertex.push_back(0);
            vertex.push_back(0);
            
            
            vertex.push_back(2.0*multi+rightMove+spacerX2);
            vertex.push_back(W_V*multi+topMove+spacerY2);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(2.0*multi+rightMove+spacerX2);
            vertex.push_back(-W_V*multi+topMove+spacerY2);
            vertex.push_back(1);
            vertex.push_back(1);
            
            rightMove+=multi;
            topMove+=T_V*multi;
        }
        
        if (!_vertexArrayWalls){
            glGenVertexArraysCon(1, &_vertexArrayWalls);
        }
        glBindVertexArrayCon(_vertexArrayWalls);
        
        if(!_vertexBufferWalls){
            glGenBuffers(1, &_vertexBufferWalls);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferWalls);
        glBufferData(GL_ARRAY_BUFFER, vertex.size()*sizeof(float), &vertex[0], GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
    }
    
    else
    
    {
        std::vector<float> vertex;
        
        double multi=10.0;
        
        
        double leftMost = 0;
        double rightMove = leftMost;
        double topMost =0;
        double topMove = topMost;
        
        
        double spacerY1 = 7.35;
        double spacerX1 = -10.14;
        
        double spacerY2 = 7.5;
        double spacerX2 = -20.2;
        
        int lineCount = 0;
        int lengthCount = 0;
        
        
        
        
        
        
        
        
        for (int counter =0; counter<width*height; counter++) {
            
            if (lengthCount >= width){
                lengthCount=0;
                lineCount++;
                rightMove=leftMost-=multi;
                
                topMost +=T_V*multi;
                topMove = topMost;
                
            }
            lengthCount++;
            
           
            
            
            // Create Wall Right Going Down
            vertex.push_back(1.0*multi+rightMove+spacerX1);
            vertex.push_back(-W_V*multi+topMove+spacerY1);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(1.0*multi+rightMove+spacerX1);
            vertex.push_back(0+topMove+spacerY1-3.4);
            vertex.push_back(0);
            vertex.push_back(0.65573770491803);
            
            
            
            vertex.push_back(2.0*multi+rightMove+spacerX1);
            vertex.push_back(-W_V*multi+topMove+spacerY1);
            vertex.push_back(1);
            vertex.push_back(1);
            
            
            //UP --3.4
            //Left 1.6
            
            vertex.push_back(1.0*multi+rightMove+spacerX1);
            vertex.push_back(0*multi+topMove+spacerY1-3.4);
            vertex.push_back(0);
            vertex.push_back(0.65573770491803);
            
            
            vertex.push_back(2.0*multi+rightMove+spacerX1);
            vertex.push_back(0*multi+topMove+spacerY1+1.6);
            vertex.push_back(1);
            vertex.push_back(0.46);
            
            vertex.push_back(2.0*multi+rightMove+spacerX1);
            vertex.push_back(-W_V*multi+topMove+spacerY1);
            vertex.push_back(1);
            vertex.push_back(1);
            
            
            
            // Create Wall Left Going Down
            
            vertex.push_back(1.0*multi+rightMove+spacerX2);
            vertex.push_back(-W_V*multi+topMove+spacerY2);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(1.0*multi+rightMove+spacerX2);
            vertex.push_back(0*multi+topMove+spacerY2+1.6);
            vertex.push_back(0);
            vertex.push_back(.463);
            
            
            
            vertex.push_back(2.0*multi+rightMove+spacerX2);
            vertex.push_back(-W_V*multi+topMove+spacerY2);
            vertex.push_back(1);
            vertex.push_back(1);
            
            
            
            
            vertex.push_back(1.0*multi+rightMove+spacerX2);
            vertex.push_back(0*multi+topMove+spacerY2+1.6);
            vertex.push_back(0);
            vertex.push_back(.463);
            
            
            vertex.push_back(2.0*multi+rightMove+spacerX2);
            vertex.push_back(0*multi+topMove+spacerY2-3.4);
            vertex.push_back(1);
            vertex.push_back(.66);
            
            vertex.push_back(2.0*multi+rightMove+spacerX2);
            vertex.push_back(-W_V*multi+topMove+spacerY2);
            vertex.push_back(1);
            vertex.push_back(1);
            
            rightMove+=multi;
            topMove+=T_V*multi;
        }
        
        if (!_vertexArrayWalls){
            glGenVertexArraysCon(1, &_vertexArrayWalls);
        }
        glBindVertexArrayCon(_vertexArrayWalls);
        
        if(!_vertexBufferWalls){
            glGenBuffers(1, &_vertexBufferWalls);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferWalls);
        glBufferData(GL_ARRAY_BUFFER, vertex.size()*sizeof(float), &vertex[0], GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
    }
    
    
    
    
    
    updated=true;
}



GLenum returnSrcGL1(int value){
    
    switch (value) {
        case 0:
        return GL_NEVER;
        case 1:
        return GL_LESS;
        case 2:
        return GL_EQUAL;
        case 3:
        return GL_LEQUAL;
        case 4:
        return GL_GREATER;
        case 5:
        return GL_NOTEQUAL;
        case 6:
        return GL_GEQUAL;
        case 7:
        return GL_ALWAYS;
        default:
        return GL_NEVER;
    }
}


GLenum returnSrcGL2(int value){
    return value/100.0;
    
}

void Tiles::renderSpritesShadow(int x, int y){
    
    
    int Xvalue = x;
    int Yvalue = y*width;
    
    if (x<0||y<0){return;}
    if (x>=width){return;}
    if (y>=height){return;}
    
    //int target = Yvalue+Xvalue;
    
    
    
    
    std::vector<Sprite*> * spriteRenderGrid = &Tiles::mapValues[Yvalue+Xvalue].renderGrid;
    
    if (spriteRenderGrid->size()>0){
        
        std::vector<Sprite*> * theVector =spriteRenderGrid;
        
        for (std::vector<Sprite*>::iterator IT = theVector->begin();
             IT!=theVector->end();
             IT++){
            if (!(*IT)->dict->prop||(*IT)->dict->aTree){
                (*IT)->renderShadow();
            }
        }
    }
}


void Tiles::renderSprites(int x, int y){
    
    
    int Xvalue = x;
    int Yvalue = y*width;
    
    if (x<0||y<0){return;}
    if (x>=width){return;}
    if (y>=height){return;}
    
    //int target = Yvalue+Xvalue;
    

  
    
    std::vector<Sprite*> * spriteRenderGrid = &Tiles::mapValues[Yvalue+Xvalue].renderGrid;
    
    
    
    if (Yvalue+Xvalue==Player::playerPosition){
    ShaderManager::enableTextureShader();

        Player::mainPlayer->render();
    
    }
    
    if (spriteRenderGrid->size()>0){
        
        
        std::vector<Sprite*> * theVector =spriteRenderGrid;
        
        for (std::vector<Sprite*>::iterator IT = theVector->begin();
             IT!=theVector->end();
             IT++){
            
       
            
            if ((*IT)->isSelected){
                float colorValue[4] = {0.0,0.0,0.0,1.0};
                
                (*IT)->generateColor(colorValue);
                
                ShaderManager::enableHighLightShaderColor((*IT)->dict->textureWidth, colorValue);
                //ShaderManager::enableHighLightShader((*IT)->dict->textureWidth);
            } else {
                              ShaderManager::enableTextureShader();
            }
             
            (*IT)->render();
            glBindVertexArrayCon(_vertexArrayWalls);
            
            if ((*IT)->isSelected){
                ShaderManager::enableTextureShader();
            }
        }
        
        
        
        
    }
    
    
}



void Tiles::renderWallA(int x, int y, bool z){


    if (_vertexArrayWalls!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(_vertexArrayWalls);
        
        {
            
            int Xvalue = x;
            int Yvalue = y*width;

             if (x<0||y<0||x>=width||y>=height){return;}
            
            
            int target = Yvalue+Xvalue;
            
            if (target>=mapValues.size()){return;}
            
            if (z){
            
                TileObject * rightWall = &mapValues[target];
                
                if (rightWall->rw){
                    glBindTexture(GL_TEXTURE_2D, *rightWall->textureWallR);
                    glDrawArrays(GL_TRIANGLES, (Yvalue+Xvalue+1)*12+6, 6);
                }

            
            } else {
            
                TileObject * leftWall = &mapValues[target];
            
                if (leftWall->lw){
                    glBindTexture(GL_TEXTURE_2D, *leftWall->textureWallL);
                    glDrawArrays(GL_TRIANGLES, (Yvalue+Xvalue)*12, 6);
                }
            
                
                
            }
            
            
        

            
        }
    
    }

}



void Tiles::renderWallR(int x, int y){
 
    //Check if the Walls have been enabled
    if (_vertexArrayWalls!=0){
        //Set the Active Texture to 0;
        glActiveTexture(GL_TEXTURE0);
        
        //Bind the Vertex Array For the Walls.
        glBindVertexArrayCon(_vertexArrayWalls);
        
        {
            
            
            
            int Xvalue = x;
            int Yvalue = y*width;
            
            //Check if the Wall is in bounds;
            if (x<0||y<0||x>=width||y>=height){return;}
            
            //This is the Index of The wall
            int target = Yvalue+Xvalue+1;
            //Door * rightDoor = 0;
            
            if (target)

            
            if (target>=mapValues.size()){return;}
            
            TileObject * rightWall = &mapValues[target];
            
            //
            if (target==hoverWall&&hoverWallSide==2&&Door::createThis){

                    ShaderManager::enableSelectShader(false);
            
                if (testDoor){
                    testDoor->render();
                }
                returnNormalShader();
            } else if (target==hoverWall&&hoverWallSide==4){
                
                ShaderManager::enableSelectShader(true);
                
                if (rightWall){
                    if (rightWall->theDoorR){
                        rightWall->theDoorR->render();
                    }
                }
                returnNormalShader();

                
            }else{
            //Draw The Wall Normally
                
   
                
            if (rightWall->theDoorR){
            
                rightWall->theDoorR->render();
                //glBindVertexArrayCon(_vertexArrayWalls);
            }
            
            if (rightWall->rw){
                glBindTexture(GL_TEXTURE_2D, *rightWall->textureWallR);
                glDrawArrays(GL_TRIANGLES, (Yvalue+Xvalue+1)*12+6, 6);
            }
            
            }
            
            
            
            
        }
    }
}



void Tiles::returnNormalShader(){

    if (GameLogic::gameMode==TILEMODE){
        ShaderManager::enableHalfTextureShader();
        
    } else {
        
        ShaderManager::enableTextureShader();
    }
    
}

void Tiles::renderWallL(int x, int y){
    
    
    if (_vertexArrayWalls!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(_vertexArrayWalls);
        
        {
            
            
            
            int Xvalue = x;
            int Yvalue = y*width;
            
            
            int doorMod = -width;
            
            if (x<0||y<0){return;}
            if (x>=width){return;}
            if (y>=height){return;}
            
            
            if (Yvalue+Xvalue+width>=mapValues.size()){return;}
            if (Yvalue+Xvalue+1>=mapValues.size()){return;}
            
            int target = Yvalue+Xvalue+width;
            
            TileObject * leftWall = &mapValues[target];
            TileObject * leftDoor=0;
            if (target>width){
                leftDoor = &mapValues[target+doorMod];
            }
            
            
            if (target==hoverWall&&hoverWallSide==1&&Door::createThis){
                
                
                ShaderManager::enableSelectShader(false);
                
                if (testDoor){
                    testDoor->render();
                }
                returnNormalShader();
                
            } else if (target==hoverWall&&hoverWallSide==3){
                
                ShaderManager::enableSelectShader(true);
                
                if (leftWall->theDoorL){
                    leftWall->theDoorL->renderB();
                }
                 
                returnNormalShader();
            }else if (target+doorMod==hoverWall&&hoverWallSide==3){
                
                ShaderManager::enableSelectShader(true);
                if (leftDoor){
                    if (leftDoor->theDoorL){
                        leftDoor->theDoorL->renderA();
                    }
                }
                returnNormalShader();
                
            } else {
                
                
                
                if (leftDoor){
                    if (leftDoor->theDoorL){
                        leftDoor->theDoorL->renderB();
                    }
                }
                
                
                if (leftWall->theDoorL){
                    if (target==hoverWall+doorMod&&hoverWallSide==3){
                    //ShaderManager::enableSelectShader(true);
                       // leftWall->theDoorL->renderA();
                      //ShaderManager::enableTextureShader();
                    }else {
                    
                    leftWall->theDoorL->renderA();
                    }
                  
                }
                
                
                if (leftWall->lw){
                    glBindVertexArrayCon(_vertexArrayWalls);
                    glBindTexture(GL_TEXTURE_2D, *leftWall->textureWallL);
                    glDrawArrays(GL_TRIANGLES, (Yvalue+Xvalue+width)*12, 6);
                
                }
                
            }
        }
    }
}


std::map<std::pair<int, int>, bool> testData;

bool checkTest (std::pair<int, int> test){
    
    if (testData.find(test)==testData.end()){
    testData[test]=false;
        return false;
    } else {
    
        return true;
    }
    


}







void Tiles::stageOneRender(){

    
    
    
    
    
    ShaderManager::enableTextureShader();
    if (_vertexArrayTiles!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(_vertexArrayTiles);
        
        for (std::map<int,std::vector< FastTileObject> >::iterator IT = fastTileSort.begin();
             IT != fastTileSort.end();
             IT++){
            
            if (!Tiles::tileDictVector[IT->first]->checker){
                glBindTexture(GL_TEXTURE_2D, *tileDictVector[IT->first]->texture);
                
                for (std::vector<FastTileObject>::iterator IT2 = IT->second.begin();
                     IT2 != IT->second.end();
                     IT2++){
                    
                    FastTileObject * object = &(*IT2);
                    
                    glDrawArrays(GL_TRIANGLES, GLuint(object->start*6), (1+object->end-object->start)*6);
                }
            } else {
            
                
                for (std::vector<FastTileObject>::iterator IT2 = IT->second.begin();
                     IT2 != IT->second.end();
                     IT2++){
                    
                    FastTileObject * object = &(*IT2);
                    
                    for (int counter = object->start; counter <=object->end; counter++ ){
                    
                        int valueEvenOrOddY = (counter/Tiles::width)%(2);
                        int valueEvenOrOddX = (counter%2);
                        
                    
                        if ((valueEvenOrOddX + valueEvenOrOddY)%2){
                            
                            glBindTexture(GL_TEXTURE_2D,*Tiles::tileDictVector[object->type]->texture);
                            glDrawArrays(GL_TRIANGLES, GLuint(counter*6), 6);
                            
                        } else {
                        
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[object->type]->textureC);
                            glDrawArrays(GL_TRIANGLES, GLuint(counter*6), 6);
                        
                        }
                    }
                    
                    
                    
                }
            
            
            }
        }
    }
}

void Tiles::stageTwoRender(){

    
    int value[2];
    
    std::pair<int,int> tilePos = findTile(cpv(Camera::posX, Camera::posY));
    
    
    value[0]=-1*tilePos.first;
    value[1]=-1*tilePos.second;
    
    int maxStep = 16*Camera::zoom;


        ShaderManager::enableSelectShader(true);
        
        if (_vertexArrayTiles!=0){
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArrayCon(_vertexArrayTiles);
            
            
            for (int counterY =value[1]+maxStep;
                 counterY >value[1]-maxStep;
                 counterY--){
                if (counterY>=0){
                    int Yvalue = ((width)*(counterY));
                    //-12,16
                    for (int counterX =value[0]+maxStep;
                         counterX > value[0]-maxStep;
                         counterX--) {
                        
                        int Xvalue = counterX;
                        if (Xvalue<0||Yvalue<0||(Yvalue+Xvalue)>=mapValues.size()){
                            continue;
                        }
                        
                        if (mapValues[Yvalue+Xvalue].type>2){
                            if (!mapValues[Yvalue+Xvalue].canWalkThere){
                                mapValues[Yvalue+Xvalue].renderTile(Yvalue+Xvalue);
                            }
                        }
                    }
                }
            }
        }
    
    
    
}

void Tiles::stageThreeRender(){

    
    int value[2];
    
    std::pair<int,int> tilePos = findTile(cpv(Camera::posX, Camera::posY));
    
    
    value[0]=-1*tilePos.first;
    value[1]=-1*tilePos.second;
    
    int maxStep = 16*Camera::zoom;

    
    if (_vertexArrayWalls!=0){
        glActiveTexture(GL_TEXTURE0);
        
        
        int xLow =  -tilePos.first+-maxStep;
        int xHigh = -tilePos.first+maxStep;
        int yLow = -tilePos.second+-maxStep;
        int yHigh = -tilePos.second+maxStep;
        
        ShaderManager::enableTextureShaderShadow(1.0);
        for (int counterX = xHigh; counterX>xLow; counterX--){
            for (int counterY = yHigh; counterY>yLow; counterY--){
                renderSpritesShadow(counterX, counterY);
            }
        }
    }

    
    
}

void Tiles::stageFourRender(){

   
    
    
    // Draw Selection
    if (startIndex!=-1&&lastIndex!=-1){
        if (overInfluence){
            ShaderManager::enableSelectShader(true);
        } else {
            ShaderManager::enableSelectShader(false);
            //            ShaderManager::enableTextureShader();
        }
        {
            
            int leftStart = startIndex%(width);
            int rightStart = lastIndex%(width);
            
            if (leftStart>rightStart){
                int holder = leftStart;
                leftStart = rightStart;
                rightStart=holder;
                
            }
            
            int downStart = startIndex/(width);
            int upStart = lastIndex/(width);
            
            if (downStart<upStart){
                int holder = downStart;
                downStart = upStart;
                upStart=holder;
                
            }
            
            
            if (_vertexArrayTiles!=0){
                glActiveTexture(GL_TEXTURE0);
                glBindVertexArrayCon(_vertexArrayTiles);
                
                //int counterX = 0;
                
                for (int counterY =upStart; counterY <downStart+1; counterY++){
                    if (counterY>=0){
                        int Yvalue = ((width)*(counterY));
                        for (int counterX =leftStart; counterX < rightStart+1; counterX++) {
                            
                            int Xvalue = counterX;
                            
                            if (Xvalue<0||Yvalue<0||(Yvalue+Xvalue)>=mapValues.size()){
                                continue;
                            }
                            
                            
                            mapValues[Yvalue+Xvalue].renderTile(Yvalue+Xvalue,(int)Tiles::changeTileType);
                        }
                    }
                }
            }
        }
    }
    
    
}



void Tiles::stageFiveRender(){
    
    
    ShaderManager::enableTextureShader();
    glBindVertexArrayCon(_vertexArrayTiles);
    if (!(startIndex!=-1&&lastIndex!=-1)){
        if (GameLogic::gameMode==TILEMODE){
            //Draw a tile over mouse cursor
            
            if (_vertexArrayTiles!=0)
            {
                if (!(hoverTile<0||hoverTile>=Tiles::width*Tiles::width)){
	
                    Tiles::mapValues[hoverTile].renderTile(hoverTile,(int)changeTileType);
                    
                    
                    
                }
            }
        }
    }


}

void Tiles::stageSixRender(){
    if (_vertexArrayWalls!=0){
        
        ShaderManager::enableSelectShader(true);
        glBindTexture(GL_TEXTURE_2D, *tileDictVector[0]->texture);
        
        for (std::set<int>::iterator IT = hoverTileSelected.begin();
             IT != hoverTileSelected.end();
             IT++){
            
            glBindVertexArrayCon(_vertexArrayTiles);
            int theValue = (*IT);
            glDrawArrays(GL_TRIANGLES, theValue*6, 6);
            
            
            
            
        }
    }
    
}

void Tiles::stageSevenRender(){

    
    ShaderManager::enableSelectShader(true);
    
    std::vector<int> theIndex = Player::returnPath();
    
    glBindVertexArrayCon(_vertexArrayTiles);
    
    for (std::vector<int>::iterator IT = theIndex.begin();
         IT != theIndex.end();
         IT++){
        
        mapValues[*IT].renderTile(*IT);
        
    }
    
    
    for (std::set<int>::iterator IT = Tiles::trapRadius.begin();
         IT != Tiles::trapRadius.end();
         IT++){
        
        mapValues[*IT].renderTile(*IT);
        
    }
    
    
    

}

void Tiles::stageEightRender(){
    if (Sprite::testSprite){
        if (Sprite::testSprite->dict->prop){
            
            SpriteDict * theDict = Sprite::testSprite->dict;
            if (Sprite::dontRenderTest==1){
                ShaderManager::enableSelectShader(false);
            } else if (Sprite::dontRenderTest==2){
                
                GLfloat colorValue[4] = {240.0/255.0,240/255.0,21/255.0,0};
                
                ShaderManager::enableColorShift(colorValue);
            } else {
                ShaderManager::enableSelectShader(true);
            }
            
            
            if (theDict->placementType==NoPlacement){
                
                
            } else {
                
                size_t maxSize = Tiles::mapValues.size();
                
                
                
                switch (theDict->placementType){
                        
                    case OneXOneLeftWall:
                    case OneXOne:
                    {
                        
                        glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite].type]->texture);
                        glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite)*6, 6);
                    }
                        break;
                        
                    case TwoXOneULeftWall:
                    case TwoXOneU:
                    {
                        if (Sprite::indexOfTestSprite+1<maxSize){
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+1)*6, 6);
                        }
                        
                    }
                        break;
                    case TwoXOneL:
                    {
                        
                        if (Sprite::indexOfTestSprite+width<maxSize){
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width)*6, 6);
                        }
                        
                        
                    }
                        break;
                        
                        ;
                    case TwoXTwo:
                    {
                        if (Sprite::indexOfTestSprite+width+1<maxSize){
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+1)*6, 6);
                        }
                        
                    }
                        break;
                        
                    case ThreeXTwoL:
                        
                    {
                        if (Sprite::indexOfTestSprite+width+width+1<maxSize){
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width+width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+width)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width+width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+width+1)*6, 6);
                            
                            
                        }
                        
                    }
                        
                        
                        break;
                        
                    case ThreeXTwoU:
                        
                    {
                        if (Sprite::indexOfTestSprite+width+2<maxSize){
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+2)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+2)*6, 6);
                        }
                        
                    }
                        
                        break;
                        
                    case ThreeXOneLRightWall:
                    case ThreeXOneL:
                        
                    {
                        if (Sprite::indexOfTestSprite+width+width<maxSize){
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width+width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+width)*6, 6);
                        }
                        
                    }
                        break;
                        
                        
                    case TwoXEight:{
                        
                        
                        if (Sprite::indexOfTestSprite+(8*width+1)<maxSize){
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+1)*6, 6);
                            
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(1*width)+0)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1*width+1*1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(1*width)+1)*6, 6);
                            
                            
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+2*width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(2*width)+0)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+2*width+1*1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(2*width)+1)*6, 6);
                            
                            
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+3*width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(3*width)+0)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+3*width+1*1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(3*width)+1)*6, 6);
                            
                            
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+4*width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(4*width)+0)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+4*width+1*1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(4*width)+1)*6, 6);
                            
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+5*width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(5*width)+0)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+5*width+1*1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(5*width)+1)*6, 6);
                            
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+6*width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(6*width)+0)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+6*width+1*1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(6*width)+1)*6, 6);
                            
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+7*width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(7*width)+0)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+7*width+1*1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(7*width)+1)*6, 6);
                            
                            
                            
                        }
                        
                        
                        
                    }
                        break;
                        
                    case SixXThreeL:{
                        
                        
                        if (Sprite::indexOfTestSprite+(6*width+2)<maxSize){
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+2)*6, 6);
                            
                            
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1*width+0].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(1*width)+0)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1*width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(1*width)+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1*width+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(1*width)+2)*6, 6);
                            
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+2*width+0].type]->texture);
                            
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(2*width)+0)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+2*width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(2*width)+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+2*width+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(2*width)+2)*6, 6);
                            
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+3*width+0].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(3*width)+0)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+3*width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(3*width)+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+3*width+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(3*width)+2)*6, 6);
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+4*width+0].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(4*width)+0)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+4*width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(4*width)+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+4*width+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(4*width)+2)*6, 6);
                            
                            
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+5*width+0].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(5*width)+0)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+5*width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(5*width)+1)*6, 6);
                            glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+5*width+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+(5*width)+2)*6, 6);
                            
                            
                        }
                        
                        
                    }
                        
                        
                        break;
                        
                        
                    case ThreeXThree:
                    {
                        if (Sprite::indexOfTestSprite+width+width+2<maxSize){
                            
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+0*width+0].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+0*width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+1)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+0*width+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+2)*6, 6);
                            
                            
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1*width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1*width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+1)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+1*width+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+2)*6, 6);
                            
                            
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+2*width].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+width)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+2*width+1].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+width+1)*6, 6);
                             glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[Sprite::indexOfTestSprite+2*width+2].type]->texture);
                            glDrawArrays( GL_TRIANGLES, (Sprite::indexOfTestSprite+width+width+2)*6, 6);
                            
                        }
                        
                    }
                        break;
                    case NoPlacement:
                        break;
                        
                        
                        
                        
                        break;
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                };
            }
        }
    }
    
    
    
    
    
	if (Sprite::moveSprite){
		if (Sprite::canMove){
			if (Sprite::moveSprite->dict->prop){

				SpriteDict * theDict = Sprite::moveSprite->dict;

				GLfloat colorValue[4] = { 240.0 / 255.0, 240 / 255.0, 21 / 255.0, 0 };
				ShaderManager::enableColorShift(colorValue);
				int index = Sprite::moveSprite->renderGridIndex;

				if (theDict->placementType == NoPlacement){


				}
				else {

					size_t maxSize = Tiles::mapValues.size();



					switch (theDict->placementType){

					case OneXOneLeftWall:
					case OneXOne:
					{

						glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
						glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
					}
					break;

					case TwoXOneULeftWall:
					case TwoXOneU:
					{
						if (index + 1 < maxSize){
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
						}

					}
					break;
					case TwoXOneL:
					{

						if (index + width < maxSize){
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
						}


					}
					break;

					;
					case TwoXTwo:
					{
						if (index + width + 1 < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 1) * 6, 6);
						}

					}
					break;

					case ThreeXTwoL:

					{
						if (index + width + width + 1 < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width + 1) * 6, 6);


						}

					}


					break;

					case ThreeXTwoU:

					{
						if (index + width + 2 < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 2) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 2) * 6, 6);
						}

					}

					break;

					case ThreeXOneLRightWall:
					case ThreeXOneL:

					{
						if (index + width + width < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width) * 6, 6);
						}

					}
					break;


					case TwoXEight:{


						if (index + (8 * width + 1) < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (1 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (1 * width) + 1) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (2 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (2 * width) + 1) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 3 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (3 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 3 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (3 * width) + 1) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 4 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (4 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 4 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (4 * width) + 1) * 6, 6);

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 5 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (5 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 5 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (5 * width) + 1) * 6, 6);

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 6 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (6 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 6 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (6 * width) + 1) * 6, 6);

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 7 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (7 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 7 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (7 * width) + 1) * 6, 6);



						}



					}
								   break;

					case SixXThreeL:{


						if (index + (6 * width + 2) < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 2) * 6, 6);



							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 0].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (1 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (1 * width) + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (1 * width) + 2) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 0].type]->texture);

							glDrawArrays(GL_TRIANGLES, (index + (2 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (2 * width) + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (2 * width) + 2) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 3 * width + 0].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (3 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 3 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (3 * width) + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 3 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (3 * width) + 2) * 6, 6);

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 4 * width + 0].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (4 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 4 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (4 * width) + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 4 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (4 * width) + 2) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 5 * width + 0].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (5 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 5 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (5 * width) + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 5 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (5 * width) + 2) * 6, 6);


						}


					}


									break;


					case ThreeXThree:
					{
						if (index + width + width + 2 < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 0 * width + 0].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 0 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 0 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 2) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 2) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width + 2) * 6, 6);

						}

					}
					break;
					case NoPlacement:
						break;




						break;













					};






				}
				index = Tiles::findTile(
					Tiles::findTile(GameLogic::lastTouchPosition));

				
				if (theDict->placementType == NoPlacement){


				}
				else {

					size_t maxSize = Tiles::mapValues.size();



					switch (theDict->placementType){

					case OneXOneLeftWall:
					case OneXOne:
					{

						glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
						glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
					}
					break;

					case TwoXOneULeftWall:
					case TwoXOneU:
					{
						if (index + 1 < maxSize){
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
						}

					}
					break;
					case TwoXOneL:
					{

						if (index + width < maxSize){
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
						}


					}
					break;

					;
					case TwoXTwo:
					{
						if (index + width + 1 < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 1) * 6, 6);
						}

					}
					break;

					case ThreeXTwoL:

					{
						if (index + width + width + 1 < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width + 1) * 6, 6);


						}

					}


					break;

					case ThreeXTwoU:

					{
						if (index + width + 2 < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 2) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 2) * 6, 6);
						}

					}

					break;

					case ThreeXOneLRightWall:
					case ThreeXOneL:

					{
						if (index + width + width < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width) * 6, 6);
						}

					}
					break;


					case TwoXEight:{


						if (index + (8 * width + 1) < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (1 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (1 * width) + 1) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (2 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (2 * width) + 1) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 3 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (3 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 3 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (3 * width) + 1) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 4 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (4 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 4 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (4 * width) + 1) * 6, 6);

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 5 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (5 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 5 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (5 * width) + 1) * 6, 6);

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 6 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (6 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 6 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (6 * width) + 1) * 6, 6);

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 7 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (7 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 7 * width + 1 * 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (7 * width) + 1) * 6, 6);



						}



					}
								   break;

					case SixXThreeL:{


						if (index + (6 * width + 2) < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 2) * 6, 6);



							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 0].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (1 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (1 * width) + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (1 * width) + 2) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 0].type]->texture);

							glDrawArrays(GL_TRIANGLES, (index + (2 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (2 * width) + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (2 * width) + 2) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 3 * width + 0].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (3 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 3 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (3 * width) + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 3 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (3 * width) + 2) * 6, 6);

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 4 * width + 0].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (4 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 4 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (4 * width) + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 4 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (4 * width) + 2) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 5 * width + 0].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (5 * width) + 0) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 5 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (5 * width) + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 5 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + (5 * width) + 2) * 6, 6);


						}


					}


									break;


					case ThreeXThree:
					{
						if (index + width + width + 2 < maxSize){

							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 0 * width + 0].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index)* 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 0 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 0 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + 2) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 1 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + 2) * 6, 6);


							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 1].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width + 1) * 6, 6);
							glBindTexture(GL_TEXTURE_2D, *tileDictVector[Tiles::mapValues[index + 2 * width + 2].type]->texture);
							glDrawArrays(GL_TRIANGLES, (index + width + width + 2) * 6, 6);

						}

					}
					break;
					case NoPlacement:
						break;




						break;













					};






				}









			}
		}
	}

}

void Tiles::stageNineRender(){

 
    if (_vertexArrayTiles!=0){
        
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(_vertexArrayTiles);
        for (std::vector<PathTile>::iterator IT = debugPath.begin();
             IT != debugPath.end();
             IT++){
            
            Tiles::mapValues[IT->index].renderTile(IT->index,(int)changeTileType);
            
            
            
            
            
        }
        
    }


}

void Tiles::stageTenRender(){

    int value[2];
    
    std::pair<int,int> tilePos = findTile(cpv(Camera::posX, Camera::posY));
    
    
    value[0]=-1*tilePos.first;
    value[1]=-1*tilePos.second;
    
    int maxStep = 16*Camera::zoom;
    

    ShaderManager::enableTextureShader();
    
    
  
    if (_vertexArrayWalls!=0){
        
        
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(_vertexArrayWalls);
        
        if (true){
            glEnable(GL_STENCIL_TEST);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.05);
            glClear(GL_STENCIL_BUFFER_BIT);
            
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        }
        
        
        int xLow =  -tilePos.first+-maxStep;
        int xHigh = -tilePos.first+maxStep;
        int yLow = -tilePos.second+-maxStep;
        int yHigh = -tilePos.second+maxStep;
        
        
        
        
        
        for (int counterX = xHigh; counterX>xLow; counterX--){
            counterTwo = 0;
            for (int counterY = yHigh; counterY>yLow; counterY--){
                
                
                glStencilFunc(GL_GEQUAL, counterTwo, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                renderWallL(counterX, counterY);
                counterTwo++;
                
                
            }
        }
        
        
        
        
    }
    
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_ALPHA_TEST);


}

void Tiles::stageElevenRender(){
    int value[2];
    
    std::pair<int,int> tilePos = findTile(cpv(Camera::posX, Camera::posY));
    
    
    value[0]=-1*tilePos.first;
    value[1]=-1*tilePos.second;
    
    int maxStep = 16*Camera::zoom;
    
 
    
    if (_vertexArrayWalls!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(_vertexArrayWalls);
        
        
        
        int xLow =  -tilePos.first+-maxStep;
        int xHigh = -tilePos.first+maxStep;
        int yLow = -tilePos.second+-maxStep;
        int yHigh = -tilePos.second+maxStep;
        
        for (int counterX = xHigh; counterX>xLow; counterX--){
            
            
            counterTwo = 0;
            for (int counterY = yHigh; counterY>yLow; counterY--){
                
                
                
                //glStencilFunc(GL_GEQUAL, counterTwo+counterOne, 0xFF);
                //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                returnNormalShader();
                renderWallR(counterX, counterY);
                counterTwo++;
            }
            
            counterTwo = 0;
            for (int counterY = yHigh; counterY>yLow; counterY--){
                
                
                
                
                returnNormalShader();
                renderWallL(counterX, counterY);
                
                glEnable(GL_STENCIL_TEST);
                glEnable(GL_ALPHA_TEST);
                glAlphaFunc(GL_GREATER, 0.05);
                
                
                glStencilFunc(GL_GEQUAL, counterTwo, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                
                returnNormalShader();
                renderSprites(counterX, counterY);
                
                if (counterX+(counterY*width)==Sprite::indexOfTestSprite){
                    returnNormalShader();
                    Sprite::testSpriteRender();
                }
                
                glDisable(GL_STENCIL_TEST);
                glDisable(GL_ALPHA_TEST);
                counterTwo++;
            }
            
            counterOne+=5;
        }
        
        
        if (true){
            glDisable(GL_STENCIL_TEST);
            glDisable(GL_ALPHA_TEST);
        }
    }
    
}

void Tiles::stageTwelveRender(){


    if (GameLogic::gameMode==TRAPMODE){
    
        for (
             std::map<size_t,Sprite*>::iterator IT = Sprite::trapsByUID.begin();
             IT != Sprite::trapsByUID.end();
             IT++){
        
            IT->second->renderSelect();
        
        }
    
    
    } else if (GameLogic::gameMode == PLAYMODE){
    
        
        
        
        /*
        for (
             std::map<size_t,Sprite*>::iterator IT = Sprite::peopleByUID.begin();
             IT != Sprite::peopleByUID.end();
             IT++){
            
            IT->second->renderFast();
            
        }
        */
    
        for (
             std::set<Sprite*>::iterator IT = Sprite::badPeople.begin();
             IT != Sprite::badPeople.end();
             IT++){
        ShaderManager::enableHalfTextureShader();
            (*IT)->renderFast();
            (*IT)->renderText();
        }
      
    
    
    }
    ShaderManager::enableHalfTextureShader();
    Player::mainPlayer->render();


}

void Tiles::render(){
    

    
    stageOneRender();
   
    
    
    
    
 
    
    
    //Render Bad Tiles
   
     stageTwoRender();
    

    stageFourRender();
    stageFiveRender();
    stageSixRender();
    stageSevenRender();
    stageEightRender();
    
    stageNineRender();
    stageThreeRender();
    stageTenRender();
    stageElevenRender();
    stageTwelveRender();
    
    
 
    //
    
    if (Bulldozer::test()){

        for (std::map<long, Bulldozer*>::iterator IT = Bulldozer::allBulldozers.begin();
             IT != Bulldozer::allBulldozers.end();
             IT++){
        
            cpVect thePos = IT->second->position;
            int cord = findTile( findTile(thePos));
            
             ShaderManager::enableSelectShader(true);
            
            glBindVertexArrayCon(_vertexArrayTiles);
            glBindTexture(GL_TEXTURE_2D, *tileDictVector[0]->texture);
            
            if (cord+Tiles::width+Tiles::width+2<Tiles::mapValues.size()){
                
                glDrawArrays( GL_TRIANGLES, cord*6, 6);
                glDrawArrays( GL_TRIANGLES, (cord+1)*6, 6);
                glDrawArrays( GL_TRIANGLES, (cord+2)*6, 6);
            
                glDrawArrays( GL_TRIANGLES, (cord+Tiles::width)*6, 6);
                glDrawArrays( GL_TRIANGLES, (cord+Tiles::width+1)*6, 6);
                glDrawArrays( GL_TRIANGLES, (cord+Tiles::width+2)*6, 6);
            
                glDrawArrays( GL_TRIANGLES, (cord+Tiles::width+Tiles::width)*6, 6);
                glDrawArrays( GL_TRIANGLES, (cord+Tiles::width+Tiles::width+1)*6, 6);
                glDrawArrays( GL_TRIANGLES, (cord+Tiles::width+Tiles::width+2)*6, 6);
            }
        }
    }
    
      
    
    ShaderManager::enableSelectShader(false);
    

}

void Tiles::updateSelection(){
    
    
    {
        std::vector<float> vertex;
        
        double multi=10.0;
        
        
        double leftMost = -multi*4;
        double rightMove = leftMost;
        double topMost =-35;
        double topMove = topMost;
        
        
        
        int lengthCount = 0;
        for (int counter =0; counter<6; counter++) {
            
            lengthCount++;
            
            
            
            
            
            // Create Tile
            vertex.push_back(-1.0*multi+rightMove);
            vertex.push_back(-T_V*multi+topMove);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(-1*multi+rightMove);
            vertex.push_back(T_V*multi+topMove);
            vertex.push_back(0);
            vertex.push_back(0);
            
            
            
            vertex.push_back(1*multi+rightMove);
            vertex.push_back(-T_V*multi+topMove);
            vertex.push_back(1);
            vertex.push_back(1);
            
            
            
            
            vertex.push_back(-1*multi+rightMove);
            vertex.push_back(T_V*multi+topMove);
            vertex.push_back(0);
            vertex.push_back(0);
            
            
            vertex.push_back(1*multi+rightMove);
            vertex.push_back(T_V*multi+topMove);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(1*multi+rightMove);
            vertex.push_back(-T_V*multi+topMove);
            vertex.push_back(1);
            vertex.push_back(1);
            
             tileButtons[counter]=cpBBNew(-1.0*multi+rightMove, -T_V*multi*2+topMove, 1*multi+rightMove, 0*multi+topMove);
            
            rightMove+=multi*2;
            
            
           
            
            //topMove+=T_V*multi;
        }
        
        if (!_vertexArraySelection){
            glGenVertexArraysCon(1, &_vertexArraySelection);
        }
        glBindVertexArrayCon(_vertexArraySelection);
        
        if(!_vertexBufferSelection){
            glGenBuffers(1, &_vertexBufferSelection);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferSelection);
        glBufferData(GL_ARRAY_BUFFER, vertex.size()*sizeof(float), &vertex[0], GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
    }


}


void Tiles::renderSelection(){
    
    if (_vertexBufferSelection){
    
    ShaderManager::enableUIShader();

    
    glBindVertexArrayCon(_vertexArraySelection);
        
    
        for (int counter =0; counter<1; counter++)
        {
            glBindTexture(GL_TEXTURE_2D, *tileDictVector[changeTileType]->texture);
            glDrawArrays(GL_TRIANGLES, counter*6, 6);
        }
    }
    
   
    
    
    

}


bool Tiles::grabMap(std::vector<char> * returnValue){
    
    FrameBufferTemp screenShot;
    FrameBufferTemp smallVersion;
    
    screenShot.initateFrameBuffer(2048, 2048);
    screenShot.enableFrameBuffer();
    
    
    glUseProgram(0);

    
    
    glViewport(0, 0, 2048, 2048);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0, 1000, 1000,0, 1, -1);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    
    
    
    
    
    
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    //Camera::grabEntireMap();
    ShaderManager::enableTextureShaderFullMap();
    
    if (_vertexArrayTiles!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(_vertexArrayTiles);
        for (int counterY =0; counterY <height; counterY++){
            int Yvalue = ((width)*(counterY));
            for (int counterX =0; counterX < width; counterX++) {
                int Xvalue = counterX;
                Tiles::mapValues[Yvalue+Xvalue].renderTile(Yvalue+Xvalue);
                

            }
        }
        
    }
    
    // Draw Selection
    
    if (_vertexArrayWalls!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(_vertexArrayWalls);
        
        for (int counterY =height-1; counterY >=0; counterY--){
            int Yvalue = ((width)*(counterY));
            for (int counterX = width-1 ; counterX >= 0; counterX--) {
                int Xvalue = counterX;
                
                TileObject * currentTile = &mapValues[Yvalue+Xvalue];
                /*if (Sprite::renderGrid[(Yvalue+Xvalue)/2].size()){
                    std::vector<Sprite*> * theVector;
                    theVector = &Sprite::renderGrid[(Yvalue+Xvalue)/2];
                    
                    for (std::vector<Sprite*>::iterator IT = theVector->begin();
                         IT!=theVector->end();
                         IT++){
                        (*IT)->render();
                        glBindVertexArrayCon(_vertexArrayWalls);
                    }
                }
                */
                if (currentTile->lw){
                    if (currentTile->textureWallL){
                    glBindTexture(GL_TEXTURE_2D, *currentTile->textureWallL);
                    glDrawArrays(GL_TRIANGLES, (Yvalue+Xvalue)*12, 6);
                }
                }
                
                if (currentTile->rw){
                    if (currentTile->textureWallR){
                    glBindTexture(GL_TEXTURE_2D, *currentTile->textureWallR);
                    glDrawArrays(GL_TRIANGLES, (Yvalue+Xvalue)*12+6, 6);
                    }
                    }
            }
        }
    }
    
    

    
    screenShot.disableFrameBuffer();
    smallVersion.initateFrameBuffer(256, 256);
    smallVersion.enableFrameBuffer();
    
    glViewport(0, 0, 256, 256);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    
    

    //Camera::setWindow(256, 256, -100, 100, -100, 100,1,-1);
    ShaderManager::enableTextureShaderMakeSmaller();
    
    GLuint _vertexArray=0;
    GLuint _vertexBuffer=0;
    
    GLfloat gBoxVertexData[] = {
        
        -100,
        100,
        0,
        0,
        
        
        -100,
        -100,
        0,
        1,
        
        100,
        100,
        1,
        0,
        
        100,
        -100,
        1,
        1
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
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenShot.textureId);
    glBindVertexArrayCon(_vertexArray);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    
    std::vector<char> rawData = smallVersion.save();
    returnValue->insert(returnValue->end(), rawData.begin(), rawData.end());
    
    smallVersion.disableFrameBuffer();
    smallVersion.deleteFrameBuffer();
    screenShot.deleteFrameBuffer();
    
    Camera::returnToNormal();


    
    //outputData("RawLevelData.raw", (char*)&rawData[0], rawData.size());
    
    return true;
   
}



void Tiles::createTile(std::vector<GLfloat> * vertex, GLfloat multi, GLfloat rightMove, GLfloat topMove ){
    
    
    {
        
        
        vertex->push_back(-1.0*multi+rightMove);
        vertex->push_back(-T_V*multi+topMove);
        vertex->push_back(0);
        vertex->push_back(1);
        
        vertex->push_back(-1*multi+rightMove);
        vertex->push_back(T_V*multi+topMove);
        vertex->push_back(0);
        vertex->push_back(0);
        
        
        
        vertex->push_back(1*multi+rightMove);
        vertex->push_back(-T_V*multi+topMove);
        vertex->push_back(1);
        vertex->push_back(1);
        
        
        
        
        vertex->push_back(-1*multi+rightMove);
        vertex->push_back(T_V*multi+topMove);
        vertex->push_back(0);
        vertex->push_back(0);
        
        
        vertex->push_back(1*multi+rightMove);
        vertex->push_back(T_V*multi+topMove);
        vertex->push_back(1);
        vertex->push_back(0);
        
        vertex->push_back(1*multi+rightMove);
        vertex->push_back(-T_V*multi+topMove);
        vertex->push_back(1);
        vertex->push_back(1);
        
        
        
        
    }
    
    
}

int Tiles::energyUsed(int start, int end){
    
    if (changeTileType<3){
        return 0;
    
    }

    int x1 = start%width;
    int x2 = end%width;
    
    int y1 = start/width;
    int y2 = end/width;
    
    return energyUsed(x1,y1,x2,y2);


}


int Tiles::energyUsed(int x1, int y1, int x2, int y2){
    
    if (changeTileType<3){
        return 0;
        
    }

    int numberOfTiles=0;
    
    int lowestX     =   (x1>x2)?x2:x1;
    int highestX    =   (x1>x2)?x1:x2;
    
    int lowestY     =   (y1>y2)?y2:y1;
    int highestY     =   (y1>y2)?y1:y2;
    
    //int cost = tileDictionary[changeTileType].energyRequired;
    
    for (int counterX =  lowestX; counterX<=highestX; counterX++){
        for (int counterY = lowestY; counterY<=highestY; counterY++){
            
            int type = mapValues[counterX+counterY*width].type;
            if (type!=changeTileType){
                
                
                numberOfTiles++;
            }
            
            
        }
        
    }
    
    
    
    
    return (int)(numberOfTiles*tileDictionary[(int)changeTileType]->energyRequired);


}



int Tiles::returnInfluenceUsed(int start, int end){

    int x1 = start%width;
    int x2 = end%width;
    
    int y1 = start/width;
    int y2 = end/width;

    return returnInfluenceUsed(x1,y1,x2,y2);

}


int Tiles::returnInfluenceUsed(int x1, int y1, int x2, int y2){
    
    int numberOfTiles=0;
    
    int lowestX     =   (x1>x2)?x2:x1;
    int highestX    =   (x1>x2)?x1:x2;
    
    int lowestY     =   (y1>y2)?y2:y1;
    int highestY     =   (y1>y2)?y1:y2;
    
    
    
    for (int counterX =  lowestX; counterX<=highestX; counterX++){
        for (int counterY = lowestY; counterY<=highestY; counterY++){
        
            int type = mapValues[counterX+counterY*width].type;
            if (type!=0&&type!=1){
                
                numberOfTiles++;
            }

        
        }
    
    }
    
    

    return numberOfTiles;

}


bool Tiles::hasBeenCheckedForWalkability = false;


void Tiles::killLostWalls(int left, int down, int right, int up){
	

    if (left!=0){
        left-=1;
    
    }
    if (right!=Tiles::width-1){
        right+=1;
    
    }
  
    if (down!=0){
        down-=1;
    }
    if (up!=Tiles::width-1){
        up +=1;
    }
    
    //FIX
   
    left-=10;
    right+=10;
    up+=10;
    down-=10;
    
   
    for (int yPos = down; yPos<=up;yPos++){
        for (int xPos = left; xPos<=right;xPos++){
            if (yPos<1||xPos<1||xPos>=Tiles::width||yPos>=Tiles::width){
                continue;
            }
            
            TileObject * theObject = &mapValues[xPos+yPos*Tiles::width];
            
            if (theObject->lw||theObject->rw){
                TileObject * rightT = theObject+1;
                
                TileObject * leftT = theObject-1;
                
                //TileObject * rightB =theObject-Tiles::width+1;
                //TileObject * leftB = theObject-Tiles::width-1;
                TileObject * topT = theObject+Tiles::width;
                TileObject * botT = theObject-Tiles::width;
                
                
                if (theObject->lw){
                    if ((leftT->theDoorL||leftT->lw)&&(rightT->lw||rightT->theDoorL)){
                        
                    } else {
                        
                        if (!(leftT->lw||leftT->theDoorL)){
                            
                            //theObject->lw=false;
                            //theObject->type = 7;
                            
                            if (theObject->rw||botT->rw||theObject->theDoorR||botT->theDoorR){
                                
                                
                            } else {
                                theObject->lw=false;
                                if (theObject->theDoorL) {
                                    Door::removeDoor(xPos+yPos*Tiles::width, false);
                                
                                }
                               
                            }
                            
                            
                        }
                        
                        
                        
                    }
                }
                
                if (theObject->rw){
                    if ((topT->theDoorR||topT->rw)&&(botT->rw||botT->theDoorL)){
                        
                        
                    } else {
                        if (!(botT->rw||botT->theDoorR)){
                        
                            if (theObject->lw||leftT->lw||theObject->theDoorL||leftT->theDoorL){
                            
                            
                            } else {
                                //theObject->type=7;
                                 theObject->rw=false;
                                if (theObject->theDoorR){
                                    Door::removeDoor(xPos+yPos*Tiles::width, true);
                                }
                            }
                            
                            
                        
                        }
                    
                    }
                    
                }
                
                
            }
        }
     }
    
    
    
    for (int yPos = up; yPos>=down;yPos--){
        for (int xPos = right; xPos>=left;xPos--){
            if (yPos<1||xPos<1||xPos>=Tiles::width||yPos>=Tiles::width){
                continue;
            }
            
            TileObject * theObject = &mapValues[xPos+yPos*Tiles::width];
            
            if (theObject->lw||theObject->rw){
                TileObject * rightT = theObject+1;
                
                TileObject * leftT = theObject-1;
                TileObject * leftT_top = theObject-1+Tiles::width;
                
                TileObject * rightB =theObject-Tiles::width+1;
                //TileObject * leftB = theObject-Tiles::width-1;
                TileObject * topT = theObject+Tiles::width;
                TileObject * botT = theObject-Tiles::width;
                
                
                if (theObject->lw){
                    if ((leftT->theDoorL||leftT->lw)&&(rightT->lw||rightT->theDoorL)){
                        
                    } else {
                        
                        if (!(rightT->lw||rightT->theDoorL)){

                            if (rightT->rw||rightB->rw||rightT->theDoorR||rightB->theDoorR){
                                
                                
                            } else {
                                
                                theObject->lw=false;
                                if (theObject->theDoorL){
                                Door::removeDoor(xPos+yPos*Tiles::width, false);
                                }
                            }
                            
                            
                        }
                        
                        
                        
                    }
                }
                
                
                if (theObject->rw){
                    if ((topT->theDoorR||topT->rw)&&(botT->rw||botT->theDoorL)){
                        
                        
                    } else {
                        if (!(topT->rw||topT->theDoorR)){
 
                             if (topT->lw||leftT_top->lw||topT->theDoorL||leftT_top->theDoorL){
                                
                            } else {
                                theObject->rw=false;
                                if (theObject->theDoorR){
                                Door::removeDoor(xPos+yPos*Tiles::width, true);
                                }
                            }
                            
                            
                            
                        }
                        
                    }
                    
                }
                
                
            }
        }
    }
    
      
        
        
        
        
        
        
   
    
}


size_t Tiles::checkWalkAbility(){


    GameLogic::numberOfTilesInPlay = 0;
    
    
    
    //getAllWalkTiles();
    
    
    for (std::vector<TileObject>::iterator IT = mapValues.begin();
         IT != mapValues.end();
         IT++){
        
        TileObject * theObject = &(*IT);
        theObject->canWalkThere = false;
        

    }
    

    
    
    
     findHouseDimensions();
    
    
    quickSearchSetup();
    
    
    std::vector<int> testedTiles;
    std::vector<int> testThese;

    testThese.push_back(69);
    
    GameLogic::awarenessSink = ModVars::reputationBuffer;
    
    while (testThese.size()) {

        int index = testThese.back();
        testThese.pop_back();
        
        if (Tiles::canMove(index, L,2)){
            if (quickSearch[index-1] != t_check){
                quickSearch[index-1] = t_check;
                testThese.push_back(index-1);
            }
        }
        if (Tiles::canMove(index, U,2)){
            if (quickSearch[index+width] != t_check){
                quickSearch[index+width] = t_check;
                testThese.push_back(index+width);
            }
        
        }
        if (Tiles::canMove(index, R,2)){
            if (quickSearch[index+1] != t_check){
                quickSearch[index+1] = t_check;
                testThese.push_back(index+1);
            }
        
        }
        if(Tiles::canMove(index, D,2)){
            if (quickSearch[index-width] != t_check){
                quickSearch[index-width] = t_check;
                testThese.push_back(index-width);
            }
        }
        quickSearch[index]=t_check;
        
        TileObject * tile = &mapValues[index]; //
        
        if (tile->type>2){
        testedTiles.push_back(index);
        
        }
        


        tile->canWalkThere = true;
        GameLogic::numberOfTilesInPlay+=Tiles::tileDictVector[tile->type]->energyRequired;
        
        if (tile->occupiedSprite){
            SpriteDict * dict = tile->occupiedSprite->dict;
            
            if (dict->prop){
                
                if (!dict->animates){
                    
                    if (tile->occupiedSprite->renderGridIndex==index){
                        
                        GameLogic::awarenessSink += tile->occupiedSprite->dict->cost*ModVars::costToAwarenessRatio;
                    }

                }
                
            }
        }
    }
        
      
    
    
    
    
   // std::vector<int> finalValues;
    
   // std::sort(testedTiles.begin(), testedTiles.end());
    
    //std::set_intersection(usedTiles.begin(), usedTiles.end(), testedTiles.begin(), testedTiles.end(),
   //                       std::back_inserter(finalValues));
    //usedTiles =std::set<int>(finalValues.begin(), finalValues.end());
    
    //GameLogic::numberOfTilesInPlay = 0;
    

    usedTiles.swap(testedTiles);
    
    hasBeenCheckedForWalkability = true;
    Sprite::outSideHouseCheck = false;

    return  usedTiles.size();

}



std::vector<int> Tiles::tilesOutSideOfHouse(){


    
    std::vector<int> thePoints;

    if (houseDimension[0]>0){
        
    
        for (int counter = houseDimension[3]; counter < houseDimension[1]; counter++){
        
            thePoints.push_back((counter*Tiles::width)+houseDimension[0]-1);


        }
        
    }

    if (houseDimension[2]>=width-2){
        
        
        for (int counter = houseDimension[3]; counter < houseDimension[1]; counter++){
            
            thePoints.push_back((counter*Tiles::width)+houseDimension[2]+1);
            
            
        }
        
    }
    
    if (houseDimension[1]>=width-2){
        
        
        for (int counter = houseDimension[0]; counter < houseDimension[2]; counter++){
            
            thePoints.push_back(counter+(houseDimension[1]+1)*width);
            
            
        }
        
    }
    
    return thePoints;

}

bool Tiles::insideHouse(int index){

    int xPos = index%Tiles::width;
    int yPos = index/Tiles::width;
    
    if (xPos>houseDimension[0]&&xPos<houseDimension[2]){
        if (yPos>houseDimension[3]&&yPos<houseDimension[1]){
        
            return true;
        }
        
    }

    return false;
}


void Tiles::findHouseDimensions(){
    
    int bottom=-1;
    int left=-1;;
    int top = -1;
    int right = -1;
    
    for (int counter = 0; counter<mapValues.size() ; counter++){
        if (mapValues[counter].type>3){
            bottom = counter/width;
            left = counter%width;
            break;
        }
    }
    
    if (bottom==-1){
        //NO_HOUSE;
        return;
    
    }
    
    
    
    for (int counter = (int)mapValues.size()-1; counter>=0 ; counter--){
        
        if (mapValues[counter].type>3){
            top = counter/width;
            right = counter%width;
            
            break;
        }
        
        
    }
    
    if (top==-1){
        //NO_HOUSE;
        return;
        
    }
    
    
    for (int counterX = 0; counterX < left; counterX++){
        for (int counterY = bottom; counterY<top; counterY++) {
       
        
        int yPos = counterY*width;
        int xPos = counterX;
        
        if (mapValues[xPos+yPos].type>3){
        
            
                left = xPos;
            break;
            
            }
        
        }
        
    
    }
    
    
    for (int counterX = width-1; counterX >= right; counterX--){
        for (int counterY = top; counterY>bottom; counterY--) {
            
            
            int yPos = counterY*width;
            int xPos = counterX;
            
            if (mapValues[xPos+yPos].type>3){
                
                
                right = xPos;
                break;
                
            }
            
        }
        
        
    }
    
    

    houseDimension[0]=left;
    houseDimension[1]=top;
    houseDimension[2]=right;
    houseDimension[3]=bottom;
    
    
}
    
    
    
    
    
    
    
    
    
    




void Tiles::getAllWalkTiles(){
    
    hasBeenCheckedForWalkability=false;
    usedTiles.clear();
    
    for (std::vector<TileObject>::iterator IT = mapValues.begin();
         IT != mapValues.end();
         IT++){
        
        TileObject * theObject = &(*IT);
        theObject->canWalkThere = false;
        
        if (doesThisTileMakeWalls(theObject->type)){
            if (!theObject->block){
                
                size_t counter = IT - mapValues.begin();
                usedTiles.push_back((int)counter);
            }
        }
    }
    
    
    findHouseDimensions();
}

int Tiles::getRandomTileHouse(){
    
    std::vector<int>::iterator IT = usedTiles.begin();
    size_t size = usedTiles.size();
    
    if (size!=0){
        size_t theTotalSize = rand()%size;

        advance(IT,theTotalSize);
        return (*IT);
    } else {
    
        return width*height-1;
        
    }
    
    

}


bool Tiles::isTileOccupied(int index){

    if (index>= 0 && index <= mapValues.size()){
        
        bool occupied = (Tiles::mapValues[index].renderGrid.size())?true:false;
        
        if (!occupied){
            if (Tiles::mapValues[index].occupiedSprite){occupied=true;}
        }
        
        return occupied;
    
    } else {
        return false;
    
    }


}



std::vector<Sprite*> Tiles::grabAllSpriteLos(int _index, int radius){
    
    std::vector<Sprite*> theSprites;
    
    quickSearchSetup();

    
    
    std::vector<int> examineThese;
    std::vector<int> goodTiles;
 
    
    examineThese.push_back(_index);
    
    while (examineThese.size()>0) {
        
        int index = examineThese.back();
        examineThese.pop_back();
       
        
        
        int xPosS = _index%Tiles::width;
        int yPosS = _index/Tiles::width;
        
        int xPosE = index%Tiles::width;
        int YPosE = index/Tiles::width;
        
        if (abs(xPosE-xPosS)>radius||abs(YPosE-yPosS)>radius){
            continue;
        }
        
        quickSearch[index]=t_check;
        goodTiles.push_back(index);
        
        if (Tiles::canMove(index, L,2)){
            if (quickSearch[index-1] != t_check){
                quickSearch[index-1] = t_check;
                examineThese.push_back(index-1);
            }
        
        }
        
        if (Tiles::canMove(index, R,2)){
            if (quickSearch[index+1] != t_check){
                quickSearch[index+1] = t_check;
                examineThese.push_back(index+1);
            }
        }
        
        if (Tiles::canMove(index, U,2)){
            if (quickSearch[index+width] != t_check){
                quickSearch[index+width] = t_check;
                examineThese.push_back(index+width);
            }
        }
        
        if (Tiles::canMove(index, D,2)) {
            if (quickSearch[index-width] != t_check){
                quickSearch[index-width] = t_check;
            examineThese.push_back(index-width);
            }
        }
        
        
        
    }
    
    for (std::vector<int>::iterator IT = goodTiles.begin();
         IT != goodTiles.end();
         IT++){
        
        
        std::vector<Sprite*> * aGrid = &Tiles::mapValues[*IT].renderGrid;
    
        for (std::vector<Sprite*>::iterator IT2 = aGrid->begin();
             IT2 != aGrid->end();
             IT2++){
            
            if ((*IT2)->dict->index>0){
                if (!(*IT2)->dict->prop){
                    theSprites.push_back(*IT2);
                }
            }
        }
    }
    
    return theSprites;
    
}



std::vector<Sprite*> Tiles::grabAllTrapsInRoom(int index){

    printf("DEad Function 11");
    
    
    

    std::vector<int> tilesInRoom = getAllTilesInRoom(index);

    std::vector<Sprite*> theReturner;
    
    for (std::vector<int>::iterator IT = tilesInRoom.begin();
         IT != tilesInRoom.end();
         IT++){
        
        TileObject * theObject = &Tiles::mapValues[*IT];
    
        if (theObject->occupiedSprite){
            if (theObject->occupiedSprite->dict->prop){
                if (theObject->occupiedSprite->dict->animates){
                    theReturner.push_back(theObject->occupiedSprite);
                }
            }
        }
    }

    return theReturner;
}


std::vector<Sprite*> Tiles::grabAllTraps(int index, int radius){
    
    
    std::pair<int,int> position = Tiles::findTile(index);
    
    int xMin=-radius;
    
    if (position.first<radius+1){
        xMin = -position.first;
    }
    
    int xMax=radius;
    
    if (position.first+radius>=Tiles::width){
        xMax = Tiles::width-position.first;
        
    }
    
    
    int yMin=-radius;
    
    if (position.second<radius+1){
        yMin = -position.second;
    }
    
    int yMax=radius;
    
    if (position.second+radius>=Tiles::width){
        yMax = Tiles::width-position.first;
        
    }
    
    std::vector<Sprite*> theSprites;
    
    for (int counterX = position.first+xMin;
         counterX<position.first+xMax+1;
         counterX++){
        
        for (int counterY=position.second+yMin;
             counterY<position.second+yMax+2;
             counterY++){
            
            int target = counterX + counterY*Tiles::width;
            
            std::vector<Sprite*> * grid = &Tiles::mapValues[target].renderGrid;
            
            for (std::vector<Sprite*>::iterator IT = grid->begin(); IT != grid->end();IT++){
                
                if ((*IT)->dict->animates){
                    if ((*IT)->dict->prop){
                        
                        theSprites.push_back((*IT));
                    }
                }
            }
        }
    }
    
    return theSprites;
    
}

std::vector<Sprite*> Tiles::grabAllSprite(int index, int radius){

    
    
    std::pair<int,int> position = Tiles::findTile(index);
    
    int xMin=-radius;
    
    if (position.first<radius+1){
        xMin = -position.first;
    }
    
    int xMax=radius;
    
    if (position.first+radius>=Tiles::width){
        xMax = Tiles::width-position.first;
        
    }
    
    
    int yMin=-radius;
    
    if (position.second<radius+1){
        yMin = -position.second;
    }
    
    int yMax=radius;
    
    if (position.second+radius>=Tiles::width){
        yMax = Tiles::width-position.first;
        
    }
    
    std::vector<Sprite*> theSprites;
    
    for (int counterX = position.first+xMin;
         counterX<position.first+xMax+1;
         counterX++){
        
        for (int counterY=position.second+yMin;
             counterY<position.second+yMax+2;
             counterY++){
            
            int target = counterX + counterY*Tiles::width;
            
            std::vector<Sprite*> * grid = &Tiles::mapValues[target].renderGrid;
            
            for (std::vector<Sprite*>::iterator IT = grid->begin(); IT != grid->end();IT++){
                
                if ((*IT)->dict->index>=0){
                    if (!(*IT)->dict->prop){
                        theSprites.push_back((*IT));
                    }
                }
            }
        }
    }

    return theSprites;
    
}




void Tiles::tileShowRadius(int _index, int radius){

    quickSearchSetup();

    std::pair<int,int> position = Tiles::findTile(_index);
    
    int xMin=-radius+position.first;
    
    if (position.first<radius+1){
        xMin = 0;
    }
    
    int xMax=radius+position.first;
    
    if (position.first+radius>=Tiles::width){
        
        xMax = Tiles::width-1;
    }
    
    
    int yMin=-radius+position.second;
    
    if (position.second<radius+1){
        yMin = 0;
    }
    
    int yMax=radius+position.second;
    
    if (position.second+radius>=Tiles::width){
        yMax = Tiles::width-1;
        
        
    }
    
    std::vector<Sprite*> theSprites;
    
    
    quickSearchSetup();
    
    std::vector<int> examineThese;
    std::vector<int> goodTiles;
    
    examineThese.push_back(_index);
    
    while (examineThese.size()>0) {
        
  
        
        
        
        int index = examineThese.back();
        examineThese.pop_back();
        quickSearch[index] = t_check;
        
        
        
        int xPosS = _index%Tiles::width;
        int yPosS = _index/Tiles::width;
        
        int xPosE = index%Tiles::width;
        int yPosE = index/Tiles::width;
        
        if (abs(xPosS-xPosE)>radius||abs(yPosS-yPosE)>radius){
            continue;
        }
        
        goodTiles.push_back(index);
        
 
        
        if (Tiles::canMove(index, L,2)){
             if (quickSearch[index-1] != t_check){
                 quickSearch[index-1] = t_check;
                examineThese.push_back(index-1);
            }
            
        }
        
        if (Tiles::canMove(index, R,2)){
            if (quickSearch[index+1] != t_check){
                quickSearch[index+1] = t_check;
                examineThese.push_back(index+1);
            }
        }
        
        if (Tiles::canMove(index, U,2)){
            if (quickSearch[index+width] != t_check){
                quickSearch[index+width] = t_check;
                examineThese.push_back(index+width);
            }
        }
        
        if (Tiles::canMove(index, D,2)){
            if (quickSearch[index-width] != t_check){
                quickSearch[index-width] = t_check;
                examineThese.push_back(index-width);
            }
        }
        
      
        
        
    }
    
    std::sort(goodTiles.begin(), goodTiles.end());
    

   
    hoverTileSelected = std::set<int>(goodTiles.begin(),goodTiles.end());
    


}

cpVect Tiles::defTile(cpVect pos){

    return findCords(findTile(pos));



}



bool Tiles::canMove2I(int start, int end){

    std::pair<int, int> s = Tiles::findTile(start);
    std::pair<int, int> e = Tiles::findTile(end);
  
    int direction = (s.first!=e.first)?(s.first>e.first)?0:2:(s.second<e.second)?1:3;
    
    return Tiles::canMove(start, direction);
    
}


bool Tiles::valueSafe(int index){
    if (index<0){
        return false;
    }
    if (index>=Tiles::width*Tiles::width){
        return false;
    }
    return true;
}
bool Tiles::valueSafe(int index, int backOffset, int forwardOffset){

    if (index-backOffset<0){
        return false;
    }
    if (index+forwardOffset>=Tiles::width*Tiles::width){
        return false;
    }

    return true;

}

