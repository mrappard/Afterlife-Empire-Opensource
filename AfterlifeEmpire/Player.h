//
//  Player.h
//  After Life Empire
//
//  Created by Autobotika on 2015-01-27.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__Player__
#define __After_Life_Empire__Player__

#include <stdio.h>

#include "Sprite.h"
#include "BitmapFontClass.h"

class PlayIcons{
    public:
    int cost;

    GLuint * texture;
    GLfloat vertex[8];
    GLfloat cords[4];
};






class Player : public Sprite
{
    public:
    Player();
    ~Player();
    
    static int playerPosition;

    static Player * mainPlayer;
    int playerState;
    
    void init();
    void think();
    void update();
    void update(cpVect pos, double rotation);
    void render();

    void renderAlpha();
    static std::vector<ScareDict> scareDict;
    
    bool mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber);
    bool loadDict(std::string * path);
    
    

      
    static void scareAbilityButton(int index);
    void scareAbility(int index);
    
    static void scareAbilityDrawRadiusButton(int index);
    void scareAbilityDrawRadius(int index);

    
    static void scareRadius(bool onOrOff);
    static void laughRadius(bool onOrOff);
    
    void scare();
    void laugh();
    void possession();
    void twirl();
    void replicate();
    
    void movePlayer(cpVect thePos);
    
    static bool scareRadiusCheck;
    static bool laughRadiusCheck;
    
    static void showPath(int path);
    static std::vector<int> returnPath();
    
    static std::string getDescription(int index);
    
    void changeBackToNormalMode();
    
    static bool UItiles(int startIndex, int numberOfTiles,
                        std::vector<PlayIcons> * dict, bool propOrNot, bool animate);

};


#endif /* defined(__After_Life_Empire__Player__) */
