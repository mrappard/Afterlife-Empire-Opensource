//
//  GameView.h
//  ThanatosIpad
//
//  Created by Autobotika on 2014-04-22.
//  Copyright (c) 2014 Autobotika. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface Gameview : NSOpenGLView
{
    NSTimer * animationTimer;
    long lastTicks;
    
}

-(void)drawRect:(NSRect)dirtyRect;
+(void)resetCounter;
+(void)updateI;

+(BOOL)updateCount;
+(BOOL)tickCount;



@end
