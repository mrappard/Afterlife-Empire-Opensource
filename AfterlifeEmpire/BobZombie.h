//
//  BobZombie.h
//  After Life Empire
//
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef After_Life_Empire_BobZombie_h
#define After_Life_Empire_BobZombie_h

#include "openglimports.h"
#include "BitmapFontClass.h"
#include "Sprite.h"
#include <bitset>


enum TutorialEnum {
    
    
    
    
    NO_TUTORIAL,
    OPENING_TEXT,
    CHARACTER_COMING_IN,
    MOVE_GHOST,
    SELECT_SCARE_FROM_BOTTOM,
    SHOW_SCARES,
    PLACE_SCARES,
    SPAWN_MORE_CHARS,
    FIND_SCARED_CHARS,
    TALK_ABOUT_SCARE_TRAP,
    SCARE_TRAP_INTERACT,
    PLACE_SCARE_TRAP,
    TRAP_PLACED,
    WAIT_TO_SELECT_MODE,
    WAIT_TO_SELECT_BAR,
    MAKE_THE_ROOM,
    TALK_ABOUT_DOOR_MODE,
    CHANGE_TO_DOOR_MODE,
    ADD_A_DOOR,
    ADD_SOME_PROPS_TEXT,
    ADD_SOME_PROPS,
    OH_NOES_AN_EXORCIST_TEXT,
    OH_NOES_AN_EXORCIST,
    SCARE_EXORCIST_THAT_WAS_HILARIOUS_TEXT,
    SCARE_EXORCIST_THAT_WAS_HILARIOUS,
    ITS_THE_COPS,
    THE_COP_IS_GONE_TEXT,
    SHOW_DASH_BOARD_1,
    SHOW_DASH_BOARD_2,
    SHOW_DASH_BOARD_3,
    SHOW_DASH_BOARD_4,
	THE_DEMO_IS_OVER,
    
    
    
    
    OH_NOES_SHE_DIED_TEXT,
    EXPLAIN_DASH_BOARD,
    LEVEL_UP,
    EXTRA,

    


};


class BobZombie
{
public:
  

    //Used for Debugging
    static std::vector<int> bobEventTest ;
    
    static void updateLoading();
    static void renderLoading();
    static void update();
    static void render();
    static void think();
    
    
    
    static void cursorUpdate();
    static void cursorRender();

    
    

    
    
    static CBitmapFont theTitle;
    static CBitmapFont helpText;
    static CBitmapFont traitText;
    
    
    static CBitmapFont setH;
    static CBitmapFont setF;
    
    static CBitmapFont escText;
    
    static GLfloat yPosSlide;
    static GLfloat yPosMax;
    
    static GLfloat timer;
    static bool toggle;
    
    static bool resetText;
    
    static bool tutorialText;
    
    static void setText(std::string newText);
    
    static bool toggleState();
    
    static void triggerTutorial();
    static size_t tutorailKey;
    static void tutorialScroll();
    
    static void setSprite(SpriteDict * target);
    
    static int waitForEffect;
    static bool bobWaitTime();
    
    static int floatingVar;
 
    
    static TutorialEnum currentTutorialState;
    static void changeTutorialState();
    
 
    
    static GLuint _vertexArray_Arrow;
    static GLuint _vertexBuffer_Arrow;
    
    static GLuint _vertexArray_BlackBox;
    static GLuint _vertexBuffer_BlackBox;
    
    
    static int sideSelector();
    
    
    
    static GLuint * _textureArrow;
    
    static size_t tick;
    
    static bool mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber);
    static bool keyBoardInput();
    
    static bool hasSelected[6];
    
    static bool spawnNewCitizens();
    static void thinkLogicBobZombie();
    
    static bool cameraMoveOn;
    static cpVect cameraMove;
    
    static void returnNormal();
    static int dashBoardDraw();
    static bool bottomBarDraw();
    
    static bool checkEvent(int event);
    static bool checkEvent(SpriteDict * trait);
    
    static bool peopleCanDie();
    static bool scaresAreFree();
    
    static size_t checkValue;
    static void levelUp(int newLevel);
    
    static unsigned long returnState();
    static void setStates(unsigned long states);
    
    static bool peopleCanTalk();
    
    static void textToEnum();
    
    static bool canThink();
 
    
    
    
    static int TEENAGER_A;
    static int TEENAGER_B;
    static int EXORCIST_A;
    static int POLICE_A;
   
	static void demoEnd();

	
	static void restSlideState();
    
private:
        static SpriteDict * theTargetSprite;
        static CBitmapFont theText;
    
    
    
    static GLuint * bodyTexture;
    static GLuint * leftArmTexture;
    static GLuint * leftHandTexture;
    static GLuint * headTexture;
    static GLuint * pupilTexture;
    static GLuint * rightArmTexture;
    static GLuint * winkTexture;
    
    
    static bool pupilInOrOut;
    static GLfloat pupilTween;
    static GLuint  nextWink;
    static GLuint  winkTime;
    
    static GLuint _vertexArray_Bob;
    static GLuint _vertexBuffer_Bob;
    static GLuint _sizeOfBob;
    
    
    static GLfloat leftArmTween;
    static bool leftArmInOrOut;
    
    static GLfloat leftHandTween;
    static bool leftHandInOrOut;
    
    static GLfloat rightArmTween;
    static bool rightHandInOrOut;
    
    
    
    
    static GLuint _vertexArray;
    static GLuint _vertexBuffer;
    
    static GLuint _loadingBarArray;
    static GLuint _loadingBarBuffer;
    static GLuint _loadingBarSize;
    
    
};

float findLargestOf8Values(GLfloat * array, int * index );
float findLargestOf8Values(GLfloat * array);

#endif
