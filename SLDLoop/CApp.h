
#include <stdio.h>
#include <math.h>
#include <vector>



#include "CEvent.h"

#include <windows.h>
#include "VecAndBB.h"


//==============================================================================
class CApp : public CEvent {
    private:
        bool            Running;

        SDL_Surface*    Surf_Display;

        SDL_Surface*    Surf_Test;
		SDL_Window * gWindow;
		SDL_GLContext gContext;


    public:

        CApp();

        int OnExecute();

		int screenWidth;
		int screenHeight;
					
    public:
        bool OnInit();

        void OnEvent(SDL_Event* Event);

		bool goFullScreen;
		int resolutionWidth;
		int resolutionHeight;
		cpVect getPosInView(int mX, int mY);
	  void OnResize(int w, int h);
	   void OnMButtonUp(int mX, int mY);
	   void OnLButtonUp(int mX, int mY);
       void OnLButtonDown(int mX, int mY);

	   void OnMouseWheel(bool Up, bool Down);

	   void OnRButtonUp(int mX, int mY);
       void OnRButtonDown(int mX, int mY);

        void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
		void OnKeyDown(SDL_Keycode sym);
		void OnKeyUp(SDL_Keycode sym);
		
        void OnExit();

        void OnLoop();


        void OnRender();

        void OnCleanup();




	

		void chipmunkTest();

};
