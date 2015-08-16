//
//  DebugTools.cpp
//  After Life Empire
//
//  Created by Autobotika on 2014-10-23.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#include "DebugTools.h"
#import <Cocoa/Cocoa.h>



void DebugTools::spawnErrorWindow(std::string errorMessage){
    
    NSString * theErrorMessage = [[NSString alloc] initWithCString:errorMessage.c_str() encoding:[NSString defaultCStringEncoding]];
    
    NSAlert * alert = [[NSAlert alloc] init];
    
    [alert setMessageText:theErrorMessage];

    [alert setAlertStyle:NSCriticalAlertStyle];
    
    if ([alert runModal] == NSAlertFirstButtonReturn){
    
        
     
    
    }

 


}

