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

    
    
};


#endif /* defined(__After_Life_Empire__GlobalDebugger__) */
