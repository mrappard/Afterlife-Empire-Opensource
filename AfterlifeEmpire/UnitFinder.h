//
//  UnitFinder.h
//  After Life Empire
//
//  Created by Autobotika on 2015-07-02.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef After_Life_Empire_UnitFinder_h
#define After_Life_Empire_UnitFinder_h

#include <map>
#include "openglimports.h"
#include "VecAndBB.h"
#include "Sprite.h"

class UnitFinder
{
public:
    
    UnitFinder();
    ~UnitFinder();
    
    static std::map<long, UnitFinder> allFinders;
    
    void think();
    void update();
    void render();
    
    static void thinkAll();
    static void updateAll();
    static void renderAll();
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    
    double rotation = 0.0;
    
    bool danger;
    bool renderIt;
    Sprite * aSprite;
    cpVect position;
    
    static GLuint _texture_Circle;
    
    static void findAndSetToActive(long UID);
    
    static void test();
    
    
};


#endif
