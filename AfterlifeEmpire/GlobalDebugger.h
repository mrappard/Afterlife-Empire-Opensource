//
//  GlobalDebugger.h
//  After Life Empire
//
//  Created by Autobotika on 2014-10-09.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__GlobalDebugger__
#define __After_Life_Empire__GlobalDebugger__

#include <stdio.h>
#include <string>


#define DEBUG_MOUSE_STATE       1
#define DEBUG_UPDATE_STATE      2
#define DEBUG_RENDER_STATE      3
#define DEBUG_TICK_STATE        4
#define DEBUG_INIT_STATE        5


#define  DEBUG_AUDIO_PLAY       50
#define  DEBUG_AUDIO_LOOP       51
#define  DEBUG_AUDIO_STOP       52
#define  DEBUG_AUDIO_LOAD       53
#define  DEBUG_AUDIO_GET_NOT_PLAY_MUSIC 54
#define  DEBUG_AUDIO_GET_NOT_PLAY    54

#define DEBUG_RENDER_TILE_1     1001
#define DEBUG_RENDER_TILE_2     1002
#define DEBUG_RENDER_TILE_3     1003
#define DEBUG_RENDER_TILE_4     1004
#define DEBUG_RENDER_TILE_5     1005
#define DEBUG_RENDER_TILE_6     1006
#define DEBUG_RENDER_TILE_7     1007
#define DEBUG_RENDER_TILE_8     1008
#define DEBUG_RENDER_TILE_9     1009
#define DEBUG_RENDER_TILE_10    1010
#define DEBUG_RENDER_TILE_11    1011
#define DEBUG_RENDER_TILE_12    1012
#define DEBUG_RENDER_TILE_13    1013
#define DEBUG_RENDER_TILE_14    1014
#define DEBUG_RENDER_TILE_15    1015

#define DEBUG_BOB_STATE_MOUSE   2000
#define DEBUG_BOB_RETURN_NORMAL 2001
#define DEBUG_BOB_THINK         2002
#define DEBUG_BOB_THINK_LOGIC   2003
#define DEBUG_BOB_CHANGE_TUTORIAL_STATE 2004
#define DEBUG_BOB_UPDATE 2005
#define DEBUG_BOB_RENDER 2006

#define DEBUG_UPDATE_STAGE_1    3001
#define DEBUG_UPDATE_STAGE_2    3002
#define DEBUG_UPDATE_STAGE_3    3003
#define DEBUG_UPDATE_STAGE_4    3004
#define DEBUG_UPDATE_STAGE_5    3005
#define DEBUG_UPDATE_STAGE_6    3006
#define DEBUG_UPDATE_STAGE_7    3007
#define DEBUG_UPDATE_STAGE_8    3008
#define DEBUG_UPDATE_STAGE_9    3009
#define DEBUG_UPDATE_STAGE_10   3010
#define DEBUG_UPDATE_STAGE_11   3011
#define DEBUG_UPDATE_STAGE_12   3012
#define DEBUG_UPDATE_STAGE_13   3013
#define DEBUG_UPDATE_LOAD_ALL_TEXTURES 3014
#define DEBUG_UPDATE_CREDIT_MODE 3015
#define DEBUG_UPDATE_CREDIT_MODE_TEXT 3016
#define DEBUG_UPDATE_CREDIT_HELP_TEXT 3017

#define DEBUG_THINK_BOB             4000
#define DEBUG_THINK_WAIT_BOB        4001
#define DEBUG_THINK_AUDIO           4003
#define DEBUG_THINK_UNIT_FINDER     4004
#define DEBUG_THINK_USERINTERFACE   4005
#define DEBUG_THINK_SPRITE          4006
#define DEBUG_THINK_DIALOG          4007
#define DEBUG_THINK_PLAYER          4008
#define DEBUG_THINK_DOOR            4009
#define DEBUG_THINK_BULLDOZER       4010
#define DEBUG_THINK_DRAIN           4011

class GDebug
{
public:
    static int value1;
    static int value2;
    static int value3;
    static int value4;
    
    static bool debuggingActive;
    
    static void changeValue(int index, int value);
    
    static bool log(std::string);
    
    
    static int currentGameState;
    static int lastFunction;
    static double mouseInputX;
    static double mouseInputY;
    static int mouseState;
    static int gameMode;
    static std::string textSave;

    
    
};


#endif /* defined(__After_Life_Empire__GlobalDebugger__) */
