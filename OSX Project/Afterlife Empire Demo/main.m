//
//  main.m
//  Afterlife Empire Demo
//
//  Created by Matthew Rappard on 2015-08-16.
//  Copyright (c) 2015 AfterlifeEmpireinc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Steam_API_Access.h"
#import "SetupVars.h"
int main(int argc, const char * argv[]) {
    

    
    
     if (
     steamInit()){
     return 1;
     
     }
    
    return NSApplicationMain(argc, argv);
    
    
}
