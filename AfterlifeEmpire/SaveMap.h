//
//  SaveMap.h
//  After Life Empire
//
//  Created by Autobotika on 2014-10-16.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__SaveMap__
#define __After_Life_Empire__SaveMap__

#include <stdio.h>
#include <map>
#include "Sprite.h"

class SaveMap{
public:

    std::map<double, Sprite*> topB;
    std::map<double, Sprite*> bottomB;
   std::map<double, Sprite*> leftB;
    std::map<double, Sprite*> rightB;
    
    
    static std::vector<std::vector<Sprite*> > spriteHolder;
    
    
    static void update(std::vector<Sprite*> * data);
    static void update();
    static void render();
    
   static void test();
    
    static cpVect findCords(int position);
    static cpVect findCords(cpVect position);
    static cpVect findCordsReverse(cpVect position);
    static cpVect findCordsTile(cpVect position);
};

#endif /* defined(__After_Life_Empire__SaveMap__) */
