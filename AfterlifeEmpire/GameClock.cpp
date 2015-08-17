//
//  GameClock.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-11-26.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#include "GameClock.h"
#include <map>
#include <vector>
#include <string>

#if defined(__APPLE__ )
#include <sys/time.h>
#include "FileManager.h"
#endif



long GameClock::time=0;





bool GameClock::checkTime(long checkTime, long delay){

    return time<checkTime+delay;



}

#if defined(__APPLE__ )
timeval startTime;
#endif


#if defined(_WIN32)
#include <windows.h>
LARGE_INTEGER startTime;
double freValue;

#endif

void GameClock::startStopWatch(){
    
    #if defined(__APPLE__ )
    gettimeofday(&startTime, NULL);
    #endif

	#if defined(_WIN32)
	QueryPerformanceCounter(&startTime);
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	freValue = double(frequency.QuadPart) / 1000;
	#endif
}


double GameClock::checkStopWatch(){
    
#if defined(__APPLE__ )
    timeval currentTime;
    
    gettimeofday(&currentTime, NULL);
    
    double startTimeD = double(startTime.tv_sec)*1000;
    startTimeD += (startTime.tv_usec)/1000;
    
    double currentTimeD = double(currentTime.tv_sec)*1000;
    currentTimeD += (currentTime.tv_usec)/1000;
    
    
    return (currentTimeD-startTimeD);
    
#endif
    
#if defined(_WIN32)
    
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    
    
    double currentTimeF = currentTime.QuadPart;
    double startTimeF = startTime.QuadPart;
    
    double checkthis = double(currentTimeF - startTimeF) / freValue;
    
    
    return (checkthis);
#endif
    
}

bool GameClock::checkStopWatch(double amount){
    
    
#if defined(__APPLE__ )
    timeval currentTime;

    gettimeofday(&currentTime, NULL);
    
    double startTimeD = double(startTime.tv_sec)*1000;
    startTimeD += (startTime.tv_usec)/1000;
    
    double currentTimeD = double(currentTime.tv_sec)*1000;
    currentTimeD += (currentTime.tv_usec)/1000;

    
    return (startTimeD+amount>=currentTimeD);

#endif

#if defined(_WIN32)

	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);


	double currentTimeF = currentTime.QuadPart;
	double startTimeF = startTime.QuadPart;

	double checkthis = double(currentTimeF - startTimeF) / freValue;


	return (checkthis < amount);
#endif

}

#if defined(_WIN32)

static std::vector<std::string> idValue;
static std::vector<double> amount;
static std::vector<std::string> id;
static bool everProfiled = false;
static LARGE_INTEGER lastCounter;
static int index = 0;

void GameClock::profile(std::string name){

	if (!everProfiled){
		QueryPerformanceCounter(&lastCounter);
	
	}

	while (amount.size() <= index){
		amount.push_back(0);
		idValue.push_back(name);
	} 


	
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	amount[index] += double(currentTime.QuadPart - lastCounter.QuadPart) / freValue;
	

	if (currentTime.QuadPart - lastCounter.QuadPart < 0){
		printf("oops");
	}
	lastCounter = currentTime;
	index++;

}

void GameClock::resetProfile(){
	index = 0;

}


void GameClock::outPutProfile(){
	for (size_t counter = 0; counter < amount.size(); counter++){
		printf("Value %s = %f \n", idValue[counter].c_str(), amount[counter]);
	}
}
#endif


#if defined(_WIN32)

static LARGE_INTEGER startGameTime;
static LARGE_INTEGER frequencyGame;
static bool clockTimed = false;
static int renderTick=0;
static int updateTick = 0;
//QueryPerformanceCounter(&startTime);

//QueryPerformanceFrequency(&frequency);

bool GameClock::winLoop(){

	if (!clockTimed){
		QueryPerformanceFrequency(&frequencyGame);
		QueryPerformanceCounter(&startGameTime);
		renderTick = 0;
		clockTimed = true;
	}





		
		double checkTick = updateTick;
		LARGE_INTEGER currentGameTime;
		QueryPerformanceCounter(&currentGameTime);

		double checkthis  = double(currentGameTime.QuadPart - startGameTime.QuadPart) / frequencyGame.QuadPart;
		if (checkthis<((double)(updateTick)) / 60.0){
			return false;
		}

		if (checkthis>(double)(updateTick + 15.0) / 60.0){
			QueryPerformanceCounter(&startGameTime);
			updateTick=0;
			renderTick = 0;
			return true;
		}

	

		return true;
}


bool GameClock::winRender(){

	if (!clockTimed){
		QueryPerformanceFrequency(&frequencyGame);
		QueryPerformanceCounter(&startGameTime);
		renderTick = 0;
		clockTimed = true;
	}






	double checkTick = renderTick;
	LARGE_INTEGER currentGameTime;
	QueryPerformanceCounter(&currentGameTime);

	double checkthis = double(currentGameTime.QuadPart - startGameTime.QuadPart) / frequencyGame.QuadPart;
	if (checkthis<((double)(renderTick)) / 60.0){
		return false;
	}

	if (checkthis>(double)(renderTick + 15.0) / 60.0){
		QueryPerformanceCounter(&startGameTime);
		renderTick = 0;
		updateTick = 0;
		return true;
	}



	return true;
}



void GameClock::renderIncrease(){
	renderTick++;


}

void GameClock::loopIncrease(){
	updateTick++;
}
#endif