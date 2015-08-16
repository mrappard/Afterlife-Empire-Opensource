//
//  AudioController.h
//  After Life Empire
//
//  Created by Autobotika on 2015-01-24.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef __After_Life_Empire__AudioController__
#define __After_Life_Empire__AudioController__

#include <iostream>
#include "OpenALImport.h"
#include <map>
#include <vector>
#include <stack>
#include <set>

struct soundItem{
	unsigned int * sourceRef;
    unsigned int buffer;    
};

class SoundBuffer{

    public:
    ALfloat position[3];
    ALuint  source;
    
    
    std::string path;
    
    std::vector<ALuint> buffers;
    std::vector<char> audioData;
    
    int fileSize;
    ALenum format;
    ALsizei sampleRate;
    size_t bufferIndex;
    bool isPlaying;
    bool interupted;
    bool loops;

    bool increaseBuffer();
    void createSource();
    bool loadNewChunk(ALuint bufferID);
    void play();

};

class AudioElement{

    public:
    std::map<std::string, std::set<std::string> > elements;
    
};

class FadeOutClass {

    public:
    bool inOrOut;
    std::string name;
    double time;
    double tween;
    bool stop;
};

enum Music_Enum {
    NOMUSIC,
    NORMAL,
    TERROR1,
    TERROR2,
    TERROR3,
    LOSE
};


class AudioController{
    
public:
    
    static ALCcontext *context;
    static ALCdevice *device;
    
    static void initAll();
    
    static std::string reverseJson(int index);
    static void jsonAudio(int index, std::string a1, std::string a2);
    
    
    static std::map<int, AudioElement> audioLink;
    static void playAudioLua(int type, std::string soundType);
    
    static double getVolume(std::string group_i);
    static std::map<std::string, std::vector<ALuint> > groups;
    static std::map<std::string, ALfloat> volumes;
    static void setVolume(std::string group_i, ALfloat volume);
    
    static void loadAudioBuffer(std::string theNameI);
    static void playBuffer(std::string theName);
    static void stopAudio(std::string theNameI);
    static void loadAudioLoop(std::string theNameI, int numberOfSources, std::string group);
    static void loadAudio(std::string theNameI, int numberOfSources, std::string group);
    static void playAudio(std::string theNameI);
    static void playAudioLoop(std::string theNameI);
    //static void playAudio(std::string theNameI, float pitch);
    
    static std::map<std::string, soundItem> loadedSounds;
    
    static std::map<std::string, SoundBuffer> bufferSounds;
    
    static void thinkAllBuffers();
    
    
    static std::map<std::string,FadeOutClass> fadeOuts;
    static void setFadeOut(std::string nameI, double time, bool inOurOut);
    
    
    static bool errorTest(std::string errorText);
    
    
    static int init();
    static int purge();
    static bool isBigEndian();
    static int convertToInt(char* buffer,int len);
    static char* loadWAV(const char* fn,int& chan,int& samplerate,int& bps,int& size);
    
    static void clearIndex(int index);
    static void clearAudio();
    static void testAllAudio();
    
    
    static double changeSize;
    static Music_Enum currentSong;
	static void playMusicForced(Music_Enum newMusic);
    static void playMusic(Music_Enum newMusic);
    
    static void gameOver();

	static unsigned int * getSourceNotPlaying();
	static void removeSoundItemfromSource(unsigned int * theSourcePointer);
	static unsigned int * getSourceNotPlayingMusic();
	

    
};

#endif /* defined(__After_Life_Empire__AudioController__) */
