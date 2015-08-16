//
//  AudioController.cpp
//  After Life Empire
//
//  Created by Autobotika on 2015-01-24.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//
#include "AudioController.h"

#include <fstream>
#include "FileManager.h"
#include <algorithm>
#include "GlobalDebugger.h"

#undef check
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>




//#include "AudioTranscode.h"

ALCcontext * AudioController::context;
ALCdevice * AudioController::device;




std::map<std::string, std::vector<ALuint> > AudioController::groups;

std::map<std::string, soundItem>  AudioController::loadedSounds;
std::map<std::string, SoundBuffer> AudioController::bufferSounds;

std::map<int, AudioElement> AudioController::audioLink;

std::map<std::string, ALfloat>  AudioController::volumes;

double AudioController::changeSize=0;

void AudioController::clearAudio(){
    
    
    for (
         std::map<std::string, std::vector<ALuint> >::iterator IT = groups.begin();
         IT != groups.end();
         IT++){
    
        for (std::vector<ALuint>::iterator IT2 = IT->second.begin();
             IT2 != IT->second.end();
             IT2++){
            
            ALuint value = *IT2;
            alSourceStop(*IT2);
            alDeleteSources(1, &value);
        
        }
    }
    

    for (
    std::map<std::string, soundItem>::iterator IT =loadedSounds.begin();
         IT != loadedSounds.end();
         IT++){
    
        alDeleteBuffers(1, &IT->second.buffer);
        
    
    
    }

    audioLink.clear();
    groups.clear();
    volumes.clear();
    loadedSounds.clear();
    bufferSounds.clear();
    


}

std::string AudioController::reverseJson(int index){

    std::string theText;
    theText.append("{");
    
    
    std::map<int,AudioElement>::iterator IT = audioLink.find(index);
    
    if (IT != audioLink.end()){
        for (std::map<std::string, std::set<std::string> >::iterator IT2 = IT->second.elements.begin();
             IT2 != IT->second.elements.end();
             IT2++
             ){
            
            theText.append("\n\t\"");
            theText.append(IT2->first);
            theText.append("\":[");
            
            theText.append(IT2->first);
            
            for (std::set<std::string>::iterator IT3 = IT2->second.begin();
                 IT3 != IT2->second.end();
                 IT3++){
                
                theText.append("\"");
                theText.append((*IT3));
                theText.append("\"");
                
                std::set<std::string>::iterator IT4 = IT3;
                IT4++;
                if (IT4!=IT2->second.end()){
                
                    theText.append(",");
                }
            }
            
            theText.append("]");
            

            std::map<std::string, std::set<std::string> >::iterator IT5 = IT2;
            
            IT5++;
            
            if (IT5 != IT->second.elements.end()){
            
                theText.append(",");
            }
            
            
        
        
        }
        
        
    }

    theText.append("}");
    return theText;
}


void AudioController::clearIndex(int index){
    
    std::map<int,AudioElement>::iterator IT = audioLink.find(index);
    
    if (IT ==audioLink.end()){
        return;
    
    }
    
    
    
    for (
    std::map<std::string, std::set<std::string> >::iterator IT2 = IT->second.elements.begin();
         IT2 != IT->second.elements.end();
         IT2++){
        
        for (std::set<std::string>::iterator IT3 = IT2->second.begin();
             IT3 != IT2->second.end();
             IT3++) {
            
            std::string key = *IT3;
            
            std::map<std::string, soundItem>::iterator item =loadedSounds.find(key);
            
            if (item != loadedSounds.end()){
                alDeleteBuffers(1, &item->second.buffer);
            }
            
            loadedSounds.erase(key);
            
        }
    
    
    
    }

    audioLink.erase(index);

}


void AudioController::jsonAudio(int index, std::string a1, std::string a2){

    std::map<int,AudioElement>::iterator IT = audioLink.find(index);
    
    if (IT ==audioLink.end()){
        audioLink[index]= AudioElement();
        IT = audioLink.find(index);
    }
    
    std::map<std::string, std::set<std::string> >::iterator IT2 = IT->second.elements.find(a1);
    
    if (IT2 == IT->second.elements.end()){
        IT->second.elements[a1] = std::set<std::string>();
        IT2 = IT->second.elements.find(a1);
    }
    
    IT2->second.insert(a2);
    
}

void AudioController::initAll(){
    
    for (
    std::map<int,AudioElement>::iterator IT = audioLink.begin();
         IT != audioLink.end();
         IT++){
    
        for (
             std::map<std::string, std::set<std::string> > ::iterator IT2 = IT->second.elements.begin();
             IT2 != IT->second.elements.end();
             IT2++){
            
         
            
            std::string value = IT2->first;
        
            for (std::set<std::string>::iterator IT3 = IT2->second.begin();
                 IT3 != IT2->second.end();
                 IT3++){
                
                
                
            AudioController::loadAudio(*IT3, 3, "Effect");
            
            }
        
        }
        
    
    }



}


void AudioController::playAudioLua(int type, std::string soundType){
    
    std::map<int,AudioElement>::iterator IT = audioLink.find(type);
    
    if (IT!= audioLink.end()){
    
        std::map<std::string, std::set<std::string> >::iterator IT2 = IT->second.elements.find(soundType);
        
        if (IT2 != IT->second.elements.end()){
            size_t size = IT2->second.size();
            
            std::set<std::string>::iterator IT3 = IT2->second.begin();//(rand()%size)
            
            advance(IT3,rand()%size);
            AudioController::playAudio(*IT3);
        }
    }

}

int AudioController::init() {
    
    const ALCchar *default_device;
    
    default_device = alcGetString(NULL,
                                  ALC_DEFAULT_DEVICE_SPECIFIER);
    
    printf("using default device: %s\n", default_device);
    
    if ((device = alcOpenDevice(default_device)) == NULL) {
        fprintf(stderr, "failed to open sound device\n");
        return -1;
    }
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alcProcessContext(context);
    
    alGetError(); /* clear any AL errors beforehand */
    
  /*
    ALCint size;
    alcGetIntegerv( device, ALC_ATTRIBUTES_SIZE, 1, &size);
    std::vector<ALCint> attrs(size);
    alcGetIntegerv( device, ALC_ALL_ATTRIBUTES, size, &attrs[0] );
    for(int i=0; i<attrs.size(); ++i)
    {
        if( attrs[i] == ALC_MONO_SOURCES )
        {
            std::cout << "max mono sources: " << attrs.at(i+1) << std::endl;
        }
    }
    
    */
    
    return 0;
}

void SoundBuffer::createSource(){

    position[0]=0.0;
    position[1]=0.0;
    position[2]=0.0;
    
    
    ALuint sourceid[1];
    
    alGenSources(1,sourceid);
    source = sourceid[0];
    
    ALfloat sourcePos[]={ 0.0, 0.0, 0.0};
    ALfloat sourceVel[]={ 0.0, 0.0, 0.0};
    ALfloat sourceOri[]={ 0.0, 0.0, 0.0};
    
    alSourcefv (source, AL_POSITION, sourcePos);
    alSourcefv (source, AL_VELOCITY, sourceVel);
    alSourcefv (source, AL_DIRECTION, sourceOri);
    alSourcei  (source, AL_LOOPING, AL_TRUE);
        
   
    
    
}

bool SoundBuffer::loadNewChunk(ALuint bufferID){

    size_t bufferSize = 32000;
    // How Many Chunks
    size_t totalChunks = fileSize/bufferSize;
    //Are We Past The End of the File
    if (bufferIndex>totalChunks){
        return false;
    }
    //Find the Offset
    size_t startOffset = bufferIndex*bufferSize;
    
    //Is this the end if so it needs to be a different size
    if (bufferIndex==totalChunks){
        size_t leftOverBytes = fileSize - (bufferSize*totalChunks);
        bufferSize = leftOverBytes;
    }
    
    if (bufferSize==0){
    return false;
    }
    alBufferData(bufferID, format, &audioData[startOffset], (int)bufferSize, sampleRate);
    bufferIndex++;
    if (bufferIndex>totalChunks && loops){
        bufferIndex=0;
    }
    return true;
    
    
    

}

void SoundBuffer::play(){

    ALenum error = alGetError();
    
    if (error){
        printf("There is an Audio Error during playing");
    
    }
    
    if (isPlaying){
        return;
    }
    
    //self.sourceID =
    alSourcei(source, AL_BUFFER, 0);
    
    bufferIndex = 0;
    
    for (std::vector<ALuint>::iterator IT = buffers.begin();
         IT != buffers.end();
         IT++){
        const ALuint emptyBufferID = (*IT);
        alSourceQueueBuffers(source, 1, &emptyBufferID);
        AudioController::errorTest("Add Buffered Sound");
    }
    
    alSourcei(source, AL_LOOPING, AL_FALSE);
    
    alSourcePlay(source);
    isPlaying=true;
    
    
}

bool SoundBuffer::increaseBuffer(){

    ALint sourceState;
    alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
    
    if (sourceState != AL_PLAYING){
    
        isPlaying = false;
        return false;
    }
    
    ALint buffersProcessed;
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);
    
    if (buffersProcessed ==0){return true;}
    
    ALuint refillBufferID;
    alSourceUnqueueBuffers(source, 1, &refillBufferID);
    
    if (!loadNewChunk(refillBufferID)){
    
        isPlaying = false;
        return false;
    }
    
    alSourceQueueBuffers(source, 1, &refillBufferID);
    
    return true;
}

void AudioController::playBuffer(std::string theName){
    
    
    //const char * theName = findFile(theNameI).c_str();
    
    std::map<std::string, SoundBuffer>::iterator IT =bufferSounds.find(theName);
    
    if (IT==bufferSounds.end()){
        
        
        
    } else {
       
        if (!IT->second.isPlaying) {
            IT->second.play();
        }
    }
}

void AudioController::thinkAllBuffers(){

    changeSize--;
    
    
    for (std::map<std::string, SoundBuffer>::iterator IT = bufferSounds.begin();
         IT != bufferSounds.end(); IT++) {
        
        if (IT->second.isPlaying){
        IT->second.increaseBuffer();
        }
    }
    

    
    
    for (std::map<std::string,FadeOutClass>::iterator IT = fadeOuts.begin();
         IT != fadeOuts.end();
         IT++){
        
        if (IT->second.stop){
            continue;
        }
    
        int play=0;
        if (IT->second.inOrOut){
            if (IT->second.tween==0){
                play=1;
            }
            
            if (IT->second.tween>=1.0){
                IT->second.stop = true;
                continue;
            }
            
        } else {
            if (IT->second.tween>=1.0){
                play=2;
                 IT->second.stop = true;
            }
           
            
        }
        
        IT->second.tween += 1.0/(IT->second.time*60.0);
        
   
        
        double volume = (IT->second.inOrOut)?IT->second.tween:1.0-IT->second.tween;
        
        std::map<std::string, soundItem>::iterator IT2 = loadedSounds.find(IT->second.name);
        
        if (IT2!=loadedSounds.end()){
            if (play==1){
            
                playAudioLoop(IT->second.name);
            
                alSourcef(*IT2->second.sourceRef, AL_GAIN, volume);
                //alSourcei(IT2->second.source[0], AL_LOOPING, AL_TRUE);
                //alSourcePlay(IT2->second.source[0]);
            } else if (play==2){
				alSourceStop(*IT2->second.sourceRef);
            } else {
                ALint playState;
                
                ALfloat theAudio = volumes["Music"];
                
				alGetSourcei(*IT2->second.sourceRef, AL_SOURCE_STATE, &playState);
            
                if (volume>1.0){
					alSourcef(*IT2->second.sourceRef, AL_GAIN, theAudio);
                
                } else if (volume<0.0){
                
					alSourcef(*IT2->second.sourceRef, AL_GAIN, 0);
                
                } else {
					alSourcef(*IT2->second.sourceRef, AL_GAIN, volume*theAudio);
                }
                   
                
                
            }
        }
    }
    
  
    
    
}

void AudioController::loadAudioBuffer(std::string theNameI){

    std::string nameReturn =  findFile(theNameI).c_str();
    const char * theName = nameReturn.c_str();
    
    
    if (!theName){return;}
    
    if (bufferSounds.find(theName)==bufferSounds.end()){
        
        SoundBuffer newBuffer;
        newBuffer.createSource();
    
        int channel,sampleRate,bps,size;
        char * data= loadWAV(theName,channel,sampleRate,bps,size);
        newBuffer.audioData = std::vector<char>(data,data+size);
        free(data);
        
        
        unsigned int bufferid;

        alGenBuffers(1,&bufferid);
        if(channel==1)
        {
            if(bps==8)
            {
                newBuffer.format=AL_FORMAT_MONO8;
            }else{
                newBuffer.format=AL_FORMAT_MONO16;
            }
        }else{
            if(bps==8)
            {
                newBuffer.format=AL_FORMAT_STEREO8;
            }else{
                newBuffer.format=AL_FORMAT_STEREO16;
            }
        }

        newBuffer.sampleRate = sampleRate;
        newBuffer.fileSize = size;
        newBuffer.bufferIndex = 0;
        newBuffer.isPlaying=false;
        
        //PAGE 88
        
        for (int counter = 0; counter<3; counter++){
        
            ALuint bufferID;
            alGenBuffers(1, &bufferID);
            newBuffer.buffers.push_back(bufferID);//.//push_back(bufferID);
            newBuffer.loadNewChunk(bufferID);
            
        
        }
        
        bufferSounds[theNameI]=newBuffer;
        //alBufferData(bufferid,format,data,size,sampleRate);
        //alBufferData(bufferid, format, outData, audioDataSize, freq);
        
        //newBuffer.increaseBuffer();
    }


    
}

void AudioController::loadAudioLoop(std::string theNameI,int numberOfSources, std::string group_i){
    
    std::string nameReturn =  findFile(theNameI).c_str();
    const char * theName = nameReturn.c_str();
    
    
    if (!theName){return;}
    
    if (loadedSounds.find(theName)==loadedSounds.end()){
        
        
        int channel,sampleRate,bps,size;
        char * data= loadWAV(theName,channel,sampleRate,bps,size);
        unsigned int bufferid,format;
        alGenBuffers(1,&bufferid);
        if(channel==1)
        {
            if(bps==8)
            {
                format=AL_FORMAT_MONO8;
            }else{
                format=AL_FORMAT_MONO16;
            }
        }else{
            if(bps==8)
            {
                format=AL_FORMAT_STEREO8;
            }else{
                format=AL_FORMAT_STEREO16;
            }
        }
        alBufferData(bufferid,format,data,size,sampleRate);
        //alBufferData(bufferid, format, outData, audioDataSize, freq);
        
		/*
        ALuint sourceid[10];
        alGenSources(10,sourceid);
        
        std::vector<unsigned int> inData;
        ALfloat sourcePos[]={ 0.0, 0.0, 0.0};
        ALfloat sourceVel[]={ 0.0, 0.0, 0.0};
        ALfloat sourceOri[]={ 0.0, 0.0, 0.0};
        
        for (int counter =0 ;counter<numberOfSources; counter++){
            alSourcei(sourceid[counter],AL_BUFFER,bufferid);
            
            
            alSourcefv (sourceid[counter], AL_POSITION, sourcePos);
            alSourcefv (sourceid[counter], AL_VELOCITY, sourceVel);
            alSourcefv (sourceid[counter], AL_DIRECTION, sourceOri);
            alSourcei  (sourceid[counter], AL_LOOPING, AL_TRUE);
            
            inData.push_back(sourceid[counter]);
            
            
            if (groups.find(group_i)==groups.end()){
                groups[group_i] = std::vector<ALuint>();
            }
            
            groups[group_i].push_back(sourceid[counter]);
            
            
            
        }
		*/
        soundItem insertMe;
        
        insertMe.buffer=bufferid;
		insertMe.sourceRef = 0;
        loadedSounds[theNameI]=insertMe;
        free(data);
    }
    
    
}

bool AudioController::errorTest(std::string errorText){

    
    if (ALenum error = alGetError()){
    
        std::string value = alGetString(error);
        printf("There was an Error at %s the number was %i with %s\n\n",errorText.c_str(), error, value.c_str());
    
		return false;
    }
	return true;
}

std::vector<ALuint> theEffectsSources;
ALuint effectSourceCounter = 0;

std::vector<ALuint> theMusicSources;
ALuint musicSourceCounter = 0;

std::map < ALuint *, std::string > theLookupTable;
#define NUMBER_OF_EFFECT_SOURCES 12
#define NUMBER_OF_MUSIC_SOURCES 4


void AudioController::setVolume(std::string group_i, ALfloat volume){

    volumes[group_i]=volume;
    
    
	if (group_i == "Music"){
		for (std::vector<ALuint>::iterator IT = theMusicSources.begin();
			IT != theMusicSources.end();
			IT++){

			alSourcef((*IT), AL_GAIN, volume);
		
		
		}
	
	}
	else if (group_i == "Effect"){
	
		for (std::vector<ALuint>::iterator IT = theEffectsSources.begin();
			IT != theEffectsSources.end();
			IT++){

			alSourcef((*IT), AL_GAIN, volume);


		}
	
	}


    




    
 
    

}


void AudioController::removeSoundItemfromSource(unsigned int * theSourcePointer){

	std::map < ALuint *, std::string >::iterator IT = theLookupTable.find(theSourcePointer);

	if (IT != theLookupTable.end()){
	
		loadedSounds[IT->second].sourceRef = 0;
		theLookupTable.erase(IT);
	}

}

unsigned int * AudioController::getSourceNotPlayingMusic(){

    GDebug::lastFunction = DEBUG_AUDIO_GET_NOT_PLAY_MUSIC;
    
	if (theMusicSources.size() == 0){

		ALuint sourceid[NUMBER_OF_MUSIC_SOURCES];

		alGenSources(NUMBER_OF_MUSIC_SOURCES, sourceid);


		ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };
		ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };
		ALfloat sourceOri[] = { 0.0, 0.0, 0.0 };

		for (int counter = 0; counter<NUMBER_OF_MUSIC_SOURCES; counter++){

			alSourcefv(sourceid[counter], AL_POSITION, sourcePos);
			alSourcefv(sourceid[counter], AL_VELOCITY, sourceVel);
			alSourcefv(sourceid[counter], AL_DIRECTION, sourceOri);
			alSourcei(sourceid[counter], AL_LOOPING, AL_TRUE);

		}

		theMusicSources = std::vector<ALuint>(sourceid, sourceid + NUMBER_OF_MUSIC_SOURCES);
	}


	for (int counter = 0; counter < NUMBER_OF_MUSIC_SOURCES; counter++){
        
        if (musicSourceCounter >= NUMBER_OF_MUSIC_SOURCES){
            musicSourceCounter = 0;
            continue;
        }
        
		ALint sourceState;
		alGetSourcei(theMusicSources[musicSourceCounter], AL_SOURCE_STATE, &sourceState);

		if (sourceState != AL_PLAYING){
			removeSoundItemfromSource(&theMusicSources[musicSourceCounter]);
			return &theMusicSources[musicSourceCounter];
		}

	
		musicSourceCounter++;
	}
	return 0;


}

unsigned int * AudioController::getSourceNotPlaying(){
    
    GDebug::lastFunction = DEBUG_AUDIO_GET_NOT_PLAY;
    
	if (theEffectsSources.size() == 0){

		ALuint sourceid[NUMBER_OF_EFFECT_SOURCES];

		alGenSources(NUMBER_OF_EFFECT_SOURCES, sourceid);


		ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };
		ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };
		ALfloat sourceOri[] = { 0.0, 0.0, 0.0 };

		for (int counter = 0; counter<NUMBER_OF_EFFECT_SOURCES; counter++){

			alSourcefv(sourceid[counter], AL_POSITION, sourcePos);
			alSourcefv(sourceid[counter], AL_VELOCITY, sourceVel);
			alSourcefv(sourceid[counter], AL_DIRECTION, sourceOri);
			alSourcei(sourceid[counter], AL_LOOPING, AL_TRUE);

		}

		theEffectsSources = std::vector<ALuint>(sourceid, sourceid + NUMBER_OF_EFFECT_SOURCES);
	}


	for (int counter = 0; counter < NUMBER_OF_EFFECT_SOURCES; counter++){
        
        if (effectSourceCounter >= NUMBER_OF_EFFECT_SOURCES){
            effectSourceCounter = 0;
            continue;
        }
        
		ALint sourceState;
		alGetSourcei(theEffectsSources[effectSourceCounter], AL_SOURCE_STATE, &sourceState);

		if (sourceState != AL_PLAYING){
			removeSoundItemfromSource(&theEffectsSources[effectSourceCounter]);
			return &theEffectsSources[effectSourceCounter];
		}

	
		effectSourceCounter++;

	
	}
	return 0;
	


}


void AudioController::stopAudio(std::string theNameI){
    GDebug::lastFunction = DEBUG_AUDIO_STOP;
    GDebug::textSave = theNameI;
    
    
    if (loadedSounds.find(theNameI)!=loadedSounds.end()){
            alSourceStop(*loadedSounds[theNameI].sourceRef);
    }
}

void AudioController::loadAudio(std::string theNameI, int numberOfSources,std::string group_i){
    
    GDebug::lastFunction = DEBUG_AUDIO_LOAD;
    GDebug::textSave = theNameI;
    
    std::string nameReturn =  findFile(theNameI).c_str();
    const char * theName = nameReturn.c_str();

    
    if (strcmp(theName,"null")==0){
        
        std::string error = "The Audio File ";
        error.append(theName);
        error.append( "could not be found");
        
        GDebug::log(error);
        
        return;}
    
    if (loadedSounds.find(theName)==loadedSounds.end()){
        
        
        int channel,sampleRate,bps,size;
        char * data= loadWAV(theName,channel,sampleRate,bps,size);
        unsigned int bufferid,format;
        alGenBuffers(1,&bufferid);
        
        errorTest("At Gen Error");
        
        if(channel==1)
        {
            if(bps==8)
            {
                format=AL_FORMAT_MONO8;
            }else{
                format=AL_FORMAT_MONO16;
            }
        }else{
            if(bps==8)
            {
                format=AL_FORMAT_STEREO8;
            }else{
                format=AL_FORMAT_STEREO16;
            }
        }
        alBufferData(bufferid,format,data,size,sampleRate);
        free(data);
        errorTest("At Buffer the Data");
        
		/*
		unsigned int sourceid[10];
		alGenSources(numberOfSources, sourceid);
        
        std::vector<unsigned int> inData;
        ALfloat sourcePos[]={ 0.0, 0.0, 0.0};
        ALfloat sourceVel[]={ 0.0, 0.0, 0.0};
        ALfloat sourceOri[]={ 0.0, 0.0, 0.0};
        
        for (int counter =0 ;counter<numberOfSources; counter++){
            alSourcei(sourceid[counter],AL_BUFFER,bufferid);
            
            
            alSourcefv (sourceid[counter], AL_POSITION, sourcePos);
            alSourcefv (sourceid[counter], AL_VELOCITY, sourceVel);
            alSourcefv (sourceid[counter], AL_DIRECTION, sourceOri);

            
            inData.push_back(sourceid[counter]);
            
            if (groups.find(group_i)==groups.end()){
                groups[group_i] = std::vector<ALuint>();
            }
            
            groups[group_i].push_back(sourceid[counter]);
            
            
        }
		if (!errorTest("At Creation of the Sources")){
			printf(theNameI.c_str());
		
		}
       
        */
		soundItem insertMe;
		insertMe.sourceRef = 0;
        insertMe.buffer=bufferid;
        loadedSounds[theNameI]=insertMe;
    }
    
    
}

void AudioController::playAudioLoop(std::string theNameI){
    GDebug::lastFunction = DEBUG_AUDIO_LOOP;
    GDebug::textSave = theNameI;
    
    std::map<std::string, soundItem>::iterator IT =loadedSounds.find(theNameI);
    
    if (IT==loadedSounds.end()){
        
        
        
    } else {
		ALuint *source =  getSourceNotPlayingMusic();
		if (source == 0){ return; }
		alSourcei(*source, AL_BUFFER, IT->second.buffer);
		alSourcei(*source, AL_LOOPING, AL_TRUE);
		IT->second.sourceRef = source;
        alSourcePlay(*source);
		theLookupTable[source] = theNameI;
        errorTest("Play Error");
    }

    
}

void AudioController::playAudio(std::string theNameI){
    
    GDebug::lastFunction = DEBUG_AUDIO_PLAY;
    GDebug::textSave = theNameI;
    
    GDebug::textSave = theNameI;
    //const char * theName = findFile(theNameI).c_str();
    
    std::map<std::string, soundItem>::iterator IT =loadedSounds.find(theNameI);
    
    
    if (volumes.find("Effect")==volumes.end()){
        volumes["Effect"]=1.0;
    
    }
    
	if (IT == loadedSounds.end()){
		AudioController::loadAudio(theNameI, 3, "Effect");
		IT = loadedSounds.find(theNameI);
	}

	if (IT != loadedSounds.end()){

		ALuint *source = getSourceNotPlaying();
		if (source == 0){ return; }
		IT->second.sourceRef = source;
	
		ALfloat volume = volumes["Effect"];

		alSourcei(*source, AL_BUFFER, IT->second.buffer);
		alSourcei(*source, AL_LOOPING, AL_FALSE);
		alSourcef(*source, AL_GAIN, volume);
		IT->second.sourceRef = source;
		alSourcePlay(*source);
		theLookupTable[source] = theNameI;
		errorTest("Play Error");


	}

	

	
}

double AudioController::getVolume(std::string group_i){

    
    std::map<std::string, ALfloat>::iterator IT=  volumes.find(group_i);
    
    
    if (IT==volumes.end()){
    
        volumes[group_i]= 1.0;
    } else {
        return IT->second;
    
    }
    
    
    
    return volumes[group_i];

}



int AudioController::purge(){
    alcDestroyContext(context);
    alcCloseDevice(device);
    return 0;
}

bool AudioController::isBigEndian()
{
    int a=1;
    return !((char*)&a)[0];
}

int AudioController::convertToInt(char* buffer,int len)
{
    int a=0;
    if(!isBigEndian()){
        for(int i=0;i<len;i++){
            ((char*)&a)[i]=buffer[i];
        }
    }else{
        for(int i=0;i<len;i++){
            ((char*)&a)[3-i]=buffer[i];
        }
    }
    return a;
}

char* AudioController::loadWAV(const char* fn,int& chan,int& samplerate,int& bps,int& size)
{
    char buffer[4];
    std::ifstream in(fn,std::ios::binary);
    in.read(buffer,4);
    if(strncmp(buffer,"RIFF",4)!=0)
    {
        std::cout << "this is not a valid WAVE file"  << std::endl;
        return NULL;
    }
    in.read(buffer,4);
    in.read(buffer,4);      //WAVE
    in.read(buffer,4);      //fmt
    in.read(buffer,4);      //16
    in.read(buffer,2);      //1
    in.read(buffer,2);
    chan=convertToInt(buffer,2);
    in.read(buffer,4);
    samplerate=convertToInt(buffer,4);
    in.read(buffer,4);
    in.read(buffer,2);
    in.read(buffer,2);
    bps=convertToInt(buffer,2);
    in.read(buffer,4);      //data
    in.read(buffer,4);
    size=convertToInt(buffer,4);
    char* data=(char*)malloc(size);//new char[size];
    in.read(data,size);
    return data;
}

void AudioController::testAllAudio(){
    
    
    for (std::map<int, AudioElement>::iterator IT = audioLink.begin();
         IT != audioLink.end();
         IT++){
    
        for (std::map<std::string, std::set<std::string> >::iterator IT2 = IT->second.elements.begin();
             IT2 != IT->second.elements.end();
             IT2++){
            
            for (std::set<std::string>::iterator IT3 = IT2->second.begin();
                 IT3 != IT2->second.end();
                 IT3++){
           
                if (findFile(*IT3)!="null"){
                
                }
                
                
            }
        
           
        
        }
        
    
    }
         
         
         //audioLink



}

std::map<std::string,FadeOutClass> AudioController::fadeOuts;

void AudioController::setFadeOut(std::string nameI, double time, bool inOurOut){

    std::map<std::string,FadeOutClass>::iterator IT = fadeOuts.find(nameI);
    
    if (IT==fadeOuts.end()){
    
        FadeOutClass insert;
        
        insert.inOrOut= !inOurOut;
        fadeOuts[nameI]=insert;
        IT = fadeOuts.find(nameI);

    }
    
    if (IT->second.inOrOut!=inOurOut){
        
        IT->second.name = nameI;
        IT->second.time = time;
        IT->second.tween = 0.0;
        IT->second.inOrOut = inOurOut;
        IT->second.stop = false;
        
    }
}


Music_Enum AudioController::currentSong = NOMUSIC;




void AudioController::gameOver(){

    std::string oldSong;
    
    switch (currentSong) {
            
        case NOMUSIC:
            break;
        case NORMAL:
            oldSong = "Music1.wav";
            break;
        case TERROR1:
            oldSong = "Terror1.wav";
            break;
        case TERROR2:
            oldSong = "Terror2.wav";
            break;
        case TERROR3:
            oldSong = "Terror3.wav";
            break;
        case LOSE:
            oldSong = "Lose.wav";
            break;
        default:
            break;
            
    }
    
    stopAudio(oldSong);
    playAudio("Lose.wav");
    


}

void  AudioController::playMusicForced(Music_Enum newMusic){
	changeSize = 0;
	playMusic(newMusic);
}


void AudioController::playMusic(Music_Enum newMusic){
    
    if (changeSize>0){
        return;
    
    }
    
    
    
    std::string oldSong;
    std::string newSong;
    
    

    switch (newMusic) {
            
        case NOMUSIC:
            break;
        case NORMAL:
            newSong = "Music1.wav";
            break;
        case TERROR1:
            newSong = "Terror1.wav";
            break;
        case TERROR2:
            newSong = "Terror2.wav";
            break;
        case TERROR3:
            newSong = "Terror3.wav";
            break;
        case LOSE:
            newSong = "Lose.wav";
            break;
        default:
            break;
    }
    
    switch (currentSong) {
        
    case NOMUSIC:
        break;
    case NORMAL:
        oldSong = "Music1.wav";
        break;
    case TERROR1:
        oldSong = "Terror1.wav";
        break;
    case TERROR2:
        oldSong = "Terror2.wav";
        break;
    case TERROR3:
        oldSong = "Terror3.wav";
        break;
    case LOSE:
        oldSong = "Lose.wav";
        break;
    default:
        break;
        
    }
    
    if (currentSong==NOMUSIC){
        AudioController::loadAudio("Lose.wav",1,"Music");
        AudioController::loadAudio("Music1.wav",1,"Music");
        AudioController::loadAudio("Terror1.wav", 1, "Music");
        AudioController::loadAudio("Terror2.wav", 1, "Music");
        AudioController::loadAudio("Terror3.wav", 1, "Music");
        AudioController::playAudioLoop(newSong);
        currentSong = NORMAL;
        return;
    }
    
    if (currentSong==newMusic){
        return;
    }
    
    
    currentSong = newMusic;
    
    AudioController::setFadeOut(newSong, 2.0, true);
    AudioController::setFadeOut(oldSong, 2.0, false);
    changeSize = 5*60;
    
    
    
}

