//
//  GameWindow.m
//  After Life Empire
//
//  Created by Autobotika on 2014-09-29.
//  Copyright (c) 2014 AfterlifeEmpireInc. All rights reserved.
//

#import "GameWindow.h"

@implementation GameWindow

- (void)windowDidResize:(NSNotification *)notification
{
    
    NSLog(@"Window Size");
   /* [super setFrameSize:newSize];
    
    // A change in size has required the view to be invalidated.
    if ([self inLiveResize])
    {
        NSRect rects[4];
        NSInteger count;
        [self getRectsExposedDuringLiveResize:rects count:&count];
        while (count-- > 0)
        {
            [self setNeedsDisplayInRect:rects[count]];
        }
    }
    else
    {
        [self setNeedsDisplay:YES];
    }
    */
}

/*

- (void)keyDown: (NSEvent *) event {
    
    NSLog(@"KeyPressed");
    
    
    if ([event keyCode] == 13){ //For return key
        
    }
    if ([event keyCode] == 9){ //For tab key
        
    }
    
}
*/
/*
static id eventMonitor;



- (void)windowDidLoad{
    
    NSLog(@"Blarg");
    
    NSEvent* (^handler)(NSEvent*) = ^(NSEvent *theEvent) {
        NSWindow *targetWindow = self;


        NSEvent *result = theEvent;
        NSLog(@"event monitor: %@", theEvent);


     return result;
   };
     eventMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSKeyDownMask handler:handler];
}

- (void)windowWillClose:(NSNotification *)notification{
      [NSEvent removeMonitor:eventMonitor];
  }

*/

@end
