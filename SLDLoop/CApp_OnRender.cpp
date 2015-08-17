//==============================================================================
#include "CApp.h"
#include "GameLogic.h"
#include "openglimports.h"
#include "GameClock.h"
//==============================================================================
void CApp::OnRender() {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GameLogic::render();
	SDL_GL_SwapWindow(gWindow);
	
	
	
}
