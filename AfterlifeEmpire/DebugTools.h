//
//  DebugTools.h
//  After Life Empire
//
//  Created by Autobotika on 2014-10-23.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__DebugTools__
#define __After_Life_Empire__DebugTools__

#include <string>

class DebugTools
{
public:

    static void spawnLuaWindow(std::string errorMessage, std::string code);
    static void despawnLuaWindow();
    

    
    static void spawnSpriteWindow();
    static void spawnTileWindow();
    static void despawnTileWindow();
    
    static void spawnErrorWindow(std::string errorMessage);
    
    static void startClock(int clock, bool reset);
   
        
};

#endif /* defined(__After_Life_Empire__DebugTools__) */
