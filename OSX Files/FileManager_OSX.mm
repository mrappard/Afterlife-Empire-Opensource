#import "FileManager.h"
#include <iostream>
#include <cstdlib>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include <vector>
#include "GlobalDebugger.h"
#include "ModManager.h"
#include "Steam_API_Access.h"


std::string saveDialogBoxMap(){

    exit(1);
}



bool outputData(std::string fileName, char * data, size_t amount){

    
       NSString * path = [NSString stringWithCString:fileName.c_str() encoding:[NSString defaultCStringEncoding]];
    
    
    NSString * theApplication = [[NSBundle mainBundle] bundlePath];
    NSURL * firstUrl = [[NSURL alloc] initFileURLWithPath:theApplication];
    NSURL * parentDirectory = [firstUrl URLByDeletingLastPathComponent];
    NSURL * modDirectory = [parentDirectory URLByAppendingPathComponent:@"SaveFiles"];
    
    
    
    // NSURL* url =   [[NSURL alloc] fileURLWithPath:theApplication];
  //  NSLog(@"%@",modDirectory);
    
    
    NSFileManager *fileManager= [NSFileManager defaultManager];
    BOOL aDir = YES;
    if(![fileManager fileExistsAtPath:modDirectory.path isDirectory:&aDir]){
        
        if(![fileManager createDirectoryAtPath:modDirectory.path withIntermediateDirectories:YES attributes:nil error:NULL]){
            
            
            GDebug::log("Count Not Make Mod Folder");
        }
        
        
    }
    NSError * anError;
    
    BOOL f = false;
    
    if ([fileManager fileExistsAtPath:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path isDirectory:&f]){
        
        [fileManager removeItemAtPath:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path error:&anError];
    }
    
    NSData * saveObject = [[NSData alloc] initWithBytesNoCopy:data length:amount freeWhenDone:false];
    BOOL didSave = [saveObject writeToFile:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path  atomically:YES];
    
    
    if (!didSave){
        
        std::string file = fileName;
        std::string errorMessage = "Could not Save " + file;
        
        
        GDebug::log(errorMessage);
        return false;
    }

    
    
    
    

    return true;

}



bool loadDialogBoxMap(std::string boxName, bool fileOrDirectory, std::string * text){
 
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    
    [openDlg setTitle:@"The Background"];
    // Enable the selection of files in the dialog.
    [openDlg setCanChooseFiles:YES];
    
    // Enable the selection of directories in the dialog.
    [openDlg setCanChooseDirectories:NO];
    
    [openDlg setAllowsMultipleSelection:NO];
    
    // Display the dialog.  If the OK button was pressed,
    // process the files.
    
    NSInteger theValue = [openDlg runModal];
    
    if (theValue == NSOKButton )
    {
        // Get an array containing the full filenames of all
        // files and directories selected.
        NSArray* files = [openDlg URLs];
        
        NSURL * theGameFile = [files objectAtIndex:0];
       
        *text =[[theGameFile path] cStringUsingEncoding:[NSString defaultCStringEncoding]];
        
        return true;
        
        
    } else if (theValue == NSCancelButton){
        
        
        
        
        return false;
    }
    
    
    
    

        
    

    return false;
    
    
    
    

}

std::string saveDialogBoxMap(std::string*theData){
    exit(1);

}



bool loadLevel(std::string fileName,std::vector<char> * data){


    
    
    NSString * path = [NSString stringWithCString:fileName.c_str() encoding:[NSString defaultCStringEncoding]];
    
    
    NSString * theApplication = [[NSBundle mainBundle] bundlePath];
    NSURL * firstUrl = [[NSURL alloc] initFileURLWithPath:theApplication];
    NSURL * parentDirectory = [firstUrl URLByDeletingLastPathComponent];
    NSURL * modDirectory = [parentDirectory URLByAppendingPathComponent:@"SaveFiles"];
    
    
    NSFileManager *fileManager= [NSFileManager defaultManager];
    BOOL aDir = YES;
    if(![fileManager fileExistsAtPath:modDirectory.path isDirectory:&aDir]){
        
        if(![fileManager createDirectoryAtPath:modDirectory.path withIntermediateDirectories:YES attributes:nil error:NULL]){
            
            
            GDebug::log("Count Not Make Save Folder");
            return false;
        }
    }
    
    //NSError * anError;
    
    BOOL f = false;
    
    if ([fileManager fileExistsAtPath:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path isDirectory:&f]){
        
        
        NSData * theFile = [[NSData alloc] initWithContentsOfFile:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path];
  
        
        char * targetPoint = (char*)[theFile bytes];
        data->resize([theFile length]);

        std::copy(targetPoint, targetPoint+[theFile length], data->begin());
        
        return true;

    } else {
    
        return false;
    
    }
    
    
        
        
    return 0;
    

}


bool saveChar(std::string fileName, SaveObject * save){

    NSString * path = [NSString stringWithCString:fileName.c_str() encoding:[NSString defaultCStringEncoding]];
    
    
    NSString * theApplication = [[NSBundle mainBundle] bundlePath];
    NSURL * firstUrl = [[NSURL alloc] initFileURLWithPath:theApplication];
    NSURL * parentDirectory = [firstUrl URLByDeletingLastPathComponent];
    NSURL * modDirectory = [parentDirectory URLByAppendingPathComponent:@"SaveFiles"];

    
    
    // NSURL* url =   [[NSURL alloc] fileURLWithPath:theApplication];
   // NSLog(@"%@",modDirectory);
    
    
    NSFileManager *fileManager= [NSFileManager defaultManager];
    BOOL aDir = YES;
    if(![fileManager fileExistsAtPath:modDirectory.path isDirectory:&aDir]){
        
        if(![fileManager createDirectoryAtPath:modDirectory.path withIntermediateDirectories:YES attributes:nil error:NULL]){
            
            GDebug::log("Count Not Make Save Folder");
            return false;
            
        }
        
        
    }
    NSError * anError;
    
    BOOL f = false;
    
    if ([fileManager fileExistsAtPath:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path isDirectory:&f]){
        
        [fileManager removeItemAtPath:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path error:&anError];
    }

    NSData * saveObject = [[NSData alloc] initWithBytesNoCopy:&save->output[0] length:save->output.size() freeWhenDone:false];
    BOOL didSave = [saveObject writeToFile:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path  atomically:YES];
    
    {
    NSURL * documentDirectory = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject];
    
    if ([fileManager fileExistsAtPath:[documentDirectory URLByAppendingPathComponent:[path lastPathComponent]].path isDirectory:&f]){
        
        [fileManager removeItemAtPath:[documentDirectory URLByAppendingPathComponent:[path lastPathComponent]].path error:&anError];
    }
    
        
        NSData * saveObject = [[NSData alloc] initWithBytesNoCopy:&save->output[0] length:save->output.size() freeWhenDone:false];

        
        [saveObject writeToFile:[documentDirectory URLByAppendingPathComponent:[path lastPathComponent]].path  atomically:YES];
        
    }
    
    
    
    if (!didSave){
        
        std::string file = fileName;
        std::string errorMessage = "Could not Save " + file;
        
        
        GDebug::log(errorMessage);
        return false;
    }

    return true;
    
    




}



//std::vector<std::string> thePaths;


std::string findFile(std::string fileName){
    
    
     NSString * thePath = [NSString stringWithCString:fileName.c_str() encoding:[NSString defaultCStringEncoding]];
    /*
    if (fileName=="Sprites.json"){
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        
        NSString * aPath = [paths objectAtIndex:0];
        aPath = [aPath stringByAppendingString:@"/Sprites.json"];
        
        
        NSFileManager *fileManager= [NSFileManager defaultManager];
        
        
        BOOL aDir = NO;
        
        if([fileManager fileExistsAtPath:aPath isDirectory:&aDir]){
            return [aPath cStringUsingEncoding:[NSString defaultCStringEncoding] ];
        }
        
        thePath = [[NSBundle mainBundle] pathForResource:[[thePath lastPathComponent] stringByDeletingPathExtension]  ofType:[thePath pathExtension]];
        
        if (thePath){
            
            [fileManager copyItemAtPath:thePath toPath:aPath error:nil];
            
            
        }
        
        
        
        
        
    }
    
    */
    
    
   

    ;
   
    /*
    for (std::vector<std::string>::iterator IT = thePaths.begin();IT!=thePaths.end();IT++){
    
            thePath = [[NSBundle mainBundle] pathForResource:[[thePath lastPathComponent] stringByDeletingPathExtension]  ofType:[thePath pathExtension]];
        
    
    
    
    }
    */
    
    {
        NSString * theApplication = [[NSBundle mainBundle] bundlePath];
        NSURL * firstUrl = [[NSURL alloc] initFileURLWithPath:theApplication];
        NSURL * parentDirectory = [firstUrl URLByDeletingLastPathComponent];
        NSURL * modDirectory = [parentDirectory URLByAppendingPathComponent:@"Mods"];
        
        modDirectory = [modDirectory URLByAppendingPathComponent:[NSString stringWithCString:fileName.c_str() encoding:[NSString defaultCStringEncoding]]];
        
        
        NSFileManager *fileManager= [NSFileManager defaultManager];
        
        
        BOOL aDir = NO;
        
        if([fileManager fileExistsAtPath:modDirectory.path isDirectory:&aDir]){
            return [modDirectory.path cStringUsingEncoding:[NSString defaultCStringEncoding] ];
        }
    }
    
    
    
    
    
    
    thePath = [[NSBundle mainBundle] pathForResource:[[thePath lastPathComponent] stringByDeletingPathExtension]  ofType:[thePath pathExtension]];
    
    if (thePath){
    
        return [thePath cStringUsingEncoding:[NSString defaultCStringEncoding]];
    
    } else {
        
            //NSString * badPath = [NSString stringWithCString:fileName.c_str() encoding:[NSString defaultCStringEncoding]];
    
    NSLog(@"The File %@ is missing" , [NSString stringWithCString:fileName.c_str() encoding:[NSString defaultCStringEncoding]]);

    }
    return "null";

}



bool deleteFile(std::string fileName){
    
    printf("Delete Me");
    
    NSString * path = [NSString stringWithCString:fileName.c_str() encoding:[NSString defaultCStringEncoding]];
    
    
    NSString * theApplication = [[NSBundle mainBundle] bundlePath];
    NSURL * firstUrl = [[NSURL alloc] initFileURLWithPath:theApplication];
    NSURL * parentDirectory = [firstUrl URLByDeletingLastPathComponent];
    NSURL * modDirectory = [parentDirectory URLByAppendingPathComponent:@"SaveFiles"];
    
    
    NSFileManager *fileManager= [NSFileManager defaultManager];
    BOOL aDir = YES;
    if(![fileManager fileExistsAtPath:modDirectory.path isDirectory:&aDir]){
        
        if(![fileManager createDirectoryAtPath:modDirectory.path withIntermediateDirectories:YES attributes:nil error:NULL]){
            
            
            GDebug::log("Count Not Make Save Folder");
            return false;
        }
    }
    
    
    NSError * anError;
    
    BOOL f = false;
    
    if ([fileManager fileExistsAtPath:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path isDirectory:&f]){
        
        [fileManager removeItemAtPath:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path error:&anError];
    }
    
        
    if (anError){
            
        return false;
            
    }
    return true;
}


bool debugSave(std::string fileName, std::string json){


    NSString * jsonCode = [NSString stringWithCString:json.c_str() encoding:[NSString defaultCStringEncoding]];
    
    

    NSString * path = [NSString stringWithCString:fileName.c_str() encoding:[NSString defaultCStringEncoding]];
    
    
    
    
    NSString * theApplication = [[NSBundle mainBundle] bundlePath];
    NSURL * firstUrl = [[NSURL alloc] initFileURLWithPath:theApplication];
    NSURL * parentDirectory = [firstUrl URLByDeletingLastPathComponent];
    NSURL * modDirectory = [parentDirectory URLByAppendingPathComponent:@"Mods"];
    
    
    
    // NSURL* url =   [[NSURL alloc] fileURLWithPath:theApplication];
    // NSLog(@"%@",modDirectory);
    
    
    NSFileManager *fileManager= [NSFileManager defaultManager];
    BOOL aDir = YES;
    if(![fileManager fileExistsAtPath:modDirectory.path isDirectory:&aDir]){
        
        if(![fileManager createDirectoryAtPath:modDirectory.path withIntermediateDirectories:YES attributes:nil error:NULL]){
            
            GDebug::log("Count Not Make Save Folder");
            return false;
            
        }
        
        
    }
    NSError * anError;
    
    BOOL f = false;
    
    if ([fileManager fileExistsAtPath:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path isDirectory:&f]){
        
        [fileManager removeItemAtPath:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path error:&anError];
    }
    
    
    
    
    NSData * saveObject = [jsonCode dataUsingEncoding:[NSString defaultCStringEncoding]];
    BOOL didSave = [saveObject writeToFile:[modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path  atomically:YES];
    
    if (!didSave){
        
        std::string file = fileName;
        std::string errorMessage = "Could not Save " + file;
        
        
        GDebug::log(errorMessage);
        return false;
    }
    
    return true;
    


}


std::vector<std::string> findAllMods(){

NSFileManager *fileManager= [NSFileManager defaultManager];

    NSError * theError;
    
    NSString * theApplication = [[NSBundle mainBundle] bundlePath];
    NSURL * firstUrl = [[NSURL alloc] initFileURLWithPath:theApplication];
    NSURL * parentDirectory = [firstUrl URLByDeletingLastPathComponent];
    NSURL * modDirectory = [parentDirectory URLByAppendingPathComponent:@"Mods"];
    
   // NSArray * theKeys  = [[NSArray alloc] initWithObjects:kCFURLIsDirectoryKey, nil];
    

   NSArray * theContents = [fileManager contentsOfDirectoryAtURL:modDirectory
                                       includingPropertiesForKeys:[NSArray arrayWithObjects:NSURLNameKey,NSURLIsDirectoryKey, nil]
                                                          options:NSDirectoryEnumerationSkipsSubdirectoryDescendants
                                                           error:&theError];
    

    NSLog(@"\n\n\n\n\n\n");

    //NSArray * fileNames =[theContents valueForKey:NSURLIsDirectoryKey];

    NSMutableArray * directories = [[NSMutableArray alloc] init];
    
    for (NSURL * thePaths in theContents){
        NSNumber * isDirectory;
        
        bool success = [thePaths getResourceValue:&isDirectory forKey:NSURLIsDirectoryKey error:nil];
        
        if (success && [isDirectory boolValue]){
            
            
            if (
                [fileManager fileExistsAtPath: [[thePaths path] stringByAppendingString:@"/Thumbnail.png"]]
                ){
                if (
                    [fileManager fileExistsAtPath: [[thePaths path] stringByAppendingString:@"/ModData.json"]]
                    ){
                    std::string aPath = [[thePaths path] UTF8String];
                    ModManager::addMod(aPath);
                
                }
            }
        }
    }
    

    
    NSLog(@"\n\n\n\n\n\n");
    
    std::vector<std::string> info;

    return info;

}


void quitTheGame(){

    steamEnd();
    [NSApp terminate: nil];

}



void testScript(){
    
    /*
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    
    // Enable the selection of files in the dialog.
    [openDlg setCanChooseFiles:NO];
    
    // Enable the selection of directories in the dialog.
    [openDlg setCanChooseDirectories:YES];
    
    [openDlg setAllowsMultipleSelection:NO];
    
    // Display the dialog.  If the OK button was pressed,
    // process the files.
    if ( [openDlg runModalForDirectory:nil file:nil] == NSOKButton )
    {
        // Get an array containing the full filenames of all
        // files and directories selected.
        NSArray* files = [openDlg filenames];
        
              
    }
    
    
    
    
    

    NSFileManager *fileManager= [NSFileManager defaultManager];
    
    NSError * theError;
    
    NSString * theApplication = [[NSBundle mainBundle] bundlePath];
    NSURL * firstUrl = [[NSURL alloc] initFileURLWithPath:theApplication];
    NSURL * parentDirectory = [firstUrl URLByDeletingLastPathComponent];
    NSURL * modDirectory = [parentDirectory URLByAppendingPathComponent:@"Mods"];
    
    // NSArray * theKeys  = [[NSArray alloc] initWithObjects:kCFURLIsDirectoryKey, nil];
    
    
    NSArray * theContents = [fileManager contentsOfDirectoryAtURL:modDirectory
                                       includingPropertiesForKeys:[NSArray arrayWithObjects:NSURLNameKey,NSURLIsDirectoryKey, nil]
                                                          options:NSDirectoryEnumerationSkipsSubdirectoryDescendants
                                                            error:&theError];
    
    
    NSLog(@"\n\n\n\n\n\n");
    
    //NSArray * fileNames =[theContents valueForKey:NSURLIsDirectoryKey];
    
    NSMutableArray * directories = [[NSMutableArray alloc] init];
    
    for (NSURL * thePaths in theContents){
        NSNumber * isDirectory;
        
        bool success = [thePaths getResourceValue:&isDirectory forKey:NSURLIsDirectoryKey error:nil];
        
        if (success && [isDirectory boolValue]){
            
            
            if (
                [fileManager fileExistsAtPath: [[thePaths path] stringByAppendingString:@"/Thumbnail.png"]]
                ){
                if (
                    [fileManager fileExistsAtPath: [[thePaths path] stringByAppendingString:@"/ModData.json"]]
                    ){
                    std::string aPath = [[thePaths path] UTF8String];
                    ModManager::addMod(aPath);
                    
                }
            }
        }
    }
    */
    
    


}


#import "GameView.h"

bool checkU(){

    return [Gameview updateCount];
}
bool checkT(){

    return [Gameview tickCount];

}


void updateI(){
    
    [Gameview updateI];

}

void test(){

    [Gameview resetCounter];

}
