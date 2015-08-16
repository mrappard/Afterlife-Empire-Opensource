//
//  TranslationMatrix.cpp
//  After Life Empire
//
//  Created by Autobotika on 2015-04-20.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#include "TranslationMatrix.h"
#include "UserInterface.h"
#include "AudioController.h"


std::map<std::string, std::string> TranslationMatrix::phrases;

std::string TranslationMatrix::translate(std::string key){

    std::map<std::string,std::string>::iterator IT = phrases.find(key);
    
    
    if (IT==phrases.end()){
        
        std::cout << "\n\nNo Such Phrase for "<< key <<" \n\n";        
        return "No Such Phrase for " + key;
    
    } else {
        return IT->second;
    }


}

bool TranslationMatrix::warning(UIMessages index){
    
    
    if (index==NOT_ENOUGH_ENERGY){
        UserInterface::spawnTextBox(translate("NOT_ENOUGH_ENERGY"), 1.0,0);
        AudioController::playAudio("Interface_Cancel.wav");
        return true;
    }else if (index==SPACE_OCCUPIED){
    
         UserInterface::spawnTextBox(translate("SPACE_OCCUPIED"), 1.0,0);
        AudioController::playAudio("Interface_Cancel.wav");
        return true;
    } else if (index == LEVEL_TO_LOW){
    
         UserInterface::spawnTextBox(translate("LEVEL_TO_LOW"), 1.0,0);
        AudioController::playAudio("Interface_Cancel.wav");
    } else if (index == NO_HOUSE){
    
        UserInterface::spawnTextBox(translate("NO_HOUSE"), 1.0,0);
        AudioController::playAudio("Interface_Cancel.wav");
    
    } else if (index == THAT_ROOM_IS_TO_SMALL){
        UserInterface::spawnTextBox(translate("THAT_ROOM_IS_TO_SMALL"), 1.0,0);
        AudioController::playAudio("Interface_Cancel.wav");
    } else if (index == NOT_ENOUGH_INFLUENCE){
    
        UserInterface::spawnTextBox(translate("NOT_ENOUGH_INFLUENCE"), 1.0,0);
        AudioController::playAudio("Interface_Cancel.wav");
    } else if (index == SAVE_THE_GAME){
        
        UserInterface::spawnTextBox(translate("SAVE_THE_GAME"), 1.0,0);
        
        AudioController::playAudio("Interface_WellDone.wav");
        
    } else if (index ==NOT_INSIDE){
        UserInterface::spawnTextBox(translate("NOT_INSIDE"), 1.0,0);
        
        AudioController::playAudio("Interface_Cancel.wav");
    
    
    } else if (index == ROOM_WRONG_SIZED){
    
        UserInterface::spawnTextBox(translate("ROOM_WRONG_SIZED"), 1.0,0);
        
        AudioController::playAudio("Interface_Cancel.wav");
    
    } else if (index == OBJECT_IN_ROOM){
    
        UserInterface::spawnTextBox(translate("ALREADY_A_TRAP_IN_ROOM"), 1.0,0);
        
        AudioController::playAudio("Interface_Cancel.wav");
    
    } else if (index == RELOAD_TIME){
    
        UserInterface::spawnTextBox(translate("PLEASE_WAIT_RELOAD_TRAP"), 1.0,0);
        
        AudioController::playAudio("Interface_Cancel.wav");
        
    } else if (index==NEAR_WALL){
    
        UserInterface::spawnTextBox(translate("NEAR_WALL"), 1.0,0);
        
        AudioController::playAudio("Interface_Cancel.wav");
    
    } else if(index == PRESS_RIGHT_CLEAR){
        
        UserInterface::spawnTextBox(translate("PRESS_RIGHT_CLEAR"), 1.0,0);
    
    }


    
    return false;

}