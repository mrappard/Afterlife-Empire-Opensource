//
//  ModManager.cpp
//  After Life Empire
//
//  Created by Autobotika on 2015-02-20.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#include "ModManager.h"
#include "TextureGenerator.h"
#include "FileManager.h"

#undef check
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "Sprite.h"
#include "Tiles.h"
#include "TranslationMatrix.h"
#include "Door.h"
#include "TextureGenerator.h"

std::map<std::string,ModElement> ModManager::mods;

std::vector<DoorDict*> ModManager::modDoor;
std::vector<SpriteDict*> ModManager::modSprite;
std::vector<TileDict*> ModManager::modTile;


static std::map<int, SpriteDict*> theSpriteAssetsBackup;
static std::map<int,TileDict*> tileDictionaryBackUp;
static std::map<int,DoorDict*> doorDictionaryBackUp;


std::vector<std::string> ModManager::targetMods;

void ModManager::loadAllMods(){
    ModManager::returnNormal();
    
    
    for (std::vector<std::string>::iterator IT =  ModManager::targetMods.begin();
         IT != ModManager::targetMods.end();
         IT++){
        
        std::string path = *IT;
    
        path= path.append("/ModData.json");
        Sprite::loadDictTextureLoader(&path);
    
    }
    
    Sprite::allTexturesInit = false;
    
    
    

}
void ModManager::removeAllMods(){
    targetMods.clear();
    

    for (std::map<std::string,ModElement>::iterator IT = mods.begin();
         IT != mods.end();
         IT++){
        IT->second.enabled = false;
        
    }
    
    
}

void ModManager::populateBack(){
    
    if (theSpriteAssetsBackup.size()>0){
        return;
    
    }
    
    theSpriteAssetsBackup = Sprite::theSpriteAssets;
    doorDictionaryBackUp = Door::theDoorAssets;
    tileDictionaryBackUp = Tiles::tileDictionary;
}

void ModManager::returnNormal(){

    std::set<GLuint*> killThese;
    
    for (std::vector<DoorDict*>::iterator IT = ModManager::modDoor.begin();
         IT != ModManager::modDoor.end();
         IT++){
    
        killThese.insert((*IT)->texture);
        
    
    }
    
    
    for (std::vector<SpriteDict*>::iterator IT = ModManager::modSprite.begin();
         IT != ModManager::modSprite.end();
         IT++){
        
        killThese.insert((*IT)->texture);
    
    
    }
    
    
    for (std::vector<TileDict*>::iterator IT = ModManager::modTile.begin();
         IT != ModManager::modTile.end();
         IT++){
        
        killThese.insert((*IT)->texture);
        killThese.insert((*IT)->textureC);
        killThese.insert((*IT)->textureL);
        killThese.insert((*IT)->textureR);
        
        
    }
    
    if (killThese.size()>0){
        
        
    for (std::map<int, SpriteDict*>::iterator IT = theSpriteAssetsBackup.begin();
         IT != theSpriteAssetsBackup.end();
         IT++){
        
        if (!IT->second->mod){
            if (killThese.find(IT->second->texture)!=killThese.end()){
                killThese.erase(IT->second->texture);
            
            }
          
        }
    }

    for (std::map<int, TileDict*>::iterator IT = tileDictionaryBackUp.begin();
         IT != tileDictionaryBackUp.end();
         IT++){
        
        if (!IT->second->mod){
            
            if (IT->second->texture){
                if (killThese.find(IT->second->texture)!=killThese.end()){
                    killThese.erase(IT->second->texture);
                    
                }
            }
            
            if (IT->second->textureC){
                if (killThese.find(IT->second->textureC)!=killThese.end()){
                    killThese.erase(IT->second->textureC);
                    
                }
            }
            
            if (IT->second->textureL){
                if (killThese.find(IT->second->textureL)!=killThese.end()){
                    killThese.erase(IT->second->textureL);
                    
                }
            }
            
            if (IT->second->textureR){
                if (killThese.find(IT->second->textureR)!=killThese.end()){
                    killThese.erase(IT->second->textureR);
                    
                }
            }
            
        }
    }
    
    for (std::map<int,DoorDict*>::iterator IT =doorDictionaryBackUp.begin();
         IT != doorDictionaryBackUp.end();
         IT++){
        
            killThese.erase(IT->second->texture);

    }
        
        for (std::set<GLuint*>::iterator IT = killThese.begin();
             IT != killThese.end();
             IT++){
            
            GLuint * texture = *IT;
            removeTexture(texture);
            
        }
    
    }

 
    
    
    

    
    Sprite::theSpriteAssets=theSpriteAssetsBackup;
    Door::theDoorAssets=doorDictionaryBackUp;
    Tiles::tileDictionary=tileDictionaryBackUp;
    
    Tiles::tileDictVector.clear();
    for (std::map<int, TileDict*>::iterator IT = tileDictionaryBackUp.begin();
         IT != tileDictionaryBackUp.end();
         IT++){
        
        while (Tiles::tileDictVector.size()<IT->first+2){
            Tiles::tileDictVector.push_back(0);
        }
        
        Tiles::tileDictVector[IT->first]=IT->second;
        
        
        
    }
    

    
    for (std::vector<DoorDict*>::iterator IT = ModManager::modDoor.begin();
         IT != ModManager::modDoor.end();
         IT++){
        
        
        
        delete *IT;
        
        
        
        
    }
    
    
    for (std::vector<SpriteDict*>::iterator IT = ModManager::modSprite.begin();
         IT != ModManager::modSprite.end();
         IT++){
        
        delete *IT;
        
        
    }
    
    
    for (std::vector<TileDict*>::iterator IT = ModManager::modTile.begin();
         IT != ModManager::modTile.end();
         IT++){
        
     delete *IT;
        
        
    }
    
    ModManager::modDoor.clear();
    ModManager::modSprite.clear();
    ModManager::modTile.clear();
    
    
    Sprite::loadDictTextureLoaderReturn(0);
    
    


}






ModElement::ModElement(){

    thumbNail=0;
}


ModElement::~ModElement(){

}

void ModElement::click(){
    
    enabled=!enabled;
    
    if (enabled){
    
        ModManager::targetMods.push_back(thePath);
        
    } else {
    
        for (size_t counter = 0;
             counter< ModManager::targetMods.size();
             counter++)
             {
                 
                 if (ModManager::targetMods[counter]==thePath){
                 
                     ModManager::targetMods.erase(ModManager::targetMods.begin()+counter);
                     return;
                 
                 }
        
        }
        
    
    }


}


void ModElement::parseJson(){
    working=false;
    name = "JSON ERROR";
    description = "JSON ERROR";
    
    std::string jsonLocation = thePath+"/ModData.json";
    
    std::ifstream f;
    f.open(jsonLocation.c_str());
    
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::json_parser::read_json(f,pt);
        
    }
    catch (std::exception const& e)
    {
        std::cerr <<"Can't find the Json for mod "<< thePath << e.what() << std::endl;
        
        
        std::string appendThis = e.what();
        description = "JSON ERROR - " + appendThis;
        
        return;
    }
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type const& v, pt){
        
        std::string itemName = v.first.c_str();
        
        if (itemName=="Info"){
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& val, v.second){
           
                
                std::string itemName = val.first.c_str();
                std::string itemData = val.second.data();
            
                
                if (itemName=="Name"){
                    name = itemData;
                }
                if (itemName=="Description"){
                    description = itemData;
                }
            
            
                
            }
            working = true;
        }
        
        
        
        
        
    }
    
    return ;
}

std::string ModElement::getName(){
    
    if (name == "NO DATA"){
        parseJson();
    }
    
    return name;

}

std::string ModElement::getDescription(){

    if (description == "NO DATA"){
        parseJson();
    }
    
    return description;
    
}

GLuint ModElement::loadThumbNail(){
    if(!thumbNail){
        int sizeValues[10];
        
        thumbNail = dontStore(thePath+"/Thumbnail.png", sizeValues);
        return thumbNail;
    
    } else {
        return thumbNail;
    
    }
}

void ModElement::destroy(){

    if (thumbNail){
    glDeleteTextures(1, &thumbNail);
    }

}

void ModManager::addMod(std::string theString){

    ModElement item;
    item.modLoaded = false;
    item.enabled = false;
    item.thePath = theString;
    item.name = "NO DATA";
    item.description = "NO DATA";
    mods[theString]=item;
    
    
}

void ModManager::populate(){

    if (mods.size()==0){
    findAllMods();
    }
    
    
}

std::vector<ModElement*> ModManager::dumpElements(size_t index){
    
    std::vector<ModElement*> elements;
    
    std::map<std::string,ModElement>::iterator IT = mods.begin();
    
    if (mods.size()>index*5){
        std::advance(IT, index*5);
        
        for (int counter = 0; counter<5; counter++){
        
            if (IT==mods.end()){
                break;
            }
            elements.push_back(&IT->second);
            IT++;
        }
    
    
    }
    
    return elements;
}

