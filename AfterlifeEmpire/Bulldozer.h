//
//  Bulldozer.h
//  After Life Empire
//
//  Created by Autobotika on 2015-04-10.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__Bulldozer__
#define __After_Life_Empire__Bulldozer__

#include <stdio.h>
#include "Sprite.h"

class Bulldozer : public Sprite{
    public:

    static cpVect spawnBulldozer();
    
    static std::map<long, Bulldozer*> allBulldozers;
    
    static void thinkAll();
    bool think();
    static void updateAll();
    static void renderAll();
	static void purgeAll();


    static bool test();
    int didItMove;
    

};

#endif /* defined(__After_Life_Empire__Bulldozer__) */
