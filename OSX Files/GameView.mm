//
//  GameView.m
//  ThanatosIpad
//
//  Created by Autobotika on 2014-04-22.
//  Copyright (c) 2014 Autobotika. All rights reserved.
//

#import "GameView.h"
#include <limits>
//
//  Gameview.m
//  MultiPlaneCreator
//
//  Created by Autobotika on 13-08-11.
//  Copyright (c) 2013 Autobotika. All rights reserved.
//

#import "Gameview.h"
#include "OpenGLImports.h"
#include <GLUT/glut.h>

#include "GameLogic.h"
#include "GlobalDebugger.h"
#include "Tiles.h"

#include "Sprite.h"
#include "SaveObject.h"
#include "Camera.h"

@implementation Gameview


static NSTrackingArea * trackingArea;
static NSRect eyeBox;

-(void)setTrackingArea {

    
   eyeBox = [self bounds];
 //   eyeBox.size.width*2;
 //   eyeBox.size.height*2;
    if (self) {
        trackingArea = [[NSTrackingArea alloc] initWithRect:eyeBox
                                                    options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveInKeyWindow )
                                                      owner:self userInfo:nil];
        NSArray * trackingAreaArray = [self trackingAreas];
        
        
        for (NSTrackingArea * theArea in trackingAreaArray){
            [self removeTrackingArea:theArea];
        
        }
        
        
        [self addTrackingArea:trackingArea];
    }
    
}
-(BOOL)acceptsFirstResponder{
    return true;
}




- (void)keyDown: (NSEvent *) event {
    
    GameLogic::keyInput(KEY_DOWN, [event keyCode]);
      // NSLog(@"%hu", [event keyCode]);
    return;
}

-(void)keyUp:(NSEvent *) event{

    GameLogic::keyInput(KEY_UP, [event keyCode]);
   // NSLog(@"%hu", [event keyCode]);
    return;


}




/*
-(void)prepareOpenGL{
    
    
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
}
*/


////////////

CVDisplayLinkRef displayLink; //display link for managing rendering thread

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(__bridge Gameview*)displayLinkContext getFrameForTime:outputTime];
    return result;
}




- (void)prepareOpenGL
{
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(displayLink,
                                   &MyDisplayLinkCallback,
                                   (__bridge void*)self);
    
    // Set the display link for the current renderer
    CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    // Activate the display link
    CVDisplayLinkStart(displayLink);
    
    startTime = clock();
}



- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    // Add your drawing codes here

    
    
     [self setNeedsDisplay:YES];
    
    return kCVReturnSuccess;
}


///////////////















-(CGPoint)convertToViewSpace:(CGPoint)target{

     NSRect frame = [self bounds];
    //float titleBarHeight = [self titleBarHeight];
    //frame.size.height -=titleBarHeight;
    
    target.x*=2;
    target.y*=2;
    
    //printf("%f,%f\n", frame.size.width, frame.size.height);
    //printf("%f,%f\n", target.x, target.y);
    
    
    if (frame.size.width>frame.size.height){
    
        double valueX = (target.x/frame.size.width)*200.0-100;
        double mod = (frame.size.height/frame.size.width);
        double valueY = ((target.y/frame.size.height)*200-100)*mod;
        
        //double valueY = (target.y/frame.size.width)*100.0-50;
    
        
        //printf("%f,%f - %f,%f - %f\n", valueX, valueY, target.x, target.y, frame.size.height);
        
        return CGPointMake(valueX, valueY);
    
    } else {
    
    
        double valueX = (target.x/frame.size.width)*200.0-100;
        double mod = (frame.size.width/frame.size.height);
        valueX = valueX*mod;
        double valueY = (target.y/frame.size.height)*200.0-100;
        
        //printf("%f,%f - %f,%f - %f \n", valueX, valueY, target.x, target.y, frame.size.height);
        
        return CGPointMake(valueX, valueY);
    
    }
    
};

- (void)mouseEntered:(NSEvent *)theEvent {

   
}



-(void)mouseMoved:(NSEvent *)theEvent{

    CGPoint thePoint = [self convertPoint:[theEvent locationInWindow]
                                 fromView:self];
    
    
    thePoint = [self convertToViewSpace:thePoint];
    
    
    cpVect target = cpv(thePoint.x, thePoint.y);
    
    GameLogic::mouseInput(MOUSE_STATE_HOVER, target, -1);





}


- (float) titleBarHeight
{
    NSRect frame = [self frame];
    
    NSRect contentRect;
    contentRect = [NSWindow contentRectForFrameRect: frame
                                          styleMask: NSTitledWindowMask];
    
    return (frame.size.height - contentRect.size.height);
    
} // titleBarHeight




- (void)scrollWheel:(NSEvent *)theEvent{

    
    Camera::changeZoom(theEvent.deltaY);
    


}

-(void)mouseDown:(NSEvent*)theEvent{
    
    
    CGPoint thePoint = [self convertPoint:[theEvent locationInWindow]
                                 fromView:self];
    
    
    thePoint = [self convertToViewSpace:thePoint];
    
    
    cpVect target = cpv(thePoint.x, thePoint.y);
    
    GameLogic::mouseInput(MOUSE_STATE_TOUCH, target, -1);
    
    
    
    
    
    
}


-(void)mouseDragged:(NSEvent*)theEvent{
    
    CGPoint thePoint = [self convertPoint:[theEvent locationInWindow]
                                 fromView:self];
    //thePoint =[self convertPointFromBacking:thePoint];
    
    thePoint = [self convertToViewSpace:thePoint];
    
    
    cpVect target = cpv(thePoint.x, thePoint.y);
    
    GameLogic::mouseInput(MOUSE_STATE_DRAG, target, -1);
    
}


-(void)mouseUp:(NSEvent *)theEvent{
    
    CGPoint thePoint = [self convertPoint:[theEvent locationInWindow]
                                 fromView:self];
    //thePoint =[self convertPointFromBacking:thePoint];
    
    thePoint = [self convertToViewSpace:thePoint];
    
    
    cpVect target = cpv(thePoint.x, thePoint.y);
    
    GameLogic::mouseInput(MOUSE_STATE_UP, target, -1);
    
}


-(void)otherMouseDown:(NSEvent *)theEvent{
    CGPoint thePoint = [self convertPoint:[theEvent locationInWindow]
                                 fromView:self];
    //thePoint =[self convertPointFromBacking:thePoint];
    
    thePoint = [self convertToViewSpace:thePoint];
    
    
    cpVect target = cpv(thePoint.x, thePoint.y);
    
    GameLogic::mouseInput(MOUSE_STATE_TOUCH, target, MOUSE_C);

}

-(void)otherMouseDragged:(NSEvent*)theEvent{
    CGPoint thePoint = [self convertPoint:[theEvent locationInWindow]
                                 fromView:self];
    //thePoint =[self convertPointFromBacking:thePoint];
    
    thePoint = [self convertToViewSpace:thePoint];
    
    
    cpVect target = cpv(thePoint.x, thePoint.y);
    
    GameLogic::mouseInput(MOUSE_STATE_DRAG, target, MOUSE_C);
}


-(void)otherMouseUp:(NSEvent *)theEvent{
    CGPoint thePoint = [self convertPoint:[theEvent locationInWindow]
                                 fromView:self];
    //thePoint =[self convertPointFromBacking:thePoint];
    
    thePoint = [self convertToViewSpace:thePoint];
    
    
    cpVect target = cpv(thePoint.x, thePoint.y);
    
    GameLogic::mouseInput(MOUSE_STATE_UP, target, MOUSE_C);
}

-(void)rightMouseDown:(NSEvent*)theEvent{
    CGPoint thePoint = [self convertPoint:[theEvent locationInWindow]
                                 fromView:self];
    //thePoint =[self convertPointFromBacking:thePoint];
    
    thePoint = [self convertToViewSpace:thePoint];
    
    
    cpVect target = cpv(thePoint.x, thePoint.y);
    
    GameLogic::mouseInput(MOUSE_STATE_TOUCH, target, -2);
    
}




-(void)rightMouseDragged:(NSEvent*)theEvent{
 
    CGPoint thePoint = [self convertPoint:[theEvent locationInWindow]
                                 fromView:self];
    //thePoint =[self convertPointFromBacking:thePoint];
    
    thePoint = [self convertToViewSpace:thePoint];
    
    
    cpVect target = cpv(thePoint.x, thePoint.y);
    
    GameLogic::mouseInput(MOUSE_STATE_DRAG, target, -2);
    
    
}

-(void)rightMouseUp:(NSEvent*)theEvent{
    
    CGPoint thePoint = [self convertPoint:[theEvent locationInWindow]
                                 fromView:self];
    //thePoint =[self convertPointFromBacking:thePoint];
    
    thePoint = [self convertToViewSpace:thePoint];
    
    
    cpVect target = cpv(thePoint.x, thePoint.y);
    
    GameLogic::mouseInput(MOUSE_STATE_UP, target, -2);
    
    
}

-(void)updateView:(NSTimer*)timer{
    
    [self setNeedsDisplay:YES];
}

double tester = 0;

static long startTime=0;
static long endTime=0;
static double counter=0;
static double counterU=0;
NSDate * theStart =0;
static long uCount=0;
static long tCount=0;

+(void)updateI{
    counterU++;
    

}

+(void)resetCounter{
    startTime=clock();
    endTime=0;
    counter=0;
    counterU =0;
    uCount =0;
    tCount =0;
    theStart = [NSDate date];

}



+(BOOL)updateCount{

    if (!theStart){
        
        theStart = [NSDate date];
    }
    

    NSTimeInterval sec = [[NSDate date] timeIntervalSinceDate:theStart];
    sec = sec*60;
    
    if (sec>uCount){
    
        uCount++;
        return true;
    
    }
    
    
    return false;
    
    
    
}



+(BOOL)tickCount{

    if (!theStart){
        
        theStart = [NSDate date];
    }

    
    NSTimeInterval sec = [[NSDate date] timeIntervalSinceDate:theStart];
    sec = sec*60;
    
    if (sec>tCount){
        
        tCount++;
        return true;
        
    }

    return false;
}



NSDate * aDate;

-(void)frameRate{
    
    if (!aDate){
        aDate = [NSDate dateWithTimeIntervalSinceNow:1];
    }
    
    tCount++;

    if ([aDate compare:[NSDate date]]==NSOrderedAscending){
     aDate = [NSDate dateWithTimeIntervalSinceNow:1];
    //   printf("%li frame rate\n",tCount);
        tCount=0;
        
     
    }
    
    /*
    clock_t currentTime = clock();
    
    if (currentTime-startTime>CLOCKS_PER_SEC){
        printf("%li frame rate\n",tCount);
        tCount=0;
        startTime = currentTime;
    }
 */
}

-(void)draw{


    
    [[self openGLContext] flushBuffer];

}


-(void)drawRect:(NSRect)dirtyRect{
    
   /* if (!animationTimer){
        animationTimer = [NSTimer
                          scheduledTimerWithTimeInterval:1/60.0
                          target:self
                          selector:@selector(updateView:)
                          userInfo:nil
                          repeats:true];
        
        startTime = clock();
    }
    */
    [self frameRate];
    
    
    //   NSRect saidLayer=dirtyRect;
    //long ticks = clock();
    //int delta_t = (int)((ticks - lastTicks)/(CLOCKS_PER_SEC/1000));
    //int fps = delta_t > 0?(int) 1000/delta_t:1000;
    //[[self window] setTitle:[NSString stringWithFormat:@"%d fps",fps]];

    
    //
     
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    GameLogic::render();
    glFlush();
    
   
    
    //lastTicks=ticks;
  
    return;

    
    
    
}


- (void)viewDidMoveToWindow
{
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowResized:) name:NSWindowDidResizeNotification
                                               object:[self window]];
}


- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];

}

- (void)reshape {
    

    NSRect frame = [[self superview] frame];
   
    
   // NSRect frame = [self bounds];
    //float titleBarHeight = [self titleBarHeight];
    //frame.size.height -=titleBarHeight;
    
    NSPoint point;
    point.x=0;
    point.y=0;
    
    frame.size.width*=2;
    frame.size.height*=2;
    
    [self setFrameSize:frame.size];
    [self setFrameOrigin:point];
    
    GameLogic::setWindow(frame.size.width, frame.size.height);

    
    [self setTrackingArea];
    

}

- (void)windowResized:(NSNotification *)notification;
{
  
}





@end

