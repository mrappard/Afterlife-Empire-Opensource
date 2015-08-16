//
//  NSObject+MoviePlayer.h
//  After Life Empire
//
//  Created by Autobotika on 2015-02-13.
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <AVFoundation/AVFoundation.h>

@interface OpenGLMovieLayer : NSObject{

   
}

+ (BOOL)setUpReader:(NSURL*)theURL;
+(BOOL)grabNextFrame;
+(GLuint)grabCurrentFrame;
+(void)closeVideo;
@end