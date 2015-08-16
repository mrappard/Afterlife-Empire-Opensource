//
//  GlobalDebugger.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-10-09.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//



#include "GlobalDebugger.h"
#include "ShaderManager.h"

#ifdef _WIN32

#ifdef _DEBUG

bool GDebug::debuggingActive = true;

#else

bool GDebug::debuggingActive = false;

#endif
#else

#ifdef DEBUG

bool GDebug::debuggingActive = true;

#else

bool GDebug::debuggingActive = false;

#endif

#endif



int GDebug::value1 = 0;
int GDebug::value2 = 0;
int GDebug::value3 = 0;
int GDebug::value4 = 0;


void GDebug::changeValue(int index, int value){

    if (index==0){
        GDebug::value1+=value;
    } else if (index==1){
        GDebug::value2+=value;
    } else if (index==2){
        GDebug::value3+=value;
    } else if (index==3){
        GDebug::value4+=value;
    }

    printf("%i,%i,%i,%i\n",value1,value2,value3,value4);
}


bool GDebug::log(std::string text){

    printf("%s", text.c_str());
    return true;
};





