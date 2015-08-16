
#import "TextureGenerator.h"
#import <Cocoa/Cocoa.h>
#import "FileManager.h"
#import <map>

static std::map<std::string,GLuint*> textureHolder;
static std::map<std::string, std::pair<int, int>> textureSize;

void removeTexture(GLuint aText){

    for (std::map<std::string,GLuint*>::iterator IT = textureHolder.begin();
         IT != textureHolder.end();
         IT++){
        if (aText==*IT->second){
            glDeleteTextures(1, IT->second);
            textureSize.erase(textureSize.find(IT->first));
            textureHolder.erase(IT);
            delete IT->second;
        }
    }
    


}


void removeTexture(GLuint * aText){
    
    for (std::map<std::string,GLuint*>::iterator IT = textureHolder.begin();
         IT != textureHolder.end();
         IT++){
        if (aText==IT->second){
            glDeleteTextures(1, IT->second);
            textureSize.erase(textureSize.find(IT->first));
            textureHolder.erase(IT);
        }
    }
}


static bool largerFileFasterLoad = false;

GLuint * generateTexture(std::string theString, int * size, bool repeat, GLuint textureRef){
    
    GLuint testValue = -1;
    
    
    
    std::map<std::string,GLuint*>::iterator IT;
    if (textureRef!= testValue){
        if (!textureRef){
            IT = textureHolder.find(theString);
    
            if (IT!=textureHolder.end()){
                
                return IT->second;
        
            }
        }
    } else {
    
        IT = textureHolder.find(theString);
        if (IT!=textureHolder.end()){
            textureRef = *IT->second;
            
        }
    
    }
    
    
    if (largerFileFasterLoad){
    NSString * path = [NSString stringWithCString:theString.c_str() encoding:[NSString defaultCStringEncoding]];
        
    NSString * theApplication = [[NSBundle mainBundle] bundlePath];
    NSURL * firstUrl = [[NSURL alloc] initFileURLWithPath:theApplication];
    NSURL * parentDirectory = [firstUrl URLByDeletingLastPathComponent];
    NSURL * modDirectory = [parentDirectory URLByAppendingPathComponent:@"SaveFiles"];
    NSString * thePath = [modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path;
    thePath = [thePath stringByDeletingPathExtension];
    thePath = [thePath stringByAppendingString:@".raw"];
        

        NSFileManager *fileManager= [NSFileManager defaultManager];
        BOOL aDir = NO;
        if([fileManager fileExistsAtPath:thePath isDirectory:&aDir]){
         
            NSData * theData = [NSData dataWithContentsOfFile:thePath];
            ;
            
            GLuint aSize = [theData length]/4;
            GLuint width = sqrt(aSize);
            GLuint height = width;
            
            
            GLuint texture_id = textureRef;
            
            if (!texture_id){
                glGenTextures(1, &texture_id);
            }
            
            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            /* glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE); */
            
            
            
            
            glTexImage2D(
                         GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         (GLsizei)width,
                         (GLsizei)height,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         [theData bytes]);
            
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            
            if (repeat){
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                
            }else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            
            
            GLuint * value;
            if (textureHolder.find(theString)==textureHolder.end()){
                
                value = new GLuint;
                
            } else {
                
                value = textureHolder[theString];
            }
            
            *value=texture_id;
            
            textureHolder[theString]=value;
            size[0] =width;
            size[1]= height;
            

            
            return value;

            
            
            
        }
        
        
        
        
        
    }
    
    

    //return 1;
    int dimensions[2];
    
    NSString * theName = [NSString stringWithCString:theString.c_str() encoding:NSUTF8StringEncoding];
    /*
     NSString * data1 = [theName stringByDeletingPathExtension];
     NSString * data2 = [theName pathExtension];
     */
    NSURL *theURL = [NSURL fileURLWithPath:theName];
    
    NSData * theData = [NSData dataWithContentsOfURL:theURL];
    
    NSImage * theImage = [[NSImage alloc] initWithData:theData];
    
    
    CGImageRef image= [theImage CGImageForProposedRect:NULL context:NULL hints:NULL];
    
    if (!image){
        
        
        printf("An Image Failed to load");
        
        GLuint texture_id = textureRef;
        
        if (!texture_id){
        glGenTextures(1, &texture_id);
        }
        glBindTexture(GL_TEXTURE_2D, texture_id);
        
        GLsizei width = 32;
        GLsizei height =32;
        
        std::vector<GLubyte> purple;
        
        purple.assign(width*height*4, 0);
        
        for (std::vector<GLubyte>::iterator IT = purple.begin();
             IT != purple.end();
             IT++){
        
        size_t counter = IT - purple.begin();
            
            if (counter%2){
                *IT=255;
            
            }
        
        }
        
        
        
        glTexImage2D(
                     GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     (GLsizei)width,
                     (GLsizei)height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     &purple[0]);
        
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        
        if (repeat){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        }else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        
        GLuint * value = new GLuint;
        
        *value=texture_id;
        
        textureHolder[theString]=value;
        //std::pair<int, int> sizes;
        size[0] = 32;
        size[1]= 32;
        
        
        
        
        //textureSize[theString]=sizes;
        
        
        
        
        
        return value;

        
        
        
        
        
        
        
        
        
        
        
        
        
        return 0;
    
    
    
    
    
    
    
    
    
    }
    
    
    unsigned long width = CGImageGetWidth(image);
    unsigned long height = CGImageGetHeight(image);
    
    dimensions[0]=(int)width;
    dimensions[1]=(int)height;
    
    void *data = malloc(width * height * 4);
    
    
    
    
   
    
    
    
    
    CGColorSpaceRef color_space = CGColorSpaceCreateDeviceRGB();
    if (!color_space){
        free(data);
        return 0;
    }
    //color_space = CGImageGetColorSpace(image);
    
    CGContextRef context = CGBitmapContextCreate(
                                                 data,
                                                 width,
                                                 height,
                                                 8,
                                                 width * 4,
                                                 color_space,
                                                 kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast);
    if (!context){
        free(data);
        return 0;
    }
    
    
    CGContextClearRect(context, NSMakeRect(0, 0, width, height));
    
    CGContextDrawImage(
                       context,
                       CGRectMake(0, 0, width, height),
                       image);
    
    if ([[theName pathExtension] isEqualToString:@"bmp"]){
        
        
        
        
        
    }
    
    
    GLuint texture_id = textureRef;
    
    if (!texture_id){
        glGenTextures(1, &texture_id);
    }
    
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    /* glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE); */
    
    
    
    
    glTexImage2D(
                 GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 (GLsizei)width,
                 (GLsizei)height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    if (repeat){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
    }else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    
    CGContextRelease(context);
    CGColorSpaceRelease(color_space);
    //CGImageRelease(image);
    
    
    
    
    if (largerFileFasterLoad){
        NSData * saveObject = [[NSData alloc]initWithBytes:data length:width*height*4 ];
        
        NSString * path = [NSString stringWithCString:theString.c_str() encoding:[NSString defaultCStringEncoding]];
        
        
        //OUTPUT SCRIPT
    NSString * theApplication = [[NSBundle mainBundle] bundlePath];
    NSURL * firstUrl = [[NSURL alloc] initFileURLWithPath:theApplication];
    NSURL * parentDirectory = [firstUrl URLByDeletingLastPathComponent];
    NSURL * modDirectory = [parentDirectory URLByAppendingPathComponent:@"SaveFiles"];
    NSString * thePath = [modDirectory URLByAppendingPathComponent:[path lastPathComponent]].path;
    thePath = [thePath stringByDeletingPathExtension];
    thePath = [thePath stringByAppendingString:@".raw"];
    
    BOOL didSave = [saveObject writeToFile:thePath   atomically:YES];
    }
    
    
    free(data);
    GLuint * value;
    if (textureHolder.find(theString)==textureHolder.end()){
    
        value = new GLuint;
    
    } else {
    
        value = textureHolder[theString];
    }
    
    *value=texture_id;
    
    textureHolder[theString]=value;
    //std::pair<int, int> sizes;
    size[0] = dimensions[0];
    size[1]= dimensions[1];
    
  /*
    bool powerOfTwo = false;
    if (dimensions[0]==dimensions[1]){
        int testerValue = 4;
        
        for (int counter = 1; counter<10; counter++){
        
            if (testerValue==dimensions[0]){
            
                powerOfTwo =true;
                counter+=99999;
            }
            
            testerValue*=2;
        }
        
    
    }
    
    if (!powerOfTwo){
    
        printf("%s, is not a power of 2\n",theString.c_str());
    
    }
    */
    
    
    
    
    //textureSize[theString]=sizes;
    

    
    
    
    return value;




}


GLuint * replaceTexture(DoorDict * dict){
    
    int data[2];
    
    
    GLuint * returnValue = generateTexture(findFile(dict->textureName), data, false, *dict->texture);
    dict->textureWidth = data[0];
    dict->textureHeight = data[1];
    
    return returnValue;
    
    
    
}
GLuint * replaceTexture(SpriteDict * dict){

    int data[2];
    
    
    GLuint * returnValue = generateTexture(findFile(dict->textureName), data, false , *dict->texture);
    dict->textureWidth = data[0];
    dict->textureHeight = data[1];
    
    return returnValue;
    
}
GLuint * replaceTexture(std::string type){

    
    int data[2];
    return generateTexture(type, data, false , -1);

}


GLuint * generateTextureR(std::string theString){
    
    int data[2];
    return generateTexture(theString, data, true, 0);

    
}


GLuint * generateTexture(std::string theString){
    
    
    int data[2];
    return generateTexture(theString, data, false , 0);
    
}



GLuint * generateTexture(DoorDict * dict){

    int data[2];
    
    
    GLuint * returnValue = generateTexture(findFile(dict->textureName), data, false, 0);
    dict->textureWidth = data[0];
    dict->textureHeight = data[1];
    
    return returnValue;
    

}



GLuint * generateTexture(SpriteDict * dict){
    
    int data[2];
    
    
    GLuint * returnValue = generateTexture(findFile(dict->textureName), data, false , 0);
    dict->textureWidth = data[0];
    dict->textureHeight = data[1];
    
    return returnValue;
}



GLuint dontStore(std::string theString, int * value){
    
    //return 1;
    int dimensions[2];
    
    NSString * theName = [NSString stringWithCString:theString.c_str() encoding:NSUTF8StringEncoding];
    /*
     NSString * data1 = [theName stringByDeletingPathExtension];
     NSString * data2 = [theName pathExtension];
     */
    NSURL *theURL = [NSURL fileURLWithPath:theName];
    
    NSData * theData = [NSData dataWithContentsOfURL:theURL];
    
    NSImage * theImage = [[NSImage alloc] initWithData:theData];
    
    
    CGImageRef image= [theImage CGImageForProposedRect:NULL context:NULL hints:NULL];
    
    if (!image){return 0;}
    
    
    unsigned long width = CGImageGetWidth(image);
    unsigned long height = CGImageGetHeight(image);
    
    dimensions[0]=(int)width;
    dimensions[1]=(int)height;
    
    value[0]=dimensions[0];
    value[1]=dimensions[1];
    
    void *data = malloc(width * height * 4);
    
    CGColorSpaceRef color_space = CGColorSpaceCreateDeviceRGB();
    if (!color_space){
        free(data);
        return 0;
    }
    //color_space = CGImageGetColorSpace(image);
    
    CGContextRef context = CGBitmapContextCreate(
                                                 data,
                                                 width,
                                                 height,
                                                 8,
                                                 width * 4,
                                                 color_space,
                                                 kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast);
    if (!context){
        free(data);
        return 0;
    }
    
    
    CGContextClearRect(context, NSMakeRect(0, 0, width, height));
    
    CGContextDrawImage(
                       context,
                       CGRectMake(0, 0, width, height),
                       image);
    
    if ([[theName pathExtension] isEqualToString:@"bmp"]){
        
        
        
        
        
    }
    
    
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    /* glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE); */
    
    
    
    
    glTexImage2D(
                 GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 (GLsizei)width,
                 (GLsizei)height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    CGContextRelease(context);
    CGColorSpaceRelease(color_space);
    //CGImageRelease(image);
    
    free(data);
    
    return texture_id;
    
    
    
    
}


void dumpAllTextures(){


    
    
    for (std::map<std::string,GLuint*>::iterator IT = textureHolder.begin();
        IT != textureHolder.end();
        IT++){
        
        printf("%s\n",IT->first.c_str());
    }

    


}























