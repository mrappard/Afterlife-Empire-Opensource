//
//  Map.h
//  ThanatosIpad
//
//  Created by Autobotika on 13-09-20.
//  Copyright (c) 2013 Autobotika. All rights reserved.
//

#ifndef ThanatosIpad_Map_h
#define ThanatosIpad_Map_h

#include "VecAndBB.h"


#include "INPUT_STATES.h"
#include <map>
#include <string>


enum GAMEMODETYPE{
    TILEMODE,
    SPRITEMODE,
    PROPMODE,
    TRAPMODE,
    DOORMODE,
    OPENINGMODE,
    PLAYMODE,
    CREDITMODE,
    DEBUGMODE,
    CREDITTEXTMODE,
    HELPSCREENMODE
};

class reputationStruct {

public:
    int energy;
    int numberOfStruct;
    std::string levelUpText;
    std::string infoText;
    std::map<float,int> woodspawn;
    int numberGoodGuy;
    int numberBadGuy;
    int numberAuthorities;
    std::map<float,int> badspawn;
    std::map<float,int> authorities;
    double speed;
    double energyDrain;
    double lowEnergy;
    double midEnergy;
    
    
};


//#include "Lantern.h"


class GameLogic
{
public:
    
    static bool toggleHints;
    static bool toggleFinderUI;
    static bool toggleBob;
    
    static double influence;
    static double energy;
    static double awareness;
    static double awarenessSink;
    static double reputation;
	static long timePassed;

    static double reputationDrainMod;
    
    static int spawnRateNormal;
    static double drainAmount;
    static double lowAmountEnergy;
    static double midAmountEnergy;
    static int currentLevel;
    

    static double width;
    static double height;
    static double screenWidth;
    static double screenHeight;
       
    static GAMEMODETYPE gameMode;
    void static changeGameMode(GAMEMODETYPE change);

    static cpVect mousePosition;
    
    static std::map<long, cpVect> theTouchesStart;
    static std::map<long, cpVect> theTouchesDrag;
    //static std::map<long, cpVect> theTouchesEnd;
    
    
    //static Lantern debugLantern;
    
    static void drawBackground();
    
    static bool initialized;
    
    static bool readyToRender;

	static void resetLevel();
    static void clearAllTouches();
	static void _initVideo();
    static void start();
    static void update();
    static void render();
    static void quit();
    static void tick(long tick);
    static void purge(bool activeInActive);
    static void mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber);
    static void keyInput(KEY_STATES input, long key);
    
    static void slideMap(MOUSE_STATES input, cpVect control, long theTouchNumber);
    static void zeroView();
    
    static void jsonDumpData(std::string * theOutput);
    static void setWindow(int widthI, int heightI);
    
    static cpVect lastTouchPosition;
    
    static bool mouseInputLeftDown;
    static bool mouseInputRightDown;
    
  
    static std::map<int,reputationStruct> reputationLevels;

    static int getCurrentLevel();

    static void setEnergyLevel(int level);
    static void * returnPlayer();
    
    static bool gameIsOver;
    
    static void increaseSpeed();
    static void decreaseSpeed();
    
    
    static int speedSetting;
    static int count;
    
    static bool pause;
    
    static bool loadingComplete;
    
    static void thinkLogicBobZombie();
    static void calculateCurrentLevel();
    
    static void mouseRender();
    
    static int numberOfTrapsInPlay;
    static int numberOfTrapsNeeded;
    static int numberOfTilesInPlay;
    static int numberOfTilesNeeded;
    
    static bool actPlay();
    
    static void drainEnergyTick();
      static long testIfDrainShouldStart;
    
    static bool actuallyThink();
    
    static void loopStart();
    
    
    static bool mouseLeftSideScreen;
    static bool mouseRightSideScreen;
    static bool mouseTopSideScreen;
    static bool mouseDownSideScreen;
    
    
    static void moveCameraSafe(cpVect moveAmount);
    static void setRandom();
    
    static void goCentre();
};


#endif
