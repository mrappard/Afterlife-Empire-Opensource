//
//  PathTile.h
//  After Life Empire
//
//  Created by Autobotika on 2015-03-04.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef After_Life_Empire_PathTile_h
#define After_Life_Empire_PathTile_h

class PathTile{
public:
    PathTile();
    PathTile(int index);
    PathTile(int index, int parent, double distanceTravel, double distanceLeftEstimate);
    
    int index;
    int parent;
    double distanceTravel;
    double distanceLeftEstimate;
    
    
    
    friend bool operator<(PathTile const & a, PathTile const & b)
    {
        return a.distanceTravel+a.distanceLeftEstimate > b.distanceTravel+b.distanceLeftEstimate;
    }
    
};
#endif
