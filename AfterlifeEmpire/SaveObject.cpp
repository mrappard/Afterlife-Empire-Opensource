//
//  SaveObject.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-11-18.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#include "SaveObject.h"

#include "Tiles.h"
#include "FileManager.h"
#include "Sprite.h"
#include <limits>
#include "GameLogic.h"
#include "Door.h"
#include "BobZombie.h"

TileSaveObject::TileSaveObject(){

    type=0;
    lw=0;
    rw=0;
    doorR=0;
    doorL=0;

}


TileSaveObject::~TileSaveObject(){}


bool SaveObject::save(std::string name){
    
    SaveObject newObject;
    
    newObject.copyWalls();
    newObject.copySprites();
    newObject.allTogetherFile();
    
    saveChar(name, &newObject);
    
    return true;
    
}


void SaveObject::getThumbnail(){


}


bool SaveObject::createSprites(){

    Sprite::purgeAll();
    
    char * start=&(*sprites.begin());
	int numberOfSprites = sprites.size()/sizeof(SpriteSaveObject);
    
    SpriteSaveObject * theStart = (SpriteSaveObject*)start;
  
    
    
    std::vector<SpriteSaveObject> theObjects = std::vector<SpriteSaveObject>(theStart,theStart+numberOfSprites);
    
    for (std::vector<SpriteSaveObject>::iterator IT = theObjects.begin();
         IT !=theObjects.end();
         IT++){
        
        Sprite::addSpriteSave(cpv(IT->xPos,IT->yPos),IT->type);
        
    }
    
    return true;

}

void SaveObject::activate(){
    
    GameLogic::purge(true);

    parseWalls();
    
    
    createSprites();
    getThumbnail();
    
    Tiles::width  = (int)widthT;
    Tiles::height = (int)heightT;
    
    GameLogic::energy = energy;
	GameLogic::awareness = awareness;
    GameLogic::currentLevel = currentLevel;
	GameLogic::timePassed = clock;
    BobZombie::setStates(bobStates);
    Tiles::hasBeenCheckedForWalkability = false;
    Tiles::checkWalkAbility();

}

bool SaveObject::load(std::string _name){
    
    
    
    
    name = _name;
    
    std::vector<char> tempBuffer;
    
    if (loadLevel(name, &tempBuffer)){
        
        if (!init(&tempBuffer)){
        
            return false;
        } 
        
        
        
   
        return true;
        
    } else {
        return false;
        
    }
    return false;
    
    
    
}


void SaveObject::clear(){

    
    if (name!="null"){
        deleteFile(name);
    }
}


void convert_size(std::vector<char> * add, char * trick, size_t index){
    
    
    
    for (size_t counter = 0; counter<8; counter++){
        add->at(index+counter)=trick[counter];
    }
}


size_t convert_to_size(std::vector<char> * from, size_t index ){
    
    size_t returnValue[2];

	memcpy(returnValue, &from->at(index), 8);

    
    return returnValue[0];
    
    
}



char * SaveObject::allTogetherFile(){




	size_t sizeOfData = header.size() +
		sizeof(size_t) * 3 +
		walls.size()*sizeof(TileSaveObject) +
		sprites.size()*sizeof(char);

	output.resize(sizeOfData,125);

	size_t copyPos = 0;
	char * frontOfTheGrid = &output[0];

	std::copy(header.begin(), header.end(), output.begin());
	copyPos += header.size();
	frontOfTheGrid = frontOfTheGrid + header.size();


	memcpy(frontOfTheGrid, &widthW, sizeof(size_t));
	copyPos += sizeof(size_t);
	frontOfTheGrid = frontOfTheGrid + sizeof(size_t);

	memcpy(frontOfTheGrid, &heightW, sizeof(size_t));
	copyPos += sizeof(size_t);
	frontOfTheGrid = frontOfTheGrid + sizeof(size_t);



	char * start = (char*)&(*walls.begin());
	char * end = start + walls.size()*sizeof(TileSaveObject);

	std::copy(start, end, (output.begin() + copyPos));
	copyPos += walls.size()*sizeof(TileSaveObject);
	frontOfTheGrid = frontOfTheGrid + walls.size()*sizeof(TileSaveObject);

	size_t sizeOfString = sprites.size();

	memcpy(frontOfTheGrid, &sizeOfString, sizeof(size_t));
	copyPos += sizeof(size_t);
	frontOfTheGrid = frontOfTheGrid + sizeof(size_t);

	std::copy(sprites.begin(), sprites.end(), output.begin() + copyPos);

	char theEnd = output.back();		
	char theEndB = *((output.rbegin())++);
	size_t currentSize = output.size();
	Tiles::grabMap(&output);


	{
		size_t sizeAfterThumb = output.size();

		
	
		for (size_t counter = 0; counter < sizeof(size_t) * 100; counter++){
			output.push_back(0);
		}


		size_t otherVars[5];
		otherVars[0] = GameLogic::energy;
		otherVars[1] = GameLogic::currentLevel;
		otherVars[2] = BobZombie::returnState();
		otherVars[3] = GameLogic::awareness;
		otherVars[4] = GameLogic::timePassed;

		char *startPart = &output[sizeAfterThumb];

		memcpy(startPart, otherVars, sizeof(otherVars));

	}
	return 0;
}
/*
char * SaveObject::allTogetherFile(){
	

	
    
    size_t sizeOfData = header.size() +
    sizeof(size_t)*3+
    walls.size()*sizeof(TileSaveObject) +
    sprites.size()*sizeof(char);
    
    output.resize(sizeOfData);
    
    size_t copyPos=0;
    
    char * trick = 0;
    std::vector<char> convert;
    
    std::copy(header.begin(), header.end(), output.begin());
    copyPos+=header.size();
    
    trick = (char*)&widthW;
    convert_size(&output, trick,copyPos);
    
    copyPos+=sizeof(size_t);
    
    trick = (char*)&heightW;
    convert_size(&output, trick,copyPos);
    
    copyPos+=sizeof(size_t);
    
    char * start = (char*)&(*walls.begin());
    char * end = start+walls.size()*sizeof(TileSaveObject);
    
    std::copy(start, end, (output.begin()+copyPos));
    copyPos+=walls.size()*sizeof(TileSaveObject);
    
    size_t sizeOfString = sprites.size();
    trick = (char*)&sizeOfString;
    convert_size(&output, trick,copyPos);
    
    copyPos+=sizeof(size_t);
    std::copy(sprites.begin(), sprites.end(), output.begin()+copyPos);
    
    Tiles::grabMap(&output);
   
    {
    size_t sizeAfterThumb = output.size();
    
    output.insert(output.end()--, sizeof(size_t)*5,0);
    
    
    size_t otherVars[5];
    otherVars[0] = GameLogic::energy;
    otherVars[1] = GameLogic::currentLevel;
    otherVars[2] = BobZombie::returnState();
        otherVars[3] = GameLogic::awareness;
    otherVars[4] = 345678901;
    

    for (size_t counter = 0; counter<  sizeof(size_t)*5; counter++){
        
        
        char * otherToChar = (char*)otherVars;
        
        output[sizeAfterThumb+counter] = otherToChar[counter];
        
        
        
    }
        
        
    }
    
    
    
    

    
   
    
    return 0;
    
}
*/
bool SaveObject::parseWalls(){
    
    


    
    int counter =0;
    for (std::vector<TileSaveObject>::iterator IT = walls.begin();
         IT != walls.end();
         IT++){
        
        TileObject newObject;
        
        newObject.type =IT->type;
        
        
        if (IT->lw==0){
            newObject.lw= false;
        
        } else {
            newObject.lw=true;
            newObject.textureWallL  =Tiles::tileDictVector[IT->lw]->textureL;
            //newObject.textureWallL  =Tiles::textureWallDictL[IT->lw];
        }
        
        if (IT->rw==0){
            newObject.rw= false;
            
            
        } else {
            newObject.rw=true;
            newObject.textureWallR  =Tiles::tileDictVector[IT->rw]->textureR;
            //newObject.textureWallR  =Tiles::textureWallDictR[IT->rw];
        }
        
        Tiles::mapValues[counter]=newObject;
        
        if (IT->doorL==0){
            newObject.theDoorL=0;
        } else {
            
           
           Tiles::mapValues[counter].theDoorL =  Door::addDoor(counter, IT->doorL, true);
            Tiles::mapValues[counter].lw=false;
        }
        
        if (IT->doorR==0){
            newObject.theDoorR=0;
        } else {
           Tiles::mapValues[counter].theDoorR = Door::addDoor(counter, IT->doorR, false);
             Tiles::mapValues[counter].rw=false;
        }
        
        
        counter++;
        
    }
    
    
    
    
    
    return true;
}

bool SaveObject::copyWalls(){
    
    
    size_t maxSize = Tiles::mapValues.size();
    
    walls.clear();
    
    
    std::map<GLuint*, int> L1;
    std::map<GLuint*, int> R1;
    
    
    

    
    for (std::map<int, TileDict*>::iterator IT = Tiles::tileDictionary.begin();
         IT != Tiles::tileDictionary.end();
         IT++){
        
        L1[IT->second->textureL]=IT->first;
        R1[IT->second->textureR]=IT->first;
 
    
    }
    
    for (size_t counter = 0; counter<maxSize; counter++){
        
        TileObject * object = &Tiles::mapValues[counter];
        TileSaveObject insert=TileSaveObject();
        
        
        insert.type=object->type;
       
        if (object->lw){
            insert.lw  =L1[object->textureWallL];
        } else {
            insert.lw = 0;
        }
        
        if (object->rw){
            insert.rw  =R1[object->textureWallR];
        } else {
            insert.rw=0;
        }
        
        
        if (object->theDoorL){
            insert.doorL = object->theDoorL->type;
        
        } else {
            insert.doorL =0;
        
        }
        
        if (object->theDoorR){
            insert.doorR = object->theDoorR->type;
        
        } else {
            insert.doorR=0;
        }
        
        
        walls.push_back(insert);
        
        
    }
    
    
    
    widthW=Tiles::width;
    heightW=Tiles::height;
    
    return  true;
    
    
}

bool SaveObject::copySprites(){
    
    std::vector<SpriteSaveObject> theObjects;

    for (std::map<size_t, Sprite*>::iterator IT=Sprite::spriteByID.begin();
         Sprite::spriteByID.end()!=IT;
         IT++){
        
        if (IT->second->dict->prop){
    
            cpVect thePos = IT->second->position;
            SpriteSaveObject insertMe;
        
            insertMe.xPos=thePos.x;
            insertMe.yPos=thePos.y;
            insertMe.type=IT->second->type;
        
            theObjects.push_back(insertMe);
        }
    }
    
    
    
    
    SpriteSaveObject * theStart = &theObjects[0];
    SpriteSaveObject * theEnd = &theObjects.back();
    char * theStartChar = (char*)theStart;
    char * theEndChar = (char*)theEnd+(sizeof(SpriteSaveObject));
    
  
    
    sprites = std::vector<char>(theStartChar, theEndChar);
    
    return true;
    
}

bool SaveObject::createThumbNail(){

    
    
    
    

    return true;
}


bool SaveObject::init( std::vector<char> * data){
    
    header.resize(12);
    
    std::copy(data->begin(), data->begin()+12, header.begin());
    
    size_t step = 12;
    if (header=="ALEmpire0.06"){
    
        widthT = convert_to_size(data, step);
        step += sizeof(size_t);
        heightT = convert_to_size(data, step);
        step += sizeof(size_t);
        
        size_t tileSize = sizeof(TileSaveObject)*widthT*heightT;
		walls.clear();
		walls.assign(widthT*heightT, TileSaveObject());
        
		memcpy(&walls[0], &data->at(step), tileSize);
        step+=tileSize;
        
        size_t spriteSize = convert_to_size(data, step);
        step+=sizeof(size_t);
        if (spriteSize){
			sprites.clear();
			sprites.assign(spriteSize, 0);
			memcpy(&sprites[0], &data->at(step), spriteSize);
            //sprites =std::vector<char>(data->begin()+step,data->begin()+step+spriteSize);
        }
        step+=spriteSize;
        
		thumbnail.clear();
		thumbnail.assign(256 * 256 * 4, 0);
		memcpy(&thumbnail[0], &data->at(step), 256 * 256 * 4);

        step += (256*256*4);
        
		
        size_t v1 = convert_to_size(data, step);
        
        energy = v1;
        step += sizeof(size_t);
        size_t v2 = convert_to_size(data, step);
        currentLevel = v2;
        step += sizeof(size_t);
        bobStates = convert_to_size(data, step);
        step += sizeof(size_t);
        size_t v4 = convert_to_size(data, step);
        if (v4 != 234567890){
			awareness = v4;
        }
        step += sizeof(size_t);
        size_t v5 = convert_to_size(data, step);
		clock = v5;
        step += sizeof(size_t);
    
        return true;
    
    }
    
    
    else if (header=="ALEmpire0.05"){
        
        widthT = convert_to_size(data, step);
        step += sizeof(size_t);
        heightT = convert_to_size(data, step);
        step += sizeof(size_t);
        
        size_t tileSize = sizeof(TileSaveObject)*widthT*heightT;
    

        
        TileSaveObject * pointerS = (TileSaveObject *) &(*(data->begin()+step));
        TileSaveObject * pointerE = pointerS+(widthT*heightT);
        
        
        walls = std::vector<TileSaveObject>(pointerS,pointerE);
        
        step+=tileSize;
        
        size_t spriteSize = convert_to_size(data, step);
        step+=sizeof(size_t);
        if (spriteSize){
            sprites =std::vector<char>(data->begin()+step,data->begin()+step+spriteSize);
        }
            step+=spriteSize;
        
        char * thumbBegin = (char*)&(*(data->begin()+step));
        char * thumbEnd = thumbBegin + (256*256*4);
        
        thumbnail = std::vector<char>(thumbBegin,thumbEnd);
        
        return true;
    } else {
        return false;
    }
    
    
    
    
    
    
    
}



SaveObject::SaveObject(){
    header = "ALEmpire0.06";
    name = "null";
    
}
SaveObject::~SaveObject(){
    
    
}

#include <fstream>
#include "DebugTools.h"
#include "GameClock.h"


void SaveObject::changeJsonToRaw(void * theSpriteDict, int type){
    

    if (type == 0){
	
        SpriteDict * aSpriteDict = (SpriteDict *)theSpriteDict;
        
        std::string fastCreateName = aSpriteDict->jsonName;
        fastCreateName = fastCreateName.append("f");
        
        std::string path = findFile(fastCreateName);
		
        if (path=="null"){
        //    if (true){
            if (aSpriteDict->textureArray.size()==0){
                return;
            }
            
            
            std::string header = "HEAD2080";
            
            
            unsigned int textureSize = aSpriteDict->textureArray.size()*sizeof(GLfloat);
			unsigned int offsetSize = aSpriteDict->offsetArray.size()*sizeof(GLfloat);
            
            
			unsigned int theWidth = aSpriteDict->segWidth;
			unsigned int theHeight = aSpriteDict->segHeight;
            
			unsigned int attrib[4] = { textureSize, offsetSize, theWidth, theHeight };
            
            
            std::vector<char> theFile;
			theFile.assign(header.size() + textureSize + offsetSize + sizeof(unsigned int) * 4, 0);
            
			unsigned int offset = 0;
            
            std::copy(header.begin(), header.end(), theFile.begin());
            
            offset+=header.size();
            
            char * start = (char*)attrib;
			char * end = start + sizeof(unsigned int) * 4;
            
            std::copy(start, end, theFile.begin()+offset);
            
			offset += sizeof(unsigned int) * 4;
            
            
            start = (char*)&(*aSpriteDict->textureArray.begin());
			unsigned int step = aSpriteDict->textureArray.size()*sizeof(GLfloat);
            end = start+step;
            
            std::copy(start, end, theFile.begin()+offset);
            
            offset+=step;
            
            start = (char*)&(*aSpriteDict->offsetArray.begin());
            step =aSpriteDict->offsetArray.size()*sizeof(GLfloat);
            end = start+step;
            
            std::copy(start, end, theFile.begin()+offset);
            
            offset+=step;
            
            std::string theName = aSpriteDict->jsonName;
            theName = theName.append("f");
            
            
            
            outputData(theName, &theFile[0], theFile.size());
            
			
            
            
            
        }else {
            
            

            
            //
            
            
            
            std::fstream in;
            unsigned long fileSize;

            
			in.open(path, std::ios_base::binary | std::ios_base::in  | std::ios::ate);
		
            if(in.fail()){
                std::string errorMessage;
                errorMessage = "The File ";
                errorMessage = errorMessage.append(path);
                errorMessage = errorMessage.append(" is corrupt");
            
                DebugTools::spawnErrorWindow(errorMessage);
                quitTheGame();
                
                return;
            }
            // Get Filesize
            //in.seekg(0,std::ios_base::end);
            fileSize=in.tellg();
            in.seekg(0,std::ios_base::beg);
			
            // allocate space for file data
            char * dat=new char[fileSize];
            
            // Read filedata
            if(!dat){
                
                std::string errorMessage = "Memory error when creating file ";
                errorMessage = errorMessage.append(path);
                
                DebugTools::spawnErrorWindow(errorMessage);
            
                
                quitTheGame();
                
                return;
            }
            
            in.read(dat,fileSize);
		
            if(in.fail())
            {
                delete [] dat;
                in.close();
                
                std::string errorMessage = "Could not read file  ";
                errorMessage = errorMessage.append(path);
                
                DebugTools::spawnErrorWindow(errorMessage);
                quitTheGame();
                
                return;
            }
            
            in.close();
			
            std::string subString = std::string(&dat[0], &dat[8]);
		
            
            if (subString != "HEAD2080"){
            
                std::string errorMessage = "The Header is Wrong for file  ";
                errorMessage = errorMessage.append(path);
                
                DebugTools::spawnErrorWindow(errorMessage);
                quitTheGame();
            
            }
         
            unsigned int textureSize, offsetSize, segWidth, seqHeight;
		
            
            // Grab the rest of the header
            memcpy(&textureSize,&dat[8],sizeof(unsigned int));
            memcpy(&offsetSize,&dat[12],sizeof(unsigned int));
			memcpy(&segWidth, &dat[16], sizeof(unsigned int));
			memcpy(&seqHeight, &dat[20], sizeof(unsigned int));
		
            aSpriteDict->segWidth = segWidth;
            aSpriteDict->segHeight = seqHeight;
            
            aSpriteDict->textureArray.clear();//
            aSpriteDict->textureArray.resize( textureSize/sizeof(GLfloat));
			aSpriteDict->offsetArray.resize(offsetSize / sizeof(GLfloat));
            
            memcpy(&aSpriteDict->textureArray[0], &dat[24], textureSize);
            memcpy(&aSpriteDict->offsetArray[0], &dat[24+textureSize], offsetSize);
            
          
            free(dat) ;
		

            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            /*
            std::vector<char> tempBuffer;
            
            if (loadLevel(path, &tempBuffer)){
                
                std::string subString = std::string(tempBuffer.begin(), tempBuffer.begin()+8);
                
                if (subString ==  "HEAD2080"){
                    
					

                    char * startAfterHeader = (&tempBuffer[0])+(sizeof(char)*8);

					unsigned int* openStuff = (unsigned int*)startAfterHeader;
                    
					
                    
					unsigned int textureSize = openStuff[0] / sizeof(GLfloat);
					unsigned int offsetSize = openStuff[1] / sizeof(GLfloat);
                    
                    
					unsigned int theWidth = openStuff[2];
					unsigned int theHeight = openStuff[3];

					printf("%s,%i,%i,%i,%i\n  ", path.c_str(), textureSize, offsetSize, theWidth, theHeight);

                    
                    GLfloat * start = (GLfloat*)(openStuff + 4);
                    
                    aSpriteDict->segWidth = theWidth;
                    aSpriteDict->segHeight = theHeight;
                    
                    aSpriteDict->textureArray  = std::vector<GLfloat>(start, start+textureSize);
                    aSpriteDict->offsetArray = std::vector<GLfloat>(start+textureSize, start+textureSize+offsetSize);
                    
                    
                }
                
                
                
            }
            */
        }
    } else if (type == 1){
    
    
        DoorDict * aSpriteDict = (DoorDict *)theSpriteDict;
        
        std::string fastCreateName = aSpriteDict->jsonName;
        fastCreateName = fastCreateName.append("f");
        
        std::string path = findFile(fastCreateName);
        
        if (path=="null"){
        //    if (true){
            if (aSpriteDict->textureArray.size()==0){
                return;
            }
            
            
            std::string header = "HEAD2080";
            
            
			unsigned int textureSize = aSpriteDict->textureArray.size()*sizeof(GLfloat);
			unsigned int offsetSize = aSpriteDict->offsetArray.size()*sizeof(GLfloat);
            
            
			unsigned int theWidth = aSpriteDict->segWidth;
			unsigned int theHeight = aSpriteDict->segHeight;
            
			unsigned int attrib[4] = { textureSize, offsetSize, theWidth, theHeight };
            
            
            std::vector<char> theFile;
			theFile.assign(header.size() + textureSize + offsetSize + sizeof(unsigned int) * 4, 0);
            
			unsigned int offset = 0;
            
            std::copy(header.begin(), header.end(), theFile.begin());
            
            offset+=header.size();
            
            char * start = (char*)attrib;
			char * end = start + sizeof(unsigned int) * 4;
            
            std::copy(start, end, theFile.begin()+offset);
            
			offset += sizeof(unsigned int) * 4;
            
            
            start = (char*)&(*aSpriteDict->textureArray.begin());
			unsigned int step = aSpriteDict->textureArray.size()*sizeof(GLfloat);
            end = start+step;
            
            std::copy(start, end, theFile.begin()+offset);
            
            offset+=step;
            
            start = (char*)&(*aSpriteDict->offsetArray.begin());
            step =aSpriteDict->offsetArray.size()*sizeof(GLfloat);
            end = start+step;
            
            std::copy(start, end, theFile.begin()+offset);
            
            offset+=step;
            
            std::string theName = aSpriteDict->jsonName;
            theName = theName.append("f");
            
            
            
            outputData(theName, &theFile[0], theFile.size());
            
            
            
            
            
        }else {
            
            std::vector<char> tempBuffer;
            
            if (loadLevel(path, &tempBuffer)){
                
                std::string subString = std::string(tempBuffer.begin(), tempBuffer.begin()+8);
                
                if (subString ==  "HEAD2080"){
                    
                    
                    char * startAfterHeader = &tempBuffer[8];

					
                    
					unsigned int * openStuff = (unsigned int*)startAfterHeader;
                    
                    
                    
					unsigned int textureSize = openStuff[0] / sizeof(GLfloat);
					unsigned int offsetSize = openStuff[1] / sizeof(GLfloat);
                    
                    
					unsigned int theWidth = openStuff[2];
					unsigned int theHeight = openStuff[3];
                    
                    GLfloat * start = (GLfloat*)(openStuff + 4);
                    
                    aSpriteDict->segWidth = theWidth;
                    aSpriteDict->segHeight = theHeight;
                    
                    aSpriteDict->textureArray  = std::vector<GLfloat>(start, start+textureSize);
                    aSpriteDict->offsetArray = std::vector<GLfloat>(start+textureSize, start+textureSize+offsetSize);
                    
                    
                }
                
                
                
            }
            
        }
    }
    
}


