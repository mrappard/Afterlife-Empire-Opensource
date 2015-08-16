//
//  Door.h
//  After Life Empire
//
//  Created by Autobotika on 2015-03-04.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef After_Life_Empire_Door_h
#define After_Life_Empire_Door_h

#include <set>
#include "Sprite.h"


class DoorDict{
public:
    int index;
    bool init;
    std::string name;
    std::string textureName;
    std::string jsonName;
    std::string brainName;
    
    double width;
    double height;
    
    GLfloat textureWidth;
    GLfloat textureHeight;
    GLfloat segWidth;
    GLfloat segHeight;
    
    
    
    GLuint * texture;
    std::vector<GLfloat> textureArray;
    std::vector<GLfloat> offsetArray;
    std::vector<Animation_Holder> animations;
    std::string brain;
    
    bool animate;
    bool rotated;
    bool trimmed;
    
    bool prop;
    
    bool oneOrTwo;
    
    
    std::vector<GLuint> frames;
    std::vector<int> other;
    
    GLfloat sludgeX_L;
    GLfloat sludgeY_L;
    GLfloat sludgeX_R;
    GLfloat sludgeY_R;
    
    bool animates;
    bool mod;
    
    

};



class Door
{
public:
    
    Door();
    Door(int index, int type, bool leftOrRight);
    ~Door();
    int index;
    bool LeftOrRight;
    bool openDoor;
    DoorDict * dict;
    
    GLuint * _texture;
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    
    bool openingOrClosing;
    int type;
    int frame;
    int animationType;
    int fix_value;
    
    static std::map<int, DoorDict*> theDoorAssets;
    static bool loadDict(std::string * path);
    
    
    
    
    bool dumpTextureCord(int frame, GLfloat * text);
    static bool dumpTextureCord(int index, int frame, GLfloat * text, DoorDict * target);
    
    void think();
    void update();
    static void updateAll();
    void render();
    void renderA();
    void renderB();
    void destroy();
    
   
    void setFirstFrame(bool leftOrRight);

    
    
    static bool parseJsonFrame(DoorDict * dict);
    
    static DoorDict * createThis;
    static void changeSelect(int index);
    
    static std::map<int, Door*> indexMap;
    static std::set<Door*> thinkingDoors;
    static void thinkAll();

    static Door* addDoor(int theIndex, bool leftOrRight);
    static Door* addDoor(int theIndex, int type, bool leftOrRight);
    static bool removeDoor(int _theIndex, bool _leftOrRight);
    static bool checkDoor(int theIndex, bool leftOrRight, Sprite*theSprite, Door ** theDoor);
    
    static bool UItiles(int startIndex,int numberOfTiles,std::vector<DoorDict*> * dict);
    
    static void keyDebug(int keyCode);
    
    static void dumpAllStruts();
    
    static void removeAllDoors();
};

#endif
