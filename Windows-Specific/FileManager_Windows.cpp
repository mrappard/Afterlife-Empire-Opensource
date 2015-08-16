#include "FileManager.h"

#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>


#include "ModManager.h"

#include "DebugTools.h"

#pragma comment(lib, "User32.lib")





BOOL DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL FileExists(LPCSTR szPath)
{
	DWORD dwAttrib = GetFileAttributesA(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


static std::string assetDirectory = "NULL";
static std::string modDirectory = "NULL";

std::string findFile(std::string input){



	if (assetDirectory == "NULL"){
		
	
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			
		

			std::string f = buffer;
			assetDirectory =  f.substr(0, f.find_last_of("\\/"));
		
		
	}


	std::string targetPath = assetDirectory;
	targetPath = targetPath.append("\\Assets\\");
	targetPath = targetPath.append(input);
	LPCSTR pcstr = targetPath.c_str();


	
	if (FileExists(pcstr)){
	

		
		return targetPath;
	
	} else {
	

		std::string errorMessage;

		errorMessage = targetPath + " is missing from the Asset Directory";
		
		


		DebugTools::spawnErrorWindow(errorMessage);
	
		
	}

	return "null";
}







std::string saveDialogBoxMap(){
	printf("Function Not Enabled A");
	return "null";
}
bool loadDialogBoxMap(std::string boxName, bool fileOrDirectory, std::string * text){
	printf("Function Not Enabled B");
	return true;
}
std::string saveDialogBoxMap(std::string*theData){
	printf("Function Not Enabled C");
	return "null";
}


std::vector<std::string> findAllMods(){

	if (modDirectory == "NULL"){


		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);



		std::string f = buffer;
		assetDirectory = f.substr(0, f.find_last_of("\\/"));


	}

	std::string targetPath = assetDirectory;
	std::string dumpPath = targetPath.append("\\Mods\\");
	targetPath = targetPath.append("*");
	
	std::vector<std::string> aPath;
	WIN32_FIND_DATA fi;
	HANDLE h = FindFirstFileExA(
		targetPath.c_str(),
		FindExInfoStandard,
		&fi,
		FindExSearchLimitToDirectories,
		NULL,
		0);
	if (h != INVALID_HANDLE_VALUE) {
		do {
			if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){

				std::string aw(fi.cFileName);

				aPath.push_back(aw);
		}
		} while (FindNextFile(h, &fi));
		FindClose(h);
	}

	aPath.erase(aPath.begin());
	aPath.erase(aPath.begin());

	for (std::vector<std::string>::iterator IT = aPath.begin();
		IT != aPath.end();
		IT++){
		ModManager::addMod(dumpPath.append(*IT));//*IT);
	
	}

	return aPath;

}
bool outputData(std::string fileName, char * data, size_t amount){
	if (assetDirectory == "NULL"){


		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);



		std::string f = buffer;
		assetDirectory = f.substr(0, f.find_last_of("\\/"));


	}

	std::string targetPath = assetDirectory;
	targetPath = targetPath.append("\\Assets\\");
	targetPath = targetPath.append(fileName);
	


	std::ofstream myfile;
	myfile.open(targetPath);
	myfile.write(data, amount);
	myfile.close();


	

	return true;
}
#include <iostream>
#include <fstream>
bool saveChar(std::string fileName, SaveObject * save){
	if (assetDirectory == "NULL"){


		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);



		std::string f = buffer;
		assetDirectory = f.substr(0, f.find_last_of("\\/"));


	}

	std::string targetPath = assetDirectory;
	targetPath = targetPath.append("\\SaveFiles\\");
	targetPath = targetPath.append(fileName);

	char * data = save->output.data();
	size_t size = save->output.size();
	
	

	std::ofstream myfile;
	myfile.open(targetPath, std::ios::out | std::ios::binary);
	myfile.write(data, size);
	myfile.close();

	return true;

}

bool loadJsonf(std::string fileName, std::vector<char> * data){


	std::ifstream is(fileName.c_str(), std::ios::binary);
	is.seekg(0, std::ios::end);
	std::streamsize sizei = is.tellg();
	is.seekg(0, std::ios::beg);

	std::vector<char> test(sizei);
	if (is.read(test.data(), sizei)){
		*data = test;
		return true;
	}





	return true;

}

bool loadLevel(std::string fileName, std::vector<char> * data){
	

	if (assetDirectory == "NULL"){


		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);



		std::string f = buffer;
		assetDirectory = f.substr(0, f.find_last_of("\\/"));


	}


	std::string targetPath = assetDirectory;
	targetPath = targetPath.append("\\SaveFiles\\");
	targetPath = targetPath.append(fileName);
	LPCSTR pcstr = targetPath.c_str();



	if (FileExists(pcstr)){

		std::ifstream is(targetPath, std::ios::binary);
		is.seekg(0, std::ios::end);
		std::streamsize sizei = is.tellg();
		is.seekg(0, std::ios::beg);

		std::vector<char> test(sizei);
		if (is.read(test.data(), sizei)){
			*data = test;
			return true;
		}






		return false;
//		return true;

	}
	else {

		return false;


	}












	return false;
}


std::vector<char> loadIconAfterLife(){
	if (assetDirectory == "NULL"){


		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);



		std::string f = buffer;
		assetDirectory = f.substr(0, f.find_last_of("\\/"));


	}

	
	std::string targetPath = assetDirectory;
	targetPath = targetPath.append("\\Assets\\");
	targetPath = targetPath.append("favicon.ico");

	LPCSTR pcstr = targetPath.c_str();

	if (FileExists(pcstr)){
		std::ifstream is(targetPath, std::ios::binary);
		is.seekg(0, std::ios::end);
		std::streamsize sizei = is.tellg();
		is.seekg(0, std::ios::beg);

		std::vector<char> test(sizei);
		if (is.read(test.data(), sizei)){
			return test;



		}
	}
	else {

	


	}

	return std::vector<char>();

}


bool deleteFile(std::string fileName){
	
	if (assetDirectory == "NULL"){


		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);



		std::string f = buffer;
		assetDirectory = f.substr(0, f.find_last_of("\\/"));


	}


	std::string targetPath = assetDirectory;
	targetPath = targetPath.append("\\SaveFiles\\");
	targetPath = targetPath.append(fileName);
	LPCSTR pcstr = targetPath.c_str();



	if (FileExists(pcstr)){


		return true;

	}
	else {

		return false;


	}

	return true;
}

static bool timeToShutdown = false;

void quitTheGame(){
	timeToShutdown = true;
	return;
}


bool shutDown(){
	return timeToShutdown;


}