//
//  Pathfinding.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-11-02.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#include "Pathfinding.h"
#include "Tiles.h"
#include "Directions.h"
#include <algorithm> 


PathTile::PathTile(){}

PathTile::PathTile(int indexi){

    index = indexi;


}


PathTile::PathTile(int indexi, int parenti, double distanceTraveli, double distanceLeftEstimatei){
    distanceTravel=distanceTraveli;
    index=indexi;
    parent=parenti;
    distanceLeftEstimate=distanceLeftEstimatei;
    
    
    
    

}


int PathFinder::manhattenValue(int start){

    int xValueS=start%Tiles::width;
    int yValueS=start/Tiles::height;
    
    int xValueE=end%Tiles::width;
    int yValueE=end/Tiles::height;
    
    return abs(xValueS-xValueE)+abs(yValueS-yValueE);
    
}


int PathFinder::addValue(int target, double currentDistance){
    
    
    int valuesAdd=0;
    
  
    
    for (int counter=0; counter<4; counter++){
        int qv;
        switch (counter) {
            case L:
                qv=target-1;
                break;
            case U:
                qv=target+Tiles::width;
                break;
            case R:
                qv=target+1;
                break;
            case D:
                qv=target-Tiles::width;
                break;
            default:
                qv=0;
                break;
        }
        
		if (qv < 0 || qv >= Tiles::width*Tiles::width){
			continue;
		}
        
    
        if (closedList[qv].parent==-1){
            if(Tiles::canMove(target, counter)){
				PathTile IT = openList[qv];

                if (IT.parent==-1){
                  
                    PathTile insertMe = PathTile(qv,target,currentDistance,manhattenValue(qv));
                    
                    
                    openList[qv]=insertMe;
                    openTiles.push_back(insertMe);
                    std::push_heap(openTiles.begin(), openTiles.end());
                } else {
                    if (IT.distanceTravel<currentDistance){
                        PathTile betterTile = IT;
                        openList[betterTile.index].parent =-1;
                        closedList[betterTile.index]=betterTile;
                        path.push_back(betterTile);
                    }
                }
            }
        } else{
			/*
            PathTile insertMe = closedList[qv];
            PathTile current = closedList[target];
            
            if (current<insertMe){
                
                
                
                
             
            
            }
        
        */
        
        }
        valuesAdd++;
        
    }
   
    return valuesAdd;
}


void PathFinder::cleanPath(){

    std::vector<PathTile> reversed;
    
    std::vector<PathTile>::reverse_iterator Last = path.rbegin();
    PathTile a1 = *Last;
    reversed.push_back(a1);
    
    for (std::vector<PathTile>::reverse_iterator IT = path.rbegin()+1; IT !=path.rend(); IT++){
    
        
 
        if (Last->parent == IT->index){
            Last=IT;
            reversed.push_back(*IT);
        };
     
        
    }

    path.swap(reversed);
    

}


bool PathFinder::findPath (int start,
              int target)
{

	end = target;

	{
		PathTile noTile;
		noTile.parent = -1;
		openList.assign(Tiles::width*Tiles::width, noTile);
		closedList.assign(Tiles::width*Tiles::width, noTile);
	}
    

    

    // Are you already at the location;
    if (start == target){
         return false;
    }
    
    int currentPosition = start;
    double currentDistance=0;
    
    ;
    int working=true;
    
    int counter = 0;
    
    
 
    
    bool checkOnce = false;
    while (working) {

        addValue(currentPosition, currentDistance+1);
        
        if (openTiles.size()==0){
            return false;
        }
        counter++;
        PathTile front = openTiles.front();
        
        //Pop the front;
        std::pop_heap(openTiles.begin(), openTiles.end()); openTiles.pop_back();
        
        openList[front.index].parent=-1;
        closedList[front.index]=front;
        
        currentPosition = front.index;
        
        currentDistance = front.distanceTravel;
        path.push_back(front);
        
        if (front.index==target){
            working=false;
        }
        

        if (openTiles.size()==0){
            if (checkOnce==true){
                path.clear();
                return false ;
            
            }
            
            
            checkOnce=true;
            continue;
            
            
            //working = false;
          
        }
        
        checkOnce = false;
        
        
        
        
        
    }
    
    cleanPath();
    
    
    
     return true;
    
}
