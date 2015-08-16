//
//  TranslationMatrix.h
//  After Life Empire
//
//  Created by Autobotika on 2015-04-20.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__TranslationMatrix__
#define __After_Life_Empire__TranslationMatrix__

#include <stdio.h>
#include <string>
#include <map>

typedef enum {
    EVERYTHING_OK,
    NOT_ENOUGH_ENERGY,
    SPACE_OCCUPIED,
    LEVEL_TO_LOW,
    NO_HOUSE,
    THAT_ROOM_IS_TO_SMALL,
    NOT_ENOUGH_INFLUENCE,
    SAVE_THE_GAME,
    NOT_INSIDE,
    ROOM_WRONG_SIZED,
    OBJECT_IN_ROOM,
    RELOAD_TIME,
    NEAR_WALL,
    PRESS_RIGHT_CLEAR,
    abcde,
} UIMessages ;


class TranslationMatrix
{
    public:
    
    static std::string translate(std::string key);
    
    static bool warning(UIMessages index);
    static std::map<std::string, std::string> phrases;
    
    
};



#endif /* defined(__After_Life_Empire__TranslationMatrix__) */
