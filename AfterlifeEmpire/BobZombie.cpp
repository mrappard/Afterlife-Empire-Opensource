//
//  BobZombie.cpp
//  After Life Empire
//
//  Created by Autobotika on 2015-05-27.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#include <stdio.h>
#include "BobZombie.h"
#include "TextureGenerator.h"
#include "FileManager.h"
#include "GlobalDebugger.h"
#include "GameLogic.h"
#include <map>
#include "ShaderManager.h"
#include "Tweener.h"
#include "UserInterface.h"
#include <algorithm>    // std::min_element, std::max_element
#include "Tiles.h"
#include "TextureGenerator.h"
#include "Player.h"
#include "Camera.h"
#include "TranslationMatrix.h"
#include "SetupVars.h"
#include "AudioController.h"


#define BODY_WIDTH          669
#define BODY_HEIGHT         1280
#define BODY_XPOS           425
#define BODY_YPOS           20

#define RIGHT_WIDTH         208
#define RIGHT_HEIGHT        362
#define RIGHT_XPOS          918
#define RIGHT_YPOS          1017

#define RIGHT_ROT_X         971
#define RIGHT_ROT_Y         1061

#define LEFT_ARM_WIDTH      212
#define LEFT_ARM_HEIGHT     326
#define LEFT_ARM_XPOS       456
#define LEFT_ARM_YPOS       836

#define LEFT_ARM_ROT_X      586
#define LEFT_ARM_ROT_Y      1114

#define LEFT_HAND_WIDTH     182
#define LEFT_HAND_HEIGHT    212
#define LEFT_HAND_XPOS      369
#define LEFT_HAND_YPOS      948

#define LEFT_HAND_ROT_X      480
#define LEFT_HAND_ROT_Y      1105




#define HEAD_XPOS           611
#define HEAD_YPOS           1147
#define HEAD_WIDTH          290
#define HEAD_HEIGHT         395

#define PUPIL_WIDTH         17
#define PUPIL_HEIGHT        17
#define PUPIL_XPOS          717
#define PUPIL_YPOS          1334

#define WINK_WIDTH          76
#define WINK_HEIGHT         81
#define WINK_XPOS           693
#define WINK_YPOS           1305


int BobZombie::TEENAGER_A = 22;
int BobZombie::TEENAGER_B = 24;
int BobZombie::EXORCIST_A= 29;
int BobZombie::POLICE_A = 17;

GLuint BobZombie::_vertexArray_Arrow = 0;
GLuint BobZombie::_vertexBuffer_Arrow = 0;

GLuint * BobZombie::_textureArrow = 0;


GLuint * BobZombie::bodyTexture       = 0;
GLuint * BobZombie::headTexture       = 0;
GLuint * BobZombie::rightArmTexture   = 0;
GLuint * BobZombie::pupilTexture      = 0;


GLuint * BobZombie::leftArmTexture = 0;
GLuint * BobZombie::leftHandTexture = 0;

GLuint * BobZombie::winkTexture = 0;

GLuint BobZombie::_vertexArray = 0;
GLuint BobZombie::_vertexBuffer = 0;

GLuint  BobZombie::nextWink=0;
GLuint  BobZombie::winkTime=0;

GLfloat BobZombie::pupilTween=0;
bool BobZombie::pupilInOrOut = true;

GLfloat BobZombie::leftArmTween=0;
bool BobZombie::leftArmInOrOut=true;

GLfloat BobZombie::leftHandTween=0;
bool BobZombie::leftHandInOrOut=true;

GLfloat BobZombie::rightArmTween =0;
bool BobZombie::rightHandInOrOut = true;

GLuint BobZombie::_vertexArray_Bob=0;
GLuint BobZombie::_vertexBuffer_Bob=0;
GLuint BobZombie::_sizeOfBob=0;


GLuint BobZombie::_loadingBarArray=0;
GLuint BobZombie::_loadingBarBuffer=0;
GLuint BobZombie::_loadingBarSize=0;

GLuint BobZombie::_vertexArray_BlackBox=0;
GLuint BobZombie::_vertexBuffer_BlackBox=0;

bool BobZombie::cameraMoveOn = false;
cpVect BobZombie::cameraMove;

size_t BobZombie::checkValue;


SpriteDict * BobZombie::theTargetSprite = 0;

CBitmapFont BobZombie::theText;
CBitmapFont BobZombie::theTitle;
CBitmapFont BobZombie::helpText;
CBitmapFont BobZombie::traitText;



CBitmapFont BobZombie::escText;


CBitmapFont BobZombie::setH;
CBitmapFont BobZombie::setF;


GLfloat BobZombie::yPosSlide=0;
GLfloat BobZombie::yPosMax=0;
size_t BobZombie::tutorailKey= -1;
GLfloat BobZombie::timer;
bool BobZombie::toggle = false;
bool BobZombie::resetText = true;
bool BobZombie::tutorialText = true;

size_t BobZombie::tick = 0;

int BobZombie::waitForEffect = -1;
int BobZombie::floatingVar = INT_MAX;

bool BobZombie::hasSelected[6] = {false,false,false,false,false,false};

std::vector<int> BobZombie::bobEventTest;


TutorialEnum BobZombie::currentTutorialState = NO_TUTORIAL;

float findLargestOf8Values(GLfloat * array, int * index ){
    
    
    float values[8];
    
    memcpy(values, array, 8*sizeof(float));
    
    float maxValue = -1;
    float maxIndex = 0;
    
    for (int counter =0; counter<8; counter++){
        
        values[counter]= (values[counter]<0)?-values[counter]:values[counter];
        
        if (values[counter]>maxValue){
            maxValue = values[counter];
            maxIndex = counter;
        }
    }
    
    *index = maxIndex;
    return maxValue;
    
}

float findLargestOf8Values(GLfloat * array){
    
    float values[8];
    
    memcpy(values, array, 8*sizeof(float));
    
    for (int counter =0; counter<8; counter++){
        values[counter]= (values[counter]<0)?-values[counter]:values[counter];
    }
    
    float * maxValue = std::max_element(values, values+7);
    return *maxValue;
    
}

static  std::pair<GLfloat, GLfloat> rotate(GLfloat x1,
                                               GLfloat y1,
                                               GLfloat x2,
                                               GLfloat y2,
                                                double r){
    
    GLfloat newX = x1 + (x2-x1)*cos(r) - (y2-y1)*sin(r);
    GLfloat newY = y1 + (x2-x1)*sin(r) + (y2-y1)*cos(r);
    

    return std::pair<GLfloat, GLfloat>(newX,newY);

}


//-----------------------------------------------------------------------------
// Purpose: Used when the Player end the tutorial early
//			Return the game to a state before the Tutorial
//-----------------------------------------------------------------------------
void BobZombie::returnNormal(){
    
    GDebug::lastFunction = DEBUG_BOB_RETURN_NORMAL;
  
    if (currentTutorialState !=NO_TUTORIAL) {
        
        std::vector<Sprite*> killThese;
        
        for (std::map<size_t,Sprite*>::iterator IT = Sprite::peopleByUID.begin();
             IT != Sprite::peopleByUID.end();
             IT++){
            
            killThese.push_back(IT->second);
        
        }
    
        for (std::vector<Sprite*>::iterator IT = killThese.begin();
             IT != killThese.end();
             IT++){
        
            Sprite::removeSprite(*IT);
        }
        
        

    restSlideState();
	cameraMoveOn = false;
    currentTutorialState = NO_TUTORIAL;
    toggle = false;
    
    
    
    if (toggle){
        
        if (timer<0){
            timer=0;
        }
        
    } else {
        
        if (timer>2.0){
            timer=2.0;
        }
        
    }

	GameLogic::energy = 1000;
	GameLogic::awareness = 0;
    
    UserInterface::despawnTextBox();
    Player::mainPlayer->path.clear();
    
    }
}



//-----------------------------------------------------------------------------
// Purpose: Sets the main text string for Bob
//
//-----------------------------------------------------------------------------
void BobZombie::setText(std::string newText){

    theText.setText(newText);

}



//-----------------------------------------------------------------------------
// Purpose: Tell the Player that they've reached a new level
//
//-----------------------------------------------------------------------------
void BobZombie::levelUp(int newLevel){
    if (GameLogic::reputationLevels.upper_bound(newLevel)!=GameLogic::reputationLevels.end()){
    
  
    std::string levelUpText = GameLogic::reputationLevels[newLevel].levelUpText;
    
        theTargetSprite = 0;
        BobZombie::setText(levelUpText);
        currentTutorialState = LEVEL_UP;
        BobZombie::toggleState();
        restSlideState();
    }

}




//-----------------------------------------------------------------------------
// Purpose: Used during the Tick Cycle to handle all the non render based settings
//
//-----------------------------------------------------------------------------
void BobZombie::think(){
    GDebug::lastFunction = DEBUG_BOB_THINK;
    if (toggle){
        timer += 1.0/30.0;
    } else {
        timer -= 1.0/30.0;
    }
   
    BobZombie::tick++;
    
    yPosSlide-=GameLogic::height/50.0;
    
    if (yPosMax>yPosSlide){
        yPosSlide=yPosMax;
    
    }
    

    
    theText.yPos = yPosSlide;
    
    
    //Used to Automatically move the Camera during the Tutorial
    if (cameraMoveOn){
        
        
        
        cpVect current = cpv(Camera::posX, Camera::posY);
        if (cpvdist(current, cameraMove)>0.25){
            cpVect amount = cpvmult(cpvnormalize_safe(cpvsub(current, cameraMove)),0.25);
            current = cpvsub(current, amount);
            
            Camera::posX = current.x;
            Camera::posY = current.y;
        } else {
        
            Camera::posX = current.x;
            Camera::posY = current.y;
        }
        
    
        
    }
 
    thinkLogicBobZombie();
    
}

//-----------------------------------------------------------------------------
// Purpose: Determine what state Bob is in and run the think settings for that state
//
//-----------------------------------------------------------------------------
void BobZombie::thinkLogicBobZombie(){
    GDebug::lastFunction = DEBUG_BOB_THINK_LOGIC;
    switch (currentTutorialState) {
            
            
        case NO_TUTORIAL:
        case OPENING_TEXT:
        case CHARACTER_COMING_IN:
        case MOVE_GHOST:
        case SELECT_SCARE_FROM_BOTTOM:
        
        case SPAWN_MORE_CHARS:
        case PLACE_SCARES:
        
            return;
        case SHOW_SCARES:
        {
            bool allScared = true;
            
            for (
            std::map<size_t,Sprite*>::iterator IT = Sprite::peopleByUID.begin();
            IT != Sprite::peopleByUID.end();
                 IT++){
        
                if (!IT->second->isScared){
                    allScared=false;
                    break;
                }
            
            
            }
            if (allScared){
            UserInterface::despawnTextBox();
        
            currentTutorialState=PLACE_SCARES;
        
            Player::mainPlayer->path.clear();
            BobZombie::waitForEffect = 180;
            }
        }
        
        case MAKE_THE_ROOM:{
      
        }
            break;
            
        case FIND_SCARED_CHARS:
            
        {
            int numberScared = 0;
            
            for (std::map<size_t, Sprite*>::iterator IT = Sprite::peopleByUID.begin();
                 IT != Sprite::peopleByUID.end();
                 IT++){

                    if(IT->second->isScared){
                        numberScared++;
                    }
                
                }
            
        
        if (numberScared != floatingVar){
            floatingVar = numberScared;
            for (std::map<size_t, Sprite*>::iterator IT = Sprite::peopleByUID.begin();
                 IT != Sprite::peopleByUID.end();
                 IT++){
                
                
                if(!IT->second->isScared){
                
                    Player::mainPlayer->findPath(IT->second->renderGridIndex);
                    
                    break;
                }
                
            }
        
        }
        
        
            if (numberScared==Sprite::peopleByUID.size()){
            
                changeTutorialState();
                UserInterface::checkCircleRadius =  -1;
                Tiles::hoverTileSelected.clear();
                Player::mainPlayer->path.clear();
                floatingVar = INT_MAX;
            
            }
            
        }
            
            
            break;
            
        case ADD_SOME_PROPS:
        {
            int counter = 0;
            
        
         for (
              std::map<size_t, Sprite*>::iterator IT = Sprite::propsByUID.begin();
              IT != Sprite::propsByUID.end();
              IT++){
         
             counter++;
         }
         
         
            if (counter>=3){
            
           
                changeTutorialState();
            }
            
            
        }
            break;

        case WAIT_TO_SELECT_MODE:
            
            if (GameLogic::gameMode==TILEMODE){
                changeTutorialState();
            }
            
            
            
            break;
        case ADD_A_DOOR:
            
            if (BobZombie::checkValue!=Door::thinkingDoors.size()){
                changeTutorialState();
            
            }
            
            
            break;
            
        case OH_NOES_AN_EXORCIST:{
            
            bool anExorcist = false;
            
            for (std::map<size_t, Sprite*>::iterator IT = Sprite::spriteByID.begin();
                 IT != Sprite::spriteByID.end();
                 IT++){
                
                
                if (IT->second->dict->index==EXORCIST_A){
                    if (IT->second->isScared){
                        
                        changeTutorialState();
                        return;
                    }
                
                    anExorcist = true;
                    
                    
                }
                
             
                
            }
            
            if (anExorcist == false){
                
                Sprite * theTeenageGirl = Sprite::addSprite(Tiles::findTile(64,2), EXORCIST_A);
                
                theTeenageGirl->setAnimationState(10);
                theTeenageGirl->maxIdleTime = INT_MAX;
                theTeenageGirl->isSelected = false;
                
            }

        }
            
            
            break;
            
        case SCARE_EXORCIST_THAT_WAS_HILARIOUS:
        {
              bool anExorcist = false;
        
            for (std::map<size_t, Sprite*>::iterator IT = Sprite::peopleByUID.begin();
                 IT != Sprite::peopleByUID.end();
                 IT++){
                
             
                
                if (IT->second->dict->index==EXORCIST_A){
                    if(IT->second->frame==IT->second->dict->frames[10]-1){
                        changeTutorialState();
                    }
                    anExorcist = true;
                }
                
                
                
                
            }
            
            if (anExorcist == false){
                int aTile = Tiles::findTile(
                                            Tiles::findTile(
                                                            Player::mainPlayer->position
                                                            ));
                Sprite * theTeenageGirl = Sprite::addSprite(aTile, EXORCIST_A);
                
                theTeenageGirl->setAnimationState(10);
                theTeenageGirl->maxIdleTime = INT_MAX;
                theTeenageGirl->isSelected = false;
                
                UserInterface::spawnTextBox(TranslationMatrix::translate("THEY_GOT_AWAY"),TEXTBOX_STAY_UP_FOREVER );
                
                
            }
            
            }
            break;
        case ITS_THE_COPS:
        
        for (std::map<size_t, Sprite*>::iterator IT = Sprite::spriteByID.begin();
             IT != Sprite::spriteByID.end();
             IT++){
            
            
            if (IT->second->dict->index==POLICE_A){
                if (IT->second->isScared){
                    
                    changeTutorialState();
                    return;
                }
                
               
                
                
            }
            
            
            
        }

        
        
        break;
        
        
        
        default:
            break;
    }


}


//-----------------------------------------------------------------------------
// Purpose: Returns if the Player is required to use Scare Energy for scare, used mostly in the tutorial
//
//-----------------------------------------------------------------------------
bool BobZombie::scaresAreFree(){

    if (currentTutorialState==NO_TUTORIAL){
        return false;
    }
    
    if (currentTutorialState<TALK_ABOUT_SCARE_TRAP){
        return true;
    }
    
    if (currentTutorialState>SCARE_EXORCIST_THAT_WAS_HILARIOUS_TEXT) {
        return true;
    }
    
    
    return false;
    
    


}

//-----------------------------------------------------------------------------
// Purpose: Returns if Sprites are able to die, used mostly during the tutorial
//
//-----------------------------------------------------------------------------
bool BobZombie::peopleCanDie(){

    switch (currentTutorialState) {
            
        case  NO_TUTORIAL:
        case SCARE_EXORCIST_THAT_WAS_HILARIOUS:
        {
            return true;
        }
      
        case     OPENING_TEXT:
        case     CHARACTER_COMING_IN:
        case     MOVE_GHOST:
        case     SELECT_SCARE_FROM_BOTTOM:
        case     SHOW_SCARES:
        case     PLACE_SCARES:
        case     SPAWN_MORE_CHARS:
        case     FIND_SCARED_CHARS:
        case     TALK_ABOUT_SCARE_TRAP:
        case     SCARE_TRAP_INTERACT:
        case     PLACE_SCARE_TRAP:
        case     TRAP_PLACED:

        
            return false;
            
            
            
        default:
            break;
    }
    
    
    
    
    return true;

}

//-----------------------------------------------------------------------------
// Purpose: Moves certain texts to the next New Line
//
//-----------------------------------------------------------------------------
void BobZombie::tutorialScroll(){
    
    
    if (theText.keys.size()==0){
        yPosMax=0;
        return;
    
    }
    
    tutorailKey++;
    
    
    if (theText.keys.size()==tutorailKey){
        tutorailKey = theText.keys.size()-1;
        
        
    }

    yPosMax = -theText.keys[tutorailKey];
    

}

//-----------------------------------------------------------------------------
// Purpose: Toggle if Bob should be up or not
//
//-----------------------------------------------------------------------------
bool BobZombie::toggleState(){
    if (currentTutorialState==NO_TUTORIAL){
        
        
        toggle = !toggle;
        
        if (toggle){
            
            if (timer<0){
                timer=0;
            }
            
        } else {
            
            if (timer>2.0){
                timer=2.0;
            }
            
        }
        
        return true;
    } else {
        
        if (toggle==false){
            
            toggle = !toggle;
            
            if (toggle){
                
                if (timer<0){
                    timer=0;
                }
                
            } else {
                
                if (timer>2.0){
                    timer=2.0;
                }
                
            }
            
            return true;
            
            
        } else {
            
            if (tutorailKey+1>=theText.keys.size()){
                
                toggle = !toggle;
                
                if (toggle){
                    
                    if (timer<0){
                        timer=0;
                    }
                    
                } else {
                    
                    if (timer>2.0){
                        timer=2.0;
                    }
                    
                }
                
                
                changeTutorialState();
                return true;
                
                
            } else {
                
                tutorialScroll();
                return false;
                
            }
            
            
            
        }
        
        
        
        
        
    }
    return false;
    
}



//-----------------------------------------------------------------------------
// Purpose: Used to stop the game when Bob's text box is up
//
//-----------------------------------------------------------------------------
bool BobZombie::canThink(){

    return !(toggle==false||timer<0);


}


//-----------------------------------------------------------------------------
// Purpose: Used to determine if the Player is able to engage in actions during the tutorial
//
//-----------------------------------------------------------------------------
bool BobZombie::bobWaitTime(){
    
    if (NO_TUTORIAL==currentTutorialState){
        return false;
        
    }

    if (waitForEffect==-1){
        return false;
    }
    
    if (waitForEffect==0){
        changeTutorialState();
        waitForEffect=-1;
        return true;
    } else {
        waitForEffect--;
        return false;
    }


};


//-----------------------------------------------------------------------------
// Purpose: Resets Bob to a neutral state
//
//-----------------------------------------------------------------------------
void BobZombie::restSlideState(){
    tutorailKey =-1;

    yPosMax=0;
    tutorialText=true;
    UserInterface::despawnTextBox();

}

//-----------------------------------------------------------------------------
// Purpose: Called when Bob when the Textbox leaves the screen, determine what should be done next
//
//-----------------------------------------------------------------------------
void BobZombie::changeTutorialState(){
    
    GDebug::lastFunction = DEBUG_BOB_CHANGE_TUTORIAL_STATE;
    
    if (NO_TUTORIAL==currentTutorialState){
        return;
    
    }

    switch (currentTutorialState) {
        case LEVEL_UP:
            currentTutorialState=NO_TUTORIAL;
            tutorailKey = false;
			restSlideState();

			currentTutorialState = NO_TUTORIAL;
			toggle = false;
            break;
 
            
        case OPENING_TEXT:
        {
            currentTutorialState = CHARACTER_COMING_IN;
            
           
            
            Sprite * theTeenageGirl = Sprite::addSprite(Tiles::findTile(64,2), TEENAGER_A);
            
            theTeenageGirl->setAnimationState(10);
            theTeenageGirl->maxIdleTime = INT_MAX;
            theTeenageGirl->isSelected = false;

          
            
            Sprite * theTeenageBoy = Sprite::addSprite(Tiles::findTile(66,2), TEENAGER_B);
            theTeenageBoy->setAnimationState(10);
            theTeenageBoy->maxIdleTime = INT_MAX;
            theTeenageBoy->isSelected = false;
   
            BobZombie::waitForEffect = 180;
            
            
            if (Tiles::mapValues[3010].occupiedSprite){
                
                Sprite::removeSprite(Tiles::mapValues[3010].occupiedSprite);
                
            }
            if (Tiles::mapValues[1993].occupiedSprite){
                
                Sprite::removeSprite(Tiles::mapValues[1993].occupiedSprite);
                
            }
            
            
            if (Tiles::mapValues[1594].occupiedSprite){
                
                Sprite::removeSprite(Tiles::mapValues[1594].occupiedSprite);
                
            }
            
            
            
        }
            break;
        case CHARACTER_COMING_IN:
            currentTutorialState = MOVE_GHOST;
            BobZombie::setText(TranslationMatrix::translate("BOB_GAME_OPENING"));
            BobZombie::toggleState();
            restSlideState();
            
            break;
        case MOVE_GHOST:{
            UserInterface::spawnTextBox(TranslationMatrix::translate("CLICK_ON_THE_END"), 90000.0,0);
            Player::showPath(Tiles::findTile(65,2));
            
            currentTutorialState = SELECT_SCARE_FROM_BOTTOM;
        }
            break;
        case SELECT_SCARE_FROM_BOTTOM:
            currentTutorialState = SHOW_SCARES;
            break;
        case SHOW_SCARES:
            currentTutorialState=PLACE_SCARES;
            break;
        case PLACE_SCARES:
            currentTutorialState = SPAWN_MORE_CHARS;
            BobZombie::setText(TranslationMatrix::translate("SCARED_THE_KIDS"));
            BobZombie::toggleState();
            restSlideState();
    
            return;
            break;
        case SPAWN_MORE_CHARS:
            
        {
            
     
            Sprite * theTeenageBoy = Sprite::addSprite(1993, TEENAGER_B);
            theTeenageBoy->setAnimationState(10);
            theTeenageBoy->maxIdleTime = INT_MAX;
            theTeenageBoy->isSelected = false;
            
            
            Sprite * theTeenageGirl = Sprite::addSprite(3010 , TEENAGER_A);
            
            
            
            theTeenageGirl->setAnimationState(10);
            theTeenageGirl->maxIdleTime = INT_MAX;
            theTeenageGirl->isSelected = false;
            
            
   

            
            Sprite * theTeenageBoyA = Sprite::addSprite(1594, TEENAGER_B);
            theTeenageBoyA->setAnimationState(10);
            theTeenageBoyA->maxIdleTime = INT_MAX;
            theTeenageBoyA->isSelected = false;
     
            
            
            currentTutorialState = FIND_SCARED_CHARS;
            
            UserInterface::spawnTextBox(
                                        TranslationMatrix::translate("USE_THE_RIGHT_MOUSE_BUTTON"), 90000.0,0);
            
         
            
        }
            break;
        case FIND_SCARED_CHARS:
        {
        
        
            BobZombie::setText(TranslationMatrix::translate("SEE_YOU_MEAT_BAGS"));
            
            BobZombie::toggleState();
            currentTutorialState = TALK_ABOUT_SCARE_TRAP;
            
            restSlideState();
        }
            break;
   
        case TALK_ABOUT_SCARE_TRAP:
        {
        
            
            //Player::mainPlayer->path.push_back(PathTile(Tiles::findTile(65,2), 0, 0, 0));
            
            if (GameLogic::energy<500){
                GameLogic::energy+=500;
            }
            
            cameraMove =cpv(-516.257887,-386.912084);
            cameraMoveOn = true;
            
            UserInterface::spawnTextBox(TranslationMatrix::translate("SELECT_A_SCARE_TRAP_FROM"), 90000.0,0);
            GameLogic::changeGameMode(TRAPMODE);
            //cameraMove=cpvzero;
            currentTutorialState = SCARE_TRAP_INTERACT;
            
        
        }
            
            
            break;
            
        case SCARE_TRAP_INTERACT:
    
            break;
            
            
        case TRAP_PLACED:
            
          
            
            UserInterface::spawnTextBox(TranslationMatrix::translate("TILE_MODE_FROM_THE_BAR"), TEXTBOX_STAY_UP_FOREVER,0);
            
            
            currentTutorialState = WAIT_TO_SELECT_MODE;
            
            break;

    
 /*
        case ALL_MODES_CLICKED_WAIT_TILL_NEXT_STEP:
        {
            BobZombie::setText(TranslationMatrix::translate("GREAT_UNLIKE_IN_YOUR_MORTAL"));
            currentTutorialState = TEXT_BUILD_FIRST_ROOM;
            restSlideState();
        }
            
            break;
        case TEXT_BUILD_FIRST_ROOM:
            
            UserInterface::spawnTextBox(TranslationMatrix::translate("TILE_MODE_FROM_THE_BAR"), TEXTBOX_STAY_UP_FOREVER,0);
            
            
            currentTutorialState = WAIT_TO_SELECT_MODE;
            
            break;
  
  */
        case WAIT_TO_SELECT_MODE:
            UserInterface::spawnTextBox(TranslationMatrix::translate("SELECT_A_TILE_FROM_BELOW"), TEXTBOX_STAY_UP_FOREVER,0);
            
            currentTutorialState = WAIT_TO_SELECT_BAR;
            
            break;
        case WAIT_TO_SELECT_BAR:
        {
              UserInterface::spawnTextBox(TranslationMatrix::translate("BY_CLICKING_AND_DRAGGING"), TEXTBOX_STAY_UP_FOREVER,0);
            currentTutorialState = MAKE_THE_ROOM;
         
        }
            break;
      
        case MAKE_THE_ROOM:
            BobZombie::setText(TranslationMatrix::translate("YAY_YOU'VE_MADE_A_ROOM"));
            BobZombie::toggleState();
     
            currentTutorialState= TALK_ABOUT_DOOR_MODE;
            restSlideState();
           
            
            break;
      
        
        case EXPLAIN_DASH_BOARD:
            currentTutorialState= SHOW_DASH_BOARD_1;
            UserInterface::spawnTextBoxBot(TranslationMatrix::translate("EXPLAIN_GHOST_ENEGRY"), 100);
            
            break;
        case   SHOW_DASH_BOARD_1:
            currentTutorialState= SHOW_DASH_BOARD_2;
            UserInterface::spawnTextBoxBot(TranslationMatrix::translate("EXPLAIN_AWARENESS"), 100);
            
            break;
        case   SHOW_DASH_BOARD_2:
            currentTutorialState= SHOW_DASH_BOARD_3;
            UserInterface::spawnTextBoxBot(TranslationMatrix::translate("EXPLAIN_NUMBER_OF_TRAPS"), 100);
            break;
        case   SHOW_DASH_BOARD_3:
            currentTutorialState= SHOW_DASH_BOARD_4;
            UserInterface::spawnTextBoxBot(TranslationMatrix::translate("EXPLAIN_ENERGY_IN_HOUSE"), 100);
            break;
        case   SHOW_DASH_BOARD_4:
            UserInterface::despawnTextBox();
            UserInterface::spawnTextBox(TranslationMatrix::translate("MAKE_TWO_SCARE_TRAPS"), TEXTBOX_STAY_UP_FOREVER,0);
            currentTutorialState = NO_TUTORIAL;
            /*
            currentTutorialState = TALK_ABOUT_DOOR_MODE;
            
            BobZombie::setText(TranslationMatrix::translate("TEXT_MAKE_DOOR_A_DOOR"));
            BobZombie::toggleState();
restSlideState();
*/
            
            ///currentTutorialState= SHOW_DASH_BOARD_1;
            
            break;
        case TALK_ABOUT_DOOR_MODE:
            currentTutorialState = CHANGE_TO_DOOR_MODE;
            
            
            break;
            
        case CHANGE_TO_DOOR_MODE:
            UserInterface::spawnTextBox(TranslationMatrix::translate("GREAT_NOW_JUST_CLICK_ON_A_WALL"), TEXTBOX_STAY_UP_FOREVER,0);
            currentTutorialState = ADD_A_DOOR;
            checkValue = Door::thinkingDoors.size();
            
            break;
  
        case ADD_A_DOOR:
        {
            BobZombie::setText(TranslationMatrix::translate("NOW_DOOR'S_DON'T_COST_ANY_ENERGY"));
            BobZombie::toggleState();
restSlideState();
            
            
            currentTutorialState = ADD_SOME_PROPS_TEXT;
        }
            break;
        case ADD_SOME_PROPS_TEXT:
        {
			UserInterface::spawnTextBox(TranslationMatrix::translate("PLACE_THREE_PROPS_AROUND"), TEXTBOX_STAY_UP_FOREVER, 0);
            currentTutorialState = ADD_SOME_PROPS;
        }
            break;
        case ADD_SOME_PROPS:
            
        {
            Sprite * theTeenageGirl = Sprite::addSprite(Tiles::findTile(64,2), EXORCIST_A);
            
            theTeenageGirl->setAnimationState(10);
            theTeenageGirl->maxIdleTime = INT_MAX;
            theTeenageGirl->isSelected = false;
            
            //cameraMove =cpv(-516.257887,-386.912084);
            //cameraMoveOn = true;
            
            BobZombie::setText(TranslationMatrix::translate("BLAST_IT'S_AN_EXORCIST"));
            
            BobZombie::toggleState();
            restSlideState();
            currentTutorialState = OH_NOES_AN_EXORCIST_TEXT;
            cameraMove = cpv(-617.438, -335.434);
            cameraMoveOn = true;
            
            
            
            
            
        }
            break;
  
 
        
        case OH_NOES_AN_EXORCIST:
        {
        
        BobZombie::setText(TranslationMatrix::translate("THAT_WAS_ABSOLUTELY_HILARIOUS"));
            
            BobZombie::toggleState();
            restSlideState();
            currentTutorialState = SCARE_EXORCIST_THAT_WAS_HILARIOUS_TEXT;
            GameLogic::changeGameMode(TILEMODE);
            
        }
            break;
        case SCARE_EXORCIST_THAT_WAS_HILARIOUS_TEXT:
        
         GameLogic::changeGameMode(PLAYMODE);
        UserInterface::spawnTextBox(TranslationMatrix::translate("SCARE_THE_EXORCIST_AGAIN"), TEXTBOX_STAY_UP_FOREVER,0);
        currentTutorialState =SCARE_EXORCIST_THAT_WAS_HILARIOUS;
        
        break;
        
        
        case OH_NOES_AN_EXORCIST_TEXT:
        {
            
            
            currentTutorialState = OH_NOES_AN_EXORCIST;
            UserInterface::spawnTextBox(TranslationMatrix::translate("SCARE_THE_EXORCIST"), TEXTBOX_STAY_UP_FOREVER,0);
            cameraMoveOn = false;
        }
        break;
        
          /*
        case SCARE_EXORCIST:
        {
        
           

        
        
        }
            */
            
            break;
        
        case SCARE_EXORCIST_THAT_WAS_HILARIOUS:
        {
            
            
                
                Sprite * theFuzz = Sprite::addSprite(Tiles::findTile(64,2), POLICE_A);
                
                theFuzz->setAnimationState(10);
                theFuzz->maxIdleTime = INT_MAX;
                theFuzz->isSelected = false;
                
                
            

            
            
        BobZombie::setText(TranslationMatrix::translate("SHE_DIES_WELL_IT'S_SORT_OF_FUNNY"));
            
            BobZombie::toggleState();
            restSlideState();

            currentTutorialState = OH_NOES_SHE_DIED_TEXT;
        
        
        }
            
            break;
        case OH_NOES_SHE_DIED_TEXT:
        
        UserInterface::spawnTextBox(TranslationMatrix::translate("THE_FUZZ"), TEXTBOX_STAY_UP_FOREVER,0);
        
        
        
        currentTutorialState = ITS_THE_COPS;
        
        break;
        
        case ITS_THE_COPS:
        
        BobZombie::setText(TranslationMatrix::translate("YOU_HAVE_TO_MUCH_AWARENESS"));
        
        currentTutorialState = THE_COP_IS_GONE_TEXT;
        BobZombie::toggleState();
        restSlideState();
       
        break;
		case THE_COP_IS_GONE_TEXT:

		{
			currentTutorialState = NO_TUTORIAL;

			GameLogic::awareness = 0;
			GameLogic::energy = ModVars::startingEnergy;

			std::vector<Sprite*> killThese;

			for (std::map<size_t, Sprite*>::iterator IT = Sprite::peopleByUID.begin();
				IT != Sprite::peopleByUID.end();
				IT++){

				killThese.push_back(IT->second);

			}

			for (std::vector<Sprite*>::iterator IT = killThese.begin();
				IT != killThese.end();
				IT++){

				Sprite::removeSprite(*IT);
			}
		}

        
        
        break;
        /*
        {
            
            currentTutorialState= SHOW_DASH_BOARD_1;
            UserInterface::spawnTextBoxBot(TranslationMatrix::translate("EXPLAIN_GHOST_ENEGRY"), 100);

            //UserInterface::spawnTextBox(TranslationMatrix::translate("MAKE_TWO_SCARE_TRAPS"), TEXTBOX_STAY_UP_FOREVER,0);
        
        
        //currentTutorialState = NO_TUTORIAL;
        
        }
            
            
            */
            break;
        
        
            
            
		case THE_DEMO_IS_OVER:
			GameLogic::changeGameMode(OPENINGMODE);
			UserInterface::menuMode = NOTHING_MODE;
			UserInterface::timer = 1;
			AudioController::playAudio("Interface_Cancel.wav");
			AudioController::playMusicForced(NORMAL);
			currentTutorialState = NO_TUTORIAL;
			BobZombie::toggle = false;
			GameLogic::timePassed = 0;

			break;
            
            
            
            break;
            
            
            
            
        default:
            return;
            break;
    }
    

    
    


};


//-----------------------------------------------------------------------------
// Purpose: Called when the Demo ends, Return the game to the main screen
//
//-----------------------------------------------------------------------------
void BobZombie::demoEnd(){
	if (currentTutorialState == THE_DEMO_IS_OVER){
		return;
	
	}
	BobZombie::setText(TranslationMatrix::translate("THE_DEMO_IS_OVER"));
	BobZombie::toggleState();
	setSprite(0);
	currentTutorialState = THE_DEMO_IS_OVER;
	restSlideState();


}



void BobZombie::updateLoading(){
    
    GLfloat start = 0;
    GLfloat ratio = ((GLfloat)Sprite::lastCount/(GLfloat)Sprite::totalCount);
    //GLfloat end = GameLogic::width*0.9*ratio;
    
    GLfloat end= GameLogic::width*0.9;
    
    
    GLfloat top=10;
    GLfloat bot=0;
    
    GLfloat topCut = (top-bot)*0.17;
    GLfloat sideCut = (end-start)*0.012;
    
    
    std::vector<GLfloat> vertex;
    
    
    //Center
    vertex.push_back(start+((end-start)/2.0));
    vertex.push_back(bot + (top-bot)/2.0);
    
    
    
    
    for (GLfloat angle = M_PI/2.0; angle>  0; angle -= M_PI/64.0){
        
        vertex.push_back(start + sideCut - (sideCut*sin(angle)));
        vertex.push_back(top-topCut+(topCut*cos(angle)));
   
        
    }
    
    
    
    for (GLfloat angle = 0; angle<  M_PI/2.0; angle += M_PI/64.0){
        
        vertex.push_back(end - sideCut + (sideCut*sin(angle)));
        vertex.push_back(top-topCut+(topCut*cos(angle)));
 
        
    }
    

    
    for (GLfloat angle = M_PI*0.5; angle<  M_PI; angle += M_PI/64.0){
        
        vertex.push_back(end-sideCut+(sideCut*sin(angle)));
        vertex.push_back(bot+topCut+(topCut*cos(angle)));
  
        
    }
    
 
    
    for (GLfloat angle = M_PI; angle<  M_PI*1.5; angle += M_PI/64.0){
        
        vertex.push_back(start+sideCut+(sideCut*sin(angle)));
        vertex.push_back(bot+topCut+(topCut*cos(angle)));

        
    }

    
     vertex.push_back(start + sideCut - (sideCut*sin(M_PI/2.0)));
     vertex.push_back(top-topCut+(topCut*cos(M_PI/2.0)));
    
        _loadingBarSize = (GLuint)vertex.size()/2;
    
    GLfloat modValue = (top-bot)*0.1;
 
    start = start + modValue;
    end = end - modValue;
    
   // modValue = (top-bot)*0.1;
    
    top= top - modValue;
    bot=bot + modValue;
    
    
    
    
    vertex.push_back(start+((end-start)/2.0));
    vertex.push_back(bot + (top-bot)/2.0);
    
    
    
    
    for (GLfloat angle = M_PI/2.0; angle>  0; angle -= M_PI/64.0){
        
        vertex.push_back(start + sideCut - (sideCut*sin(angle)));
        vertex.push_back(top-topCut+(topCut*cos(angle)));
        
        
    }
    
    
    
    for (GLfloat angle = 0; angle<  M_PI/2.0; angle += M_PI/64.0){
        
        vertex.push_back(end - sideCut + (sideCut*sin(angle)));
        vertex.push_back(top-topCut+(topCut*cos(angle)));
        
        
    }
    
    
    
    for (GLfloat angle = M_PI*0.5; angle<  M_PI; angle += M_PI/64.0){
        
        vertex.push_back(end-sideCut+(sideCut*sin(angle)));
        vertex.push_back(bot+topCut+(topCut*cos(angle)));
        
        
    }
    
    
    
    for (GLfloat angle = M_PI; angle<  M_PI*1.5; angle += M_PI/64.0){
        
        vertex.push_back(start+sideCut+(sideCut*sin(angle)));
        vertex.push_back(bot+topCut+(topCut*cos(angle)));
        
        
    }
    
    
    vertex.push_back(start + sideCut - (sideCut*sin(M_PI/2.0)));
    vertex.push_back(top-topCut+(topCut*cos(M_PI/2.0)));
    
    
    
    
    
    
    
    
    ///
    
    start = start + modValue;
    end = end - modValue;
    
    top= top - modValue;
    bot=bot + modValue;
 
    
    GLfloat amount = (end-start)/2.0;
    GLfloat center = 45.0;
    start = center - amount*ratio;
    end = center + amount*ratio;
    
    
    
    
    vertex.push_back(start+((end-start)/2.0));
    vertex.push_back(bot + (top-bot)/2.0);
    
    
    
    
    for (GLfloat angle = M_PI/2.0; angle>  0; angle -= M_PI/64.0){
        
        vertex.push_back(start + sideCut - (sideCut*sin(angle)));
        vertex.push_back(top-topCut+(topCut*cos(angle)));
        
        
    }
    
    
    
    for (GLfloat angle = 0; angle<  M_PI/2.0; angle += M_PI/64.0){
        
        vertex.push_back(end - sideCut + (sideCut*sin(angle)));
        vertex.push_back(top-topCut+(topCut*cos(angle)));
        
        
    }
    
    
    
    for (GLfloat angle = M_PI*0.5; angle<  M_PI; angle += M_PI/64.0){
        
        vertex.push_back(end-sideCut+(sideCut*sin(angle)));
        vertex.push_back(bot+topCut+(topCut*cos(angle)));
        
        
    }
    
    
    
    for (GLfloat angle = M_PI; angle<  M_PI*1.5; angle += M_PI/64.0){
        
        vertex.push_back(start+sideCut+(sideCut*sin(angle)));
        vertex.push_back(bot+topCut+(topCut*cos(angle)));
        
        
    }
    
    
    vertex.push_back(start + sideCut - (sideCut*sin(M_PI/2.0)));
    vertex.push_back(top-topCut+(topCut*cos(M_PI/2.0)));

    
    
    
   

    
    if (!_loadingBarArray){
        glGenVertexArraysCon(1, &_loadingBarArray);
    }
    glBindVertexArrayCon(_loadingBarArray);
    
    
    if(!_loadingBarBuffer){
        glGenBuffers(1, &_loadingBarBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _loadingBarBuffer);
    
    
    
    glBufferData(GL_ARRAY_BUFFER,
                 vertex.size()*sizeof(GLfloat),
                 &vertex[0],
                 GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*2, BUFFER_OFFSET(0));


    
    
    start = 0;
    
    end= GameLogic::width*0.9;
    
    
    GLfloat theWidthValue = GameLogic::width*(6.0/100.0);
    
    theText.setText("Loading...");
    theText.setMod(theWidthValue);
    theText.lineWidth = GameLogic::width*0.85;
    theText.SetCursor(end, bot-theWidthValue-theWidthValue);
    theText.updateR();
    
    if (UserInterface::gameTips.size()>0){
        
        
        int theT = -timer;
        
        if (theT%5==0 ){
            if (resetText == true){
            resetText =false;
        
   
        
        std::set<std::string>::iterator IT = UserInterface::gameTips.begin();
           
     
                
                
        std::advance(IT, rand()%UserInterface::gameTips.size());
        
        
        std::string tip = *IT;
    
        theTitle.setText(tip);
        theTitle.setMod(theWidthValue);
                ;
                
        theTitle.lineWidth = GameLogic::width*2.0 * 0.85;
        theTitle.SetCursor(0, -GameLogic::height+(GameLogic::height*0.10)+theWidthValue+theWidthValue+theWidthValue);
        theTitle.updateCNewLine();
            } else {
                theTitle.updateCNewLine();
            
            }
        } else {
            resetText = true;
            theTitle.updateCNewLine();
        
        }
    }
    
    


}


void BobZombie::renderLoading(){

    GLfloat color[] = {138.0/255.0,138.0/255.0,138.0/255.0,1.0};
    
    ShaderManager::enableSolidColorUI(color);
    glBindVertexArrayCon(_loadingBarArray);
    
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    
    glColorMask(false, false, false, false);
    
    
    glStencilFunc(GL_GEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


    
    glDrawArrays(GL_TRIANGLE_FAN, _loadingBarSize, _loadingBarSize);
 
    glColorMask(true, true, true, true);
    glStencilFunc(GL_GEQUAL, 0, 0xFF);
    
    
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, _loadingBarSize);
    glDisable(GL_STENCIL_TEST);
    

    color[0] = 230.0/255.0;
    color[1] = 23.0/255.0;
    color[2] = 197.0/255.0;
    color[3] = 1.0;
    
    
    ShaderManager::enableSolidColorUI(color);
    
    glDrawArrays(GL_TRIANGLE_FAN, _loadingBarSize*2, _loadingBarSize);
    
    
    ShaderManager::enableUIShader();
    theText.render();
    theTitle.render();
    

}

void BobZombie::update(){
    GDebug::lastFunction = DEBUG_BOB_UPDATE;
    
    switch (currentTutorialState) {

        case WAIT_TO_SELECT_MODE:
        case CHANGE_TO_DOOR_MODE:
        case ADD_SOME_PROPS:
        case OH_NOES_AN_EXORCIST:
        case    SHOW_DASH_BOARD_1:
        case    SHOW_DASH_BOARD_2:
        case    SHOW_DASH_BOARD_3:
        case    SHOW_DASH_BOARD_4:
            cursorUpdate();
            return;
            break;
        default:
            break;
    }
    
    
    if (!BobZombie::_textureArrow){
        
        _textureArrow = generateTexture(findFile("PokeArrow.png"));
    }
    
    
    
    {
    

        
        
        std::vector<GLfloat> vertex;
        
        vertex.push_back(-GameLogic::width*3.0);
        vertex.push_back(-GameLogic::height*3.0);
        
        vertex.push_back(-GameLogic::width*3.0);
        vertex.push_back(GameLogic::height*3.0);
        
        vertex.push_back(GameLogic::width*3.0);
        vertex.push_back(-GameLogic::height*3.0);
        
        vertex.push_back(GameLogic::width*3.0);
        vertex.push_back(GameLogic::height*3.0);
        
        if (!_vertexArray_BlackBox){
            glGenVertexArraysCon(1, &_vertexArray_BlackBox);
        }
        
        glBindVertexArrayCon(_vertexArray_BlackBox);
        
        if (!_vertexBuffer_BlackBox){
            glGenBuffers(1, &_vertexBuffer_BlackBox);
            
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer_BlackBox);
        
        
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(GLfloat)*vertex.size(),
                     &vertex[0],
                     GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*2, BUFFER_OFFSET(0));

    }
      
    
    
    
    
    
    
    
    GLfloat tweenValue;

    
    if (timer<-1.0){
        return;
    }
    
    
   if (BobZombie::timer<-1.0){
    
        tweenValue = (1.0)*(GameLogic::height*2);
    
    } else {
        
        double quickValue = Tweener::quadEaseBoth(BobZombie::timer, 0.0, 1.0, 2.0);
        
        tweenValue = (1.0-quickValue)*(GameLogic::height*2);
        
        if (tweenValue<0.0){
            tweenValue=0.0;
        
        }
    }
    
    
    //Background
    {
    GLfloat top     = +GameLogic::height * 0.9;
    GLfloat bottom  = -GameLogic::height * 0.9;
    
    GLfloat left    = -GameLogic::width * 0.9;
    GLfloat right   = +GameLogic::width * 0.9;
    
    GLfloat cutter = top/9.0;
    
    std::vector<GLfloat> vertex;
    
    vertex.push_back(0);
    vertex.push_back(0);
    vertex.push_back(0);
    vertex.push_back(0);
    
    
    vertex.push_back(left);
    vertex.push_back(0);
    vertex.push_back(0);
    vertex.push_back(0);
    
    
    vertex.push_back(left);
    vertex.push_back(top-cutter);
    vertex.push_back(0);
    vertex.push_back(0);
    
    for (GLfloat angle = M_PI/2.0; angle>  0; angle -= M_PI/64.0){
        
        vertex.push_back(left + cutter - (cutter*sin(angle)));
        vertex.push_back(top-cutter+(cutter*cos(angle)));
        vertex.push_back(0);
        vertex.push_back(0);
        
    }
    
    
    
    
    
    vertex.push_back(left+cutter);
    vertex.push_back(top);
    vertex.push_back(0);
    vertex.push_back(0);
    
    vertex.push_back(right-cutter);
    vertex.push_back(top);
    vertex.push_back(0);
    vertex.push_back(0);
    
    
    for (GLfloat angle = 0; angle<  M_PI/2.0; angle += M_PI/64.0){
        
        vertex.push_back(right - cutter + (cutter*sin(angle)));
        vertex.push_back(top-cutter+(cutter*cos(angle)));
        vertex.push_back(0);
        vertex.push_back(0);
        
    }
    
    
    vertex.push_back(right);
    vertex.push_back(top-cutter);
    vertex.push_back(0);
    vertex.push_back(0);
    
    
    vertex.push_back(right);
    vertex.push_back(bottom+cutter);
    vertex.push_back(0);
    vertex.push_back(0);
    
    for (GLfloat angle = M_PI*0.5; angle<  M_PI; angle += M_PI/64.0){
        
        vertex.push_back(right-cutter+(cutter*sin(angle)));
        vertex.push_back(bottom+cutter+(cutter*cos(angle)));
        vertex.push_back(0);
        vertex.push_back(0);
        
    }
    
    
    vertex.push_back(right-cutter);
    vertex.push_back(bottom);
    vertex.push_back(0);
    vertex.push_back(0);
    
    
    
    
    vertex.push_back(left+cutter);
    vertex.push_back(bottom);
    vertex.push_back(0);
    vertex.push_back(0);
    
    
    for (GLfloat angle = M_PI; angle<  M_PI*1.5; angle += M_PI/64.0){
        
        vertex.push_back(left+cutter+(cutter*sin(angle)));
        vertex.push_back(bottom+cutter+(cutter*cos(angle)));
        vertex.push_back(0);
        vertex.push_back(0);
        
    }
    
    
    
    vertex.push_back(left);
    vertex.push_back(bottom+cutter);
    vertex.push_back(0);
    vertex.push_back(0);
    
    vertex.push_back(left);
    vertex.push_back(0);
    vertex.push_back(0);
    vertex.push_back(0);
    
    
    
        for (int counter = 0;
             counter< vertex.size();
             counter++){
        
            int indexV = counter%4;
            
            if (indexV==1){
                vertex[counter]-=tweenValue;
            
            }
            

        
        
        }
        

    
    _sizeOfBob=(GLuint)vertex.size()/4;
    
        GLfloat modValue;
        
        {
        
            size_t counter = tick%60;
            

            
            if (counter>=30){
                
                
                
                modValue = (30+(30-counter))/(30.0);
                modValue =(modValue)*(cutter/2.0);
                
                
                
            } else {
                modValue = ((GLfloat)counter)/(30.0);
                modValue =(modValue)*(cutter/2.0);
                
            }

        
        
        }
        
        
   
         
         vertex.push_back(right-cutter);
         vertex.push_back(bottom+cutter+cutter-tweenValue+modValue);
         vertex.push_back(0);
         vertex.push_back(0);
         
         
         vertex.push_back(right-cutter);
         vertex.push_back(bottom+cutter-tweenValue+modValue);
         vertex.push_back(1);
         vertex.push_back(0);
         
         
         
         vertex.push_back(right);
         vertex.push_back(bottom+cutter+cutter-tweenValue+modValue);
         vertex.push_back(0);
         vertex.push_back(1);
         
         
         
         vertex.push_back(right);
         vertex.push_back(bottom+cutter-tweenValue+modValue);
         vertex.push_back(1);
         vertex.push_back(1);
         
         
    
    if (!_vertexArray_Bob){
        glGenVertexArraysCon(1, &_vertexArray_Bob);
    }
    glBindVertexArrayCon(_vertexArray_Bob);
    
    
    if(!_vertexBuffer_Bob){
        glGenBuffers(1, &_vertexBuffer_Bob);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer_Bob);
    
    
    
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat)*vertex.size(),
                 &vertex[0],
                 GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    }

    
      GLfloat heightMod = GameLogic::width/8.0;
    //Bob
    {
    if (!bodyTexture){
        bodyTexture = generateTexture(findFile("BobBody.png"));
    
    }
    
    if (!headTexture){
        headTexture = generateTexture(findFile("BobHead.png"));
    }
    
    if (!rightArmTexture){
        rightArmTexture = generateTexture(findFile("BobRightArm.png"));
    }
    
    if (!leftArmTexture){
        leftArmTexture = generateTexture(findFile("BobLeftArm.png"));
    }
    
    if (!leftHandTexture){
        leftHandTexture = generateTexture(findFile("BobLeftHand.png"));
    }
    
    if (!winkTexture){
        winkTexture = generateTexture(findFile("BobWink.png"));
    }
    
    

    
    if (!pupilTexture){
        pupilTexture = generateTexture(findFile("BobPupil.png"));
    }
    
    if (leftArmInOrOut){
    
        leftArmTween-= 1/480.0;
        
        if (leftArmTween<-90.0/120.0){
            leftArmInOrOut = false;
        
        }
    
    } else {
    
        leftArmTween+= 1/480.0;
        
        if (leftArmTween>0){
            leftArmInOrOut = true;
            
        }
    
    }
    
    if (leftHandInOrOut){
    
        leftHandTween -= 1/360.0;
        
        if (leftHandTween<-0.502){
            leftHandInOrOut = false;
        
        }
    
    
    } else {
 
    
        leftHandTween += 1/360.0;
        
        if (leftHandTween>-0.063888967){
            leftHandInOrOut =  true;
            
        }
    }
    
    if (rightHandInOrOut){
    
        rightArmTween-= 1/360.0;
        
        if (rightArmTween<-0.44){
            rightHandInOrOut = !rightHandInOrOut;
        
        }
    
    }else{
        
        rightArmTween += 1/360.0;
        
        if (rightArmTween>-0.05){
            rightHandInOrOut = !rightHandInOrOut;
        
        }
    
    }
    

    

    
    
    GLfloat leftW=LEFT_ARM_XPOS+LEFT_ARM_WIDTH,
            leftH=LEFT_ARM_YPOS+LEFT_ARM_HEIGHT,
            leftX=LEFT_ARM_XPOS,
            leftY=LEFT_ARM_YPOS;
    
        std::pair<GLfloat,GLfloat> pointX1Y1Arm = rotate(LEFT_ARM_ROT_X, LEFT_ARM_ROT_Y, leftX, leftY, (leftArmTween));
    
        std::pair<GLfloat,GLfloat> pointX1Y2Arm = rotate(LEFT_ARM_ROT_X, LEFT_ARM_ROT_Y, leftX, leftH, (leftArmTween));
    
        std::pair<GLfloat,GLfloat> pointX2Y1Arm = rotate(LEFT_ARM_ROT_X, LEFT_ARM_ROT_Y, leftW, leftY, (leftArmTween));
    
        std::pair<GLfloat,GLfloat> pointX2Y2Arm = rotate(LEFT_ARM_ROT_X, LEFT_ARM_ROT_Y, leftW, leftH, (leftArmTween));
    
    
    GLfloat handW=LEFT_HAND_XPOS+LEFT_HAND_WIDTH,
            handH=LEFT_HAND_YPOS+LEFT_HAND_HEIGHT,
            handX=LEFT_HAND_XPOS,
            handY=LEFT_HAND_YPOS;
    

    
    std::pair<GLfloat, GLfloat> newRotation = rotate(LEFT_ARM_ROT_X, LEFT_ARM_ROT_Y, LEFT_HAND_ROT_X, LEFT_HAND_ROT_Y, leftArmTween);
    
    
    
    
    
    std::pair<GLfloat,GLfloat> pointX1Y1H = rotate(LEFT_ARM_ROT_X, LEFT_ARM_ROT_Y, handX, handY, (leftArmTween));
    
    std::pair<GLfloat,GLfloat> pointX1Y2H = rotate(LEFT_ARM_ROT_X, LEFT_ARM_ROT_Y, handX, handH, (leftArmTween));
    
    std::pair<GLfloat,GLfloat> pointX2Y1H = rotate(LEFT_ARM_ROT_X, LEFT_ARM_ROT_Y, handW, handY, (leftArmTween));
    
    std::pair<GLfloat,GLfloat> pointX2Y2H = rotate(LEFT_ARM_ROT_X, LEFT_ARM_ROT_Y, handW, handH, (leftArmTween));
    
        pointX1Y1H = rotate(newRotation.first, newRotation.second, pointX1Y1H.first, pointX1Y1H.second, leftHandTween);
    
        pointX1Y2H = rotate(newRotation.first, newRotation.second, pointX1Y2H.first, pointX1Y2H.second, leftHandTween);
    
        pointX2Y1H = rotate(newRotation.first, newRotation.second, pointX2Y1H.first, pointX2Y1H.second, leftHandTween);
    
        pointX2Y2H = rotate(newRotation.first, newRotation.second, pointX2Y2H.first, pointX2Y2H.second, leftHandTween);
    
    
    
    
    
    GLfloat rightW=RIGHT_XPOS+RIGHT_WIDTH,
            rightH=RIGHT_YPOS+RIGHT_HEIGHT,
            rightX=RIGHT_XPOS,
            rightY=RIGHT_YPOS;
    
    std::pair<GLfloat,GLfloat> pointX1Y1R = rotate(RIGHT_ROT_X, RIGHT_ROT_Y, rightX, rightY, rightArmTween);
    std::pair<GLfloat,GLfloat> pointX2Y1R = rotate(RIGHT_ROT_X, RIGHT_ROT_Y, rightW, rightY, rightArmTween);
    std::pair<GLfloat,GLfloat> pointX1Y2R = rotate(RIGHT_ROT_X, RIGHT_ROT_Y, rightX, rightH, rightArmTween);
    std::pair<GLfloat,GLfloat> pointX2Y2R = rotate(RIGHT_ROT_X, RIGHT_ROT_Y, rightW, rightH, rightArmTween);
    
    
    
    
    
    
    
    GLfloat headW=HEAD_XPOS+HEAD_WIDTH,
            headH=HEAD_YPOS+HEAD_HEIGHT,
            headX=HEAD_XPOS,
            headY=HEAD_YPOS;
    
    GLfloat pupilW=PUPIL_WIDTH+PUPIL_XPOS,
            pupilH=PUPIL_HEIGHT+PUPIL_YPOS,
            pupilX=PUPIL_XPOS,
            pupilY=PUPIL_YPOS;
    
    GLfloat winkW= WINK_WIDTH+WINK_XPOS,
            winkH= WINK_HEIGHT+WINK_YPOS,
            winkX= WINK_XPOS,
            winkY= WINK_YPOS;
    
    
    
    
    
    
    
    
    if (pupilInOrOut){
    
        
        
        pupilTween += 0.25;
        if (pupilTween<25.0){
            pupilW+=pupilTween;
            pupilX+=pupilTween;
        } else {
            pupilW+=25.0;
            pupilX+=25.0;

        
        }
        
        if (pupilTween>35.0){
            pupilInOrOut = !pupilInOrOut;
            pupilTween=25.0;
        }
    
    } else {
        
        pupilTween -= 0.25;
        
        
        if (pupilTween>-15.0){
        
        pupilW+=pupilTween;
        pupilX+=pupilTween;
        
        } else {
        
            pupilW+=-15;
            pupilX+=-15;
        
        }
     
        
        if (pupilTween<-25.0){
            pupilInOrOut = !pupilInOrOut;
            pupilTween = -15;
        }
    
    
    }
    
    if (nextWink==0){
        nextWink = rand()%(60*5)+(150);
        winkTime =15;
        
        
        
        
    }
    winkTime--;
    nextWink--;
    
    
  //dumpTextureCord(int index, int frame, GLfloat * text, SpriteDict * target){
    GLfloat text[12]={0,0,0,0,0,0,0,0,0,0,0,0};
   
    
    if (theTargetSprite){

    
    Sprite::dumpTextureCord(theTargetSprite->index, -1, text,theTargetSprite);
   
   
    
    
    GLfloat xPos = heightMod/2;
    GLfloat yPos = heightMod+heightMod*0.4;
    
    int targetValue;
    
    float largestValue  = findLargestOf8Values(&text[4], &targetValue);
    
    
    text[4]/=   largestValue;
    text[4]*=   heightMod;
    text[4]+=   xPos;
    
    text[5]/=   largestValue;
    text[5]*=   heightMod;
    text[5]+=   yPos;
    
    text[6]/=   largestValue;
    text[6]*=   heightMod;
    text[6]+=   xPos;
    
    text[7]/=   largestValue;
    text[7]*=   heightMod;
    text[7]+=   yPos;
    
    text[8]/=   largestValue;
    text[8]*=   heightMod;
    text[8]+=   xPos;
    
    text[9]/=   largestValue;
    text[9]*=   heightMod;
    text[9]+=   yPos;
    
    text[10]/=   largestValue;
    text[10]*=  heightMod;
    text[10]+=  xPos;
    
    text[11]/=   largestValue;
    text[11]*=  heightMod;
    text[11]+=  yPos;
    
    
    }
    
    
    
    
    
    
    
    GLfloat vertex [] = {
    
        BODY_XPOS,              BODY_YPOS+BODY_HEIGHT,  0,  0,
        BODY_XPOS,              BODY_YPOS,              0,  1,
        BODY_XPOS+BODY_WIDTH,   BODY_YPOS+BODY_HEIGHT,  1,  0,
        BODY_XPOS+BODY_WIDTH,   BODY_YPOS,              1,  1,
        
        
        pointX1Y2Arm.first,        pointX1Y2Arm.second,       0,  0,
        pointX1Y1Arm.first,        pointX1Y1Arm.second,       0,  1,
        pointX2Y2Arm.first,        pointX2Y2Arm.second,       1,  0,
        pointX2Y1Arm.first,        pointX2Y1Arm.second,       1,  1,
        
        pointX1Y2H.first,          pointX1Y2H.second,                  0,  0,
        pointX1Y1H.first,          pointX1Y1H.second,                  0,  1,
        pointX2Y2H.first,          pointX2Y2H.second,                  1,  0,
        pointX2Y1H.first,          pointX2Y1H.second,                  1,  1,
        
        pointX1Y2R.first,       pointX1Y2R.second,                 0,  0,
        pointX1Y1R.first,       pointX1Y1R.second,                 0,  1,
        pointX2Y2R.first,       pointX2Y2R.second,                 1,  0,
        pointX2Y1R.first,       pointX2Y1R.first,                 1,  1,
        
        headX,                  headH,                  0,  0,
        headX,                  headY,                  0,  1,
        headW,                  headH,                  1,  0,
        headW,                  headY,                  1,  1,
        
        pupilX,                 pupilH,                 0,  0,
        pupilX,                 pupilY,                 0,  1,
        pupilW,                 pupilH,                 1,  0,
        pupilW,                 pupilY,                 1,  1,
        
        winkX,                  winkH,                  0,  0,
        winkX,                  winkY,                  0,  1,
        winkW,                  winkH,                  1,  0,
        winkW,                  winkY,                  1,  1,
        
        
        text[4],    text[5],        text[0],  text[1],
        text[6],    text[7],        text[0],  text[3],
        text[8],    text[9],        text[2],  text[1],
        text[10],   text[11],       text[2],  text[3]
        
    
    
    };
    
    
    //L 366 R 1125   // 746
    //B 90 H 1540    // 815
    
    if ((GameLogic::width/2.0)/746 > (2*GameLogic::height/1300)*0.70){
    
        
        GLfloat mod = (2*GameLogic::height/1300)*0.70;
        
        GLfloat leftNum = - GameLogic::width/2.0;
   
        
        
        
        
        for (int counter = 0;
             counter< sizeof(vertex)/sizeof(GLfloat)-16;
             counter++){
            
            if (counter%4<2){
                
                
                if (counter%2==0){
                    vertex[counter]-=746;
                    
                } else {
                    vertex[counter]-=815;
                    
                }
                
                
                vertex[counter]*=mod;
                
               
                
                if (counter%2==0){
                    vertex[counter]+=leftNum;
                    
                } else {
                    //  vertex[counter]-=815;
                    
                }
                
                
                
            }
            
        }

        
    
    
    } else {
        
        GLfloat mod =   (GameLogic::width/2.0)/746;
        GLfloat leftNum = - GameLogic::width/2.0;
        
        for (int counter = 0;
             counter< sizeof(vertex)/sizeof(GLfloat)-16;
             counter++){
            
            if (counter%4<2){
                
                
                if (counter%2==0){
                    vertex[counter]-=746;
                    
                } else {
                    vertex[counter]-=815;
                    
                }
                
                
                vertex[counter]*=mod;
                
                if (counter%2==0){
                    vertex[counter]+=leftNum;
                    
                } else {
                    //  vertex[counter]-=815;
                    
                }
                
            }
            
        }

    
    
    
    
    
    }
    
    


    
    for (int counter = 0 ; counter< sizeof(vertex)/sizeof(GLfloat); counter++) {
        
        
        int indexV = counter%4;
        
        if (indexV==1){
            vertex[counter]-=tweenValue;
            
        }

        
        
   
    }
    
    
    
    
    
    
    if (!_vertexArray){
        glGenVertexArraysCon(1, &_vertexArray);
    }
    glBindVertexArrayCon(_vertexArray);
    
    
    if(!_vertexBuffer){
        glGenBuffers(1, &_vertexBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    
    
    
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertex),
                 &vertex[0],
                 GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    }
    
    {
    
      
        GLfloat theWidthValue = GameLogic::width*(6.0/100.0);
        
        
        escText.setMod(theWidthValue);
        escText.lineWidth = GameLogic::width*0.85;
        
        escText.setText(TranslationMatrix::translate("PRESS_ESC_TO_SKIP"));
    
        escText.SetCursor(0, -GameLogic::height);
        escText.update();
    
    }
    
    
    
    
    {
          if (tutorialText){
          
          
          GLfloat theWidthValue = GameLogic::width*(6.0/100.0);
          
          
              theText.setMod(theWidthValue);
              theText.lineWidth = GameLogic::width*0.85;
              
              
              theText.SetCursor(0, (GameLogic::height * 0.9)-theWidthValue-theWidthValue-tweenValue);
              theText.updateLNewLine(0);
              
              traitText.setText("");
              traitText.updateL();
              theTitle.setText("");
              theTitle.updateL();
              
              //setH.setText("HAPPY");
              //setF.setText("SAD");
          
          
          
          }else {
            
            GLfloat theWidthValue = GameLogic::width*(6.0/100.0);
            
            if (theTargetSprite){
                theTitle.setText(theTargetSprite->name);
                
            } else {
                theTitle.setText("");
            }
              
              if (theTargetSprite){
              
                  if (theTargetSprite->trait!=NORMAL_CITIZEN){
                      
                      switch (theTargetSprite->trait) {
                              
                              
                          case  POLICE:
                              
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              traitText.setText(TranslationMatrix::translate("TRAIT_POLICE"));
                              
                              break;
                          case DOCTOR:
                              traitText.color[0]=0.0;
                              traitText.color[1]=1.0;
                              traitText.color[2]=0.0;
                              traitText.setText(TranslationMatrix::translate("TRAIT_DOCTOR"));
                              break;
                          case GOTH:
                              traitText.color[0]=0.0;
                              traitText.color[1]=1.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_GOTH"));
                              break;
                              
                          case EXORCIST:
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_EXORCIST"));
                              break;
                          case REPORTER:
                              
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              traitText.setText(TranslationMatrix::translate("TRAIT_REPORTER"));
                              break;
                          case SCAREDYCAT:
                              
                              
                              traitText.color[0]=0.0;
                              traitText.color[1]=1.0;
                              traitText.color[2]=0.0;
                              traitText.setText(TranslationMatrix::translate("TRAIT_SCAREDYCAT"));
                              break;
                          case COUPLE:
                              
                              
                              
                              traitText.color[0]=0.0;
                              traitText.color[1]=1.0;
                              traitText.color[2]=0.0;
                              traitText.setText(TranslationMatrix::translate("TRAIT_COUPLE"));
                              break;
                          case ZOMBIE:
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_ZOMBIE"));
                              break;
                          case GILDA:
                              
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_GILDA"));
                              break;
                          case HERO:
                              break;
                          case NINJA:
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_NINJA"));
                              break;
                          case INVESTIGATOR:
                              
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_INVESTIGATOR"));
                              break;
                          case VIVIAN:
                              
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              
                               traitText.setText(TranslationMatrix::translate("TRAIT_VIVIAN"));
                              break;
                          case GAMER:
                              
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_GAMER"));
                              break;
                              
                          case RENOVATOR:
                              
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_RENOVATOR"));
                              break;
                          case SKEPTIC:
                              
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_SKEPTIC"));
                              break;
                          case GHOSTHUNTER:
                              
                              traitText.color[0]=1.0;
                              traitText.color[1]=0.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_GHOSTHUNTER"));
                              break;
                          case IDIOT:
                              
                              traitText.color[0]=0.0;
                              traitText.color[1]=1.0;
                              traitText.color[2]=0.0;
                              
                              traitText.setText(TranslationMatrix::translate("TRAIT_IDIOT"));
                              break;
                          case BULLDOZER:
                          case MAX_TRAITS:
                          
                              break;
                              
                          default:
                              break;
                      }
                  
                  
                  }else {
                  
                      traitText.setText("");
                  
                  }
                  
              
              } else {
              
                  traitText.setText("");
              
              }
              
              
              traitText.setMod(theWidthValue*0.75);
              traitText.SetCursor(heightMod*1.4, -theWidthValue+heightMod*1.4-tweenValue);
              traitText.lineWidth = GameLogic::width*0.75;
              traitText.updateLNewLine();
      //qwert
              
            theTitle.setMod(theWidthValue);
            theTitle.lineWidth = GameLogic::width * 0.9 - heightMod*1.4;
              
            theTitle.SetCursor(heightMod*1.4, heightMod*1.4-tweenValue);
            theTitle.updateL();
            
            if (theTargetSprite){
                theText.setText(theTargetSprite->bobText);
            } else {
                
            }
            
            theText.setMod(theWidthValue);
            theText.lineWidth = GameLogic::width*0.85;
            
            theText.SetCursor(0, -tweenValue);
            theText.updateLNewLine();
            
            //setH.setText("HAPPY");
            //6setF.setText("SAD");
            
            
        }
    }
    
  
    

}

void BobZombie::render(){
    GDebug::lastFunction = DEBUG_BOB_RENDER;
    
    switch (currentTutorialState) {

        case WAIT_TO_SELECT_MODE:
        case CHANGE_TO_DOOR_MODE:
        case ADD_SOME_PROPS:
        case OH_NOES_AN_EXORCIST:
        case    SHOW_DASH_BOARD_1:
        case    SHOW_DASH_BOARD_2:
        case    SHOW_DASH_BOARD_3:
        case    SHOW_DASH_BOARD_4:
            
            cursorRender();
            return;
            break;
        default:
            break;
    }
    
  
    
    
    if (timer<-1.0){
        
        if (currentTutorialState!=NO_TUTORIAL){
        escText.render();
        
        }
        
        return;
    }
    
    
 
    if (_vertexArray_BlackBox){
        
        GLfloat color[4]={0.0,0.0,0.0,0.0};
        
        
        
        if (BobZombie::timer<-1.0){
            
            color[3]=0.0;
            
        } else if (BobZombie::timer>2.0){
            
        color[3] = 0.33;
        }else {
            
            double quickValue = Tweener::quadEaseBoth(BobZombie::timer, 0.0, 1.0, 2.0);
            
            color[3] = (quickValue)*0.33;
            
            
        }
        
        
        
     
        
        
        
        ShaderManager::enableSolidColorUI(color);
        
    glBindVertexArrayCon(_vertexArray_BlackBox);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    
    
    
    
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    
   
    
    
    glStencilFunc(GL_GEQUAL, 20, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    
    if (_vertexArray_Bob){
        
        GLfloat color[] = {0.5,0.5,0.5,0.5};
        
        ShaderManager::enableSolidColorUI(color);
        glBindVertexArrayCon(_vertexArray_Bob);
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, _sizeOfBob);
        
            
     
        
        
        
    }

    glStencilFunc(GL_LEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
;
    
    
    if (_vertexArray!=0){
        
        ShaderManager::enableUIShader();
        
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayCon(_vertexArray);
        
        glBindTexture(GL_TEXTURE_2D, *leftArmTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        
        glBindTexture(GL_TEXTURE_2D, *bodyTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
   
        
        glBindTexture(GL_TEXTURE_2D, *leftHandTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        
        glBindTexture(GL_TEXTURE_2D, *rightArmTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        
        glBindTexture(GL_TEXTURE_2D, *headTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        
        glBindTexture(GL_TEXTURE_2D, *pupilTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        
        if (winkTime<5){
        
            glBindTexture(GL_TEXTURE_2D, *winkTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
        
        }
        
 
        if (theTargetSprite){
        glBindTexture(GL_TEXTURE_2D, *theTargetSprite->texture);
        glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
        }
        
        
    }
    
    theText.renderYMove();
    ShaderManager::enableUIShader();
    theTitle.render();
    traitText.renderColorChangeUI();
    
    
    
    
    
    if (_textureArrow){
        if (_vertexArray_Bob){
            
            GLfloat color[4] = {10.0,10.0,10.0,1.0};
            
            ShaderManager::enableColorShiftUI(color);
            
            if (tutorailKey+1>=theText.keys.size()){}else{
                glBindVertexArrayCon(_vertexArray_Bob);
                glBindTexture(GL_TEXTURE_2D, *_textureArrow);
                glDrawArrays(GL_TRIANGLE_STRIP, _sizeOfBob,4);
            }
        }
    }
    glDisable(GL_STENCIL_TEST);
    
    
    if (currentTutorialState!=NO_TUTORIAL){
        escText.render();
    }
    
}

void BobZombie::setSprite(SpriteDict * target){
    
    
    
    theTargetSprite = target;
    BobZombie::tutorailKey=-1;
    tutorialText = false;
    currentTutorialState = NO_TUTORIAL;
    

}

void BobZombie::triggerTutorial(){

    Camera::posX = -567.776442;
    Camera::posY = -370.506811;
    
    Player::mainPlayer->position = cpv(510.0,387.61904761904731);
    tutorialText = true;
    theTargetSprite = 0;
 

    theTitle.setText("");
 
    BobZombie::setText(TranslationMatrix::translate("OPENING_TUTORIAL_TEXT"));
    BobZombie::toggleState();
    
    currentTutorialState = OPENING_TEXT;
    restSlideState();









}






void BobZombie::cursorUpdate(){
    
    
    if (!BobZombie::_textureArrow){
        
        _textureArrow = generateTexture(findFile("PokeArrow.png"));
    }
    
    
    
    
    
    GLfloat sOfOB = GameLogic::width*0.09;
    
    
    

    
    
   
    
    int position;
    
    
    
  
    
    switch (currentTutorialState) {
        case WAIT_TO_SELECT_MODE:
            position = 3;
            break;
            
        case CHANGE_TO_DOOR_MODE:
            position = 4;
            break;
        case ADD_SOME_PROPS:
            position = 2;
            break;
        case OH_NOES_AN_EXORCIST:
            position = 0;
            break;
            
            

         /*
        case GHOST_MODE_ARROW:
            position = 0;
            break;
        case SCARE_MODE_ARROW:
            position = 1;
            break;
        case PROPS_MODE_ARROW:
            position = 2;
            break;
        case TILES_MODE_ARROW:
            position = 3;
            break;
        case DOOR_MODE_ARROW:
            position = 4;
            break;
          */
        case SHOW_DASH_BOARD_1:
            position=6;
            break;
        case SHOW_DASH_BOARD_2:
            position=7;
            break;
        case SHOW_DASH_BOARD_3:
            position=8;
            break;
        case SHOW_DASH_BOARD_4:
            position=9;
            break;
            
        default:
            position = 0;
            break;
    }
    
   
    if (position<6){
    
    cpBB mid = UserInterface::changeModeButtons[position];
    
    GLfloat midButton = (mid.b+mid.t)/2.0;//0+sOfOB+sOfOB+sOfOB;
    
    GLfloat topArrow = midButton + sOfOB*0.5;
    GLfloat botArrow = midButton - sOfOB*0.5;
        
        GLfloat leftArrow = GameLogic::width-sOfOB-sOfOB;
        GLfloat rightArrow = GameLogic::width-sOfOB;
    
    size_t counter = tick%60;
    
    GLfloat pos;
    
    if (counter>=30){
        
        
        
        pos = (30+(30-counter))/(30.0);
        pos =(pos)*(sOfOB/3.0);
        
        
        
    } else {
        pos = ((GLfloat)counter)/(30.0);
        pos =(pos)*(sOfOB/3.0);

    }
    
    
    GLfloat vertex[] = {
        leftArrow-pos,     topArrow,                        0,0,
        leftArrow-pos,     botArrow,                        0,1,
        rightArrow-pos,    topArrow,                        1,0,
        
        
        leftArrow-pos,     botArrow,                        0,1,
        rightArrow-pos,    topArrow,                        1,0,
        rightArrow-pos,    botArrow,                        1,1
    };
    
    
    
    
    
    
    
    if (!_vertexArray_Arrow){
        glGenVertexArraysCon(1, &_vertexArray_Arrow);
    }
    glBindVertexArrayCon(_vertexArray_Arrow);
    
    
    if(!_vertexBuffer_Arrow){
        glGenBuffers(1, &_vertexBuffer_Arrow);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer_Arrow);
    
    
    
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertex),
                 &vertex[0],
                 GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    helpText.SetCursor(leftArrow-sOfOB/2.0, botArrow+sOfOB/4.0);
    helpText.setMod( GameLogic::width*(6.0/100.0));
    switch (position) {
        case 0:
            
            helpText.setText("Ghost Mode");
            helpText.updateR();
            break;
        case 1:
            
            helpText.setText("Scare Traps");
            helpText.updateR();
            break;
        case 2:
            
            helpText.setText("Prop Mode");
            helpText.updateR();
            
            break;
        case 3:
            
            helpText.setText("Tile Mode");
            helpText.updateR();
            
            break;
        case 4:
            
            helpText.setText("Door Mode");
            helpText.updateR();
            
            
            break;
        case 5:
            
            helpText.setText("Sprite Mode");
            helpText.updateR();
            break;
            
            
            
            
            
        default:
            break;
    }
    
    } else {
    
        cpBB mid;
        
        switch (position) {
            case 6:
                mid = cpBBNew(UserInterface::scareEnergy.bbL,
                              UserInterface::scareEnergy.bbB,
                              UserInterface::scareEnergy.bbR,
                              UserInterface::scareEnergy.bbT);
                break;
            case 7:
                mid = cpBBNew(UserInterface::awarenessLevel.bbL,
                              UserInterface::awarenessLevel.bbB,
                              UserInterface::awarenessLevel.bbR,
                              UserInterface::awarenessLevel.bbT);
                break;
            case 8:
                mid = cpBBNew(UserInterface::tileAmount.bbL,
                              UserInterface::tileAmount.bbB,
                              UserInterface::tileAmount.bbR,
                              UserInterface::tileAmount.bbT);
                break;

            case 9:
                mid = cpBBNew(UserInterface::trapCount.bbL,
                              UserInterface::trapCount.bbB,
                              UserInterface::trapCount.bbR,
                              UserInterface::trapCount.bbT);
                break;
                

           
                
            default:
                 mid=cpBBNew(0, 0, 0, 0);
                break;
        }
        
    
        //cpBB mid = UserInterface::changeModeButtons[position];
        
        GLfloat midButton = (mid.t+mid.b)/2.0;//(mid.b+mid.t)/2.0;//0+sOfOB+sOfOB+sOfOB;
        
        GLfloat topArrow = midButton + sOfOB*0.5;
        GLfloat botArrow = midButton - sOfOB*0.5;
        
        GLfloat leftArrow = mid.l-sOfOB;//GameLogic::width-sOfOB-sOfOB;
        GLfloat rightArrow = mid.l; //GameLogic::width-sOfOB;
        
        
        size_t counter = tick%60;
        
        GLfloat pos;
        
        if (counter>=30){
            
            
            
            pos = (30+(30-counter))/(30.0);
            pos =(pos)*(sOfOB/3.0);
            
            
            
        } else {
            pos = ((GLfloat)counter)/(30.0);
            pos =(pos)*(sOfOB/3.0);
            
        }
        
        
    
        
        GLfloat vertex[] = {
            leftArrow-pos,     topArrow,                        0,0,
            leftArrow-pos,     botArrow,                        0,1,
            rightArrow-pos,    topArrow,                        1,0,
            
            
            leftArrow-pos,     botArrow,                        0,1,
            rightArrow-pos,    topArrow,                        1,0,
            rightArrow-pos,    botArrow,                        1,1
        };
        
    
        if (!_vertexArray_Arrow){
            glGenVertexArraysCon(1, &_vertexArray_Arrow);
        }
        glBindVertexArrayCon(_vertexArray_Arrow);
        
        
        if(!_vertexBuffer_Arrow){
            glGenBuffers(1, &_vertexBuffer_Arrow);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer_Arrow);
        
        
        
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertex),
                     &vertex[0],
                     GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        helpText.SetCursor(leftArrow-sOfOB/2.0, botArrow+sOfOB/4.0);
        helpText.setMod( GameLogic::width*(6.0/100.0));
    
        switch (position) {
                case 6:
                helpText.setText("Scare Energy");
                break;
                case 7:
                helpText.setText("Awareness Level");
                break;
            case 8:
                helpText.setText("Number of Traps");
                break;
                case 9:
                helpText.setText("Energy in the House");
                break;
              
        
        
        
        }
        helpText.updateR();
    
    }
    
    
    
    
    
    
}
void BobZombie::cursorRender(){


    ShaderManager::enableUIShader();
    if (_vertexArray_Arrow){
        glBindTexture(GL_TEXTURE_2D, *_textureArrow);
        
        glBindVertexArrayCon(_vertexArray_Arrow);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
    }
    
    
    helpText.render();
    
}


bool BobZombie::mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber){

    GDebug::lastFunction = DEBUG_BOB_STATE_MOUSE;
   
    
    
    if (BobZombie::toggle){
  
       
        if (input==MOUSE_STATE_UP){
            if (timer>2.0){
                BobZombie::toggleState();
            }
        }
        
        
        
        return true;
        
    }
    
    if (NO_TUTORIAL==currentTutorialState){
        return false;
        
    }
    
    
    
    switch (currentTutorialState) {
        case SCARE_EXORCIST_THAT_WAS_HILARIOUS:
        return false;
        break;
            
        case OPENING_TEXT:
            return true;
            break;
        case CHARACTER_COMING_IN:
            return true;
            break;
        case MOVE_GHOST:
        
            
            
            return true;
            break;
            
        case SELECT_SCARE_FROM_BOTTOM:
        {
            if (theTouchNumber==MOUSE_R){
                return true;
            }
            
            if (input==MOUSE_STATE_UP){
                
                control = cpvmult(control, Camera::zoom);
                cpVect pos = cpvadd(control,cpv(-Camera::posX,-Camera::posY));
                
                int index = Tiles::findTile(Tiles::findTile(pos));
                
                
                if (index ==Tiles::findTile(65,2)){
                    Player::mainPlayer->mouseInput(input, control, theTouchNumber);
                    
                    UserInterface::spawnTextBox(
                                                TranslationMatrix::translate("SELECT_YOUR_SCARE_TYPE_FROM"), 900000.0,0);
                    currentTutorialState = SHOW_SCARES;
                    
                    
                    
                    return true;
                }
            }
            
        }
            
            return true;
            break;

    
        case PLACE_SCARE_TRAP:
        {
            
            if (theTouchNumber==MOUSE_R){
                
                //int target = UserInterface::testMouse(input, control, theTouchNumber);
                
                return true;
            }
            
            if (input==MOUSE_STATE_HOVER){
            
                Sprite::mouseInput(input, control, theTouchNumber);
            
            }
            
            
            
            if (input==MOUSE_STATE_UP){
                
                control = cpvmult(control, Camera::zoom);
                cpVect pos = cpvadd(control,cpv(-Camera::posX,-Camera::posY));
                
                int index = Tiles::findTile(Tiles::findTile(pos));
                
                
                if (index ==1857){
                    Sprite::mouseInput(input, control, theTouchNumber);
        
                    
                    currentTutorialState = TRAP_PLACED;
                    BobZombie::cameraMoveOn = false;
                    
                    BobZombie::setText(TranslationMatrix::translate("EXCELLENT_YOU'VE_PLACED_A_SCARE_TRAP"));
                    BobZombie::toggleState();
                    Player::mainPlayer->path.clear();
restSlideState();
                    
                    
                    return true;
                }
            }
            
          
        
        
        
        
        }
            break;
        case SCARE_TRAP_INTERACT:
        {
            
            if (theTouchNumber==MOUSE_R){
                
                //int target = UserInterface::testMouse(input, control, theTouchNumber);
                
                return true;
            }
            
          
            if (input==MOUSE_STATE_UP){
                int target = UserInterface::testMouse(input, control, theTouchNumber);
                if (target == 0 || target == 1 || target == 2){
                    UserInterface::mouseInput(input, control, theTouchNumber);
                    UserInterface::spawnTextBox(TranslationMatrix::translate("PLACE_THE_TRAP_INSIDE_YOUR_HOUSE"), 900000.0,0);
                    Player::mainPlayer->path.push_back(PathTile(1857, 0, 0, 0));
                    currentTutorialState = PLACE_SCARE_TRAP;
                }
            }
            
            
            
        }
            break;
            
    
            
            
            return true;
            break;
        case SHOW_SCARES:
        {
            if (input == MOUSE_STATE_UP){
                if (UserInterface::testMouse(input, control, theTouchNumber)==0){
                    
                    
                    
                    int goodIndex =Tiles::findTile(Tiles::findTile(Player::mainPlayer->position));
                    
                    if (Tiles::findTile(65,2)==goodIndex ){
                        
                        UserInterface::mouseInput(input, control, theTouchNumber);
                        UserInterface::despawnTextBox();
                        
                        currentTutorialState=PLACE_SCARES;
                        
                        Player::mainPlayer->path.clear();
                        BobZombie::waitForEffect = 180;
                    }
                }
            }
        }
            return true;
            break;
        case PLACE_SCARES:
            return false;
            break;
        
        /*
        case     GHOST_MODE_ARROW:
        case     SCARE_MODE_ARROW:
        case     PROPS_MODE_ARROW:
        case     TILES_MODE_ARROW:
        case     DOOR_MODE_ARROW:
        {
            
            
            
            if (input==MOUSE_STATE_UP){
        
            switch (UserInterface::modeSelector(input, control,theTouchNumber)) {
                case 0:
                    currentTutorialState = GHOST_MODE_ARROW;
                    UserInterface::spawnTextBox(TranslationMatrix::translate("MODE_MOVES_GHOST"), 900000.0,0);
                    hasSelected[0]=true;
                    break;
                case 1:
                    currentTutorialState = SCARE_MODE_ARROW;
                    UserInterface::spawnTextBox(::TranslationMatrix::translate("MODE_SCARE_TRAPS"), 900000.0,0);
                    hasSelected[1]=true;
                    break;
                case 2:
                    currentTutorialState = PROPS_MODE_ARROW;
                    UserInterface::spawnTextBox(TranslationMatrix::translate("MODE_PLACE_PROPS"), 900000.0,0);
                    hasSelected[2]=true;
                    break;
                case 3:
                    currentTutorialState = TILES_MODE_ARROW;
                    UserInterface::spawnTextBox(TranslationMatrix::translate("MODE_PLACE_TILE"), 900000.0,0);
                    hasSelected[3]=true;
                    break;
                case 4:
                    currentTutorialState = DOOR_MODE_ARROW;
                       UserInterface::spawnTextBox(TranslationMatrix::translate("MODE_PLACE_DOOR"), 900000.0,0);
                    hasSelected[4]=true;
                    break;
                case 5:
                    break;
                case -1:
                    break;
                    
                default:
                    break;
            }
        
            }
        
        
        }
            
            
            
            break;
         */
  
  
        case WAIT_TO_SELECT_MODE:
        {
            
            
            if (input==MOUSE_STATE_UP||input==MOUSE_STATE_HOVER){
                
                switch (UserInterface::modeSelector(input, control,theTouchNumber)) {
                    case 0:
                        
                        UserInterface::spawnTextBox(TranslationMatrix::translate("THIS_IS_NO_TILE_MODE"), TEXTBOX_STAY_UP_FOREVER,0);
                        
                        break;
                    case 1:
                        
                       UserInterface::spawnTextBox(TranslationMatrix::translate("THIS_IS_NO_TILE_MODE"), TEXTBOX_STAY_UP_FOREVER,0);
                        
                        break;
                    case 2:
                        UserInterface::spawnTextBox(TranslationMatrix::translate("THIS_IS_NO_TILE_MODE"), TEXTBOX_STAY_UP_FOREVER,0);
                        break;
                    case 3:
                        if (input==MOUSE_STATE_HOVER){
                            UserInterface::spawnTextBox(TranslationMatrix::translate("THIS_IS_THE_TILE_MODE"),  TEXTBOX_STAY_UP_FOREVER,0);
                        } else {
                            GameLogic::changeGameMode(TILEMODE);
                            changeTutorialState();
                        }
                        break;
                    case 4:
                        UserInterface::spawnTextBox(TranslationMatrix::translate("THIS_IS_NO_TILE_MODE"), TEXTBOX_STAY_UP_FOREVER,0);
                        break;
                    case 5:
                        break;
                    case -1:
                        break;
                        
                    default:
                        break;
                }
                
            }

            
            
        }
    
            
            
            break;
            
        case WAIT_TO_SELECT_BAR:
        {

            
            if (input==MOUSE_STATE_UP){
                int target = UserInterface::testMouse(input, control, theTouchNumber);
                if (target == 0 || target == 1  ||target == -1){
                    UserInterface::spawnTextBox(TranslationMatrix::translate("NOT_INSIDE_HOUSE"), TEXTBOX_STAY_UP_FOREVER,0);
                } else {
                    UserInterface::mouseInput(input, control, theTouchNumber);
                    changeTutorialState();
                    checkValue = Tiles::checkWalkAbility();
                }
            }
            
            return true;
            break;
        }
            
        case MAKE_THE_ROOM:
            
            
            if (theTouchNumber==MOUSE_R){
                return false;
            
            }
            if (input==MOUSE_STATE_UP){
                if (2== Tiles::mouseInput(input, control, theTouchNumber)){
                    changeTutorialState();
                    Tiles::startIndex=-1;
                    Tiles::lastIndex=-1;
                    
                };
            } else {
                Tiles::mouseInput(input, control, theTouchNumber);
            }
                    return true;
            break;
            
            
        case     SHOW_DASH_BOARD_1:
        case    SHOW_DASH_BOARD_2:
        case    SHOW_DASH_BOARD_3:
        case    SHOW_DASH_BOARD_4:
            
            if (input==MOUSE_STATE_UP){
                changeTutorialState();
            }
            return true;
            break;
        case CHANGE_TO_DOOR_MODE:
            
            if (input==MOUSE_STATE_UP||input==MOUSE_STATE_HOVER){
                
                switch (UserInterface::modeSelector(input, control,theTouchNumber)) {
                    case 0:
                        
                        UserInterface::spawnTextBox(TranslationMatrix::translate("NOT_DOOR_MODE"), TEXTBOX_STAY_UP_FOREVER,0);
                        
                        break;
                    case 1:
                        
                        UserInterface::spawnTextBox(TranslationMatrix::translate("NOT_DOOR_MODE"),TEXTBOX_STAY_UP_FOREVER,0);
                    
                        break;
                    case 2:
                        
                        UserInterface::spawnTextBox(TranslationMatrix::translate("NOT_DOOR_MODE"), TEXTBOX_STAY_UP_FOREVER,0);
                        break;
                        
                    
                    case 3:
                        UserInterface::spawnTextBox(TranslationMatrix::translate("NOT_DOOR_MODE"), TEXTBOX_STAY_UP_FOREVER,0);
                        break;
                    case 4:
                        if (input==MOUSE_STATE_HOVER){
                            UserInterface::spawnTextBox(TranslationMatrix::translate( "THIS_IS_DOOR_MODE"),  TEXTBOX_STAY_UP_FOREVER,0);
                        } else {
                            GameLogic::changeGameMode(DOORMODE);
                            changeTutorialState();
                        }
                       
                        break;
                    case 5:
                        break;
                    case -1:
                        break;
                        
                    default:
                        break;
                }
                
            }

            
            
            
            break;
            
        case ADD_A_DOOR:
        {
        
         Tiles::doorMouseInput(input, control, theTouchNumber);
        
        }
            
            break;
            
        case ADD_SOME_PROPS:
        {
        
            return false;
        
        
        
        }
            
        case OH_NOES_AN_EXORCIST:
        {
        
            return false;
        }
 
        case NO_TUTORIAL:
            return false;
            break;
        case SPAWN_MORE_CHARS:
            return false;
            break;
        case FIND_SCARED_CHARS:
            return false;
            break;
        case SCARE_EXORCIST_THAT_WAS_HILARIOUS_TEXT:
        return false;
        break;
        case ITS_THE_COPS:
        return false;
        default:
        printf("%i",currentTutorialState);
        
            break;
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    return true;

}


bool BobZombie::keyBoardInput(){
    switch (currentTutorialState) {
        case OPENING_TEXT:
        case CHARACTER_COMING_IN:
        case MOVE_GHOST:
        case FIND_SCARED_CHARS:
        case OH_NOES_AN_EXORCIST:
        case OH_NOES_AN_EXORCIST_TEXT:
            return true;
            break;
        case NO_TUTORIAL:
            return false;
            break;
            
        case     SELECT_SCARE_FROM_BOTTOM:
        case     SHOW_SCARES:
        

        case SPAWN_MORE_CHARS:
       case     PLACE_SCARES:

            return false;
        
        
        default:
            
            break;
    }
    return true;
}

bool BobZombie::bottomBarDraw(){
    
    switch (currentTutorialState) {
        case NO_TUTORIAL:
        default:
            return true;
        case    SHOW_DASH_BOARD_1:
        case SHOW_DASH_BOARD_2:
        case SHOW_DASH_BOARD_3:
        case SHOW_DASH_BOARD_4:
            return false;
            
    }

}
static std::string oldText;
void BobZombie::textToEnum(){

    std::string text;

    switch (currentTutorialState) {
        
        
        
        case NO_TUTORIAL:
        text = "NO_TUTORIAL";
        break;
        
        
        
        case OPENING_TEXT:
        text = "OPENING_TEXT";
        break;
        
        case CHARACTER_COMING_IN:
        text = "CHARACTER_COMING_IN";
        break;
        
        case MOVE_GHOST:
        text = "MOVE_GHOST";
        break;
        
        case SELECT_SCARE_FROM_BOTTOM:
        text = "SELECT_SCARE_FROM_BOTTOM";
        break;
        
        case SHOW_SCARES:
        text = "SHOW_SCARES";
        break;
        
        case PLACE_SCARES:
        text = "PLACE_SCARES";
        break;
        
        case SPAWN_MORE_CHARS:
        text = "SPAWN_MORE_CHARS";
        break;
        
        case FIND_SCARED_CHARS:
        text = "FIND_SCARED_CHARS";
        break;
        
        case EXPLAIN_DASH_BOARD:
        text = "EXPLAIN_DASH_BOARD";
        break;
        
        case SHOW_DASH_BOARD_1:
        text = "SHOW_DASH_BOARD_1";
        break;
        
        case SHOW_DASH_BOARD_2:
        text = "SHOW_DASH_BOARD_2";
        break;
        
        case SHOW_DASH_BOARD_3:
        text = "SHOW_DASH_BOARD_3";
        break;
        
        case SHOW_DASH_BOARD_4:
        text = "SHOW_DASH_BOARD_4";
        break;
        
        case TALK_ABOUT_DOOR_MODE:
        text = "TALK_ABOUT_DOOR_MODE";
        break;
        
        case TALK_ABOUT_SCARE_TRAP:
        text = "TALK_ABOUT_SCARE_TRAP";
        break;
        
        case SCARE_TRAP_INTERACT:
        text = "SCARE_TRAP_INTERACT";
        break;
        
        case PLACE_SCARE_TRAP:
        text = "PLACE_SCARE_TRAP";
        break;
        
        case TRAP_PLACED:
        text = "TRAP_PLACED";
        break;
        
        case WAIT_TO_SELECT_MODE:
        text = "WAIT_TO_SELECT_MODE";
        break;
        case WAIT_TO_SELECT_BAR:
        text = "WAIT_TO_SELECT_BAR";
        break;
        case MAKE_THE_ROOM:
        text = "MAKE_THE_ROOM";
        break;
        case CHANGE_TO_DOOR_MODE:
        text = "CHANGE_TO_DOOR_MODE";
        break;
        
        
        case ADD_A_DOOR:
        text = "ADD_A_DOOR";
        break;
        
        case ADD_SOME_PROPS_TEXT:
        text = "ADD_SOME_PROPS_TEXT";
        break;
        
        case  ADD_SOME_PROPS:
        text = "ADD_SOME_PROPS";
        break;
        
        case  OH_NOES_AN_EXORCIST:
        text = "OH_NOES_AN_EXORCIST";
        break;
        
        case  OH_NOES_AN_EXORCIST_TEXT:
        text = "OH_NOES_AN_EXORCIST_TEXT";
        break;
       
        case  SCARE_EXORCIST_THAT_WAS_HILARIOUS_TEXT:
        text = "SCARE_EXORCIST_THAT_WAS_HILARIOUS_TEXT";
        break;
        
        case  SCARE_EXORCIST_THAT_WAS_HILARIOUS:
        text = "SCARE_EXORCIST_THAT_WAS_HILARIOUS";
        break;
        
        case  OH_NOES_SHE_DIED_TEXT:
        text = "OH_NOES_SHE_DIED_TEXT";
        break;
        
        case ITS_THE_COPS:
        text = "ITS_THE_COPS";
        break;
        
        case THE_COP_IS_GONE_TEXT:
        text = "THE_COP_IS_GONE_TEXT";
        break;
        
        case EXTRA:
        text = "EXTRA--Should Not Run";
        break;
        case LEVEL_UP:
            text = "Level up";
            break;
        
    }
    
    if (oldText !=text){
        oldText=text;
        
        text = text.append("\n");
        
        printf("%s",text.c_str());
        
    }
    
    
}




int BobZombie::dashBoardDraw(){
    
 
    if (currentTutorialState==NO_TUTORIAL){
        return 10;
    }
    
    
    if (currentTutorialState>=ADD_SOME_PROPS){
        return 10;
    }
    
    if (currentTutorialState>=TALK_ABOUT_DOOR_MODE){
        return 4;
    }
    
    if (currentTutorialState>=TALK_ABOUT_SCARE_TRAP){
    
        return 3;
    }
    
    
    if (currentTutorialState>=FIND_SCARED_CHARS){
    
        return 2;
    }
   
    return 0;
    
    
    
    
    
    
    
    
    
    return true;


};

bool BobZombie::peopleCanTalk(){
    if (currentTutorialState==NO_TUTORIAL){
        return true;
    } else {
        return false;
    }


}


int BobZombie::sideSelector(){

    switch (currentTutorialState) {
        case NO_TUTORIAL:
        case LEVEL_UP:
            

        case WAIT_TO_SELECT_MODE:
            

        case MAKE_THE_ROOM:
            
       
            
        case ADD_A_DOOR:
        case ADD_SOME_PROPS_TEXT:
        case ADD_SOME_PROPS:
            
        case OH_NOES_AN_EXORCIST:
        case OH_NOES_AN_EXORCIST_TEXT:

        case SCARE_EXORCIST_THAT_WAS_HILARIOUS_TEXT:
        case SCARE_EXORCIST_THAT_WAS_HILARIOUS:
        case OH_NOES_SHE_DIED_TEXT:
 
        case WAIT_TO_SELECT_BAR:
        case CHANGE_TO_DOOR_MODE:
        case    SCARE_TRAP_INTERACT:    
        case ITS_THE_COPS:
        case THE_COP_IS_GONE_TEXT:
		case THE_DEMO_IS_OVER:
            return 0;
            break;

            return 2;
            

        case    OPENING_TEXT:
        case   CHARACTER_COMING_IN:
        case    MOVE_GHOST:
        case   SELECT_SCARE_FROM_BOTTOM:
        case    SHOW_SCARES:
        case    SPAWN_MORE_CHARS:
        case    FIND_SCARED_CHARS:
        case   TALK_ABOUT_SCARE_TRAP:
        
        case    PLACE_SCARE_TRAP:
        case    TRAP_PLACED:

        case PLACE_SCARES:
        case    SHOW_DASH_BOARD_1:
        case    SHOW_DASH_BOARD_2:
        case    SHOW_DASH_BOARD_3:
        case    SHOW_DASH_BOARD_4:
        case    EXPLAIN_DASH_BOARD:
        case TALK_ABOUT_DOOR_MODE:
            
            
            
            
   
            
            
            
            return 1;
            
        default:
            printf("This Enumerator was not defined bzm");
            break;
    }
    
    
 

    return 0;

}

bool BobZombie::spawnNewCitizens(){
    
    switch (currentTutorialState) {
        case NO_TUTORIAL:
            return true;
            break;
            
        default:
            return false;
            break;
    }


}

bool BobZombie::checkEvent(SpriteDict * trait){

    bool aValue = false;
    
    if (checkEvent(trait->trait)){
        if (GameLogic::toggleBob){
            BobZombie::setSprite(trait);
            BobZombie::toggleState();
        }
        
    }
    return aValue;
}

bool BobZombie::checkEvent(int event){
    
    while (bobEventTest.size()<=event){
        bobEventTest.push_back(0);
    }
    
    if (bobEventTest[event]!=0){
        return false;
    } else {
        bobEventTest[event]=1;
    }
    
    
    return true;
}

unsigned long BobZombie::returnState(){
    
    std::bitset<sizeof(unsigned long)*8> bobEventData;
    bobEventData.reset();
    
    while (bobEventTest.size()<sizeof(unsigned long)*8){
        bobEventTest.push_back(0);
    
    }

    for (size_t index =0;
         index <sizeof(unsigned long)*8||index < bobEventTest.size();
         index++){
    
        
            bobEventData[index]=(bobEventTest[index])?true:false;
    
    }
    
   
    

    return bobEventData.to_ulong();
    
    
    
}
void BobZombie::setStates(unsigned long states){
 
    std::bitset<sizeof(unsigned long)*8> bobEventData{states};
   
    for (size_t counter =0; counter<bobEventData.size(); counter++){
    
        if (bobEventTest.size()<=counter){
            bobEventTest.push_back(0);
        }
        
        
        bobEventTest[counter] = bobEventData[counter];
    
    }
   
}





