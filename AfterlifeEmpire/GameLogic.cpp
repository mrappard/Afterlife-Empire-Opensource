//
//  GameMap.cpp
//  ThanatosIpad
//
//  Created by Autobotika on 13-09-20.
//  Copyright (c) 2013 Autobotika. All rights reserved.
//

#include "GameLogic.h"

#include "FrameBuffer.h"
#include "Camera.h"
#include "Tiles.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "ScreenTranslator.h"
#include "GlobalDebugger.h"


#include "Sprite.h"


#include "SaveObject.h"
#include "GameLogic.h"
#include "GameClock.h"
#include "UserInterface.h"




#include "AudioController.h"
#include "Tweener.h"

#include "Player.h"
#include "MovieFullScreen.h"
#include "FileManager.h"
#include "Circle.h"
#include <time.h> 

#if defined(__APPLE__ )
#include <sys/time.h>
#endif

#include "BitmapFontClass.h"

#include "TextureGenerator.h"
#include "Bulldozer.h"

#include "Steam_API_Access.h"

#include "DebugTools.h"

#include "GameClock.h"
#include "TranslationMatrix.h"

#include "BobZombie.h"
#include "UnitFinder.h"
#include "SetupVars.h"




double ModVars::exorcistDrain;
double ModVars::policeDrain;
int ModVars::policeRadius;
int ModVars::renovatorRadius;
int ModVars::zombieRadius;
double ModVars::skepticDrain;
double ModVars::reporterDrain;
int ModVars::coupleRadius;
double ModVars::coupleBoost;

int ModVars::startingEnergy=0;
int ModVars::reputationBuffer=0;
int ModVars::doctorRadius = 0;
int ModVars::gildaDrain =0;

double ModVars::investigatorDrain = 0;
int ModVars::ninjaKillTime = 0;
int ModVars::ninjaRadius = 0;
double ModVars::ninjaRepGain = 0;
double ModVars::reputationDrain=0;
double ModVars::costToAwarenessRatio = 0.1;

double ModVars::ghostHunterDrain = 0;
int ModVars::ghostHunterKillArea=1;
int ModVars::ghostHunterKillTime=600;

bool  ModVars::gamerGateMode=false;


int ModVars::gamerRadius = 2;
int ModVars::vivianRadius = 2;

CBitmapFont loading;


static Player player;
cpVect GameLogic::lastTouchPosition=cpvzero;


bool GameLogic::toggleHints=true;
bool GameLogic::toggleFinderUI = true;
bool GameLogic::toggleBob = true;

double GameLogic::width=0;
double GameLogic::height=0;

double GameLogic::screenWidth =0;
double GameLogic::screenHeight = 0;


GAMEMODETYPE GameLogic::gameMode = CREDITMODE;

bool GameLogic::initialized=false;


double GameLogic::influence = 0;
double GameLogic::energy = 100;

bool GameLogic::mouseInputLeftDown = false;
bool GameLogic::mouseInputRightDown = false;

double GameLogic::awareness = 0;
double GameLogic::awarenessSink = 0;
double GameLogic::drainAmount = 0;
double GameLogic::reputationDrainMod = 0;

std::map<int,reputationStruct> GameLogic::reputationLevels;


static GLuint _texture = 0;
static GLuint _vertexArray = 0;
static GLuint _vertexBuffer = 0;


static cpVect mouseMoveCredit;
static cpVect creditOffset;

bool GameLogic::gameIsOver = false;


int GameLogic::speedSetting = 0;
int GameLogic::count = 0;

int GameLogic::spawnRateNormal = 600;

int GameLogic::numberOfTrapsInPlay = 0;
int GameLogic::numberOfTrapsNeeded = 0;
int GameLogic::numberOfTilesInPlay = 0;
int GameLogic::numberOfTilesNeeded = 0;

double GameLogic::lowAmountEnergy;
double GameLogic::midAmountEnergy;

static GAMEMODETYPE lastGameMode;

bool GameLogic::pause = false;


bool GameLogic::loadingComplete=false;


long GameLogic::timePassed=0;

static bool gameQuit=false;


static bool newRenderScheme = false;

bool GameLogic::mouseLeftSideScreen= false ;
bool GameLogic::mouseRightSideScreen = false;
bool GameLogic::mouseTopSideScreen = false;
bool GameLogic::mouseDownSideScreen = false;



long GameLogic::testIfDrainShouldStart;


void * GameLogic::returnPlayer(){

    return &player;
}

static bool testIfGameMode(){

    switch (GameLogic::gameMode) {
        case TILEMODE:
        case SPRITEMODE:
        case PROPMODE:
        case TRAPMODE:
        case DOORMODE:
        case PLAYMODE:
            return true;
            
        case OPENINGMODE:
        case CREDITMODE:
        case DEBUGMODE:
        case CREDITTEXTMODE:
        case HELPSCREENMODE:
            return false;

    }
}

void GameLogic::changeGameMode(GAMEMODETYPE change){
    gameMode = change;
    UserInterface::uiOffset =0;
    if (Sprite::testSprite){
        Sprite::testSprite->destroy(true);
        Sprite::testSprite=0;
    }
    
    if (_texture){
    
        glDeleteTextures(1, &_texture);
        _texture =0 ;
    }
    
    creditOffset = cpvzero;
    

    Tiles::hoverWallSide=0;
    Tiles::hoverTile=-1;
    Tiles::startIndex=-1;
    Tiles::lastIndex=-1;
    
    UserInterface::checkCircleRadius =  -1;
    Tiles::hoverTileSelected.clear();
    
    Sprite::moveSprite =0;
 
    update();

}

int GameLogic::currentLevel=0;

int GameLogic::getCurrentLevel(){
    return currentLevel;
}

bool GameLogic::actPlay(){
    return (gameMode==TILEMODE||
               gameMode==SPRITEMODE||
               gameMode==TRAPMODE||
               gameMode==PROPMODE||
               gameMode==DOORMODE||
               gameMode==PLAYMODE||
               gameMode==DEBUGMODE
            );
}

bool GameLogic::actuallyThink(){

    return (gameMode==PLAYMODE);
    
    
}

#include "Achievements.h"

void GameLogic::calculateCurrentLevel(){

    std::pair<int,int> info = Sprite::getEnergyUsedOnProps();
    


    
    for (std::map<int,reputationStruct>::reverse_iterator IT=GameLogic::reputationLevels.rbegin();IT!=GameLogic::reputationLevels.rend();IT++){
        
        if (IT->second.numberOfStruct<=info.second)
            if (IT->second.energy<=info.first){
                {
                    if (currentLevel<IT->first){
                        
                        currentLevel=IT->first;
                        BobZombie::levelUp(currentLevel);
                        
                        if (currentLevel==4){
                        
                            Achievement::reachLevel4();
                        
                        }
                        
                        return;
                    }
                }
            }
    }
    
    
    std::map<int,reputationStruct>::iterator IT = GameLogic::reputationLevels.find(currentLevel+1);
    
    
    
    numberOfTrapsInPlay = info.second;
    numberOfTilesInPlay = info.first;

    if (IT!=GameLogic::reputationLevels.end()){
        numberOfTrapsNeeded = IT->second.numberOfStruct;
        numberOfTilesNeeded = IT->second.energy;
        drainAmount = IT->second.energyDrain;
        lowAmountEnergy = IT->second.lowEnergy;
        midAmountEnergy = IT->second.midEnergy;
    } else {
        numberOfTrapsNeeded = 999999;
        numberOfTilesNeeded = 999999;
    }

}


static bool onlyRunOne = false;

#include "SaveMap.h"


void GameLogic::_initVideo(){

	

}

void GameLogic::start(){
    
    
   

    
    
    if (!initialized){
        
        if (onlyRunOne){
            return;
        
        }
        
    

        
       
        
        
        onlyRunOne= true;

        
      

//working
//		

        //New
        if (
        !Sprite::loadDictTextureLoader(0)
            ){
            gameQuit = true;
            return;
        
        }


        srand((unsigned int)time(NULL));
    
   


    
    Camera::setModelMatrix(0, 0, 0);
   
    
    
  
    
    AudioController::init();

        
        AudioController::setVolume("Music", 0.75);
        AudioController::setVolume("Effect", 0.75);

    AudioController::loadAudio("AudioTest.wav", 4, "Effect");
        AudioController::playMusic(NORMAL);
        
  
		initVideo();
        
       
        
 

        
        /*    Tiles::initialize(128, 128);
        player.init();
        
         ShaderManager::enableUIShader();
        cpBodySetPos(player.theBody, Tiles::findCords(std::pair<int, int>(64,13)));
          */
                    
		

        
        
        initialized= true;
        
        
        setEnergyLevel(-1);
    
        // Dumps all the description in the game.
        if (/* DISABLES CODE */ (false)){
        std::cerr << "\n\n\n\n";
        
        for (std::map<int, SpriteDict*>::iterator IT = Sprite::theSpriteAssets.begin();
             IT != Sprite::theSpriteAssets.end();
             IT++){
            
            if (!IT->second->prop){
                
                std::cerr << IT->second->name << "--" << IT->second->bobText << "\n";

                for (std::map<std::string,std::string>::iterator IT2 = IT->second->dialog.begin();
                     IT2 != IT->second->dialog.end();
                     IT2++){
                    std::cerr << IT2->first << "--" << IT2->second << "\n";
                    
                }
                
                std::cerr << "\n\n";
                
            }
        }
        
        std::cerr << "Level up descriptions\n\n";
        
        for (std::map<int,reputationStruct>::iterator IT = reputationLevels.begin();
             IT != reputationLevels.end();
             IT++) {
        
            std::cerr << IT->first << " -- "<< IT->second.levelUpText << "\n\n";
        }
        
        std::cerr << "Tutorial Description\n\n";
        
        for (std::map<std::string,std::string>::iterator IT = TranslationMatrix::phrases.begin();
             IT != TranslationMatrix::phrases.end();
             IT++) {
            
            std::cerr << IT->first <<":"<<IT->second << "\n\n\n";
        }
            
        std::cerr << "The Scare Description\n\n";
        
            
            for (std::vector<ScareDict>::iterator IT = Player::scareDict.begin();
                 IT !=  Player::scareDict.end();
                 IT++) {
                
                std::cerr << IT->name <<":"<<IT->bobText << "\n\n\n";
            }
            
    
            
        
        std::cerr << "\n\n\n\n";
        }
    
        initialized=true;

        
    }
    
 

}


void GameLogic::setWindow(int widthI, int heightI){
    
    if (widthI>heightI){
        
        width=100;
        height = ((double)heightI/(double)widthI) *100;
        
    } else {
    
        height=100;
        width = ((double)widthI/(double)heightI)*100;
        
    
    }
    

    screenWidth =widthI;
    screenHeight = heightI;
    
  
    
    Camera::setWindow( screenWidth, screenHeight, -width, width, -height, height, 1, -1);
    update();
    

    

}


void GameLogic::zeroView(){

    //setCamera(cpvzero);

}


void GameLogic::drainEnergyTick(){
    
    if (BobZombie::currentTutorialState==NO_TUTORIAL){
    
    if (testIfDrainShouldStart<GameClock::time-30*60){
        GameLogic::energy -=GameLogic::drainAmount;
    
        GameLogic::awareness -= GameLogic::awarenessSink*reputationDrainMod;
    
    }
    
    
        

 
    if (GameLogic::awareness<0){
        GameLogic::awareness =0;
    }
        
    }
    
    
    if (BobZombie::currentTutorialState == NO_TUTORIAL){
        
        if (GameLogic::energy<100){
            
            AudioController::playMusic(TERROR1);
            
        } else if (GameLogic::awareness>GameLogic::awarenessSink*2){
            
            AudioController::playMusic(TERROR3);
            
        } else if (GameLogic::awareness>GameLogic::awarenessSink){
            
            AudioController::playMusic(TERROR2);
            
        } else {
            
            AudioController::playMusic(NORMAL);
            
        }
    }
    
    
    
}

double timeValue;

bool testAudio = false;

void GameLogic::tick(long tick){
   
    /*if (
    !GameClock::tCheck()
        ){
        return;
    
    }
     */
    
	if (false){
		if (actPlay()){
		if (GameLogic::timePassed > 60 * 60*15){

			BobZombie::demoEnd();

		}
	}
	}



    
    
    BobZombie::think();
    BobZombie::bobWaitTime();
    AudioController::thinkAllBuffers();
    UnitFinder::thinkAll();
    if (tick%100==0){
        calculateCurrentLevel();
    }
    
    if (actPlay()){
        
        
        if (!mouseInputRightDown){
            
            cpVect moveAmount = cpvzero;
            
            if (mouseLeftSideScreen){
                moveAmount.x+=1;
            }
            
            if (mouseRightSideScreen){
                moveAmount.x-=1;
            }
            
            if (mouseDownSideScreen){
                moveAmount.y+=1;
            }
            
            if (mouseTopSideScreen){
                moveAmount.y-=1;
            }
            
            
            moveCameraSafe(moveAmount);
        }
        
        
		GameLogic::timePassed++;
        
        if (GameLogic::gameIsOver||!actuallyThink()||GameLogic::pause||BobZombie::canThink()){
          UserInterface::think();
            return;
        
        }
        
    
        
        UserInterface::think();
        UserInterface::createCircle();
        Circle::tickAll();
        
        
        if (GameLogic::speedSetting>0){
        
            for (int counter = 0; counter<GameLogic::speedSetting; counter++){
        
                GameClock::time++;
                Sprite::thinkAll();
                UserInterface::dialogThink();
                player.think();
                Door::thinkAll();
                Bulldozer::thinkAll();
                drainEnergyTick();
            
            }
            
            
        
        } else if (GameLogic::speedSetting==0){
        
        
            GameClock::time++;
            Sprite::thinkAll();
            UserInterface::dialogThink();
            player.think();
            Door::thinkAll();
            Bulldozer::thinkAll();
            drainEnergyTick();
         
        
        }else {
         
            if (count<=speedSetting){
            
                GameClock::time++;
                Sprite::thinkAll();
                UserInterface::dialogThink();
                player.think();
                Door::thinkAll();
                Bulldozer::thinkAll();
                drainEnergyTick();

                count=0;
            
            }
            count--;
           
            
        }
        
        
       
        
        
      
        if (GameLogic::energy<0&&!GameLogic::gameIsOver){
        
            Sprite::gameOver();
        
        }
        
        
    
    } else if (gameMode==OPENINGMODE){
        UserInterface::think();
    
    }
    
}

static GLuint * backgroundTexture = 0;
static GLuint _vertexArrayBackground = 0;
static GLuint _vertexBufferBackground = 0;



void GameLogic::drawBackground(){

    if (!backgroundTexture){
        
        backgroundTexture = generateTextureR(findFile("BackgroundGame.png"));
        
    }
    
    if (!_vertexArrayBackground){
    
    GLfloat vertex [] = {
    
        -300.0,  -300.0,   0,      6.0,
        -300.0,   300.0,   0,      0,
         300.0,  -300.0,  5.0,   6.0,
         300.0,   300.0,  5.0,   0,
    
    
    };
    
    if (!_vertexArrayBackground){
        glGenVertexArraysCon(1, &_vertexArrayBackground);
    }
    glBindVertexArrayCon(_vertexArrayBackground);
    
    if(!_vertexBufferBackground){
        glGenBuffers(1, &_vertexBufferBackground);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferBackground);
    
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertex),
                 vertex,
                 GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    }
    
    ShaderManager::enableUIShader();

    glBindVertexArrayCon(_vertexArrayBackground);
    glBindTexture(GL_TEXTURE_2D, *backgroundTexture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


}


double frameRateTest;









void GameLogic::update(){
    
 
    

 
    ShaderManager::currentSelect=NOSHADER;
   
    /*
    if (
        !GameClock::uCheck()
        ){
        return;
        
    }
    */
    

    if (gameQuit){
        return;
    }

    
    
   


    if (!initialized){
   
        return;
    }
    

    
    

    if (!Sprite::loadAllTextureTimed()){
        UserInterface::loadScreenUpdate();
        return;
    }
   
    
  GameLogic::loadingComplete= true;
    
 
    
    

    if (gameMode==CREDITMODE) {

        if(updateVideo()){
            return;
        }else{
            gameMode=OPENINGMODE;
        
        }
        
    }
    
    if (gameMode==CREDITTEXTMODE){
    
        
       
        GLfloat xValue = width;
        GLfloat yValue = width*(3005.0/1024.0);
        
    
        
        
        if (!_texture){
            int heldValues[10];
            _texture = dontStore(findFile("CreditScreen.png"),heldValues);
        }
        
        GLfloat offsetY = creditOffset.y;
        offsetY = (offsetY<0)?0:offsetY*5.0;
        
        offsetY = (offsetY>+yValue+yValue-height*2.0)?+yValue+yValue-height*2.0:offsetY;
        
        
        
        
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)(-xValue),
            (GLfloat)(height+offsetY),
            0,  0,
            
            
            (GLfloat)(-xValue),
            (GLfloat)(height-yValue-yValue+offsetY),
            0,  1,
            
            (GLfloat)(xValue),
            (GLfloat)(height+offsetY),
            1,  0,
            
            (GLfloat)(xValue),
            (GLfloat)(height-yValue-yValue+offsetY),
            1,  1
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
        
        
        
        
        
        return;
    }
    
    
    if (gameMode==CREDITTEXTMODE){
        
        
        
        GLfloat xValue = width;
        GLfloat yValue = width*(3005.0/1024.0);
        
        
        
        
        if (!_texture){
            int heldValues[10];
            _texture = dontStore(findFile("CreditScreen.png"),heldValues);
        }
        
        GLfloat offsetY = creditOffset.y;
        offsetY = (offsetY<0)?0:offsetY*5.0;
        
        offsetY = (offsetY>+yValue+yValue-height*2.0)?+yValue+yValue-height*2.0:offsetY;
        
        
        
        
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)(-xValue),
            (GLfloat)(height+offsetY),
            0,  0,
            
            
            (GLfloat)(-xValue),
            (GLfloat)(height-yValue-yValue+offsetY),
            0,  1,
            
            (GLfloat)(xValue),
            (GLfloat)(height+offsetY),
            1,  0,
            
            (GLfloat)(xValue),
            (GLfloat)(height-yValue-yValue+offsetY),
            1,  1
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
        
        
        
        
        
        return;
    }
    
    
    if (gameMode==HELPSCREENMODE){
    
        
        GLfloat xValue = width;
        GLfloat yValue = width*(2);
        
        
        
        
        if (!_texture){
            int heldValues[10];
            _texture = dontStore(findFile("HelpScreenScroll.png"),heldValues);
        }
        
        GLfloat offsetY = creditOffset.y;
        offsetY = (offsetY<0)?0:offsetY*5.5;
        
        offsetY = (offsetY>+yValue+yValue-height*2.0)?+yValue+yValue-height*2.0:offsetY;
        
        
        
        
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)(-xValue),
            (GLfloat)(height+offsetY),
            0,  0,
            
            
            (GLfloat)(-xValue),
            (GLfloat)(height-yValue-yValue+offsetY),
            0,  1,
            
            (GLfloat)(xValue),
            (GLfloat)(height+offsetY),
            1,  0,
            
            (GLfloat)(xValue),
            (GLfloat)(height-yValue-yValue+offsetY),
            1,  1
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
        
        
        
    }
    
    
    
    if (gameMode!=OPENINGMODE){
        
        if (newRenderScheme){
        SaveMap::update();
        }
        
        Tiles::update();
        Tiles::updateSelection();
        Door::updateAll();
        Sprite::updateAll();
        UserInterface::update();
        UserInterface::dialogUpdate();
        player.update();
        Sprite::testSpriteUpdate();
        Circle::updateAll();
        Bulldozer::updateAll();
        
        UserInterface::bobUpdate();
      
        UnitFinder::updateAll();
        
    } else {
        
    UserInterface::update();
    
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    
  
    
    

    
    
    
    
    
#if defined(__APPLE__ )

	/*gettimeofday( &afterTime,NULL);

	double startTimeD = double(startTime.tv_sec)*1000.0;
	startTimeD  += (startTime.tv_usec)/1000.0;

	double endTimeD = double(afterTime.tv_sec)*1000.0;
	endTimeD += (afterTime.tv_usec) / 1000.0;
*/

#endif




};

GLenum returnTypeStencil(int value){
    switch (value){
        case 0:
            return GL_KEEP;
        case 1:
            return GL_ZERO;
        case 2:
            return GL_REPLACE;
        case 3:
            return GL_INCR;
        case 4:
            return GL_INCR_WRAP;
        case 5:
            return GL_DECR;
        case 6:
            return GL_DECR_WRAP;
        case 7:
            return GL_INVERT;
        default:
            return GL_KEEP;
            
            
            
    }
    
    
    return 0;
}



GLuint _vertexArray_m = 0;
GLuint _vertexBuffer_m = 0;

void GameLogic::mouseRender(){
    
    
    cpVect thePos =cpvzero;
    
 
    thePos = cpvadd(thePos, cpv(-5.7, -5.7));
    
    cpVect pointA, pointB;
    
    pointA = SaveMap::findCordsReverse(thePos);
    pointB = SaveMap::findCordsReverse(cpvadd(cpv((11.197*GDebug::value2), (11.197*GDebug::value1)), thePos));
    

    
    GLfloat x = pointA.x;
    GLfloat y = pointA.y;
    
    GLfloat a = pointB.x;
    GLfloat b = pointB.y;
    
    GLfloat size  =1.0;
    
    
    
    GLfloat gBoxVertexData[] = {
        
        (GLfloat)x,
        (GLfloat)y,
       
        
        
        (GLfloat)(x+0),
        (GLfloat)(y+size),
     
        
        (GLfloat)(x+size),
        (GLfloat)(y+0),
      
        
        (GLfloat)(x+size),
        (GLfloat)(y+size),
        
        
        
        (GLfloat)a,
        (GLfloat)b,
        
        
        
        (GLfloat)(a+0),
        (GLfloat)(b+size),
        
        
        (GLfloat)(a+size),
        (GLfloat)(b+0),
        
        
        (GLfloat)(a+size),
        (GLfloat)(b+size),
        
    };
    
    if (!_vertexArray_m){
        glGenVertexArraysCon(1, &_vertexArray_m);
    }
    glBindVertexArrayCon(_vertexArray_m);
    
    if(!_vertexBuffer_m){
        glGenBuffers(1, &_vertexBuffer_m);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer_m);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gBoxVertexData), gBoxVertexData, GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*2, BUFFER_OFFSET(0));

    
    
    
    
    
    glBindVertexArrayCon(0);
    
    
    GLfloat white[] = {1.0,1.0,1.0,1.0};
    
    ShaderManager::enableSolidColor(white);
    
    glBindVertexArrayCon(_vertexArray_m);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    
   
}


CBitmapFont textGen;




void GameLogic::render(){
    
    if (gameQuit){
        return;
    
    }

    if (!initialized){
         glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        return;
    }
    
    
    
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

     if(!Sprite::loadAllTextureTimed()){
         UserInterface::loadScreenRender();
    
    }
   
    GameLogic::loadingComplete = true;

    
    
    
    if (gameMode ==HELPSCREENMODE){
        
        ShaderManager::enableUIShader();
        
        if (_texture){
            if (_vertexArray){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, _texture);
                glBindVertexArrayCon(_vertexArray);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				return;
            }
        }
        
        
       
    
    
    }


   
    if (gameMode==CREDITMODE){
    
    ShaderManager::enableUIShader();
    renderVideo();
        return;
        
    } else if (gameMode==CREDITTEXTMODE){
        ShaderManager::enableUIShader();
        
        if (_texture){
            if (_vertexArray){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, _texture);
                glBindVertexArrayCon(_vertexArray);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
        }
        
    
        return;
    } else if (gameMode==OPENINGMODE){
    
          UserInterface::render();
        ShaderManager::enableTextureShader();
        
    } else {
    
          drawBackground();
        Camera::setModelMatrix(Camera::posX, Camera::posY, 0);
        
        if (newRenderScheme){
            SaveMap::render();
            return;
        }
        
        Tiles::render();
        
        

        //Sprite::testSpriteRender();
        
        
        if (toggleFinderUI){
            UnitFinder::renderAll();
        }
		ShaderManager::enableTextureShader();
        UserInterface::render();
        
        if (toggleHints){
            UserInterface::dialogRender();
        }
        
       
        ShaderManager::enableTextureShader();
        
        UserInterface::bobRender();
        
    }
    
};

static cpVect mouseDragStartPoint;
static cpVect mouseDragOrigin;


void GameLogic::moveCameraSafe(cpVect moveAmount){
    
    
    
    std::pair<int,int> target = Tiles::findTile(cpv(Camera::posX, Camera::posY));
    
    
    bool noLeft = target.first>0;
    bool noRight = target.first<-123;
    bool noDown = target.second>0;
    bool noUp = target.second<-123;
    
    
    if (moveAmount.x<0){
        if (noRight){
        } else {
            if (noDown){
            } else {
                Camera::posX +=moveAmount.x;
            }
        }
    } else if (moveAmount.x>0){
        if (noLeft){
        } else {
            if (noUp){
            } else {
                Camera::posX+=moveAmount.x;
            }
        }
    }
    
    if (moveAmount.y<0){
        if (noUp){
            
        } else {
            if (noRight){
            } else {
                Camera::posY +=moveAmount.y;
            }
        }
    }else if (moveAmount.y>0){
        if (noDown){
        } else {
            if (noLeft){
            } else {
                Camera::posY +=moveAmount.y;
            }
        }
        
        
    }
    
    
}





void GameLogic::mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
    
    
    
    
    cpVect ui =control;
    control = cpvmult(control, Camera::zoom);
    lastTouchPosition = cpvadd(control,cpv(-Camera::posX,-Camera::posY));
    
  
 
    
    if (
    BobZombie::mouseInput(input, control, theTouchNumber)
        ){
    
        return;
    
    }


    

    cpVect changeValue = cpvsub(mouseMoveCredit, control);
    
    creditOffset = cpvsub(creditOffset, changeValue);
    
    mouseMoveCredit = control;
    
    
  
    
    
    if (theTouchNumber>=0){
        theTouchNumber=MOUSE_L;
    
    }
    
    
    if (theTouchNumber==MOUSE_L){
        if (input==MOUSE_STATE_TOUCH){
            mouseInputLeftDown=true;
        }
        if (input==MOUSE_STATE_UP){
            mouseInputLeftDown=false;
        }
    }
    
    if (theTouchNumber==MOUSE_R){
        if (input==MOUSE_STATE_TOUCH){
            mouseInputRightDown=true;
        }
        if (input==MOUSE_STATE_UP){
            mouseInputRightDown=false;
        }
    }

    
    if (gameMode==CREDITTEXTMODE){
        if (input==MOUSE_STATE_UP){
            changeGameMode(OPENINGMODE);
        
        }
        
    
    
    }
    

    
   
    if (!mouseInputRightDown){
        if (input != MOUSE_STATE_UP){
            
     
            GameLogic::mouseRightSideScreen = false;
            GameLogic::mouseLeftSideScreen = false;
            GameLogic::mouseDownSideScreen = false;
            GameLogic::mouseTopSideScreen = false;

            
            if (ui.x>GameLogic::width*0.95){
                
                GameLogic::mouseRightSideScreen = true;
                
            } else if (ui.x<GameLogic::width*-0.95){
                
                GameLogic::mouseLeftSideScreen = true;
                
            }
            
            if (ui.y<GameLogic::height*-0.95){
                
                GameLogic::mouseDownSideScreen = true;
                
            } else if (ui.y>GameLogic::height*0.95){
                
                GameLogic::mouseTopSideScreen = true;
            }
            
            
            
            
            
        }
        
        
        
        
    }
    
    
    
    if(UserInterface::mouseInput(input, ui, theTouchNumber)){
        return;
    
    }

    
    
    
    
   
    if (theTouchNumber==MOUSE_R){
        if (actPlay()){
            
            if (input == MOUSE_STATE_TOUCH){
                
                mouseDragStartPoint=control;
                mouseDragOrigin = control;
                
            } else if (input == MOUSE_STATE_DRAG){
                
                cpVect moveAmount = cpvsub(control, mouseDragStartPoint);
                
                
                
                
                moveAmount = cpvmult(moveAmount, Camera::zoom);
                
                
                
                
                
                
                moveCameraSafe(moveAmount);
             

                
                mouseDragStartPoint = control;
                
               
          
                
             
                
            } else if (input == MOUSE_STATE_UP){
                
                if (cpvdist(mouseDragOrigin, control)<0.25*0.25){
                    
                    
                    if (gameMode == SPRITEMODE){
                        Sprite::mouseInputDelete(input, control, theTouchNumber);
                    } if (gameMode == PLAYMODE){
                        Sprite::mouseInputFind(input, control, theTouchNumber);
                        
					}
					else if (gameMode == PROPMODE){
					
						if (!Sprite::mouseInputFind(input, control, theTouchNumber)){

							Sprite::mouseInputC(input, control, theTouchNumber);
						}
					
					}
					else {
                        if (!Sprite::mouseInputFind(input, control, theTouchNumber)){
                            
                            Sprite::mouseInputDelete(input, control, theTouchNumber);
                        }
                    }
                }
            }
        }
    }
    
    if (theTouchNumber==MOUSE_L){
        if (gameMode==TILEMODE){
            Tiles::mouseInput(input,control,theTouchNumber);
        } else if (gameMode==SPRITEMODE){
            Sprite::mouseInput(input, control, theTouchNumber);
        } else if (gameMode==TRAPMODE){
            Sprite::mouseInput(input, control, theTouchNumber);
        
        } else if (gameMode==PROPMODE){
            Sprite::mouseInput(input, control, theTouchNumber);
        
        
        } else if (gameMode==PLAYMODE){
        
            Sprite::mouseInputGame(input, control, theTouchNumber);
            player.mouseInput(input, control, theTouchNumber);
            
            
            
        
        } else if(gameMode==DEBUGMODE){
        
            Sprite::mouseDebugGame(input, control, theTouchNumber);
        } else if (gameMode==DOORMODE){
    
            Tiles::doorMouseInput(input, control, theTouchNumber);
            
        }
        
        
        
        
    }
    
    if (theTouchNumber==MOUSE_C){
        if (gameMode==PROPMODE){
            
            Sprite::mouseInputC(input, control, theTouchNumber);
        }
        
    }
    
    
    
    
    
}


void GameLogic::jsonDumpData(std::string * theOutput){
/*
    theOutput->append("{\n");
    theOutput->append("\"Level\":{\n");

  
    
    
    
    
    
    theOutput->append("}\n");
    //theOutput->append("}\n");
*/
}

void GameLogic::purge(bool activeInActive){

    if (activeInActive){
        Sprite::purgeAll();
		Bulldozer::purgeAll();
        Door::removeAllDoors();
        Tiles::purge();
        Tiles::createBlankMap();
        Tiles:: checkWalkAbility();

        currentLevel = 0;
        testIfDrainShouldStart = GameClock::time;
    }
    

    setEnergyLevel(-1);

}

void GameLogic::setEnergyLevel(int level){
    
    if (level ==-1){
     GameLogic::energy = ModVars::startingEnergy;;
        GameLogic::awareness = 0;
        return;
    }
    


}



void GameLogic::clearAllTouches(){



}

void GameLogic::increaseSpeed(){

    if (GameLogic::speedSetting<8){
        
        switch (GameLogic::speedSetting) {
            case 0:
                GameLogic::speedSetting=2;
                break;
            case 2:
                GameLogic::speedSetting=4;
                break;
            case 4:
                GameLogic::speedSetting=8;
                break;
                
            default:
                GameLogic::speedSetting++;
                break;
        }
        
        
        
        
    }
    
    count =0;
    

}
void GameLogic::decreaseSpeed(){
    
    if (GameLogic::speedSetting>-3){
        switch (GameLogic::speedSetting) {
            case 0:
                GameLogic::speedSetting=-1;
                break;
            case 2:
                GameLogic::speedSetting=0;
                break;
            case 4:
                GameLogic::speedSetting=2;
                break;
            case 8:
                GameLogic::speedSetting=4;
                break;
                
            default:
                GameLogic::speedSetting--;
                break;
        }

    }

    count=0;
}




#if defined(__APPLE__ )


#define KEY_F               3


#define KEY_ONE             18
#define KEY_TWO             19
#define KEY_THREE           20
#define KEY_FOUR            21
#define KEY_FIVE            23
#define KEY_SIX             22
#define KEY_SEVEN           26
#define KEY_ZERO            29
#define KEY_SQUARE_OPEN     33
#define KEY_SQUARE_CLOSE    30
#define KEY_SEMICOLAN       41
#define KEY_APOST           39
#define KEY_COMMA           43
#define KEY_PEROID          47
#define KEY_TAB             48
#define KEY_UP_PRESS        126
#define KEY_DOWN_PRESS      125
#define KEY_RIGHT           124
#define KEY_LEFT            123
#define KEY_Q               12
#define KEY_B               11
#define KEY_D               2
#define KEY_P               35
#define KEY_V               9
#define KEY_C               8
#define KEY_X               7

#define KEY_F1              122
#define KEY_F3              99
#define KEY_F5              96
#define KEY_F7              98
#define KEY_F8              100
#define KEY_G               5
#define KEY_U               32
#define KEY_I               34
#define KEY_MINUS           27
#define KEY_PLUS            24
#define KEY_SHIFT           845


#define KEY_S               1
#define KEY_D               2
#define KEY_A               0

#define KEY_ESC             53

#define KEY_SPACE_BAR       49

#define KEY_Q               12
#define KEY_W               13
#define KEY_E               14
#define KEY_R               15
#define KEY_T               17
#define KEY_DELETE          51



#else

#define KEY_ONE             49
#define KEY_TWO             50
#define KEY_THREE           51
#define KEY_FOUR            52
#define KEY_FIVE            53
#define KEY_SIX             54
#define KEY_SEVEN           55
#define KEY_ZERO            48
#define KEY_SQUARE_OPEN     91
#define KEY_SQUARE_CLOSE    93
#define KEY_SEMICOLAN       59
#define KEY_APOST           39
#define KEY_COMMA           44
#define KEY_PEROID          46
#define KEY_UP_PRESS        273
#define KEY_DOWN_PRESS      274
#define KEY_RIGHT           275
#define KEY_LEFT            276
#define KEY_Q               113
#define KEY_B               98
#define KEY_D               100
#define KEY_P               112
#define KEY_V               118
#define KEY_C               99
#define KEY_X               120
#define KEY_F5              286
#define KEY_F7              288
#define KEY_F8              289
#define KEY_G               103
#define KEY_U               117

#define KEY_F1				282
#define KEY_F3				284
#define KEY_MINUS           45
#define KEY_PLUS			61

#define KEY_A				97	
#define KEY_ESC				27
#define KEY_TAB				9
#define KEY_I				106
#define KEY_S				115
#define KEY_W				119


#define KEY_E				101
#define KEY_R				114
#define KEY_T				116
#define KEY_SPACE_BAR		32
#define KEY_DELETE          127

#define KEY_CTRL			306
#define KEY_ENTER			13

#define KEY_F				102

static bool ctrl_isDown = false;

#include "Capp.h"

#endif


void GameLogic::goCentre(){
    
    int Xvalue = (Tiles::houseDimension[0]+Tiles::houseDimension[2])/2;
    int YValue = (Tiles::houseDimension[1]+Tiles::houseDimension[3])/2;
    
    cpVect pos = Tiles::findCords(std::pair<int, int>(Xvalue,YValue));
    Camera::posX = -pos.x;
    Camera::posY = -pos.y;
    
    player.movePlayer(pos);
    
}

static int pathStart = 0;

void GameLogic::keyInput(KEY_STATES input, long key ){
	
	//printf("%lu\n", key);

    if (!GameLogic::loadingComplete){
        return;
    
    }
    
    if (input==KEY_UP){
        if (key==KEY_ESC){
            BobZombie::returnNormal();
        }
    }
    
    

    
    if (GDebug::debuggingActive){
    
        if (key==KEY_U){
            if (input==KEY_UP){
               // AudioController::playMusic(NORMAL);
            }
            
            
        }
        
        if (key==KEY_I){
            if (input==KEY_UP){
     
                AudioController::playMusic(TERROR3);
                
            }
            
        }
        
        
    }
    
    

    if (BobZombie::keyBoardInput()){
        
        return;
        
    }
    


     if (gameMode==CREDITMODE) {
         gameMode=OPENINGMODE;
         closeVideo();
         
     }
    
    
    if (GDebug::debuggingActive){
        if (input==KEY_UP){
            
            if (key==KEY_A){
                GDebug::changeValue(3,-1);
                Sprite::keyDebug(KEY_SEMICOLAN);
                Door::keyDebug(KEY_SEMICOLAN);
                
            }else if (key==KEY_D){
                GDebug::changeValue(3,1);
                Sprite::keyDebug(KEY_APOST);
                Door::keyDebug(KEY_APOST);
            } else if (key==KEY_S){
                GDebug::changeValue(2,-1);
                Sprite::keyDebug(43);
                Door::keyDebug(43);
            }else if (key==KEY_W){
                GDebug::changeValue(2,1);
                Sprite::keyDebug(47);
                Door::keyDebug(47);
            } else if (key==KEY_Q){
                
                //SWITCH
                Door::keyDebug(KEY_Q);
                
            } else if (KEY_UP_PRESS == key){ //For UP key
                GDebug::changeValue(0,1);
                
                Sprite::keyDebug(126);
                Door::keyDebug(126);
                if (GameLogic::gameMode==DEBUGMODE){
                    Sprite::printAnimationState();
                }
                
            } else if (KEY_DOWN_PRESS == key){ //For DOWN key
                GDebug::changeValue(0,-1);
                
                if (GameLogic::gameMode==DEBUGMODE){
                    Sprite::printAnimationState();
                }
                
                Sprite::keyDebug(125);
                Door::keyDebug(125);
            } else if (KEY_RIGHT == key){ //RIGHT
                GDebug::changeValue(1,1);
                
                Sprite::keyDebug(124);
                Door::keyDebug(124);
                
            } else if (KEY_LEFT==key){ //LEFT
                GDebug::changeValue(1,-1);
                
                Sprite::keyDebug(123);
                Door::keyDebug(123);
                
            } else if (KEY_D==key ){ // For D key
                
                GDebug::value4 = Tiles::hoverTile;
                //  Sprite::killSprite();
                
            } else if (KEY_P==key){// For P key
                
                //Sprite::testFunction();
                Tiles::findPath(pathStart,Tiles::hoverTile);
                pathStart=Tiles::hoverTile;
                
            } else if (key==KEY_V){
                
                AudioController::setVolume("Music", 0);
                AudioController::setVolume("Effect", 0);
                
            } else if (key==KEY_C){
                GameLogic::energy+=100000;
                GameLogic::influence+=100000;
                
                for (std::map<int, SpriteDict*>::iterator IT = Sprite::theSpriteAssets.begin();
                     IT != Sprite::theSpriteAssets.end();
                     IT++){
                    
                    
                    IT->second->repLevelRequired=0;
                }
                for (std::vector<ScareDict>::iterator IT = Player::scareDict.begin();
                     IT != Player::scareDict.end();
                     IT++){
                
                    IT->repLevelRequired=0;
                
                }
                
                for (std::vector<TileDict*>::iterator IT = Tiles::tileDictVector.begin();
                     IT != Tiles::tileDictVector.end(); IT++) {
                    if (*IT){
                        (*IT)->requiredLevel=0;
                    }
                }
                
                for (std::map<int,TileDict*>::iterator IT = Tiles::tileDictionary.begin();
                     IT != Tiles::tileDictionary.end(); IT++) {
                    IT->second->requiredLevel=0;
                }
                
                
                

                
                
                
       
                
                
                
            }else if (key==KEY_X){
                
                GameLogic::energy=0;
                GameLogic::influence=0;
                
            } else if (key==KEY_F5){
                SaveObject::save("SaveTest.map");
                TranslationMatrix::warning(SAVE_THE_GAME);
                
            } else if (key==KEY_F7){
                
                SaveObject newObject;
                if (newObject.load("SaveTest.map")){
                    newObject.activate();
                }
                
            } else if (key==KEY_F8){
                
                
                
            } else if (key==KEY_B){
                
                cpVect thePos = Bulldozer::spawnBulldozer();
                thePos = cpvmult(thePos, -1.0);
                
                Camera::posX = thePos.x;
                Camera::posY = thePos.y;
                
            }  else if (key==KEY_U){
                
                
                
                
                
                
                
            }else if (key==KEY_F3){
                
                
                AudioController::testAllAudio();
                
                
            }
            
        }
    
    
    
    }
    

    
    
    if (input==KEY_UP){
    
        
        if (gameMode == HELPSCREENMODE){
            changeGameMode(lastGameMode);
            lastGameMode = HELPSCREENMODE;
            return;
        }
        
        
        
    if (KEY_ONE       == key){
        if (testIfGameMode()){
            GameLogic::changeGameMode(PLAYMODE);
        }
    } else if (KEY_TWO      == key){
        if (testIfGameMode()){
            GameLogic::changeGameMode(TRAPMODE);
        }
    } else if (KEY_THREE    == key){
        if (testIfGameMode()){
            GameLogic::changeGameMode(PROPMODE);
        }
    } else if (KEY_FOUR == key){
        if (testIfGameMode()){
            GameLogic::changeGameMode(TILEMODE);
        }
    } else if (KEY_FIVE == key){
        if (testIfGameMode()){
            GameLogic::changeGameMode(DOORMODE);
        }
    }else if (KEY_ZERO     == key){
        if (GDebug::debuggingActive){
            if (testIfGameMode()){
                GameLogic::changeGameMode(SPRITEMODE);
            }
        }
        
    } else if (KEY_SQUARE_OPEN   == key){ //For [ key
      GameLogic::decreaseSpeed();
    } else if (KEY_SQUARE_CLOSE == key){ //For ] key
          GameLogic::increaseSpeed();
    }   else if (key==KEY_PLUS){
        Camera::increaseZoom();
        
        
    }else if (key==KEY_MINUS){
        Camera::decreaseZoom();
        
    } else if (key==KEY_F1){

        
        if (gameMode != HELPSCREENMODE){
            lastGameMode = gameMode;
            changeGameMode(HELPSCREENMODE);
        } else {
            changeGameMode(lastGameMode);
            lastGameMode = HELPSCREENMODE;
            
        }
        
    }  else if (key==KEY_TAB){
    
        if (input==KEY_UP){
            GameLogic::pause =!GameLogic::pause;
        }
    
    } else if (key==KEY_SPACE_BAR ){
    
        if (actPlay()){
            if (!BobZombie::toggle){
        
                goCentre();
            }
        }
       // Player::mainPlayer->activateLastScare();
    
    } else if (key==KEY_DELETE){

   
        
    
    Sprite::mouseInputDeleteAfterCameraMath(MOUSE_STATE_UP, lastTouchPosition, MOUSE_R);
    
    } else if (key==KEY_F){
    
	
		int value = Tiles::findTile(Tiles::findTile(lastTouchPosition));
    

		printf("%i, Tile Position\n" ,value);
        
        
        
        
        
    }
        
        
        
    }
    
    
    if (GameLogic::gameMode==PLAYMODE){
        if (key==KEY_Q){
            Player::scareAbilityButton(0);
        } else if (key==KEY_W){
            Player::scareAbilityButton(1);
        } else if (key==KEY_E){
            Player::scareAbilityButton(2);
        }else if (key==KEY_R){
            Player::scareAbilityButton(3);
        }else if (key==KEY_T){
            Player::scareAbilityButton(4);
        }
    }
    
}


void GameLogic::quit(){
    
    quitTheGame();

}

