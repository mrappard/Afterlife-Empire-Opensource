#include "DebugTools.h"
#include "windows.h"
#include "GameLogic.h"

void DebugTools::spawnLuaWindow(std::string errorMessage, std::string code){
	printf("Function Not Enabled");
}
void DebugTools::despawnLuaWindow(){
	printf("Function Not Enabled");
}
void DebugTools::spawnSpriteWindow(){
	printf("Function Not Enabled");
}
void DebugTools::spawnTileWindow(){
	printf("Function Not Enabled");
}
void DebugTools::despawnTileWindow(){
	printf("Function Not Enabled");
}

void DebugTools::spawnErrorWindow(std::string errorMessage){

	MessageBoxA(NULL, errorMessage.c_str(), "Oh Noes Afterlife Empire has a problem", MB_OK);
	
	//GameLogic::quit();

}