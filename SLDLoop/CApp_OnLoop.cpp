//==============================================================================

#include "CApp.h"
#include "GameLogic.h"
#include "GameClock.h"

#include "GlobalDebugger.h"

long tick = 0;
//==============================================================================
void CApp::OnLoop() {

	

	if (GameClock::winLoop()){

	GameLogic::tick(tick++);
	GameLogic::update();
	GameClock::loopIncrease();
	}
}

//==============================================================================
