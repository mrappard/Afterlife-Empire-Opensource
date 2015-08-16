#ifndef _BITMAPFONTCLASS_H
#define _BITMAPFONTCLASS_H

#include <fstream>
#include <string>
#include "openglimports.h"
#include <vector>

#define BFG_RS_NONE  0x0      // Blend flags
#define BFG_RS_ALPHA 0x1
#define BFG_RS_RGB   0x2
#define BFG_RS_RGBA  0x4

#define BFG_MAXSTRING 255     // Maximum string length

#define WIDTH_DATA_OFFSET  20 // Offset to width data with BFF file
#define MAP_DATA_OFFSET   276 // Offset to texture image data with BFF file

// This definition is missing from some GL libs
#ifndef GL_CLAMP_TO_EDGE 
#define GL_CLAMP_TO_EDGE 0x812F 
#endif

typedef struct
 {
  unsigned char ID1,ID2;
  unsigned char BPP;
  int ImageWidth,ImageHeight,CellWidth,CellHeight;
  unsigned char StartPoint;
 }FontFileHeader;

class CBitmapFont
 {
  public:
   static  bool initalized;
     static bool Load(std::string fname);
     
      GLfloat lineWidth;
    
     CBitmapFont(GLfloat Xvalue, GLfloat Yvalue);
   CBitmapFont();
   ~CBitmapFont();
   
   void SetScreen(int x, int y);

     size_t frameIndex;
     GLfloat yPos;
     GLfloat tween;
     
     GLfloat color[4];
     
     double bbL;
     double bbR;
     double bbT;
     double bbB;
     
     void checkXPos(double refValue);
     void checkYPos(double refValue);
     
     bool move();
     
   
     std::vector<GLfloat> keys;
     
     
     int newLineRef;
     void setNewLineChange(int newLine);
     
     
   void SetCursor(GLfloat x, GLfloat y); 


     
                  
              

   int  GetWidth(char *Text);
  
   
     bool hasChanged;
    
     
     GLuint _vertexArray;
     GLuint _vertexBuffer;
     GLuint _vertexSize;
     
     long timerTick;
     
     bool think();
     void update();
     void updateRC();
     void updateR();
     void updateL();
     void renderNoShaderChange();
     void renderColorChange();
     void renderColorChangeUI();
     void render();
     void renderYMove();
     void customRender();
     void updateCNewLine();
     void updateLNewLine();
     void updateLNewLine(int substring);
     
     void setText(std::string theText);
     void setMod(GLfloat _mod);
  private:
     
     GLfloat mod;
     GLfloat raiseAmount;
     
      std::string text;
     long generateHash();
     long hash;
   static int CellX,CellY,YOffset,RowPitch;
   static char Base;
   static char Width[256];
   static GLuint TexID;
   GLfloat CurX,CurY;
   static float RowFactor,ColFactor;
   static int RenderStyle;
   float Rd,Gr,Bl;
   bool InvertYAxis;
    
 };

#endif