//
//  Sprite.cpp
//  ThanatosIpad
//
//  Created by Autobotika on 13-09-20.
//  Copyright (c) 2013 Autobotika. All rights reserved.
//

#include "Sprite.h"

#include "TextureGenerator.h"
#include "FileManager.h"
#include "GlobalDebugger.h"
#include "ShaderManager.h"
#include "Tiles.h"
#include "Camera.h"


//Hack For Cocoa to get Boost to Work

#undef check
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "GameClock.h"

#include "GameLogic.h"
#include "DebugTools.h"
#include "UserInterface.h"
#include "Pathfinding.h"
#include "AudioController.h"

#include "GameClock.h"

#include <algorithm>

#include "TranslationMatrix.h"
#include "Player.h"


#include "SaveObject.h"
#include "BobZombie.h"

#include "TranslationMatrix.h"

#include "UnitFinder.h"
#include "SetupVars.h"

#include "Achievements.h"

#include "ModManager.h"

std::set<int> Sprite::trees;

bool Sprite::canMove = false;

size_t Sprite::changeSpriteType = -1;



std::map<size_t, Sprite*> Sprite::spriteByID;
std::map<int, SpriteDict*> Sprite::theSpriteAssets;




std::vector<Sprite> Sprite::selectorSprites;


std::set<Sprite*> Sprite::graveYard;
Sprite * Sprite::mouse = 0;

Sprite * Sprite::testSprite=0;
SpriteDict * Sprite::debugDict = 0;

bool Sprite::spriteCostToMuch = false;

#define SCREAM_FRAME 8
#define DIE_FRAME 9
#define IDLE_FRAME 10

long Sprite::UIDCounter;

static cpVect DebugPosition;

int Sprite::dontRenderTest=0;

bool Sprite::overSpriteForRadius=false;

bool Sprite::ShouldFlip=false;


std::map<size_t, Sprite*> Sprite::trapsByUID;
std::map<size_t, Sprite*>  Sprite::peopleByUID;
std::map<size_t, Sprite*>  Sprite::propsByUID;


std::vector<GLuint*> Sprite::traitTexture;


std::set<Sprite*>    Sprite::goodPeople;
std::set<Sprite*>    Sprite::badPeople;
std::set<Sprite*>    Sprite::authorites;

ScareType::ScareType(bool working){
    name="FAILED";
    scareModifier = 0;
    damageModifier = 0;
    awarenessModifier = 0;
}
    
    
    

ScareType::ScareType(){
    
    name = "";
    
    scareModifier = 0;
    damageModifier = 0;
    awarenessModifier = 0;
    
}
ScareType::ScareType(std::string _name, double scare, double damage, double awareness){
    
    name=_name;
    
    scareModifier=scare;
    damageModifier=damage;
    awarenessModifier=awareness;
    
}

ScareType::~ScareType(){
    
    
}


void Sprite::spawnTestCiv(){
    
    if (!Sprite::changeSpriteType){
        return;
        
    }
    Sprite * theSprite = addSprite(1212,(int)Sprite::changeSpriteType);
    
    PathTile aTile;
    
    std::vector<PathTile> fPath;
    
    aTile.index=1212;
    
    for (int counter = 0; counter<8; counter++){
        aTile.index += Tiles::width;
        fPath.push_back(aTile);
    }
    for (int counter = 0; counter<8; counter++){
        aTile.index += 1;
        fPath.push_back(aTile);
    }
    for (int counter = 0; counter<8; counter++){
        aTile.index -= Tiles::width;
        fPath.push_back(aTile);
    }
    for (int counter = 0; counter<8; counter++){
        aTile.index -= 1;
        fPath.push_back(aTile);
    }
    
    
    while (fPath.size()>0) {
        theSprite->path.push_back(fPath.back());
        fPath.pop_back();
        
    }
    
    
    
    theSprite->pathThink=999999999;
    
    
    
}


void Sprite::keyDebug(int keyCode){
    
    
   
    
    if (!debugDict) {
        return;
    }
    
    
    
    
    
    if (GameLogic::gameMode==SPRITEMODE||GameLogic::gameMode==PROPMODE||GameLogic::gameMode==TRAPMODE){
        if (keyCode == 126){ //For UP key
            
            debugDict->sludgeY+=0.125;
        } else if (keyCode == 125){ //For DOWN key
            debugDict->sludgeY-=0.125;
        } else if (keyCode==124){ //RIGHT
           
            debugDict->sludgeX+=0.125;
        } else if (keyCode==123){ //LEFT
          
            debugDict->sludgeX-=0.125;
        } else if (keyCode == 41){ //For ; key
            debugDict->width-=0.1;
        } else if (keyCode == 39){ //For ' key
            debugDict->width+=0.1;
            
        } else if (keyCode == 43){ //For ; key
            debugDict->height-=0.1;
        } else if (keyCode == 47){ //For ' key
            debugDict->height+=0.1;
            
        }
        
        char holdString[600];
        
        
        sprintf(holdString, "Name - %s, X = %f, Y = %f, W = %f, H =%f",debugDict->name.c_str(), debugDict->sludgeX, debugDict->sludgeY, debugDict->width, debugDict->height);
        
        /*
         printf("Name - %s, X = %f, Y = %f, W = %f, H =%f",debugDict->name.c_str(), debugDict->sludgeX, debugDict->sludgeY, debugDict->width, debugDict->height);
         */
        
        UserInterface::spawnTextBox(holdString, TEXTBOX_STAY_UP_FOREVER, debugDict);
    
    
    }
    
  
    
    
    
    
    /*
    
    if (GameLogic::gameMode==SPRITEMODE||GameLogic::gameMode==PROPMODE||GameLogic::gameMode==TRAPMODE){
        if (keyCode == 126){ //For UP key
            
            debugDict->sludgeY+=0.125;
        } else if (keyCode == 125){ //For DOWN key
            debugDict->sludgeY-=0.125;
        } else if (keyCode==124){ //RIGHT
            debugDict->sludgeX+=0.125;
        } else if (keyCode==123){ //LEFT
            debugDict->sludgeX-=0.125;
        } else if (keyCode == 41){ //For ; key
            debugDict->width-=0.1;
        } else if (keyCode == 39){ //For ' key
            debugDict->width+=0.1;
            
        } else if (keyCode == 43){ //For ; key
            debugDict->height-=0.1;
        } else if (keyCode == 47){ //For ' key
            debugDict->height+=0.1;
            
        }
        
        char holdString[600];
        
        
        sprintf(holdString, "Name - %s, X = %f, Y = %f, W = %f, H =%f",debugDict->name.c_str(), debugDict->sludgeX, debugDict->sludgeY, debugDict->width, debugDict->height);
        
     
        UserInterface::spawnTextBox(holdString, TEXTBOX_STAY_UP_FOREVER, debugDict);
        
        
    }
    
    */
    
    
}




















enum JSON_STRINGS{
    
    ENUM_NAME,
    ENUM_INDEX,
    ENUM_ENERGY,
    ENUM_RADIUS,
    ENUM_REPREQUIRED,
    ENUM_ICONOFFSET,
    ENUM_AUDIO,
    ENUM_SCAREMODIFIERS,
    ENUM_DAMAGE,
    ENUM_AWARENESS,
    ENUM_SCARE,
    ENUM_RELOAD,
    ENUM_SPECIAL_FACTOR,
    
    
    ENUM_FRAME,
    ENUM_SPRITESOURCESIZE,
    ENUM_SOURCESIZE,
    ENUM_TRIMMED,
    ENUM_ROTATED,
    
    ENUM_X,
    ENUM_Y,
    ENUM_W,
    ENUM_H,
    
    
    ENUM_BOB_TEXT,
    
    
};

std::map<std::string, JSON_STRINGS> jsonMap;

void buildMap(){
    
    if (jsonMap.size()!=0){return;}
    
    
    jsonMap["Name"]=ENUM_NAME;
    jsonMap["Index"]=ENUM_INDEX;
    jsonMap["EnergyReq"]=ENUM_ENERGY;
    jsonMap["Radius"]= ENUM_RADIUS;
    
    jsonMap["RepRequired"]=ENUM_REPREQUIRED;
    jsonMap["IconOffset"]=ENUM_ICONOFFSET;
    jsonMap["Audio"]=ENUM_AUDIO;
    jsonMap["ScareModifiers"]=ENUM_SCAREMODIFIERS;
    jsonMap["Damage"]= ENUM_DAMAGE;
    jsonMap["Awareness"]= ENUM_AWARENESS;
    jsonMap["Scare"]=ENUM_SCARE;
    jsonMap["Name"]=ENUM_NAME;
    jsonMap["Reload"]=ENUM_RELOAD;
    
    
    jsonMap["frame"]=ENUM_FRAME;
    jsonMap["spriteSourceSize"]=ENUM_SPRITESOURCESIZE;
    jsonMap["sourceSize"]=ENUM_SOURCESIZE;
    jsonMap["rotated"]=ENUM_ROTATED;
    jsonMap["trimmed"]=ENUM_TRIMMED;
    
    jsonMap["SpecialFactor"]=ENUM_SPECIAL_FACTOR;
    jsonMap["BobText"]=ENUM_BOB_TEXT;
    
    
    jsonMap["x"]=ENUM_X;
    jsonMap["y"]=ENUM_Y;
    jsonMap["w"]=ENUM_W;
    jsonMap["h"]=ENUM_H;
    
    
}

bool Sprite::parseJsonFrame(SpriteDict * dict){
    
    
    
    dict->textureArray.clear();
    dict->offsetArray.clear();
    
    
    std::string jsonLocation = findFile(dict->jsonName);
    
    std::ifstream f;
    f.open(jsonLocation.c_str());
    
    
    
    
    
    
    buildMap();
    
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::json_parser::read_json(f,pt);
    }
    catch (std::exception const& e)
    {
        
        std::string error = "There is an issues with the Sprite Texture Info ";
        error = error.append(dict->name);
        error = error.append(" - ");
        error = error.append(e.what());
        
        
        DebugTools::spawnErrorWindow(error);
        
        quitTheGame();
        
        return false;
    }
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type const& v, pt.get_child("meta")){
        //		BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
        
        std::string keyString = v.first.data();
        
        
        
        if (keyString=="size"){
            
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, v.second){
                
                std::string keyString = val2.first.data();
                if (keyString=="w"){
                    
                    dict->textureWidth = (GLfloat)atof(val2.second.data().c_str());
                }
                else if (keyString=="h"){
                    {
                        dict->textureHeight = (GLfloat)atof(val2.second.data().c_str());
                    }
                    
                    
                    break;
                }
            }
            
        }
    }
    
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type const& v, pt.get_child("frames"))
    {
        
        GLfloat frame_xywh[4];
        GLfloat sprite_xywh[4];
        GLfloat source_wh[2];
        
        
        BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
            
            std::string keyString = val.first.data();
            
            //int key = translate[keyString];
            
            JSON_STRINGS key = jsonMap[keyString];
            
            switch (key) {
                case ENUM_FRAME:{
                    
                    
                    
                    
                    BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                        
                        std::string keyString = val2.first.data();
                        
                        if (keyString=="x"){
                            frame_xywh[0]=(GLfloat)atof(val2.second.data().c_str());
                        } else if (keyString=="y"){
                            frame_xywh[1]=(GLfloat)atof(val2.second.data().c_str());
                        } else if (keyString=="w"){
                            frame_xywh[2]=(GLfloat)atof(val2.second.data().c_str());
                        } else if (keyString=="h"){
                            frame_xywh[3]=(GLfloat)atof(val2.second.data().c_str());
                            
                        }
                    }
                    
                }
                    break;
                case ENUM_SPRITESOURCESIZE:
                    
                {
                    
                    BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                        std::string keyString = val2.first.data();
                        
                        if (keyString=="x"){
                            sprite_xywh[0]=(GLfloat)atof(val2.second.data().c_str());
                        } else if (keyString=="y"){
                            sprite_xywh[1]=(GLfloat)atof(val2.second.data().c_str());
                        } else if (keyString=="w"){
                            sprite_xywh[2]=(GLfloat)atof(val2.second.data().c_str());
                        } else if (keyString=="h"){
                            sprite_xywh[3]=(GLfloat)atof(val2.second.data().c_str());
                        }
                    }
                    
                }
                    break;
                case ENUM_SOURCESIZE:{
                    
                    BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                        std::string keyString = val2.first.data();
                        
                        if (keyString=="w"){
                            source_wh[0]=(GLfloat)atof(val2.second.data().c_str());
                        } else if (keyString=="h"){
                            source_wh[1]=(GLfloat)atof(val2.second.data().c_str());
                        }
                    }
                }
                    break;
                case ENUM_ROTATED:{
                    
                    std::string itemData = val.second.data();
                    dict->rotated=(strncmp(itemData.c_str(),"true",4))?true:false;
                    
                }
                    break;
                case ENUM_TRIMMED:{
                    
                    
                    
                    
                    std::string itemData = val.second.data();
                    dict->trimmed=(strncmp(itemData.c_str(),"true",4))?true:false;
                    
                    
                }
                    break;
                default:
                    break;
            }
            
        }
        
        dict->textureArray.push_back(frame_xywh[0]);
        dict->textureArray.push_back(frame_xywh[1]);
        dict->textureArray.push_back(frame_xywh[2]);
        dict->textureArray.push_back(frame_xywh[3]);
        
        dict->offsetArray.push_back(sprite_xywh[0]);
        dict->offsetArray.push_back(sprite_xywh[1]);
        dict->offsetArray.push_back(sprite_xywh[2]);
        dict->offsetArray.push_back(sprite_xywh[3]);
        
        
        
        
        
        
        dict->segWidth=source_wh[0];
        dict->segHeight=source_wh[1];
        
        
    }
    
    
    
    
    return true;
}

void resolve_Doors_JSON(boost::property_tree::ptree::value_type const& v, bool original){
    
    {
        DoorDict newOne;
        newOne.init = false;
        newOne.texture = 0;
        newOne.mod = !original;
        BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second)
        {
            std::string itemName = val.first.c_str();
            std::string itemData = val.second.data();
            
            if (itemName=="Name"){
                newOne.name = itemData;
            } else if (itemName=="PlacementType"){
                if (itemData=="2X2"){
                    newOne.oneOrTwo=true;
                }else {
                    newOne.oneOrTwo=false;
                }
                
            } else if (itemName=="TextureName"){
                newOne.textureName = itemData;
            } else if (itemName=="JsonName"){
                newOne.jsonName = itemData;
            } else if (itemName=="BrainName"){
                newOne.brainName= itemData;
            } else if (itemName=="Index"){
                newOne.index = atoi(itemData.c_str());
            } else if (itemName=="Width"){
                newOne.width = atof(itemData.c_str());
            } else if (itemName=="Height"){
                newOne.height = atof(itemData.c_str());
            } else if (itemName=="Animates"){
                newOne.animates=(itemData=="true")?true:false;
                
            } else if (itemName=="Prop"){
                
                newOne.prop=(itemData=="true")?true:false;
                
                
            } else if (itemName=="Frames"){
                
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    
                    std::string itemName = val2.first.c_str();
                    std::string itemData = val2.second.data();
                    
                    newOne.frames.push_back(atoi(itemData.c_str()));
                    
                    
                }
                
                
                
            }  else if (itemName=="SludgeX_L"){
                newOne.sludgeX_L=atof(itemData.c_str());
            } else if (itemName=="SludgeY_L"){
                newOne.sludgeY_L=atof(itemData.c_str());
                
                
            } else if (itemName=="SludgeX_R"){
                newOne.sludgeX_R=atof(itemData.c_str());
            } else if (itemName=="SludgeY_R"){
                newOne.sludgeY_R=atof(itemData.c_str());
                
                
            }
            
            
            
        }
        
        
        if (Door::theDoorAssets.find(newOne.index)!=Door::theDoorAssets.end()){
            
            DoorDict * oldOne = Door::theDoorAssets[newOne.index];
            newOne.texture = oldOne->texture;
            
            *Door::theDoorAssets[newOne.index]=newOne;
            newOne.init = false;
            
        } else {
            DoorDict * newDict = new DoorDict;
            *newDict = newOne;
            newOne.init = false;
            
            
            Door::theDoorAssets[newOne.index]=newDict;
            if (!original){
                ModManager::modDoor.push_back(newDict);
            
            }
            
            
            
        }
        
    }


}

void resolve_Tiles_JSON(boost::property_tree::ptree::value_type const& v, bool original)
{
    TileDict newOne;
    newOne.init = false;
    newOne.texture = 0;
    newOne.textureL = 0;
    newOne.textureR = 0;
    newOne.textureC = 0;
    newOne.checker = false;
    
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second)
    {
        std::string itemName = val.first.c_str();
        std::string itemData = val.second.data();
        
        if (itemName=="Name"){
            newOne.name=itemData;
        } else if (itemName=="Image"){
            newOne.image=itemData;;
        } else if (itemName=="Block"){
            if (itemData=="true"){
                newOne.block=true;
            }else{
                newOne.block=false;
            }
        } else if (itemName=="Index"){
            newOne.index = atoi(itemData.c_str());
        } else if (itemName=="Walls"){
            if (itemData=="true"){
                newOne.walls=true;
            }else{
                newOne.walls=false;
            }
        } else if (itemName=="ImageL"){
            newOne.imageL=itemData;
        } else if (itemName=="ImageR"){
            newOne.imageR=itemData;
        } else if (itemName=="RepRequired"){
            newOne.requiredLevel = atoi(itemData.c_str());
        }else if (itemName=="EnergyRequired"){
            
            newOne.energyRequired = atoi(itemData.c_str());
            
            
            
        } else if (itemName=="Checker"){
            newOne.checkered = itemData;
            newOne.checker = true;
        }
    }
    
    newOne.mod = !original;
    
    TileDict * pointer = new TileDict;
    *pointer = newOne;
    
    Tiles::tileDictionary[newOne.index]=pointer;
    
    if (!original){
        ModManager::modTile.push_back(pointer);
    }
    
}
void resolve_Trees_JSON(boost::property_tree::ptree::value_type const& v)
{   std::string itemName = v.first.c_str();
    std::string itemData = v.second.data();
    
    
    Sprite::trees.insert(atoi(itemData.c_str()));
}

void resolve_Sprites_JSON(boost::property_tree::ptree::value_type const& v, bool original){
    
    {
        SpriteDict newOne;
        newOne.init = false;
        newOne.texture = 0;
        newOne.health = 100;
        newOne.cost = 0;
        newOne.scareTypeByName["Default"]= ScareType("Default",1.0,1.0,1.0);
        newOne.placementType = OneXOne;
        newOne.idleNumber = 60;
        newOne.tileGen = -1;
        newOne.dialog["Fine"]="I Am Fine.";
        newOne.dialog["Feared"]="I Am Filled With Fear.";
        newOne.dialog["Scared"]="I Am Scared";
        newOne.leaveHappyRep=-10;
        newOne.leaveFearRep=5;
        newOne.leaveScaredRep=20;
        newOne.repLevelRequired = 0;
        newOne.activationEnergy = 5;
        newOne.textureGen = false;
        newOne.bobText = "Not much to say about them.";
        newOne.spritesSpawned = std::map<float, int> ();
        newOne.trait = NORMAL_CITIZEN;
        newOne.mod = true;
        std::map<std::string, std::set<std::string> > addSoundData;
        
        
        
        BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second)
        {
            std::string itemName = val.first.c_str();
            std::string itemData = val.second.data();
            
            if (itemName=="Name"){
                newOne.name = itemData;
            }else if (itemName=="SpritesSpawned"){
                std::vector<std::string> keys;
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    std::string itemName = val2.first.c_str();
                    std::string itemData = val2.second.data();
                    keys.push_back(itemData);
                }
                
                for (size_t counter = 0; counter<keys.size(); counter+=2){
                    float percentage = atof(keys[counter].c_str());
                    int index = atoi(keys[counter+1].c_str());
                    
                    newOne.spritesSpawned[percentage]=index;
                    
                    
                }
            } else if (itemName=="Trait"){
                
                if (itemData=="POLICE"){
                    newOne.trait=POLICE;
                } else if (itemData=="NORMAL_CITIZEN"){
                    newOne.trait=NORMAL_CITIZEN;
                    
                } else if (itemData=="EXORCIST"){
                    
                    newOne.trait=EXORCIST;
                    
                } else if (itemData=="GOTH"){
                    
                    newOne.trait=GOTH;
                    
                } else if (itemData=="COUPLE"){
                    newOne.trait = COUPLE;
                    
                } else if (itemData=="REPORTER"){
                    newOne.trait = REPORTER;
                } else if (itemData=="SCAREDYCAT"){
                    
                    newOne.trait = SCAREDYCAT;
                }else if (itemData=="GAMER"){
                    newOne.trait = GAMER;
                } else if (itemData=="VIVIAN"){
                    newOne.trait = VIVIAN;
                } else if (itemData=="RENOVATOR"){
                    newOne.trait = RENOVATOR;
                } else if (itemData == "SKEPTIC"){
                    newOne.trait = SKEPTIC;
                }else if (itemData == "ZOMBIE"){
                    
                    newOne.trait = ZOMBIE;
                } else if (itemData == "GILDA"){
                    
                    newOne.trait = GILDA;
                } else if (itemData =="DOCTOR"){
                    
                    newOne.trait = DOCTOR;
                }else if (itemData == "HERO"){
                    
                    newOne.trait = HERO;
                }else if (itemData == "GHOSTHUNTER"){
                    
                    newOne.trait = GHOSTHUNTER;
                }else if (itemData == "INVESTIGATOR"){
                    
                    newOne.trait = INVESTIGATOR;
                } else if (itemData == "NINJA"){
                    
                    newOne.trait = NINJA;
                } else if (itemData == "IDIOT"){
                    
                    newOne.trait = IDIOT;
                    
                }else {
                    std::string errorText;
                    
                    errorText = "There is an undefined Trait ";
                    errorText = errorText.append(itemData);
                    
                    printf("%s\n", errorText.c_str());
                    
                    
                }
                
                
                
                
            } else if (itemName=="BobText"){
                newOne.bobText = itemData;
            } else if (itemName=="RepRequired"){
                newOne.repLevelRequired = atoi(itemData.c_str());
            } else if (itemName=="LeaveHappy"){
                newOne.leaveHappyRep=atoi(itemData.c_str());
            } else if (itemName=="LeaveFear"){
                newOne.leaveFearRep=atoi(itemData.c_str());
            } else if (itemName=="LeaveScared"){
                newOne.leaveScaredRep=atoi(itemData.c_str());
            } else if (itemName=="IdleNumber"){
                newOne.idleNumber = atoi(itemData.c_str());
            } else if (itemName=="TileSpawn"){
                newOne.tileGen = atoi(itemData.c_str());
            } else if (itemName=="TextureName"){
                newOne.textureName = itemData;
            } else if (itemName=="Health"){
                newOne.health = atof(itemData.c_str());
            } else if (itemName=="ReqEnergy"){
                newOne.activationEnergy = atoi(itemData.c_str());
            } else if (itemName=="Cost"){
                newOne.cost = atoi(itemData.c_str());
            }else if (itemName=="Dialog"){
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    std::string itemName = val2.first.c_str();
                    std::string itemData = val2.second.data();
                    newOne.dialog[itemName]=itemData;
                    
                }
                
                
            }else if (itemName=="ScareModifiers"){
                
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    
                    std::string itemName = val2.first.c_str();
                    
                    ScareType newModifier;
                    
                    BOOST_FOREACH(boost::property_tree::ptree::value_type const& scareMod, val2.second){
                        
                        
                        std::string itemName = scareMod.first.c_str();
                        std::string itemData = scareMod.second.data();
                        
                        
                        if (itemName=="Damage"){
                            newModifier.damageModifier = atof(itemData.c_str());
                        } else if (itemName=="Awareness"){
                            newModifier.awarenessModifier = atof(itemData.c_str());
                        } else if (itemName=="Scare"){
                            newModifier.scareModifier = atof(itemData.c_str());
                        } else if (itemName=="Name"){
                            newModifier.name = itemData;
                        }
                    }
                    
                    newOne.scareTypeByName[newModifier.name]=newModifier;
                    
                }
            }else if (itemName=="JsonName"){
                newOne.jsonName = itemData;
            } else if (itemName=="PlacementType"){
                
                
                
                
                if (itemData=="2X2"){
                    
                    newOne.placementType=TwoXTwo;
                } else if (itemData=="3X3"){
                    
                    newOne.placementType=ThreeXThree;
                } else if (itemData=="2X1U"){
                    
                    newOne.placementType=TwoXOneU;
                } else if (itemData=="2X1L"){
                    newOne.placementType=TwoXOneL;
                    
                } else if (itemData=="1X1"){
                    
                    newOne.placementType=OneXOne;
                    
                } else if (itemData=="3X2L"){
                    newOne.placementType=ThreeXTwoL;
                }else if (itemData=="3X1L"){
                    newOne.placementType=ThreeXOneL;
                }else if (itemData=="3X1LWallR"){
                    
                    newOne.placementType = ThreeXOneLRightWall;
                    
                } else if (itemData=="6X3L"){
                    newOne.placementType=SixXThreeL;
                    
                }else if (itemData=="2X8") {
                    
                    newOne.placementType=TwoXEight;
                    
                    
                    
                }else if (itemData=="3X2U"){
                    
                    newOne.placementType=ThreeXTwoU;
                }else if (itemData=="1X1LW"){
                    
                    newOne.placementType=OneXOneLeftWall;
                    
                } else if (itemData=="2X1UW"){
                    
                    newOne.placementType=TwoXOneULeftWall;
                }else{
                    
                    printf("WHAH");
                    printf("WHAH");
                }
                
                
                
                
            } else if (itemName=="Index"){
                newOne.index = atoi(itemData.c_str());
            } else if (itemName=="Width"){
                newOne.width = atof(itemData.c_str());
            } else if (itemName=="Height"){
                newOne.height = atof(itemData.c_str());
            } else if (itemName=="Animates"){
                newOne.animates=(itemData=="true")?true:false;
                
            } else if (itemName=="Prop"){
                
                newOne.prop=(itemData=="true")?true:false;
                
                
            } else if (itemName=="Audio"){
                
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    
                    std::string itemName = val2.first.c_str();
                    
                    
                    BOOST_FOREACH(boost::property_tree::ptree::value_type const& audio, val2.second){
                        
                        
                        std::string itemData = audio.second.data();
                        
                        
                        addSoundData[itemName].insert(itemData);
                    }
                }
            }else if (itemName=="Frames"){
                
                //int counter=0;
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    
                    std::string itemName = val2.first.c_str();
                    std::string itemData = val2.second.data();
                    
                    newOne.frames.push_back(atoi(itemData.c_str()));
                    // newOne.frames[counter]=atoi(itemData.c_str());
                    // counter++;
                    
                }
                
                
                
            }  else if (itemName=="SludgeX"){
                newOne.sludgeX=atof(itemData.c_str());
            } else if (itemName=="SludgeY"){
                newOne.sludgeY=atof(itemData.c_str());
                
                
            }
            
            
            
        }
        
        for (std::map<std::string, ScareType>::iterator E1 = newOne.scareTypeByName.begin();
             E1 != newOne.scareTypeByName.end();
             E1++){
            
            newOne.scareByScareModifier.insert(
                                               std::pair<double, ScareType>(E1->second.scareModifier, E1->second)
                                               );
        }
        
        
        
        SpriteDict * newDict = new SpriteDict;
        *newDict = newOne;
        newDict->init = false;
        newDict->mod = !original;
        Sprite::theSpriteAssets[newOne.index]=newDict;
        
        if (!original){
            ModManager::modSprite.push_back(newDict);
        }
        
        

            
        
        AudioController::clearIndex(newOne.index);
        
        for (std::map<std::string, std::set<std::string> >::iterator E1 = addSoundData.begin();
             addSoundData.end()!=E1;
             E1++){
            for (std::set<std::string>::iterator E2 = E1->second.begin();
                 E2!=E1->second.end();
                 E2++){
                
                AudioController::jsonAudio(newOne.index, E1->first, *E2);
            }
            
        }
        
        
        
        
        
    }
    
    
}

void resolve_PlayerTraits_JSON(boost::property_tree::ptree::value_type const& v){
    
    
    
        
        ScareDict newScare;
        newScare.energy = 0;
        newScare.radius = 2;
        newScare.iconOffset =0;
        newScare.index = 0;
        newScare.reload = 0;
        newScare.lastClick = -999999;
        newScare.specialFactor = NOFACTORS;
        newScare.bobText = "This ability does not have text";
        std::set<std::string> addSoundData;
        
        
        BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second)
        {
            
            
            std::string itemName = val.first.c_str();
            std::string itemData = val.second.data();
            
            JSON_STRINGS index = jsonMap[itemName];
            
            switch (index) {
                case ENUM_BOB_TEXT:
                    newScare.bobText = itemData.c_str();
                    break;
                case ENUM_RELOAD:
                    newScare.reload =atoi(itemData.c_str());;
                    break;
                case ENUM_NAME:
                    newScare.name = itemData.c_str();
                    
                    break;
                case ENUM_INDEX:
                    newScare.index = atoi(itemData.c_str());
                    break;
                case ENUM_ENERGY:
                    newScare.energy = atoi(itemData.c_str());
                    break;
                case ENUM_RADIUS:
                    newScare.radius = atoi(itemData.c_str());
                    break;
                case ENUM_REPREQUIRED:
                    newScare.repLevelRequired = atoi(itemData.c_str());
                    break;
                case ENUM_ICONOFFSET:
                    newScare.iconOffset= atoi(itemData.c_str());
                    break;
                case ENUM_SPECIAL_FACTOR:
                    
                    if (itemData=="2X2"){
                        
                        newScare.specialFactor =TWOXTWO;
                    } else {
                        newScare.specialFactor =THREEXTHREE;
                        
                    }
                    
                    break;
                    
                    
                case ENUM_AUDIO:
                {
                    BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                        
                        
                        std::string itemData = val2.second.data();
                        
                        addSoundData.insert(itemData);
                        
                    }
                }
                    break;
                case ENUM_SCAREMODIFIERS:{
                    
                    BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                        
                        std::string itemName = val2.first.c_str();
                        
                        ScareType newModifier;
                        
                        BOOST_FOREACH(boost::property_tree::ptree::value_type const& scareMod, val2.second){
                            
                            
                            std::string itemName = scareMod.first.c_str();
                            std::string itemData = scareMod.second.data();
                            
                            index = jsonMap[itemName];
                            
                            
                            switch(index){
                                case ENUM_DAMAGE:
                                    newModifier.damageModifier = atof(itemData.c_str());
                                    break;
                                case ENUM_AWARENESS:
                                    newModifier.awarenessModifier = atof(itemData.c_str());
                                    break;
                                case ENUM_SCARE:
                                    newModifier.scareModifier = atof(itemData.c_str());
                                    break;
                                case ENUM_NAME:
                                    newModifier.name = itemData;
                                    break;
                                default:
                                    break;
                                    
                            }
                            
                            
                        }
                        newScare.scareTypeByName[newModifier.name]=newModifier;
                        
                        
                        newScare.scareByScareModifier.insert(
                                                             std::pair<double, ScareType>(newModifier.scareModifier, newModifier)
                                                             );
                    }
                    
                }
                    break;
                default:
                    break;
                    
                    
                    
            }
            
        }
    
    
    
    
    
    

    
    
        for (std::set<std::string>::iterator IT = addSoundData.begin();
             IT != addSoundData.end();
             IT++){
            
            
            AudioController::jsonAudio(-1, newScare.name, *IT);
        }
        
        
        while (Player::scareDict.size()<=newScare.index){
            Player::scareDict.push_back(newScare);
        }
        
        Player::scareDict[newScare.index] = (newScare);
        
        
        
        
        
        
        
        
    
    
}

void resolve_Tooltips_JSON(boost::property_tree::ptree::value_type const& v){

    std::string itemName = v.first.c_str();
    std::string itemData = v.second.data();
    
    UserInterface::gameTips.insert(itemData);

}

void resolve_RepLevels_JSON(boost::property_tree::ptree::value_type const& v){
    
    std::string itemName = v.first.c_str();
    std::string itemData = v.second.data();
    
    
    
    
    {
        
        
        
        int index = atoi(itemName.c_str());
        int requirement = 0;
        int influence = 0;
        int numberInWoods = 0;
        int numberBadGuy =0;
        int numberOfAuthorities = 0;
        double energyDrain =0;
        double lowEnergy = 100;
        double midEnergy = 200;
        double speed = 1.0;
        
        std::string bobText;
        std::string levelUpText;
        
        std::map<float,int> woodspawn;
        std::map<float,int> badspawn;
        std::map<float,int> authorites;
        
        BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second)
        {
            
            std::string itemName = val.first.c_str();
            std::string itemData = val.second.data();
            
            
            if (itemName =="RequireMent"){
                requirement = atoi(itemData.c_str());
                
            } else if (itemName == "Influence"){
                influence =atoi(itemData.c_str());
            } else if (itemName == "BobText"){
                
                bobText = itemData;
            } else if (itemName == "InfoText"){
                
                levelUpText = itemData;
                
            } else if (itemName == "WoodsSpawn"){
                
                std::vector<std::string> keys;
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    std::string itemName = val2.first.c_str();
                    std::string itemData = val2.second.data();
                    keys.push_back(itemData);
                }
                int fixer = (keys.size()%2==1)?1:0;
                for (size_t counter = 0; counter<keys.size()-fixer; counter+=2){
                    float percentage = atof(keys[counter].c_str());
                    int index = atoi(keys[counter+1].c_str());
                    
                    woodspawn[percentage]=index;
                    
                    
                    
                }
            } else if (itemName == "SpawningInWoods"){
                
                numberInWoods =atoi(itemData.c_str());
                
            } else if (itemName == "NumberBadGuy"){
                
                numberBadGuy = atoi(itemData.c_str());
                
                
            } else if (itemName == "BadSpawn"){
                
                std::vector<std::string> keys;
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    std::string itemName = val2.first.c_str();
                    std::string itemData = val2.second.data();
                    keys.push_back(itemData);
                }
                
                int fixer = (keys.size()%2==1)?1:0;
                for (size_t counter = 0; counter<keys.size()-fixer ; counter+=2){
                    float percentage = atof(keys[counter].c_str());
                    int index = atoi(keys[counter+1].c_str());
                    
                    badspawn[percentage]=index;
                    
                    
                    
                }
            } else if (itemName == "Speed"){
                
                speed =atof(itemData.c_str());
                
            } else if (itemName == "EnergyDrain"){
                
                energyDrain = atof(itemData.c_str());
                
            } else if (itemName == "LowEnergy"){
                lowEnergy = atoi(itemData.c_str());
            } else if (itemName == "MidEnergy"){
                midEnergy = atoi(itemData.c_str());
            } else if (itemName == "NumberAuthorities"){
                numberOfAuthorities = atoi(itemData.c_str());
            } else if (itemName == "Authorities"){
                
                
                
                std::vector<std::string> keys;
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    std::string itemName = val2.first.c_str();
                    std::string itemData = val2.second.data();
                    keys.push_back(itemData);
                }
                
                int fixer = (keys.size()%2==1)?1:0;
                for (size_t counter = 0; counter<keys.size()-fixer; counter+=2){
                    float percentage = atof(keys[counter].c_str());
                    int index = atoi(keys[counter+1].c_str());
                    
                    authorites[percentage]=index;
                    
                    
                    
                }
                
                
            }
            
            
            
            
        }
        
        
        reputationStruct insertMe;
        insertMe.energy = requirement;
        insertMe.numberOfStruct = influence;
        insertMe.levelUpText = bobText;
        insertMe.infoText = levelUpText;
        insertMe.woodspawn = woodspawn;
        insertMe.numberGoodGuy = numberInWoods;
        insertMe.badspawn = badspawn;
        insertMe.numberBadGuy = numberBadGuy;
        insertMe.energyDrain = energyDrain;
        insertMe.lowEnergy = lowEnergy;
        insertMe.midEnergy = midEnergy;
        insertMe.numberAuthorities = numberOfAuthorities;
        insertMe.authorities = authorites;
        GameLogic::reputationLevels[index] = insertMe;
        
        
        
    }
    
    
    
}

void resolve_TranslationMatrix_JSON(boost::property_tree::ptree::value_type const& v){
    
    std::string itemName = v.first.c_str();
    std::string itemData = v.second.data();
    
    TranslationMatrix::phrases[itemName]=itemData;
    
}

void resolve_Meta_JSON(boost::property_tree::ptree::value_type const& v){
    
    std::string itemName = v.first.c_str();
    std::string itemData = v.second.data();

    if (itemName=="CoupleRadius"){
        ModVars::coupleRadius  = atoi(itemData.c_str());
    } else if (itemName=="CoupleBoost"){
        ModVars::coupleBoost  = atof(itemData.c_str());
    } else if (itemName=="DoctorRadius"){
        ModVars::doctorRadius  = atoi(itemData.c_str());
    } else if (itemName=="ExorcistDrain"){
            ModVars::exorcistDrain= atof(itemData.c_str());
    } else if (itemName=="GamerRadius"){
        ModVars::gamerRadius=atoi(itemData.c_str());
    } else if (itemName=="GhostHunterDrain"){
        ModVars::ghostHunterDrain= atof(itemData.c_str());
    }else if (itemName=="GhostHunterKillArea"){
        ModVars::ghostHunterKillArea=atoi(itemData.c_str());;
    }else if (itemName=="GhostHunterKillTime"){
        ModVars::ghostHunterKillTime=atoi(itemData.c_str());;
    } else if (itemName=="GildaDrain"){
        ModVars::gildaDrain = atoi(itemData.c_str());
    } else if (itemName=="InvestigatorDrain"){
        ModVars::investigatorDrain = atof(itemData.c_str());
    } else if (itemName=="NinjaRadius"){
        ModVars::ninjaRadius = atoi(itemData.c_str());
    } else if (itemName=="NinjaKillTime"){
        ModVars::ninjaKillTime = atoi(itemData.c_str());
    } else if (itemName=="NinjaRepGain"){
        ModVars::ninjaRepGain = atof(itemData.c_str());
    } else if (itemName=="PoliceDrain"){
        ModVars::policeDrain = atof(itemData.c_str());
    } else if (itemName=="PoliceRadius"){
        ModVars::policeRadius = atoi(itemData.c_str());
    }  else if (itemName=="SkepticDrain"){
        ModVars::skepticDrain  = atof(itemData.c_str());
    } else if (itemName=="RenovatorRadius"){
        ModVars::renovatorRadius =  atoi(itemData.c_str());
    } else if (itemName=="ReporterDrain"){
        ModVars::reporterDrain  = atof(itemData.c_str());
    }else if (itemName=="VivianRadius"){
        ModVars::vivianRadius = atoi(itemData.c_str());
    } else if (itemName=="ZombieRadius"){
        ModVars::zombieRadius =  atoi(itemData.c_str());
    } else if (itemName=="StartingEnergy"){
        ModVars::startingEnergy =  atof(itemData.c_str());
    } else if (itemName=="ReputationBuffer"){
        ModVars::reputationBuffer =  atof(itemData.c_str());
    } else if (itemName=="ReputationDrain"){
        ModVars::reputationDrain = atof(itemData.c_str());
        GameLogic::reputationDrainMod = ModVars::reputationDrain;
    } else if (itemName=="TUT_TEENAGER_A"){
        BobZombie::TEENAGER_A = atoi(itemData.c_str());
    } else if (itemName=="TUT_TEENAGER_B"){
        BobZombie::TEENAGER_B = atoi(itemData.c_str());
    } else if (itemName=="TUT_EXORCIST"){
        BobZombie::EXORCIST_A = atoi(itemData.c_str());
    } else if (itemName=="TUT_POLICE"){
        BobZombie::POLICE_A = atoi(itemData.c_str());
    } else if (itemName=="CostToAwareness"){
        ModVars::costToAwarenessRatio=atof(itemData.c_str());
    }
    
    
    
}




bool Sprite::loadDictTextureLoaderReturn(std::string * path){

    buildMap();
    
    std::string theFile;
    
    
    char buffer [1000];
    
    
    bool original = false;
    
    if (path==0){
        
        theFile = findFile("Sprites.json");
        sprintf (buffer, "%s", theFile.c_str());
        original = true;
        
    }else{
        sprintf (buffer, "%s", path->c_str());
        theFile = *path;
        original = false;
    }
    
    std::ifstream f;
    f.open(buffer);
    
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::json_parser::read_json(f,pt);
        
    }
    catch (std::exception const& e)
    {
        
        std::string error = "There is an issues with the ";
        error = error.append(theFile);
        error.append(" file - ");
        error = error.append(e.what());
        
        
        DebugTools::spawnErrorWindow(error);
        
        quitTheGame();
        
        
        return false;
    }
    
    
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type const& v, pt){
        
        std::string itemName = v.first.c_str();
        
        
        if (itemName=="Meta"){
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                resolve_Meta_JSON(val);
            }
            
            
        } else if (itemName=="TranslationMatrix"){
            
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                resolve_TranslationMatrix_JSON(val);
            }
            
            
            
        } else if (itemName=="RepLevels"){
            
            GameLogic::reputationLevels.clear();
            
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                
                resolve_RepLevels_JSON(val);
            }
        } else if (itemName=="Tooltips"){
            
            UserInterface::gameTips.clear();
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                resolve_Tooltips_JSON(val);
            };
            
        } else if (itemName=="PlayerTraits"){
            
            AudioController::clearIndex(-1);
            Player::scareDict.clear();
            
            
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                resolve_PlayerTraits_JSON(val);
            };
            
            
        }  else if (itemName=="Trees"){
            trees.clear();
            
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                
                resolve_Trees_JSON(val);
            }
            
            
            
        }
        
        
        
        
    }
    
    
    for (std::map<int,SpriteDict*>::iterator IT = theSpriteAssets.begin();
         IT != theSpriteAssets.end();
         IT++){
        
        IT->second->aTree = false;
        
    }
    
    
    
    for (std::set<int>::iterator IT = trees.begin();
         IT != trees.end();
         IT++){
        
        theSpriteAssets[*IT]->aTree=true;
        
        
    }
    return true;
    

}

bool Sprite::loadDictTextureLoader(std::string * path){
    
    buildMap();
    
    std::string theFile;
    
    
    char buffer [1000];
    
    
    bool original = false;
    
    if (path==0){
        
        theFile = findFile("Sprites.json");
        sprintf (buffer, "%s", theFile.c_str());
        original = true;
        
    }else{
        sprintf (buffer, "%s", path->c_str());
        theFile = *path;
        original = false;
    }
    
    
    std::ifstream f;
    f.open(buffer);
    
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::json_parser::read_json(f,pt);
        
    }
    catch (std::exception const& e)
    {
        
        std::string error = "There is an issues with the ";
        error = error.append(theFile);
        error.append(" file - ");
        error = error.append(e.what());
        
        
        DebugTools::spawnErrorWindow(error);
        
        quitTheGame();
        
        
        return false;
    }
    
  
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type const& v, pt){
    
        std::string itemName = v.first.c_str();
        
        
        if (itemName=="Meta"){
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                resolve_Meta_JSON(val);
            }
            
            
        } else if (itemName=="TranslationMatrix"){
            
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                resolve_TranslationMatrix_JSON(val);
            }
            
            
            
        } else if (itemName=="RepLevels"){
            
            GameLogic::reputationLevels.clear();
            
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                
                resolve_RepLevels_JSON(val);
            }
        } else if (itemName=="Tooltips"){
            
            UserInterface::gameTips.clear();
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                resolve_Tooltips_JSON(val);
            };
            
        } else if (itemName=="PlayerTraits"){
            
            AudioController::clearIndex(-1);
            Player::scareDict.clear();
            
            
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                resolve_PlayerTraits_JSON(val);
            };
            
            
        } else if (itemName=="Sprites"){
            
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
            
                resolve_Sprites_JSON(val,original);
        
                };
        
        } else if (itemName=="Trees"){
            trees.clear();
        
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
                
                resolve_Trees_JSON(val);
            }
        
            
        
        } else if (itemName=="Tiles"){
        
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
        
            resolve_Tiles_JSON(val,original);
            }
        } else if (itemName=="Doors"){
        
        
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
             
                resolve_Doors_JSON(val, original);
                
            }
        
        }
        
        
    
    
    }
    
    
    for (std::map<int,SpriteDict*>::iterator IT = theSpriteAssets.begin();
         IT != theSpriteAssets.end();
         IT++){
    
        IT->second->aTree = false;
    
    }
    
    

    for (std::set<int>::iterator IT = trees.begin();
         IT != trees.end();
         IT++){
    
        theSpriteAssets[*IT]->aTree=true;
        
    
    }
 
    return true;
}

bool Sprite::allTexturesInit = false;;

std::string Sprite::loadingString = "Loading Assets";
int Sprite::lastCount = 0;
int Sprite::totalCount = 0;

bool Sprite::loadAllTextureTimed(){
    
    if (allTexturesInit){
        return true;
    }
    
    totalCount = int(Door::theDoorAssets.size()+Tiles::tileDictionary.size()*2+theSpriteAssets.size());
    
    
    loadingString = "Loading Assets ";
    loadingString = loadingString.append(std::to_string(lastCount));
    loadingString = loadingString.append(" of ");
    loadingString = loadingString.append(std::to_string(totalCount));
    
    
    lastCount = 0;
    
    
    
    GameClock::startStopWatch();
    
    for (std::map<int, DoorDict*>::iterator IT = Door::theDoorAssets.begin(); IT!=Door::theDoorAssets.end(); IT++){
        
        if (!GameClock::checkStopWatch(1000.0/60.0)){
            return false;
        }
        
        if (IT->second->init){
            
            lastCount++;
            
            continue;
            
        }
        
        if (!IT->second->texture){
            
            IT->second->texture = generateTexture(IT->second);
            Door::parseJsonFrame(IT->second);
            IT->second->init = true;
        }
        
        
        lastCount++;
    }
    
    
    for (std::map<int, SpriteDict*>::iterator IT = theSpriteAssets.begin(); IT!=theSpriteAssets.end(); IT++){
        
        if (!GameClock::checkStopWatch(1000.0/60.0)){
            
            
            return false;
            
        }
        
        
        
        if (IT->second->init){
            
            lastCount++;
            continue;
        }
        
        
        
  
        
        
       
        
       
        
        IT->second->texture = generateTexture(IT->second);
        
        if (IT->second->animates){
            
            
            SaveObject::changeJsonToRaw(IT->second,0);
            
            if (IT->second->offsetArray.size()==0){
                parseJsonFrame(IT->second);
                SaveObject::changeJsonToRaw(IT->second,0);
            }
            
            
            
        }
        IT->second->init = true;
        
        
        
        lastCount++;
        
        
    }
    
    
    
    
    for (std::map<int,TileDict*>::iterator IT = Tiles::tileDictionary.begin(); IT != Tiles::tileDictionary.end(); IT++){
        
        if (!GameClock::checkStopWatch(1000.0/60.0)){
            lastCount++;
            return false;
        }
        
        if (IT->second->texture==0){
            
            IT->second->texture = generateTexture(findFile(IT->second->image));
            
        }
        
        if (IT->second->checker){
            
            IT->second->textureC = generateTexture(findFile(IT->second->checkered));
            
        }

        
        lastCount++;
    }
    
    
    for (std::map<int,TileDict*>::iterator IT = Tiles::tileDictionary.begin(); IT != Tiles::tileDictionary.end(); IT++){
        
        if (!GameClock::checkStopWatch(1000.0/60.0)){
            lastCount++;
            return false;
        }
        
        if (IT->second->textureL==0){
            IT->second->textureL =  generateTexture(findFile(IT->second->imageL));
        }
        
        if (IT->second->textureR==0){
            IT->second->textureR = generateTexture(findFile(IT->second->imageR));
        }
        
        
        
     
        lastCount++;
    }
    
    Tiles::tileDictVector.clear();
    
    for (std::map<int,TileDict*>::iterator IT = Tiles::tileDictionary.begin(); IT != Tiles::tileDictionary.end(); IT++) {
        
        
        while (Tiles::tileDictVector.size()<IT->first+1){
            Tiles::tileDictVector.push_back(0);
        }
        
        Tiles::tileDictVector[IT->first]=IT->second;
        
    }
    
    
    
    
    
    
    Tiles::initialize(128, 128);
    
    Player * thePlayer = (Player*)GameLogic::returnPlayer();
    thePlayer->init();
    
    ShaderManager::enableUIShader();
    thePlayer->position = Tiles::findCords(std::pair<int, int>(64,13));

    
    allTexturesInit=true;
    
      AudioController::initAll();
    
    ModManager::populateBack();
    
    return true;
    
}




Sprite::Sprite(){
    
    isScared=false;
    kill=false;
    
    
}

Sprite::Sprite(int anType){
    
    type=anType;
    frame=0;
    goto_AnimationState=INT_MAX;
    dict = getInfoForSprite(type);
    direction=0;
    _texture = 0;
    
    isScared=false;
    kill=false;
    health = dict->health;
    lastTimeItWasScared = 0;
    
    
    /*
     generate(cpvzero,anType, false);
     */
}

Sprite::Sprite(cpVect anPosition, int anType, bool graveYardAble){
    
    generate(anPosition,anType,graveYardAble);
    
}


Sprite::Sprite(std::pair<int,int> anPosition, int anType, bool graveYardAble){
    
    generate(anPosition,anType,graveYardAble);
    
}




SpriteDict * Sprite::getInfoForSprite(int type){
    
    
    std::map<int, SpriteDict*>::iterator IT = theSpriteAssets.find(type);
    
    
    if (IT!=theSpriteAssets.end()){
        
        return IT->second;
        
    }
    
    
    
    else {
        GDebug::log("The Sprite Type Was Not Found ");
        
        return 0;;
    }
    
    return 0;
}


long Sprite::generateUID(){
    
    UIDCounter++;
    return UIDCounter;
    
    
}


bool Sprite::findPath(int endX){
    
    
    cpVect pos = position;
    int startX = Tiles::findTile(Tiles::findTile(pos));
    
    return findPath(startX,endX);
    
    
}


bool Sprite::findPath(int start, int end){
    
    if (end==-1){
        return false;
    }
    
    PathFinder test;
    
    test.theSprite = this;
    bool successfulOrNot = test.findPath(start, end);
    
    pathThink = generateUID();
    
    if (successfulOrNot){
        path =test.path;
       int index = path.back().index;
        setAnimationState(Tiles::moveDirection(start,index));
        tween =1.0;
    }
    

    return successfulOrNot;
}



void Sprite::generate(std::pair<int, int> target, int anType, bool graveYardAble){
    
    cpVect position = Tiles::findCords(target);
    
    generate(position, anType, graveYardAble);
    
    
    
    
}



void Sprite::generate(cpVect anPosition, int anType, bool graveYardAble){
    
    UID = Sprite::generateUID();
    
    
    type=anType;
    frame=0;
    goto_AnimationState=INT_MAX;
    dict = getInfoForSprite(type);
    direction=0;
    _texture=0;
    
    
   

    position = anPosition;

    
    tween =0;
    startKey=anPosition;
    endKey=anPosition;
    isScared=false;
    

    flip=false;

    
    
    _vertexArray = 0;
    _vertexBuffer = 0;
    _vertexArrayShadow = 0;
    _vertexBufferShadow = 0;
    
    _vertexArraySymbol = 0;
    _vertexBufferSymbol= 0;
    textureSymbol = 0;
    
    
    graveYardAble=graveYardAble;
    summonPosition=anPosition;
    kill=false;
    
    maxIdleTime=5;
    idleTime=0;
    
    health = dict->health;
    
    stumbleTimes=3;
    walkingTarget= 0;
    
    timeTileDead=3.0;
    
    isSelected=false;
    
    timeWhenSpawned=GameClock::time;
    lastTimeItWasScared=GameClock::time;
	lastSeen = GameClock::time;


    maxIdleTime = dict->idleNumber;
    
    renderGridIndex = Tiles::findTile(Tiles::findTile(anPosition));
    
    
    if (graveYardAble){
    if (dict->prop&&dict->animates){
        
        trapsByUID[UID]=this;
        
    }
    
    
    
    if (dict->prop&&!dict->aTree&&!dict->animates){
            propsByUID[UID]=this;
    }
    
    if (!dict->prop){
        peopleByUID[UID]=this;
    }
    
    
    
    if (checkIfBadGuy(this)){
    
        UnitFinder::allFinders[UID]=UnitFinder();
        UnitFinder::allFinders[UID].aSprite = this;
        
        
        Sprite::badPeople.insert(this);
    
    }

    }
}

bool Sprite::checkIfBadGuy(Sprite* value){

    reputationStruct * theStruct = &GameLogic::reputationLevels[GameLogic::getCurrentLevel()];
    
    
    
    for (std::map<float,int>::iterator IT = theStruct->badspawn.begin();
         IT != theStruct->badspawn.end();
         IT++){
        
        if (value->dict->index==IT->second){
        
            return true;
        
        }
    
    }
    
    
    for (std::map<float,int>::iterator IT = theStruct->authorities.begin();
         IT != theStruct->authorities.end();
         IT++){
        
        if (value->dict->index==IT->second){
            
            return true;
            
        }
        
    }
    
    
    
    return false;

}


void Sprite::destroy(bool forEver){
    
    
    
    for (
         std::vector<int>::iterator IT= blockedTiles.begin();
         IT != blockedTiles.end();
         IT++){
        
        
        Tiles::mapValues[*IT].block = false;
        Tiles::mapValues[*IT].occupiedSprite = 0;
        
        
        
        
        
    }
    
    if (trapsByUID.find(UID)!=trapsByUID.end()){
        
        trapsByUID.erase(UID);
        
    }
    
    if (peopleByUID.find(UID)!=peopleByUID.end()){
        peopleByUID.erase(UID);
    }
    
    if (propsByUID.find(UID)!=propsByUID.end()){
        propsByUID.erase(UID);
    }
    
    if (UnitFinder::allFinders.find(UID)!=UnitFinder::allFinders.end()){
        UnitFinder::allFinders.erase(UID);
    
    }
    
    if (Sprite::badPeople.find(this)!=Sprite::badPeople.end()){
        Sprite::badPeople.erase(this);
    }
    
   
    if (_vertexArraySymbol){
        glDeleteVertexArraysCon(1, &_vertexArraySymbol);
        _vertexArray = 0;
    }
    
    if (_vertexBufferSymbol){
    glDeleteBuffers(1, &_vertexBufferSymbol);
        _vertexBuffer = 0;
    }
    

    if (_vertexArray){
        glDeleteVertexArraysCon(1, &_vertexArray);
        _vertexArray = 0;
        
        
    }
    
    if (_vertexBuffer){
        glDeleteBuffers(1, &_vertexBuffer);
        _vertexBuffer = 0;
        
    }
    
    if (!forEver){
        if (graveYardAble){
            graveYard.insert(this);
        }
    }
    
    
    
    
}



void Sprite::advanceFrame(){
    
    SpriteDict * theType = dict;
    
    Animation_Holder theAnimation = theType->animations.at(animationState);
    
    if (theAnimation.speed>0){
        
        
        animationStateHack++;
        if (animationStateHack >= theAnimation.speed){
            animationStateHack=0;
            frame+=1;
        }
        
        
        
        if (frame<theAnimation.start){
            frame=theAnimation.start;
            animationStateHack=0;
            return;
        }
        
        if (frame>theAnimation.end){
            
            if (goto_AnimationState==INT_MAX){
                frame=theAnimation.start;
                animationStateHack=0;
            } else {
                Animation_Holder theAnimationGoto = theType->animations.at(goto_AnimationState);
                frame=theAnimationGoto.start;
                animationStateHack=0;
                animationState=goto_AnimationState;
                goto_AnimationState=INT_MAX;
                
            }
            return;
        }
        
    } else {
        
        animationStateHack++;
        if (animationStateHack >= -theAnimation.speed){
            animationStateHack=0;
            frame-=1;
        }
        
        
        if (frame>theAnimation.start){
            frame=theAnimation.start;
            animationStateHack=0;
            return;
        }
        
        if (frame<theAnimation.end){
            
            if (goto_AnimationState==INT_MAX){
                frame=theAnimation.start;
                animationStateHack=0;
            } else {
                Animation_Holder theAnimationGoto = theType->animations.at(goto_AnimationState);
                frame=theAnimationGoto.start;
                animationStateHack=0;
                animationState=goto_AnimationState;
                goto_AnimationState=INT_MAX;
                
            }
            return;
        }
        
        
        
    }
    
}


void Sprite::printAnimationState(){
    
    switch (GDebug::value1) {
        case 0:
            UserInterface::spawnTextBox("Walk Up Left", 0.0);
            break;
        case 1:
            UserInterface::spawnTextBox("Walk Up Right", 0.0);
            break;
        case 2:
            UserInterface::spawnTextBox("Walk Down Right", 0.0);
            break;
        case 3:
            UserInterface::spawnTextBox("Walk Down Left", 0.0);
            break;
        case 4:
            UserInterface::spawnTextBox("Run Up Left", 0.0);
            break;
        case 5:
            UserInterface::spawnTextBox("Run Up Right", 0.0);
            break;
        case 6:
            UserInterface::spawnTextBox("Run Down Right", 0.0);
            break;
        case 7:
            UserInterface::spawnTextBox("Run Down Left", 0.0);
            break;
        case 8:
            UserInterface::spawnTextBox("Scream", 0.0);
            break;
        case 9:
            UserInterface::spawnTextBox("Die", 0.0);
            break;
        case 10:
            UserInterface::spawnTextBox("Idle", 0.0);
            break;
        default:
            UserInterface::spawnTextBox("No Animation", 0.0);
            break;
            
    }
    
    
}


void Sprite::setAnimationState(int theFrame){
    
    
    previousPosition = direction;
    
    if (dict->frames.size()-1>theFrame){
        
        int min = dict->frames[theFrame];
        int max = dict->frames[theFrame+1];
        
        if (frame>=min&&frame<max){
            return;
        }
        
        
        frame = dict->frames[theFrame];
        direction =  theFrame;
    }
    
    
}

bool Sprite::removeSprite(Sprite * theSprite){
    
    Sprite::moveSprite = 0;
	Sprite::canMove = false;
    
    
    std::vector<Sprite*> * removeGrid = &Tiles::mapValues[theSprite->renderGridIndex].renderGrid;
    
    for (std::vector<Sprite*>::iterator IT = removeGrid->begin();
         IT != removeGrid->end();
         IT++){
        
        if (*IT == theSprite){
            removeGrid->erase(IT);
            break;
        }
        
        
        
    }
    
    spriteByID.erase(theSprite->UID);
    theSprite->destroy(true);
    
    
    
    
    delete theSprite;
    
    return true;
}



static int spawnPerson =0;

#include "Bulldozer.h"

void Sprite::thinkAll(){
    
    
    
    spawnPerson++;
    
    
    if (BobZombie::spawnNewCitizens()){
        if ( Sprite::peopleByUID.size() <150){
            
            if (spawnPerson%GameLogic::spawnRateNormal==0){
                spawnCivilian();
                
            }
            
            reputationStruct * theStruct = &GameLogic::reputationLevels[GameLogic::getCurrentLevel()];
            
            if (spawnPerson%theStruct->numberBadGuy==0){
                
                
                
                
                
                
                spawnWander(1);
                
            }
            if ((spawnPerson+60)%theStruct->numberGoodGuy==0){
                spawnWander(0);
                
            }
            
            
            if (spawnPerson%theStruct->numberAuthorities==0){
                
                if (GameLogic::awareness>GameLogic::awarenessSink){
                    spawnWander(2);
                }
                
                if (GameLogic::awareness>GameLogic::awarenessSink*2){
                    Bulldozer::spawnBulldozer();
                }
            }
            
        }
        
    }
    
    
    
    std::vector<Sprite*> killHolder;
    
    
    for (std::map<size_t, Sprite*>::iterator IT=Sprite::spriteByID.begin();Sprite::spriteByID.end()!=IT; IT++){
        IT->second->think();
        if (IT->second->kill){
            killHolder.push_back(IT->second);
        }
    }
    
    
    for (std::vector<Sprite*> ::iterator IT = killHolder.begin();IT != killHolder.end(); IT++){
        
        
        removeSprite((*IT));
        
        
    }
    
    
    
    
    
}



void Sprite::thinkText(){
    
    std::vector<size_t> killThem;
    
    for (
         std::vector<CBitmapFont*>::iterator IT =  theTexts.begin();
         IT !=theTexts.end();
         IT++){
        
        if (
            (*IT)->think()
            ){
            
            size_t counter = IT - theTexts.begin();
            killThem.push_back(counter);
            
            
        }
    }
    
    
    for (std::vector<size_t>::reverse_iterator IT = killThem.rbegin();
         IT != killThem.rend();
         IT++){
        
        std::vector<CBitmapFont*>::iterator killThis = theTexts.begin() + (*IT);
        
        
        delete (*killThis);
        
        theTexts.erase(killThis);
    }
    
    
    
    
}

void Sprite::idle(){


    setAnimationState(IDLE_FRAME);



}


int Sprite::generateReputation(){
    
    double healthRatio = health/dict->health;
    
    
    if (healthRatio>0.75){
        return dict->leaveHappyRep;
    }else if (healthRatio>0.25){
        return dict->leaveFearRep;
    } else if (healthRatio<=0.25){
        return dict->leaveScaredRep;
    } else {
        return 0;
        
        
    }
    
    return 0;
    
    
}

void Sprite::scarePeople(){
    
    if (lastTimeItWasScared+120 <= GameClock::time ){
        
        if (frame>=dict->frames[1]&&frame<dict->frames[2]){
            return ;
            
        } else {
            
            scareAllSpritesGood(position, 1, dict, UID);
            setAnimationState(1);
            
            
            lastTimeItWasScared = GameClock::time;
            return;
            
            
        }
        
        
    }
    
    
    
}


void Sprite::addTrait(size_t _traits){
    
    
    while(traits.size()<_traits+1){
        traits.push_back(SIZE_T_MAX);
    }
    
    traits[_traits]=GameClock::time;
    
}


size_t Sprite::testTraitTime(size_t _traits){

    if (traits.size()>_traits){
        
        if (traits[_traits]==SIZE_T_MAX){
            return 0;
            
        }
        

        
        return (GameClock::time-traits[_traits]);
 
        
    } else {
        return 0;
        
    }
    
    return 0;
    

}

bool Sprite::testTrait(size_t _traits, size_t _time){
    
    if (traits.size()>_traits){
        
        if (traits[_traits]==SIZE_T_MAX){
            return false;
            
        }
        
        if (traits[_traits]+_time>GameClock::time){
            
            return true;
            
        } else {
            
            return false;
        }
        
        
    } else {
      return false;
    
    }
    
    return false;
}

void Sprite::traitTest(){
    
    switch (dict->trait) {
        case GOTH:
        case SCAREDYCAT:
        case COUPLE:
        case HERO:
        case IDIOT:
        break;
        
        
        
        

        
        break;
        
        case GILDA:
            
            
         
                    
            if (!isDead()){
        if(!isScared){
            UnitFinder::findAndSetToActive(UID);
            GameLogic::awareness += ModVars::gildaDrain*GameLogic::awarenessSink;
            
            if (GameClock::time%60==0){
                
                addText(std::to_string(int(ModVars::gildaDrain*GameLogic::awarenessSink*60)), 240.0/255.0,240/255.0,21/255.0);
                
            }
            
            
            UserInterface::spawnTextBox(TranslationMatrix::translate("GILDA_IN_HOUSE"), TEXTBOX_STAY_UP_FOREVER);
            
 
        }
            }
        break;
     
        
        case NINJA:{
            if (!isDead()){
            if (!isScared){
                if ((GameClock::time+NINJA)%ModVars::ninjaKillTime==0){

                    std::vector<Sprite*> returnShapes = Tiles::grabAllSprite(renderGridIndex, ModVars::ninjaRadius);

                    for (std::vector<Sprite*>::iterator IT = returnShapes.begin(); IT !=returnShapes.end(); IT++){
                        Sprite * theSprite = (*IT);
                        ScareType aType = theSprite->dict->scareTypeByName["Default"];
                        GameLogic::awareness += ModVars::ninjaRepGain*aType.awarenessModifier;
                        if (theSprite !=this){
                            theSprite->die();
                        }
                    }
                }
            }
        }
        }
        break;
        
        
        case INVESTIGATOR:
        if (!isDead()){
        if(!isScared){
            if (Tiles::mapValues[renderGridIndex].type > 2){
                
                
            
                GameLogic::awareness += ModVars::investigatorDrain;
            
            }
        }
        }
        break;
        
        case GHOSTHUNTER:
        if (!isDead()){
        if(!isScared){
            if (Tiles::mapValues[renderGridIndex].type > 2){
                UnitFinder::findAndSetToActive(UID);

                GameLogic::awareness += ModVars::ghostHunterDrain*GameLogic::awarenessSink;
                
                //NEW TEXT
                UserInterface::spawnTextBox(TranslationMatrix::translate("GHOSTHUNTER_IN_THE_HOUSE"),TEXTBOX_STAY_UP_FOREVER);
                
                if (GameClock::time%60==0){
                    
                    addText(std::to_string(int(ModVars::ghostHunterDrain*GameLogic::awarenessSink*60)), 240.0/255.0,240/255.0,21/255.0);
                    
                }
                
                
                if ((GameClock::time+8)%15==0){
             
                    std::vector<Sprite*> theTrap = Tiles::grabAllTraps(renderGridIndex,ModVars::ghostHunterKillArea);
                
                    
                    for (std::vector<Sprite*>::iterator IT = theTrap.begin();
                         IT !=theTrap.end();
                         IT++){
                        
                        
                        Sprite * theSprite = (*IT);
                        
                        if (theSprite->dict->prop&&theSprite->dict->animates){
                            size_t lastTime = theSprite->testTraitTime(GHOSTHUNTER);
                            
                            if (lastTime==0) {
                                theSprite->addTrait(GHOSTHUNTER);
                            } else  if (lastTime>ModVars::ghostHunterKillTime*2){
                                theSprite->addTrait(GHOSTHUNTER);
                            } else  if (lastTime>ModVars::ghostHunterKillTime) {
                                Sprite::removeSprite(theSprite);
                            }
                        }
                    }
                }
            }
        }
        }
        break;
        
        
        case RENOVATOR:{
            if (!isDead()){
            if (GameClock::time%15==0){
                if (!isScared){
                    std::vector<Sprite*> theTrap = Tiles::grabAllTraps(renderGridIndex,ModVars::renovatorRadius);
                    
                    for (std::vector<Sprite*>::iterator IT = theTrap.begin();
                         IT !=theTrap.end();
                         IT++){
                        UnitFinder::findAndSetToActive(UID);
                        
                        Sprite * theSprite = (*IT);
                        if (theSprite->dict->prop&&theSprite->dict->animates){
                            theSprite->addTrait(RENOVATOR);
                            //NEW TEXT
                            UserInterface::spawnTextBox(TranslationMatrix::translate("RENOVATOR_IN_THE_HOUSE"),TEXTBOX_STAY_UP_FOREVER);
                        }
                    }
                }
            }
            }
        }
            break;
        case DOCTOR:{
            if (!isDead()){
            std::vector<Sprite*> returnShapes = Tiles::grabAllSprite(renderGridIndex, ModVars::doctorRadius);
            
            for (std::vector<Sprite*>::iterator IT = returnShapes.begin(); IT !=returnShapes.end(); IT++){
                
                Sprite * theSprite = (*IT);
                
                theSprite->addTrait(DOCTOR);
                
                
            }
            }
            
            
            
        }
            break;
        case VIVIAN:
            
            
            if (!isDead()){
                if (!isScared){
                    UserInterface::spawnTextBox(TranslationMatrix::translate("VIVIAN_IN_THE_HOUSE"),TEXTBOX_STAY_UP_FOREVER);
                    scareAllSprites(position, ModVars::vivianRadius, NULL, UID);
                }
            }
            
            break;
        case GAMER:{
            
            if (!isDead()){
                if (!isScared){
                    UserInterface::spawnTextBox(TranslationMatrix::translate("GAMER_IN_THE_HOUSE"),TEXTBOX_STAY_UP_FOREVER);
                    scareAllSprites(position, ModVars::gamerRadius, NULL, UID);
                }
            }
            
        
        
        }
            break;
        case ZOMBIE:{
            
            if (scaredByList.size()==0){
                scaredByList.insert(UID);
            }
            if (!isDead()){
            if (!isScared){
                UserInterface::spawnTextBox(TranslationMatrix::translate("ZOMBIE_IN_THE_HOUSE"),TEXTBOX_STAY_UP_FOREVER);
                scareAllSprites(position, ModVars::zombieRadius, NULL, UID);
            }
            }
            
            
            
            
            
        }
            
            
            
            break;
            
        case REPORTER:
            if (!isDead()){
            if (!isScared){
                if (Tiles::mapValues[renderGridIndex].type > 2){
                    UnitFinder::findAndSetToActive(UID);
                    GameLogic::awareness += ModVars::reporterDrain*GameLogic::awarenessSink;
                    
                    if (GameClock::time%60==0){
                        
                        addText(std::to_string(int(ModVars::reporterDrain*GameLogic::awarenessSink*60)), 240.0/255.0,240/255.0,21/255.0);
                        
                    }
                    
                    //NEW TEXT
                    UserInterface::spawnTextBox(TranslationMatrix::translate("REPORTER_IN_THE_HOUSE"),TEXTBOX_STAY_UP_FOREVER);
                 
                }
            }
            }
            break;
        
        case EXORCIST:
            if (!isDead()){
            if (!isScared){
                if (Tiles::mapValues[renderGridIndex].type > 2){
                    UnitFinder::findAndSetToActive(UID);
                    
                    
                    
                    double drainValue = GameLogic::energy*ModVars::exorcistDrain;
                    drainValue = (drainValue<15.0/60.0)?15.0/60.0:drainValue;
                    if (GameClock::time%60==0){
                    
                        addText(std::to_string(int(-drainValue*60)), 181.0/255.0,36/255.0,36/255.0);
                    
                    }
                    
                    
                    
                    GameLogic::energy -= drainValue ;
                    
                    //NEW TEXT
                    UserInterface::spawnTextBox(TranslationMatrix::translate("EXORCIST_IN_THE_HOUSE"), TEXTBOX_STAY_UP_FOREVER);
                    
                    
                }
            }
            }
            break;
        case SKEPTIC:
            if (!isDead()){
            if (!isScared){
                if (Tiles::mapValues[renderGridIndex].type > 2){
                    
                    if (GameClock::time%60==0){
                        
                        addText(std::to_string(ModVars::skepticDrain*60), 220.0/255.0, 217.0/255.0, 22.0/255.0);
                        
                    }
                    
                    
                    GameLogic::awareness += ModVars::skepticDrain;
                }
            }
            }
            break;
        case POLICE:
        if (!isDead()){
            if (!isScared){
                if(BobZombie::currentTutorialState==NO_TUTORIAL){
                    
                 
                    
                    
                    //NEW TEXT
                    
                    UserInterface::spawnTextBox(TranslationMatrix::translate("POLICE_IN_THE_HOUSE"), TEXTBOX_STAY_UP_FOREVER);

                    UnitFinder::findAndSetToActive(UID);
                    double drainValue = GameLogic::energy*ModVars::policeDrain;

                    drainValue = (drainValue<15.0/60.0)?15.0/60.0:drainValue;
                    
                    if (GameClock::time%60==0){
                        
                        addText(std::to_string(int(-drainValue*60)), 181.0/255.0,36/255.0,36/255.0);
                        
                    }
                    
                    GameLogic::energy -= drainValue ;
                    
                    if (GameClock::time%15==0){
                        
                        std::vector<Sprite*> returnShapes = Tiles::grabAllSprite(renderGridIndex, ModVars::policeRadius);
                        
                        for (std::vector<Sprite*>::iterator IT = returnShapes.begin(); IT !=returnShapes.end(); IT++){
                            
                            Sprite * theSprite = (*IT);
                            if (theSprite != this){
                                if (!theSprite->testTrait(POLICE, 9000)){
                                    
                                    theSprite->findPath(69);
                                    theSprite->addTrait(POLICE);
                                    
                                }
                            }
                        }
                    }
                }
            }
        }
        
        break;
    
        case NORMAL_CITIZEN:
    
        
        
        break;
        
        case BULLDOZER:
        case MAX_TRAITS:
        printf("NEVER HAPPENS");
        break;
        
    }
    
    ;
    
    
    
}


void Sprite::think(){
    
    //AIManager::think(UID);
    
    
    //Deal with the floating Text
    thinkText();
    
    //Because we don't have the frames to animate every frame
    animationStateHack++;
    
    
    
    if (dict->prop&&dict->animates){
        
        if (!testTrait(RENOVATOR, 20)){
            std::vector<int> vector = getRadiusAroundLargeSprite(this);
            
            for (std::vector<int>::iterator IT = vector.begin();
                 IT != vector.end();
                 IT++){
                
                int index = *IT;
                    
                    for (std::vector<Sprite*>::iterator IT = Tiles::mapValues[index].renderGrid.begin();
                         IT != Tiles::mapValues[index].renderGrid.end();
                         IT++){
                        
                        if (!(*IT)->dict->prop){
                            if(!checkIfBadGuy((*IT))){
                                if ((*IT)->scaredByList.find(UID)==(*IT)->scaredByList.end()){
                                    
                                    //Break Loop
                                    
                                    scarePeople();
                                
                            }
                        }
                    }
                }
            }
        }
        
        if (animationStateHack>1){
            frame++;
            animationStateHack =0;
            if (dict->frames[direction+1]<=frame){
                frame=0;
                direction=0;
                animationStateHack =0;
            }
        }
        return;
    }
    
    
    
    
    if (!dict->prop){
        
        traitTest();
        
    }else {
        
        return;
    }
    
    
    if (animationStateHack>3){
        if (direction==DIE_FRAME){
            // If the person's dead animation is completed hold frame;
            if (frame==dict->frames[DIE_FRAME+1]-1){
                timeTileDead-=1.0/120.0;
                
                if (timeTileDead<0){
                    kill= true;
                }
                
                return;
                
            }
        }
        
        
        frame++;
        if (dict->frames[direction+1]<=frame){
            
            if (dict->prop){
                frame=0;
                direction=0;
            } else {
                frame=dict->frames[direction];
                
                if (direction==SCREAM_FRAME){
                    
                    cpVect pos = position;
                    
                    if (previousPosition <8){
                        
                        int newPosition = previousPosition%4;
                        
                        if (isScared){
                            direction = newPosition+4;
                            
                        } else {
                            
                            direction = newPosition;
                        }
                        
                        
                    } else {
                        
                        
                        
                        int startIndex = Tiles::findTile(Tiles::findTile(pos));
                        int endIndex = Tiles::findTile(Tiles::findTile(endKey));
                        
                        
                        
                        
                        
                        
                        
                        if (isScared){
                            direction = Tiles::moveDirection(startIndex ,endIndex)+4;
                        } else {
                            direction = Tiles::moveDirection(startIndex,endIndex );
                        }
                    }
                }
            }
        }
        animationStateHack=0;
    }
    
    if (direction == SCREAM_FRAME ){
        
        return;
        
    }
    
    if (direction == DIE_FRAME){
        
        return;
    }
    
    if (direction == IDLE_FRAME){
    idleTime++;
    
    }
    
    
    
    if (isScared) {
        
        tween+=3.0/60.0;
    } else {
        tween+=1.0/120.0;
    }
    // For when things go wrong it will try to make a new path.
    if (!cpveql(startKey, endKey)){
        if (cpvdistsq(startKey, endKey)>10000){
            if (path.size()>0){
                startKey =endKey;
                tween=1.0;
                PathTile endValue = *path.begin();
                
                if (!findPath(endValue.index)){
                    idle();
                    return;
                    
                }
            }
        }
    }
    
    position = cpvlerp(startKey, endKey, tween);
    

  
    
    if (tween>=1.0){
        
        if (isScared){
            int thePos = Tiles::findTile(Tiles::findTile(endKey));
            
            if (!Tiles::
                insideHouse(thePos)){
                
                
                if (!Camera::isBoundBoxVisible(boundingBox.l, boundingBox.b, boundingBox.r, boundingBox.t)){ //
                    kill=true;
                    return;
                }
                
            }
            
        }
        
        
        tween=0;
        if (path.size()==0){
            startKey=endKey;
            
            std::pair<int,int> value = Tiles::findTile(endKey);
            
            if (value.second==0||value.second==Tiles::width-1||value.first==0||value.first==Tiles::width-1){
                this->kill=true;
            }
            
            setAnimationState(IDLE_FRAME);
            
           
            switch (dict->trait) {
                case DOCTOR:
                case EXORCIST:
                case NORMAL_CITIZEN:
                case SKEPTIC:
                case VIVIAN:
                case GAMER:
                case ZOMBIE:
                {
                    if (maxIdleTime!=INT_MAX){
                findPath(Tiles::getRandomTileHouse());
                    }
                }
                break;
                
                default:
                break;
            }
                
            
            
            
            
            
            
            if (idleTime>=maxIdleTime){
                switch (dict->trait) {
                    case GOTH:
                    case COUPLE:
                    case SCAREDYCAT:
                    //case IDIOT:
                    {
                    
                        if (lastSeen<GameClock::time-400){
                        
                            kill=true;
                            return;
                        }
                    
                    
                    }                    break;
                    
                    default:
                    break;
                }
 
                if (idleTime%30==0){
                    if (!findPath(69)){
                        if (GameClock::time-400>lastSeen){
                            kill=true;
                            return;
                        }
                    }
                }
                
                
            }
            return;
        }
        
        
        PathTile value = *path.rbegin();
        
        if (!Tiles::canMove2I(renderGridIndex, value.index)){
            
            startKey =endKey;
            tween=1.0;
            PathTile endValue = *path.begin();
            
            if (!findPath(endValue.index)){
                
                
                if (GameClock::time-60>lastSeen){
                    kill=true;
                    return;
                }
                
                idle();
            }
            
            
            return;
            
        }
        
        
        startKey =endKey;
        
        
        
        int currentPost = Tiles::findTile(Tiles::findTile(position));
        
        int newDirection = Tiles::moveDirection(currentPost,value.index);
        
        
        
        if (isScared){
            if (newDirection+4 != direction){
                setAnimationState(newDirection+4);
            }
            
            
        }else {
            if (newDirection != direction){
                setAnimationState(newDirection);
            }
        }
        
        path.pop_back();
        
        
        
        cpVect pos = Tiles::findCords(Tiles::findTile(value.index));
        endKey = pos;
        
        position = startKey;

        
        
        tween=0;
        
    }
    
    
    
    
    //printf("NO BRAIN");
    
}

void Sprite::update(){
    
    cpVect pos ;
    double rotation;
    /*
     if (!cpBodyIsStatic(theBody)){
     pos = cpBodyGetPos(theBody);
     rotation = cpBodyGetAngle(theBody);
     } else {
     pos = cpBodyGetPos(theBody);
     rotation = cpBodyGetAngle(theBody);
     
     }
     */
    
    pos =position;
    rotation = 0;
    update(pos,rotation);
    
    if (!dict->prop){
        
        updateShadow(pos, rotation);
    }
    
    if (dict->aTree){
    
        updateShadow(pos, rotation);
    }
    
    
    
    
    for (std::vector<CBitmapFont*>::iterator IT = theTexts.begin();
         IT != theTexts.end();
         IT++){
        
        (*IT)->SetCursor(pos.x, pos.y+(15.0));
        (*IT)->update();
        
        
    }
    
    
}

#include "Tweener.h"

void Sprite::updateSymbol(){

    std::vector<GLfloat> vertex;
    vertex.reserve(4*4);
    
    
    
    double factor = (GameClock::time%120<60)?(GameClock::time%60)/60.0:(60.0-(GameClock::time%60))/60.0;
    
    
    GLfloat ta = Tweener::quadEaseBoth(factor, 0.0, 1.0, 2.0);
    
    GLfloat width = 3.3+(.6*ta);
    GLfloat yOffset = 15.0;
    
    vertex.push_back(position.x+dict->sludgeX-width);
    vertex.push_back(position.y-width+yOffset);
    vertex.push_back(0);
    vertex.push_back(1);
    
    vertex.push_back(position.x+dict->sludgeX-width);
    vertex.push_back(position.y+width+yOffset);
    vertex.push_back(0);
    vertex.push_back(0);
    
    vertex.push_back(position.x+dict->sludgeX+width);
    vertex.push_back(position.y-width+yOffset);
    vertex.push_back(1.0);
    vertex.push_back(1.0);
    
    vertex.push_back(position.x+dict->sludgeX+width);
    vertex.push_back(position.y+width+yOffset);
    vertex.push_back(1.0);
    vertex.push_back(0.0);
    
    
    while (traitTexture.size()==0){
        traitTexture.assign(100, 0);
    }
    
    if (!traitTexture[RENOVATOR]){
    
        traitTexture[RENOVATOR] = generateTexture(findFile("Police_Shield.png"));
    }
    
    
    if (!traitTexture[GHOSTHUNTER]){
        traitTexture[GHOSTHUNTER] = generateTexture(findFile("GhostHunterIcon.png"));
    }
    
    
    
    if (!_vertexArraySymbol){
        glGenVertexArraysCon(1, &_vertexArraySymbol);
    }
    glBindVertexArrayCon(_vertexArraySymbol);
    
    if(!_vertexBufferSymbol){
        glGenBuffers(1, &_vertexBufferSymbol);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferSymbol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex.size(), &vertex[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));

}

void Sprite::renderSymbol(){

    if (testTrait(RENOVATOR, 20)){
        if (_vertexArraySymbol){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *traitTexture[RENOVATOR]);
            glBindVertexArrayCon(_vertexArraySymbol);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        }
    } else if (testTrait(GHOSTHUNTER, ModVars::ghostHunterKillTime)){
        if (_vertexArraySymbol){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *traitTexture[GHOSTHUNTER]);
            glBindVertexArrayCon(_vertexArraySymbol);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
        }
    }
    

}

void Sprite::updateShadow(cpVect pos, double rotation){
    
    
    if (_texture==0){
        
        if (dict->texture==0){
            
            
            generateTexture(dict);
            
            
            
        }
        _texture = dict->texture;
        
    }
    
    if ( dict->animates   ){
        
        
        
        GLfloat text[12];
        
        dumpTextureCord(frame, text);
        
        //4,6 left
        //7,11 down
        
        GLfloat modLeft = (0.90)*(text[8]-text[4]);//(text[8]-text[4])*0.33333333;
        
        
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)(text[4]+pos.x+dict->sludgeX-modLeft),
            (GLfloat)(text[5]*0.01+pos.y+dict->sludgeY),
            text[0],  text[1],
            
            
            (GLfloat)(text[6]+pos.x+dict->sludgeX),
            (GLfloat)(text[7]+pos.y+dict->sludgeY),
            text[0],  text[3],
            
            (GLfloat)(text[8]+pos.x+dict->sludgeX-modLeft),
            (GLfloat)(text[9]*0.01+pos.y+dict->sludgeY),
            text[2],  text[1],
            
            (GLfloat)(text[10]+pos.x+dict->sludgeX),
            (GLfloat)(text[11]+pos.y+dict->sludgeY),
            text[2],  text[3]
        };
        
        if (!_vertexArrayShadow){
            glGenVertexArraysCon(1, &_vertexArrayShadow);
        }
        glBindVertexArrayCon(_vertexArrayShadow);
        
        if(!_vertexBufferShadow){
            glGenBuffers(1, &_vertexBufferShadow);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferShadow);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gBoxVertexData), gBoxVertexData, GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
        
        
        
        glBindVertexArrayCon(0);
        
    } else {
        
        
        
        
        
        GLfloat text[12];
        
        dumpTextureCord(frame, text);
        
        GLfloat modLeft = (text[8]-text[4])*0.33333333;
        
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)(text[4]+pos.x+dict->sludgeX-modLeft),
            (GLfloat)(text[5]*0.01+pos.y+dict->sludgeY),
            text[0],  text[1],
            
            
            (GLfloat)(text[6]+pos.x+dict->sludgeX),
            (GLfloat)(text[7]+pos.y+dict->sludgeY),
            text[0],  text[3],
            
            (GLfloat)(text[8]+pos.x+dict->sludgeX-modLeft),
            (GLfloat)(text[9]*0.01+pos.y+dict->sludgeY),
            text[2],  text[1],
            
            (GLfloat)(text[10]+pos.x+dict->sludgeX),
            (GLfloat)(text[11]+pos.y+dict->sludgeY),
            text[2],  text[3]
        };
        
       
     
        
        
        
        
        //////
        
        
        
        
        
        
        if (!_vertexArrayShadow){
            glGenVertexArraysCon(1, &_vertexArrayShadow);
        }
        glBindVertexArrayCon(_vertexArrayShadow);
        
        if(!_vertexBufferShadow){
            glGenBuffers(1, &_vertexBufferShadow);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferShadow);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gBoxVertexData), gBoxVertexData, GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
        
        
        
        glBindVertexArrayCon(0);
        
        
        
        
        
        
        
    }
    
    
    
    
    
    
    
    
    
}


void Sprite::updateNoGL(){

    cpVect pos = position;
    if (_texture==0){
        
        if (dict->texture==0){
            
            
            generateTexture(dict);
            
            
            
        }
        _texture = dict->texture;
        
    }
    
    if ( dict->animates   ){
        
        GLfloat text[12];
        
        minPoint = dumpTextureCord(frame, text);
        
        minPoint.x=pos.x;//text[6]+pos.x+dict->sludgeX;
        minPoint.y=pos.y;//text[7]+pos.y+dict->sludgeY;
        
        boundingBox = cpBBNew(text[6]+pos.x+dict->sludgeX,
                              text[7]+pos.y+dict->sludgeY,
                              text[8]+pos.x+dict->sludgeX,
                              text[5]+pos.y+dict->sludgeY);
        textureBox = cpBBNew(text[0],text[3],text[2],text[1]);
       
    } else {
        
        GLfloat text[12];
        
        minPoint = dumpTextureCord(frame, text);
        
        minPoint.x=pos.x;
        minPoint.y=pos.y;
        
        boundingBox = cpBBNew(text[6]+pos.x+dict->sludgeX,
                              text[7]+pos.y+dict->sludgeY,
                              text[8]+pos.x+dict->sludgeX,
                              text[5]+pos.y+dict->sludgeY);
        textureBox = cpBBNew(0,1,1,0);
        
    }

    
    for (std::vector<CBitmapFont*>::iterator IT = theTexts.begin();
         IT != theTexts.end();
         IT++){
        
        (*IT)->SetCursor(pos.x, pos.y+(15.0));
        (*IT)->update();
        
        
    }
    
    if (testTrait(RENOVATOR, 20)){
        updateSymbol();
    
    }
    
    if (testTrait(GHOSTHUNTER, 20)){
        updateSymbol();
    }
    
    
    
}


void Sprite::updateGL(){
    
    if (!dict->prop){
        
        updateShadow(position, 0);
    }
    
    if (dict->aTree){
        
        updateShadow(position, 0);
    }
    
    
    
    if ( dict->animates   ){
        
        
        
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)(boundingBox.l),
            (GLfloat)(boundingBox.b),
            (GLfloat)textureBox.l,
            (GLfloat)textureBox.b,
            
            
            (GLfloat)(boundingBox.l),
            (GLfloat)(boundingBox.t),
            (GLfloat)textureBox.l,
            (GLfloat)textureBox.t,
            
            (GLfloat)(boundingBox.r),
            (GLfloat)(boundingBox.b),
            (GLfloat)textureBox.r,
            (GLfloat)textureBox.b,
            
            (GLfloat)(boundingBox.r),
            (GLfloat)(boundingBox.t),
            (GLfloat)textureBox.r,
            (GLfloat)textureBox.t,
            
        };
        
        
        
        
        if (!_vertexArray){
            glGenVertexArraysCon(1, &_vertexArray);
        }
        glBindVertexArrayCon(_vertexArray);
        
        if(!_vertexBuffer){
            glGenBuffers(1, &_vertexBuffer);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gBoxVertexData), gBoxVertexData, GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
        
        
        
        glBindVertexArrayCon(0);
        
    } else {
        
        
        if (flip){
            
            if (dict->placementType==OneXOne||
                dict->placementType==TwoXTwo||
                dict->placementType==ThreeXThree){
                
           
            
        
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)(boundingBox.l),
            (GLfloat)(boundingBox.b),
            (GLfloat)textureBox.r,
            (GLfloat)textureBox.b,
            
            
            (GLfloat)(boundingBox.l),
            (GLfloat)(boundingBox.t),
            (GLfloat)textureBox.r,
            (GLfloat)textureBox.t,
            
            (GLfloat)(boundingBox.r),
            (GLfloat)(boundingBox.b),
            (GLfloat)textureBox.l,
            (GLfloat)textureBox.b,
            
            (GLfloat)(boundingBox.r),
            (GLfloat)(boundingBox.t),
            (GLfloat)textureBox.l,
            (GLfloat)textureBox.t,
            
        };
        
        
        
        
        if (!_vertexArray){
            glGenVertexArraysCon(1, &_vertexArray);
        }
        glBindVertexArrayCon(_vertexArray);
        
        if(!_vertexBuffer){
            glGenBuffers(1, &_vertexBuffer);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gBoxVertexData), gBoxVertexData, GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
        
        
        
        glBindVertexArrayCon(0);
        
            return;
                 }
        }
        
        
        
        
        {
            GLfloat gBoxVertexData[] = {
                
                (GLfloat)(boundingBox.l),
                (GLfloat)(boundingBox.b),
                (GLfloat)textureBox.l,
                (GLfloat)textureBox.b,
                
                
                (GLfloat)(boundingBox.l),
                (GLfloat)(boundingBox.t),
                (GLfloat)textureBox.l,
                (GLfloat)textureBox.t,
                
                (GLfloat)(boundingBox.r),
                (GLfloat)(boundingBox.b),
                (GLfloat)textureBox.r,
                (GLfloat)textureBox.b,
                
                (GLfloat)(boundingBox.r),
                (GLfloat)(boundingBox.t),
                (GLfloat)textureBox.r,
                (GLfloat)textureBox.t,
                
            };
            
            
            
            
            if (!_vertexArray){
                glGenVertexArraysCon(1, &_vertexArray);
            }
            glBindVertexArrayCon(_vertexArray);
            
            if(!_vertexBuffer){
                glGenBuffers(1, &_vertexBuffer);
            }
            glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(gBoxVertexData), gBoxVertexData, GL_STATIC_DRAW);
            
            
            glEnableVertexAttribArray(GLKVertexAttribPosition);
            glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
            glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
            
            
            
            
            
            glBindVertexArrayCon(0);
        
        
        
        
        }
        
        
        
    }
    
    
    
}





void Sprite::update(cpVect pos, double rotation){
    if (_texture==0){
        
        if (dict->texture==0){
            
            
            generateTexture(dict);
            
            
            
        }
        _texture = dict->texture;
        
    }
    
    if ( dict->animates   ){
        
        
        
        GLfloat text[12];
        
        minPoint = dumpTextureCord(frame, text);
        
        minPoint.x=pos.x;//text[6]+pos.x+dict->sludgeX;
        minPoint.y=pos.y;//text[7]+pos.y+dict->sludgeY;
        
        
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)(text[4]+pos.x+dict->sludgeX),
            (GLfloat)(text[5]+pos.y+dict->sludgeY),
            text[0],  text[1],
            
            
            (GLfloat)(text[6]+pos.x+dict->sludgeX),
            (GLfloat)(text[7]+pos.y+dict->sludgeY),
            text[0],  text[3],
            
            (GLfloat)(text[8]+pos.x+dict->sludgeX),
            (GLfloat)(text[9]+pos.y+dict->sludgeY),
            text[2],  text[1],
            
            (GLfloat)(text[10]+pos.x+dict->sludgeX),
            (GLfloat)(text[11]+pos.y+dict->sludgeY),
            text[2],  text[3],
            
        };
        
        boundingBox = cpBBNew(text[6]+pos.x+dict->sludgeX,
                              text[5]+pos.y+dict->sludgeY,
                              text[8]+pos.x+dict->sludgeX,
                              text[7]+pos.y+dict->sludgeY);
        textureBox = cpBBNew(text[0],text[1],text[2],text[3]);
        
        
        if (!_vertexArray){
            glGenVertexArraysCon(1, &_vertexArray);
        }
        glBindVertexArrayCon(_vertexArray);
        
        if(!_vertexBuffer){
            glGenBuffers(1, &_vertexBuffer);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gBoxVertexData), gBoxVertexData, GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
        
        
        
        glBindVertexArrayCon(0);
        
    } else {
        
        
        
        
        GLfloat text[12];
        
        minPoint = dumpTextureCord(frame, text);
        
        minPoint.x=pos.x;
        minPoint.y=pos.y;
        
        
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)(text[4]+pos.x+dict->sludgeX),
            (GLfloat)(text[5]+pos.y+dict->sludgeY),
            text[0],  text[1],
            
            
            (GLfloat)(text[6]+pos.x+dict->sludgeX),
            (GLfloat)(text[7]+pos.y+dict->sludgeY),
            text[0],  text[3],
            
            (GLfloat)(text[8]+pos.x+dict->sludgeX),
            (GLfloat)(text[9]+pos.y+dict->sludgeY),
            text[2],  text[1],
            
            (GLfloat)(text[10]+pos.x+dict->sludgeX),
            (GLfloat)(text[11]+pos.y+dict->sludgeY),
            text[2],  text[3]
        };
        
        boundingBox = cpBBNew(text[6]+pos.x+dict->sludgeX,
                              text[5]+pos.y+dict->sludgeY,
                              text[8]+pos.x+dict->sludgeX,
                              text[7]+pos.y+dict->sludgeY);
        textureBox = cpBBNew(text[0],text[1],text[2],text[3]);
        
        
        if (!_vertexArray){
            glGenVertexArraysCon(1, &_vertexArray);
        }
        glBindVertexArrayCon(_vertexArray);
        
        if(!_vertexBuffer){
            glGenBuffers(1, &_vertexBuffer);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gBoxVertexData), gBoxVertexData, GL_STATIC_DRAW);
        
        
        glEnableVertexAttribArray(GLKVertexAttribPosition);
        glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
        glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
        
        
        
        
        
        glBindVertexArrayCon(0);
        
        
        
        
        
        
        
    }
    
}

int Sprite::indexOfTestSprite = 0;

void Sprite::testSpriteUpdate(){
    if (testSprite){
        
        cpVect pos = Tiles::findCords(Tiles::findTile(GameLogic::lastTouchPosition));
        testSprite->position = pos;
     
        testSprite->updateNoGL();
        testSprite->updateGL();
        
        
        int index = Tiles::findTile(Tiles::findTile(pos));
        indexOfTestSprite = index;
        
//        GameLogic::awareness = index;
        
        

    }
    
}

void Sprite::testSpriteRender(){
    
    if (!testSprite){
        return;
    }
    
    if (!dontRenderTest){
        return;
    }
    
    
    
    
    
    
    if (spriteCostToMuch){
        ShaderManager::enableSelectShader(true);
    }else{
        ShaderManager::enableTextureShader();;
    }
    
    if (testSprite){
        testSprite->render();
    }
    
    if (spriteCostToMuch){
        ShaderManager::enableTextureShader();
    }
    
    /*
     glDisable(GL_STENCIL_TEST);
     glAlphaFunc(GL_GREATER, 0.05);
     glDisable(GL_ALPHA_TEST);
     
     */
}


void Sprite::renderShadow(){
    
    if (timeTileDead<1.0){
        ShaderManager::enableTextureShaderShadow(timeTileDead);
    } else {
        ShaderManager::enableTextureShaderShadow(1.0);
        
    }
    
    if (_vertexArrayShadow!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *_texture);
        glBindVertexArrayCon(_vertexArrayShadow);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    

    
}

void Sprite::renderFast(){

    
    if (this==Player::mainPlayer || this ==testSprite){
        return;
        
    }

    if (_vertexArray!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *_texture);
        glBindVertexArrayCon(_vertexArray);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    
}


void Sprite::renderSelect(){

    if (this==Player::mainPlayer || this ==testSprite){
        return;
    
    }

    float colorValue[4] = {0.0,1.0,0.0,1.0};
    
    ShaderManager::enableHighLightShaderColor(dict->textureWidth,colorValue);

    if (_vertexArray!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *_texture);
        glBindVertexArrayCon(_vertexArray);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }


}



void Sprite::render(){
    
    if (this == Player::mainPlayer){
        Player::mainPlayer->render();
        return;
    }
    
    if (this == testSprite){
        if (spriteCostToMuch){
            ShaderManager::enableSelectShader(true);
        }else{
            ShaderManager::enableTextureShader();;
        }

        if (_vertexArray!=0){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *_texture);
            glBindVertexArrayCon(_vertexArray);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        
        if (spriteCostToMuch){
            ShaderManager::enableTextureShader();
        }
        
        return;
    
    }
    

    

    
    if (timeTileDead<1.0){
        ShaderManager::enableTextureAlpha(timeTileDead);
    }
    
    if (_vertexArray!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *_texture);
        glBindVertexArrayCon(_vertexArray);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    
    if (timeTileDead<1.0){
        ShaderManager::enableTextureShader();
    }
   
    Sprite::renderText();
    
    ShaderManager::enableTextureShader();
    if (testTrait(RENOVATOR, 20)){
        renderSymbol();
    }
    
}

void Sprite::renderText(){
    
    for (std::vector<CBitmapFont*>::iterator IT = theTexts.begin();
         IT != theTexts.end();
         IT++){
        (*IT)->renderColorChange();//;
    }

}

int Sprite::selectedType=INT_MAX;


Sprite * Sprite::addSpriteSave(cpVect anPosition, int anType){
    
    
    if (theSpriteAssets.find(anType)==theSpriteAssets.end()){
        return 0;
    }
    SpriteDict * theDict = theSpriteAssets[anType];
    
    
    if (theDict->prop){
        if (theDict->animates){
            return addSprite(anPosition, anType);
        } else {
            int pos =
            Tiles::findTile(
                            Tiles::findTile(anPosition));
            return addSpriteLarge(pos, theDict);
            
        }
        
    }
    
    return 0;
    
}


Sprite * Sprite::addSprite(cpVect anPosition, int anType){
    
    
    
    Sprite * theSprite = new Sprite(anPosition,anType,true);
    spriteByID[theSprite->UID]=theSprite;

    
    return theSprite;
}

Sprite * Sprite::addSprite(std::pair<int,int> anPosition, int anType){
    
    Sprite * theSprite = new Sprite(anPosition,anType,true);
    spriteByID[theSprite->UID]=theSprite;
    
    return theSprite;// theSprite;
}

Sprite * Sprite::addSprite(int anPosition, int anType){
    
    std::pair<int,int> value = Tiles::findTile(anPosition);
    
    
    Sprite * theSprite = new Sprite(value,anType,true);
    
    spriteByID[theSprite->UID]=theSprite;
    // AIManager::addObject(theSprite);
    if (!theSprite->dict->prop){
        theSprite->setAnimationState(10);
    }
    return theSprite;// theSprite;
}

#include "Directions.h"

std::vector<int> Sprite::getRadiusAroundLargeSprite(Sprite* theSprite){
   int theIndex = theSprite->renderGridIndex;
    
    int xCount=-1;
    int yCount=-1;
    
    switch (theSprite->dict->placementType) {
            
        case NoPlacement:
            xCount=1;
            yCount=1;
            break;
        case SixXThreeL:
            xCount=6;
            yCount=3;
            break;
          case TwoXEight:
            xCount=2;
            yCount=8;
            break;
        case OneXOne:
            xCount=1;
            yCount=1;
            break;
        case TwoXOneU:
            xCount=2;
            yCount=1;
            break;
        case TwoXOneL:
            xCount=1;
            yCount=2;
            break;
        case TwoXTwo:
            xCount=2;
            yCount=2;
            break;
        case ThreeXTwoU:
            xCount=3;
            yCount=2;
            break;
         case ThreeXTwoL:
            xCount=2;
            yCount=3;
            break;
        case ThreeXOneL:
            xCount=1;
            yCount=3;
            break;
    
        case ThreeXThree:
            xCount=3;
            yCount=3;
            break;
        

        case OneXOneLeftWall:
            xCount=1;
            yCount=1;
        case TwoXOneULeftWall:
            xCount=2;
            yCount=1;
        case ThreeXOneLRightWall:
            xCount=3;
            yCount=1;
            break;
         
            
       
    }
    
    int xStart = theIndex%Tiles::width;
    int yStart = theIndex/Tiles::width;
    
    
    
    std::vector<int> items;
    
    //for(int xPos=xStart-1; xPos<xStart+xCount+1&&xStart<Tiles::width;xPos++){
    for(int xPos=xStart; xPos<xStart+xCount&&xStart<Tiles::width;xPos++){
        if (xPos<0){continue;}
        
        TileObject * bTile = &Tiles::mapValues[xPos+(yStart)*Tiles::width];
        TileObject * tTile = &Tiles::mapValues[xPos+(yStart+yCount)*Tiles::width];
        
        if (!(bTile->lw||bTile->theDoorL)){
           items.push_back(xPos+(yStart-1)*Tiles::width);
        }
        
        if (!(tTile->lw||tTile->theDoorL)){
            items.push_back(xPos+(yStart+yCount)*Tiles::width);
        }
        
    }
    
    for(int yPos=yStart; yPos<yStart+yCount&&yStart<Tiles::width;yPos++){
        if (yPos<0){
            continue;
        }
       
        TileObject * rTile = &Tiles::mapValues[xStart+(yPos)*Tiles::width];
        TileObject * lTile = &Tiles::mapValues[xStart+xCount+(yPos)*Tiles::width];
        
        if (!(rTile->rw||rTile->theDoorR)){
            //Left
            items.push_back(xStart-1+(yPos)*Tiles::width);
            
        }
        
        if (!(lTile->rw||lTile->theDoorR)){
        //Right
             items.push_back(xStart+xCount+(yPos)*Tiles::width);
          
        }
    }
    
   if (true) {
        int x1=xStart-1,x2=xStart+xCount;
        int y1=yStart-1,y2=yStart+yCount;
        
        
        TileObject * lTile = &Tiles::mapValues[x1+(yStart)*Tiles::width];
        TileObject * rTile = &Tiles::mapValues[x2+(yStart+yCount)*Tiles::width];
       TileObject * rtTile = &Tiles::mapValues[xStart+xCount+(yStart+yCount)*Tiles::width];
       TileObject * ltTile = &Tiles::mapValues[xStart-1+(yStart+yCount)*Tiles::width];
       TileObject * rbTile = &Tiles::mapValues[xStart+xCount+(yStart-1)*Tiles::width];
  //     TileObject * lbTile = &Tiles::mapValues[xStart-1+(yStart-1)*Tiles::width];
       
       TileObject * bTile = &Tiles::mapValues[xStart+(y1)*Tiles::width];
        TileObject * tTile = &Tiles::mapValues[xStart+xCount+(y2)*Tiles::width];
        
        if (!(rtTile->lw||rtTile->theDoorL)&&!(rtTile->rw||rtTile->theDoorR)){
                items.push_back(x2+(y2)*Tiles::width);
        }
    
        if (!(ltTile->lw||ltTile->theDoorL)||!(tTile->rw||tTile->theDoorR)){
                items.push_back(x1+(y2)*Tiles::width);
            
        }
        
        if (!(rbTile->rw||rbTile->theDoorR)||!(rTile->lw||rTile->theDoorL)){
                items.push_back(x2+(y1)*Tiles::width);
            }
        
        if (!(lTile->lw||lTile->theDoorL)||!(bTile->rw||bTile->theDoorR)){
                items.push_back(x1+(y1)*Tiles::width);
            }
       
    }
    
    

    return items;
    

}

bool Sprite::testSpriteLarge(int location, SpriteDict * target){
    
    switch (target->placementType) {
            
        case NoPlacement:
            
            break;
            
            
        case SixXThreeL:
            return ((Tiles::checkTileOffestForPlacement(location,0,0))&&
                    (Tiles::checkTileOffestForPlacement(location,1,0))&&
                    (Tiles::checkTileOffestForPlacement(location,2,0))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,1))&&
                    (Tiles::checkTileOffestForPlacement(location,1,1))&&
                    (Tiles::checkTileOffestForPlacement(location,2,1))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,2))&&
                    (Tiles::checkTileOffestForPlacement(location,1,2))&&
                    (Tiles::checkTileOffestForPlacement(location,2,2))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,3))&&
                    (Tiles::checkTileOffestForPlacement(location,1,3))&&
                    (Tiles::checkTileOffestForPlacement(location,2,3))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,4))&&
                    (Tiles::checkTileOffestForPlacement(location,1,4))&&
                    (Tiles::checkTileOffestForPlacement(location,2,4))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,5))&&
                    (Tiles::checkTileOffestForPlacement(location,1,5))&&
                    (Tiles::checkTileOffestForPlacement(location,2,5))
                    
                    
                    
                    
                    );
            break;
            
        case TwoXEight:
            return (
                    (Tiles::checkTileOffestForPlacement(location,0,0))&&
                    (Tiles::checkTileOffestForPlacement(location,1,0))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,1))&&
                    (Tiles::checkTileOffestForPlacement(location,1,1))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,2))&&
                    (Tiles::checkTileOffestForPlacement(location,1,2))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,3))&&
                    (Tiles::checkTileOffestForPlacement(location,1,3))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,4))&&
                    (Tiles::checkTileOffestForPlacement(location,1,4))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,5))&&
                    (Tiles::checkTileOffestForPlacement(location,1,5))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,6))&&
                    (Tiles::checkTileOffestForPlacement(location,1,6))&&
                    
                    (Tiles::checkTileOffestForPlacement(location,0,7))&&
                    (Tiles::checkTileOffestForPlacement(location,1,7))
                    );
            
            
            break;
        
        
        case OneXOneLeftWall:
        
        if (Tiles::checkTileOffestForPlacement(location,0,0)){
            if (location>0||location<Tiles::width*Tiles::width-Tiles::width){
            
                if (Tiles::mapValues[location+Tiles::width].lw){
                    return true;
                }else {
                    return false;
                }
                
            }else {
                return false;
            }
        
        } else {
            return false;
        
        }
        
        break;
            
        case OneXOne:
            
            return (Tiles::checkTileOffestForPlacement(location,0,0));
            break;
            
        case TwoXOneU:
        {
            return (
                    (Tiles::checkTileOffestForPlacement(location,0,0))&&
                    (Tiles::checkTileOffestForPlacement(location,1,0))  );
        }
            break;
            
        case TwoXOneL:
        {
            return (
                    (Tiles::checkTileOffestForPlacement(location,0,0))&&
                    (Tiles::checkTileOffestForPlacement(location,0,1))  );
        }
            break;
        case TwoXOneULeftWall:
        {
        
            if  (
                    (Tiles::checkTileOffestForPlacement(location,0,0))&&
                    (Tiles::checkTileOffestForPlacement(location,1,0))  ){
                
                
                if (location>0||location<=Tiles::width*Tiles::width-Tiles::width-1){
                    if (Tiles::mapValues[location+Tiles::width].lw){
                        return true;
                
                
                    } else {
                        return false;
                    }
            
                } else {
                    return false;
                }
            
            } else {
                return false;
            
            }
        
            
            
            
        
        }
            
        case TwoXTwo:
        {
            return  (
                     Tiles::checkTileOffestForPlacement(location,0,0)&&
                     Tiles::checkTileOffestForPlacement(location,1,0)&&
                     Tiles::checkTileOffestForPlacement(location,0,1)&&
                     Tiles::checkTileOffestForPlacement(location,1,1)
                     );
        }
            break;
            
        case ThreeXTwoU:
        {
            
            return  (
                     Tiles::checkTileOffestForPlacement(location,0,0)&&
                     Tiles::checkTileOffestForPlacement(location,1,0)&&
                     Tiles::checkTileOffestForPlacement(location,2,0)&&
                     Tiles::checkTileOffestForPlacement(location,0,1)&&
                     Tiles::checkTileOffestForPlacement(location,1,1)&&
                     Tiles::checkTileOffestForPlacement(location,2,1)
                     );
        }
            break;
            
        case ThreeXTwoL:
            
            return  (
                     Tiles::checkTileOffestForPlacement(location,0,0)&&
                     Tiles::checkTileOffestForPlacement(location,1,0)&&
                     Tiles::checkTileOffestForPlacement(location,0,1)&&
                     Tiles::checkTileOffestForPlacement(location,1,1)&&
                     Tiles::checkTileOffestForPlacement(location,0,2)&&
                     Tiles::checkTileOffestForPlacement(location,1,2)
                     );
            
            
            
            
            break;
        
        case ThreeXOneL:
            return  (
                     Tiles::checkTileOffestForPlacement(location,0,0)&&
                     Tiles::checkTileOffestForPlacement(location,0,1)&&
                     Tiles::checkTileOffestForPlacement(location,0,2));
            break;
        case ThreeXOneLRightWall:
        
        if (
            Tiles::checkTileOffestForPlacement(location,0,0)&&
            Tiles::checkTileOffestForPlacement(location,0,1)&&
            Tiles::checkTileOffestForPlacement(location,0,2)){
            
            if (location>0||location<=Tiles::width*Tiles::width-Tiles::width-1){
                if (Tiles::mapValues[location+1].rw&&Tiles::mapValues[location+Tiles::width+1].rw&&Tiles::mapValues[location+Tiles::width+Tiles::width+1].rw){
                    return true;
                    
                    
                } else {
                    return false;
                }
                
            } else {
                return false;
            }
            
        } else {
            return false;
            
        }
        
    
    
        break;
    
    
        case ThreeXThree:
            return (Tiles::checkTileOffestForPlacement(location,0,0)&&
                    Tiles::checkTileOffestForPlacement(location,1,0)&&
                    Tiles::checkTileOffestForPlacement(location,0,1)&&
                    Tiles::checkTileOffestForPlacement(location,1,1)&&
                    Tiles::checkTileOffestForPlacement(location,1,1)&&
                    Tiles::checkTileOffestForPlacement(location,2,0)&&
                    Tiles::checkTileOffestForPlacement(location,2,1)&&
                    Tiles::checkTileOffestForPlacement(location,2,2)&&
                    Tiles::checkTileOffestForPlacement(location,0,2)&&
                    Tiles::checkTileOffestForPlacement(location,1,2));
            
            
            
            
            
            
            break;
    }
    
    return false;
    
}


Sprite * Sprite::addSpriteLarge(int _index, int _type){
    
    std::map<int, SpriteDict*>::iterator IT = theSpriteAssets.find(_type);
    
    if (IT != theSpriteAssets.end()){
        
        return addSpriteLarge(_index, (IT->second));
        
    } else {
        return 0;
        
    }
    
    return 0;
}

Sprite * Sprite::addSpriteLarge(int anType, SpriteDict * target){
    
    
    
    
    
    switch (target->placementType) {
        case NoPlacement:
            return 0;
            break;
        case OneXOneLeftWall:
        
        if (testSpriteLarge(anType, target)){
            Sprite * theSprite = addSprite(anType, target->index);
            
            Tiles::mapValues[anType].block=true;
            Tiles::mapValues[anType].occupiedSprite = theSprite;
            
            theSprite->blockedTiles.push_back(anType);
        }
        
        break;
        
            
        case OneXOne:
            if (testSpriteLarge(anType, target)){
                Sprite * theSprite = addSprite(anType, target->index);
                
                Tiles::mapValues[anType].block=true;
                Tiles::mapValues[anType].occupiedSprite = theSprite;
                
                theSprite->blockedTiles.push_back(anType);
                
            }
            
            break;
        case TwoXOneL:
            
            if (testSpriteLarge(anType, target)){
                
                Sprite * theSprite = addSprite(anType, target->index);
                
                Tiles::mapValues[anType].block=true;
                Tiles::mapValues[anType].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType);
                
                
                
                Tiles::mapValues[anType+Tiles::width].block=true;
                Tiles::mapValues[anType+Tiles::width].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+Tiles::width);
                
                
                
                
                
                
            }
            
            break;
        
            
            
        case SixXThreeL:
            
            if (testSpriteLarge(anType, target)){
                
                Sprite * theSprite = addSprite(anType, target->index);
                
                Tiles::mapValues[anType].block=true;
                Tiles::mapValues[anType].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType);
                
                Tiles::mapValues[anType+1].block=true;
                Tiles::mapValues[anType+1].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+1);
                
                Tiles::mapValues[anType+2].block=true;
                Tiles::mapValues[anType+2].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+2);
                
                
                //
                Tiles::mapValues[anType+(1*Tiles::width)+0].block=true;
                Tiles::mapValues[anType+(1*Tiles::width)+0].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(1*Tiles::width)+0);
                
                Tiles::mapValues[anType+(1*Tiles::width)+1].block=true;
                Tiles::mapValues[anType+(1*Tiles::width)+1].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(1*Tiles::width)+1);
                
                Tiles::mapValues[anType+(1*Tiles::width)+2].block=true;
                Tiles::mapValues[anType+(1*Tiles::width)+2].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(1*Tiles::width)+2);
                
                //
                Tiles::mapValues[anType+(2*Tiles::width)+0].block=true;
                Tiles::mapValues[anType+(2*Tiles::width)+0].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(2*Tiles::width)+0);
                
                Tiles::mapValues[anType+(2*Tiles::width)+1].block=true;
                Tiles::mapValues[anType+(2*Tiles::width)+1].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(2*Tiles::width)+1);
                
                Tiles::mapValues[anType+(2*Tiles::width)+2].block=true;
                Tiles::mapValues[anType+(2*Tiles::width)+2].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(2*Tiles::width)+2);
                
                //
                
                Tiles::mapValues[anType+(3*Tiles::width)+0].block=true;
                Tiles::mapValues[anType+(3*Tiles::width)+0].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(3*Tiles::width)+0);
                
                Tiles::mapValues[anType+(3*Tiles::width)+1].block=true;
                Tiles::mapValues[anType+(3*Tiles::width)+1].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(3*Tiles::width)+1);
                
                Tiles::mapValues[anType+(3*Tiles::width)+2].block=true;
                Tiles::mapValues[anType+(3*Tiles::width)+2].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(3*Tiles::width)+2);
                
                //
                
                Tiles::mapValues[anType+(4*Tiles::width)+0].block=true;
                Tiles::mapValues[anType+(4*Tiles::width)+0].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(4*Tiles::width)+0);
                
                Tiles::mapValues[anType+(4*Tiles::width)+1].block=true;
                Tiles::mapValues[anType+(4*Tiles::width)+1].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(4*Tiles::width)+1);
                
                Tiles::mapValues[anType+(4*Tiles::width)+2].block=true;
                Tiles::mapValues[anType+(4*Tiles::width)+2].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(4*Tiles::width)+2);
                
                
                Tiles::mapValues[anType+(5*Tiles::width)+0].block=true;
                Tiles::mapValues[anType+(5*Tiles::width)+0].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(5*Tiles::width)+0);
                
                Tiles::mapValues[anType+(5*Tiles::width)+1].block=true;
                Tiles::mapValues[anType+(5*Tiles::width)+1].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(5*Tiles::width)+1);
                
                Tiles::mapValues[anType+(5*Tiles::width)+2].block=true;
                Tiles::mapValues[anType+(5*Tiles::width)+2].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+(5*Tiles::width)+2);
            }
            
            
            break;
            
        case TwoXEight:
            
            
            break;
            
        case TwoXOneU:
            
            if (testSpriteLarge(anType, target)){
                
                Sprite * theSprite = addSprite(anType, target->index);
                
                Tiles::mapValues[anType].block=true;
                Tiles::mapValues[anType].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType);
                
                Tiles::mapValues[anType+1].block=true;
                Tiles::mapValues[anType+1].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+1);
                
                
                
            }
            
            break;
        
        case TwoXOneULeftWall:
        
        if (testSpriteLarge(anType, target)){
            
            Sprite * theSprite = addSprite(anType, target->index);
            
            Tiles::mapValues[anType].block=true;
            Tiles::mapValues[anType].occupiedSprite = theSprite;
            theSprite->blockedTiles.push_back(anType);
            
            Tiles::mapValues[anType+1].block=true;
            Tiles::mapValues[anType+1].occupiedSprite = theSprite;
            theSprite->blockedTiles.push_back(anType+1);
            
            
            
        }
        
        
        break;
        
        case ThreeXTwoL:
            if (testSpriteLarge(anType, target)){
                
                
                Sprite * theSprite = addSprite(anType, target->index);
                
                Tiles::mapValues[anType].block=true;
                Tiles::mapValues[anType].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType);
                
                Tiles::mapValues[anType+1].block=true;
                Tiles::mapValues[anType+1].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+1);
                
                Tiles::mapValues[anType+Tiles::width].block=true;
                Tiles::mapValues[anType+Tiles::width].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+Tiles::width);
                
                Tiles::mapValues[anType+Tiles::width+1].block=true;
                Tiles::mapValues[anType+Tiles::width+1].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+Tiles::width+1);
                
                Tiles::mapValues[anType+Tiles::width+Tiles::width].block=true;
                Tiles::mapValues[anType+Tiles::width+Tiles::width].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+Tiles::width+Tiles::width);
                
                
                Tiles::mapValues[anType+Tiles::width+Tiles::width+1].block=true;
                Tiles::mapValues[anType+Tiles::width+Tiles::width+1].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+Tiles::width+Tiles::width+1);
                
            }
            
            break;
        case ThreeXOneLRightWall:
        case  ThreeXOneL:
            
            if (testSpriteLarge(anType, target)){
                
                Sprite * theSprite = addSprite(anType, target->index);
                
                Tiles::mapValues[anType].block=true;
                Tiles::mapValues[anType].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType);
                
                
                
                Tiles::mapValues[anType+Tiles::width].block=true;
                Tiles::mapValues[anType+Tiles::width].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+Tiles::width);
                
                
                
                Tiles::mapValues[anType+Tiles::width+Tiles::width].block=true;
                Tiles::mapValues[anType+Tiles::width+Tiles::width].occupiedSprite = theSprite;
                theSprite->blockedTiles.push_back(anType+Tiles::width+Tiles::width);
                
                
            }
            
            break;
            
        case ThreeXTwoU:
        {
            
            Sprite * theSprite = addSprite(anType, target->index);
            
            Tiles::mapValues[anType].block=true;
            Tiles::mapValues[anType].occupiedSprite = theSprite;
            theSprite->blockedTiles.push_back(anType);
            
            Tiles::mapValues[anType+1].block=true;
            Tiles::mapValues[anType+1].occupiedSprite = theSprite;
            theSprite->blockedTiles.push_back(anType+1);
            
            Tiles::mapValues[anType+2].block=true;
            Tiles::mapValues[anType+2].occupiedSprite = theSprite;
            theSprite->blockedTiles.push_back(anType+2);
            
            Tiles::mapValues[anType+Tiles::width].block=true;
            Tiles::mapValues[anType+Tiles::width].occupiedSprite = theSprite;
            theSprite->blockedTiles.push_back(anType+Tiles::width);
            
            Tiles::mapValues[anType+Tiles::width+1].block=true;
            Tiles::mapValues[anType+Tiles::width+1].occupiedSprite = theSprite;
            theSprite->blockedTiles.push_back(anType+Tiles::width+1);
            
            Tiles::mapValues[anType+Tiles::width+2].block=true;
            Tiles::mapValues[anType+Tiles::width+2].occupiedSprite = theSprite;
            theSprite->blockedTiles.push_back(anType+Tiles::width+2);
            
        }
            
            break;
            
        case TwoXTwo:
        {
            if (testSpriteLarge(anType, target)){
                
                Sprite * theSprite = addSprite(anType, target->index);
                
                Tiles::mapValues[anType].block=true;
                Tiles::mapValues[anType].occupiedSprite = theSprite;
                Tiles::mapValues[anType+1].block=true;
                Tiles::mapValues[anType+1].occupiedSprite = theSprite;
                Tiles::mapValues[anType+Tiles::width].block=true;
                Tiles::mapValues[anType+Tiles::width].occupiedSprite = theSprite;
                Tiles::mapValues[anType+Tiles::width+1].block=true;
                Tiles::mapValues[anType+Tiles::width+1].occupiedSprite=theSprite;
                
                theSprite->blockedTiles.push_back(anType);
                theSprite->blockedTiles.push_back(anType+1);
                theSprite->blockedTiles.push_back(anType+Tiles::width);
                theSprite->blockedTiles.push_back(anType+Tiles::width+1);
                
                return  theSprite;
            }
            
        }
            break;
        case ThreeXThree:
            
            if (testSpriteLarge(anType, target)){
                
                Sprite * theSprite = addSprite(anType, target->index);
                
                Tiles::mapValues[anType].block=true;
                Tiles::mapValues[anType].occupiedSprite = theSprite;
                Tiles::mapValues[anType+1].block=true;
                Tiles::mapValues[anType+1].occupiedSprite = theSprite;
                Tiles::mapValues[anType+Tiles::width].block=true;
                Tiles::mapValues[anType+Tiles::width].occupiedSprite = theSprite;
                Tiles::mapValues[anType+Tiles::width+1].block=true;
                Tiles::mapValues[anType+Tiles::width+1].occupiedSprite=theSprite;
                
                Tiles::mapValues[anType+Tiles::width+Tiles::width].block=true;
                Tiles::mapValues[anType+Tiles::width+Tiles::width].occupiedSprite = theSprite;
                Tiles::mapValues[anType+Tiles::width+Tiles::width+1].block=true;
                Tiles::mapValues[anType+Tiles::width+Tiles::width+1].occupiedSprite = theSprite;
                Tiles::mapValues[anType+Tiles::width+Tiles::width+2].block=true;
                Tiles::mapValues[anType+Tiles::width+Tiles::width+2].occupiedSprite = theSprite;
                Tiles::mapValues[anType+2].block=true;
                Tiles::mapValues[anType+2].occupiedSprite = theSprite;
                Tiles::mapValues[anType+Tiles::width+2].block=true;
                Tiles::mapValues[anType+Tiles::width+2].occupiedSprite = theSprite;
                
                
                theSprite->blockedTiles.push_back(anType);
                theSprite->blockedTiles.push_back(anType+1);
                theSprite->blockedTiles.push_back(anType+Tiles::width);
                theSprite->blockedTiles.push_back(anType+Tiles::width+1);
                
                theSprite->blockedTiles.push_back(anType+Tiles::width+Tiles::width);
                theSprite->blockedTiles.push_back(anType+Tiles::width+Tiles::width+1);
                theSprite->blockedTiles.push_back(anType+Tiles::width+Tiles::width+2);
                theSprite->blockedTiles.push_back(anType+2);
                theSprite->blockedTiles.push_back(anType+Tiles::width+2);
                
                
                
                
                
                return theSprite;
                
            }
            
            
            break;
            
            
    }
    
    

    
    
    return 0;
    
    
}




//static Sprite * selectedSprite;
#define TOUCHSPACE 5


void Sprite::deselectAll(){
    
    
    for (std::map<size_t, Sprite*>::iterator IT=Sprite::spriteByID.begin();Sprite::spriteByID.end()!=IT; IT++){

        
        IT->second->isSelected = false;
        
    }
    
    
    
    
    
}
/*

static void setAnimation(cpShape *shape, void *data){
    
    std::map<cpShape*, Sprite*>::iterator IT = Sprite::levelSprites.find(shape);
    
    int * theValue = (int*)(data);
    if (IT!=Sprite::levelSprites.end()){
        IT->second->setAnimationState(*theValue);
        
    }
    
}

*/





void Sprite::mouseDebugGame(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
    printf("This Function No Long Workers 8394");

    
}

std::string Sprite::speak(){
    
    double currentHealth = health/dict->health;
    
    if (currentHealth>0.75){
        
        return dict->dialog["Fine"];
        
    } else if (currentHealth>0.25){
        
        return dict->dialog["Feared"];
        
    } else {
        
        return dict->dialog["Scared"];
        
    }
    
    
    
}

bool Sprite::mouseInputFind(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
    
    std::pair<int,int> tilePos = Tiles::findTile(cpvadd(control,cpv(-Camera::posX,-Camera::posY)));
    int index = Tiles::findTile(tilePos);
    
    if (index>0 && index<Tiles::mapValues.size()){
        
        std::vector<Sprite*> * theSprites = &Tiles::mapValues[index].renderGrid;
        
        for (std::vector<Sprite*>::iterator IT = theSprites->begin();
             IT!= theSprites->end();
             IT++){
            
            if (!(*IT)->dict->prop){
                
				BobZombie::yPosSlide = 0;
                BobZombie::setSprite((*IT)->dict);
                BobZombie::toggleState();
                
                return true;
            }
            
        }
        
    }
   /*
    tilePos.first +=1;
    tilePos.second -=1;
    
    index = Tiles::findTile(tilePos);
    
    if (index>0 && index<Tiles::mapValues.size()){
        
        std::vector<Sprite*> * theSprites = &Tiles::mapValues[index].renderGrid;
        
        for (std::vector<Sprite*>::iterator IT = theSprites->begin();
             IT!= theSprites->end();
             IT++){
            
            if (!(*IT)->dict->prop){
                
                BobZombie::setSprite((*IT)->dict);
                BobZombie::toggleState();
                
                return true;
            }
            
        }
        
    }
    */
    return false;
    
    
}



bool Sprite::mouseInputGame(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
 
    Tiles::trapRadius.clear();
    
    
    if (input==MOUSE_STATE_HOVER){
        deselectAll();
        

        
        
        
        std::pair<int,int> tilePos = Tiles::findTile(cpvadd(control,cpv(-Camera::posX,-Camera::posY)));
        int index = Tiles::findTile(tilePos);
        
        overSpriteForRadius = false;
        
        if (index>0 && index<Tiles::mapValues.size()){
            
            std::vector<Sprite*> * theSprites = &Tiles::mapValues[index].renderGrid;
            
            for (std::vector<Sprite*>::iterator IT = theSprites->begin();
                 IT!= theSprites->end();
                 IT++){
                
                
                if (Sprite::trees.find((*IT)->dict->index)==Sprite::trees.end()){
                    if (!(*IT)->dict->prop){
                        (*IT)->isSelected = true;
                    }
//                    (*IT)->dict->animates&&
                    if ((*IT)->dict->prop&&(*IT)->dict->animates){
                        
                        std::vector<int> value = Sprite::getRadiusAroundLargeSprite(*IT);
                        
                        std::sort(value.begin(), value.end());
                        Tiles::trapRadius = std::set<int>(value.begin(),value.end());
                        //trapRadius
                        
                    
                    }
                }
            
                
                
                
            }
        }
        
        
        tilePos.first -= 1;
        tilePos.second -= 1;
        
        index = Tiles::findTile(tilePos);
        
        if (index>0 && index<Tiles::mapValues.size()){
            
            std::vector<Sprite*> * theSprites = &Tiles::mapValues[index].renderGrid;
            
            for (std::vector<Sprite*>::iterator IT = theSprites->begin();
                 IT!= theSprites->end();
                 IT++){
                
                
                if (Sprite::trees.find((*IT)->dict->index)==Sprite::trees.end()){
                    if (!(*IT)->dict->prop){
                        (*IT)->isSelected = true;
                    }
                }
                
                
                
                
            }
        }
        
        
        
    }
    
    if (input==MOUSE_STATE_UP){
        if (BobZombie::peopleCanTalk()){
            
            std::pair<int,int> tilePos = Tiles::findTile(cpvadd(control,cpv(-Camera::posX,-Camera::posY)));
            int index = Tiles::findTile(tilePos);
            
            if (index>0 && index<Tiles::mapValues.size()){
                
                std::vector<Sprite*> * theSprites = &Tiles::mapValues[index].renderGrid;
                
                for (std::vector<Sprite*>::iterator IT = theSprites->begin();
                     IT!= theSprites->end();
                     IT++){
                    
                    if (!(*IT)->dict->prop){
                        std::string text = (*IT)->speak();
                        
                        UserInterface::spawnTextBox(text, 2.0, (*IT)->dict);
                        return true;
                    }
                    
                }
            }
            
            tilePos.first -=1;
            tilePos.second -=1;
            
            index = Tiles::findTile(tilePos);
            
            if (index>0 && index<Tiles::mapValues.size()){
                
                std::vector<Sprite*> * theSprites = &Tiles::mapValues[index].renderGrid;
                
                for (std::vector<Sprite*>::iterator IT = theSprites->begin();
                     IT!= theSprites->end();
                     IT++){
                    
                    if (!(*IT)->dict->prop){
                        std::string text = (*IT)->speak();
                        
                        UserInterface::spawnTextBox(text, 2.0, (*IT)->dict);
                        return true;
                    }
                    
                }
            }
            
        }
    }
    
    return false;
}


void Sprite::mouseInputDeleteAfterCameraMath(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
    
     std::pair<int,int> tilePos = Tiles::findTile(control);
    int index = Tiles::findTile(tilePos);
    
    
    if (index>0&&index<Tiles::mapValues.size()){
        
        Sprite * theSprite = Tiles::mapValues[index].occupiedSprite;
        
        if (theSprite){
            
            if (theSprite->dict->prop){
                if (Sprite::trees.find(theSprite->type)==Sprite::trees.end()){
                    GameLogic::energy += theSprite->dict->cost/2.0;
                    Sprite::removeSprite(theSprite);
					return;
                }
                
            }
        }
    }

	tilePos.first -= 1;
	tilePos.second -= 1;

	index = Tiles::findTile(tilePos);

	if (index>0 && index<Tiles::mapValues.size()){

		Sprite * theSprite = Tiles::mapValues[index].occupiedSprite;

		if (theSprite){

			if (theSprite->dict->prop){
				if (Sprite::trees.find(theSprite->type) == Sprite::trees.end()){
					GameLogic::energy += theSprite->dict->cost / 2.0;
					Sprite::removeSprite(theSprite);
					return;
				}

			}
		}
	}
    
    
}
void Sprite::mouseInputDelete(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
 
    control = cpvmult(control, Camera::zoom);
    
    control = cpvadd(control,cpv(-Camera::posX,-Camera::posY));
    
    mouseInputDeleteAfterCameraMath(input, control,theTouchNumber);
    
    
    
}

Sprite * Sprite::moveSprite;


void Sprite::mouseInputC(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
    
    std::pair<int,int> tilePos = Tiles::findTile(cpvadd(control,cpv(-Camera::posX,-Camera::posY)));
    int index = Tiles::findTile(tilePos);
    
    if (input==MOUSE_STATE_UP){
        
        if (moveSprite){
			if (canMove){
				if (testSpriteLarge(index, moveSprite->dict)){

					SpriteDict * theDict = moveSprite->dict;
					Sprite::removeSprite(moveSprite);
					Sprite * aSprite = Sprite::addSpriteLarge(index, theDict);
					if (aSprite){
						changeSelect(aSprite->dict->index);
						Sprite::moveSprite = aSprite;
						canMove = true;
					}
					return;
				}

			}
        }
        
        
        
        Sprite * theTarget = Tiles::mapValues[index].occupiedSprite;
        if (theTarget){
            if (theTarget->dict->prop&&!theTarget->dict->animates&&!theTarget->dict->aTree){
                changeSelect(theTarget->type);
                Sprite::moveSprite =  theTarget;
				canMove = false;
            }
        }
        
    }
    
}


void Sprite::mouseInput(MOUSE_STATES input, cpVect control, long theTouchNumber){
    
    DebugPosition=cpvadd(control,cpv(-Camera::posX,-Camera::posY));
    
    
    bool propSpaceOccupied = false;
    
    

    
    if (mouseInputGame(input, control, theTouchNumber)){
        return;
    }
    
    std::pair<int,int> tilePos = Tiles::findTile(cpvadd(control,cpv(-Camera::posX,-Camera::posY)));
    int index = Tiles::findTile(tilePos);
    
    
    if(GameLogic::gameMode==PROPMODE){
        if (index>0&&index<Tiles::width*Tiles::width) {
        
            if (Tiles::mapValues[index].occupiedSprite) {
            
            TranslationMatrix::warning(PRESS_RIGHT_CLEAR);
            }
        }
    }
 
    
    
    
    std::map<int, SpriteDict*>::iterator IT = theSpriteAssets.find((int)Sprite::changeSpriteType);
    
    if (IT ==theSpriteAssets.end()){
        spriteCostToMuch=false;
    } else {
        if (IT->second->cost>GameLogic::energy){
            spriteCostToMuch=true;
            
        } else {
            if (IT->second->prop){
                
                if (testSpriteLarge(index, IT->second)){
                    dontRenderTest=1;
                    spriteCostToMuch=false;
                } else {
                    propSpaceOccupied = true;
                    dontRenderTest=0;
                    spriteCostToMuch=true;
                }
            } else {
                
                if (Tiles::isTileOccupied(index)){
                    dontRenderTest=0;
                }else {
                    dontRenderTest=1;
                }
                
                spriteCostToMuch=false;
                
            }
        }
    }
    
    
    if ((index<0||index>=Tiles::width*Tiles::width) ){
        return;
    } else {
        Tiles::hoverTile = index;
        if (input==MOUSE_STATE_UP){
            if (changeSpriteType!=-1){
                if (IT ==theSpriteAssets.end()){
                    Sprite::mouseInputGame(input, control, theTouchNumber);
                } else {
                    if (IT->second->prop){
                        if (!propSpaceOccupied){
                            if (!spriteCostToMuch){
                                
                                if (IT->second->animates&&IT->second->prop){
                                    
                                    if (
                                        Tiles::mapValues[index].type<2){
                                        
                                        TranslationMatrix::warning(NOT_INSIDE);
                                        return;
                                        
                                    }
                                    
                                    std::vector<int> theInfo = Tiles::getSizeOrRoom(index);
                                    
                      
                                    
                                    
                                    
                                    if (theInfo[0]<49&&theInfo[1]<5&&theInfo[2]<5){
                                        TranslationMatrix::warning(ROOM_WRONG_SIZED);
                                        return;
                                    }
                                    
                                    if (theInfo[7]>=1){
                                        TranslationMatrix::warning(OBJECT_IN_ROOM);
                                        return;
                                        
                                    }
                                    
                                   
                                   
                                    
                                    
                                    
                                }
                                
                                
                                addSpriteLarge(index, IT->second);
                                 GameLogic::energy -= IT->second->cost;
                                 AudioController::playAudio("Interface_Accept.wav");
                                 Tiles::checkWalkAbility();
                            } else {
                                TranslationMatrix::warning(NOT_ENOUGH_ENERGY);
                            }
                        } else {
                            
                            
                            
                               
                            TranslationMatrix::warning(SPACE_OCCUPIED);
                            
                        }
                    } else {
                        if (!spriteCostToMuch){
                            
                            bool occupied = Tiles::isTileOccupied(index);
                            
                            
                            if (!occupied){
                                
                                
                                
                                
                                
                                Sprite * theSprite = addSprite(index,(int)Sprite::changeSpriteType);
                                
                                if (theSprite->dict->prop){
                                    Tiles::mapValues[index].block=true;
                                    theSprite->blockedTiles.push_back(index);
                                    Tiles::mapValues[index].occupiedSprite = theSprite;
                                }
                                GameLogic::energy -= IT->second->cost;
                                AudioController::playAudio("Interface_Accept.wav");
                                
                                Tiles::checkWalkAbility();
                            } else {
                                TranslationMatrix::warning(SPACE_OCCUPIED);
                                
                                
                                
                            }
                        } else {
                            TranslationMatrix::warning(NOT_ENOUGH_ENERGY);
                        }
                    }
                }
            } else {
                Sprite::mouseInputGame(input, control, theTouchNumber);
            }
        }
    }
}















std::set<size_t> Sprite::gridsLastTurn;


bool sortSpriteFunction(Sprite*s1, Sprite*s2){
    
    cpVect pos1, pos2;
    pos1 = s1->position;
    pos2 = s2->position;
    
    return pos1.y<pos2.y;
}

#include "SaveMap.h"

void Sprite::updateAll(){
    
    
    std::set<size_t>::iterator IT;
    
    std::vector<Sprite*> blank;
    
    
    
    for (IT = Sprite::gridsLastTurn.begin();
         IT != Sprite::gridsLastTurn.end();
         IT++
         ){
        Tiles::mapValues[*IT].renderGrid.clear();
    }
    
    Sprite::gridsLastTurn.clear();
    
    std::vector<Sprite*> aSpriteAdd;
    
    for (std::map<size_t, Sprite*>::iterator IT=Sprite::spriteByID.begin();Sprite::spriteByID.end()!=IT; IT++){

        Sprite * theSprite = IT->second;
        theSprite->updateNoGL();
        

        if (Camera::isBoundBoxVisible(theSprite->boundingBox.l,
                                      theSprite->boundingBox.b,
                                      theSprite->boundingBox.r,
                                      theSprite->boundingBox.t)){
            
            theSprite->updateGL();
            aSpriteAdd.push_back(theSprite);
        
            theSprite->lastSeen = GameClock::time;
        } else {
            
        
        
        }

        
        cpVect pos = IT->second->minPoint;
        
        
        int index = Tiles::findTile(Tiles::findTile(pos));
        
        
            IT->second->renderGridIndex = index;
        
        
        
        if (index>=0&&Tiles::mapValues.size()>index){
            
            
            if(theSprite->lastSeen==GameClock::time){
                Sprite::gridsLastTurn.insert(index);
                Tiles::mapValues[index].renderGrid.push_back(IT->second);
                IT->second->renderGridIndex = index;
            }
            
        } else {
            
        }

    }
    
    
    
    for (IT = Sprite::gridsLastTurn.begin();
         IT != Sprite::gridsLastTurn.end();
         IT++
         ){
        
        
        std::sort(
                  Tiles::mapValues[*IT].renderGrid.begin(),
                  Tiles::mapValues[*IT].renderGrid.end(),
                  sortSpriteFunction);
        
    }
    
  
    Sprite * pSprite = Player::mainPlayer;
    
    pSprite->updateNoGL();
    pSprite->updateGL();
    
    cpVect pos =pSprite->minPoint;
    
    
    pSprite->renderGridIndex = Tiles::findTile(Tiles::findTile(pos));
    
    
    aSpriteAdd.push_back(pSprite);
    aSpriteAdd.push_back(Sprite::testSprite);
    
    SaveMap::update(&aSpriteAdd);
    
    
}



void Sprite::renderAll(){
    ShaderManager::enableTextureShader();
for (std::map<size_t, Sprite*>::iterator IT=Sprite::spriteByID.begin();Sprite::spriteByID.end()!=IT; IT++){
        IT->second->render();
    }
    
    
}

cpBB Sprite::getDimension(){
    
    SpriteDict * theDict =dict;
    
    return cpBBNew(-theDict->width/2.0f,-theDict->height/2.0f,theDict->width/2.0f,theDict->height/2.0f);
}

void Sprite::updateSelect(){
    
    
    
}



/*
void grabSprites(cpShape *shape, void *data){
    
    std::vector<Sprite*> * theShapes = (std::vector<Sprite*>* )data;
    
    std::map<cpShape*, Sprite*>::iterator IT = Sprite::levelSprites.find(shape);
    
    if (IT!=Sprite::levelSprites.end()){
        
        theShapes->push_back(IT->second);
        
    }
}
*/



void Sprite::summonAllSprites(cpVect location, double radius){
    
    printf("No Run");
    
    /*
     std::vector<Sprite*> returnShapes = Sprite::getAllSprite(location, radius);
     
     for (std::vector<Sprite*>::iterator IT = returnShapes.begin(); IT !=returnShapes.end(); IT++){
     
     Sprite * theSprite = (*IT);
     
     theSprite->summon(location);
     
     
     
     }
     */
    
}


void Sprite::summon(cpVect location){
    
    if (dict->prop){
        return;
    }
    
    std::pair<int,int> target;
    
    
    int tileToGoTo = Tiles::findTile(Tiles::findTile(location));
    
    if(!findPath(tileToGoTo)){
        idle();
    }
    
    
    
    
    
}

void Sprite::scareAllSpritesGood(cpVect location, int radius, SpriteDict * target, long uid_i){

   // int index =  Tiles::findTile(Tiles::findTile(location));
    
   
    std::vector<Sprite*> returnShapes;
    std::vector<int> tilesV = getRadiusAroundLargeSprite(this);
    
    for (std::vector<int>::iterator IT = tilesV.begin(); IT != tilesV.end(); IT++) {
        TileObject * theTile = &Tiles::mapValues[*IT];
        
        for (std::vector<Sprite*>::iterator IT2 = theTile->renderGrid.begin();
             IT2 != theTile->renderGrid.end();
             IT2++
             ){
        
            if (!(*IT2)->dict->prop){
                returnShapes.push_back(*IT2);
            }
        }
    }
    
    for (std::vector<Sprite*>::iterator IT = returnShapes.begin(); IT !=returnShapes.end(); IT++){
        
        Sprite * theSprite = (*IT);
        
        if (!checkIfBadGuy(theSprite)){
            
            if (
                theSprite->scaredByList.find(uid_i)!=theSprite->scaredByList.end()){
                continue;
                
            }
            
            
            if (theSprite->dict->trait==IDIOT){
            
                theSprite->scareIdiot(location,target, this);
                
                
                
            
            }else {
            
            
            
            
                
                if (target){
                    theSprite->scare(location, target);
                } else {
                    theSprite->scare(location);
                    
                }
                 theSprite->scaredByList.insert(uid_i);
            }
           
            
        }
    }
}


void Sprite::scareAllSprites(cpVect location, int radius, SpriteDict * target, long uid_i){
    
    
    int index =  Tiles::findTile(Tiles::findTile(location));
    
    std::vector<Sprite*> returnShapes = Tiles::grabAllSpriteLos(index, radius);
    
    for (std::vector<Sprite*>::iterator IT = returnShapes.begin(); IT !=returnShapes.end(); IT++){
        
        Sprite * theSprite = (*IT);
        
        if (
            theSprite->scaredByList.find(uid_i)!=theSprite->scaredByList.end()){
            continue;
        }
        
        if (theSprite->dict->trait==VIVIAN){
            continue;
        }else if (theSprite->dict->trait==GAMER){
            continue;
        
        }else if (theSprite->dict->trait==ZOMBIE){
            continue;
        }
        
        if (target){
            theSprite->scare(location, target);
        } else {
            theSprite->scare(location);
            
        }
        theSprite->scaredByList.insert(uid_i);
        
    }
    
    
    
}





void Sprite::scareAllSprites(cpVect location, int radius, SpriteDict * target){
    
    
    int index =  Tiles::findTile(Tiles::findTile(location));
    
    std::vector<Sprite*> returnShapes = Tiles::grabAllSpriteLos(index, radius);
    
    for (std::vector<Sprite*>::iterator IT = returnShapes.begin(); IT !=returnShapes.end(); IT++){
        
        Sprite * theSprite = (*IT);
        
        theSprite->scare(location, target);
        
    }
    
    
    
}

void Sprite::scareAllSprites(cpVect location, int radius){
    
    int index =  Tiles::findTile(Tiles::findTile(location));
    
    
    std::vector<Sprite*> returnShapes = Tiles::grabAllSpriteLos(index, radius);
    
    for (std::vector<Sprite*>::iterator IT = returnShapes.begin(); IT !=returnShapes.end(); IT++){
        
        Sprite * theSprite = (*IT);
        
        theSprite->scare(location);
        
        
        
    }
    
    
    
    
}

bool Sprite::die(SpriteDict * _target){

    
    std::pair<ScareType, ScareType> data = findScareModifier(_target);
    

    GameLogic::awareness += (data.first.awarenessModifier *data.second.awarenessModifier)*15.0;
    
    
    int min = dict->frames[DIE_FRAME];
    int max = dict->frames[DIE_FRAME+1];
    
    if (frame>=min&&frame<max){
        return false;
    }
    
    
    AudioController::playAudioLua(dict->index, "Die");
    setAnimationState(DIE_FRAME);
    return true;

}


bool Sprite::die(){
    
    
    int min = dict->frames[DIE_FRAME];
    int max = dict->frames[DIE_FRAME+1];
    
    if (frame>=min&&frame<max){
        return false;
    }
    
    
    AudioController::playAudioLua(dict->index, "Die");
    setAnimationState(DIE_FRAME);
    return true;
    
}


bool Sprite::extraScareEnergyFromCouple(){

    
     std::vector<Sprite*> theFoundSprites = Tiles::grabAllSprite(renderGridIndex, 4);
    
    
    for (std::vector<Sprite*>::iterator IT = theFoundSprites.begin();
         IT != theFoundSprites.end();
         IT++){
    
        if ((*IT)->dict->trait == COUPLE){
            if ((*IT)!=this){
     
                return false;
            
            }
        
        
        }
    }
    
   
    return true;

}

#include "Achievements.h"

int scareAPerson =0 ;

void Sprite::addText(std::string theText){

    addText(theText,233.0/255.0,204.0/255.0,185.0/255.0);

}

void Sprite::addText(std::string theText,GLfloat red, GLfloat green, GLfloat blue){

    
    CBitmapFont * aText = new CBitmapFont;
    aText->setText(theText);
    aText->color[0]=red;
    aText->color[1]=green;
    aText->color[2]=blue;
    
    
    GLfloat theWidthValue = GameLogic::width*(3.0/100.0);
    aText->setMod(theWidthValue);
    theTexts.push_back(aText);

}


void Sprite::scareFromPlayer(cpVect location,  ScareDict * _target){
    
    SpriteDict proxy;
    
    
    proxy.scareByScareModifier = _target->scareByScareModifier;
    proxy.scareTypeByName = _target->scareTypeByName;

    std::pair<ScareType, ScareType> theType = findScareModifier(&proxy);
    
    
    
    double scarePower = theType.first.scareModifier * theType.second.scareModifier;
    health -= theType.first.damageModifier * theType.second.damageModifier;
    GameLogic::awareness += theType.first.awarenessModifier * theType.second.awarenessModifier;
    
    GameLogic::energy +=scarePower;
    
    scareAPerson++;
    
    if (scareAPerson>=100){
        Achievement::scareAPersion();
    
    }
    
    switch (dict->trait) {
        case EXORCIST:
            
            Achievement::scareExorcist();
            break;
        case REPORTER:
            Achievement::scareReporter();
            break;
        case POLICE:
            Achievement::scarePolice();
            break;
        case GHOSTHUNTER:
            Achievement::scareGhostHunter();
            break;
            

      
        
            
        default:
            break;
    }
    
   
    
    
    
    
    if (health>0 || !BobZombie::peopleCanDie()){
        
        
        
        if (dict->trait == COUPLE){
            
            if (extraScareEnergyFromCouple()){
                GameLogic::energy +=scarePower*11;
            
                scarePower +=scarePower*11;
            }
            
            
        }
        
        
  
        
        addText(std::to_string(int(scarePower)));
        
        
        {
            
            
            std::pair<int,int> target;
            
            cpVect currentPos = position;
            
            if (currentPos.y<=location.y){
                //GO DOWN
                target.second=0;
            } else {
                //GO UP
                target.second = Tiles::height-1;
            }
            
            if (currentPos.x<=location.x){
                //GO LEFT
                target.first = 0;
            } else {
                //GO RIGHT
                target.first = Tiles::width-1;
            }
            
            
            
            
            int tileToGoTo = Tiles::findTile(target);
            
            if(!findPath(tileToGoTo)){
                die(&proxy);
                return;
            }
            
            isScared=true;
            setAnimationState(SCREAM_FRAME);
            
            
            AudioController::playAudioLua(dict->index, "Scream");
            
            
            
            
            
        }
        
        
        
        
        
    }else{
        
        die(&proxy);
        
    }
    
    
    
    
    
    
    
    return;
    
   
    
    

}



std::pair<ScareType,ScareType> Sprite::findScareModifier(SpriteDict * _scare){

    
    for (
         std::multimap<double, ScareType>::reverse_iterator IT = _scare->scareByScareModifier.rbegin();
         IT != _scare->scareByScareModifier.rend();
         IT++
         ){
        
        
        
        std::map<std::string, ScareType>::iterator IT2 = dict->scareTypeByName.find(IT->second.name);
        
        
        
        if (IT2!=dict->scareTypeByName.end()){
            
            ScareType A = IT->second;
            ScareType B = IT2->second;
            return std::pair<ScareType, ScareType>(A,B);
        }
        
    }
    
    {
        std::map<std::string, ScareType>::iterator IT2 = dict->scareTypeByName.find("Default");
        
        std::map<std::string, ScareType>::iterator IT = _scare->scareTypeByName.find("Default");
      
        
        
        if (IT2!=dict->scareTypeByName.end()){
            ScareType A = IT->second;
            ScareType B = IT2->second;
            return std::pair<ScareType, ScareType>(A,B);
        }
    }
    
    ScareType A  = ScareType(false);
    ScareType B = ScareType(false);
    
    return std::pair<ScareType, ScareType>(A,B);
}

double Sprite::resolveScare(SpriteDict * _scare){
    
    std::pair<ScareType, ScareType> theType = findScareModifier(_scare);
    
   
    
    double scarePower = theType.first.scareModifier * theType.second.scareModifier;
    health -= theType.first.damageModifier * theType.second.damageModifier;
    GameLogic::awareness += theType.first.awarenessModifier * theType.second.awarenessModifier;
    
    GameLogic::energy +=scarePower;
    return scarePower;
    
    
    /*
    
    for (
         std::multimap<double, ScareType>::reverse_iterator IT = _scare->scareByScareModifier.rbegin();
         IT != _scare->scareByScareModifier.rend();
         IT++
         ){
        
        
        
        std::map<std::string, ScareType>::iterator IT2 = dict->scareTypeByName.find(IT->second.name);
        
        
        
        if (IT2!=dict->scareTypeByName.end()){
            
            double scarePower = IT->second.scareModifier * IT2->second.scareModifier;
            health -= IT->second.damageModifier * IT2->second.damageModifier;
            
            GameLogic::energy +=scarePower;
            return scarePower;
            
        }
        
    }
    
    {
        
        std::map<std::string, ScareType>::iterator IT2 = dict->scareTypeByName.find("Default");
        
        std::map<std::string, ScareType>::iterator IT = _scare->scareTypeByName.find("Default");
        
        if (IT2!=dict->scareTypeByName.end()){
            
            double scarePower = IT->second.scareModifier * IT2->second.scareModifier;
            health -= IT->second.damageModifier * IT2->second.damageModifier;
            
            GameLogic::energy +=scarePower;
            return scarePower;
            
        }
        
        
        
        
    }
    
    
    
    
    
    return 0;
    */
}


bool Sprite::scareTraitFactors(){

    if (dict->trait==NORMAL_CITIZEN){
        return true;
    }
    
    switch (dict->trait) {
        case EXORCIST:
        case POLICE:
        case ZOMBIE:
        case GAMER:
        case VIVIAN:
        case REPORTER:
        case RENOVATOR:
            if (isScared){
                return true;
            } else {
                return false;
            }
    default:
            return true;
            
    }
    
    return true;
}

void Sprite::scareFast(cpVect location){
    
    
    isScared = true;
    std::pair<int,int> target;
    
    cpVect currentPos = position;
    
    if (currentPos.y<=location.y){
        //GO DOWN
        target.second=0;
    } else {
        //GO UP
        target.second = Tiles::height-1;
    }
    
    if (currentPos.x<=location.x){
        //GO LEFT
        target.first = 0;
    } else {
        //GO RIGHT
        target.first = Tiles::width-1;
    }
    
    
    //int currentTile = Tiles::findTile(Tiles::findTile(currentPos));
    int tileToGoTo = Tiles::findTile(target);
    
    if(!findPath(tileToGoTo)){
        die();
        return;
    }
    
    
    
}

void Sprite::scare(cpVect location, SpriteDict * _target){
    
    if (dict->prop){
        return;
    }
    if (direction==SCREAM_FRAME){
        return;
    }
    
    if (direction==DIE_FRAME){
        return;
    }
    
    if (!scareTraitFactors()){
        return;
        
    }
 
    
    
    double scareAmount = resolveScare(_target);
    
    if (health>0||!BobZombie::peopleCanDie()||testTrait(DOCTOR, 15)){
        
        addText(std::to_string(int(scareAmount)));

    } else {
        
        die(_target);
        
        return;
        
    }
    
    scare(location);
    
    
    
    
    
    
    
}


void  Sprite::scareIdiot(cpVect location,SpriteDict * target, Sprite * trap){

    if (dict->prop){
        return;
    }
    if (direction==SCREAM_FRAME){
        return;
    }
    
    if (direction==DIE_FRAME){
        return;
    }
    
    if (trapsByUID.size()<2){
    
        scare(location, target);
        return;
    }
    
    if ((*scaredByList.begin())==trap->UID){
        return;
    }
    
    
    
    int thePos;
    while (true) {
        
        scaredByList.clear();
        scaredByList.insert(trap->UID);
        
    
    std::map<size_t, Sprite*>::iterator IT = trapsByUID.begin();
    
    std::advance(IT, rand()%trapsByUID.size());
    
        
        if (IT->second != trap){
        
            thePos = IT->second->renderGridIndex;
        
            break;
        }
    
    
        
    
    }
    
    
    bool runningSystem = true;
    
    while (runningSystem ) {
        switch (rand()%4) {
            case 0:
                if (Tiles::canMove(thePos, 0)){
                    thePos-=1;
                    runningSystem = false;
                };
                break;
            case 1:
                if (Tiles::canMove(thePos, 1)){
                    
                    thePos+=Tiles::width;
                    runningSystem = false;
                }
                break;
            case 2:
                Tiles::canMove(thePos, 2);
                if (Tiles::canMove(thePos, 0)){
                    thePos+=1;
                    runningSystem = false;
                };
                break;
            case 3:
                if (
                    Tiles::canMove(thePos, 3)){
                    thePos-=Tiles::width;
                    runningSystem = false;
                }
                break;
                
            default:
                break;
        }
        
        
        
    }
    
    if (runningSystem==true){
    
    
        scare(location, target);
        return;
    
    }
    
    
    if(!findPath(thePos)){
        idle();
        return;
    }
    
    isScared=true;
    setAnimationState(SCREAM_FRAME);
    
  
    
    
    
    double scareAmount = resolveScare(target);
    
    if (health>0||!BobZombie::peopleCanDie()||testTrait(DOCTOR, 15)){
        
        addText(std::to_string(int(scareAmount)));
   
    } else {
        
        die(target);
        
        return;
        
    }
    
    
    AudioController::playAudioLua(dict->index, "Scream");
    
    

}

bool Sprite::isDead(){

    return direction==DIE_FRAME;

}




void Sprite::scare(cpVect location){
    
    if (dict->prop){
        return;
    }
    if (direction==SCREAM_FRAME){
        return;
    }
    
    if (direction==DIE_FRAME){
        return;
    }
    
    std::pair<int,int> target;
    
    cpVect currentPos = position;
    
    if (currentPos.y<=location.y){
        //GO DOWN
        target.second=0;
    } else {
        //GO UP
        target.second = Tiles::height-1;
    }
    
    if (currentPos.x<=location.x){
        //GO LEFT
        target.first = 0;
    } else {
        //GO RIGHT
        target.first = Tiles::width-1;
    }
    
    
    //int currentTile = Tiles::findTile(Tiles::findTile(currentPos));
    int tileToGoTo = Tiles::findTile(target);
    
    if(!findPath(tileToGoTo)){
        idle();
        return;
    }
    
    isScared=true;
    setAnimationState(SCREAM_FRAME);
    
    
    AudioController::playAudioLua(dict->index, "Scream");
    
}


void Sprite::purgeAll(){
    
    
    
    for (std::map<size_t, Sprite*>::iterator IT=Sprite::spriteByID.begin();Sprite::spriteByID.end()!=IT; IT++){
        IT->second->destroy(true);
        delete IT->second;
        
    }
    
    spriteByID.clear();
    graveYard.clear();
}

void Sprite::changeSelect(size_t type){
    
    Sprite::changeSpriteType=type;
    debugDict = theSpriteAssets[(int)type];
    Sprite::moveSprite = 0;
	Sprite::canMove = false;
    
    if (testSprite){
        testSprite->destroy(true);
        
        delete testSprite;
        testSprite=0;
        
    }
    
    if (type==-1){
        Sprite::changeSpriteType=0;
        return;
    }
    
    testSprite = new Sprite(GameLogic::lastTouchPosition,(int)type,false);
    if (!testSprite->dict->prop){
        
        testSprite->setAnimationState(10);
        
    }
}

void Sprite::resetLevel(){
    
    printf("Oh noes");
    /*
     for (std::set<Sprite*>::iterator IT =graveYard.begin();IT!=graveYard.end();IT++){
     
     (*IT)->generate((*IT)->summonPosition,(*IT)->type,true);
     levelSprites[(*IT)->theShape]=(*IT);
     
     }
     graveYard.clear();
     
     
     for (std::map<cpShape*, Sprite*>::iterator IT =levelSprites.begin();
     IT!=levelSprites.end();
     IT++){
     
     SpriteDict * theType = getInfoForSprite(IT->second->type);
     
     cpBodySetPos(IT->second->theBody,IT->second->summonPosition);
     cpBodySetVel(IT->second->theBody,cpvzero);
     cpBodySetAngle(IT->second->theBody,0);
     cpBodySetAngVel(IT->second->theBody,0);
     cpBodySetTorque(IT->second->theBody,0);
     
     
     cpShapeSetCollisionType(IT->second->theShape,theType->collisionType);
     
     }
     */
}



void Sprite::jsonDumpData(std::string * theOutput){
    

    std::map<size_t, Sprite*>::iterator IT;
    
    theOutput->append("{\"Sprites\":[");
    
for (IT=Sprite::spriteByID.begin();Sprite::spriteByID.end()!=IT; IT++){
        
        std::string enterData;
        IT->second->position = IT->second->summonPosition;
 
        enterData+="{  \"position\":{\"X\":";
        enterData+=std::to_string(IT->second->summonPosition.x);
        enterData+=",\"Y\":";
        enterData+= std::to_string(IT->second->summonPosition.y);
        enterData+= "},\n";
        enterData+=" \"type\":";
        enterData+=std::to_string(IT->second->type);
        
        
        enterData+=",\n\"startFrame\":";
        enterData+= std::to_string(IT->second->startFrame);
        enterData+="},";
        
        theOutput->append(enterData);
        
        
        
    }
    
    if (spriteByID.size()!=0){
        theOutput->erase(theOutput->begin()+(theOutput->size()-1));
    }
    theOutput->append("]}");
    
    
    
    
}


bool Sprite::loadJson(std::string * theOutput){
    
    for (std::set<Sprite*>::iterator IT = graveYard.begin(); IT != graveYard.end(); IT++){
        
        Sprite * deathMark = *IT;
        
        delete deathMark;
        
    }
    graveYard.clear();
    
    
    for (std::map<size_t, Sprite*>::iterator IT=Sprite::spriteByID.begin();Sprite::spriteByID.end()!=IT; IT++){
        delete IT->second;
    }
    spriteByID.clear();
    
    std::stringstream ss;
    ss << *theOutput;
    
    
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::json_parser::read_json(ss,pt);
        
    }
    catch (std::exception const& e)
    {
        
        std::string error = "There is an issues with the file ";
        
        error = error.append(e.what());
        
        
        DebugTools::spawnErrorWindow(error);
        
        quitTheGame();
        
        
        return false;
    }
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type const& v, pt.get_child("Sprites"))
    {
        
        
        int type;
        int startFrame;
        double xPos;
        double yPos;
        
        BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
            
            std::string keyString = val.first.data();
            
            if (keyString=="position"){
                
                
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    
                    std::string keyString = val2.first.data();
                    
                    if (keyString=="X"){
                        
                        xPos = atof(val2.second.data().c_str());
                        
                    } else if (keyString=="Y"){
                        
                        yPos = atof(val2.second.data().c_str());
                    }
                    
                    
                }
                
                
            } else if (keyString=="type"){
                type=atoi(val.second.data().c_str());
                
                
                
                
            } else if (keyString=="startFrame"){
                startFrame=atoi(val.second.data().c_str());
                
            }
            
        }
        
        Sprite * theSprite = addSprite(cpv(xPos,yPos),type);
        theSprite->startFrame = startFrame;
        
        
        
    }
    
    
    
    return true;
    
}



bool Sprite::UItiles(int startIndex,
                     int numberOfTiles,
                     std::vector<SpriteDict*> * dict,
                     bool propOrNot,
                     bool animate){
    
    
    int currentLevel = GameLogic::getCurrentLevel();
    
    std::map<int, SpriteDict*>::iterator IT;
    
    // Is there a Sprite at the start Index
    IT = theSpriteAssets.find(startIndex);
    
    // If not then grab the next Sprite on the lists
    if (IT == theSpriteAssets.end()){
        IT = theSpriteAssets.upper_bound(startIndex);
    }
    
    // Is the Sprite the correct type keep searching
    
    if (IT==theSpriteAssets.end()){
        
        return false;
    }
    
    while (IT != theSpriteAssets.end()){
        if (IT->second->prop==propOrNot){
            if (IT->second->animates==animate){
                if (IT->second->repLevelRequired<=currentLevel){
                    break;
                }
            }
            
        }
        IT++;
    }
    
    
    
    // If there are no more sprite return False.
    
    if (IT==theSpriteAssets.end()){
        
        return false;
    }
    
    //Add the first sprite to the vector
    dict->push_back(IT->second);
    
    
    for (int counter=1; counter<numberOfTiles; ){
        //Advance the Iterator
        IT++;
        
        //Are we at the end
        if (IT ==theSpriteAssets.end()){
            
            //Fail
            return false;
        }
        
        //Check the next Sprite Type
        if (IT->second->prop==propOrNot){
            if (IT->second->animates==animate){
                if (IT->second->repLevelRequired<=currentLevel){
                    dict->push_back(IT->second);
                    // advance the counter
                    counter++;
                }
            }
            
            
            
            
            
            
        } else {
            // If not then continue
            continue;
            
            
        }
    }
    
    
    return true;
    
}

cpVect Sprite::dumpTextureCord(int index, int frame, GLfloat * text, SpriteDict * target){
    
    
    SpriteDict * dict;
    
    if (!target){
        
        dict = getInfoForSprite(index);
    } else {
        
        dict=target;
    }
    
    if (frame==-1){
        
        frame=dict->frames[2];
        
        
    }
    
    
    if (dict->animates){
        
        GLfloat textX;
        GLfloat textY;
        GLfloat textW;
        GLfloat textH;
        if (dict->textureArray.size()>(4*frame)+3){
            
            textX = dict->textureArray.at((4*frame)+0);
            textY = dict->textureArray.at((4*frame)+1);
            textW = dict->textureArray.at((4*frame)+2);
            textH = dict->textureArray.at((4*frame)+3);
            
        } else {
            textX   =     0.0;
            textY   =     0.0;
            textW   =     1.0;
            textH   =     1.0;
        }
        GLfloat offsetX;
        GLfloat offsetY;
        if (dict->offsetArray.size()>(4*frame)+1){
            offsetX = dict->offsetArray.at((4*frame)+0);
            offsetY = dict->offsetArray.at((4*frame)+1);
            
        } else {
            
            offsetX =0;
            offsetY =0;
            
            
            
        }
        
        
        
        GLfloat theSizeW =dict->segWidth;
        GLfloat theSizeH = dict->segHeight;
        
        
        GLfloat actualCenterPointX = theSizeW/2.0;
        GLfloat actualCenterPointY = theSizeH/2.0;
        
        GLfloat centerPointImageX = offsetX+(textW/2);
        GLfloat centerPointImageY = offsetY+(textH/2);
        
        GLfloat sludgeX = actualCenterPointX - centerPointImageX;
        GLfloat sludgeY = actualCenterPointY - centerPointImageY;
        
        sludgeX*=-2;
        sludgeY*=-2;
        
        double maxX=((textW+sludgeX)/dict->segWidth)*dict->width,
        minX=-(textW-sludgeX)/dict->segWidth*dict->width,
        maxY=(textH-sludgeY)/dict->segHeight*dict->height,
        minY=-(textH+sludgeY)/dict->segHeight*dict->height;
        
        textW=(textX+textW)/(dict->textureWidth);
        textH=(textY+textH)/(dict->textureHeight);
        textX=textX/(dict->textureWidth);
        textY=textY/(dict->textureHeight);
        
        
        cpVect X1Y1 = cpv(minX,minY);
        cpVect X2Y1 = cpv(maxX,minY);
        cpVect X1Y2 = cpv(minX,maxY);
        cpVect X2Y2 = cpv(maxX,maxY);
        
        
        text[0]=textX;
        text[1]=textY;
        text[2]=textW;
        text[3]=textH;
        
        text[4]= (GLfloat)(X1Y2.x);
        text[5]= (GLfloat)(X1Y2.y);
        text[6]= (GLfloat)(X1Y1.x);
        text[7]= (GLfloat)(X1Y1.y);
        text[8]= (GLfloat)(X2Y2.x);
        text[9]= (GLfloat)(X2Y2.y);
        text[10]=(GLfloat)(X2Y1.x);
        text[11]=(GLfloat)(X2Y1.y);
        
        
        return X1Y1;
        
        
    } else {
        
        GLfloat theSizeW =dict->width;
        GLfloat theSizeH = dict->height;
        
        
        text[0]=0;
        text[1]=0;
        text[2]=1.0;
        text[3]=1.0;
        
        
        
        text[4]=  -theSizeW/2.0;
        text[5]=  theSizeH/2.0;
        text[6]=  -theSizeW/2.0;
        text[7]=  -theSizeH/2.0;
        text[8]=  theSizeW/2.0;
        text[9]=  theSizeH/2.0;
        text[10]= theSizeW/2.0;
        text[11]= -theSizeH/2.0;
    }
    return cpvzero;
}

void Sprite::killSprite(){
    for (std::map<size_t, Sprite*>::iterator IT=Sprite::spriteByID.begin();Sprite::spriteByID.end()!=IT; IT++){
        if (!IT->second->dict->prop){
            IT->second->die();
        }
    }
}


cpVect Sprite::dumpTextureCord(int frame, GLfloat * text){
    
    
    return  Sprite::dumpTextureCord(dict->index, frame, text, dict);
    
    
    
    
}

int Sprite::getRandomCivilian(){
    
    std::map<int, SpriteDict*>::iterator IT = theSpriteAssets.begin();
    
    std::vector<SpriteDict*> theCivs;
    int currentLevel = GameLogic::getCurrentLevel();
    
    
    
    
    
    while (IT != theSpriteAssets.end()) {
        if (!IT->second->prop){
            if (IT->second->index>=0){
                if (IT->second->repLevelRequired<=currentLevel){
                    theCivs.push_back(IT->second);
                }
            }
        }
        IT++;
    }
    
    int randomIndex = rand()%theCivs.size();
    
    return theCivs[randomIndex]->index;
    
    
    
    
}


bool Sprite::checkForSpawning(){


    if (!Tiles::hasBeenCheckedForWalkability){
        
        Tiles::checkWalkAbility();
        
    }
    
    
    
    
    if (Tiles::usedTiles.size()==0){
        
        TranslationMatrix::warning(NO_HOUSE);
        
        return false;
    }

    return true;
}


void Sprite::spawnCivilian(){
    
    
    
    
    
    
    
    
    
    
    std::vector<int> spawnAbleSpaces;
    
    

    if (!checkForSpawning()){
        return;
    }
    
    
    spawnAbleSpaces = outSideHouseTiles();
    
    for (std::map<size_t, Sprite*>::iterator IT = trapsByUID.begin();
         IT != trapsByUID.end();
         IT++){
        
        float testValue = rand()/(float)RAND_MAX;
        
        std::map<float,int>::iterator spawnSprite = IT->second->dict->spritesSpawned.upper_bound(testValue);
        
        if (spawnSprite != IT->second->dict->spritesSpawned.end()){

            std::map<int, SpriteDict*>::iterator spawnDict = theSpriteAssets.find(spawnSprite->second);
            
            if (spawnDict != theSpriteAssets.end()){
                
                int space = spawnAbleSpaces.at(rand()%spawnAbleSpaces.size());
                
                Sprite  * theSprite = addSprite(space,spawnSprite->second);
                theSprite->direction=1;
                theSprite->isSelected=false;
                theSprite->tween = 1.0;
                
                
                switch (theSprite->dict->trait) {

                    case NORMAL_CITIZEN:
                        
                    {
                        
                        
                        int thePos = IT->second->renderGridIndex;
                        
                        
                        
                        
                        
                        bool runningSystem = true;
                        
                        int counterCheck = 0;
                        while (runningSystem ) {
                            counterCheck++;
                            
                            if (counterCheck >5){
                             Sprite::removeSprite(theSprite);
                                theSprite=0;
                                runningSystem=false;
                                continue;
                            }
                            
                            switch (rand()%4) {
                                case 0:
                                    if (Tiles::canMove(thePos, 0)){
                                        thePos-=1;
                                        runningSystem = false;
                                    };
                                    break;
                                case 1:
                                    if (Tiles::canMove(thePos, 1)){
                                        
                                        thePos+=Tiles::width;
                                        runningSystem = false;
                                    }
                                    break;
                                case 2:
                                    Tiles::canMove(thePos, 2);
                                    if (Tiles::canMove(thePos, 0)){
                                        thePos+=1;
                                        runningSystem = false;
                                    };
                                    break;
                                case 3:
                                    if (
                                        Tiles::canMove(thePos, 3)){
                                        thePos-=Tiles::width;
                                        runningSystem = false;
                                    }
                                    break;
                                    
                                default:
                                    break;
                            }
                            
                            
                            
                        }
                        
                        
                        
                        
                        
                        if (theSprite){
                            if (!theSprite->findPath(thePos)){
                            
                                Sprite::removeSprite(theSprite);
                            
                            }
                        }
                        
                        
                        
                        
                    }
                        
                        break;
                    default:
                        break;
                }
                
                
                
                
                
            }
        }
    }
    
}

void Sprite::spawnWander(int badOrGood){
    
    if (!checkForSpawning()){
        
        return;
    }
    
    
     std::vector<int> goodTiles = outSideHouseTiles();
    
    reputationStruct * theStruct = &GameLogic::reputationLevels[GameLogic::getCurrentLevel()];
    
    
    
    bool work = true;
    while (work) {
        
        
        std::vector<int>::iterator IT = goodTiles.begin();
        std::advance(IT,rand()%goodTiles.size());
        
        std::map<float,int> * unitSpawner = &theStruct->badspawn;
        if (badOrGood==1){
        
            unitSpawner = &theStruct->badspawn;
        } else if (badOrGood ==0) {
            unitSpawner = &theStruct->woodspawn;
        } else if (badOrGood ==2) {
            unitSpawner = &theStruct->authorities;
        }
        
        
        float testValue = rand()/(float)RAND_MAX;
        
        std::map<float,int>::iterator spawnSprite = unitSpawner->upper_bound(testValue);
        
        if (Camera::isPointVisible(*IT)){
            
        }else{
            
            work=false;
            
            
            ///
            
            Sprite * theSprite = Sprite::addSprite(*IT, spawnSprite->second);
            theSprite->isSelected = false;
            theSprite->direction=1;
            theSprite->tween = 1.0;
            
            switch (theSprite->dict->trait) {
                case VIVIAN:
                    Achievement::meetVivan();
                    break;
                case GILDA:
                    Achievement::meetGilda();
                    break;
                    
                default:
                    break;
            }
            
            
            switch (theSprite->dict->trait) {
               
                
                case SCAREDYCAT:
                case GOTH:
                {
                    theSprite->isSelected = false;
                    theSprite->tween=1.0;
                    work = false;
                    BobZombie::checkEvent(theSprite->dict);
                }
                break;
                
                case COUPLE:{

                    std::map<int,SpriteDict*>::iterator person = theSpriteAssets.find(spawnSprite->second);
                    std::map<int,SpriteDict*>::iterator partner;
                    
                    if (person!=theSpriteAssets.begin()){
                        
                        partner= person;
                        partner--;
                        
                        if (partner->second->trait != COUPLE){
                            
                            partner++;
                            partner++;
                        }
                        
                    } else {
                        
                        partner++;
                        
                    }
                    
                    
                    if (partner->second->trait == COUPLE){
                        
                        int otherTile = -1;
                        int direction = rand()%4;
                        
                        for (int counter = direction; counter<direction+4; counter++){
                            
                            switch (counter%4) {
                                case 0:
                                if (Tiles::canMove(*IT, 0)){
                                    otherTile = *IT-1;
                                    counter+=1000;
                                }
                                
                                break;
                                case 1:
                                if (Tiles::canMove(*IT, 1)){
                                    otherTile = *IT + Tiles::width;
                                    counter+=1000;
                                }
                                break;
                                case 2:
                                
                                if (Tiles::canMove(*IT, 2)){
                                    otherTile = *IT+ 1;
                                    counter+=1000;
                                }
                                break;
                                case 3:
                                
                                if (Tiles::canMove(*IT, 3)){
                                    otherTile = *IT -Tiles::width;
                                    counter+=1000;
                                }
                                break;
                                
                                default:
                                break;
                            }
                            
                            
                            
                        }
                        
                        if (otherTile ==-1){
                            Sprite::removeSprite(theSprite);
                            continue;
                            
                        } else {
                            
                            
                            
                            Sprite * theSpriteO = Sprite::addSprite(otherTile, partner->first);
                            theSpriteO->isSelected = false;
                            BobZombie::checkEvent(theSprite->dict);
                            work = false;
                        }
                        
                        
                        
                        
                    } else {
                        Sprite::removeSprite(theSprite);
                        continue;
                        
                        
                    }
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                }
                break;
                
                
                case SKEPTIC:
                case ZOMBIE:
                case GAMER:
                case REPORTER:
                case EXORCIST:
                case POLICE:
                case DOCTOR:
                case VIVIAN:
                case HERO:
            
                case NINJA:
                case INVESTIGATOR:
    
                case GILDA:
                
                {
                    
                    BobZombie::checkEvent(theSprite->dict);
                    
                    
                    int counter =0;
                    while (!theSprite->findPath(Tiles::getRandomTileHouse())){
                        
                        counter++;
                        if (counter>3){
                            
                            Sprite::removeSprite(theSprite);
                            return;
                            
                        }
                    }
                }
                    break;
                case NORMAL_CITIZEN:
                case RENOVATOR:
                case GHOSTHUNTER:
                case IDIOT:
                {
                    BobZombie::checkEvent(theSprite->dict);
                    
                    for (int counter = 0; counter<3; counter++)
                    {
                        
                        if (trapsByUID.size()==0){
                            
                            if (unitSpawner->size()>1){
                                spawnWander(1);
                                Sprite::removeSprite(theSprite);
                                return;
                            } else {
                                Sprite::removeSprite(theSprite);
                                return;
                            }
                            
                        }
                        
                        std::map<size_t, Sprite*>::iterator IT = trapsByUID.begin();
                        
                        std::advance(IT, rand()%trapsByUID.size());
                        
                        int thePos = IT->second->renderGridIndex;
                        
                        
                        
                        
                        
                        bool runningSystem = true;
                        
                        while (runningSystem ) {
                            switch (rand()%4) {
                                case 0:
                                    if (Tiles::canMove(thePos, 0)){
                                        thePos-=1;
                                        runningSystem = false;
                                    };
                                    break;
                                case 1:
                                    if (Tiles::canMove(thePos, 1)){
                                        
                                        thePos+=Tiles::width;
                                        runningSystem = false;
                                    }
                                    break;
                                case 2:
                                    Tiles::canMove(thePos, 2);
                                    if (Tiles::canMove(thePos, 0)){
                                        thePos+=1;
                                        runningSystem = false;
                                    };
                                    break;
                                case 3:
                                    if (
                                        Tiles::canMove(thePos, 3)){
                                        thePos-=Tiles::width;
                                        runningSystem = false;
                                    }
                                    break;
                                    
                                default:
                                    break;
                            }
                            
                            
                            
                        }
                        
                        
                        
                        
                        
                        
                        if (!theSprite->findPath(thePos)){
                            
                            Sprite::removeSprite(theSprite);
                            work = true;
                            
                        } else {
                            
                            
                            
                        }
                        
                        
                        
                        
                    }
                    
                    
                    
                    
                    
                    
                }
                    break;
                case BULLDOZER:
                case MAX_TRAITS:
                printf("A Sprite was Spawned that should not have been.");
                
                break;
        

                    
                    
            }
        }
    }
}



std::vector<int> Sprite::outSideHouse;

bool Sprite::outSideHouseCheck = false;

std::vector<int> Sprite::outSideHouseTiles(){
    
    
    if (!outSideHouseCheck||!Tiles::hasBeenCheckedForWalkability){
        if (!Tiles::hasBeenCheckedForWalkability){
            Tiles::checkWalkAbility();
        }
        
        
        
        int leftS = Tiles::houseDimension[0]-8;
        int leftE = Tiles::houseDimension[0]-12;
        
        int topS = Tiles::houseDimension[1]+8;
        int topE = Tiles::houseDimension[1]+12;
        
        int rightS = Tiles::houseDimension[2]+8;
        int rightE = Tiles::houseDimension[2]+12;
        
        int botS = Tiles::houseDimension[3]-8;
        int botE = Tiles::houseDimension[3]-12;
        
        std::vector<int> directions;
        
        
        // Create LeftSide
        if (leftS>0){
            for (int counterX=leftE;counterX<=leftS;counterX++){
                for (int counterY = botE; counterY<=topE;counterY++){
                    directions.push_back(counterX+counterY*Tiles::width);
                }
            }
        } else {
            
            for (int counterY = botE; counterY<=topE;counterY++){
                directions.push_back(0+counterY*Tiles::width);
            }
            
            
        }
        
        // Create TopSide
        if (topS<Tiles::width-2){
            for (int counterX=leftE;counterX<=rightE;counterX++){
                for (int counterY = topS; counterY<=topE;counterY++){
                    directions.push_back(counterX+counterY*Tiles::width);
                }
            }
            
            
        } else {
            
            for (int counterX=leftE;counterX<=rightE;counterX++){
                directions.push_back(counterX+(Tiles::width-1)*Tiles::width);
            }
            
        }
        
        // Create RightSide
        if (rightS<Tiles::width-2){
            for (int counterX=rightS;counterX<=rightE;counterX++){
                for (int counterY = botE; counterY<=topE;counterY++){
                    directions.push_back(counterX+counterY*Tiles::width);
                }
            }
            
        } else {
            
            int rightValue = Tiles::width-1;
            for (int counterY = botE; counterY<=topE;counterY++){
                directions.push_back(rightValue+counterY*Tiles::width);
            }
        }
        
        
        // Create BotSide
        if (botS>0){
            for (int counterX=leftE;counterX<=rightE;counterX++){
                for (int counterY = botS; counterY<=botE;counterY++){
                    directions.push_back(counterX+counterY*Tiles::width);
                }
            }
        } else {
            for (int counterX=leftE;counterX<=rightE;counterX++){
                directions.push_back(counterX);
            }
        }
        
        
        std::vector<int> goodTiles;
        
        for (std::vector<int>::iterator IT = directions.begin();
             IT != directions.end();
             IT++){
            
            if (*IT<0){continue;}
            if (*IT>=Tiles::width*Tiles::width){continue;};
            if (Tiles::mapValues[*IT].block){continue;};
            if (Tiles::mapValues[*IT].occupiedSprite){continue;};
            
            
            goodTiles.push_back(*IT);
            
        }
        
        
        
        
        
        outSideHouse = goodTiles;
        outSideHouseCheck = true;
        
        
        
    }
    
    std::vector<int> goodTiles;
    
    for (std::vector<int>::iterator IT = outSideHouse.begin();
         IT != outSideHouse.end();
         IT++){
        
        if (Camera::isPointVisible(*IT)){
            continue;
        }
        
        goodTiles.push_back(*IT);
    }
    
    
    
    
    return goodTiles;
}




void Sprite::test(){

    std::vector<int> goodTiles = outSideHouseTiles();
    

    
    Player::mainPlayer->path.clear();
    
    for (std::vector<int>::iterator IT = goodTiles.begin();
         IT != goodTiles.end();
         IT++){
        
        int value = *IT;
        
        Player::mainPlayer->path.push_back(PathTile(value));
        
    }

}

/*
void Sprite::spawnWander(){
    
    if (!checkForSpawning()){
        
        return;
    }
    
    
    
   
    
    std::vector<int> goodTiles = outSideHouseTiles();
    
    
    reputationStruct * theStruct = &GameLogic::reputationLevels[GameLogic::getCurrentLevel()];
    
    bool work = true;
    while (work) {
        
        
        
        
        std::vector<int>::iterator IT = goodTiles.begin();
        std::advance(IT,rand()%goodTiles.size());
        
        std::map<float,int> * woodspawner = &theStruct->woodspawn;
        
        
        float testValue = rand()/(float)RAND_MAX;
        
        std::map<float,int>::iterator spawnSprite = woodspawner->upper_bound(testValue);
        
        if (Camera::isPointVisible(*IT)){
            ;
        }else{
            
            
            if (theSpriteAssets[spawnSprite->second]->trait==COUPLE){
            
                std::map<int,SpriteDict*>::iterator person = theSpriteAssets.find(spawnSprite->second);
            
                std::map<int,SpriteDict*>::iterator partner;
                
                if (person!=theSpriteAssets.begin()){
                
                    partner= person;
                    partner--;
                    
                    if (partner->second->trait != COUPLE){
                    
                        partner++;
                        partner++;
                    }
                    
                } else {
                
                     partner++;
                
                }
                
                
                if (partner->second->trait == COUPLE){
                    
                    int otherTile = -1;
                    int direction = rand()%4;
                    
                    for (int counter = direction; counter<direction+4; counter++){
                    
                        switch (counter%4) {
                            case 0:
                                if (Tiles::canMove(*IT, 0)){
                                    otherTile = *IT-1;
                                    counter+=1000;
                                }
                                
                                break;
                            case 1:
                                if (Tiles::canMove(*IT, 1)){
                                    otherTile = *IT + Tiles::width;
                                    counter+=1000;
                                }
                                    break;
                            case 2:
                                
                                if (Tiles::canMove(*IT, 2)){
                                    otherTile = *IT+ 1;
                                    counter+=1000;
                                }
                                break;
                            case 3:
                                
                                if (Tiles::canMove(*IT, 3)){
                                    otherTile = *IT -Tiles::width;
                                    counter+=1000;
                                }
                                    break;
                                
                            default:
                                break;
                        }
                    
                    
                    
                    }
                    
                    if (otherTile ==-1){
                        continue;
                    
                    } else {
                    
                        Sprite * theSprite = Sprite::addSprite(*IT, spawnSprite->second);
                        theSprite->isSelected = false;
                        
                        Sprite * theSpriteO = Sprite::addSprite(otherTile, partner->first);
                        theSpriteO->isSelected = false;
                        
                        work = false;
                        
                        
                    
                    
                    }
                    
                    
                
                
                } else {
                    continue;
                
                
                }
                
                
                
                
                
            
                
                
            
            } else {
            
            
            
            Sprite * theSprite = Sprite::addSprite(*IT, spawnSprite->second);
            theSprite->isSelected = false;
            work = false;
        
            }
            
        
        
        
        }
        
        
        
        
        
        
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
*/


Sprite::~Sprite(){
    
    
    
    
}


void Sprite::generateColor(float * color){
    
    
    float aHealth = health/dict->health;
    
    
    //Green     =   0.0     1.0,    0.0
    //Yellow    =   1.0     1.0     0.0
    //Red       =   1.0,    0.0,    0.0
    
    
    
    
    color[0] = (aHealth<=0.5)?1.0:1.0-((aHealth-0.5)/0.5);
    color[1] = (aHealth>=0.5)?1.0:aHealth/0.5;
    
    
    
    
    
    
    
    
    
}



void Sprite::generateTextForLevelStatus(){

    int level = GameLogic::getCurrentLevel();
    
    
    if (level != GameLogic::reputationLevels.rbegin()->first){
        
        reputationStruct info = GameLogic::reputationLevels[level+1];
        
        std::pair<int,int> value = getEnergyUsedOnProps();
        std::string theText= "To reach the next level you need ";
        int value1 = info.energy-value.first;
        int value2 = info.numberOfStruct-value.second;
        theText = theText.append(std::to_string(value1));
        theText = theText.append(" energy in props and ");
        theText = theText.append(std::to_string(value2));
        theText = theText.append(" scare traps to get to the next level.");
        
        
        UserInterface::spawnTextBox(theText,60);
        
    }
    
    
    
}




std::pair<int,int> Sprite::getEnergyUsedOnProps(){
    
    int totalEnergy = 0;
    int scareTraps = 0;
    
    for (std::map<size_t, Sprite*>::iterator IT=Sprite::trapsByUID.begin();
         Sprite::trapsByUID.end()!=IT;
         IT++){

        
        if (IT->second->dict->prop){
            if (IT->second->dict->animates){
                scareTraps++;
                
            }else{
             //   totalEnergy+=IT->second->dict->cost;
            }
            
            
        }
        
        
        
        
    }
    
    
    for (std::vector<int>::iterator IT = Tiles::usedTiles.begin();
         IT != Tiles::usedTiles.end();
         IT++){
        
        totalEnergy += Tiles::tileDictVector[Tiles::mapValues[*IT].type]->energyRequired;
        
        
    }
    
    
    
    
    return std::pair<int,int>(totalEnergy,scareTraps);
    
}


std::pair<int,int> Sprite::dimensionsForBlockType(blockTypes _type){
    
    switch (_type) {
            
        case    NoPlacement:
            return std::pair<int,int>(9999,9999);
        case   OneXOneLeftWall:
        case   OneXOne:
            return std::pair<int,int>(1,1);
        case   TwoXTwo:
            return std::pair<int,int>(2,2);
        case  ThreeXThree:
            return std::pair<int,int>(3,3);
        case ThreeXTwoL:
            return std::pair<int,int>(2,3);
        case TwoXOneULeftWall:
        case TwoXOneU:
            return std::pair<int,int>(2,1);
        case TwoXOneL:
            return std::pair<int,int>(1,2);
        case ThreeXOneL:
        case ThreeXOneLRightWall:
            return std::pair<int,int>(1,3);
        case ThreeXTwoU:
            return std::pair<int,int>(2,3);
        case SixXThreeL:
            return std::pair<int,int>(3,6);
        case TwoXEight:
            return std::pair<int,int>(2,8);
        default:
            return std::pair<int,int>(999999,999999);
            break;
    }
    
    
}






void Sprite::spawnTrees(){
    
    
    
    int maxTrees = Tiles::width*Tiles::width/25;
    
    for (int counter =0; counter<maxTrees; counter++){
        
        std::set<int>::iterator IT = trees.begin();
        
        size_t sizeOfTree = rand()%trees.size();
        std::advance(IT, sizeOfTree);
        int theTreeType = (*IT);
        
        int target = Tiles::findTile(std::pair<int,int>((rand()%(Tiles::width-2))+1,(rand()%(Tiles::width-2))+1));
        
        
        Sprite::addSpriteLarge(target,theTreeType);
        
        
        
        
        
        
        
        
    }
    
    
    
    
    
    
}

bool Sprite::checkIfLocationIsInvalid(){
    
    std::pair<int,int> dimensions = dimensionsForBlockType(dict->placementType);
    
    int xPos = renderGridIndex%Tiles::width;
    int yPos = renderGridIndex/Tiles::width;
    
    
    
    for (int counterX = xPos; counterX<xPos+dimensions.first+1; counterX++){
        for (int counterY = yPos; counterY<yPos+dimensions.second+1; counterY++){
            
            TileObject * theTile = &Tiles::mapValues[xPos+yPos*Tiles::width];
            if (theTile->rw||theTile->lw||theTile->theDoorL||theTile->theDoorR){
                
                return true;
                
                
            }
            
            
            
            
        }
    }
    
    
    
    
    
    
    
    
    return false;
    
}



void Sprite::gameOver(){

    if (!GameLogic::gameIsOver){
    UserInterface::timer = 0;
    GameLogic::gameIsOver = true;
    
    AudioController::gameOver();
    }
    
    

}