//
//  Pathfinding.h
//  After Life Empire
//
//  Created by Autobotika on 2014-11-02.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__Pathfinding__
#define __After_Life_Empire__Pathfinding__

#include <stdio.h>
#include <vector>
#include <set>
#include <map>
#include "PathTile.h"
#include "Sprite.h"





class PathFinder{
    public:
    int start;
    int end;

    int manhattenValue(int start);
    int addValue(int target, double currentDistance);
    
    std::vector<PathTile> path;
    
    Sprite * theSprite;
    void cleanPath();
    bool findPath (int start,int target);
    std::vector<PathTile> openList;
    std::vector<PathTile> closedList;
    
    std::vector<PathTile> openTiles;
    
};
#endif /* defined(__After_Life_Empire__Pathfinding__) */
