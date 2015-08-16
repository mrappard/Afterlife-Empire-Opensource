//
//  SaveObject.h
//  After Life Empire
//
//  Created by Autobotika on 2014-11-18.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__SaveObject__
#define __After_Life_Empire__SaveObject__

#include <stdio.h>
#include <string>
#include <vector>

class SpriteSaveObject{
    public:
    int type;
    double xPos;
    double yPos;
    
    
};




class TileSaveObject{
    public:
    TileSaveObject();
    ~TileSaveObject();
    short type;
    int lw;
    int  rw;
    int doorR;
    int doorL;
};

class SaveObject{
public:

    static bool save(std::string name);
    bool load(std::string name);

    
    std::string name;
    std::string header;
    std::vector<TileSaveObject> walls;
    size_t widthW;
    size_t heightW;

 
    unsigned long widthT;
    unsigned long heightT;
    
    unsigned long energy;
	unsigned long awareness;
	unsigned long currentLevel;
	unsigned long clock;

    unsigned long bobStates;
    
    std::vector<char> sprites;
    std::vector<char> thumbnail;
    
    std::vector<char> output;
    
    bool copyWalls();
    bool copySprites();
    bool createSprites();
    bool createThumbNail();
    
    bool parseWalls();
    
    char * allTogetherFile();
    
    void getThumbnail();
    
    void activate();
    
    SaveObject();
    ~SaveObject();
    
	
    
    bool init(std::vector<char> * input );
    
    void clear();
    
    static void changeJsonToRaw(void * theSpriteDict, int type);;

    
};



#endif /* defined(__After_Life_Empire__SaveObject__) */
