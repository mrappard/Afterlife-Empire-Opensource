//==============================================================================
#include "CApp.h"
#include "GameLogic.h"
#include "Camera.h"


//==============================================================================
void CApp::OnEvent(SDL_Event* Event) {


     CEvent::OnEvent(Event);
}



void CApp::OnResize(int w, int h)
{
	if (!goFullScreen){


		//SDL_SetVideoMode(w, h, 32, SDL_OPENGL | SDL_RESIZABLE | SDL_DOUBLEBUF);
	}

	GameLogic::setWindow(w, h);

	screenWidth = w;
	screenHeight = h;


}

void CApp::OnMButtonUp(int mX, int mY){
	
	//GameLogic::mouseInput(MOUSE_STATE_UP, getPosInView(mX, mY), MOUSE_C);
}

void CApp::OnMouseWheel(bool Up, bool Down){

	if (Up){

		Camera::changeZoom(1.0);

	}

	if (Down){
		Camera::changeZoom(-1.0);
	}



}

cpVect CApp::getPosInView(int mX, int mY){
	double amountX;
	double amountY;

	mY = screenHeight - mY;


	if (GameLogic::width > GameLogic::height){

		double width = screenWidth;
		double height = screenHeight;

		double xPos = (double)mX;
		double yPos = (double)mY;

		xPos -= width / 2.0;
		yPos -= height / 2.0;

		amountX = xPos / (width / 2.0);
		amountY = yPos / (width / 2.0);

		amountX *= 100;
		amountY *= 100;

	}
	else {

		double width = screenWidth;
		double height = screenHeight;

		double xPos = (double)mX;
		double yPos = (double)mY;

		xPos -= width / 2.0;
		yPos -= height / 2.0;

		amountX = xPos / (height / 2.0);
		amountY = yPos / (height / 2.0);

		amountX *= 100;
		amountY *= 100;

	}


	//printf("%f, %f - %i, %i \n", amountX, amountY, mX, mY);


	return cpv(amountX, amountY);

}


void CApp::OnLButtonDown(int mX, int mY) {
	GameLogic::mouseInput(MOUSE_STATE_TOUCH, getPosInView(mX, mY), MOUSE_L);

}

void CApp::OnLButtonUp(int mX, int mY){
	GameLogic::mouseInput(MOUSE_STATE_UP, getPosInView(mX, mY), MOUSE_L);
	
}


void CApp::OnRButtonDown(int mX, int mY) {

	GameLogic::mouseInput(MOUSE_STATE_TOUCH, getPosInView(mX, mY), MOUSE_R);

}

void CApp::OnRButtonUp(int mX, int mY){
	GameLogic::mouseInput(MOUSE_STATE_UP, getPosInView(mX, mY), MOUSE_R);
	
}



void  CApp::OnKeyUp(SDL_Keycode sym){
	GameLogic::keyInput(KEY_UP, sym);

}




void  CApp::OnKeyDown(SDL_Keycode sym){
	GameLogic::keyInput(KEY_DOWN, sym);
	
}












void CApp::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle){

	if (Left){

		GameLogic::mouseInput(MOUSE_STATE_DRAG, getPosInView(mX, mY), MOUSE_L);
	}
	else if (Right){

		GameLogic::mouseInput(MOUSE_STATE_DRAG, getPosInView(mX, mY), MOUSE_R);

	}
	else {

		GameLogic::mouseInput(MOUSE_STATE_HOVER, getPosInView(mX, mY), MOUSE_L);

	}

}


void CApp::OnExit() {
    Running = false;
}

