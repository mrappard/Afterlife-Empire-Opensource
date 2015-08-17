//==============================================================================

#include "CApp.h"
#include "GameLogic.h"
#include "GameClock.h"
#include "FileManager.h"
#include "SDL_syswm.h"
#include "SDL_Image.h"
#include "openglimports.h"
//==============================================================================
bool CApp::OnInit() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}

	screenWidth = 640;
	screenHeight = 480;




	GameLogic::width = 640;
	GameLogic::height = 480;


	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);


	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);



	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create context



	//Create window
	//

	if (!goFullScreen){
		gWindow = SDL_CreateWindow("Afterlife Empire", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GameLogic::width, GameLogic::height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}
		if (
			SDL_Surface *surface = IMG_Load(findFile("Icon.png").c_str())
			){
			SDL_SetWindowIcon(gWindow, surface);


		}
	}
	else {

		gWindow = SDL_CreateWindow("Afterlife Empire",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			resolutionWidth, resolutionHeight,
			SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

		GameLogic::width = 1920;
		GameLogic::height = 1080;
		screenWidth = 1920;
		screenHeight = 1080;

	}
	gContext = SDL_GL_CreateContext(gWindow);
	if (gContext == NULL)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());

	}
	else
	{
		//Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK)
		{
			printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}


	}




	glEnable(GL_MULTISAMPLE);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);


	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);

	glViewport(0, 0, GameLogic::width, GameLogic::height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, 512, 512, 0, 1, -1);

	glMatrixMode(GL_MODELVIEW);

	glPointSize(10.0); // 10 pixel dot!











	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);



	SDL_GL_SwapWindow(gWindow);





	GameLogic::start();
	GameClock::startStopWatch();

	GameLogic::setWindow(screenWidth, screenHeight);


	return true;

}

//==============================================================================


