//
//  UserInterface.h
//  After Life Empire
//
//  Created by Autobotika on 2015-01-13.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef After_Life_Empire_UserInterface_h
#define After_Life_Empire_UserInterface_h


#include "OpenGLImports.h"
#include "INPUT_STATES.h"
#include "VecAndBB.h"
#include <string>
#include "BitmapFontClass.h"
#include "SaveObject.h"
#include <set>

#define TEXTBOX_STAY_UP_FOREVER 2147483647
#define TEXTBOX_STAY_UP         20
#define TEXTBOX_USER_READ       4


enum SpecialDisplayMode {
    NOTHING_MODE,
    GAME_MODE,
    MOD_MODE,
    SAVE_OR_NOT_MODE,
    BOB_INFO
};



class SpeechBox{
    public:
    
    SpeechBox();
    SpeechBox(std::string text, double time);
    ~SpeechBox();
    
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    static GLuint * _texture;
    void * talkBox;

    double despawnTime;
    double currentTime;
    
    std::string text;
    CBitmapFont textBox;
    
    void update();
    void render();
    void renderText();
    bool tick();
    bool topBot;
    
    
};


class UserInterface
{
public:
    
    static std::set<std::string> gameTips;
    
    
    static std::vector<SpeechBox> dialog;
  
    static GLuint _vertexArray;
    static GLuint _vertexBuffer;
    
    static GLuint _vertexArrayUserBar;
    static GLuint _vertexBufferUserBar;
    
    static GLuint _vertexArrayOption ;
    static GLuint _vertexBufferOption ;
    
    static GLuint _vertexArraySaveGame;
    static GLuint _vertexBufferSaveGame;
    
    static GLuint _vertexArrayWindow;
    static GLuint _vertexBufferWindow;
    
    static GLuint _vertexArrayChangeUI;
    static GLuint _vertexBufferChangeUI;
    
    static cpBB optionButton;
 
    
 
    static GLuint * _openScreen;
    static GLuint * _hightLightButton;
    
    static GLuint * arrowLeft;
    static GLuint * arrowRight;
    
    
    static cpBB changeModeButtons[8];
    
    static int previousGamemode;
    
    static void think();
    static void update();
    static void render();
    
    static void dialogThink();
    static void dialogUpdate();
    static void dialogRender();

    static void slideRight();
    static void slideLeft();
    
    static bool mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    
    static void spawnTextBox(std::string text, double timeDisplayed);
    static void spawnTextBoxBot(std::string text, double timeDisplayed);
    static void spawnTextBoxConditional(std::string text, double timeDisplayed);
    static void despawnTextBox();
    
    
    static SpecialDisplayMode menuMode;
    static void changeMenuMode(SpecialDisplayMode _newMode);
    
    static int timer;
    
    static int uiOffset;
    
    static int currentSlotRunning;
    static SaveObject * loadedSaves[3];
    static void loadThumbNails();
    
    static GLuint * thumbNailTextures[3];
    

    
    static void spawnTextBox(std::string text, double timeDisplayed, void * spriteDict);
    
    
    static void generatePlayIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues );
    
    
    static void generateTrapIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues );
    
    static void generateTileIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues );
    
    static void generateSpriteIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues );
    
    static void generatePropIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues );
    
    static void generateDoorIcons(int start, std::vector<GLfloat> * values, std::pair<GLfloat, GLfloat> startVector, GLfloat step, std::pair<GLfloat, GLfloat> maxValues );
    
    
    
    
    static bool testOptionButton(MOUSE_STATES input, cpVect control, long theTouchNumber);
    static bool testOptionSaveGame(MOUSE_STATES input, cpVect control, long theTouchNumber);


    static void updateOptionSaveGame();
    
    static bool gameOverDrawRenderTest();
    static bool gameOverDrawUpdateTest();
        
    static void bobUpdate();
    static void bobRender();
    
    static void loadScreenUpdate();
    static void loadScreenRender();
    
    static void updateBackground();
    static void renderBackground();
    
    static void updateSound();
    static void renderSound();
    
    static void playSelect();
    
    
    static int modeSelector(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    static int testMouse(MOUSE_STATES input, cpVect control, long theTouchNumber);

    
    static void renderModeSelector();
    
    static int checkCircleRadius;
    
    static void createCircle();
    
    static void cleanUpInterfaceErrors();
    
    static bool mouseInputSound(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    static void loadUITextures();
    
    static CBitmapFont scareEnergy;
    static CBitmapFont awarenessLevel;
    static CBitmapFont tileAmount;
    static CBitmapFont trapCount;
    
    static CBitmapFont warning;
    static void updateWarningMessages();
    static void renderWarningMessages();
    
    static void updateDashboard();
    static void renderDashboard();


  
    
};


#endif
