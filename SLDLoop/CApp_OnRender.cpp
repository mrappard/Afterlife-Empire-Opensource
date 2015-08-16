//==============================================================================
#include "CApp.h"
#include "GameLogic.h"
#include "openglimports.h"
//==============================================================================
void CApp::OnRender() {


	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	GameLogic::render();

	SDL_GL_SwapWindow(gWindow);
}
