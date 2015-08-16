#ifndef FileManager_h
#define FileManager_h

#include <string>

#include "SaveObject.h"

std::string saveDialogBoxMap();
bool loadDialogBoxMap(std::string boxName, bool fileOrDirectory, std::string * text);
std::string saveDialogBoxMap(std::string*theData);

std::string findFile(std::string);
std::vector<std::string> findAllMods();
bool outputData(std::string fileName, char * data, size_t amount);

bool saveChar(std::string fileName, SaveObject * save);

bool loadLevel(std::string fileName,std::vector<char> * data);
bool loadJsonf(std::string fileName, std::vector<char> * data);

bool deleteFile(std::string fileName);



void quitTheGame();
bool shutDown();

void test();
void updateI();


bool checkU();
bool checkT();

std::vector<char> loadIconAfterLife();


#endif