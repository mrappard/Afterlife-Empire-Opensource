// AfterlifeEmpire.cpp : Defines the entry point for the application.
//


#include <SDL.h>
#include <stdio.h>
#include "CApp.h"
#include "FileManager.h"





CApp::CApp() {
	Surf_Display = NULL;

	Running = true;
}

int CApp::OnExecute() {
	if (OnInit() == false) {
		return -1;
	}


	SDL_Event Event;



	while (Running) {



		while (SDL_PollEvent(&Event)) {
			OnEvent(&Event);
		}


		OnLoop();
		OnRender();




		if (shutDown()){
			Running = false;
		}



	}

	OnCleanup();

	return 0;
}

#if defined(STEAM)



#include "GlobalDebugger.h"
#include "steam_api.h"
#include "GlobalDebugger.h"
#include "Camera.h"
#include "Steam_API_Access.h"





void MiniDumpFunction(unsigned int nExceptionCode, EXCEPTION_POINTERS *pException)
{
	// You can build and set an arbitrary comment to embed in the minidump here,
	// maybe you want to put what level the user was playing, how many players on the server,
	// how much memory is free, etc...

	char holdString[900];

	
	sprintf_s(holdString, "Mode %i, CS %i, LF %i, MS %i, MX %f, MY %f, CX %f, CY %f, CZ %f", GDebug::gameMode, GDebug::currentGameState, GDebug::lastFunction, GDebug::mouseState, GDebug::mouseInputX, GDebug::mouseInputY, Camera::posX, Camera::posY, Camera::zoom );
	SteamAPI_SetMiniDumpComment(holdString);

	// The 0 here is a build ID, we don't set it





	SteamAPI_WriteMiniDump(nExceptionCode, pException, 367710);
}
#endif



int main(int argc, char* args[])
{

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}

#if defined(STEAM)
	int didItWork = steamInit();

	if (didItWork){
		return 0;
	}

#endif






	CApp theApp;

	int value;


	for (int counter = 0; counter < argc; counter++){

		std::string theData = args[counter];

		if (theData == "Fullscreen"){
			theApp.goFullScreen = true;
		}
		if (theData[0] == 'w'){
			std::string theNumber = theData.substr(1, theData.size());

			int width = atoi(theNumber.c_str());
			theApp.resolutionWidth = width;
		}

		if (theData[0] == 'h'){
			std::string theNumber = theData.substr(1, theData.size());

			int height = atoi(theNumber.c_str());
			theApp.resolutionHeight = height;
		}

}



#if defined(STEAM)
	_set_se_translator(MiniDumpFunction);
#endif

	try  // this try block allows the SE translator to work
	{
		value = theApp.OnExecute();
		//steamEnd();
		return value;
	}
	catch (...)
	{
		return -1;
	}


	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}


