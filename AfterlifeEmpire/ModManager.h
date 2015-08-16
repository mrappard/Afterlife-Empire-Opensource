//
//  ModManager.h
//  After Life Empire
//
//  Created by Autobotika on 2015-02-20.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__ModManager__
#define __After_Life_Empire__ModManager__

#include <string>
#include <map>
#include <vector>
#include "openglimports.h"
#include "Door.h"
#include "Sprite.h"
#include "Tiles.h"

class ModElement{
public:
    bool modLoaded;
    bool enabled;
    bool working;
    
    void click();
    
    std::string name;
    std::string description;
    
    std::string thePath;
    GLuint thumbNail;
    
    GLuint loadThumbNail();
    
    std::string getName();
    std::string getDescription();
    
    void destroy();
    void parseJson();
    ModElement();
    ~ModElement();
};

class ModManager{
    public:
    
    static std::vector<DoorDict*> modDoor;
    static std::vector<SpriteDict*> modSprite;
    static std::vector<TileDict*> modTile;
    
    
    static std::vector<std::string> targetMods;
    
    static std::map<std::string,ModElement> mods;
    static void addMod(std::string theString);
    static void populate();
    static std::vector<ModElement*> dumpElements(size_t index);
    static void populateBack();
    static void returnNormal();
    
    static void loadAllMods();
    static void removeAllMods();

    
};

#endif /* defined(__After_Life_Empire__ModManager__) */
