//
//  AppDelegate.m
//  LuxGame
//
//  Created by Autobotika on 2014-04-21.
//  Copyright (c) 2014 Autobotika. All rights reserved.
//

#import "AppDelegate.h"
//#import "ShaderProgramManager.h"
#import "GameLogic.h"
//#import "Camera.h"
#import <GLKit/GLKit.h>



@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end


@implementation AppDelegate





-(void)quitGame:(id)sender{

  [NSApp terminate: nil];

}

-(void)startGame{

    GameLogic::start();


}


NSTimer * timer;



- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    
    //SetUpGL
    
    srand((unsigned int)time(NULL));
    
    [self performSelectorInBackground:@selector(startGame) withObject:nil];


    
    [NSTimer scheduledTimerWithTimeInterval:1.0/60.0 target:self selector:@selector(tick:) userInfo:nil repeats:YES];

    

    /*
    [NSTimer scheduledTimerWithTimeInterval:1.0/60.0 target:self selector:@selector(update:) userInfo:nil repeats:YES];
    */
    
   
    
    NSWindow * theWindow = _window;
    
    
    NSButton * theButton = [theWindow standardWindowButton:NSWindowCloseButton];
    
    [theButton setTarget:self];
    [theButton setAction:@selector(quitGame:)];

    
    
    
  
   
    
   
   

    
    
    
    
    
    // Insert code here to initialize your application
}

static long value = 0;


-(void) tick:(NSTimer *) timer
{
    
   
    
    value++;
    GameLogic::tick(value);
     GameLogic::update();
    
}

-(void) update:(NSTimer *) timer
{
    
   
    
}


// Creates if necessary and returns the managed object model for the application.

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    
    return NSTerminateNow;
}

@end
