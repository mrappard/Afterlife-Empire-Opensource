#import "OpenGLMovieLayer.h"
#import "OpenGLImports.h"

@implementation OpenGLMovieLayer

static AVAssetReader *assetReader;
static AVAssetReaderOutput *videoOutput ;
static GLuint videoTexture;
static CGSize trackDimensions;

+ (BOOL)setUpReader:(NSURL*)theURL
{
    NSError *localError = nil;
    
    AVAsset *localAsset = [AVAsset assetWithURL:theURL];
    assetReader = [[AVAssetReader alloc] initWithAsset:localAsset error:&localError];
    
    AVAssetTrack *audioTrack = nil, *videoTrack = nil;
    
    NSArray *audioTracks = [localAsset tracksWithMediaType:AVMediaTypeAudio];
    if ([audioTracks count] > 0)
        audioTrack = [audioTracks objectAtIndex:0];
    NSArray *videoTracks = [localAsset tracksWithMediaType:AVMediaTypeVideo];
    if ([videoTracks count] > 0)
        videoTrack = [videoTracks objectAtIndex:0];

   /*
    if (audioTrack)
    {
        // Decompress to Linear PCM with the asset reader
        NSDictionary *decompressionAudioSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                                    [NSNumber numberWithUnsignedInt:kAudioFormatLinearPCM], AVFormatIDKey,
                                                    nil];
        AVAssetReaderOutput *output = [AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:audioTrack outputSettings:decompressionAudioSettings];
        [assetReader addOutput:output];
    }

*/
    if (videoTrack)
    {
        // Decompress to ARGB with the asset reader
        NSDictionary *decompressionVideoSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                                    [NSNumber numberWithUnsignedInt:kCVPixelFormatType_24RGB], (id)kCVPixelBufferPixelFormatTypeKey,
                                                    [NSDictionary dictionary], (id)kCVPixelBufferIOSurfacePropertiesKey,
                                                    nil];
        videoOutput = [AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:videoTrack outputSettings:decompressionVideoSettings];
        [assetReader addOutput:videoOutput];
        [assetReader startReading];
        
        
        CMFormatDescriptionRef formatDescription = NULL;
        NSArray *formatDescriptions = [videoTrack formatDescriptions];
        if ([formatDescriptions count] > 0)
            formatDescription = (__bridge CMFormatDescriptionRef)[formatDescriptions objectAtIndex:0];
        
        // Grab track dimensions from format description
        
        if (formatDescription){
            trackDimensions = CMVideoFormatDescriptionGetPresentationDimensions(formatDescription, false, false);
        }else{
            trackDimensions = [videoTrack naturalSize];
        }
        
        printf("Size Of Video %f,%f \n",trackDimensions.width,trackDimensions.height);

        
        
    }// Addded
    

    
    
    
    
    
        /*
        // Get the format description of the track, to fill in attributes of the video stream that we don't want to change
        CMFormatDescriptionRef formatDescription = NULL;
        NSArray *formatDescriptions = [videoTrack formatDescriptions];
        if ([formatDescriptions count] > 0)
            formatDescription = (__bridge CMFormatDescriptionRef)[formatDescriptions objectAtIndex:0];
        
        // Grab track dimensions from format description
        CGSize trackDimensions = {
            .width = 0.0,
            .height = 0.0,
        };
        if (formatDescription)
            trackDimensions = CMVideoFormatDescriptionGetPresentationDimensions(formatDescription, false, false);
        else
            trackDimensions = [videoTrack naturalSize];
        
        // Grab clean aperture, pixel aspect ratio from format description
        NSDictionary *compressionSettings = nil;
        if (formatDescription)
        {
            NSDictionary *cleanAperture = nil;
            NSDictionary *pixelAspectRatio = nil;
            CFDictionaryRef cleanApertureFromCMFormatDescription = CMFormatDescriptionGetExtension(formatDescription, kCMFormatDescriptionExtension_CleanAperture);
            if (cleanApertureFromCMFormatDescription)
            {
                cleanAperture = [NSDictionary dictionaryWithObjectsAndKeys:
                                 CFDictionaryGetValue(cleanApertureFromCMFormatDescription, kCMFormatDescriptionKey_CleanApertureWidth), AVVideoCleanApertureWidthKey,
                                 CFDictionaryGetValue(cleanApertureFromCMFormatDescription, kCMFormatDescriptionKey_CleanApertureHeight), AVVideoCleanApertureHeightKey,
                                 CFDictionaryGetValue(cleanApertureFromCMFormatDescription, kCMFormatDescriptionKey_CleanApertureHorizontalOffset), AVVideoCleanApertureHorizontalOffsetKey,
                                 CFDictionaryGetValue(cleanApertureFromCMFormatDescription, kCMFormatDescriptionKey_CleanApertureVerticalOffset), AVVideoCleanApertureVerticalOffsetKey,
                                 nil];
            }
            CFDictionaryRef pixelAspectRatioFromCMFormatDescription = CMFormatDescriptionGetExtension(formatDescription, kCMFormatDescriptionExtension_PixelAspectRatio);
            if (pixelAspectRatioFromCMFormatDescription)
            {
                pixelAspectRatio = [NSDictionary dictionaryWithObjectsAndKeys:
                                    CFDictionaryGetValue(pixelAspectRatioFromCMFormatDescription, kCMFormatDescriptionKey_PixelAspectRatioHorizontalSpacing), AVVideoPixelAspectRatioHorizontalSpacingKey,
                                    CFDictionaryGetValue(pixelAspectRatioFromCMFormatDescription, kCMFormatDescriptionKey_PixelAspectRatioVerticalSpacing), AVVideoPixelAspectRatioVerticalSpacingKey,
                                    nil];
            }
            
            if (cleanAperture || pixelAspectRatio)
            {
                NSMutableDictionary *mutableCompressionSettings = [NSMutableDictionary dictionary];
                if (cleanAperture)
                    [mutableCompressionSettings setObject:cleanAperture forKey:AVVideoCleanApertureKey];
                if (pixelAspectRatio)
                    [mutableCompressionSettings setObject:pixelAspectRatio forKey:AVVideoPixelAspectRatioKey];
                compressionSettings = mutableCompressionSettings;
            }
        }
    }
    */
    return TRUE;
}

+(BOOL)grabNextFrame{
    
    //GLuint theError = glGetError();
    CMSampleBufferRef sampleBuffer =[videoOutput copyNextSampleBuffer];
    if (!sampleBuffer){
        
        
        [assetReader cancelReading];
        
        
        return false;

    }
    CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    if (!videoTexture){
        glGenTextures(1, &videoTexture);
      
    }
    glBindTexture(GL_TEXTURE_2D,videoTexture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, trackDimensions.width, trackDimensions.height, 0, GL_RGB, GL_UNSIGNED_BYTE, CVPixelBufferGetBaseAddress(pixelBuffer));
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    CFRelease(sampleBuffer);
    
    //theError = glGetError();
    

    return true;
}
+(GLuint)grabCurrentFrame{
    return videoTexture;

}

+(void)closeVideo{
 [assetReader cancelReading];

}



@end
