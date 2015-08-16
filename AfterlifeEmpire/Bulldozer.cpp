//
//  Bulldozer.cpp
//  After Life Empire
//
//  Created by Autobotika on 2015-04-10.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#include "Bulldozer.h"
#include "Tiles.h"
#include "GlobalDebugger.h"
#include "UnitFinder.h"

std::map<long, Bulldozer*> Bulldozer::allBulldozers;

bool Bulldozer::test(){
    return false;

}

cpVect Bulldozer::spawnBulldozer(){

    int theTile = Tiles::getRandomTileHouse();
    int yPos = theTile/Tiles::width;
    
    int xPos = Tiles::width;
    
    Bulldozer * newBullDozer = new Bulldozer;
    newBullDozer->didItMove = -1;
    
    newBullDozer->generate(std::pair<int, int>(xPos,yPos), -2, false);
    UnitFinder::allFinders[newBullDozer->UID]=UnitFinder();
    UnitFinder::allFinders[newBullDozer->UID].aSprite =newBullDozer;

    UnitFinder * item = &UnitFinder::allFinders[newBullDozer->UID];
    item->danger = true;;
    
    allBulldozers[newBullDozer->UID]=newBullDozer;
    
    
    return newBullDozer->position;

}



void Bulldozer::thinkAll(){

	std::vector<long> killThese;

    for (std::map<long, Bulldozer*>::iterator IT = allBulldozers.begin();
         IT!=allBulldozers.end();
         IT++){
        IT->second->think();
    

		if (IT->second->kill){
			killThese.push_back(IT->first);
			IT->second->destroy(true);
			delete IT->second;
		
		}
    }
    
	for (std::vector<long>::iterator IT = killThese.begin();
		IT != killThese.end();
		IT++){

		allBulldozers.erase(*IT);
	}
    
}


bool Bulldozer::think(){
    
    
    frame++;
    
    if (frame>=dict->frames[1]){
        frame=0;
    }
    
    cpVect thePos = position;
    
    cpVect cordA = Tiles::findCords(std::pair<int,int>(0,0));
    cpVect cordB = Tiles::findCords(std::pair<int,int>(1,0));
    
    cpVect amount = cpvsub(cordA, cordB);
    amount = cpvmult(amount, 1.0/60.0);
    position =  cpvadd(thePos, amount);
    
    thePos.x +=-6;
    thePos.y +=-6;
   
    std::pair<int, int> pos = Tiles::findTile(thePos);
    
    
    int widthAmount = Tiles::width-pos.first;
    
    if (widthAmount<1){
        return true;
    }
    
    if (widthAmount>1){
        widthAmount=1;
    }
    
    
   
    int cord = Tiles::findTile( pos);
    
    
    int xPosB = cord%Tiles::width;
    int yPosB = cord/Tiles::width;


    
    

    
    if (didItMove != xPosB+yPosB*15){
    
        if (xPosB<0||yPosB<0||xPosB>Tiles::width*Tiles::width||yPosB>Tiles::width*Tiles::width){
        
        } else {
        
            
            didItMove=xPosB+yPosB*15;
            Tiles::fillTilesMakeWalls(xPosB, yPosB, xPosB+widthAmount, yPosB+widthAmount, 2);
            
            if (Tiles::mapValues[xPosB+yPosB*Tiles::width].occupiedSprite){
                Sprite::removeSprite(Tiles::mapValues[xPosB+yPosB*Tiles::width].occupiedSprite);
            }
            if (Tiles::mapValues[xPosB+1+yPosB*Tiles::width].occupiedSprite){
                Sprite::removeSprite(Tiles::mapValues[xPosB+yPosB*Tiles::width].occupiedSprite);
            }
            
            if (Tiles::mapValues[xPosB+yPosB*Tiles::width+Tiles::width].occupiedSprite){
                Sprite::removeSprite(Tiles::mapValues[xPosB+yPosB*Tiles::width].occupiedSprite);
            }
            if (Tiles::mapValues[xPosB+1+yPosB*Tiles::width+Tiles::width].occupiedSprite){
                Sprite::removeSprite(Tiles::mapValues[xPosB+yPosB*Tiles::width].occupiedSprite);
            }
        
        }
        
    }
    
    
    


    if (pos.first<-1){
        
		kill = true;
		return false;

    
    }
    
    
    std::vector<Sprite*> spritesAroundBulldozer = Tiles::grabAllSprite(cord, 4);
    
    for (std::vector<Sprite*>::iterator IT = spritesAroundBulldozer.begin();
         IT != spritesAroundBulldozer.end();
         IT++){
    if (!(*IT)->testTrait(BULLDOZER, 300)){
        (*IT)->scareFast(thePos);
        
            (*IT)->addTrait(BULLDOZER);
        }
    
    }
    
    spritesAroundBulldozer = Tiles::grabAllSprite(cord, 2);
    
    for (std::vector<Sprite*>::iterator IT = spritesAroundBulldozer.begin();
         IT != spritesAroundBulldozer.end();
         IT++){
        if (!(*IT)->testTrait(BULLDOZER, 300)){
(*IT)->addTrait(BULLDOZER);
        (*IT)->setAnimationState(9);
        }
    }
    
    
    return true;
    

}


void Bulldozer::updateAll(){
    
    for (std::map<long, Bulldozer*>::iterator IT = allBulldozers.begin();
         IT!=allBulldozers.end();
         IT++){
        
        IT->second->update();
        
        cpVect pos = IT->second->position;
        
        int index = Tiles::findTile(Tiles::findTile(pos));
        if (index>=0&&Tiles::mapValues.size()>index){
        
        Tiles::mapValues[index].renderGrid.push_back(IT->second);
        
        Sprite::gridsLastTurn.insert(index);
        }
        
    }

}

void Bulldozer::purgeAll(){


	for (std::map<long, Bulldozer*>::iterator IT = allBulldozers.begin();
		IT != allBulldozers.end();
		IT++){
		IT->second->destroy(true);
		delete IT->second;
	}
}


void Bulldozer::renderAll(){
    for (std::map<long, Bulldozer*>::iterator IT = allBulldozers.begin();
         IT!=allBulldozers.end();
         IT++){
        
        IT->second->render();
        
    }

}