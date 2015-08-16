//
//  Door.cpp
//  After Life Empire
//
//  Created by Autobotika on 2015-03-04.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#include <stdio.h>
#include "Door.h"
#include "FileManager.h"
#include "TextureGenerator.h"
#include "Tiles.h"
#include "TextureGenerator.h"
#include "GlobalDebugger.h"
#include "DebugTools.h"

#undef check
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "GameLogic.h"
#include "SaveObject.h"

#include "Userinterface.h"


#define CLOSED_L 0
#define OPEN_L   1
#define CLOSED_R 2
#define OPEN_R   3






std::map<int, Door*> Door::indexMap;

std::set<Door*> Door::thinkingDoors;

std::map<int, DoorDict*> Door::theDoorAssets;

DoorDict * Door::createThis;

static bool debugLeftOrRight;


void Door::dumpAllStruts(){

    
    std::string data;
    
    for (std::map<int, DoorDict*>::iterator IT = theDoorAssets.begin();
         IT != theDoorAssets.end();
         IT++){
    
        
        data = data.append("{\n");
       
        
        data = data.append("\"Name\":\"" );
        data = data.append(IT->second->name);
        data = data.append("\",\n");
       
        
        data = data.append("\"TextureName\":\"");
        data = data.append(IT->second->textureName);
        data = data.append("\",\n");
        
        data = data.append("\"JsonName\":\"");
        data = data.append(IT->second->jsonName);
        data = data.append("\",\n");
        
        data = data.append("\"Index\":");
        data = data.append(std::to_string(IT->second->index));
        data = data.append(",\n");
        
        
        data = data.append("\"Width\":");
        data = data.append(std::to_string(IT->second->width));
        data = data.append(",\n");
        
        
        
        
        data = data.append("\"Height\":");
        data = data.append(std::to_string(IT->second->height));
        data = data.append(",\n");
        
        data = data.append("\"Animates\":");
        data = data.append("true,\n");
        
        data = data.append("\"Frames\":");
        //data = data.append(std::to_string(IT->second->sludgeY_L));
        
        
        data = data.append("\"SludgeX_R\":");
        data = data.append(std::to_string(IT->second->sludgeX_R));
        data = data.append(",\n");
        
        data = data.append("\"SludgeY_R\":");
        data = data.append(std::to_string(IT->second->sludgeY_R));
        data = data.append(",\n");
        
        data = data.append("\"SludgeX_L\":");
        data = data.append(std::to_string(IT->second->sludgeX_L));
        data = data.append(",\n");
        
        data = data.append("\"SludgeY_L\":");
        data = data.append(std::to_string(IT->second->sludgeY_L));
        data = data.append("\n");

                           
                           
                           
    
        
        
        
        
        data = data.append("},\n");
        
        
        

        
        
  
    
    
    
    }
    
    printf("%s",data.c_str());
    


}


void Door::keyDebug(int keyCode){
    

   
    
    if (!createThis) {
        return;
    }
    
    if (GameLogic::gameMode==DOORMODE){
        if (debugLeftOrRight){
            
            if (keyCode== 12 ) { // For Q Key
                debugLeftOrRight=!debugLeftOrRight;
            
            
            } else if (keyCode == 126){ //For UP key
                
                createThis->sludgeY_L+=0.125;
            } else if (keyCode == 125){ //For DOWN key
                createThis->sludgeY_L-=0.125;
            } else if (keyCode==124){ //RIGHT
                createThis->sludgeX_L+=0.125;
            } else if (keyCode==123){ //LEFT
                createThis->sludgeX_L-=0.125;
            } else if (keyCode == 41){ //For ; key
                createThis->width-=0.1;
            } else if (keyCode == 39){ //For ' key
                createThis->width+=0.1;
                
            } else if (keyCode == 43){ //For ; key
                createThis->height-=0.1;
            } else if (keyCode == 47){ //For ' key
                createThis->height+=0.1;
                
            }
            
                printf("Name - %s, X = %f, Y = %f, W = %f, H =%f",createThis->name.c_str(), createThis->sludgeX_L, createThis->sludgeY_L, createThis->width, createThis->height);
            
            
        }else{
            
            
            
            
            if (keyCode== 12 ) { // For Q Key
                debugLeftOrRight=!debugLeftOrRight;

            } else if (keyCode == 126){ //For UP key
                
                createThis->sludgeY_R+=0.125;
            } else if (keyCode == 125){ //For DOWN key
                createThis->sludgeY_R-=0.125;
            } else if (keyCode==124){ //RIGHT
                createThis->sludgeX_R+=0.125;
            } else if (keyCode==123){ //LEFT
                createThis->sludgeX_R-=0.125;
            } else if (keyCode == 41){ //For ; key
                createThis->width-=0.1;
            } else if (keyCode == 39){ //For ' key
                createThis->width+=0.1;
                
            } else if (keyCode == 43){ //For ; key
                createThis->height-=0.1;
            } else if (keyCode == 47){ //For ' key
                createThis->height+=0.1;
                
            }
            
                printf("Name - %s, X = %f, Y = %f, W = %f, H =%f",createThis->name.c_str(), createThis->sludgeX_R, createThis->sludgeY_R, createThis->width, createThis->height);
            
        }
    
         char holdString[600];
        
        
        sprintf(holdString, "Name - %s, XL = %f, YL = %f, XR = %f, YR = %f, W = %f, H =%f",createThis->name.c_str(), createThis->sludgeX_L, createThis->sludgeY_L, createThis->sludgeX_R, createThis->sludgeY_R, createThis->width, createThis->height);
        
        /*
         printf("Name - %s, X = %f, Y = %f, W = %f, H =%f",debugDict->name.c_str(), debugDict->sludgeX, debugDict->sludgeY, debugDict->width, debugDict->height);
         */
        
        UserInterface::spawnTextBox(holdString, TEXTBOX_STAY_UP_FOREVER);
        
        
    }
    
    
    
}




bool Door::parseJsonFrame(DoorDict * dict){
    
    SaveObject::changeJsonToRaw(dict, 1);
    if (dict->textureArray.size()){
        return 1;
    }
    
    
    
    dict->textureArray.clear();
    dict->offsetArray.clear();
    
    std::string jsonLocation = findFile(dict->jsonName);
    
    std::ifstream f;
    f.open(jsonLocation.c_str());
    
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::json_parser::read_json(f,pt);
        
    }
    catch (std::exception const& e)
    {
        
        std::string error = "There is an issues with the Doors json file ";
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
            
            if (keyString=="frame"){
                
                
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
                
            } else if (keyString=="spriteSourceSize"){
                
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
                
            } else if (keyString=="sourceSize"){
                
                BOOST_FOREACH(boost::property_tree::ptree::value_type const& val2, val.second){
                    std::string keyString = val2.first.data();
                    
                    if (keyString=="w"){
                        source_wh[0]=(GLfloat)atof(val2.second.data().c_str());
                    } else if (keyString=="h"){
                        source_wh[1]=(GLfloat)atof(val2.second.data().c_str());
                    }
                }
            } else if (keyString=="rotated"){
                
                std::string itemData = val.second.data();
                dict->rotated=(strncmp(itemData.c_str(),"true",4))?true:false;
                
            } else if (keyString=="trimmed"){
                
                
                std::string itemData = val.second.data();
                dict->trimmed=(strncmp(itemData.c_str(),"true",4))?true:false;
                
                
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
    SaveObject::changeJsonToRaw(dict, 1);
    return true;
}


void Door::changeSelect(int index){
    
    std::map<int, DoorDict*>::iterator IT = theDoorAssets.find(index);
    
    if (theDoorAssets.end() !=IT){
    
        createThis = IT->second;
    
    } else {
    
        createThis = 0;
    
    }

    

}

bool Door::UItiles(int startIndex,int numberOfTiles,std::vector<DoorDict*> * dict){

    std::map<int, DoorDict*>::iterator IT;
    
    // Is there a Sprite at the start Index
    IT = theDoorAssets.find(startIndex);
    
    // If not then grab the next Sprite on the lists
    if (IT == theDoorAssets.end()){
        IT = theDoorAssets.upper_bound(startIndex);
    }
    
    // Is the Sprite the correct type keep searching
    
    if (IT==theDoorAssets.end()){
        
        return false;
    }
    
    
    //Add the first sprite to the vector
    dict->push_back(IT->second);
    
    for (int counter=1; counter<numberOfTiles; ){
        //Advance the Iterator
        IT++;
        
        //Are we at the end
        if (IT == theDoorAssets.end()){
            
            //Fail
            return false;
        }
        
        dict->push_back(IT->second);
    }
    
    
    return true;
}

void Door::setFirstFrame(bool leftOrRight){

    if (leftOrRight){
        
        frame = dict->frames[CLOSED_L];
        
        
        
    } else {
        
        frame = dict->frames[CLOSED_R];
        
    }

}

void Door::removeAllDoors(){


    
    
    for (std::set<Door*>::iterator IT = thinkingDoors.begin();
         IT != thinkingDoors.end();
         IT++){
    
        Door * theDoor = (*IT);
        
        if (theDoor != 0){
            
            delete theDoor;
           
        }
    }

    thinkingDoors.clear();
    indexMap.clear();
    
}

bool Door::loadDict(std::string * path){
    
    
    
    char buffer [1000];
    
    if (path==0){
        
        std::string theFile = findFile("Tiles.json");
        sprintf (buffer, "%s", theFile.c_str());
        
    }else{
        sprintf (buffer, "%s", path->c_str());
    }
    
    
    std::ifstream f;
    f.open(buffer);
    
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::json_parser::read_json(f,pt);
        
    }
    catch (std::exception const& e)
    {
        std::cerr <<"Oh now I can't find the Json file with the Sprite File info" << e.what() << std::endl;
        return false;
    }
    
    
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type const& v, pt.get_child("Doors"))
    {
        DoorDict newOne;
        newOne.init = false;
        newOne.texture = 0;
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
        
        
        if (theDoorAssets.find(newOne.index)!=theDoorAssets.end()){
            
            *theDoorAssets[newOne.index]=newOne;
            newOne.init = false;
            
        } else {
            DoorDict * newDict = new DoorDict;
            *newDict = newOne;
            newOne.init = false;
            
            
            theDoorAssets[newOne.index]=newDict;
            
            
            
        }
        
    }
    
    
    
    for (std::map<int, DoorDict*>::iterator IT = theDoorAssets.begin(); IT!=theDoorAssets.end(); IT++){
        
        
        
        IT->second->texture = generateTexture(IT->second);
        parseJsonFrame(IT->second);
        IT->second->init = true;
    }
    
    
    
    
    
    
    return false;
}

void Door::updateAll(){

    for (std::set<Door*>::iterator IT= thinkingDoors.begin();
         IT != thinkingDoors.end();
         IT++){
        
        (*IT)->update();
        
    }

}

void Door::thinkAll(){

    for (std::set<Door*>::iterator IT= thinkingDoors.begin();
         IT != thinkingDoors.end();
         IT++){
    
        (*IT)->think();
    
    }
         
    

}

void Door::think(){
    

    
    int range =2;
    
    
     bool testValue = false;
    
    if (index>0&&index<Tiles::width*Tiles::width){
        
        std::pair<int,int> position = Tiles::findTile(index);
        
        int xMin=-range;
        
        if (position.first<range+1){
            xMin = -position.first;
        }
    
        int xMax=3;
        
        if (position.first+range>=Tiles::width){
            xMax = Tiles::width-position.first;
        
        }
        
        
        int yMin=-range;
        
        if (position.second<range+1){
            yMin = -position.second;
        }
        
        int yMax=range;
        
        if (position.second+range>=Tiles::width){
            yMax = Tiles::width-position.first;
            
        }
        
        
        
        for (int counterX = position.first+xMin;
             counterX<position.first+xMax+1;
             counterX++){
            
            for (int counterY=position.second+yMin;
                 counterY<position.second+yMax+2;
                 counterY++){
                
                
                
                int target = counterX + counterY*Tiles::width;
                if (!Tiles::valueSafe(target)){
                    continue;
                
                }
                
                std::vector<Sprite*> * grid = &Tiles::mapValues[target].renderGrid;
            
                for (std::vector<Sprite*>::iterator IT = grid->begin(); IT != grid->end();IT++){
                
                    if (!(*IT)->dict->prop){
                    
                        testValue=true;
                        counterX+=100;
                        counterY+=100;
                    
                    }
                }
            }
        }
    }
    
    

    
    
     openDoor=testValue;
    
   
    if (openDoor){
    
        int targetFrame;
        if (LeftOrRight){
        
            targetFrame = dict->frames[OPEN_R];
            
            
        
        } else {
            
            targetFrame = dict->frames[OPEN_L];
            
        }
        
        
        if (targetFrame>frame){
            frame++;
        
        }
    
    
    
    } else {
        int targetFrame;
        
        if (LeftOrRight){
            targetFrame = dict->frames[CLOSED_R];
            
        
        } else {
            targetFrame = dict->frames[CLOSED_L];
        
        
        }
        
        if (targetFrame<frame){
            frame--;
            
        }
    
    
    }
    
   
    
    
    

    
    
}

bool Door::dumpTextureCord(int frame, GLfloat * text){
    
    
    return  Door::dumpTextureCord(dict->index, frame, text, dict);
    
    
    
    
}

bool Door::dumpTextureCord(int index, int frame, GLfloat * text, DoorDict * target){
        
    if (!target){
    
        target = Door::theDoorAssets[index];
    
    }


    DoorDict * dict = target;
    
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
        
        
    } else {
        
        text[0]=  0.0;
        text[1]=  0.0;
        text[2]=  1.0;
        text[3]=  1.0;
        text[4]= -1.0;
        text[5]=  1.0;
        text[6]= -1.0;
        text[7]= -1.0;
        text[8]=  1.0;
        text[9]=  1.0;
        text[10]= 1.0;
        text[11]=-1.0;
        
        
        
    }
    
    
    
    return true;
    
    
    
    
}

void Door::update(){
    
   

    
    if (_texture==0){
        if (dict->texture==0){
            generateTexture(dict);
        }
        _texture = dict->texture;
    }
    
    if ( dict->animates   ){
        
        cpVect pos = Tiles::findCords(Tiles::findTile(index));
        
        GLfloat text[12];
        
        dumpTextureCord(frame, text);
        
        GLfloat sludgeX = (LeftOrRight)?dict->sludgeX_R:dict->sludgeX_L;
        GLfloat sludgeY = (LeftOrRight)?dict->sludgeY_R:dict->sludgeY_L;
        
        GLfloat midWayPoint = (text[4]+text[8])/2.0;

        GLfloat textureMidPoint = (text[0]+text[2])/2.0;
        
        GLfloat gBoxVertexData[] = {
            
            (GLfloat)(text[4]+pos.x+sludgeX),
            (GLfloat)(text[5]+pos.y+sludgeY),
                    text[0],  text[1],
            
            
            (GLfloat)(text[6]+pos.x+sludgeX),
            (GLfloat)(text[7]+pos.y+sludgeY),
                    text[0],  text[3],
            
            (GLfloat)(midWayPoint+pos.x+sludgeX),
            (GLfloat)(text[9]+pos.y+sludgeY),
                    textureMidPoint,  text[1],
            
            (GLfloat)(midWayPoint+pos.x+sludgeX),
            (GLfloat)(text[11]+pos.y+sludgeY),
                  textureMidPoint,  text[3],
            
            
            
            
            
            
            
            
            (GLfloat)(midWayPoint+pos.x+sludgeX),
            (GLfloat)(text[5]+pos.y+sludgeY),
            textureMidPoint,  text[1],
            
            
            (GLfloat)(midWayPoint+pos.x+sludgeX),
            (GLfloat)(text[7]+pos.y+sludgeY),
            textureMidPoint,  text[3],
            
            (GLfloat)(text[8]+pos.x+sludgeX),
            (GLfloat)(text[9]+pos.y+sludgeY),
            text[2],  text[1],
            
            (GLfloat)(text[10]+pos.x+sludgeX),
            (GLfloat)(text[11]+pos.y+sludgeY),
            text[2],  text[3]
            
            
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

void Door::render(){
    
   
    
    if (_vertexArray!=0){
        if (_texture){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *_texture);
            glBindVertexArrayCon(_vertexArray);
        
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        }
    }


}


void Door::renderA(){
    
    
    
    if (LeftOrRight){
        if (frame == dict->frames[CLOSED_R]){
            render();
            return;
        }
    } else {
        if (frame == dict->frames[CLOSED_L]){
            render();
            return;
        }
        
        
    }


    
    
    
   
    
    
    if (_vertexArray!=0){
        if (_texture){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *_texture);
            glBindVertexArrayCon(_vertexArray);
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   
        }
    }
    
    
}

void Door::renderB(){
    
    if (LeftOrRight){
        if (frame == dict->frames[CLOSED_R]){
            //render();
            return;
        }
    } else {
        if (frame == dict->frames[CLOSED_L]){
            //render();
            return;
        }
        
        
    }
    
    
    if (_vertexArray!=0){
        if (_texture){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *_texture);
            glBindVertexArrayCon(_vertexArray);
            
         
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        }
    }
    
    
}









void Door::destroy(){
    
    
    if (_vertexArray){
        glDeleteVertexArraysCon(1, &_vertexArray);
        
    }
    
    if (_vertexBuffer){
        glDeleteBuffers(1, &_vertexBuffer);
    }

}

Door::Door(){
    _texture=0;
    _vertexArray=0;
    _vertexBuffer=0;
}

Door::Door(int _index, int _type, bool _leftOrRight){

    LeftOrRight=_leftOrRight;
    
    index=_index;
    type=_type;
    openDoor=false;
    _texture=0;
    _vertexArray=0;
    _vertexBuffer=0;
    dict=theDoorAssets[type];
    
    frame = (LeftOrRight)?dict->frames[CLOSED_R]:dict->frames[CLOSED_L];
    
}

Door::~Door(){



}


Door* Door::addDoor(int _theIndex, int type, bool _leftOrRight){


    int target =  (_leftOrRight)?_theIndex*2:_theIndex*2+1;
    
    if (indexMap.find(type)==indexMap.end()){
    
        Door * newDoor = new Door(_theIndex,type ,_leftOrRight);
        indexMap[target]=newDoor;
        thinkingDoors.insert(newDoor);
        return newDoor;
        
    
    } else {
    
        return 0;
    
    
    }
}


Door* Door::addDoor(int _theIndex, bool _leftOrRight){
    
    return addDoor(_theIndex, 1, _leftOrRight);
}
bool Door::removeDoor(int _theIndex, bool _leftOrRight){
    
    int target =  (_leftOrRight)?_theIndex*2:_theIndex*2+1;
    
    if (indexMap.find(target)==indexMap.end()){
        return false;
    } else {
        
        Door * aDoor = indexMap[target];
        thinkingDoors.erase(aDoor);
        indexMap.erase(target);
        aDoor->destroy();
        
        return 0;
        
        
    }
}



bool Door::checkDoor(int _theIndex, bool _leftOrRight, Sprite*theSprite, Door ** theDoor){

    int target =  (_leftOrRight)?_theIndex*2:_theIndex*2+1;
    
    
    if (indexMap.find(target)==indexMap.end()){
        return true;
    }
    
    *theDoor = indexMap[target];
    return false;
    
    
}



