//
//  UserInterface.cpp
//  After Life Empire
//
//  Created by Autobotika on 2015-01-13.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#include "UserInterface.h"

#include "GameLogic.h"
#include "LazyVars.h"
#include "ShaderManager.h"
#include <vector>
#include "TextureGenerator.h"
#include "FileManager.h"
#include "Tiles.h"
#include "Sprite.h"
#include "Tweener.h"
#include "GlobalDebugger.h"
#include "Player.h"
#include <algorithm>    // std::min_element, std::max_element
#include "ModManager.h"
#include "Player.h"
#include "BitmapFontClass.h"
#include "BobZombie.h"
#include "AudioController.h"
#include "GameClock.h"
#include "Camera.h"
#include "Window_OSX_Linux_fix.h"

GLuint UserInterface::_vertexArray=0;
GLuint UserInterface::_vertexBuffer=0;

GLuint UserInterface::_vertexArrayUserBar=0;
GLuint UserInterface::_vertexBufferUserBar=0;

GLuint UserInterface::_vertexArrayOption = 0;
GLuint UserInterface::_vertexBufferOption = 0;

GLuint UserInterface::_vertexArraySaveGame=0;
GLuint UserInterface::_vertexBufferSaveGame=0;




GLuint UserInterface::_vertexArrayChangeUI = 0;
GLuint UserInterface::_vertexBufferChangeUI = 0;

static GLuint _vertexArrayLoadLine =0;
static GLuint _vertexBufferLoadLone = 0;


cpBB UserInterface::changeModeButtons[8];


static GLuint numberOfTilesMiddle;


size_t index_value_mod = 0;

int UserInterface::timer = 0;




static GLuint * optionButtonTexture = 0;
static GLuint * backButtonTexture = 0;
static GLuint * tileButtonTexture = 0;
static GLuint * doorButtonTexture = 0;
static GLuint * spriteButtonTexture = 0;

static GLuint * ghostButtonTexture = 0;
static GLuint * propButtonTexture = 0;
static GLuint * trapButtonTexture = 0;


static GLuint * triangleBacking = 0;
static GLuint * textureBorder = 0;
static GLuint * textureBottomBorder;

static GLuint * middleIcons = 0;
static GLuint * endIcons = 0;
static GLuint * barTop =0;

static GLuint * backgroundTexture=0;
static GLuint * ghostLoadScreen = 0;
static GLuint * creditsButton = 0;

static GLuint * eye1 =0;
static GLuint * eye2 =0;
static GLuint * eye3 =0;
static GLuint * eye4 =0;

static GLuint * playButton;
static GLuint * deleteButton;
static GLuint * modStartButton;
static GLuint * moveSpriteButton;

static GLuint _playDeleteButtonArray = 0;
static GLuint _playDeleteButtonBuffer = 0;



static cpBB playButtonLocation;
static cpBB deleteButtonLocation;
static cpBB modButtonLocation;

static GLuint * gameOverTexture = 0;
static GLuint _vertexArrayGameOver = 0;
static GLuint _vertexBufferGameOver = 0;


static GLuint _vertexArrayOpenScreen;
static GLuint _vertexBufferOpenScreen;






static std::vector<ModElement*> theCurrentMods;


static GLuint * freePlay =0;
static GLuint * scenerio = 0;
static GLuint * mods = 0;
static GLuint * newGame = 0;

static cpBB theBarArea;

int UserInterface::currentSlotRunning=-1;
SaveObject * UserInterface::loadedSaves[3]={0,0,0};





static std::vector<CBitmapFont*> costs;


int UserInterface::uiOffset=0;;


cpBB UserInterface::optionButton;

static cpBB gameA;
static cpBB gameB;
static cpBB gameC;

static cpBB removeTheText;

static cpBB modButton[10];

static cpBB _saveGameButton;
static cpBB _noSaveGameButton;

static GLuint * saveGameTexture;
static GLuint * noSaveGameTexture;


static int highLightSaveGame = -1;



GLuint * UserInterface::arrowLeft=0;
GLuint * UserInterface::arrowRight=0;


GLuint * UserInterface::thumbNailTextures[3] = {0,0,0};



static CBitmapFont speedDisplay;



CBitmapFont UserInterface::scareEnergy;
CBitmapFont UserInterface::awarenessLevel;
CBitmapFont UserInterface::tileAmount;
CBitmapFont UserInterface::trapCount;



CBitmapFont toggleHints;
CBitmapFont toggleFinderUI;
CBitmapFont toggleBob;

cpBB toggleHintsBB;
cpBB toggleFinderUIBB;
cpBB toggleBobBB;

CBitmapFont UserInterface::warning;


static GLuint * uiIcons[4] = {0,0,0,0};

static GLuint _vertexArrayDashboard =0;
static GLuint _vertexBufferDashboard = 0;

SpecialDisplayMode UserInterface::menuMode = NOTHING_MODE;





SpeechBox modName;
SpeechBox modText;

int UserInterface::checkCircleRadius=-1;

std::vector<SpeechBox> UserInterface::dialog;


int UserInterface::previousGamemode=999999;

static std::vector<cpBB> buttons;
static std::vector<DoorDict*> doorButtonDict;
static std::vector<TileDict*> tileButtonDict;
static std::vector<SpriteDict*> spriteButtonDict;
static std::vector<SpriteDict*> propButtonDict;
static std::vector<PlayIcons> playButtonDict;


GLuint * SpeechBox::_texture = 0;


std::set<std::string> UserInterface::gameTips;


void confineHitbox(GLfloat * vertex, GLfloat top, GLfloat bot){

    
    GLfloat height = top-bot;
    GLfloat spriteHeight = vertex[5]-vertex[7];
 
    GLfloat sizeRatio = height/spriteHeight;
    
    
    for (int counter =0; counter<8; counter++){
        vertex[counter+4]*=sizeRatio;
    
    
    }

}


SpeechBox::SpeechBox(){
    talkBox=0;
    topBot=false;
}

SpeechBox::SpeechBox(std::string textI, double time){
    text=textI;
    despawnTime=time;
    currentTime=-1.0;
    _vertexArray = 0;
    _vertexBuffer = 0;
    talkBox =0;
	topBot = false;
    

}
SpeechBox::~SpeechBox(){}


void SpeechBox::update(){
    
    float moveValue;
    
    if (currentTime<0){
        
        moveValue = Tweener::cubicEaseOut(currentTime+1, -35, 35, 1.0);
        
        
    } else if (currentTime>despawnTime){
        
        
        moveValue = Tweener::cubicEaseIn(currentTime-despawnTime, -35, 35, 1.0);
        moveValue+=35.0;
        moveValue*=-1;
        
        
        
    } else {
        
        moveValue =0;
        
    }
    // UGLY HACK
    moveValue = 0;
    
    if (!_texture){
        _texture = generateTexture(findFile("overlayForText.png"));
    }
    
    
    
    GLfloat sOfOB = (GLfloat)GameLogic::width*0.18;
    GLfloat spacer = (GLfloat)sOfOB / 5.0;
    
    //optionButton=cpBBNew(leftTouch, topTouch-sOfOB, leftTouch+sOfOB, topTouch);
    
    
    GLfloat
    left = (GLfloat)(-GameLogic::width + sOfOB + spacer),
    right = (GLfloat)(GameLogic::width - sOfOB - spacer),
    top = (GLfloat)(GameLogic::height + sOfOB - moveValue - sOfOB - spacer),
    bottom = (GLfloat)(GameLogic::height - moveValue - sOfOB - spacer);
    
    if (topBot){
    
        left = (GLfloat)(-GameLogic::width + sOfOB + spacer);
        right = (GLfloat)(GameLogic::width - sOfOB - spacer);
        top = (GLfloat)(-GameLogic::height - sOfOB + moveValue + sOfOB + spacer);
        bottom = (GLfloat)(-GameLogic::height + moveValue + sOfOB + spacer);
        textBox.SetCursor(left+spacer+spacer, bottom-spacer-spacer);
       
    }else {
        left = (GLfloat)(-GameLogic::width + sOfOB + spacer);
        right = (GLfloat)(GameLogic::width - sOfOB - spacer);
        top = (GLfloat)(GameLogic::height + sOfOB - moveValue - sOfOB - spacer);
        bottom = (GLfloat)(GameLogic::height - moveValue - sOfOB - spacer);
        textBox.SetCursor(left+spacer+spacer, top-spacer-spacer);
    
    }
    
    textBox.setText(text);
    GLfloat theWidthValue = (GLfloat)(GameLogic::width*(3.0 / 100.0));
    textBox.setMod(theWidthValue);
    
    
    textBox.updateL();
    
    
    //textBox.textWidth= right-left-spacer-spacer;
    // textBox.textUpdateAlpha(left+spacer, top-spacer, 0.25, 1.0);
    
    
    if (!talkBox){
		GLfloat stepValue = (top - bottom) / 5.0;

        GLfloat gBoxVertexData[]={
            left,   top,        0,      0,
            left,   bottom,     0,      1.0,
            right,  top,        1.0,    0,
            right,  bottom,     1.0,    1.0,

			right - stepValue, top, 0, 0,
			right - stepValue, top - stepValue, 0, 1.00,
			right, top, 1.0, 0,
			right, top - stepValue, 1.0, 1.0,

			right - stepValue, top, 0, 0,
			right - stepValue, top - stepValue, 0, 1.00,
			right, top, 1.0, 0,
			right, top - stepValue, 1.0, 1.0
            
        };
		removeTheText = cpBBNew(right - stepValue, top - stepValue, right, top);
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
        
        glBindVertexArrayCon(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        
    } else {
        
        SpriteDict * theDict = (SpriteDict*)talkBox;
        
        GLfloat texture [20];
        
        Sprite::dumpTextureCord(theDict->index, theDict->frames[3], texture, theDict);
        
        int index = 0;
        float largestValue  = findLargestOf8Values(&texture[4], &index);
        
        
        
        
        //float largestValue = *largestValuePointer;
        
        
        
        texture[4]/=largestValue;
        texture[5]/=largestValue;
        texture[6]/=largestValue;
        texture[7]/=largestValue;
        texture[8]/=largestValue;
        texture[9]/=largestValue;
        texture[10]/=largestValue;
        texture[11]/=largestValue;
        
        
        
        
        left = (GLfloat)(-GameLogic::width + sOfOB + spacer),
        right = (GLfloat)(GameLogic::width - sOfOB - spacer),
        top = (GLfloat)(GameLogic::height + sOfOB - moveValue - sOfOB - spacer),
        bottom = (GLfloat)(GameLogic::height - moveValue - sOfOB - spacer);
        
        GLfloat stepValue = (top-bottom)/5.0;
        
        float YStep = (GLfloat)((top + bottom) / 2.0);
        float XStep = (GLfloat)(right - ((top - bottom) / 2.0));
        
        
        
        GLfloat sV = (top-bottom)*0.40;
        
        removeTheText = cpBBNew(right-stepValue, top-stepValue, right, top);
        
        GLfloat gBoxVertexData[]={
            left,   top,        0,      0,
            left,   bottom,     0,      1.0,
            right,  top,        1.0,    0,
            right,  bottom,     1.0,    1.0,
            
            texture[4]*sV+XStep, texture[5]*sV+YStep, texture[0], texture[1],
            texture[6]*sV+XStep, texture[7]*sV+YStep,texture[0],texture[3],
            texture[8]*sV+XStep,texture[9]*sV+YStep,texture[2],texture[1],
            texture[10]*sV+XStep,texture[11]*sV+YStep,texture[2],texture[3],
            
            right-stepValue,    top,            0,      0,
            right-stepValue,    top-stepValue,  0,      1.00,
            right,              top,            1.0,    0,
            right,              top-stepValue,  1.0,    1.0,
            
            
            
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
        
        glBindVertexArrayCon(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        
        
        
    }
    
}


void SpeechBox::render(){
    

    if (_vertexArray!=0){
        if (!talkBox){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *_texture);
            glBindVertexArrayCon(_vertexArray);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            glBindTexture(GL_TEXTURE_2D, *deleteButton);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            
        } else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *_texture);
            glBindVertexArrayCon(_vertexArray);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            glBindTexture(GL_TEXTURE_2D, *deleteButton);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            
            SpriteDict * theDict = (SpriteDict*)talkBox;
            glBindTexture(GL_TEXTURE_2D, *theDict->texture);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        
        
        }
    }
  
}

void SpeechBox::renderText(){
    textBox.render();;

}


bool SpeechBox::tick(){
    
    currentTime+=1.0/60.0;
    
    if (currentTime>despawnTime+1.0){
    
        //Ugly Hack
        //return true;
    }
    return false;

    
    
}


void UserInterface::changeMenuMode(SpecialDisplayMode _newMode){

    menuMode=_newMode;

    

}


void UserInterface::dialogThink(){
    if (dialog.size()>0){
        
        if (dialog.front().tick()){
            dialog.erase(dialog.begin());
        }
    }

   
}

void UserInterface::dialogUpdate(){

    if (dialog.size()>0){
        dialog.front().update();
    }
}
void UserInterface::dialogRender(){
    if (dialog.size()>0){
        dialog.front().render();
        dialog.front().renderText();
    }
    
    
    
}


void UserInterface::spawnTextBoxBot(std::string text, double timeDisplayed){

    if (text ==""){
        if (dialog.size()==0){
            return;
        }
        if (dialog[0].despawnTime<1.0){
            return;
        }
        
    }
    
    if (dialog.size()==0){
        dialog.push_back(SpeechBox(text,timeDisplayed));
        dialog[0].topBot=true;
    } else {
        dialog[0].text=text;
        dialog[0].despawnTime=timeDisplayed;
        dialog[0].talkBox=0;
        dialog[0].topBot=true;
        dialog[0].talkBox=0;
    }

    
    
}

void UserInterface::spawnTextBox(std::string text, double timeDisplayed){
    
    if (text ==""){
        if (dialog.size()==0){
            return;
        }
        if (dialog[0].despawnTime<1.0){
            return;
        }
    
    }
    
    if (dialog.size()==0){
        dialog.push_back(SpeechBox(text,timeDisplayed));
        dialog[0].topBot=false;
    } else {
        dialog[0].text=text;
        dialog[0].despawnTime=timeDisplayed;
        dialog[0].talkBox=0;
        dialog[0].topBot=false;
        
    
    }
}

void UserInterface::spawnTextBoxConditional(std::string text, double timeDisplayed){

    if (dialog.size()==0){
        dialog.push_back(SpeechBox(text,timeDisplayed));
        
    } else {
        if (dialog[0].despawnTime==0){
        dialog[0].text=text;
        dialog[0].despawnTime+=timeDisplayed;
        dialog[0].topBot=false;
        dialog[0].talkBox=0;
        }
    }
    
    

}

void UserInterface::spawnTextBox(std::string text, double timeDisplayed, void * spriteDict){
    
    if (dialog.size()==0){
        dialog.push_back(SpeechBox(text,timeDisplayed));
        dialog[0].talkBox=spriteDict;
    } else {
        dialog[0].text=text;
        dialog[0].despawnTime+=timeDisplayed;
        dialog[0].talkBox=spriteDict;
        dialog[0].topBot=false;
        
    }
}

void UserInterface::despawnTextBox(){

    for (size_t counter =0; counter<dialog.size(); counter++) {
        dialog[counter].despawnTime=0;
        dialog[counter].text = "";
       
    }

    dialog.clear();
}


void UserInterface::slideLeft(){
    
    
    int level = GameLogic::getCurrentLevel();

    if (GameLogic::gameMode==SPRITEMODE){
        if (uiOffset==0){
            std::map<int, SpriteDict*>::reverse_iterator IT = Sprite::theSpriteAssets.rbegin();
            int counter = 0;
            while  (counter<numberOfTilesMiddle-1){
                if (IT->second->prop){
                    IT++;
                } else {
                    if (IT->second->repLevelRequired<=level){
                    IT++;
                    counter++;
                    } else {
                        IT++;
                    }
                    
                }
                
                if (Sprite::theSpriteAssets.rend()==IT){
                    return;
                }
            }
            uiOffset = IT->second->index;
            return;
        }
        
        
        
        std::map<int, SpriteDict*>::iterator IT;
        
        // Is there a Sprite at the start Index
        IT = Sprite::theSpriteAssets.find(uiOffset);
        
        // If not then grab the next Sprite on the lists
        if (IT == Sprite::theSpriteAssets.end()){
            IT = Sprite::theSpriteAssets.upper_bound(uiOffset);
            if (IT == Sprite::theSpriteAssets.end()){
                IT = Sprite::theSpriteAssets.lower_bound(uiOffset);
            }
        }
        
        // Is the Sprite the correct type keep searching
        
        
        std::map<int, SpriteDict*>::reverse_iterator RIT(IT);
        
        int counter = 0;
        while (RIT!=Sprite::theSpriteAssets.rend()) {
            
            if (RIT->second->index<0){
                break;
            }
            if (RIT->second->prop){
            
            
                
            } else {
                if (IT->second->repLevelRequired<=level){
                counter++;
                
                if (counter==numberOfTilesMiddle){
                    uiOffset=RIT->second->index;
                    return;
                }
                }
            }
            RIT++;
        }
    
        
        uiOffset=0;
        
        
        
        
        
        
        
        
    } else if (GameLogic::gameMode==TILEMODE){
        
        
        if (uiOffset==0){
            
            std::map<int, TileDict*>::reverse_iterator IT = Tiles::tileDictionary.rbegin();
            int counter = 0;
            
            while  (counter<numberOfTilesMiddle-1){
                
                if (Tiles::tileDictionary.rend()==IT){
                    uiOffset=0;
                    return;
                }
                if (IT->second->requiredLevel<=level){
                    counter++;
                }
                IT++;
                
            }
            uiOffset=IT->second->index;
            
            return;
            
        }
        
        std::map<int,TileDict*>::iterator IT = Tiles::tileDictionary.upper_bound(uiOffset);
        

        std::map<int,TileDict*>::reverse_iterator RIT(IT);
        int counter=0;
        while (RIT!=Tiles::tileDictionary.rend()) {
            RIT++;
            
            if (RIT->second->requiredLevel<=level){
                counter++;
            }
            if (counter==numberOfTilesMiddle){
                uiOffset=RIT->second->index;
                return;
            }
        }
        
        
        uiOffset=0;
        
        
    } else if (GameLogic::gameMode==PROPMODE){
        
        
        int theLevel = GameLogic::getCurrentLevel();
        
        if (uiOffset==0){
           uiOffset = Sprite::theSpriteAssets.rbegin()->second->index;
        } else {
            uiOffset=propButtonDict.front()->index;
        }
        
        std::map<int, SpriteDict*>::iterator IT = Sprite::theSpriteAssets.find(uiOffset);
        std::map<int, SpriteDict*>::reverse_iterator RIT = std::map<int, SpriteDict*>::reverse_iterator(IT);
        
        
        int counter =0;
    
        while (RIT !=Sprite::theSpriteAssets.rend()) {
            if (!RIT->second->prop){
                
            } else {
                if (!RIT->second->animates){
                    if (theLevel>=RIT->second->repLevelRequired){
                        uiOffset=RIT->second->index;
                        counter++;
                        if (counter>=numberOfTilesMiddle){
                            return;
                        
                        }
                    }
                }
               
            }
             RIT++;
        }
        
        
        uiOffset=0;
        
        
        return;
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
      
        
        
    } else if (GameLogic::gameMode==TRAPMODE){
        int theLevel = GameLogic::getCurrentLevel();
        
        if (uiOffset==0){
            uiOffset = Sprite::theSpriteAssets.rbegin()->second->index;
        } else {
            uiOffset=propButtonDict.front()->index;
        }
        
        std::map<int, SpriteDict*>::iterator IT = Sprite::theSpriteAssets.find(uiOffset);
        std::map<int, SpriteDict*>::reverse_iterator RIT = std::map<int, SpriteDict*>::reverse_iterator(IT);
        
        
        int counter =0;
        
        while (RIT !=Sprite::theSpriteAssets.rend()) {
            if (!RIT->second->prop){
                
            } else {
                if (RIT->second->animates){
                    if (theLevel>=RIT->second->repLevelRequired){
                        uiOffset=RIT->second->index;
                        counter++;
                        if (counter>=numberOfTilesMiddle){
                            return;
                            
                        }
                    }
                }
                
            }
            RIT++;
        }
        
        
        uiOffset=0;
        
        
        return;
        
        
        
        
        
    } else if (GameLogic::gameMode==DOORMODE){
       
        if (uiOffset==0){
            std::map<int, DoorDict*>::reverse_iterator IT = Door::theDoorAssets.rbegin();
            int counter = 0;
            while  (counter<numberOfTilesMiddle-1){
                IT++;
                if (Door::theDoorAssets.rend()==IT){
                    return;
                }
                counter++;
                
            }
            uiOffset=IT->second->index;
            return;
            
        }
        
        std::map<int, DoorDict*>::iterator IT = Door::theDoorAssets.upper_bound(uiOffset);
        
        
        std::map<int, DoorDict*>::reverse_iterator RIT(IT);
        int counter=0;
        while (RIT!=Door::theDoorAssets.rend()) {
            RIT++;
            counter++;
            if (counter==numberOfTilesMiddle){
                uiOffset=RIT->second->index;
                return;
            }
        }
        
        
        uiOffset=0;

        
        
        
        
    }
}

void UserInterface::slideRight(){
    
    if (GameLogic::gameMode==SPRITEMODE){
        if (spriteButtonDict.size()==0){
            uiOffset=0;
            return;
        }
        uiOffset=spriteButtonDict.back()->index;
        
        std::map<int, SpriteDict*>::iterator IT = Sprite::theSpriteAssets.upper_bound(uiOffset);
        while (IT !=Sprite::theSpriteAssets.end()) {
        
            if (IT->second->prop){
            
            } else {
                uiOffset=IT->second->index;
                return;
            }
            IT++;
        }
        
        uiOffset=0;
        
        
        
        
        
    } else if (GameLogic::gameMode==TILEMODE){
        if (tileButtonDict.size()==0){
            uiOffset=0;
            return;
        }
        uiOffset=tileButtonDict.back()->index;
        
        if (Tiles::tileDictionary.upper_bound(uiOffset)==Tiles::tileDictionary.end()){
            uiOffset=0;
        }
        
        
        
        
    } else if (GameLogic::gameMode==PROPMODE){
        
        if (propButtonDict.size()==0){
            uiOffset=0;
            return;
        }
        uiOffset=propButtonDict.back()->index;
        
        std::map<int, SpriteDict*>::iterator IT = Sprite::theSpriteAssets.upper_bound(uiOffset);
        while (IT !=Sprite::theSpriteAssets.end()) {
            if (!IT->second->prop){
                
            } else {
                if (!IT->second->animates){
                    uiOffset=IT->second->index;
                    return;
                }
            }
            IT++;
        }
        
        uiOffset=0;

        
    } else if (GameLogic::gameMode==TRAPMODE){
        if (propButtonDict.size()==0){
            uiOffset=0;
            return;
        }
        uiOffset=propButtonDict.back()->index;
        
        std::map<int, SpriteDict*>::iterator IT = Sprite::theSpriteAssets.upper_bound(uiOffset);
        while (IT !=Sprite::theSpriteAssets.end()) {
            if (!IT->second->prop){
                
            } else {
                if (IT->second->animates){
                    uiOffset=IT->second->index;
                    return;
                }
            }
            IT++;
        }
        
        uiOffset=0;
    } else if (GameLogic::gameMode==DOORMODE){
        if (doorButtonDict.size()==0){
            uiOffset=0;
            return;
        }
        uiOffset=doorButtonDict.back()->index;
        
        if (Door::theDoorAssets.upper_bound(uiOffset)==Door::theDoorAssets.end()){
            uiOffset=0;
        }
    }
    
    

}



void UserInterface::think(){
    UserInterface::timer++;
    

}






void UserInterface::generatePlayIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues ){
    
    
    previousGamemode=GameLogic::gameMode;
    
    playButtonDict.clear();
    
    int counterMax = numberOfTilesMiddle;
    
    Player::UItiles(uiOffset, counterMax, &playButtonDict, false, true);
    
    
    for (int counter = 0; counter<playButtonDict.size()&&counter<numberOfTilesMiddle; counter++){
        
        PlayIcons theData = playButtonDict[counter];
        
        int index;
        
        float largestValue  = findLargestOf8Values(&theData.vertex[0], &index);
        
        GLfloat sV;
        
        if (index%2){
            sV=maxValues.second*0.75;
            
        }else {
            sV=maxValues.first*0.75;
            
        }
        
        theData.vertex[0]/=largestValue;
        theData.vertex[1]/=largestValue;
        theData.vertex[2]/=largestValue;
        theData.vertex[3]/=largestValue;
        theData.vertex[4]/=largestValue;
        theData.vertex[5]/=largestValue;
        theData.vertex[6]/=largestValue;
        theData.vertex[7]/=largestValue;
        
        GLfloat XStep=startVector.first+(counter*step);
        
        GLfloat YStep = startVector.second;
        
        (*values)[(start+counter)*16+ 0]=theData.vertex[0]*sV+XStep;
        (*values)[(start+counter)*16+ 1]=theData.vertex[1]*sV+YStep;
        (*values)[(start+counter)*16+ 2]=theData.cords[0];
        (*values)[(start+counter)*16+ 3]=theData.cords[1];
        
        (*values)[(start+counter)*16+ 4]=theData.vertex[2]*sV+XStep;
        (*values)[(start+counter)*16+ 5]=theData.vertex[3]*sV+YStep;
        (*values)[(start+counter)*16+ 6]=theData.cords[0];
        (*values)[(start+counter)*16+ 7]=theData.cords[3];
        
        (*values)[(start+counter)*16+ 8]=theData.vertex[4]*sV+XStep;
        (*values)[(start+counter)*16+ 9]=theData.vertex[5]*sV+YStep;
        (*values)[(start+counter)*16+10]=theData.cords[2];
        (*values)[(start+counter)*16+11]=theData.cords[1];
        
        (*values)[(start+counter)*16+12]=theData.vertex[6]*sV+XStep;
        (*values)[(start+counter)*16+13]=theData.vertex[7]*sV+YStep;
        (*values)[(start+counter)*16+14]=theData.cords[2];
        (*values)[(start+counter)*16+15]=theData.cords[3];
    }
}




void UserInterface::generateDoorIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues ){

    previousGamemode=GameLogic::gameMode;
    
    doorButtonDict.clear();
    
    int counterMax = numberOfTilesMiddle;
    
    Door::UItiles(uiOffset, counterMax, &doorButtonDict);
    
    
    
    for (int counter = 0; counter<doorButtonDict.size()&&counter<numberOfTilesMiddle; counter++){
        
        GLfloat texture[12];
        DoorDict * value = doorButtonDict[counter];
        
        if (!value->texture){
            value->texture= generateTexture(value);
            
        }
        
        int target = value->frames[0];
        
        Door::dumpTextureCord(value->index, target, texture,0);
        int index = 0;
        float largestValue  = findLargestOf8Values(&texture[4], &index);
        
        GLfloat sV;
        
        if (index%2){
            sV=maxValues.second*0.75;
            
        }else {
            sV=maxValues.first*0.75;
            
        }
        
        
        texture[4]/=largestValue;
        texture[5]/=largestValue;
        texture[6]/=largestValue;
        texture[7]/=largestValue;
        texture[8]/=largestValue;
        texture[9]/=largestValue;
        texture[10]/=largestValue;
        texture[11]/=largestValue;
        
        
        
        GLfloat XStep=startVector.first+(counter*step);
        
        GLfloat YStep = startVector.second;
        
        
        
        (*values)[(start+counter)*16+ 0]=texture[4]*sV+XStep;
        (*values)[(start+counter)*16+ 1]=texture[5]*sV+YStep;
        (*values)[(start+counter)*16+ 2]=texture[0];
        (*values)[(start+counter)*16+ 3]=texture[1];
        
        (*values)[(start+counter)*16+ 4]=texture[6]*sV+XStep;
        (*values)[(start+counter)*16+ 5]=texture[7]*sV+YStep;
        (*values)[(start+counter)*16+ 6]=texture[0];
        (*values)[(start+counter)*16+ 7]=texture[3];
        
        (*values)[(start+counter)*16+ 8]=texture[8]*sV+XStep;
        (*values)[(start+counter)*16+ 9]=texture[9]*sV+YStep;
        (*values)[(start+counter)*16+10]=texture[2];
        (*values)[(start+counter)*16+11]=texture[1];
        
        (*values)[(start+counter)*16+12]=texture[10]*sV+XStep;
        (*values)[(start+counter)*16+13]=texture[11]*sV+YStep;
        (*values)[(start+counter)*16+14]=texture[2];
        (*values)[(start+counter)*16+15]=texture[3];
        
        
        
    }
    
    
    
    
}



void UserInterface::generateSpriteIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues ){
    
    previousGamemode=GameLogic::gameMode;
    
    
   
    
    spriteButtonDict.clear();
    
    int counterMax = numberOfTilesMiddle;
    
    Sprite::UItiles(uiOffset, counterMax, &spriteButtonDict, false, true);

    
    for (int counter = 0; counter<spriteButtonDict.size()&&counter<numberOfTilesMiddle; counter++){
        
        
        
        
        GLfloat texture[12];
        SpriteDict * value = spriteButtonDict[counter];
        
        if (!value->texture){
            value->texture= generateTexture(value);
            
        }
        
        int target = value->frames[3];
        
        Sprite::dumpTextureCord(value->index, target, texture,0);
        int index = 0;
        float largestValue  = findLargestOf8Values(&texture[4], &index);
        
        GLfloat sV;
        
        if (index%2){
            sV=maxValues.second*0.75;
            
        }else {
            sV=maxValues.first*0.75;
            
        }
        
        
        //float largestValue = *largestValuePointer;
        
        
        
        texture[4]/=largestValue;
        texture[5]/=largestValue;
        texture[6]/=largestValue;
        texture[7]/=largestValue;
        texture[8]/=largestValue;
        texture[9]/=largestValue;
        texture[10]/=largestValue;
        texture[11]/=largestValue;
        
        
        
        GLfloat XStep=startVector.first+(counter*step);
        
        GLfloat YStep = startVector.second;
        
        
        
        (*values)[(start+counter)*16+ 0]=texture[4]*sV+XStep;
        (*values)[(start+counter)*16+ 1]=texture[5]*sV+YStep;
        (*values)[(start+counter)*16+ 2]=texture[0];
        (*values)[(start+counter)*16+ 3]=texture[1];
        
        (*values)[(start+counter)*16+ 4]=texture[6]*sV+XStep;
        (*values)[(start+counter)*16+ 5]=texture[7]*sV+YStep;
        (*values)[(start+counter)*16+ 6]=texture[0];
        (*values)[(start+counter)*16+ 7]=texture[3];
        
        (*values)[(start+counter)*16+ 8]=texture[8]*sV+XStep;
        (*values)[(start+counter)*16+ 9]=texture[9]*sV+YStep;
        (*values)[(start+counter)*16+10]=texture[2];
        (*values)[(start+counter)*16+11]=texture[1];
        
        (*values)[(start+counter)*16+12]=texture[10]*sV+XStep;
        (*values)[(start+counter)*16+13]=texture[11]*sV+YStep;
        (*values)[(start+counter)*16+14]=texture[2];
        (*values)[(start+counter)*16+15]=texture[3];
        
        
        
    }
    
    
    
    
}


void UserInterface::generateTrapIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues ){
    
    
    previousGamemode=GameLogic::gameMode;
    
    
    
    
    propButtonDict.clear();
    
    int counterMax = numberOfTilesMiddle;
    
    Sprite::UItiles(uiOffset, counterMax, &propButtonDict, true, true);
    
    //::UItiles(uiOffset, counterMax, &tileButtonDict);
    
    for (int counter = 0; counter<propButtonDict.size(); counter++){
        
        
        
        
        GLfloat texture[12];
        SpriteDict * value = propButtonDict[counter];
        
        if (!value->texture){
            value->texture= generateTexture(value);
            
        }
        
        Sprite::dumpTextureCord(value->index, 0, texture,0);
        int index = 0;
        float largestValue  = findLargestOf8Values(&texture[4], &index);
        
        GLfloat sV;
        
        if (index%2){
            sV=maxValues.second;
            
        }else {
            sV=maxValues.first;
            
        }
        
        
        //float largestValue = *largestValuePointer;
        
        
        
        texture[4]/=largestValue;
        texture[5]/=largestValue;
        texture[6]/=largestValue;
        texture[7]/=largestValue;
        texture[8]/=largestValue;
        texture[9]/=largestValue;
        texture[10]/=largestValue;
        texture[11]/=largestValue;
        
        
        
        GLfloat XStep=startVector.first+(counter*step);
        
        GLfloat YStep = startVector.second;
        
        
        
        (*values)[(start+counter)*16+ 0]=texture[4]*sV+XStep;
        (*values)[(start+counter)*16+ 1]=texture[5]*sV+YStep;
        (*values)[(start+counter)*16+ 2]=texture[0];
        (*values)[(start+counter)*16+ 3]=texture[1];
        
        (*values)[(start+counter)*16+ 4]=texture[6]*sV+XStep;
        (*values)[(start+counter)*16+ 5]=texture[7]*sV+YStep;
        (*values)[(start+counter)*16+ 6]=texture[0];
        (*values)[(start+counter)*16+ 7]=texture[3];
        
        (*values)[(start+counter)*16+ 8]=texture[8]*sV+XStep;
        (*values)[(start+counter)*16+ 9]=texture[9]*sV+YStep;
        (*values)[(start+counter)*16+10]=texture[2];
        (*values)[(start+counter)*16+11]=texture[1];
        
        (*values)[(start+counter)*16+12]=texture[10]*sV+XStep;
        (*values)[(start+counter)*16+13]=texture[11]*sV+YStep;
        (*values)[(start+counter)*16+14]=texture[2];
        (*values)[(start+counter)*16+15]=texture[3];
        
        
        
    }
    

    
}





void UserInterface::generatePropIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues ){

    previousGamemode=GameLogic::gameMode;
    
    

    
    propButtonDict.clear();
    
    int counterMax = numberOfTilesMiddle;
    
    Sprite::UItiles(uiOffset, counterMax, &propButtonDict, true, false);
    
    //::UItiles(uiOffset, counterMax, &tileButtonDict);
    
    for (int counter = 0; counter<propButtonDict.size(); counter++){
        
        
        
        
        GLfloat texture[12];
        SpriteDict * value = propButtonDict[counter];
        
        if (!value->texture){
            value->texture= generateTexture(value);
            
        }
        
        Sprite::dumpTextureCord(value->index, 0, texture,0);
        int index = 0;
        float largestValue  = findLargestOf8Values(&texture[4], &index);
        
        GLfloat sV;
        
        if (index%2){
            sV=maxValues.second;
        
        }else {
            sV=maxValues.first;
        
        }
        
        
        //float largestValue = *largestValuePointer;
        
        
        
        texture[4]/=largestValue;
        texture[5]/=largestValue;
        texture[6]/=largestValue;
        texture[7]/=largestValue;
        texture[8]/=largestValue;
        texture[9]/=largestValue;
        texture[10]/=largestValue;
        texture[11]/=largestValue;
        
        
        
        GLfloat XStep=startVector.first+(counter*step);
        
        GLfloat YStep = startVector.second;
        
        
        
        (*values)[(start+counter)*16+ 0]=texture[4]*sV+XStep;
        (*values)[(start+counter)*16+ 1]=texture[5]*sV+YStep;
        (*values)[(start+counter)*16+ 2]=texture[0];
        (*values)[(start+counter)*16+ 3]=texture[1];
        
        (*values)[(start+counter)*16+ 4]=texture[6]*sV+XStep;
        (*values)[(start+counter)*16+ 5]=texture[7]*sV+YStep;
        (*values)[(start+counter)*16+ 6]=texture[0];
        (*values)[(start+counter)*16+ 7]=texture[3];
        
        (*values)[(start+counter)*16+ 8]=texture[8]*sV+XStep;
        (*values)[(start+counter)*16+ 9]=texture[9]*sV+YStep;
        (*values)[(start+counter)*16+10]=texture[2];
        (*values)[(start+counter)*16+11]=texture[1];
        
        (*values)[(start+counter)*16+12]=texture[10]*sV+XStep;
        (*values)[(start+counter)*16+13]=texture[11]*sV+YStep;
        (*values)[(start+counter)*16+14]=texture[2];
        (*values)[(start+counter)*16+15]=texture[3];
        

     
    }
    
    
    
    
}

void UserInterface::generateTileIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues ){
    

    previousGamemode=GameLogic::gameMode;

    tileButtonDict.clear();
    
    int counterMax = numberOfTilesMiddle;
    
    Tiles::UItiles(uiOffset, counterMax, &tileButtonDict);
    
    for (int counter = 0; counter<tileButtonDict.size(); counter++){
        
        GLfloat X1 =startVector.first-maxValues.first+(counter*step);
        GLfloat X2 =startVector.first+maxValues.first+(counter*step);
        
        GLfloat Y1 = startVector.second+maxValues.first*T_V;
        GLfloat Y2 = startVector.second-maxValues.first*T_V;
        
        (*values)[(start+counter)*16+ 0]=X1;
        (*values)[(start+counter)*16+ 1]=Y1;
        (*values)[(start+counter)*16+ 2]=0;
        (*values)[(start+counter)*16+ 3]=0;
        
        (*values)[(start+counter)*16+ 4]=X1;
        (*values)[(start+counter)*16+ 5]=Y2;
        (*values)[(start+counter)*16+ 6]=0;
        (*values)[(start+counter)*16+ 7]=1;
        
        (*values)[(start+counter)*16+ 8]=X2;
        (*values)[(start+counter)*16+ 9]=Y1;
        (*values)[(start+counter)*16+10]=1;
        (*values)[(start+counter)*16+11]=0;
        
        (*values)[(start+counter)*16+12]=X2;
        (*values)[(start+counter)*16+13]=Y2;
        (*values)[(start+counter)*16+14]=1;
        (*values)[(start+counter)*16+15]=1;
        
       
    }
}





void UserInterface::updateDashboard(){
    if (!uiIcons[0]){
        uiIcons[0] = generateTexture(findFile("EnergyButtonIcon.png"));
    }
    
    if (!uiIcons[1]){
        uiIcons[1] = generateTexture(findFile("AwarenessButtonIcon.png"));
    }
    
    if (!uiIcons[2]){
        uiIcons[2] = generateTexture(findFile("TrapButtonIcon.png"));
    }
    
    if (!uiIcons[3]){
        uiIcons[3] = generateTexture(findFile("TileButtonIcon.png"));
    }
    
    
    {
               
        ///////////
        
        GLfloat width = GameLogic::width*(4*0.01);
        
        GLfloat centerX = GameLogic::width-width-width/10.0;
        GLfloat centerY = GameLogic::height-width;
        
        
        
        GLfloat theWidthValue = GameLogic::width*(3.0/100.0);
        GLfloat modValue = theWidthValue*(1.0+2*0.1);
        
        //GLfloat rightSideBox = GameLogic::width,  top=GameLogic::height,bot=centerY-width*9.0f;
        
        std::string trapP = std::to_string((int)GameLogic::numberOfTrapsInPlay);
        std::string trapN = std::to_string((int)GameLogic::numberOfTrapsNeeded);
        std::string tilesP = std::to_string((int)GameLogic::numberOfTilesInPlay);
        std::string tilesN = std::to_string((int)GameLogic::numberOfTilesNeeded);
        
        std::string taText = trapP +"/" + trapN;
        std::string tiText = tilesP +"/" + tilesN;
        
        char textValue[400];
		sprintfAL(textValue, "%i/%i", (int)GameLogic::awareness, (int)GameLogic::awarenessSink);
        
        scareEnergy.setText(std::to_string((int)GameLogic::energy));
        scareEnergy.setMod(modValue);
        scareEnergy.SetCursor(centerX-width+(width*-0.4), centerY+width-width*1);
        scareEnergy.updateRC();
        
        if (GameLogic::energy<100){
        
            scareEnergy.color[0]= 181.0/255.0;
            scareEnergy.color[1]=  36.0/255.0;
            scareEnergy.color[2]=  36.0/255.0;
            
        }else if (GameLogic::energy<250){
        
            scareEnergy.color[0]=240.0/255.0;
            scareEnergy.color[1]=240.0/255.0;
            scareEnergy.color[2]=21.0/255.0;
            
            }else{
        scareEnergy.color[0]= 168.0/255.0;
        scareEnergy.color[1]= 50.0/255.0;
        scareEnergy.color[2]= 156.0/255.0;
        }
        
        awarenessLevel.setText(textValue);
        awarenessLevel.setMod(modValue);
        awarenessLevel.SetCursor(centerX-width+(width*-0.4), centerY+width-width*3.5f);
        awarenessLevel.updateRC();
        if (GameLogic::awareness>GameLogic::awarenessSink*2.0){
            
            awarenessLevel.color[0]= 181.0/255.0;
            awarenessLevel.color[1]=  36.0/255.0;
            awarenessLevel.color[2]=  36.0/255.0;
            
        }else if (GameLogic::awareness>GameLogic::awarenessSink){
            
            awarenessLevel.color[0]=240.0/255.0;
            awarenessLevel.color[1]=240.0/255.0;
            awarenessLevel.color[2]=21.0/255.0;
            
        }else{
            awarenessLevel.color[0]= 168.0/255.0;
            awarenessLevel.color[1]= 50.0/255.0;
            awarenessLevel.color[2]= 156.0/255.0;
        }
        trapCount.setText(taText);
        trapCount.setMod(modValue);
        trapCount.SetCursor(centerX-width+(width*-0.4), centerY+width-width*6.0f);
        trapCount.updateRC();
        trapCount.color[0]= 168.0/255.0;
        trapCount.color[1]= 50.0/255.0;
        trapCount.color[2]= 156.0/255.0;
        
        tileAmount.setText(tiText);
        tileAmount.setMod(modValue);
        tileAmount.SetCursor(centerX-width+(width*-0.4), centerY+width-width*8.5f);
        tileAmount.updateRC();
        tileAmount.color[0]= 168.0/255.0;
        tileAmount.color[1]= 50.0/255.0;
        tileAmount.color[2]= 156.0/255.0;
        
        //GLfloat leftSideBox=GameLogic::width-(modValue*9.2);
        
        
        GLfloat vertex [] = {
            
            centerX-width,  centerY+width-width*0,  0.0,0.0,
            centerX-width,  centerY-width-width*0,  0.0,1.0,
            centerX+width,  centerY+width-width*0,  1.0,0.0,
            centerX+width,  centerY-width-width*0,  1.0,1.0,
            
            centerX-width,  centerY+width-width*2.50f,  0.0,0.0,
            centerX-width,  centerY-width-width*2.50f,  0.0,1.0,
            centerX+width,  centerY+width-width*2.50f,  1.0,0.0,
            centerX+width,  centerY-width-width*2.50f,  1.0,1.0,
            
            centerX-width,  centerY+width-width*5.0f,  0.0,0.0,
            centerX-width,  centerY-width-width*5.0f,  0.0,1.0,
            centerX+width,  centerY+width-width*5.0f,  1.0,0.0,
            centerX+width,  centerY-width-width*5.0f,  1.0,1.0,
            
            centerX-width,  centerY+width-width*7.5f,  0.0,0.0,
            centerX-width,  centerY-width-width*7.5f,  0.0,1.0,
            centerX+width,  centerY+width-width*7.5f,  1.0,0.0,
            centerX+width,  centerY-width-width*7.5f,  1.0,1.0,
            
			static_cast<GLfloat>(tileAmount.bbL), static_cast<GLfloat>(tileAmount.bbT), 0.0, 0.0,
			static_cast<GLfloat>(tileAmount.bbL), static_cast<GLfloat>(tileAmount.bbB), 0.0, 0.0,
			static_cast<GLfloat>(tileAmount.bbR), static_cast<GLfloat>(tileAmount.bbT), 0.0, 0.0,
			static_cast<GLfloat>(tileAmount.bbR), static_cast<GLfloat>(tileAmount.bbB), 0.0, 0.0,
            
			static_cast<GLfloat>(trapCount.bbL), static_cast<GLfloat>(trapCount.bbT), 0.0, 0.0,
			static_cast<GLfloat>(trapCount.bbL), static_cast<GLfloat>(trapCount.bbB), 0.0, 0.0,
			static_cast<GLfloat>(trapCount.bbR), static_cast<GLfloat>(trapCount.bbT), 0.0, 0.0,
			static_cast<GLfloat>(trapCount.bbR), static_cast<GLfloat>(trapCount.bbB), 0.0, 0.0,

			static_cast<GLfloat>(awarenessLevel.bbL), static_cast<GLfloat>(awarenessLevel.bbT), 0.0, 0.0,
			static_cast<GLfloat>(awarenessLevel.bbL), static_cast<GLfloat>(awarenessLevel.bbB), 0.0, 0.0,
			centerX, static_cast<GLfloat>(awarenessLevel.bbT), 0.0, 0.0,
			centerX, static_cast<GLfloat>(awarenessLevel.bbB), 0.0, 0.0,

			static_cast<GLfloat>(scareEnergy.bbL), static_cast<GLfloat>(scareEnergy.bbT), 0.0, 0.0,
			static_cast<GLfloat>(scareEnergy.bbL), static_cast<GLfloat>(scareEnergy.bbB), 0.0, 0.0,
			centerX, static_cast<GLfloat>(scareEnergy.bbT), 0.0, 0.0,
			centerX, static_cast<GLfloat>(scareEnergy.bbB), 0.0, 0.0,
            
            
            
        };
        
        
        if (!_vertexArrayDashboard){
            glGenVertexArraysCon(1, &_vertexArrayDashboard);
        }
        glBindVertexArrayCon(_vertexArrayDashboard);
        
        if(!_vertexBufferDashboard){
            glGenBuffers(1, &_vertexBufferDashboard);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferDashboard);
        
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertex),
                     vertex,
                     GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
    }
    
    {
     //   GLfloat tileHeight = (GameLogic::height*2)*0.16;
     //   GLfloat y1 =-GameLogic::height+tileHeight;
        
        
     
    }
    

    
}

void UserInterface::renderDashboard(){
    
    
    
    if (_vertexArrayDashboard){
        
        
        int value = BobZombie::dashBoardDraw();
        
        float color[4] = {244.0/255.0,216.0/255.0,195.0/255.0,0.3};
        
      
        
        
        
        
        
        
        if (value>1){
            ShaderManager::enableSolidColorUI(color);
            
            glBindVertexArrayCon(_vertexArrayDashboard);
            glBindTexture(GL_TEXTURE_2D, *uiIcons[3]);
            //glDrawArrays(GL_TRIANGLE_STRIP, 16,	4 );
			//glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
			//glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
			//glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
            
            ShaderManager::enableUIShader();
            glBindVertexArrayCon(_vertexArrayDashboard);
            glBindTexture(GL_TEXTURE_2D, *uiIcons[0]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            scareEnergy.renderColorChangeUI();
			
            
            if (value>4){
                ShaderManager::enableUIShader();
                glBindVertexArrayCon(_vertexArrayDashboard);
                glBindTexture(GL_TEXTURE_2D, *uiIcons[1]);
                glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

                awarenessLevel.renderColorChangeUI();
				
            }
            
            if (value>2){
                ShaderManager::enableUIShader();
                glBindVertexArrayCon(_vertexArrayDashboard);
                glBindTexture(GL_TEXTURE_2D, *uiIcons[2]);
                glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

                trapCount.renderColorChangeUI();
				
            }
            
            if (value>3){
                ShaderManager::enableUIShader();
                glBindVertexArrayCon(_vertexArrayDashboard);
                glBindTexture(GL_TEXTURE_2D, *uiIcons[3]);
                glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	
                tileAmount.renderColorChangeUI();
				
                
            }
        }
    }
    

    
}

void UserInterface::loadUITextures(){

    if (UserInterface::arrowLeft){
        return;
    }
    
    if (!UserInterface::arrowLeft){
        UserInterface::arrowLeft = generateTexture(findFile("ArrowL.png"));
    }
    
    if (!UserInterface::arrowRight){
        UserInterface::arrowRight= generateTexture(findFile("ArrowR.png"));
        
    }
    
    if (!optionButtonTexture){
        optionButtonTexture = generateTexture(findFile("OptionButton.png"));
        
    }
    
    if (!backButtonTexture){
        backButtonTexture = generateTexture(findFile("BackButton.png"));
        
    }
    
    
    if (!tileButtonTexture){
        tileButtonTexture = generateTexture(findFile("TileChangeButton.png"));
    }
    
    if (!doorButtonTexture){
        doorButtonTexture = generateTexture(findFile("DoorButton.png"));
    }
    
    if (!spriteButtonTexture){
        spriteButtonTexture = generateTexture(findFile("SpriteButton.png"));
    }
    if (!ghostButtonTexture){
        ghostButtonTexture = generateTexture(findFile("GhostButton.png"));
    }
    
    if (!propButtonTexture){
        propButtonTexture = generateTexture(findFile("PropButton.png"));
    }
    
    if (!trapButtonTexture){
        trapButtonTexture = generateTexture(findFile("TrapButton.png"));
    }
    
    if (!middleIcons){
        middleIcons= generateTexture(findFile("IconPlace.png"));
        
    }
    
    if (!endIcons){
        endIcons = generateTexture(findFile("OptionEND.png"));
    }
    
    if (!barTop){
        barTop = generateTexture(findFile("HeaderUIBar.png"));
    }
    
    if (!textureBorder){
        textureBorder = generateTextureR(findFile("WindowBorder.png"));
        
    }
    
    if (!textureBottomBorder){
        textureBottomBorder=generateTextureR(findFile("BottomBorder.png"));
    }
    
    if (!creditsButton){
        creditsButton = generateTexture(findFile("Credits.png"));
        
    }

}

#include "TranslationMatrix.h"

void UserInterface::updateWarningMessages(){
    GLfloat tileHeight = (GameLogic::height*2)*0.16;
    GLfloat y1 =-GameLogic::height+tileHeight+2;
    
    

    if (SAVE_OR_NOT_MODE == menuMode){
        warning.setText(TranslationMatrix::translate("PRESS_F1_FOR"));
    }else if (GameLogic::energy<200){
		warning.setText(TranslationMatrix::translate("ENERGY_GETTING_LOW"));
	} else if (GameLogic::awareness>GameLogic::awarenessSink*2){
        warning.setText(TranslationMatrix::translate("BULLDOZER_BE_COMING"));
    } else if (GameLogic::awareness>GameLogic::awarenessSink){
        warning.setText(TranslationMatrix::translate("COPS_BE_COMING"));
    } else if (GameLogic::awareness>GameLogic::awarenessSink*0.8){
         warning.setText(TranslationMatrix::translate("AWARENESS_ALMOST_TO_HIGH"));
    } else if (GameLogic::gameMode==TILEMODE){
        char textValue[400];
        sprintfAL(textValue, TranslationMatrix::translate("NEED_SPEND_MORE_ON_TILES").c_str(),GameLogic::numberOfTilesNeeded-GameLogic::numberOfTilesInPlay);
        
        warning.setText(textValue);
    
    } else if (GameLogic::gameMode==TRAPMODE){
        
        char textValue[400];
        sprintfAL(textValue, TranslationMatrix::translate("NEED_MORE_TRAPS_A").c_str(),GameLogic::numberOfTrapsNeeded-GameLogic::numberOfTrapsInPlay);
    
        warning.setText(textValue);
    } else if (GameLogic::gameMode==PROPMODE){
        char textValue[400];
		sprintfAL(textValue, TranslationMatrix::translate("NEED_MORE_PROP_VALUE").c_str(), int(GameLogic::awareness), int(GameLogic::awarenessSink));
        
        warning.setText(textValue);
    
    }
    
    
    GLfloat modValue = GameLogic::width*(6*0.01);
    warning.lineWidth = GameLogic::width*2;
     warning.setMod(modValue);
     warning.SetCursor(0, y1+modValue);
    warning.updateCNewLine();

    
    
}

void UserInterface::renderWarningMessages(){

	if (BobZombie::currentTutorialState == NO_TUTORIAL){
		if (GameLogic::gameMode == TRAPMODE || GameLogic::gameMode == TILEMODE || GameLogic::gameMode == PROPMODE){

			warning.renderYMove();
		}
		else if (GameLogic::awareness > GameLogic::awarenessSink*0.8||GameLogic::energy<200||SAVE_OR_NOT_MODE == menuMode){
			warning.renderYMove();
        } 
	}

}

/*
static void updateWarningMessages();
static void renderWarningMessages();
*/

static GLuint _vertexArrayMoveButtons;
static GLuint _vertexBufferMoveButtons;
static cpBB moveTheSpriteButton;
static cpBB deleteTheSpriteButton;


void moveSpriteUpdate(){

	if (Sprite::moveSprite){

		GLfloat iconWidth = GameLogic::width / 25.0;

		Sprite::moveSprite->updateNoGL();
		Sprite::moveSprite->updateGL();

		GLfloat movePosX = Sprite::moveSprite->boundingBox.l;
		GLfloat movePosY = Sprite::moveSprite->boundingBox.t;

		GLfloat erasePosX = Sprite::moveSprite->boundingBox.r;
		GLfloat erasePosY = Sprite::moveSprite->boundingBox.t;

		GLfloat vertex[] = {
			movePosX, movePosY, 0, 0,
			movePosX, movePosY - iconWidth, 0, 1,
			movePosX + iconWidth, movePosY, 1, 0,
			movePosX + iconWidth, movePosY - iconWidth, 1, 1,

			erasePosX, erasePosY, 0, 0,
			erasePosX, erasePosY - iconWidth, 0, 1,
			erasePosX - iconWidth, erasePosY, 1, 0,
			erasePosX - iconWidth, erasePosY - iconWidth, 1, 1,

		};
		
		moveTheSpriteButton = cpBBNew(movePosX, movePosY - iconWidth, movePosX + iconWidth, movePosY);
		deleteTheSpriteButton = cpBBNew(erasePosX - iconWidth, movePosY - iconWidth, erasePosX, movePosY);


		if (!_vertexArrayMoveButtons){
			glGenVertexArraysCon(1, &_vertexArrayMoveButtons);
		}
		glBindVertexArrayCon(_vertexArrayMoveButtons);

		if (!_vertexBufferMoveButtons){
			glGenBuffers(1, &_vertexBufferMoveButtons);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferMoveButtons);

		glBufferData(GL_ARRAY_BUFFER,
			sizeof(vertex),
			vertex,
			GL_STATIC_DRAW);


		glEnableVertexAttribArray(GLKVertexAttribPosition);
		glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4 * 4, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
		glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4 * 4, BUFFER_OFFSET(8));


	}



}

void moveSpriteRender(){

	if (Sprite::moveSprite){

		ShaderManager::enableTextureShader();
		glBindVertexArrayCon(_vertexArrayMoveButtons);
		
		glBindTexture(GL_TEXTURE_2D, *moveSpriteButton);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		glBindTexture(GL_TEXTURE_2D, *deleteButton);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	}
}

bool moveSpriteInput(MOUSE_STATES input, cpVect control, long theTouchNumber){
	if (input == MOUSE_STATE_UP){
		if (Sprite::moveSprite){
			control = cpvmult(control, Camera::zoom);
			cpVect thePos = cpvadd(control, cpv(-Camera::posX, -Camera::posY));

			if (cpBBContainsVect(moveTheSpriteButton, thePos)){
				Sprite::canMove = true;
				return true;
			}

			if (cpBBContainsVect(deleteTheSpriteButton, thePos)){
				Sprite::removeSprite(Sprite::moveSprite);
				return true;
			}
		}
	}
	return false;
}





void UserInterface::update(){
    //Warning Message
    updateWarningMessages();
    
    //Check if the Game is Over and load textures.
    gameOverDrawUpdateTest();
    
    //Load all the UI textures //NOTE THESE ARE HARDCODED
    loadUITextures();

    if (SAVE_OR_NOT_MODE == menuMode){
        UserInterface::updateSound();
    }
    
    // Update the Dashboard on the top right of the screen.
    if (GameLogic::actPlay()){
        updateDashboard();

		moveSpriteUpdate();

    }
  
    
    {
        
        GLfloat sOfOB = GameLogic::width*0.09;
    
        GLfloat leftButton = GameLogic::width-sOfOB;
        GLfloat rightButton = GameLogic::width;

        GLfloat midButton;
        {
         GLfloat tileHeight = (GameLogic::height*2)*0.16;
         GLfloat y1 =-GameLogic::height+tileHeight;
         GLfloat theWidthValue = -GameLogic::width*(3.0/100.0);
            
            midButton = y1+theWidthValue+sOfOB;
        }
        
        
       // GLfloat midButton = 0+sOfOB;
        
        
        
        
        

        
        GLfloat vertex[] = {
            leftButton,     midButton,                      0,1,
            leftButton,     midButton+sOfOB,                0,0,
            rightButton,    midButton,                      1,1,
            rightButton,    midButton+sOfOB,                1,0,
            
            leftButton,     midButton+(sOfOB*1.00f),        0,1,
            leftButton,     midButton+(sOfOB*2.00f),        0,0,
            rightButton,    midButton+(sOfOB*1.00f),        1,1,
            rightButton,    midButton+(sOfOB*2.00f),        1,0,
            
            leftButton,     midButton+(sOfOB*2.00f),        0,1,
            leftButton,     midButton+(sOfOB*3.00f),        0,0,
            rightButton,    midButton+(sOfOB*2.00f),        1,1,
            rightButton,    midButton+(sOfOB*3.00f),        1,0,
            
            leftButton,     midButton+(sOfOB*3.00f),        0,1,
            leftButton,     midButton+(sOfOB*4.00f),        0,0,
            rightButton,    midButton+(sOfOB*3.00f),        1,1,
            rightButton,    midButton+(sOfOB*4.00f),        1,0,
            
            leftButton,     midButton+(sOfOB*4.00f),        0,1,
            leftButton,     midButton+(sOfOB*5.00f),        0,0,
            rightButton,    midButton+(sOfOB*4.00f),        1,1,
            rightButton,    midButton+(sOfOB*5.00f),        1,0,
        
            leftButton,     midButton+(sOfOB*5.00f),        0,1,
            leftButton,     midButton+(sOfOB*6.00f),        0,0,
            rightButton,    midButton+(sOfOB*5.00f),        1,1,
            rightButton,    midButton+(sOfOB*6.00f),        1,0,
        };
        
    
        changeModeButtons[0]=cpBBNew(leftButton, midButton+sOfOB*0, rightButton, midButton+sOfOB*1);
        changeModeButtons[1]=cpBBNew(leftButton, midButton+sOfOB*1, rightButton, midButton+sOfOB*2);
        changeModeButtons[2]=cpBBNew(leftButton, midButton+sOfOB*2, rightButton, midButton+sOfOB*3);
        changeModeButtons[3]=cpBBNew(leftButton, midButton+sOfOB*3, rightButton, midButton+sOfOB*4);
        changeModeButtons[4]=cpBBNew(leftButton, midButton+sOfOB*4, rightButton, midButton+sOfOB*5);
        changeModeButtons[5]=cpBBNew(leftButton, midButton+sOfOB*5, rightButton, midButton+sOfOB*6);
        //optionButton=cpBBNew(leftTouch, topTouch-sOfOB, leftTouch+sOfOB, topTouch);
        
        
        if (!_vertexArrayChangeUI){
            glGenVertexArraysCon(1, &_vertexArrayChangeUI);
        }
        glBindVertexArrayCon(_vertexArrayChangeUI);
        
        if(!_vertexBufferChangeUI){
            glGenBuffers(1, &_vertexBufferChangeUI);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferChangeUI);
        
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertex),
                     vertex,
                     GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    
    }
    
    
    
    //UIBar
    {
        
 
        


        // Option Button
        {
            GLfloat leftTouch = -GameLogic::width;
            GLfloat topTouch = GameLogic::height-(GameLogic::width*0.03);
            GLfloat sOfOB = GameLogic::width*0.18;

        
            if (!_vertexArrayOption){
                glGenVertexArraysCon(1, &_vertexArrayOption);
            }
            glBindVertexArrayCon(_vertexArrayOption);
            
            if(!_vertexArrayOption){
                glGenBuffers(1, &_vertexArrayOption);
            }
            glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayOption);
            
            GLfloat vertex[] = {
                leftTouch,topTouch,0,0,
                leftTouch,topTouch-sOfOB,0,1,
                leftTouch+sOfOB,topTouch,1,0,
                leftTouch+sOfOB,topTouch-sOfOB,1,1,
            };
            
            optionButton=cpBBNew(leftTouch, topTouch-sOfOB, leftTouch+sOfOB, topTouch);
            

            glBufferData(GL_ARRAY_BUFFER,
                         sizeof(vertex),
                         vertex,
                         GL_STATIC_DRAW);
            
            
            glEnableVertexAttribArray(GLKVertexAttribPosition);
            glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
            glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));

        }
        
        
    
    // Userbar
    {
        
        
     
        
        
        GLfloat tileHeight = (GameLogic::height*2)*0.16;
        GLfloat y1 =-GameLogic::height+tileHeight;
        GLfloat tileWidth = tileHeight*(700.0/1000.0);
        int numberOfTiles = (GameLogic::width*2.0)/tileWidth;
        numberOfTilesMiddle=numberOfTiles-2;
        GLfloat spaceWidth = ((GameLogic::width*2.0) - (tileWidth*numberOfTiles))/2.0;
        
        
        GLfloat ls = -GameLogic::width;
        
        GLfloat sizeOfButton = GameLogic::height*2*0.068;
        GLfloat xSpaceButton = (tileWidth -sizeOfButton)/2;
        GLfloat ySpaceButton = (tileHeight - sizeOfButton)/2;
        
        
        
        //Userbar
        
        
        
        //Change This
        
        if (GameLogic::actPlay()){
            theBarArea = cpBBNew(ls+spaceWidth, -GameLogic::height, GameLogic::width-spaceWidth, y1);
            //
            std::vector<GLfloat> vertex;
            vertex.assign(16*numberOfTiles*2+16, 0);
            
            buttons.clear();
            
            
            {
                int counter = numberOfTiles*2;
                
                vertex[counter*16+0]=ls+spaceWidth;
                vertex[counter*16+1]=y1+2;
                vertex[counter*16+2]=0;
                vertex[counter*16+3]=0;
                
                vertex[counter*16+4]=ls+spaceWidth;
                vertex[counter*16+5]=y1;
                vertex[counter*16+6]=0;
                vertex[counter*16+7]=1;
                
                vertex[counter*16+8]=GameLogic::width-spaceWidth;
                vertex[counter*16+9]=y1+2;
                vertex[counter*16+10]=numberOfTiles;
                vertex[counter*16+11]=0;
                
                vertex[counter*16+12]=GameLogic::width-spaceWidth;
                vertex[counter*16+13]=y1;
                vertex[counter*16+14]=numberOfTiles;
                vertex[counter*16+15]=1;
                
                
            }
            
            
            
            for (int counter = 0; counter<numberOfTiles; counter++){
                
                GLfloat theWidthValue = -GameLogic::width*(3.0/100.0);
                
                GLfloat x1 = ls+spaceWidth+(tileWidth*counter);
                GLfloat x2 = ls+spaceWidth+(tileWidth*(counter+1));
                GLfloat Y1 = y1-theWidthValue;
                GLfloat Y2 = y1-tileHeight-theWidthValue;
                
                
                
                
                vertex[counter*16+0]=x1;
                vertex[counter*16+1]=Y1;
                vertex[counter*16+2]=0;
                vertex[counter*16+3]=0;
                
                vertex[counter*16+4]=x1;
                vertex[counter*16+5]=Y2;
                vertex[counter*16+6]=0;
                vertex[counter*16+7]=1;
                
                vertex[counter*16+8]=x2;
                vertex[counter*16+9]=Y1;
                vertex[counter*16+10]=1;
                vertex[counter*16+11]=0;
                
                vertex[counter*16+12]=x2;
                vertex[counter*16+13]=Y2;
                vertex[counter*16+14]=1;
                vertex[counter*16+15]=1;
                
               
                if (costs.size()<=counter){
                    
                    CBitmapFont * theNewText = new CBitmapFont();
                    costs.push_back(theNewText);
                  
                    
                }
                
                if (counter!=0){
            
               
                //costs[counter-1]->setText("");
                costs[counter-1]->setMod(tileWidth/4.5);
                costs[counter-1]->SetCursor((x2+x1)/2.0, y1-tileWidth/4.5);
                
                }
                
                buttons.push_back(cpBBNew(ls+spaceWidth+(tileWidth*counter), y1-tileHeight, ls+spaceWidth+(tileWidth*(counter+1)), y1));
            }
            
            
            {
                
                size_t counter = numberOfTiles;
                
                vertex[counter*16+0]=ls+spaceWidth+xSpaceButton;
                vertex[counter*16+1]=y1-ySpaceButton;
                vertex[counter*16+2]=0;
                vertex[counter*16+3]=0;
                
                vertex[counter*16+4]=ls+spaceWidth+xSpaceButton;
                vertex[counter*16+5]=y1-ySpaceButton-sizeOfButton;
                vertex[counter*16+6]=0;
                vertex[counter*16+7]=1;
                
                vertex[counter*16+8]=ls+spaceWidth+xSpaceButton+sizeOfButton;
                vertex[counter*16+9]=y1-ySpaceButton;
                vertex[counter*16+10]=1;
                vertex[counter*16+11]=0;
                
                vertex[counter*16+12]=ls+spaceWidth+xSpaceButton+sizeOfButton;
                vertex[counter*16+13]=y1-ySpaceButton-sizeOfButton;
                vertex[counter*16+14]=1;
                vertex[counter*16+15]=1;
                
                
                
                
            }
            
            
            if (GameLogic::gameMode==TILEMODE){
                
                std::pair<GLfloat, GLfloat> startVector =std::pair<GLfloat, GLfloat>(
                                                                                     ls+spaceWidth+(tileWidth*1.5),
                                                                                     y1-(tileHeight/2.0));
                std::pair<GLfloat, GLfloat> maxValues = std::pair<GLfloat, GLfloat>(
                                                                                    tileWidth/2.2,tileHeight/2.2);
                
                generateTileIcons(numberOfTiles+1,
                                  &vertex, startVector, tileWidth, maxValues);
                
                int counter=0;
                
                for (std::vector<TileDict*>::iterator IT = tileButtonDict.begin(); IT != tileButtonDict.end(); IT++) {
                    
                    if (counter<costs.size()){
                        costs[counter]->setText(std::to_string((*IT)->energyRequired));
                        costs[counter]->update();
                    }
                    counter++;
                    
                }
                
                for (counter=counter; counter<costs.size(); counter++){
                    costs[counter]->setText("");
                    costs[counter]->update();
                    
                    
                }
                
                
                
                
            } else if (GameLogic::gameMode==PROPMODE){
                
                std::pair<GLfloat, GLfloat> startVector =std::pair<GLfloat, GLfloat>(
                                                                                     ls+spaceWidth+(tileWidth*1.5),
                                                                                     y1-(tileHeight/2.0));
                std::pair<GLfloat, GLfloat> maxValues = std::pair<GLfloat, GLfloat>(
                                                                                    tileWidth/3.5,tileHeight/3.5);
                
                generatePropIcons(numberOfTiles+1,
                                  &vertex, startVector, tileWidth, maxValues);
                
                int counter=0;
                
                for (std::vector<SpriteDict*>::iterator IT = propButtonDict.begin();
                     IT != propButtonDict.end();
                     IT++){
                    
                    if (counter<costs.size()){
                        costs[counter]->setText(std::to_string((*IT)->cost));
                        costs[counter]->update();
                    }
                    counter++;
                }
                
                for (counter+=0; counter<costs.size(); counter++){
                    costs[counter]->setText("");
                    costs[counter]->update();
                    
                    
                }
                
            
            }else if (GameLogic::gameMode==TRAPMODE){
                
                
                std::pair<GLfloat, GLfloat> startVector =std::pair<GLfloat, GLfloat>(
                                                                                     ls+spaceWidth+(tileWidth*1.5),
                                                                                     y1-(tileHeight/2.0));
                std::pair<GLfloat, GLfloat> maxValues = std::pair<GLfloat, GLfloat>(
                                                                                    tileWidth/3.5,tileHeight/3.5);
                
                generateTrapIcons(numberOfTiles+1,
                                  &vertex, startVector, tileWidth, maxValues);
                
                int counter=0;
                
                for (std::vector<SpriteDict*>::iterator IT = propButtonDict.begin();
                     IT != propButtonDict.end();
                     IT++){
                    
                    if (counter<costs.size()){
                        costs[counter]->setText(std::to_string((*IT)->cost));
                        costs[counter]->update();
                    }
                    counter++;
                }
                
                for (counter=counter; counter<costs.size(); counter++){
                    costs[counter]->setText("");
                    costs[counter]->update();
                
                
                }
                
                
                
                
                
            } else if (GameLogic::gameMode == SPRITEMODE){
                
                std::pair<GLfloat, GLfloat> startVector =std::pair<GLfloat, GLfloat>(
                                                                                     ls+spaceWidth+(tileWidth*1.5),
                                                                                     y1-(tileHeight/2.0));
                std::pair<GLfloat, GLfloat> maxValues = std::pair<GLfloat, GLfloat>(
                                                                                    tileWidth/2.2,tileHeight/2.2);
                
                generateSpriteIcons(numberOfTiles+1,
                                    &vertex, startVector, tileWidth, maxValues);
                
                
                int counter=0;
                
                for (std::vector<SpriteDict*>::iterator IT = spriteButtonDict.begin();
                     IT != spriteButtonDict.end();
                     IT++){
                    
                    if (counter<costs.size()){
                        costs[counter]->setText(std::to_string((*IT)->cost));
                        costs[counter]->update();
                    }
                    counter++;
                }
                
                
                
            } else if (GameLogic::gameMode == DOORMODE){
            
                
                std::pair<GLfloat, GLfloat> startVector =std::pair<GLfloat, GLfloat>(
                                                                                     ls+spaceWidth+(tileWidth*1.5),
                                                                                     y1-(tileHeight/2.0));
                std::pair<GLfloat, GLfloat> maxValues = std::pair<GLfloat, GLfloat>(
                                                                                    tileWidth/2.2,tileHeight/2.2);
                
                generateDoorIcons(numberOfTiles+1,
                                    &vertex, startVector, tileWidth, maxValues);
                
                
            
            } else if (GameLogic::gameMode == PLAYMODE){
            
                
                std::pair<GLfloat, GLfloat> startVector =std::pair<GLfloat, GLfloat>(
                                                                                     ls+spaceWidth+(tileWidth*1.5),
                                                                                     y1-(tileHeight/2.0));
                std::pair<GLfloat, GLfloat> maxValues = std::pair<GLfloat, GLfloat>(
                                                                                    tileWidth/2.2,tileHeight/2.2);
                
                generatePlayIcons(numberOfTiles+1,
                                    &vertex, startVector, tileWidth, maxValues);
                
                
                int counter=0;
                
                for (std::vector<PlayIcons>::iterator IT = playButtonDict.begin();
                     IT != playButtonDict.end();
                     IT++){
                    
                    if (counter<costs.size()){
                        costs[counter]->setText(std::to_string((*IT).cost));
                        costs[counter]->update();
                    }
                    counter++;
                }

             
                
            
            
            
            }
            
            
            {
                
                size_t counter = numberOfTiles+numberOfTiles-1;
                
                
                
                
                
                
                vertex[counter*16+0]=ls+spaceWidth+(tileWidth*(counter-numberOfTiles))+xSpaceButton;
                vertex[counter*16+1]=y1-ySpaceButton;
                vertex[counter*16+2]=0;
                vertex[counter*16+3]=0;
                
                vertex[counter*16+4]=ls+spaceWidth+(tileWidth*(counter-numberOfTiles))+xSpaceButton;
                vertex[counter*16+5]=y1-ySpaceButton-sizeOfButton;
                vertex[counter*16+6]=0;
                vertex[counter*16+7]=1;
                
                vertex[counter*16+8]=ls+spaceWidth+(tileWidth*(counter-numberOfTiles))+xSpaceButton+sizeOfButton;
                vertex[counter*16+9]=y1-ySpaceButton;
                vertex[counter*16+10]=1;
                vertex[counter*16+11]=0;
                
                vertex[counter*16+12]=ls+spaceWidth+(tileWidth*(counter-numberOfTiles))+xSpaceButton+sizeOfButton;
                vertex[counter*16+13]=y1-ySpaceButton-sizeOfButton;
                vertex[counter*16+14]=1;
                vertex[counter*16+15]=1;
                
                
                
                
                
            }
            
            
            
          /*
            {
                
                size_t counter = numberOfTiles+numberOfTiles-1;
                
                
                
                
                
                
                vertex[counter*16+0]=ls+spaceWidth+(tileWidth*(counter-numberOfTiles))+xSpaceButton;
                vertex[counter*16+1]=y1-ySpaceButton;
                vertex[counter*16+2]=0;
                vertex[counter*16+3]=0;
                
                vertex[counter*16+4]=ls+spaceWidth+(tileWidth*(counter-numberOfTiles))+xSpaceButton;
                vertex[counter*16+5]=y1-ySpaceButton-sizeOfButton;
                vertex[counter*16+6]=0;
                vertex[counter*16+7]=1;
                
                vertex[counter*16+8]=ls+spaceWidth+(tileWidth*(counter-numberOfTiles))+xSpaceButton+sizeOfButton;
                vertex[counter*16+9]=y1-ySpaceButton;
                vertex[counter*16+10]=1;
                vertex[counter*16+11]=0;
                
                vertex[counter*16+12]=ls+spaceWidth+(tileWidth*(counter-numberOfTiles))+xSpaceButton+sizeOfButton;
                vertex[counter*16+13]=y1-ySpaceButton-sizeOfButton;
                vertex[counter*16+14]=1;
                vertex[counter*16+15]=1;
                
                
                
                
                
            }
            
            */
            
            
            
            if (!_vertexArrayUserBar){
                glGenVertexArraysCon(1, &_vertexArrayUserBar);
            }
            glBindVertexArrayCon(_vertexArrayUserBar);
            
            if(!_vertexBufferUserBar){
                glGenBuffers(1, &_vertexBufferUserBar);
            }
            glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferUserBar);

            
            
            glBufferData(GL_ARRAY_BUFFER,
                         sizeof(GLfloat)*vertex.size(),
                         &vertex[0],
                         GL_STATIC_DRAW);
            
            
            glEnableVertexAttribArray(GLKVertexAttribPosition);
            glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
            glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
            
            
            vertex.clear();
            vertex.assign(16*numberOfTiles, 0);
            
            
         
            
            
            for (int counter = 0; counter<numberOfTiles; counter++){
                
                
                
                float amount =0;
              
                if (playButtonDict.size()>counter){
                    
                    long sizeLength =  GameClock::time - Player::scareDict[counter].lastClick;
                    
                    
                    if (sizeLength>Player::scareDict[counter].reload){
                        
                        amount = 1.0;
                        
                        
                    }else {
                        amount = ((double)sizeLength)/((double)Player::scareDict[counter].reload);
                        
                        
                    }
                }
                
                
                
                GLfloat theWidthValue = -GameLogic::width*(3.0/100.0);
                
                GLfloat x1 = ls+spaceWidth+(tileWidth*counter)+tileWidth;
                GLfloat x2 = ls+spaceWidth+(tileWidth*counter)+tileWidth+tileWidth*amount;
                GLfloat Y1 = y1-tileHeight-theWidthValue+(tileHeight/20.0);
                GLfloat Y2 = y1-tileHeight-theWidthValue;
                
                
                
                
                vertex[counter*16+0]=x1;
                vertex[counter*16+1]=Y1;
                vertex[counter*16+2]=0;
                vertex[counter*16+3]=0;
                
                vertex[counter*16+4]=x1;
                vertex[counter*16+5]=Y2;
                vertex[counter*16+6]=0;
                vertex[counter*16+7]=1;
                
                vertex[counter*16+8]=x2;
                vertex[counter*16+9]=Y1;
                vertex[counter*16+10]=1;
                vertex[counter*16+11]=0;
                
                vertex[counter*16+12]=x2;
                vertex[counter*16+13]=Y2;
                vertex[counter*16+14]=1;
                vertex[counter*16+15]=1;
                
            }
            
            
            
            
            if (!_vertexArrayLoadLine){
                glGenVertexArraysCon(1, &_vertexArrayLoadLine);
            }
            glBindVertexArrayCon(_vertexArrayLoadLine);
            
            if(!_vertexBufferLoadLone){
                glGenBuffers(1, &_vertexBufferLoadLone);
            }
            glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferLoadLone);
            
            
            
            glBufferData(GL_ARRAY_BUFFER,
                         sizeof(GLfloat)*vertex.size(),
                         &vertex[0],
                         GL_STATIC_DRAW);
            
            
            glEnableVertexAttribArray(GLKVertexAttribPosition);
            glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
            glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
            
        }
        
    }
    }
    
    

    if (GameLogic::gameMode==OPENINGMODE){
        
        previousGamemode=GameLogic::gameMode;
        
        
        if (!playButton){
            playButton = generateTexture(findFile("PlayButton.png"));
        }
        
        if (!deleteButton){
            deleteButton = generateTexture(findFile("DeleteButton.png"));
        }

		if (!moveSpriteButton){
			moveSpriteButton = generateTexture(findFile("MoveSpriteIcon.png"));
		}

        
        if (!modStartButton){
            modStartButton = generateTexture(findFile("ModActivate.png"));
        }
        
        
        {
            
            GLfloat buttonsHeight = GameLogic::width/8.0;
            GLfloat playButtonWidth = buttonsHeight * 442.0/256.0;
            GLfloat deleteButtonWidth = buttonsHeight * 256.0/283.0;
            
            GLfloat bCW = GameLogic::width/3.0;
            GLfloat bCH = -GameLogic::height/2.0-GameLogic::height/4.0;
            
            
            GLfloat vertex[] = {
                -bCW-playButtonWidth,bCH+buttonsHeight,0,0,
                -bCW-playButtonWidth,bCH-buttonsHeight,0,1,
                -bCW+playButtonWidth,bCH+buttonsHeight,1,0,
                -bCW+playButtonWidth,bCH-buttonsHeight,1,1,
                
                 bCW-deleteButtonWidth,bCH+buttonsHeight,0,0,
                 bCW-deleteButtonWidth,bCH-buttonsHeight,0,1,
                 bCW+deleteButtonWidth,bCH+buttonsHeight,1,0,
                 bCW+deleteButtonWidth,bCH-buttonsHeight,1,1,
                
                
                -bCW-deleteButtonWidth,bCH+buttonsHeight,0,0,
                -bCW-deleteButtonWidth,bCH-buttonsHeight,0,1,
                -bCW+deleteButtonWidth,bCH+buttonsHeight,1,0,
                -bCW+deleteButtonWidth,bCH-buttonsHeight,1,1,
                
                
                
            };
            
            
            playButtonLocation =cpBBNew(-bCW-playButtonWidth,
                                         bCH-buttonsHeight,
                                        -bCW+playButtonWidth,
                                         bCH+buttonsHeight);
            deleteButtonLocation = cpBBNew(bCW-deleteButtonWidth,
                                           bCH-buttonsHeight,
                                           bCW+deleteButtonWidth,
                                           bCH+buttonsHeight);
            modButtonLocation = cpBBNew(-bCW-deleteButtonWidth, bCH-buttonsHeight, -bCW+deleteButtonWidth, bCH+buttonsHeight);
            
            
            
            if (!_playDeleteButtonArray){
                glGenVertexArraysCon(1, &_playDeleteButtonArray);
            }
            glBindVertexArrayCon(_playDeleteButtonArray);
            
            if(!_playDeleteButtonBuffer){
                glGenBuffers(1, &_playDeleteButtonBuffer);
            }
            glBindBuffer(GL_ARRAY_BUFFER, _playDeleteButtonBuffer);
            
            
            
            glBufferData(GL_ARRAY_BUFFER,
                         sizeof(vertex),
                         vertex,
                         GL_STATIC_DRAW);

            glEnableVertexAttribArray(GLKVertexAttribPosition);
            glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
            glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
            
        

            
          
        
        }
        

        if (!freePlay){
            freePlay = generateTexture(findFile("FreePlay.png"));
        }
        
        if (!scenerio){
            scenerio = generateTexture(findFile("Scenerio.png"));
        }
        
        if (!mods){
            mods = generateTexture(findFile("Mods.png"));
        }
        
        if (!newGame){
            newGame = generateTexture(findFile("NewGame.png"));
            
            thumbNailTextures[0]=newGame;
            thumbNailTextures[1]=newGame;
            thumbNailTextures[2]=newGame;
        }
        
        
        updateBackground();
        
        
        
        
        
        
        

            
            for (int counter =0; counter<3; counter++){
            
                if (loadedSaves[counter]){
                    if (thumbNailTextures[counter]==newGame){
                        GLuint texture_id;
                        glGenTextures(1, &texture_id);
                        glBindTexture(GL_TEXTURE_2D, texture_id);
        
                        glTexImage2D(
                                     GL_TEXTURE_2D,
                                     0,
                                     GL_RGBA,
                                     256,
                                     256,
                                     0,
                                     GL_RGBA,
                                     GL_UNSIGNED_BYTE,
                                     &loadedSaves[counter]->thumbnail[0]);
                        
                        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    
                        GLuint * pointerTexture = new GLuint;
                        *pointerTexture = texture_id;
                        thumbNailTextures[counter]=pointerTexture;
                    }
                    
                
                }
            
            
            }

            
        
        if (menuMode==MOD_MODE){
            //modName.update();
            modText.update();
            
        }
        
        if (!_vertexArray){
            glGenVertexArraysCon(1, &_vertexArray);
        }
        glBindVertexArrayCon(_vertexArray);
        
        if(!_vertexBuffer){
            glGenBuffers(1, &_vertexBuffer);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        
     
        
    
        std::vector<GLfloat> vertex;
        
        vertex.push_back(-100);
        vertex.push_back(100);
        vertex.push_back(0);
        vertex.push_back(0);
        
        vertex.push_back(-100);
        vertex.push_back(-100);
        vertex.push_back(0);
        vertex.push_back(1);
        
        vertex.push_back(100);
        vertex.push_back(100);
        vertex.push_back(1);
        vertex.push_back(0);
        
        vertex.push_back(100);
        vertex.push_back(-100);
        vertex.push_back(1);
        vertex.push_back(1);
        
        
        GLfloat barHeight = 5;
        
        vertex.push_back(10);
        vertex.push_back(barHeight);
        vertex.push_back(0);
        vertex.push_back(0);
        
        vertex.push_back(10);
        vertex.push_back(-barHeight);
        vertex.push_back(0);
        vertex.push_back(1);
        
        vertex.push_back(GameLogic::width-5);
        vertex.push_back(barHeight);
        vertex.push_back(1);
        vertex.push_back(0);
        
        vertex.push_back(GameLogic::width-5);
        vertex.push_back(-barHeight);
        vertex.push_back(1);
        vertex.push_back(1);
        
        
        vertex.push_back(10);
        vertex.push_back(barHeight-15);
        vertex.push_back(0);
        vertex.push_back(0);
        
        vertex.push_back(10);
        vertex.push_back(-barHeight-15);
        vertex.push_back(0);
        vertex.push_back(1);
        
        vertex.push_back(GameLogic::width-5);
        vertex.push_back(barHeight-15);
        vertex.push_back(1);
        vertex.push_back(0);
        
        vertex.push_back(GameLogic::width-5);
        vertex.push_back(-barHeight-15);
        vertex.push_back(1);
        vertex.push_back(1);
        
        //
        
        vertex.push_back(10);
        vertex.push_back(barHeight-30);
        vertex.push_back(0);
        vertex.push_back(0);
        
        vertex.push_back(10);
        vertex.push_back(-barHeight-30);
        vertex.push_back(0);
        vertex.push_back(1);
        
        vertex.push_back(GameLogic::width-5);
        vertex.push_back(barHeight-30);
        vertex.push_back(1);
        vertex.push_back(0);
        
        vertex.push_back(GameLogic::width-5);
        vertex.push_back(-barHeight-30);
        vertex.push_back(1);
        vertex.push_back(1);
        
        
        
        {
            double widthFreePlay = 289.0/65.0;
            double widthTotal = (GameLogic::width-5)-10;
            double centerPosition = 10 + (widthTotal/2.0);
            double spacer = widthFreePlay*3;
            
            GLfloat leftPos = centerPosition-spacer;
            GLfloat rightPos = centerPosition+spacer;
        
            
            
            vertex.push_back(leftPos);
            vertex.push_back(barHeight-2);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(leftPos);
            vertex.push_back(-barHeight+2);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(rightPos);
            vertex.push_back(barHeight-2);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(rightPos);
            vertex.push_back(-barHeight+2);
            vertex.push_back(1);
            vertex.push_back(1);
            
            
            
        }
        
        
        {
            double widthFreePlay =  301.0/54.0;
            double widthTotal = (GameLogic::width-5)-10;
            double centerPosition = 10 + (widthTotal/2.0);
            double spacer = widthFreePlay*3;
            
            GLfloat leftPos = centerPosition-spacer;
            GLfloat rightPos = centerPosition+spacer;
            
            
            
            vertex.push_back(leftPos);
            vertex.push_back(barHeight-15-2);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(leftPos);
            vertex.push_back(-barHeight-15+2);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(rightPos);
            vertex.push_back(barHeight-15-2);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(rightPos);
            vertex.push_back(-barHeight-15+2);
            vertex.push_back(1);
            vertex.push_back(1);
        
        
        }
        
        
        {
        
            
            double widthFreePlay =  185.0/52.0;
            double widthTotal = (GameLogic::width-5)-10;
            double centerPosition = 10 + (widthTotal/2.0);
            double spacer = widthFreePlay*3;
            
            GLfloat leftPos = centerPosition-spacer;
            GLfloat rightPos = centerPosition+spacer;
            
            
            
            vertex.push_back(leftPos);
            vertex.push_back(barHeight-30-2);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(leftPos);
            vertex.push_back(-barHeight-30+2);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(rightPos);
            vertex.push_back(barHeight-30-2);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(rightPos);
            vertex.push_back(-barHeight-30+2);
            vertex.push_back(1);
            vertex.push_back(1);

        }
        

        
        
        {
        // SAVE GAME ATTRIBUTES
            
            GLfloat spacer = GameLogic::width/2.0;
            GLfloat width;
        
            width = GameLogic::width/2.25;
            
            GLfloat halfWidth = width/2.0;
            GLfloat leftMid = -GameLogic::width+spacer;
            GLfloat middleMid = leftMid+spacer;
            GLfloat rightMid = middleMid+spacer;

    
            GLfloat amountToMoveA = (UserInterface::timer+0.0)/100.0;
            GLfloat amountToMoveB = (UserInterface::timer-7.50)/100.0;
            GLfloat amountToMoveC = (UserInterface::timer-15.0)/100.0;

            amountToMoveA = (amountToMoveA>1.0)?1.0:amountToMoveA;
            amountToMoveB = (amountToMoveB>1.0)?1.0:amountToMoveB;
            amountToMoveC = (amountToMoveC>1.0)?1.0:amountToMoveC;
            
            
            GLfloat tweenLeft = Tweener::cubicEaseOut(amountToMoveA, -150, 150, 1.0);
            GLfloat tweenMid = Tweener::cubicEaseOut(amountToMoveB, -150, 150, 1.0);
            GLfloat tweenRight = Tweener::cubicEaseOut(amountToMoveC, -150, 150, 1.0);

            vertex.push_back(leftMid-halfWidth);
            vertex.push_back(halfWidth+tweenLeft);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(leftMid-halfWidth);
            vertex.push_back(-halfWidth+tweenLeft);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(leftMid+halfWidth);
            vertex.push_back(halfWidth+tweenLeft);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(leftMid+halfWidth);
            vertex.push_back(-halfWidth+tweenLeft);
            vertex.push_back(1);
            vertex.push_back(1);
            
            
            gameA = cpBBNew(leftMid-halfWidth, -halfWidth+tweenLeft, leftMid+halfWidth, halfWidth+tweenLeft);
        
            //MID
            
            vertex.push_back(middleMid-halfWidth);
            vertex.push_back(halfWidth+tweenMid);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(middleMid-halfWidth);
            vertex.push_back(-halfWidth+tweenMid);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(middleMid+halfWidth);
            vertex.push_back(halfWidth+tweenMid);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(middleMid+halfWidth);
            vertex.push_back(-halfWidth+tweenMid);
            vertex.push_back(1);
            vertex.push_back(1);
        
            gameB = cpBBNew(middleMid-halfWidth, -halfWidth+tweenMid, middleMid+halfWidth, halfWidth+tweenMid);
            
        
            //RIGHT
        
            vertex.push_back(rightMid-halfWidth);
            vertex.push_back(halfWidth+tweenRight);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(rightMid-halfWidth);
            vertex.push_back(-halfWidth+tweenRight);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(rightMid+halfWidth);
            vertex.push_back(halfWidth+tweenRight);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(rightMid+halfWidth);
            vertex.push_back(-halfWidth+tweenRight);
            vertex.push_back(1);
            vertex.push_back(1);
            
            gameC = cpBBNew(rightMid-halfWidth, -halfWidth+tweenRight, rightMid+halfWidth, halfWidth+tweenRight);
        
        }

        {
            // MOD ATTRIBUTES
            
            GLfloat spacer = GameLogic::width/4.0;
            GLfloat width;
            
            width = GameLogic::width/4.25;
            
            GLfloat halfWidth = width/2.0;
            
            GLfloat a = -GameLogic::width+spacer;
            GLfloat b = a+spacer;
            GLfloat c = b+spacer;
            GLfloat d = c+spacer;
            GLfloat e = d+spacer;
            GLfloat f = e+spacer;
            GLfloat g = f+spacer;
            GLfloat h = g+spacer;
            GLfloat i = h+spacer;
        
           
            
            GLfloat amountToMoveA = (UserInterface::timer+0.0)/100.0;
            GLfloat amountToMoveB = (UserInterface::timer-7.50)/100.0;
            GLfloat amountToMoveC = (UserInterface::timer-15.0)/100.0;
            GLfloat amountToMoveD = (UserInterface::timer-22.5)/100.0;
            GLfloat amountToMoveE = (UserInterface::timer-30.00)/100.0;
            GLfloat amountToMoveF = (UserInterface::timer-37.5)/100.0;
            GLfloat amountToMoveG = (UserInterface::timer-45.0)/100.0;
            GLfloat amountToMoveH = (UserInterface::timer-52.5)/100.0;
            GLfloat amountToMoveI = (UserInterface::timer-60.0)/100.0;
            
            amountToMoveA = (amountToMoveA>1.0)?1.0:amountToMoveA;
            amountToMoveB = (amountToMoveB>1.0)?1.0:amountToMoveB;
            amountToMoveC = (amountToMoveC>1.0)?1.0:amountToMoveC;
            amountToMoveD = (amountToMoveD>1.0)?1.0:amountToMoveD;
            amountToMoveE = (amountToMoveE>1.0)?1.0:amountToMoveE;
            amountToMoveF = (amountToMoveF>1.0)?1.0:amountToMoveF;
            amountToMoveG = (amountToMoveG>1.0)?1.0:amountToMoveG;
            amountToMoveH = (amountToMoveH>1.0)?1.0:amountToMoveH;
            amountToMoveI = (amountToMoveI>1.0)?1.0:amountToMoveI;
            
            GLfloat tweenA = Tweener::cubicEaseOut(amountToMoveA, -150, 150, 1.0);
            GLfloat tweenB = Tweener::cubicEaseOut(amountToMoveB, -150, 150, 1.0);
            GLfloat tweenC = Tweener::cubicEaseOut(amountToMoveC, -150, 150, 1.0);
            GLfloat tweenD = Tweener::cubicEaseOut(amountToMoveD, -150, 150, 1.0);
            GLfloat tweenE = Tweener::cubicEaseOut(amountToMoveE, -150, 150, 1.0);
            GLfloat tweenF = Tweener::cubicEaseOut(amountToMoveF, -150, 150, 1.0);
            GLfloat tweenG = Tweener::cubicEaseOut(amountToMoveG, -150, 150, 1.0);
            GLfloat tweenH = Tweener::cubicEaseOut(amountToMoveH, -150, 150, 1.0);
            GLfloat tweenI = Tweener::cubicEaseOut(amountToMoveI, -150, 150, 1.0);
            

            
            vertex.push_back(a-halfWidth);
            vertex.push_back(halfWidth+tweenA);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(a-halfWidth);
            vertex.push_back(-halfWidth+tweenA);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(a+halfWidth);
            vertex.push_back(halfWidth+tweenA);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(a+halfWidth);
            vertex.push_back(-halfWidth+tweenA);
            vertex.push_back(1);
            vertex.push_back(1);
            
            modButton[0] = cpBBNew(a-halfWidth, -halfWidth+tweenA, a+halfWidth, halfWidth+tweenA);
            
            vertex.push_back(b-halfWidth);
            vertex.push_back(halfWidth+tweenB);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(b-halfWidth);
            vertex.push_back(-halfWidth+tweenB);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(b+halfWidth);
            vertex.push_back(halfWidth+tweenB);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(b+halfWidth);
            vertex.push_back(-halfWidth+tweenB);
            vertex.push_back(1);
            vertex.push_back(1);
            //
            modButton[1] = cpBBNew(b-halfWidth, -halfWidth+tweenB, b+halfWidth, halfWidth+tweenB);
            
            vertex.push_back(c-halfWidth);
            vertex.push_back(halfWidth+tweenC);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(c-halfWidth);
            vertex.push_back(-halfWidth+tweenC);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(c+halfWidth);
            vertex.push_back(halfWidth+tweenC);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(c+halfWidth);
            vertex.push_back(-halfWidth+tweenC);
            vertex.push_back(1);
            vertex.push_back(1);
            
            //
            modButton[2] = cpBBNew(c-halfWidth, -halfWidth+tweenC, c+halfWidth, halfWidth+tweenC);
            
            vertex.push_back(d-halfWidth);
            vertex.push_back(halfWidth+tweenD);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(d-halfWidth);
            vertex.push_back(-halfWidth+tweenD);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(d+halfWidth);
            vertex.push_back(halfWidth+tweenD);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(d+halfWidth);
            vertex.push_back(-halfWidth+tweenD);
            vertex.push_back(1);
            vertex.push_back(1);
            
            modButton[3] = cpBBNew(d-halfWidth, -halfWidth+tweenD, d+halfWidth, halfWidth+tweenD);
            
            vertex.push_back(e-halfWidth);
            vertex.push_back(halfWidth+tweenE);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(e-halfWidth);
            vertex.push_back(-halfWidth+tweenE);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(e+halfWidth);
            vertex.push_back(halfWidth+tweenE);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(e+halfWidth);
            vertex.push_back(-halfWidth+tweenE);
            vertex.push_back(1);
            vertex.push_back(1);
            
            //F
            
            modButton[4] = cpBBNew(e-halfWidth, -halfWidth+tweenE, e+halfWidth, halfWidth+tweenE);
            
            vertex.push_back(f-halfWidth);
            vertex.push_back(halfWidth+tweenF);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(f-halfWidth);
            vertex.push_back(-halfWidth+tweenF);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(f+halfWidth);
            vertex.push_back(halfWidth+tweenF);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(f+halfWidth);
            vertex.push_back(-halfWidth+tweenF);
            vertex.push_back(1);
            vertex.push_back(1);
            
            modButton[5] = cpBBNew(f-halfWidth, -halfWidth+tweenF, f+halfWidth, halfWidth+tweenF);
            
            vertex.push_back(g-halfWidth);
            vertex.push_back(halfWidth+tweenG);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(g-halfWidth);
            vertex.push_back(-halfWidth+tweenG);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(g+halfWidth);
            vertex.push_back(halfWidth+tweenG);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(g+halfWidth);
            vertex.push_back(-halfWidth+tweenG);
            vertex.push_back(1);
            vertex.push_back(1);
            
            modButton[6] = cpBBNew(g-halfWidth, -halfWidth+tweenG, g+halfWidth, halfWidth+tweenG);
            
            vertex.push_back(h-halfWidth);
            vertex.push_back(halfWidth+tweenH);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(h-halfWidth);
            vertex.push_back(-halfWidth+tweenH);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(h+halfWidth);
            vertex.push_back(halfWidth+tweenH);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(h+halfWidth);
            vertex.push_back(-halfWidth+tweenH);
            vertex.push_back(1);
            vertex.push_back(1);
            
            modButton[7] = cpBBNew(h-halfWidth, -halfWidth+tweenH, h+halfWidth, halfWidth+tweenH);
            
            vertex.push_back(i-halfWidth);
            vertex.push_back(halfWidth+tweenI);
            vertex.push_back(0);
            vertex.push_back(0);
            
            vertex.push_back(i-halfWidth);
            vertex.push_back(-halfWidth+tweenI);
            vertex.push_back(0);
            vertex.push_back(1);
            
            vertex.push_back(i+halfWidth);
            vertex.push_back(halfWidth+tweenI);
            vertex.push_back(1);
            vertex.push_back(0);
            
            vertex.push_back(i+halfWidth);
            vertex.push_back(-halfWidth+tweenI);
            vertex.push_back(1);
            vertex.push_back(1);
            
            modButton[8] = cpBBNew(h-halfWidth, -halfWidth+tweenH, h+halfWidth, halfWidth+tweenH);
            
            
        }

        

      
        
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(GLfloat)*vertex.size(),
                     &vertex[0],
                     GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    }
    
    updateOptionSaveGame();
    
    
    
     GLfloat theWidthValue = GameLogic::width*(3.0/100.0);
    

    
    
    speedDisplay.setText(std::to_string(GameLogic::speedSetting)+ "X");
    speedDisplay.setMod(theWidthValue*3.0);
    {

        
        
       
       
    
        GLfloat tileHeight = (GameLogic::height*2)*0.16;
       
        
        speedDisplay.SetCursor(GameLogic::width-(tileHeight/5.0), -GameLogic::height+tileHeight+(tileHeight/5.0));
    speedDisplay.updateR();
    }

}


void UserInterface::updateBackground(){
    {
        if (!backgroundTexture){
            backgroundTexture = generateTextureR(findFile("BackgroundPattern.png"));
        }
        
        if (!ghostLoadScreen){
            ghostLoadScreen = generateTexture(findFile("GHOSTLoadScreen.png"));
        }
        
        if (!eye1){
            eye1 = generateTexture(findFile("EYES-01.png"));
        }
        
        if (!eye2){
            eye2 = generateTexture(findFile("EYES-02.png"));
        }
        if (!eye3){
            eye3 = generateTexture(findFile("EYES-03.png"));
        }
        if (!eye4){
            eye4 = generateTexture(findFile("EYES-04.png"));
        }
        
        
        if (!textureBorder){
            textureBorder = generateTextureR(findFile("WindowBorder.png"));
            
        }
        
        if (!textureBottomBorder){
            textureBottomBorder=generateTextureR(findFile("BottomBorder.png"));
        }
        
        if (!triangleBacking){
            triangleBacking = generateTexture(findFile("Triangle.png"));
        }
        
        
        
        
        
        std::vector<GLfloat> vertex;
        vertex.assign(16*12, 0);
        int counter = 0;
        
        GLfloat textXMax = GameLogic::width/16;
        GLfloat textYMax = GameLogic::height/16;
        
        
        vertex[counter*16+ 0] = -GameLogic::width;
        vertex[counter*16+ 1] = GameLogic::height;
        vertex[counter*16+ 2] = 0;
        vertex[counter*16+ 3] = 0;
        
        vertex[counter*16+ 4] = -GameLogic::width;
        vertex[counter*16+ 5] = -GameLogic::height;
        vertex[counter*16+ 6] = 0;
        vertex[counter*16+ 7] = textYMax;
        
        vertex[counter*16+ 8] = GameLogic::width;
        vertex[counter*16+ 9] = GameLogic::height;
        vertex[counter*16+10] = textXMax;
        vertex[counter*16+11] = 0;
        
        vertex[counter*16+12] = GameLogic::width;
        vertex[counter*16+13] = -GameLogic::height;
        vertex[counter*16+14] = textXMax;
        vertex[counter*16+15] = textYMax;
        
        
        GLfloat widthG = GameLogic::width*2*0.35;
        if (widthG>GameLogic::height*1.6){
            widthG=GameLogic::height*1.6;
            //TSG = GameLogic::height - (GameLogic::height*0.05);
        }
        GLfloat heightG = 592.0/523.0*widthG;
        
        GLfloat LSG = -GameLogic::width + ((GameLogic::width-widthG)/2.0);
        GLfloat TSG = heightG/2.0;
        
        
        
        counter=1;
        
        vertex[counter*16+ 0] = LSG;
        vertex[counter*16+ 1] = TSG;
        vertex[counter*16+ 2] = 0;
        vertex[counter*16+ 3] = 0;
        
        vertex[counter*16+ 4] = LSG;
        vertex[counter*16+ 5] = TSG-heightG;
        vertex[counter*16+ 6] = 0;
        vertex[counter*16+ 7] = 1;
        
        vertex[counter*16+ 8] = LSG+widthG;
        vertex[counter*16+ 9] = TSG;
        vertex[counter*16+10] = 1;
        vertex[counter*16+11] = 0;
        
        vertex[counter*16+12] = LSG+widthG;
        vertex[counter*16+13] = TSG-heightG;
        vertex[counter*16+14] = 1;
        vertex[counter*16+15] = 1;
        
        
        //
        
        GLfloat widthIcon = GameLogic::width/2.0;
        GLfloat heightIcon = widthIcon/(2.884);
        
        GLfloat startX = (GameLogic::width-widthIcon)/2.0;
        GLfloat startY = heightIcon*1.5;
        
        
        
        counter=2;
        
        vertex[counter*16+ 0] = startX;
        vertex[counter*16+ 1] = startY;
        vertex[counter*16+ 2] = 0;
        vertex[counter*16+ 3] = 0;
        
        vertex[counter*16+ 4] = startX;
        vertex[counter*16+ 5] = startY-heightIcon;
        vertex[counter*16+ 6] = 0;
        vertex[counter*16+ 7] = 1;
        
        vertex[counter*16+ 8] = startX+widthIcon;
        vertex[counter*16+ 9] = startY;
        vertex[counter*16+10] = 1;
        vertex[counter*16+11] = 0;
        
        vertex[counter*16+12] = startX+widthIcon;
        vertex[counter*16+13] = startY-heightIcon;
        vertex[counter*16+14] = 1;
        vertex[counter*16+15] = 1;
        
        
        
        counter=3;
        
        vertex[counter*16+ 0] = startX;
        vertex[counter*16+ 1] = startY-heightIcon;
        vertex[counter*16+ 2] = 0;
        vertex[counter*16+ 3] = 0;
        
        vertex[counter*16+ 4] = startX;
        vertex[counter*16+ 5] = startY-heightIcon*2;
        vertex[counter*16+ 6] = 0;
        vertex[counter*16+ 7] = 1;
        
        vertex[counter*16+ 8] = startX+widthIcon;
        vertex[counter*16+ 9] = startY-heightIcon;
        vertex[counter*16+10] = 1;
        vertex[counter*16+11] = 0;
        
        vertex[counter*16+12] = startX+widthIcon;
        vertex[counter*16+13] = startY-heightIcon*2;
        vertex[counter*16+14] = 1;
        vertex[counter*16+15] = 1;
        
        
        
        counter=4;
        
        vertex[counter*16+ 0] = startX;
        vertex[counter*16+ 1] = startY-heightIcon*2;
        vertex[counter*16+ 2] = 0;
        vertex[counter*16+ 3] = 0;
        
        vertex[counter*16+ 4] = startX;
        vertex[counter*16+ 5] = startY-heightIcon*3;
        vertex[counter*16+ 6] = 0;
        vertex[counter*16+ 7] = 1;
        
        vertex[counter*16+ 8] = startX+widthIcon;
        vertex[counter*16+ 9] = startY-heightIcon*2;
        vertex[counter*16+10] = 1;
        vertex[counter*16+11] = 0;
        
        vertex[counter*16+12] = startX+widthIcon;
        vertex[counter*16+13] = startY-heightIcon*3;
        vertex[counter*16+14] = 1;
        vertex[counter*16+15] = 1;
        
        
        
        counter=5;
        
        vertex[counter*16+ 0] = -GameLogic::width;
        vertex[counter*16+ 1] = GameLogic::height;
        vertex[counter*16+ 2] = 0;
        vertex[counter*16+ 3] = 0;
        
        vertex[counter*16+ 4] = -GameLogic::width;
        vertex[counter*16+ 5] = GameLogic::height-(GameLogic::height*0.10);
        vertex[counter*16+ 6] = 0;
        vertex[counter*16+ 7] = 1;
        
        vertex[counter*16+ 8] = GameLogic::width;
        vertex[counter*16+ 9] = GameLogic::height;
        vertex[counter*16+10] = textXMax*2;
        vertex[counter*16+11] = 0;
        
        vertex[counter*16+12] = GameLogic::width;
        vertex[counter*16+13] = GameLogic::height-(GameLogic::height*0.10);
        vertex[counter*16+14] = textXMax*2;
        vertex[counter*16+15] = 1;
        
        counter=6;
        
        vertex[counter*16+ 0] = -GameLogic::width;
        vertex[counter*16+ 1] = -GameLogic::height+(GameLogic::height*0.10);
        vertex[counter*16+ 2] = 0;
        vertex[counter*16+ 3] = 0;
        
        vertex[counter*16+ 4] = -GameLogic::width;
        vertex[counter*16+ 5] = -GameLogic::height;
        vertex[counter*16+ 6] = 0;
        vertex[counter*16+ 7] = 1;
        
        vertex[counter*16+ 8] = GameLogic::width;
        vertex[counter*16+ 9] = -GameLogic::height+(GameLogic::height*0.10);
        vertex[counter*16+10] = textXMax*2;
        vertex[counter*16+11] = 0;
        
        vertex[counter*16+12] = GameLogic::width;
        vertex[counter*16+13] = -GameLogic::height;
        vertex[counter*16+14] = textXMax*2;
        vertex[counter*16+15] = 1;
        
        counter=7;
        
        GLfloat modV = 15.5/24.5;
        GLfloat spaceW = 0.025*GameLogic::width*4;
        GLfloat spaceH = GameLogic::height*0.15;
        
        vertex[counter*16+ 0] = GameLogic::width-spaceW-(widthIcon*modV);
        vertex[counter*16+ 1] = -GameLogic::height+spaceH +(heightIcon*modV);
        vertex[counter*16+ 2] = 0;
        vertex[counter*16+ 3] = 0;
        
        vertex[counter*16+ 4] = GameLogic::width-spaceW-(widthIcon*modV);
        vertex[counter*16+ 5] = -GameLogic::height+spaceH ;
        vertex[counter*16+ 6] = 0;
        vertex[counter*16+ 7] = 1;
        
        vertex[counter*16+ 8] = GameLogic::width-spaceW;
        vertex[counter*16+ 9] = -GameLogic::height+spaceH +(heightIcon*modV);
        vertex[counter*16+10] = 1.0;
        vertex[counter*16+11] = 0;
        
        vertex[counter*16+12] = GameLogic::width-spaceW;
        vertex[counter*16+13] = -GameLogic::height+spaceH ;
        vertex[counter*16+14] = 1.0;
        vertex[counter*16+15] = 1;
        
        
        
        if (menuMode!=GAME_MODE&&menuMode!=MOD_MODE){
            buttons.clear();
            
            buttons.push_back(cpBBNew(startX, startY-heightIcon, startX+widthIcon, startY));
            buttons.push_back(cpBBNew(startX, startY-heightIcon*2, startX+widthIcon, startY-heightIcon));
            
            buttons.push_back(cpBBNew(startX, startY-heightIcon*3, startX+widthIcon, startY-heightIcon*2));
            
            
            buttons.push_back(
                              cpBBNew(GameLogic::width-spaceW-(widthIcon*modV),
                                      -GameLogic::height+spaceH,
                                      GameLogic::width-spaceW,
                                      -GameLogic::height+spaceH +(heightIcon*modV)));
            
        }
        
        
        
        
        //9.4
        //38.6
        //56.9
        //84.6
        
        //20
        //24
        //18
        //19
        
        {
            
            counter=8;
            GLfloat centerEX = GameLogic::width*2*0.094-GameLogic::width;
            GLfloat centerEY = GameLogic::height-(GameLogic::height*2*0.20);
            GLfloat eyeWidth = GameLogic::width*0.04;
            
            
            vertex[counter*16+ 0] = centerEX-eyeWidth;
            vertex[counter*16+ 1] = centerEY-eyeWidth;
            vertex[counter*16+ 2] = 0;
            vertex[counter*16+ 3] = 1;
            
            vertex[counter*16+ 4] = centerEX-eyeWidth;
            vertex[counter*16+ 5] = centerEY+eyeWidth;
            vertex[counter*16+ 6] = 0;
            vertex[counter*16+ 7] = 0;
            
            vertex[counter*16+ 8] = centerEX+eyeWidth;
            vertex[counter*16+ 9] = centerEY-eyeWidth;
            vertex[counter*16+10] = 1.0;
            vertex[counter*16+11] = 1;
            
            vertex[counter*16+12] = centerEX+eyeWidth;
            vertex[counter*16+13] = centerEY+eyeWidth;
            vertex[counter*16+14] = 1.0;
            vertex[counter*16+15] = 0;
            
        }
        
        
        
        
        counter=9;
        
        {
            GLfloat centerEX = GameLogic::width*2*0.386-GameLogic::width;
            GLfloat centerEY = GameLogic::height-(GameLogic::height*2*0.24);
            GLfloat eyeWidth = GameLogic::width*0.04;
            
            
            vertex[counter*16+ 0] = centerEX-eyeWidth;
            vertex[counter*16+ 1] = centerEY-eyeWidth;
            vertex[counter*16+ 2] = 0;
            vertex[counter*16+ 3] = 1;
            
            vertex[counter*16+ 4] = centerEX-eyeWidth;
            vertex[counter*16+ 5] = centerEY+eyeWidth;
            vertex[counter*16+ 6] = 0;
            vertex[counter*16+ 7] = 0;
            
            vertex[counter*16+ 8] = centerEX+eyeWidth;
            vertex[counter*16+ 9] = centerEY-eyeWidth;
            vertex[counter*16+10] = 1.0;
            vertex[counter*16+11] = 1;
            
            vertex[counter*16+12] = centerEX+eyeWidth;
            vertex[counter*16+13] = centerEY+eyeWidth;
            vertex[counter*16+14] = 1.0;
            vertex[counter*16+15] = 0;
            
        }
        
        counter=10;
        
        {
            GLfloat centerEX = GameLogic::width*2*0.569-GameLogic::width;
            GLfloat centerEY = GameLogic::height-(GameLogic::height*2*0.18);
            GLfloat eyeWidth = GameLogic::width*0.04;
            
            
            vertex[counter*16+ 0] = centerEX-eyeWidth;
            vertex[counter*16+ 1] = centerEY-eyeWidth;
            vertex[counter*16+ 2] = 0;
            vertex[counter*16+ 3] = 1;
            
            vertex[counter*16+ 4] = centerEX-eyeWidth;
            vertex[counter*16+ 5] = centerEY+eyeWidth;
            vertex[counter*16+ 6] = 0;
            vertex[counter*16+ 7] = 0;
            
            vertex[counter*16+ 8] = centerEX+eyeWidth;
            vertex[counter*16+ 9] = centerEY-eyeWidth;
            vertex[counter*16+10] = 1.0;
            vertex[counter*16+11] = 1;
            
            vertex[counter*16+12] = centerEX+eyeWidth;
            vertex[counter*16+13] = centerEY+eyeWidth;
            vertex[counter*16+14] = 1.0;
            vertex[counter*16+15] = 0;
            
        }
        counter=11;
        {
            GLfloat centerEX = GameLogic::width*2*0.846-GameLogic::width;
            GLfloat centerEY = GameLogic::height-(GameLogic::height*2*0.19);
            GLfloat eyeWidth = GameLogic::width*0.04;
            
            
            vertex[counter*16+ 0] = centerEX-eyeWidth;
            vertex[counter*16+ 1] = centerEY-eyeWidth;
            vertex[counter*16+ 2] = 0;
            vertex[counter*16+ 3] = 1;
            
            vertex[counter*16+ 4] = centerEX-eyeWidth;
            vertex[counter*16+ 5] = centerEY+eyeWidth;
            vertex[counter*16+ 6] = 0;
            vertex[counter*16+ 7] = 0;
            
            vertex[counter*16+ 8] = centerEX+eyeWidth;
            vertex[counter*16+ 9] = centerEY-eyeWidth;
            vertex[counter*16+10] = 1.0;
            vertex[counter*16+11] = 1;
            
            vertex[counter*16+12] = centerEX+eyeWidth;
            vertex[counter*16+13] = centerEY+eyeWidth;
            vertex[counter*16+14] = 1.0;
            vertex[counter*16+15] = 0;
            
        }
        
        if (!_vertexArrayOpenScreen){
            glGenVertexArraysCon(1, &_vertexArrayOpenScreen);
        }
        glBindVertexArrayCon(_vertexArrayOpenScreen);
        
        if(!_vertexBufferOpenScreen){
            glGenBuffers(1, &_vertexBufferOpenScreen);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferOpenScreen);
        
        
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(GLfloat)*vertex.size(),
                     &vertex[0],
                     GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
    }
}


void UserInterface::renderBackground(){

    if (!_vertexArrayOpenScreen){
        return;
    
    }
    ShaderManager::enableUIShader();
    glBindVertexArrayCon(_vertexArrayOpenScreen);
    glBindTexture(GL_TEXTURE_2D, *backgroundTexture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    ShaderManager::enableTextureAlphaUI(0);
    
    glBindTexture(GL_TEXTURE_2D, *triangleBacking);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    
    ShaderManager::enableUIShader();
    
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    glBindTexture(GL_TEXTURE_2D, *ghostLoadScreen);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    
    glBindTexture(GL_TEXTURE_2D, *eye1);
    glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
    
    glBindTexture(GL_TEXTURE_2D, *eye2);
    glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);
    
    glBindTexture(GL_TEXTURE_2D, *eye3);
    glDrawArrays(GL_TRIANGLE_STRIP, 40, 4);
    
    glBindTexture(GL_TEXTURE_2D, *eye4);
    glDrawArrays(GL_TRIANGLE_STRIP, 44, 4);
    

    
    glBindTexture(GL_TEXTURE_2D, *textureBorder);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
    
    glBindTexture(GL_TEXTURE_2D, *textureBottomBorder);
    glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);



}


void UserInterface::renderModeSelector(){
    
    ShaderManager::enableUIShader();
    int key = BobZombie::sideSelector();
    if (key == 0){
        glBindVertexArrayCon(_vertexArrayChangeUI);
        glBindTexture(GL_TEXTURE_2D, *ghostButtonTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, *trapButtonTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glBindTexture(GL_TEXTURE_2D, *propButtonTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glBindTexture(GL_TEXTURE_2D, *tileButtonTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        glBindTexture(GL_TEXTURE_2D, *doorButtonTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        if (GDebug::debuggingActive){
            glBindTexture(GL_TEXTURE_2D, *spriteButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        }
        
        
    } else if (key==2){
        
      
            glBindVertexArrayCon(_vertexArrayChangeUI);
            
            
            if (BobZombie::hasSelected[0]){
                ShaderManager::enableUIShader();
            }else{
                ShaderManager::enableHalfTextureShaderUI();
            }
            
            
           
            
            glBindTexture(GL_TEXTURE_2D, *ghostButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            if (BobZombie::hasSelected[1]){
                ShaderManager::enableUIShader();
            }else{
                ShaderManager::enableHalfTextureShaderUI();
            }
            glBindTexture(GL_TEXTURE_2D, *trapButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
            
            if (BobZombie::hasSelected[2]){
                ShaderManager::enableUIShader();
            }else{
                ShaderManager::enableHalfTextureShaderUI();
            }
            glBindTexture(GL_TEXTURE_2D, *propButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            
            if (BobZombie::hasSelected[3]){
                ShaderManager::enableUIShader();
            }else{
                ShaderManager::enableHalfTextureShaderUI();
            }
            glBindTexture(GL_TEXTURE_2D, *tileButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
            
            if (BobZombie::hasSelected[4]){
                ShaderManager::enableUIShader();
            }else{
                ShaderManager::enableHalfTextureShaderUI();
            }
            glBindTexture(GL_TEXTURE_2D, *doorButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        
        ShaderManager::enableUIShader();
            if (GDebug::debuggingActive){
                glBindTexture(GL_TEXTURE_2D, *spriteButtonTexture);
                glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
            }
  
        
    } else if (key == 1){
        glBindVertexArrayCon(_vertexArrayChangeUI);

        
    }
    
    
    
}


void UserInterface::render(){
    
    if (GameLogic::actPlay()){
    
        if (gameOverDrawRenderTest()){
            return;
        }
        renderDashboard();
        renderWarningMessages();
		moveSpriteRender();
    }
    
    
    
    
    if (GameLogic::gameMode==TILEMODE){
    
        ShaderManager::enableUIShader();
        
        
        renderModeSelector();

        
        if (previousGamemode==GameLogic::gameMode){
            
            glBindVertexArrayCon(_vertexArrayOption);
            glBindTexture(GL_TEXTURE_2D, *optionButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            if (BobZombie::bottomBarDraw()){
            
            glBindVertexArrayCon(_vertexArrayUserBar);
            glBindTexture(GL_TEXTURE_2D, *endIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            glBindTexture(GL_TEXTURE_2D, *middleIcons);
            for (int counter=0; counter<numberOfTilesMiddle; counter++){
                
                glBindVertexArrayCon(_vertexArrayUserBar);
                glBindTexture(GL_TEXTURE_2D, *middleIcons);
                glDrawArrays(GL_TRIANGLE_STRIP, 4+counter*4, 4);
                
                costs[counter]->render();
            }
            
            glBindVertexArrayCon(_vertexArrayUserBar);
            glBindTexture(GL_TEXTURE_2D, *endIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+4, 4);
            
            
            glBindTexture(GL_TEXTURE_2D, *arrowLeft);
            glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+8, 4);
            
            glBindTexture(GL_TEXTURE_2D, *arrowRight);
            glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+12, 4);
            
            glBindTexture(GL_TEXTURE_2D, *barTop);
            glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+16, 4);
            

            for (int counter=0; counter<tileButtonDict.size(); counter++){
                
                if (tileButtonDict[counter]->index==Tiles::changeTileType){
                    
                    ShaderManager::enableHighLightShaderUI(32);
                    
                    if (tileButtonDict.size()>counter){
                        glBindTexture(GL_TEXTURE_2D, *tileButtonDict[counter]->texture);
                        glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
                    }
                
                    ShaderManager::enableUIShader();
                    
                } else {
                    ShaderManager::enableHalfTextureShaderUI();
                    if (tileButtonDict.size()>counter){
                        glBindTexture(GL_TEXTURE_2D, *tileButtonDict[counter]->texture);
                        glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
                    }
                    ShaderManager::enableUIShader();
                }
            }
            
            }
      
          
            if (GameLogic::speedSetting!=0){
                speedDisplay.render();
            }
            
        }
        
        
        
    } else if (GameLogic::gameMode == DOORMODE){
        
   
        renderModeSelector();
        if (GDebug::debuggingActive){
        glBindTexture(GL_TEXTURE_2D, *spriteButtonTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        }
        if (previousGamemode==GameLogic::gameMode){
            
            
           
            
            
            glBindVertexArrayCon(_vertexArrayOption);
            glBindTexture(GL_TEXTURE_2D, *optionButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            
             if (BobZombie::bottomBarDraw()){
            glBindVertexArrayCon(_vertexArrayUserBar);
            glBindTexture(GL_TEXTURE_2D, *endIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            glBindTexture(GL_TEXTURE_2D, *middleIcons);
            for (int counter=0; counter<numberOfTilesMiddle; counter++){
                
                glBindVertexArrayCon(_vertexArrayUserBar);
                glBindTexture(GL_TEXTURE_2D, *middleIcons);
                glDrawArrays(GL_TRIANGLE_STRIP, 4+counter*4, 4);
                
                //costs[counter]->render();
            }
            
            glBindVertexArrayCon(_vertexArrayUserBar);
            
            
            glBindTexture(GL_TEXTURE_2D, *endIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+4, 4);
            
            
            glBindTexture(GL_TEXTURE_2D, *arrowLeft);
            glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+8, 4);
            
            glBindTexture(GL_TEXTURE_2D, *arrowRight);
            glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+12, 4);
            
            glBindTexture(GL_TEXTURE_2D, *barTop);
            glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+16, 4);
            
            
            for (int counter=0; counter<doorButtonDict.size(); counter++){
                
                if (doorButtonDict[counter]==Door::createThis){
                    
                    ShaderManager::enableHighLightShaderUI(doorButtonDict[counter]->textureWidth);
                    
                    glBindTexture(GL_TEXTURE_2D, *doorButtonDict[counter]->texture);
                    glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
                    
                    
                    ShaderManager::enableUIShader();
                    
                } else {
                    ShaderManager::enableHalfTextureShaderUI();
                    glBindTexture(GL_TEXTURE_2D, *doorButtonDict[counter]->texture);
                    glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
                    ShaderManager::enableUIShader();
                }
            }
            
             }
 
            
            if (GameLogic::speedSetting!=0){
                speedDisplay.render();
            }
        
        
        }
    
    
    }else if (GameLogic::gameMode==SPRITEMODE){
        
        ShaderManager::enableUIShader();
        renderModeSelector();
        if (GDebug::debuggingActive){
            glBindTexture(GL_TEXTURE_2D, *spriteButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        }
        
        if (previousGamemode==GameLogic::gameMode){
            
            

            
            
            glBindVertexArrayCon(_vertexArrayOption);
            glBindTexture(GL_TEXTURE_2D, *optionButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
             if (BobZombie::bottomBarDraw()){
            
            glBindVertexArrayCon(_vertexArrayUserBar);
            glBindTexture(GL_TEXTURE_2D, *endIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            glBindTexture(GL_TEXTURE_2D, *middleIcons);
            for (int counter=0; counter<numberOfTilesMiddle; counter++){
                
                glBindVertexArrayCon(_vertexArrayUserBar);
                glBindTexture(GL_TEXTURE_2D, *middleIcons);
                glDrawArrays(GL_TRIANGLE_STRIP, 4+counter*4, 4);
                
                costs[counter]->render();
            }
            
            glBindVertexArrayCon(_vertexArrayUserBar);
   
            
            glBindTexture(GL_TEXTURE_2D, *endIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+4, 4);
            
            
            glBindTexture(GL_TEXTURE_2D, *arrowLeft);
            glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+8, 4);
            
            glBindTexture(GL_TEXTURE_2D, *arrowRight);
            glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+12, 4);
            
            glBindTexture(GL_TEXTURE_2D, *barTop);
            glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+16, 4);
            
            
            for (int counter=0; counter<spriteButtonDict.size(); counter++){
                
                if (spriteButtonDict[counter]->index==Sprite::changeSpriteType){
                    
                    ShaderManager::enableHighLightShaderUI(spriteButtonDict[counter]->textureWidth);
                    
                    glBindTexture(GL_TEXTURE_2D, *spriteButtonDict[counter]->texture);
                    glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
                    
                    
                    ShaderManager::enableUIShader();
                    
                } else {
                    ShaderManager::enableHalfTextureShaderUI();
                    glBindTexture(GL_TEXTURE_2D, *spriteButtonDict[counter]->texture);
                    glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
                    ShaderManager::enableUIShader();
                }
            }
             }

            
            if (GameLogic::speedSetting!=0){
                speedDisplay.render();
            }
            
        }
    } else if (GameLogic::gameMode==TRAPMODE){
        
        
        renderModeSelector();

        
        if (previousGamemode==GameLogic::gameMode){
            
        
            glBindVertexArrayCon(_vertexArrayOption);
            glBindTexture(GL_TEXTURE_2D, *optionButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
             if (BobZombie::bottomBarDraw()){
            
            glBindVertexArrayCon(_vertexArrayUserBar);
            glBindTexture(GL_TEXTURE_2D, *endIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            glBindTexture(GL_TEXTURE_2D, *middleIcons);
            for (int counter=0; counter<numberOfTilesMiddle; counter++){
                
                glBindVertexArrayCon(_vertexArrayUserBar);
                glBindTexture(GL_TEXTURE_2D, *middleIcons);
                glDrawArrays(GL_TRIANGLE_STRIP, 4+counter*4, 4);
                
                costs[counter]->render();
            }
            
            glBindVertexArrayCon(_vertexArrayUserBar);
            
            glBindTexture(GL_TEXTURE_2D, *endIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+4, 4);
            
            
            glBindTexture(GL_TEXTURE_2D, *arrowLeft);
            glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+8, 4);
            
            glBindTexture(GL_TEXTURE_2D, *arrowRight);
            glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+12, 4);
            
            glBindTexture(GL_TEXTURE_2D, *barTop);
            glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+16, 4);
            
            
            for (int counter=0; counter<propButtonDict.size(); counter++){
                
                if (propButtonDict[counter]->index==Sprite::changeSpriteType){
                    
                    ShaderManager::enableHighLightShaderUI(propButtonDict[counter]->textureWidth);
                    
                    glBindTexture(GL_TEXTURE_2D, *propButtonDict[counter]->texture);
                    glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
                    
                    
                    ShaderManager::enableUIShader();
                    
                } else {
                    ShaderManager::enableHalfTextureShaderUI();
                    glBindTexture(GL_TEXTURE_2D, *propButtonDict[counter]->texture);
                    glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
                    ShaderManager::enableUIShader();
                }
            }
            
             }

            
            if (GameLogic::speedSetting!=0){
                speedDisplay.render();
            }
            
        }
    } else if (GameLogic::gameMode==PROPMODE){
        
        
        ShaderManager::enableUIShader();
        renderModeSelector();
        
        
        if (previousGamemode==GameLogic::gameMode){
            
            

            
            glBindVertexArrayCon(_vertexArrayOption);
            glBindTexture(GL_TEXTURE_2D, *optionButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
             if (BobZombie::bottomBarDraw()){
            
            glBindVertexArrayCon(_vertexArrayUserBar);
            glBindTexture(GL_TEXTURE_2D, *endIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            glBindTexture(GL_TEXTURE_2D, *middleIcons);
            for (int counter=0; counter<numberOfTilesMiddle; counter++){
                
                glBindVertexArrayCon(_vertexArrayUserBar);
                glBindTexture(GL_TEXTURE_2D, *middleIcons);
                glDrawArrays(GL_TRIANGLE_STRIP, 4+counter*4, 4);
                
                costs[counter]->render();
            }
            
            glBindVertexArrayCon(_vertexArrayUserBar);
            
            glBindTexture(GL_TEXTURE_2D, *endIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+4, 4);
            
            
            glBindTexture(GL_TEXTURE_2D, *arrowLeft);
            glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+8, 4);
            
            glBindTexture(GL_TEXTURE_2D, *arrowRight);
            glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+12, 4);
            
            glBindTexture(GL_TEXTURE_2D, *barTop);
            glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+16, 4);
            
            
            for (int counter=0; counter<propButtonDict.size(); counter++){
                
                if (propButtonDict[counter]->index==Sprite::changeSpriteType){
                    
                    ShaderManager::enableHighLightShaderUI(propButtonDict[counter]->textureWidth);
                    
                    glBindTexture(GL_TEXTURE_2D, *propButtonDict[counter]->texture);
                    glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
                    
                    
                    ShaderManager::enableUIShader();
                    
                } else {
                    ShaderManager::enableHalfTextureShaderUI();
                    glBindTexture(GL_TEXTURE_2D, *propButtonDict[counter]->texture);
                    glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
                    ShaderManager::enableUIShader();
                }
            }
            
     
             }
            
            if (GameLogic::speedSetting!=0){
                speedDisplay.render();
            }
            
        }
    } else if (GameLogic::gameMode ==OPENINGMODE){
        ShaderManager::enableUIShader();
        if (previousGamemode==GameLogic::gameMode){
            
            
            
            

            
            
            renderBackground();
            
            
            
            glBindTexture(GL_TEXTURE_2D, *freePlay);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            
            glBindTexture(GL_TEXTURE_2D, *scenerio);
            glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
            
            glBindTexture(GL_TEXTURE_2D, *mods);
            glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
            
 
            
            glBindTexture(GL_TEXTURE_2D, *creditsButton);
            glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
            
            
            if (NOTHING_MODE == menuMode){
                glBindVertexArrayCon(_vertexArrayOption);
                glBindTexture(GL_TEXTURE_2D, *optionButtonTexture);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            }

            
            if (GAME_MODE==menuMode){
                glBindVertexArrayCon(_vertexArrayOption);
                glBindTexture(GL_TEXTURE_2D, *backButtonTexture);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                
            
                glBindVertexArrayCon(_vertexArray);

				GLfloat color[4] = { 0, 0, 0, 1.0 };
				ShaderManager::enableSolidColorUI(color);

				if (newGame != thumbNailTextures[0]){
					glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
				}
				if (newGame != thumbNailTextures[1]){
					glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
				}
				if (newGame != thumbNailTextures[2]){
					glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);
				}


                
                if (currentSlotRunning==0){
                
                    ShaderManager::enableUIShader();
					
                    glBindTexture(GL_TEXTURE_2D, *thumbNailTextures[0]);
                    glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
                    
                    
                } else {
                
                    ShaderManager::deselectShaderUI();
                    glBindTexture(GL_TEXTURE_2D, *thumbNailTextures[0]);
                    glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
                
                }
                
            
                if (currentSlotRunning==1){
                    ShaderManager::enableUIShader();
                    glBindTexture(GL_TEXTURE_2D, *thumbNailTextures[1]);
                    glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
                    
                } else {
                
                    ShaderManager::deselectShaderUI();
                    glBindTexture(GL_TEXTURE_2D, *thumbNailTextures[1]);
                    glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
                
                }
            
                if (currentSlotRunning==2){
                    ShaderManager::enableUIShader();
                    glBindTexture(GL_TEXTURE_2D, *thumbNailTextures[2]);
                    glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);
     
                } else {
                    ShaderManager::deselectShaderUI();
                    
                    glBindTexture(GL_TEXTURE_2D, *thumbNailTextures[2]);
                    glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);
                
                }
                
                ShaderManager::enableUIShader();
                
                if (currentSlotRunning>=0){
                    
                    
                    
                    glBindVertexArrayCon(_playDeleteButtonArray);
                    glBindTexture(GL_TEXTURE_2D, *playButton);
                    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                    glBindTexture(GL_TEXTURE_2D, *deleteButton);
                    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
                }
                
                
                
                
            }
            
            if (MOD_MODE == menuMode){
                
                glBindVertexArrayCon(_vertexArray);
            glBindTexture(GL_TEXTURE_2D, *UserInterface::arrowLeft);

                glDrawArrays(GL_TRIANGLE_STRIP, 40, 4);
            
                
                for (int counter =0; counter<theCurrentMods.size(); counter++){
            
                    
                        glBindTexture(GL_TEXTURE_2D, theCurrentMods[counter]->loadThumbNail());
                    if (theCurrentMods[counter]->enabled){
                        ShaderManager::enableUIShader();
                    } else {
                        ShaderManager::deselectShaderUI();
                    }
                        glDrawArrays(GL_TRIANGLE_STRIP, 44+(counter*4), 4);
                    
                }
            ShaderManager::enableUIShader();
            glBindTexture(GL_TEXTURE_2D, *UserInterface::arrowRight);
            glDrawArrays(GL_TRIANGLE_STRIP, 64, 4);
                
                //modName.render();
                modText.render();
                modText.renderText();
                
                glBindVertexArrayCon(_vertexArrayOption);
                glBindTexture(GL_TEXTURE_2D, *backButtonTexture);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                
                glBindVertexArrayCon(_playDeleteButtonBuffer);
                glBindTexture(GL_TEXTURE_2D, *deleteButton);
                glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
                glBindTexture(GL_TEXTURE_2D, *modStartButton);
                glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
                
            }
            
        }
        
 
        
        
    } else if (GameLogic::gameMode == PLAYMODE){
        
        ShaderManager::enableUIShader();
        
        
        glBindVertexArrayCon(_vertexArrayOption);
        glBindTexture(GL_TEXTURE_2D, *optionButtonTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        if (BobZombie::bottomBarDraw()){
        glBindVertexArrayCon(_vertexArrayUserBar);
        glBindTexture(GL_TEXTURE_2D, *endIcons);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glBindTexture(GL_TEXTURE_2D, *middleIcons);
        for (int counter=0; counter<numberOfTilesMiddle; counter++){
            
            glBindVertexArrayCon(_vertexArrayUserBar);
            glBindTexture(GL_TEXTURE_2D, *middleIcons);
            glDrawArrays(GL_TRIANGLE_STRIP, 4+counter*4, 4);
            
            if (counter<playButtonDict.size()){
                costs[counter]->render();
                }
            
            float color [] ={204.0/255.0,131.0/255.0,128.0/255.0,1.0};
            
            ShaderManager::enableSolidColorUI(color);
            glBindVertexArrayCon(_vertexArrayLoadLine);
            glDrawArrays(GL_TRIANGLE_STRIP, counter*4, 4);
             ShaderManager::enableUIShader();
            
        }
        
        glBindVertexArrayCon(_vertexArrayUserBar);
        glBindTexture(GL_TEXTURE_2D, *endIcons);
        glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+4, 4);
        
        
        glBindTexture(GL_TEXTURE_2D, *arrowLeft);
        glDrawArrays(GL_TRIANGLE_STRIP, numberOfTilesMiddle*4+8, 4);
        
        glBindTexture(GL_TEXTURE_2D, *arrowRight);
        glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+12, 4);
        
        glBindTexture(GL_TEXTURE_2D, *barTop);
        glDrawArrays(GL_TRIANGLE_STRIP, 2*numberOfTilesMiddle*4+16, 4);
        
       
        
        for (int counter=0; counter<playButtonDict.size(); counter++){
            
            //glBindTexture(GL_TEXTURE_2D, GDebug::value3);
            glBindTexture(GL_TEXTURE_2D, *playButtonDict[counter].texture);
            glDrawArrays(GL_TRIANGLE_STRIP, (numberOfTilesMiddle*4+12)+counter*4, 4);
            
          
        }

        
        
        
        }
        
        renderModeSelector();
        
        if (GameLogic::speedSetting!=0){
            speedDisplay.render();
        }
        
        if (previousGamemode==GameLogic::gameMode){
            
            glBindVertexArrayCon(_vertexArrayOption);
            glBindTexture(GL_TEXTURE_2D, *optionButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            

      
            

            
            
        }

    
    
    
    }
    
    if (SAVE_OR_NOT_MODE == menuMode){
        if (GameLogic::actPlay()){
            
            
            glBindVertexArrayCon(_vertexArraySaveGame);
            glBindTexture(GL_TEXTURE_2D, *saveGameTexture);
            
            if (highLightSaveGame!=0){
                ShaderManager::deselectShaderUI();
            } else {
                ShaderManager::enableUIShader();
            }
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            

            
            glBindTexture(GL_TEXTURE_2D, *noSaveGameTexture);
            if (highLightSaveGame!=1){
                ShaderManager::deselectShaderUI();
            } else {
                ShaderManager::enableUIShader();
            }
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
            
            
            ShaderManager::enableUIShader();
            
            UserInterface::renderSound();
            
        } else {
        
            ShaderManager::enableUIShader();
            
            UserInterface::renderSound();
            
            glBindVertexArrayCon(_vertexArrayOption);
            glBindTexture(GL_TEXTURE_2D, *backButtonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        
        }
    }
    



}


void UserInterface::loadThumbNails(){

    for (int counter = 0; counter<3; counter++){
        if (!loadedSaves[counter]){
            
            loadedSaves[counter] = new SaveObject;
            std::string target;
            if (counter==0){
                target = "SaveGameA.map";
            } else if (counter==1){
                target = "SaveGameB.map";
            } else if (counter==2){
                target = "SaveGameC.map";
            }
            
            if (loadedSaves[counter]->load(target)){
                
                
            } else {
                
                delete loadedSaves[counter];
                loadedSaves[counter]=0;
                
            }
        }
    }
    
}

int UserInterface::modeSelector(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
    
        
        if (theTouchNumber == MOUSE_L){
            
            
            for (int counter = 0 ;counter<6; counter++){
                
                if (cpBBContainsVect(changeModeButtons[counter], control)){
                    
                    return counter;
                }
            }
        }
    
    return -1;
}

std::map<std::string, cpBB> overTest;

void UserInterface::createCircle(){
    
    if (checkCircleRadius==-1){
    
        
        if (!Sprite::overSpriteForRadius){
            Tiles::hoverTileSelected.clear();
        }
    }else{

        Player::scareAbilityDrawRadiusButton(checkCircleRadius);
    }
}


void UserInterface::cleanUpInterfaceErrors(){

    Tiles::startIndex = -1;
    UserInterface::spawnTextBox("",TEXTBOX_STAY_UP);

}


bool UserInterface::mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
    
    
 
    
    
    
    // If the game is over disable all input
    if (GameLogic::gameIsOver){
    
        
        if (timer<90){
        
            return true;
        
        }
        
        else {
        
            if (input == MOUSE_STATE_UP){
            
            
                GameLogic::gameIsOver = false;
                
                GameLogic::changeGameMode(OPENINGMODE);
            
            }
        
        
        }
    
        
    
    }
    
    // Stop Tiles from being stupid.
    if (input ==MOUSE_STATE_UP){
        if (Tiles::lastIndex!=-1||Tiles::startIndex!=-1){
            
            int xPos1 = Tiles::lastIndex%Tiles::width;
            int xPos2 = Tiles::startIndex%Tiles::width;
            
            int yPos1 = Tiles::lastIndex/Tiles::width;
            int yPos2 = Tiles::startIndex/Tiles::width;
            
            if (abs(xPos1-xPos2)+abs(yPos1-yPos2)>9){
            
                return false;
                
            }
            
            
            
    
        }
    }
    
    
    
    // Change between modes
    
    if (theTouchNumber == MOUSE_L){
        
        if (input==MOUSE_STATE_UP){
            switch (modeSelector(input, control, theTouchNumber)) {
                case 0:
                    GameLogic::changeGameMode(PLAYMODE);
                    playSelect();
                    return true;
                    break;
                case 1:
                    GameLogic::changeGameMode(TRAPMODE);
                    playSelect();
                    return true;
                    break;
                case 2:
                    GameLogic::changeGameMode(PROPMODE);
                    playSelect();
                    return true;
                    break;
                case 3:
                    GameLogic::changeGameMode(TILEMODE);
                    playSelect();
                    return true;
                    break;
                case 4:
                    GameLogic::changeGameMode(DOORMODE);
                    playSelect();
                    return true;
                    break;
                    
                case 5:
                    if (GDebug::debuggingActive){
                        GameLogic::changeGameMode(SPRITEMODE);
                        playSelect();
                        return true;
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    // Test if you'll save game
    if (testOptionSaveGame(input, control, theTouchNumber)){
        return true;
    }
    
    
    if (true){
    
        if (dialog.size()>0){
            if (input==MOUSE_STATE_UP){
                if (cpBBContainsVect(removeTheText, control)){
                
                    UserInterface::despawnTextBox();
                    return true;
                }
            
            }
            
        
        }
    }
    
    
    if (GameLogic::gameMode==TILEMODE){
        
        if(theTouchNumber==MOUSE_L){
            if (input==MOUSE_STATE_UP){
                if(testOptionButton(input, control, theTouchNumber)){
                    return true;
                }
                
                for (std::vector<cpBB>::iterator IT = buttons.begin();
                     IT !=buttons.end();
                     IT++){
                    
                    if (cpBBContainsVect(*IT, control)){
                        size_t counter = IT - buttons.begin();
                        
                        if (counter==0){
                            playSelect();
                            slideLeft();
                            cleanUpInterfaceErrors();
                            return true;
                            
                        }
                        if (counter==buttons.size()-1){
                            playSelect();
                            slideRight();
                            cleanUpInterfaceErrors();
                            return true;
                        }
                        
                        if (counter-1<tileButtonDict.size()){
                            
                            
                            Tiles::changeTileType = tileButtonDict[counter-1]->index;
                            cleanUpInterfaceErrors();
                            playSelect();
                        }
                        
                        
                    }
                }
                
                if (cpBBContainsVect(theBarArea,control)){
                    return true;
                }
            }
        }
        //Note this is only for Debugging
    } else if (GameLogic::gameMode==SPRITEMODE){

	

        
        if(theTouchNumber==MOUSE_L){
            if (input==MOUSE_STATE_UP){
                if(testOptionButton(input, control, theTouchNumber)){
                    return true;
                }
                
              
                for (std::vector<cpBB>::iterator IT = buttons.begin();
                     IT !=buttons.end();
                     IT++){
                    
                    if (cpBBContainsVect(*IT, control)){
                        size_t counter = IT - buttons.begin();
                        
                        if (counter==0){
                            slideLeft();
                            playSelect();
                            return true;
                            
                        }
                        if (counter==buttons.size()-1){
                            slideRight();
                            playSelect();
                            return true;
                        }
                        if (counter-1<spriteButtonDict.size()){
                            
                            
                            
                            if (Sprite::changeSpriteType==spriteButtonDict[counter-1]->index){
                            
                                playSelect();
                                Sprite::changeSelect(-1);
                            
                            } else {
                            
                                playSelect();
                            Sprite::changeSelect(spriteButtonDict[counter-1]->index);
                            
                            }
                        }
                    }
                }
            }
        }
        
        if (input==MOUSE_STATE_TOUCH||input==MOUSE_STATE_UP){
            if (cpBBContainsVect(theBarArea,control)){
                return true;
            }
        }
    }
    else if (GameLogic::gameMode==TRAPMODE){
        if(theTouchNumber==MOUSE_L){
            if (input==MOUSE_STATE_UP){
                if(testOptionButton(input, control, theTouchNumber)){
                    return true;
                }
                
                
            }
        }
        
            
        if (input==MOUSE_STATE_UP){
            for (std::vector<cpBB>::iterator IT = buttons.begin();
                 IT !=buttons.end();
                 IT++){
                
                if (cpBBContainsVect(*IT, control)){
                    size_t counter = IT - buttons.begin();
                    
                    if (counter==0){

                        playSelect();
                        slideLeft();
                        return true;
                        
                    }
                    if (counter==buttons.size()-1){
                        playSelect();
                        slideRight();
                        return true;
                    }
                    
                    if (counter-1<propButtonDict.size()){
                        if(MOUSE_L==theTouchNumber){
                        playSelect();
                        Sprite::changeSelect(propButtonDict[counter-1]->index);
                        } else if (MOUSE_R==theTouchNumber) {
                        
                            BobZombie::setSprite(propButtonDict[counter-1]);
                            BobZombie::toggleState();
                        }
                    }
                }
            }
        }
        
        
        if (input==MOUSE_STATE_TOUCH||input==MOUSE_STATE_UP){
            if (cpBBContainsVect(theBarArea,control)){
                return true;
            }
        }
        
    }
    else if (GameLogic::gameMode==PROPMODE){

		if (
			moveSpriteInput(input, control, theTouchNumber)
		){
			return true;
		}

        if(theTouchNumber==MOUSE_L){
            if (input==MOUSE_STATE_UP){
                if(testOptionButton(input, control, theTouchNumber)){
                    
                    return true;
                }
         
            }
        }
        
        if(theTouchNumber==MOUSE_L){
            if (input==MOUSE_STATE_UP){
                for (std::vector<cpBB>::iterator IT = buttons.begin();
                     IT !=buttons.end();
                     IT++){
                    
                    if (cpBBContainsVect(*IT, control)){
                        size_t counter = IT - buttons.begin();
                        
                        if (counter==0){
                            slideLeft();
                            playSelect();
                            return true;
                            
                        }
                        if (counter==buttons.size()-1){
                            slideRight();
                            playSelect();
                            return true;
                        }
                        
                        if (counter-1<propButtonDict.size()){
                            Sprite::changeSelect(propButtonDict[counter-1]->index);
                            playSelect();
                        }
                        
                        //Sprite::changeSpriteType=propButtonDict[counter-1]->index;
                        
                        
                    }
                }
            }
        }
        
        if (input==MOUSE_STATE_TOUCH||input==MOUSE_STATE_UP){
            if (cpBBContainsVect(theBarArea,control)){
                return true;
            }
        }
        
    }
    else if (GameLogic::gameMode==DOORMODE){
        
        if(theTouchNumber==MOUSE_L){
            if (input==MOUSE_STATE_UP){
                if(testOptionButton(input, control, theTouchNumber)){
                    return true;
                }
                
             
            }
        }
        
        if(theTouchNumber==MOUSE_L){
            if (input==MOUSE_STATE_UP){
                for (std::vector<cpBB>::iterator IT = buttons.begin();
                     IT !=buttons.end();
                     IT++){
                    
                    if (cpBBContainsVect(*IT, control)){
                        size_t counter = IT - buttons.begin();
                        
                        if (counter==0){
                            slideLeft();
                            playSelect();
                            return true;
                            
                        }
                        if (counter==doorButtonDict.size()+1){
                            slideRight();
                            playSelect();
                            return true;
                        }
                        
                        if (counter-1<doorButtonDict.size()){
                            Door::changeSelect(doorButtonDict[counter-1]->index);
                            playSelect();
                            
                        }
                    }
                }
            }
        }
        
        if (input==MOUSE_STATE_TOUCH||input==MOUSE_STATE_UP){
            if (cpBBContainsVect(theBarArea,control)){
                return true;
            }
        }

        
        
    
    
    }
    else if (GameLogic::gameMode==OPENINGMODE){




		if (menuMode == MOD_MODE){
			if (input == MOUSE_STATE_HOVER){

				if (cpBBContainsVect(modButton[1], control)){
					if (theCurrentMods.size()>0){
						modText.text = theCurrentMods[0]->getDescription();
						//modText.text = theCurrentMods[0]->getName();
					}
					return true;
				}

				if (cpBBContainsVect(modButton[2], control)){
					if (theCurrentMods.size()>1){
						modText.text = theCurrentMods[1]->getDescription();
						//modText.text = theCurrentMods[1]->getName();
					}
				}

				if (cpBBContainsVect(modButton[3], control)){
					if (theCurrentMods.size()>2){
						modText.text = theCurrentMods[2]->getDescription();
						//modText.text = theCurrentMods[2]->getName();
					}
					return true;
				}

				if (cpBBContainsVect(modButton[4], control)){
					if (theCurrentMods.size()>3){
						modText.text = theCurrentMods[3]->getDescription();
						//modText.text = theCurrentMods[3]->getName();
					}
					return true;
				}

				if (cpBBContainsVect(modButton[5], control)){
					if (theCurrentMods.size()>4){
						modText.text = theCurrentMods[4]->getDescription();
						//modText.text = theCurrentMods[4]->getName();
					}
					return true;
				}



			}
		}





        if(theTouchNumber==MOUSE_L){
            if (input==MOUSE_STATE_UP){
                if (menuMode==MOD_MODE){
                    
                    
                    if (cpBBContainsVect(optionButton, control)){
                        menuMode = NOTHING_MODE;
                        //UserInterface::timer=1;
                        AudioController::playAudio("Interface_Wrong.wav");
                        return true;
                    }
                    
                    if (cpBBContainsVect(modButton[0], control)){
                        if (index_value_mod!=0){
                            index_value_mod--;
                        }
                        theCurrentMods = ModManager::dumpElements(index_value_mod);
                        return true;
                    }
                    
                    if (cpBBContainsVect(modButton[1], control)){
                        if (theCurrentMods.size()>0){
                            theCurrentMods[0]->click();
                        }
                        return true;
                    }
                    
                    if (cpBBContainsVect(modButton[2], control)){
                        if (theCurrentMods.size()>1){
                            theCurrentMods[1]->click();
                        }
                    }
                    
                    if (cpBBContainsVect(modButton[3], control)){
                        if (theCurrentMods.size()>2){
                            theCurrentMods[2]->click();
                        }
                        return true;
                    }
                    
                    if (cpBBContainsVect(modButton[4], control)){
                        if (theCurrentMods.size()>3){
                            theCurrentMods[3]->click();
                        }
                        return true;
                    }
                    
                    if (cpBBContainsVect(modButton[5], control)){
                        if (theCurrentMods.size()>4){
                            theCurrentMods[4]->click();
                        }
                        return true;
                    }
                    
                    if (cpBBContainsVect(modButton[6], control)){
                        index_value_mod++;
                        theCurrentMods = ModManager::dumpElements(index_value_mod);
                        return true;
                    }
                    
                    if (cpBBContainsVect(modButtonLocation, control)){
            
                        menuMode = NOTHING_MODE;
                        ModManager::loadAllMods();
                        AudioController::playAudio("Interface_WellDone.wav");
                        return true;
                    }
                    
                    
                    if (cpBBContainsVect(deleteButtonLocation, control)){
                        ModManager::removeAllMods();
                        AudioController::playAudio("Interface_Wrong.wav");
                        return true;
                    }
                    
                    
                }
                if (menuMode==GAME_MODE){
                    
                    if (cpBBContainsVect(optionButton, control)){
                        changeMenuMode(NOTHING_MODE);
                        AudioController::playAudio("Interface_Wrong.wav");
            
                        //UserInterface::timer=1;
                        return true;
                    }
                    
                    
                    if (currentSlotRunning>=0){
                    
                        if (cpBBContainsVect(playButtonLocation, control)){
                        
                            AudioController::playAudio("Interface_WellDone.wav");
                            
                           
                            if (loadedSaves[currentSlotRunning]){
                                loadedSaves[currentSlotRunning]->activate();
                            } else {
								GameLogic::timePassed = 0;
                                GameLogic::purge(true);
                                BobZombie::triggerTutorial();
                                
                            }
                            GameLogic::gameMode = PLAYMODE;
                            changeMenuMode(NOTHING_MODE);
                            
                            
                        }
                        
                        if (cpBBContainsVect(deleteButtonLocation, control)){
                        
                            AudioController::playAudio("Interface_Wrong.wav");
                            
                            if (loadedSaves[currentSlotRunning]){
                                loadedSaves[currentSlotRunning]->clear();
                                
                            }
                            
                            SaveObject * theObject = loadedSaves[currentSlotRunning];
                            delete theObject;
                            loadedSaves[currentSlotRunning] = 0;
                            
            
                            
                            
                            GLuint * textureTarget = thumbNailTextures[currentSlotRunning];
                            
                            if (textureTarget!=newGame){
                                glDeleteTextures(1, textureTarget);
                                delete textureTarget;
                            }
                            
                            thumbNailTextures[currentSlotRunning] = newGame;
                            currentSlotRunning = -1;

                            
                            
                            
                            
                            
                        
                        
                        }
                    
                    
                    }
                    
                    
       
                    
                    
                    
                    if ( cpBBContainsVect(gameA, control)){
                  
                        currentSlotRunning = 0;
            
                        AudioController::playAudio("Interface_Select.wav");
                        return true;
                    }
                    
                    if ( cpBBContainsVect(gameB, control)){
                    
                        currentSlotRunning = 1;
                     
                        AudioController::playAudio("Interface_Select.wav");
                        return true;
                    }
                    
                    if ( cpBBContainsVect(gameC, control)){
                        
                        currentSlotRunning = 2;
                        
                        AudioController::playAudio("Interface_Select.wav");

                        return true;
                    }
                    
                }
                
                if (menuMode!=GAME_MODE&&menuMode!=MOD_MODE){
                for (std::vector<cpBB>::iterator IT = buttons.begin();
                     IT !=buttons.end();
                     IT++){
                    
                    if (input==MOUSE_STATE_UP){
                        if(testOptionButton(input, control, theTouchNumber)){
                            return true;
                        }
                        
                        
                    }
                    
                    
                    if (cpBBContainsVect(*IT, control)){
                        size_t counter = IT - buttons.begin();
                        
                        if (counter==0){
                            
                             AudioController::playAudio("Interface_Select.wav");
                            UserInterface::timer=1;
                            changeMenuMode(GAME_MODE);
                    
                            
                            UserInterface::loadThumbNails();
                            GameLogic::update();
                            return true;
                           
                            
                        
                        }else if (counter==1){
                             AudioController::playAudio("Interface_Cancel.wav");
                            GameLogic::quit();
                        } else if (counter==2){
                        //MOD MODE DISABLED
                             AudioController::playAudio("Interface_Select.wav");
                            
                            UserInterface::timer=1;
                            changeMenuMode(MOD_MODE);
                            
                            ModManager::populate();
                            index_value_mod=0;
                            theCurrentMods = ModManager::dumpElements(index_value_mod);
                           GameLogic::update();
                            return true;
                         
                        
                        } else if (counter==3){
                        
                            AudioController::playAudio("Interface_Select.wav");
                            GameLogic::changeGameMode(CREDITTEXTMODE);
                        } else if (counter==4){
                        
                            
                        }
                        
                    }
                    
                    }
                     return true;
                }
            }
        }
    }
    else if (GameLogic::gameMode==PLAYMODE){

		if (input == MOUSE_STATE_HOVER){
            bool overIcon = false;
			for (std::vector<cpBB>::iterator IT = buttons.begin();
				IT != buttons.end();
				IT++){
                

				if (cpBBContainsVect(*IT, control)){
					size_t counter = IT - buttons.begin();

					if (counter == 0){
						//slideLeft();
						return true;

					}

                    

					if (counter - 1 < playButtonDict.size()){


                        checkCircleRadius= (int)(counter - 1);
						
                        overIcon = true;
                    
                        break;






					}
                    
                  
				}
                
			}
            
            if (!overIcon){
            
                checkCircleRadius=-1;
            
            }
            
            
		}

        if (theTouchNumber==MOUSE_R){
        
            if (input==MOUSE_STATE_UP){
        
                
                for (std::vector<cpBB>::iterator IT = buttons.begin();
                     IT !=buttons.end();
                     IT++){
                    
                    if (cpBBContainsVect(*IT, control)){
                        size_t counter = IT - buttons.begin();
                        
                        if (counter==0){
                            //slideLeft();
                            return true;
                            
                        }
                        
                        
                        if (counter-1<playButtonDict.size()){
                            
                            
                            
                            BobZombie::setText(Player::getDescription(counter-1));
                            BobZombie::toggleState();

							BobZombie::setSprite(0);
						
							BobZombie::currentTutorialState = LEVEL_UP;
							BobZombie::restSlideState();

                            
                        }
                        
                            
                        }
                    }
                }
                
        
        }
        
        
        if(theTouchNumber==MOUSE_L){
            
            if (input==MOUSE_STATE_UP){
                
                
                for (std::vector<cpBB>::iterator IT = buttons.begin();
                     IT !=buttons.end();
                     IT++){
                    
                    if (cpBBContainsVect(*IT, control)){
                        size_t counter = IT - buttons.begin();
                        
                        if (counter==0){
                            //slideLeft();
                            return true;
                            
                        }
  
                        
                        if (counter-1<playButtonDict.size()){
                            
                          

                                Player::scareAbilityButton((int)(counter-1));
                                playSelect();
                                    
                             
                            
                            
                            
                        }
                    }
                }
                
                
                
                
                
                
                
               
                
                
                if(testOptionButton(input, control, theTouchNumber)){
                
                    return true;
                }
          
                
           

                

            }
            
            
            if (input==MOUSE_STATE_TOUCH||input==MOUSE_STATE_UP){
                if (cpBBContainsVect(theBarArea,control)){
                    return true;
                }
            }
            
            
            
        }
        
        
    }
    
    return false;
    
    
    
}


bool UserInterface::testOptionButton(MOUSE_STATES input, cpVect control, long theTouchNumber){
    if (cpBBContainsVect(optionButton, control)){
        
        if (menuMode==SAVE_OR_NOT_MODE){
            changeMenuMode(NOTHING_MODE);
        }else{
            changeMenuMode(SAVE_OR_NOT_MODE);
            UserInterface::timer=1;
            GameLogic::update();
            return true;
            
        }
        return true;
    }
    return false;
}



bool UserInterface::testOptionSaveGame(MOUSE_STATES input, cpVect control, long theTouchNumber){
   
    if (menuMode==SAVE_OR_NOT_MODE){
        
            if (
                UserInterface::mouseInputSound(input, control,theTouchNumber)){
            
                return true;
            }
        
        
        
    if (input==MOUSE_STATE_UP){
    
        if (cpBBContainsVect(_saveGameButton, control)){
            menuMode = NOTHING_MODE;
            UserInterface::timer=1;
            
            AudioController::playAudio("Interface_WellDone.wav");
            
            if (currentSlotRunning==0){
                SaveObject::save("SaveGameA.map");
                SaveObject * target = loadedSaves[0];
                loadedSaves[0] = 0;
                delete target;
                
                
                GLuint * textureTarget = thumbNailTextures[0];
                
                if (textureTarget!=newGame){
                    glDeleteTextures(1, textureTarget);
                    delete textureTarget;
                }
                
                thumbNailTextures[0]=newGame;
                
            } else if (currentSlotRunning==1){
                SaveObject::save("SaveGameB.map");
                SaveObject * target = loadedSaves[1];
                loadedSaves[1] = 0;
                delete target;
                
                GLuint * textureTarget = thumbNailTextures[1];
                
                if (textureTarget!=newGame){
                    glDeleteTextures(1, textureTarget);
                    delete textureTarget;
                }
                
                thumbNailTextures[1]=newGame;
                
            } else if (currentSlotRunning==2){
                SaveObject::save("SaveGameC.map");
                SaveObject * target = loadedSaves[2];
                loadedSaves[2] = 0;
                delete target;
                
                GLuint * textureTarget = thumbNailTextures[2];
                
                if (textureTarget!=newGame){
                    glDeleteTextures(1, textureTarget);
                    delete textureTarget;
                }
                
                thumbNailTextures[2]=newGame;
                
                
            }
            
            
            //GameLogic::changeGameMode(OPENINGMODE);
            //menuMode = NOTHING_MODE;
            
            return true;
        }
        
        
        if (cpBBContainsVect(_noSaveGameButton, control)){
            GameLogic::changeGameMode(OPENINGMODE);
            menuMode = NOTHING_MODE;
            
            UserInterface::timer=1;
            AudioController::playAudio("Interface_Cancel.wav");
			AudioController::playMusicForced(NORMAL);
            GameLogic::update();
            
             
            return true;
            
        }
        
        menuMode = NOTHING_MODE;
        
        UserInterface::timer=1;
        GameLogic::update();
        return true;
        
        
        
    }
    }
    if (menuMode==SAVE_OR_NOT_MODE){
    if (input==MOUSE_STATE_HOVER){
        if (cpBBContainsVect(_saveGameButton, control)){
            highLightSaveGame=0;
            return false;
        }
        
        if (cpBBContainsVect(_noSaveGameButton, control)){
            highLightSaveGame=1;
            return false;
            
        }
        
        highLightSaveGame=-1;
        return false;
    
    
    }
    }
    
    
    
    
    
    
    
    
    return false;
    
}

static cpBB musicSlideBox;
static cpBB effectSlideBox;

static cpBB musicSlideHitbox;
static cpBB effectSlideHitbox;

static GLuint _vertexArray_Sound=0;
static GLuint _vertexBuffer_Sound=0;
static GLuint * _texture_musicButton=0;
static GLuint * _texture_effectButton=0;


static int  selectionDown = -1;

bool UserInterface::mouseInputSound(MOUSE_STATES input, cpVect control, long theTouchNumber){

    if (input == MOUSE_STATE_TOUCH){
        if (cpBBContainsVect(musicSlideHitbox, control)){
            selectionDown =0;
        }
    
        if (cpBBContainsVect(effectSlideHitbox, control)){
            selectionDown =1;
        }
    }
    
    if (input==MOUSE_STATE_UP){
        if (cpBBContainsVect(toggleBobBB, control)){
            GameLogic::toggleBob =!GameLogic::toggleBob;
            return true;
            
        }
        
        if (cpBBContainsVect(toggleFinderUIBB, control)){
            GameLogic::toggleFinderUI =! GameLogic::toggleFinderUI;
            return true;
            
        }
        
        if (cpBBContainsVect(toggleHintsBB, control)){
            GameLogic::toggleHints =! GameLogic::toggleHints;
            return true;
            
        }
    }
    
    
    
    if (input==MOUSE_STATE_UP){
    
        if (selectionDown == -1){
            return false;
        }
        selectionDown = -1;
        return true;
    
    }
    
 
    if (selectionDown ==0){
        if (cpBBContainsVect(cpBBNew(-GameLogic::width, musicSlideBox.b, GameLogic::width, musicSlideBox.t), control)){
            
            GLfloat amount =    (control.x-musicSlideBox.l)/(musicSlideBox.r-musicSlideBox.l);
            
            amount = (amount<0)?0:(amount>1.0)?1.0:amount;
            
            AudioController::setVolume("Music", amount);
            return true;
        }
    }
    
    if (selectionDown == 1){
        if (cpBBContainsVect(cpBBNew(-GameLogic::width,effectSlideBox.b,GameLogic::width,effectSlideBox.t), control)){
            
            GLfloat amount =    (control.x-effectSlideBox.l)/(effectSlideBox.r-effectSlideBox.l);
            
            amount = (amount<0)?0:(amount>1.0)?1.0:amount;
            
            AudioController::setVolume("Effect", amount);
            return true;
        }
    }
    
    

    
    
    return false;
}


void UserInterface::updateSound(){
    
    if (!_texture_musicButton){
        _texture_musicButton  = generateTexture(findFile("MusicButton.png"));
    }
    if (!_texture_effectButton){
        _texture_effectButton = generateTexture(findFile("AudioButton.png"));
    }
    double audioValue = AudioController::getVolume("Effect");
    double musicValue = AudioController::getVolume("Music");
    
    GLfloat left    = -GameLogic::width*0.45;
    GLfloat max = GameLogic::width*0.90;

  

    
    GLfloat amountToMoveA = (UserInterface::timer+0.0)/100.0;
    
    
    amountToMoveA = (amountToMoveA>1.0)?1.0:amountToMoveA;
   
    
    GLfloat tweenOne = Tweener::cubicEaseOut(amountToMoveA, -150, 150, 1.0);
    

	GLfloat height = GameLogic::width*0.05;
	GLfloat width = GameLogic::width*0.05;

	GLfloat top = GameLogic::height - height - height +tweenOne;
    
    
    GLfloat aCX = left +max*audioValue;
    GLfloat bCX = left + max*musicValue;

	GLfloat linespace = height*0.025;

    
    GLfloat bCY = top;
    GLfloat aCY = top-height-height-height;
    
    musicSlideBox = cpBBNew(left, bCY-height, max+left, bCY+height);
    effectSlideBox = cpBBNew(left, aCY-height, max+left, aCY+height);
    
    musicSlideHitbox = cpBBNew(left-width, bCY-height, max+left+width, bCY+height);
    effectSlideHitbox = cpBBNew(left-width, aCY-height, max+left+width, aCY+height);
    
    GLfloat vertex[] = {
        aCX-width, aCY+height,0,0,
        aCX+width, aCY+height,1,0,
        aCX-width, aCY-height,0,1,
        aCX+width, aCY-height,1,1,

        bCX-width, bCY+height,0,0,
        bCX+width, bCY+height,1,0,
        bCX-width, bCY-height,0,1,
        bCX+width, bCY-height,1,1,

		left,		aCY + linespace, 0, 0,
		left + max, aCY + linespace, 0, 0,
		left,		aCY - linespace, 0, 0,
		left + max, aCY - linespace, 0, 0,

		left,		bCY + linespace, 0, 0,
		left + max, bCY + linespace, 0, 0,
		left,		bCY - linespace, 0, 0,
		left + max, bCY - linespace, 0, 0,
        
    
    };

    if (!_vertexArray_Sound){
        glGenVertexArraysCon(1, &_vertexArray_Sound);
    }
    glBindVertexArrayCon(_vertexArray_Sound);
    
    if(!_vertexBuffer_Sound){
        glGenBuffers(1, &_vertexBuffer_Sound);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer_Sound);
    
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertex),
                 vertex,
                 GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    
    toggleHints.setText("Toggle Hints");
    toggleHints.SetCursor(0, aCY-height*3);
   
    
    GLfloat modValue = GameLogic::width*(6*0.01);
    toggleHints.lineWidth = GameLogic::width*2;
    toggleHints.setMod(modValue);
    toggleHints.updateCNewLine();
    
    if (GameLogic::toggleHints){
        toggleHints.color[0]=249.0/255.0;
        toggleHints.color[1]=233.0/255.0;
        toggleHints.color[2]=222.0/255.0;
    } else {
    
        toggleHints.color[0]=0.3;
        toggleHints.color[1]=0.3;
        toggleHints.color[2]=0.3;
    
    }
    
    
    
    
    
    
    
    toggleFinderUI.setText("Toggle Finders");
    toggleFinderUI.SetCursor(0, aCY-height*4);
    
    

    toggleFinderUI.lineWidth = GameLogic::width*2;
    toggleFinderUI.setMod(modValue);
    toggleFinderUI.updateCNewLine();
    
    if (GameLogic::toggleFinderUI){
        toggleFinderUI.color[0]=249.0/255.0;
        toggleFinderUI.color[1]=233.0/255.0;
        toggleFinderUI.color[2]=222.0/255.0;
    } else {
        
        toggleFinderUI.color[0]=0.3;
        toggleFinderUI.color[1]=0.3;
        toggleFinderUI.color[2]=0.3;
        
    }
    
    
    toggleBob.setText("Toggle Help From Bob");
    toggleBob.SetCursor(0, aCY-height*5);
    
    
    toggleBob.lineWidth = GameLogic::width*5;
    toggleBob.setMod(modValue);
    toggleBob.updateCNewLine();
    
    if (GameLogic::toggleBob){
        toggleBob.color[0]=249.0/255.0;
        toggleBob.color[1]=233.0/255.0;
        toggleBob.color[2]=222.0/255.0;
    } else {
        
        toggleBob.color[0]=0.3;
        toggleBob.color[1]=0.3;
        toggleBob.color[2]=0.3;
        
    }
    
    
    toggleHintsBB = cpBBNew(toggleHints.bbL, toggleHints.bbB, toggleHints.bbR, toggleHints.bbT);
    
    toggleFinderUIBB  = cpBBNew(toggleFinderUI.bbL, toggleFinderUI.bbB, toggleFinderUI.bbR, toggleFinderUI.bbT);
    
    toggleBobBB = cpBBNew(toggleBob.bbL, toggleBob.bbB, toggleBob.bbR, toggleBob.bbT);
    
    
    
    
}


void UserInterface::renderSound(){
    
   
    
    if (_vertexArray_Sound!=0){
      
        glActiveTexture(GL_TEXTURE0);
		glBindVertexArrayCon(_vertexArray_Sound);
		GLfloat color[] = { 0, 0, 0, 1 };
		ShaderManager::enableSolidColorUI(color);

		glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        

        glBindTexture(GL_TEXTURE_2D, *_texture_effectButton);
        
		ShaderManager::enableUIShader();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
       glBindTexture(GL_TEXTURE_2D, *_texture_musicButton);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);


        
        toggleHints.renderColorChangeUI();
        toggleBob.renderColorChangeUI();
        toggleFinderUI.renderColorChangeUI();

		
    }

	ShaderManager::enableUIShader();
}



void UserInterface::updateOptionSaveGame(){
    
    
    
    if (!saveGameTexture){
        saveGameTexture = generateTexture(findFile("SaveGame.png"));
    }
    
    if (!noSaveGameTexture){
        noSaveGameTexture = generateTexture(findFile("NoSaveGame.png"));
    }
    
    
    
    
    
    
    
    GLfloat width;
    
    width = GameLogic::width/5.75;
    
    GLfloat halfWidth = width/2.0;
    GLfloat leftMid  = -halfWidth-5;
    GLfloat rightMid = halfWidth+5;
    GLfloat yPos = -GameLogic::height/4.0;
    
    
    GLfloat amountToMoveA = (UserInterface::timer+0.0)/100.0;
    GLfloat amountToMoveB = (UserInterface::timer-7.50)/100.0;
    
    
    amountToMoveA = (amountToMoveA>1.0)?1.0:amountToMoveA;
    amountToMoveB = (amountToMoveB>1.0)?1.0:amountToMoveB;
    
    
    GLfloat tweenLeft = Tweener::cubicEaseOut(amountToMoveA, -150, 150, 1.0);
    GLfloat tweenMid = Tweener::cubicEaseOut(amountToMoveB, -150, 150, 1.0);
    
    std::vector<GLfloat> vertex;
    
    vertex.push_back(leftMid-halfWidth);
    vertex.push_back(halfWidth+tweenLeft+yPos);
    vertex.push_back(0);
    vertex.push_back(0);
    
    vertex.push_back(leftMid-halfWidth);
    vertex.push_back(-halfWidth+tweenLeft+yPos);
    vertex.push_back(0);
    vertex.push_back(1);
    
    vertex.push_back(leftMid+halfWidth);
    vertex.push_back(halfWidth+tweenLeft+yPos);
    vertex.push_back(1);
    vertex.push_back(0);
    
    vertex.push_back(leftMid+halfWidth);
    vertex.push_back(-halfWidth+tweenLeft+yPos);
    vertex.push_back(1);
    vertex.push_back(1);
    
    _saveGameButton = cpBBNew(leftMid-halfWidth, -halfWidth+tweenLeft+yPos, leftMid+halfWidth, halfWidth+tweenLeft+yPos);
    
    //RIGHT
    
    vertex.push_back(rightMid-halfWidth);
    vertex.push_back(halfWidth+tweenMid+yPos);
    vertex.push_back(0);
    vertex.push_back(0);
    
    vertex.push_back(rightMid-halfWidth);
    vertex.push_back(-halfWidth+tweenMid+yPos);
    vertex.push_back(0);
    vertex.push_back(1);
    
    vertex.push_back(rightMid+halfWidth);
    vertex.push_back(halfWidth+tweenMid+yPos);
    vertex.push_back(1);
    vertex.push_back(0);
    
    vertex.push_back(rightMid+halfWidth);
    vertex.push_back(-halfWidth+tweenMid+yPos);
    vertex.push_back(1);
    vertex.push_back(1);
    
    _noSaveGameButton = cpBBNew(rightMid-halfWidth, -halfWidth+tweenMid+yPos, rightMid+halfWidth, halfWidth+tweenMid+yPos);
    
    
    
    if (!_vertexArraySaveGame){
        glGenVertexArraysCon(1, &_vertexArraySaveGame);
    }
    glBindVertexArrayCon(_vertexArraySaveGame);
    
    if(!_vertexBufferSaveGame){
        glGenBuffers(1, &_vertexBufferSaveGame);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferSaveGame);
    
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat)*vertex.size(),
                 &vertex[0],
                 GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    
    
    previousGamemode=GameLogic::gameMode;
    
    

    
    
}


bool UserInterface::gameOverDrawUpdateTest(){
    
    if (GameLogic::gameIsOver){
    
        if (!gameOverTexture){
            gameOverTexture = generateTexture(findFile("Gameover.png"));
        }

		GLfloat left = -GameLogic::width;
		GLfloat right = GameLogic::width;
		GLfloat top = GameLogic::height;
		GLfloat bottom = -GameLogic::height;
        
		if (GameLogic::height > GameLogic::width){
			left = -GameLogic::width;
			right = GameLogic::width;
			top = GameLogic::width;
			bottom = -GameLogic::width;
		
		}
		else {
			left = -GameLogic::height;
			right = GameLogic::height;
			top = GameLogic::height;
			bottom = -GameLogic::height;

		}

        
        GLfloat amountToMoveA = (UserInterface::timer)/100.0;

        
        amountToMoveA = (amountToMoveA>1.0)?1.0:amountToMoveA;
        
        
        GLfloat tweenUp = Tweener::cubicEaseOut(amountToMoveA, -GameLogic::height*2, GameLogic::height*2, 1.0);
        
        top += tweenUp;
        bottom += tweenUp;
        
        GLfloat vertex[] = {
        left, top, 0,0,
        left, bottom, 0,1,
        right, top, 1,0,
        right, bottom,1,1
        
        };
        
        
        if (!_vertexArrayGameOver){
            glGenVertexArraysCon(1, &_vertexArrayGameOver);
        }
        glBindVertexArrayCon(_vertexArrayGameOver);
        
        
        if(!_vertexBufferGameOver){
            glGenBuffers(1, &_vertexBufferGameOver);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferGameOver);
        
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertex),
                     vertex,
                     GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        

    
        return true;

    
    }

    return false;
}


bool UserInterface::gameOverDrawRenderTest(){
    
    if (GameLogic::gameIsOver){
        if (_vertexArrayGameOver){
        
            ShaderManager::enableUIShader();
            glBindVertexArrayCon(_vertexArrayGameOver);
            glBindTexture(GL_TEXTURE_2D, *gameOverTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        
            
            
            
            return true;

        
        }
    
    
    
    }
    
    
    

    
    return false;

}




void UserInterface::bobUpdate(){
    
   
    

    BobZombie::update();
    

}

void UserInterface::bobRender(){

    BobZombie::render();
}


void  UserInterface::loadScreenUpdate(){
    
    

    
    updateBackground();
    BobZombie::updateLoading();

}
void  UserInterface::loadScreenRender(){

    renderBackground();
    BobZombie::renderLoading();
}

void UserInterface::playSelect(){

    AudioController::playAudio("Interface_Select.wav");

}

int UserInterface::testMouse(MOUSE_STATES input, cpVect control, long theTouchNumber){

    
        
        if (input==MOUSE_STATE_UP||input==MOUSE_STATE_HOVER){
            
            
            for (std::vector<cpBB>::iterator IT = buttons.begin();
                 IT !=buttons.end();
                 IT++){
                
                if (cpBBContainsVect(*IT, control)){
                    size_t counter = IT - buttons.begin();
                    
                    if (counter==0){
                        //slideLeft();
                        return -1;
                        
                    }
                    
                    if (GameLogic::gameMode==TRAPMODE)
                    {
                        if (counter-1<propButtonDict.size()){
                            return counter = 1;
                        
                        }
                        
                    
                    
                    } else if (GameLogic::gameMode==PLAYMODE){
                    
                    
                    if (counter-1<playButtonDict.size()){
                        
                        
                        return int( counter -1);
                    }
                    } else if (GameLogic::gameMode==TILEMODE){
                    
                        if (counter-1<tileButtonDict.size()){
                        
                            return int (counter -1);
                        }
                        
                    
                    } else if (GameLogic::gameMode==PROPMODE){
                    
                        if (counter -1 < propButtonDict.size()){
                        
                            return int(counter -1);
                        }
                    
                    }
                        
                        
                        
                        
                        
                    
                }
            }
        }
        
        
        if (input==MOUSE_STATE_TOUCH||input==MOUSE_STATE_UP){
            if (cpBBContainsVect(theBarArea,control)){
                return -1;
            }
        }
        
        
        
    
    return -1;

}
