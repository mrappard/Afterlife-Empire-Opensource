//
//  Sprite.h
//  ThanatosIpad
//
//  Created by Autobotika on 13-09-20.
//  Copyright (c) 2013 Autobotika. All rights reserved.
//

#ifndef ThanatosIpad_Sprite_h
#define ThanatosIpad_Sprite_h

#include "VecAndBB.h"
#include <vector>
#include "INPUT_STATES.h"
#include <set>
#include <map>
#include <string>
#include "OpenGLImports.h"
#include "PathTile.h"
#include <set>


#include "BitmapFontClass.h"
#include "Traits.h"

enum SpecialFactor{

    NOFACTORS,
    TWOXTWO,
    THREEXTHREE
    
};

enum blockTypes{
    NoPlacement,
    OneXOne,
    OneXOneLeftWall,
    TwoXTwo,
    ThreeXThree,
    ThreeXTwoL,
    TwoXOneU,
    TwoXOneL,
    TwoXOneULeftWall,
    ThreeXOneL,
    ThreeXOneLRightWall,
    ThreeXTwoU,
    
    SixXThreeL,
    TwoXEight
};




class Animation_Holder{
    
public:
    int start;
    int end;
    int speed;
};




class ScareType{
public:
    
    std::string name;
    double scareModifier;
    double damageModifier;
    double awarenessModifier;
    ScareType(bool working);
    ScareType();
    ScareType(std::string _name, double scare, double damage, double awareness);
    ~ScareType();
};


class ScareDict{
public:
    std::string name;
    int index;
    int energy;
    int radius;
    int repLevelRequired;
    int iconOffset;
    int reload;
    long lastClick;
     std::string bobText;
   
    
    SpecialFactor specialFactor;
    std::map<std::string, ScareType> scareTypeByName;
    std::multimap<double, ScareType> scareByScareModifier;
    
};


class SpriteDict{
public:
    int index;
    bool init;
    std::string name;
    std::string textureName;
    std::string jsonName;
    
    double width;
    double height;
    
    GLfloat textureWidth;
    GLfloat textureHeight;
    GLfloat segWidth;
    GLfloat segHeight;
    
    GLfloat health;
    int cost;
    
    bool textureGen;
    GLuint * texture;
    std::vector<GLfloat> textureArray;
    std::vector<GLfloat> offsetArray;
    std::vector<Animation_Holder> animations;
    std::string brain;
    
    std::string bobText;
    
    bool rotated;
    bool trimmed;
    bool prop;
    bool aTree;
    
    int idleNumber;
    int tileGen;
    
    
    std::vector<GLuint> frames;
    std::vector<int> other;
    
    std::map<float, int> spritesSpawned;

    
    
    GLfloat sludgeX;
    GLfloat sludgeY;
    
    
    bool animates;
    
    std::map<std::string, ScareType> scareTypeByName;
    std::multimap<double, ScareType> scareByScareModifier;
    
    blockTypes placementType;
    
    std::map<std::string, std::string> dialog;
    
    int leaveHappyRep;
    int leaveFearRep;
    int leaveScaredRep;
    
    int repLevelRequired;
    int activationEnergy;
    
    TRAITS trait;
    
    bool mod;
    
};



class Sprite
{
public:
    
    //Creation
    Sprite();
    Sprite(cpVect anPosition, int anType, bool graveYardAble);
    Sprite(std::pair<int, int> anPosition, int anType, bool graveYardAble);
    Sprite(int anType);
    ~Sprite();
    
    //Main functions
    void think();
    void thinkText();
    void updateNoGL();
    void updateGL();
    void update();
    void update(cpVect pos, double rotation);
    void updateShadow(cpVect pos, double rotation);
    void updateSymbol();
    void renderSymbol();
    void render();
    void renderShadow();
    
    // Main Mouse Input Function
    static void mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    static void mouseInputC(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    static bool mouseInputGame(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    // Used to debug animation states
    static void mouseDebugGame(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    // Used to delete a Sprite
    static void mouseInputDelete(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    // Find a Sprite and Pull the Info
    static bool mouseInputFind(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    
    static Sprite * addSpriteSave(cpVect anPosition, int anType);
    static Sprite * addSpriteLarge(int anType, SpriteDict * target);
    static Sprite * addSpriteLarge(int _index, int _type);
    static Sprite * addSprite(cpVect anPosition, int anType);
    static Sprite * addSprite(std::pair<int,int>, int anType);
    static Sprite * addSprite(int anPosition, int anType);
    
    
    static Sprite * moveSprite;

    
    
    
    
    
    //These Function Control the Sprite drawn for the map
    static void testSpriteUpdate();
    static void testSpriteRender();
    
    static void thinkAll();
    static void updateAll();
    static void renderAll();
    static void purgeAll();
    
    
    // Genereate A Sprite
    void generate(cpVect anPosition, int anType, bool graveYardAble);
    void generate(std::pair<int, int>, int anType, bool graveYardAble);
    
    //Call this before freeing a Sprite
    
    void destroy(bool forEver);


    
    // USED TO LOAD ASSETS
    static bool allTexturesInit;
    static std::string loadingString;
    static int lastCount;
    static int totalCount;
    
    //Used to generate a path
    bool findPath(int start, int end);
    bool findPath(int end);
    long pathThink;
    std::vector<PathTile> path;

    
    
    
    
    
    
    
    
    
    
    //Used to change the state
    void idle();

    // Used to clean RenderGrid
    static std::set<size_t> gridsLastTurn;
    
    // The type of Sprite
    int type;
    
    // Used to generate UID
    static long generateUID();
    static long UIDCounter;
    long UID;
    
    // How Long a Sprite will exist in the Map before leaving
    int maxIdleTime;
    // How Long a Sprite has existed in the Map
    int idleTime;
    
    // Used to determine the direction
    int direction;
    
    
    
    

    static size_t changeSpriteType;
    
    //cpBody * theBody;
   //cpShape * theShape;
    cpVect position;
    cpVect grid;
    
    SpriteDict * dict;
    GLuint * _texture;
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    
    GLuint _vertexArrayShadow;
    GLuint _vertexBufferShadow;

    GLuint _vertexArraySymbol;
    GLuint _vertexBufferSymbol;
    
    GLuint * textureSymbol;
    
    static std::vector<GLuint*> traitTexture;
    
    
    GLuint animationStateHack;
    GLuint animationState;
    GLuint goto_AnimationState;
    GLuint frame;
    
    GLuint startFrame;
    bool isSelected;
    bool isScared;
    
    cpVect startKey;
    cpVect endKey;
    int startPos;
    int endPos;
    double tween;
    
    long lastSeen;
    
    int previousPosition;
    
    
    int renderGridIndex;
   
    cpBB boundingBox;
    cpBB textureBox;
    
    void advanceFrame();
    void setAnimationState(int theFrame);
    


 
    
    static Sprite * testSprite;
    
    cpBB getDimension();
    

    static bool loadDictTextureLoader(std::string * path);
    static bool loadAllTextureTimed();
    
    static bool testSpriteLarge(int location, SpriteDict * target);
    


    
    static void updateSelect();
    void renderSelect();
    void renderFast();
    
    static void resetLevel();

   
    
    static void deselectAll();
    
    static std::map<size_t, Sprite*> spriteByID;
    static std::set<Sprite*> graveYard;
    
    static std::vector<Sprite> selectorSprites;
    static int selectedType;
    
    static void changeSelect(size_t type);
    
    static Sprite * mouse;
    
    std::map<std::string,int> metaData;
    
    //std::map<SpriteDict,int> dictionary;
    
    cpVect summonPosition;
    bool graveYardAble;
    bool kill;
    static bool spriteCostToMuch;
    static int indexOfTestSprite ;
    
    static bool parseJsonFrame(SpriteDict * dict);
    
    static SpriteDict * getInfoForSprite(int type);
    static std::map<int, SpriteDict*> theSpriteAssets;
    
    
    static void jsonDumpData(std::string * theOutput);
    static bool loadJson(std::string * theOutput);
    
    cpVect dumpTextureCord(int frame, GLfloat * text);
    
    static cpVect dumpTextureCord(int index, int frame, GLfloat * text, SpriteDict * target);
    
    
    
    
    static bool UItiles(int startIndex, int numberOfTiles,
                        std::vector<SpriteDict*> * dict, bool propOrNot, bool animate);
    
    
    static SpriteDict * debugDict;
    static void keyDebug(int keyCode);
 
    static void printAnimationState();
    
    static void spawnCivilian();
    static int getRandomCivilian();
    

    static void scareAllSprites(cpVect location, int radius);
    
    static void scareAllSprites(cpVect location, int radius, SpriteDict * target);
    
    static void scareAllSprites(cpVect location, int radius, SpriteDict * target, long uid_i);
    
    void scare(cpVect location);
    void scare(cpVect location, SpriteDict * target);
    void scareIdiot(cpVect location, SpriteDict * target, Sprite * trap);
    void scareFast(cpVect location);
   
    void scareFromPlayer(cpVect location,  ScareDict * target);
    
    double resolveScare(SpriteDict * scare);
    
    static void summonAllSprites(cpVect location, double radius);
    void summon(cpVect location);
    

    bool die();
    bool die(SpriteDict * _target);

    
    static bool removeSprite(Sprite * theSprite);
    void generateColor(float * color);
    int  generateReputation();
   
    

    
    
    static void spawnTestCiv();
    
    static void killSprite();

	
	static bool parseJsonFrameNew(SpriteDict * dict);
    
    static bool overSpriteForRadius;
    
    
    void scarePeople();
    void traitTest();
    
    
        long timeWhenSpawned;
    size_t lastTimeItWasScared;
    std::set<long> scaredByList;
     std::string speak();
    std::vector<CBitmapFont*> theTexts;
    
    GLfloat timeTileDead;
    cpVect minPoint;
    std::vector<int> blockedTiles;
    static int dontRenderTest;
    int stumbleTimes;
    int walkingTarget;
    
    double health;
    
    bool isDead();
    
    
    std::vector<size_t> traits;
    
    
    void addTrait(size_t traits);
    bool testTrait(size_t _traits, size_t _time);
    size_t testTraitTime(size_t _traits);
    static std::pair<int,int> getEnergyUsedOnProps();
    static void generateTextForLevelStatus();
    
   static std::pair<int,int> dimensionsForBlockType(blockTypes type);
    
    
    static std::set<int> trees;
    
   static void spawnTrees();
    
   
    static void spawnWander(int badOrGood);
    
    bool checkIfLocationIsInvalid();
    
    static std::map<size_t, Sprite*> trapsByUID;
    static std::map<size_t, Sprite*> peopleByUID;
    static std::map<size_t, Sprite*> propsByUID;
    
    static std::set<Sprite*>    goodPeople;
    static std::set<Sprite*>    badPeople;
    static std::set<Sprite*>    authorites;
    
    bool extraScareEnergyFromCouple();
    
    
    static bool checkForSpawning();
    
    static void gameOver();
    
    
    static bool checkIfBadGuy(Sprite* value);
    
    static bool outSideHouseCheck;
    static std::vector<int> outSideHouseTiles();
    static std::vector<int> outSideHouse;
    
    static void test();
    bool scareTraitFactors();
    std::pair<ScareType,ScareType> findScareModifier(SpriteDict * _scare);
    
    void scareAllSpritesGood(cpVect location, int radius, SpriteDict * target, long uid_i);

    static void mouseInputDeleteAfterCameraMath(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    static bool loadDictTextureLoaderReturn(std::string * path);
    
    void addText(std::string theText);
    void addText(std::string theText,GLfloat red, GLfloat green, GLfloat blue);
    void renderText();
    
    
   static std::vector<int> getRadiusAroundLargeSprite(Sprite* theSprite);
    
   static bool canMove;
    static bool ShouldFlip;
    bool flip;
};


#endif
