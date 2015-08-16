//
//  INPUT_STATES.h
//  ThanatosIpad
//
//  Created by Autobotika on 13-09-20.
//  Copyright (c) 2013 Autobotika. All rights reserved.
//

#ifndef ThanatosIpad_INPUT_STATES_h
#define ThanatosIpad_INPUT_STATES_h


enum MOUSE_STATES{
    MOUSE_STATE_TOUCH,
    MOUSE_STATE_DRAG,
    MOUSE_STATE_UP,
    MOUSE_DOUBLE_TAP,
    MOUSE_STATE_HOVER,
};

enum KEY_STATES{
    KEY_DOWN,
    KEY_UP

};

#define MOUSE_L -1
#define MOUSE_R -2
#define MOUSE_C -3
#define MOUSE_NONE -4


#endif