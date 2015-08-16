//
//  Tiles.h
//  After Life Empire
//
//  Created by Autobotika on 2014-09-30.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__Tiles__
#define __After_Life_Empire__Tiles__

#include <stdio.h>
#include <vector>
#include "INPUT_STATES.h"
#include "VecAndBB.h"
#include <string>
#include "OpenGLImports.h"
#include <map>
#include <utility>
#include "Door.h"

#define DOOR_OK_SPRITES_NO 0
#define DOOR_NO_SPRITES_NO 1
#define DOOR_OK_SPRITES_OK 2
#define DOON_NO_SPIRTES_OK 3


class FastTileObject {
    public:
    short type;
    int start;
    int end;


};

class TileDict{
public:
    std::string name;
    std::string image;
    std::string checkered;
    
    bool block;
    bool walls;
  
    std::string imageL;
    std::string imageR;
    
    GLuint * texture;
    
    GLuint * textureC;
    
    bool checker;
    
    GLuint * textureL;
    GLuint * textureR;
    
    int requiredLevel;
    int energyRequired;
    
    bool init;
    bool mod;
    

    int index;

};

class TileObject{
    public:
        TileObject();
        short type;
        bool lw;
        bool rw;
        bool window;
    
      bool canWalkThere;
    
        //GLuint * textureFloor;
        GLuint * textureWallL;
        GLuint * textureWallR;
        Door * theDoorL;
        Door * theDoorR;
        bool block;
        long thinkValue;
        Door * theDoor;
    
    
    
        std::vector<Sprite*> renderGrid;
    
      Sprite * occupiedSprite;
    void renderTile();
    void renderTile(size_t index);
    void renderTile(size_t index, int _type);
};



class Tiles
{
public:
    

    static bool hasBeenCheckedForWalkability;
    static size_t checkWalkAbility();
    static std::vector<int> usedTiles;
    
    static GLuint _vertexArrayTiles;
    static GLuint _vertexBufferTiles;
    
    static GLuint _vertexArrayWalls;
    static GLuint _vertexBufferWalls;
    
    
    static GLuint _vertexArraySelection;
    static GLuint _vertexBufferSelection;

    
    static int hoverWall;
    static int hoverWallSide;
    static int hoverTile;
    static bool halfWalls;
    

    
    
    static int startIndex;
    static int lastIndex;
    
    
    
    static std::vector<TileDict*> tileDictVector;
    static std::map<int,TileDict*> tileDictionary;
    
    
   // static std::vector<GLuint*> textureTileDict;
   // static std::vector<GLuint*> textureWallDictL;
   // static std::vector<GLuint*> textureWallDictR;
    
    static bool updated;
    static std::vector<TileObject> mapValues;
    static int width;
    static int height;
    static void createTileTextures();
    
	static size_t changeTileType;
    
    static bool canMove(int index, int x);
  
    static bool canMove(int index, int x, int type );
    
    static void findPath(int start, int end);
    
    static void saveDict();
    
    static int moveDirection(int start ,int end);
    
    static std::pair<int,int> findTile(int path);
    static int findTile(std::pair<int,int> path);
    static int findTile(int valueOne, int valueTwo);
    static std::pair<int,int> findTile(cpVect control);
    
    static cpVect findCords(std::pair<int,int> control);
    //static void findTile(cpVect control, int * returnValue);
    static void initialize(int widthI, int heightI);
    static void update();
    static void render();
    static void renderWallA(int x, int y, bool z);
    static void renderWallL(int x, int y);
    
    static void renderWallDR(int x, int y);
    static void renderWallR(int x, int y);
    
    
    static void renderSprites(int x, int y);
    static void renderSpritesShadow(int x, int y);

    
    static void updateSelection();
    static void renderSelection();
    
    static int clearWall(int index, MOUSE_STATES input);
    
    static bool saveToFile();
    
    static bool UItiles(int startIndex, int numberOfTiles,
                   std::vector<TileDict*> * dict);
    
    static bool doesThisTileMakeWalls(int type);
    
    
    static bool doorMouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    static int mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber);
    
    static bool grabMap(std::vector<char> * returnValue);
    
    static void createTile(std::vector<GLfloat> * vertex, GLfloat multi, GLfloat rightMove, GLfloat topMove );
    
    
    static void fillTiles(int left, int up, int right, int down, size_t type);
    static void fillTilesDebug(int left, int up, int right, int down, size_t type);
    static void fillTilesMakeWalls(int left, int up, int right, int down, size_t type);
    
    static int energyUsed(int start, int end);
    static int energyUsed(int x1, int y1, int x2, int y2);
 
    
    static int returnInfluenceUsed(int x1, int y1, int x2, int y2);
    static int returnInfluenceUsed(int start, int end);
    static void getAllWalkTiles();
    static int getRandomTileHouse();
    
    static int goDirect(int index, int direction);
    static bool checkTileOffestForPlacement(int index, int offsetX, int offsetY);
  
    static bool isTileOccupied(int index);
    
    
    static bool purge();
    static void createBlankMap();
    
    static std::vector<Sprite*> grabAllSprite(int index, int radius);
    static std::vector<Sprite*> grabAllSpriteLos(int index, int radius);
    static std::vector<Sprite*> grabAllTraps(int index, int radius);
    static std::vector<Sprite*> grabAllTrapsInRoom(int index);
    
    static void tileShowRadius(int _index, int radius);
    
    static int clearWall(int index, bool create);
  
    static std::set<int> hoverTileSelected;
    static std::set<int> trapRadius;
    
    static cpVect defTile(cpVect pos);
    
    static void findHouseDimensions();
    static std::vector<int> tilesOutSideOfHouse();
    static int houseDimension[4];
    
    
    static std::vector<int>  getSizeOrRoom(int index);
    
    static bool insideHouse(int index);
    static std::vector<int> getAllTilesInRoom(int index);
    static int checkIfEdge(int edge);
    static int checkIfEdge(int edge, bool * returner);
    static bool canMove2I(int start, int end);
    
    static void changeTile(int index, size_t type, long idValue);
    
    static void returnNormalShader();
    
    
    
    
    static void stageOneRender();
    static void stageTwoRender();
    static void stageThreeRender();
    static void stageFourRender();
    static void stageFiveRender();
    static void stageSixRender();
    static void stageSevenRender();
    static void stageEightRender();
    static void stageNineRender();
    static void stageTenRender();
    static void stageElevenRender();
    static void stageTwelveRender();
    
    static std::map<int,std::vector< FastTileObject> > fastTileSort;
    
    static void killLostWalls(int left, int down, int up, int right );
    
    static int checkWall(int index, bool leftOrRight);
    
    static bool valueSafe(int index);
    static bool valueSafe(int index, int backOffset, int forwardOffset);
 
};



#endif /* defined(__After_Life_Empire__Tiles__) */
