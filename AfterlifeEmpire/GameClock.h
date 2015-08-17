//
//  GameClock.h
//  After Life Empire
//
//  Created by Autobotika on 2014-11-26.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__GameClock__
#define __After_Life_Empire__GameClock__

#include <stdio.h>
#include <string>



class GameClock
{
public:
    
    static long time;
    static bool checkTime(long checkTime, long delay);
    
    static void startStopWatch();
    static bool checkStopWatch(double amount);
    static double checkStopWatch();
    
	static void profile(std::string);
	static void outPutProfile();
	static void resetProfile();
    


	static bool winLoop();
	static bool winRender();
	static void renderIncrease();
	static void loopIncrease();
};

#endif /* defined(__After_Life_Empire__GameClock__) */