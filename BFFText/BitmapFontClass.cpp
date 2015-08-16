#include "BitmapFontClass.h"
#include <vector>
#include "GlobalDebugger.h"
#include "FileManager.h"
#include "ShaderManager.h"
#include "GameClock.h"
#include "float.h"
#include "VecAndBB.h"

using namespace std;

int CBitmapFont::CellX;
int CBitmapFont::CellY;
int CBitmapFont::YOffset;
int CBitmapFont::RowPitch;
char CBitmapFont::Base;
char CBitmapFont::Width[256];
GLuint CBitmapFont::TexID;
float CBitmapFont::RowFactor,CBitmapFont::ColFactor;
int CBitmapFont::RenderStyle;



CBitmapFont::CBitmapFont()
 {
     raiseAmount=0;
     hash=-1;
  CurX=CurY=0;
  Rd=Gr=Bl=1.0f;
  InvertYAxis=false;
     initalized=false;
     _vertexArray = 0;
     _vertexBuffer = 0;
	 _vertexSize = 0;
     hasChanged=true;
     lineWidth =80;
     timerTick= 0;
     yPos=0;
     frameIndex=0;
     color[0] =1.0;
     color[1] =1.0;
     color[2] =1.0;
     color[3] =1.0;
 }

CBitmapFont::CBitmapFont(GLfloat Xvalue, GLfloat Yvalue){
    
    raiseAmount=0;
    
    CurX = Xvalue;
    CurY = Yvalue;
    
    Rd=Gr=Bl=1.0f;
    InvertYAxis=false;
    initalized=false;
    _vertexArray = 0;
    _vertexBuffer = 0;
    
    
    color[0] =1.0;
    color[1] =1.0;
    color[2] =1.0;
    color[3] =1.0;


    hash=-1;
    
    hasChanged=true;
}


CBitmapFont::~CBitmapFont()
 {
     if (_vertexArray){
         glDeleteVertexArraysCon(1, &_vertexArray);
         
     }
     
     if (_vertexBuffer){
         glDeleteBuffers(1, &_vertexBuffer);
     }


 }


bool CBitmapFont::initalized=false;

bool CBitmapFont::Load(std::string fname)
 {
     if (initalized){
         return true;
     }
     
  char *dat,*img;
  fstream in;
  unsigned long fileSize;
  char bpp;
  int ImgX,ImgY;

  in.open(fname, ios_base::binary | ios_base::in);

   if(in.fail())
    return false;

  // Get Filesize
  in.seekg(0,ios_base::end);
  fileSize=in.tellg();
  in.seekg(0,ios_base::beg);

  // allocate space for file data
  dat=new char[fileSize];

   // Read filedata
   if(!dat)
    return false;

  in.read(dat,fileSize);

   if(in.fail())
    {
     delete [] dat;
     in.close();
     return false;
    }

  in.close();

   // Check ID is 'BFF2'
   if((unsigned char)dat[0]!=0xBF || (unsigned char)dat[1]!=0xF2)
    {
     delete [] dat;
     return false;
    }

  // Grab the rest of the header
  memcpy(&ImgX,&dat[2],sizeof(int));
  memcpy(&ImgY,&dat[6],sizeof(int));
  memcpy(&CellX,&dat[10],sizeof(int));
  memcpy(&CellY,&dat[14],sizeof(int));
  bpp=dat[18];
  Base=dat[19];

   // Check filesize
   if(fileSize!=((MAP_DATA_OFFSET)+((ImgX*ImgY)*(bpp/8))))
	   return false;

  // Calculate font params
  RowPitch=ImgX/CellX;
  ColFactor=(float)CellX/(float)ImgX;
  RowFactor=(float)CellY/(float)ImgY;
  YOffset=CellY;

  // Determine blending options based on BPP
   switch(bpp)
    {
     case 8: // Greyscale
      RenderStyle=BFG_RS_ALPHA;
      break;

     case 24: // RGB
      RenderStyle=BFG_RS_RGB;
      break;

     case 32: // RGBA
      RenderStyle=BFG_RS_RGBA;
      break;

     default: // Unsupported BPP
      delete [] dat;
      return false;
      break;
    }

  // Allocate space for image
  img=new char[(ImgX*ImgY)*(bpp/8)];

   if(!img)
    {
     delete [] dat;
     return false;
    }

  // Grab char widths
  memcpy(Width,&dat[WIDTH_DATA_OFFSET],256);

  // Grab image data
  memcpy(img,&dat[MAP_DATA_OFFSET],(ImgX*ImgY)*(bpp/8));

  // Create Texture
  glGenTextures(1,&TexID);
  glBindTexture(GL_TEXTURE_2D,TexID);
  // Fonts should be rendered at native resolution so no need for texture filtering
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  // Stop chararcters from bleeding over edges
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

  // Tex creation params are dependent on BPP
   switch(RenderStyle)
    {
     case BFG_RS_ALPHA:
      glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,ImgX,ImgY,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,img);
      break;

     case BFG_RS_RGB:
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ImgX,ImgY,0,GL_RGB,GL_UNSIGNED_BYTE,img);
      break;

     case BFG_RS_RGBA:
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,ImgX,ImgY,0,GL_RGBA,GL_UNSIGNED_BYTE,img);
      break;
    }

  // Clean up
  delete [] img;
  delete [] dat;
  
     initalized=true;
  return true;
 }

// Set the position for text output, this will be updated as text is printed
void CBitmapFont::SetCursor(GLfloat x, GLfloat y)
 {
     
     if (x!=CurX||y!=CurY){
  CurX=x;
  CurY=y;
         
         hasChanged=true;
     }
 }







// Sets up an Ortho screen based on the supplied values 
void CBitmapFont::SetScreen(int x, int y)
 {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
   if(InvertYAxis)
    glOrtho(0,x,y,0,-1,1);
   else
    glOrtho(0,x,0,y,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
 }



void CBitmapFont::setText(std::string theText){

    
    if (theText!=text){
    text =theText;
    hasChanged=true;
    }
}


void CBitmapFont::setMod(GLfloat _mod){

    if (mod!=_mod){
    mod=_mod;
    hasChanged=true;

    }
}

bool CBitmapFont::think(){


    hasChanged = true;
    raiseAmount += mod/30.0;
    
    if (raiseAmount>mod*2.0){
    
        return true;
    
    }
    
    
    return false;
}



long CBitmapFont::generateHash(){
    
    std::hash<std::string> hash_fn;
    std::size_t str_hash = hash_fn(text);
    return raiseAmount*134 + CurX*CurX*12 + CurY*CurY*CurY + mod*mod*mod*mod*72+str_hash;

}


void CBitmapFont::updateRC(){
    
    if (!initalized){
        std::string link = findFile("TestFont.bff");
        Load(link );
        
    }
    
    
    if (!hasChanged){
        return;
        
    }
    hasChanged=false;
    
    
    
    
    GLfloat oldCurX = CurX;
    
    size_t sLen;
    size_t Loop;
    
    int Row,Col;
    float U1,V1,U2,V2;
    
    sLen=text.size();
    
    
    
    
    
    
    //GLfloat step = (float)RowPitch/(float)CellX;
    
    GLfloat stepX = mod;//CellX;
    GLfloat stepY = mod;
    
    
    std::vector<GLfloat> vertex;
    vertex.reserve(4*6*text.size());
    for(Loop=0;Loop!=sLen;++Loop)
    {
        Row=(text[Loop]-Base)/RowPitch;
        Col=(text[Loop]-Base)-Row*RowPitch;
        
        
        /*
         GLfloat X1 = CurX;
         GLfloat X2 = CurX+(CellX*mod);
         
         GLfloat Y1 = CurY;
         GLfloat Y2 = CurY+(YOffset*mod);
         */
        
        GLfloat X1 = CurX;
        GLfloat X2 = CurX+stepX;
        
        GLfloat Y1 = CurY+raiseAmount;
        GLfloat Y2 = CurY+stepY+raiseAmount;
        
        
        U1=Col*ColFactor;
        V1=Row*RowFactor;
        U2=U1+(ColFactor*(0.99));
        V2=V1+RowFactor;
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y1);
        
        vertex.push_back(U2);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        //
        
        vertex.push_back(X1);
        vertex.push_back(Y2);
        
        vertex.push_back(U1);
        vertex.push_back(V1);
        
        
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        GLfloat moveAmount = (GLfloat)Width[text[Loop]]/(GLfloat)CellX;
        
        moveAmount*=mod;
        
        
        CurX+=moveAmount;
    }
    
    GLfloat howFar =(CurX-oldCurX);
    
    for (size_t counter = 0; counter<vertex.size(); counter++){
        
        size_t checkValue = counter%4;
        
        if (checkValue==0){
            
            vertex[counter]-=howFar;
            
        }
        if (checkValue==1){
            vertex[counter]-=mod/2.0;
            
        }
        
    }
    
    if (vertex.size()>4){
    
    bbL = vertex[0];
    bbT = vertex[1];
    bbR = vertex[vertex.size() - 4];
	bbB = vertex[vertex.size() - 3];
    
    }
    
    if (!_vertexArray){
        glGenVertexArraysCon(1, &_vertexArray);
    }
    glBindVertexArrayCon(_vertexArray);
    
    if(!_vertexBuffer){
        glGenBuffers(1, &_vertexBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex.size(), &vertex[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    _vertexSize = (GLuint)vertex.size()/4;
    
    CurX=oldCurX;
    
}


void CBitmapFont::updateR(){
    
    if (!initalized){
        std::string link = findFile("TestFont.bff");
        Load(link );
        
    }
    
    
    if (!hasChanged){
        return;
        
    }
    hasChanged=false;
    
    
  
    
    GLfloat oldCurX = CurX;
    
    size_t sLen;
    size_t Loop;
    
    int Row,Col;
    float U1,V1,U2,V2;
    
    sLen=text.size();
    
    
    
    
    //GLfloat step = (float)RowPitch/(float)CellX;
    
    GLfloat stepX = mod;//CellX;
    GLfloat stepY = mod;
    
    
    std::vector<GLfloat> vertex;
    vertex.reserve(4*6*text.size());
    for(Loop=0;Loop!=sLen;++Loop)
    {
        Row=(text[Loop]-Base)/RowPitch;
        Col=(text[Loop]-Base)-Row*RowPitch;
        
        
        /*
         GLfloat X1 = CurX;
         GLfloat X2 = CurX+(CellX*mod);
         
         GLfloat Y1 = CurY;
         GLfloat Y2 = CurY+(YOffset*mod);
         */
        
        GLfloat X1 = CurX;
        GLfloat X2 = CurX+stepX;
        
        GLfloat Y1 = CurY+raiseAmount;
        GLfloat Y2 = CurY+stepY+raiseAmount;
        
        
        U1=Col*ColFactor;
        V1=Row*RowFactor;
        U2=U1+(ColFactor*(0.99));
        V2=V1+RowFactor;
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y1);
        
        vertex.push_back(U2);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        //
        
        vertex.push_back(X1);
        vertex.push_back(Y2);
        
        vertex.push_back(U1);
        vertex.push_back(V1);
        
        
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        GLfloat moveAmount = (GLfloat)Width[text[Loop]]/(GLfloat)CellX;
        
        moveAmount*=mod;
        
        
        CurX+=moveAmount;
    }
    
    GLfloat howFar =(CurX-oldCurX);
    
    for (size_t counter = 0; counter<vertex.size(); counter++){
        
        size_t checkValue = counter%4;
        
        if (checkValue==0){
            
            vertex[counter]-=howFar;
            
        }
        
    }
    
    if (vertex.size()>0){
        
        bbL = vertex[0];
        bbT = vertex[1];
        bbR = vertex[vertex.size()-4];
        bbB = vertex[vertex.size()-3];
        
    }
    
    
    
    if (!_vertexArray){
        glGenVertexArraysCon(1, &_vertexArray);
    }
    glBindVertexArrayCon(_vertexArray);
    
    if(!_vertexBuffer){
        glGenBuffers(1, &_vertexBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex.size(), &vertex[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    _vertexSize = (GLuint)vertex.size()/4;
    
    CurX=oldCurX;
    
}

#define CARRIAGERETURN 10
#define SPACE 32

void CBitmapFont::updateLNewLine(){
    updateLNewLine(0);
}



void CBitmapFont::setNewLineChange(int newLine){

    if (newLineRef!=newLine){
        hasChanged=true;
        newLineRef=newLine;
    }

}


bool CBitmapFont::move(){
    

    return true;
}


void CBitmapFont::updateCNewLine(){

    int substring = 0;
    setNewLineChange(substring);
    
    
    
    if (!initalized){
        std::string link = findFile("TestFont.bff");
        Load(link );
        
    }
    
    
    if (!hasChanged){
        return;
        
    }
    
    
    bbL= 9999999;
    bbR= -9999999;
    bbT=-9999999;
    bbB=9999999;
    
    
    
    
    
    hasChanged=false;
    
    keys.clear();
    
    
    
    GLfloat oldCurX = CurX;
    GLfloat oldCurY = CurY;
    
    size_t sLen;
    size_t Loop;
    
    int Row,Col;
    float U1,V1,U2,V2;
    
    

    
    std::string textS;
    
    if (substring==0){
        
        textS = text;
        
        
    } else {
        
        textS = text;
        
        for (size_t counter = 0; counter<substring; counter++){
            
            size_t value = textS.find_first_of("\n");
            if (value == SIZE_T_MAX){
                break;
            }
            
            textS = textS.substr(value+1);
            
            while (textS[0]=='\n'&&textS.size()!=0) {
                textS = textS.substr(1);
            }
            
            
            
            
        }
        
        
        
        
        
    }
    
    
    
    
    
    sLen=textS.size();
    
    
    
    
    //GLfloat step = (float)RowPitch/(float)CellX;
    
    GLfloat stepX = mod;//CellX;
    GLfloat stepY = mod;
    
    
    std::vector<GLfloat> vertex;
    vertex.reserve(4*6*textS.size());
    
    size_t lastSpace = 0;
    
    
    for(Loop=0;Loop!=sLen;++Loop)
    {
        
        
        
        Row=(textS[Loop]-Base)/RowPitch;
        Col=(textS[Loop]-Base)-Row*RowPitch;
        
        
        GLfloat X1 = CurX;
        GLfloat X2 = CurX+stepX;
        
        GLfloat Y1 = CurY+raiseAmount;
        GLfloat Y2 = CurY+stepY+raiseAmount;
        
        
        U1=Col*ColFactor;
        V1=Row*RowFactor;
        U2=U1+(ColFactor*(0.99));
        V2=V1+RowFactor;
        
        if (X2>oldCurX+lineWidth){
            
            size_t lettersToDestroy = Loop - lastSpace-1;
            
            if (lettersToDestroy>0){
                
                //  vertex.erase(vertex.begin()+(lastSpace*24), vertex.begin()+(lastSpace*24+lettersToDestroy*24));
                
                for (int counter = 0; counter<lettersToDestroy*24; counter++){
                    vertex.pop_back();
                }
                
                
                
                Loop = lastSpace;
                CurX=oldCurX;
                CurY-=stepY;
                
                continue;
                
            }
            
            
            
        }
        
        
        if (textS[Loop] == '\n'){
            
            CurX=oldCurX;
            CurY-=stepY+stepY;
            keys.push_back(oldCurY-CurY);
            continue;
            
        }
        
        if (textS[Loop] == '\t'){
            
            CurX=oldCurX;
            CurY-=stepY;
            keys.push_back(oldCurY-CurY);
            continue;
            
        }
        
        
        if (textS[Loop] ==' '){
            lastSpace = Loop;
        }
        
        
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y1);
        
        vertex.push_back(U2);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        //
        
        vertex.push_back(X1);
        vertex.push_back(Y2);
        
        vertex.push_back(U1);
        vertex.push_back(V1);
        
        
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        bbL = (bbL>X1)?X1:bbL;
        bbR = (bbR<X2)?X2:bbR;
        bbB = (bbB>Y1)?Y1:bbB;
        bbT = (bbT<Y2)?Y2:bbT;
        
        
        
        GLfloat moveAmount = (GLfloat)Width[textS[Loop]]/(GLfloat)CellX;
        
        moveAmount*=mod;
        
        
        CurX+=moveAmount;
    }
    
   
    GLfloat howFar = (bbR-oldCurX)/2.0;
   
    
    for (size_t counter = 0; counter<vertex.size(); counter++){
        
        size_t checkValue = counter%4;
        
        if (checkValue==0){
            
            vertex[counter]-=howFar;
            
        }
        if (checkValue==1){
            vertex[counter]-=mod/2.0;
            
        }
        
    }
    
    bbR-=howFar;
    bbL-=howFar;
    bbT-=mod/2.0;
    bbB-=mod/2.0;
	if (vertex.size() == 0){
		return;
	}
    
    
    if (!_vertexArray){
        glGenVertexArraysCon(1, &_vertexArray);
    }
    glBindVertexArrayCon(_vertexArray);
    
    if(!_vertexBuffer){
        glGenBuffers(1, &_vertexBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex.size(), &vertex[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    _vertexSize = (GLuint)vertex.size()/4;
    
    CurX=oldCurX;
    CurY=oldCurY;


}


void CBitmapFont::updateLNewLine(int substring){

    setNewLineChange(substring);
    


    if (!initalized){
        std::string link = findFile("TestFont.bff");
        Load(link );
        
    }
    
    
    if (!hasChanged){
        return;
        
    }
    
    
 
    
    
    hasChanged=false;
    
    keys.clear();
    
    
    
    GLfloat oldCurX = CurX;
    GLfloat oldCurY = CurY;
    
    size_t sLen;
    size_t Loop;
    
    int Row,Col;
    float U1,V1,U2,V2;
    
    
    
    
    std::string textS;
    
    if (substring==0){
    
        textS = text;
        
    
    } else {
    
        textS = text;
        
        for (size_t counter = 0; counter<substring; counter++){
        
            size_t value = textS.find_first_of("\n");
            if (value == SIZE_T_MAX){
                break;
            }
            
            textS = textS.substr(value+1);
            
            while (textS[0]=='\n'&&textS.size()!=0) {
               textS = textS.substr(1);
            }
            
            
          
        
        }
        
        
        
        
    
    }
    
    
    
    
    
    sLen=textS.size();
    
    
    
    
    //GLfloat step = (float)RowPitch/(float)CellX;
    
    GLfloat stepX = mod;//CellX;
    GLfloat stepY = mod;
    
    
    std::vector<GLfloat> vertex;
    vertex.reserve(4*6*textS.size());
    
    size_t lastSpace = 0;
    
    
    for(Loop=0;Loop!=sLen;++Loop)
    {
      
        
        
        Row=(textS[Loop]-Base)/RowPitch;
        Col=(textS[Loop]-Base)-Row*RowPitch;
        
    
        GLfloat X1 = CurX;
        GLfloat X2 = CurX+stepX;
        
        GLfloat Y1 = CurY+raiseAmount;
        GLfloat Y2 = CurY+stepY+raiseAmount;
        
        
        U1=Col*ColFactor;
        V1=Row*RowFactor;
        U2=U1+(ColFactor*(0.99));
        V2=V1+RowFactor;
        
        if (X2>oldCurX+lineWidth){
        
            size_t lettersToDestroy = Loop - lastSpace-1;
            
            if (lettersToDestroy>0){
                
                //  vertex.erase(vertex.begin()+(lastSpace*24), vertex.begin()+(lastSpace*24+lettersToDestroy*24));
                
                for (int counter = 0; counter<lettersToDestroy*24; counter++){
                    vertex.pop_back();
                }
                

            
                Loop = lastSpace;
                CurX=oldCurX;
                CurY-=stepY;
                
                continue;
                
            }
            
        
        
        }
     
        
        if (textS[Loop] == '\n'){
        
            CurX=oldCurX;
            CurY-=stepY+stepY;
            keys.push_back(oldCurY-CurY);
            continue;
            
        }
        
        
        if (textS[Loop] ==' '){
            lastSpace = Loop;
        }
        
        
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y1);
        
        vertex.push_back(U2);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        //
        
        vertex.push_back(X1);
        vertex.push_back(Y2);
        
        vertex.push_back(U1);
        vertex.push_back(V1);
        
        
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        GLfloat moveAmount = (GLfloat)Width[textS[Loop]]/(GLfloat)CellX;
        
        moveAmount*=mod;
        
        
        CurX+=moveAmount;
    }
    
    if (vertex.size()>0){
        
        bbL = vertex[0];
        bbT = vertex[1];
        bbR = vertex[vertex.size()-4];
        bbB = vertex[vertex.size()-3];
        
    }
    
	_vertexSize = (GLuint)vertex.size() / 4;

	if (_vertexSize == 0){
		return;
	
	}
    
    if (!_vertexArray){
        glGenVertexArraysCon(1, &_vertexArray);
    }
    glBindVertexArrayCon(_vertexArray);
    
    if(!_vertexBuffer){
        glGenBuffers(1, &_vertexBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex.size(), &vertex[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    _vertexSize = (GLuint)vertex.size()/4;
    
    CurX=oldCurX;
    CurY=oldCurY;




}

void CBitmapFont::updateL(){

    if (!initalized){
        std::string link = findFile("TestFont.bff");
        Load(link );
        
    }
    
    
    if (!hasChanged){
        return;
        
    }
    hasChanged=false;
    
    
 
    
    GLfloat oldCurX = CurX;
    
    size_t sLen;
    size_t Loop;
    
    int Row,Col;
    float U1,V1,U2,V2;
    
    sLen=text.size();
    
    
    
    
    //GLfloat step = (float)RowPitch/(float)CellX;
    
    GLfloat stepX = mod;//CellX;
    GLfloat stepY = mod;
    
    
    std::vector<GLfloat> vertex;
    vertex.reserve(4*6*text.size());
    for(Loop=0;Loop!=sLen;++Loop)
    {
        Row=(text[Loop]-Base)/RowPitch;
        Col=(text[Loop]-Base)-Row*RowPitch;
        
        
        /*
         GLfloat X1 = CurX;
         GLfloat X2 = CurX+(CellX*mod);
         
         GLfloat Y1 = CurY;
         GLfloat Y2 = CurY+(YOffset*mod);
         */
        
        GLfloat X1 = CurX;
        GLfloat X2 = CurX+stepX;
        
        GLfloat Y1 = CurY+raiseAmount;
        GLfloat Y2 = CurY+stepY+raiseAmount;
        
        
        U1=Col*ColFactor;
        V1=Row*RowFactor;
        U2=U1+(ColFactor*(0.99));
        V2=V1+RowFactor;
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y1);
        
        vertex.push_back(U2);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        //
        
        vertex.push_back(X1);
        vertex.push_back(Y2);
        
        vertex.push_back(U1);
        vertex.push_back(V1);
        
        
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        GLfloat moveAmount = (GLfloat)Width[text[Loop]]/(GLfloat)CellX;
        
        moveAmount*=mod;
        
        
        CurX+=moveAmount;
    }
    

    if (vertex.size()>0){
        
        bbL = vertex[0];
        bbT = vertex[1];
        bbR = vertex[vertex.size()-4];
        bbB = vertex[vertex.size()-3];
        
    }
    
	if (vertex.size() == 0){
        _vertexSize=0;
		return;
	}

    
    if (!_vertexArray){
        glGenVertexArraysCon(1, &_vertexArray);
    }
    glBindVertexArrayCon(_vertexArray);
    
    if(!_vertexBuffer){
        glGenBuffers(1, &_vertexBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex.size(), &vertex[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    _vertexSize = (GLuint)vertex.size()/4;
    
    CurX=oldCurX;
    
    
    


}


void CBitmapFont::update(){
    
    if (!initalized){
            std::string link = findFile("TestFont.bff");
            Load(link );
        
    }


    if (!hasChanged){
        return;
    
    }
    hasChanged=false;
    
    
   
    
    GLfloat oldCurX = CurX;
    
    size_t sLen;
    size_t Loop;
    
    int Row,Col;
    float U1,V1,U2,V2;
    
    sLen=text.size();
    

   
    
    //GLfloat step = (float)RowPitch/(float)CellX;
    
    GLfloat stepX = mod;//CellX;
    GLfloat stepY = mod;
    
    
    std::vector<GLfloat> vertex;
    vertex.reserve(4*6*text.size());
    for(Loop=0;Loop!=sLen;++Loop)
    {
        Row=(text[Loop]-Base)/RowPitch;
        Col=(text[Loop]-Base)-Row*RowPitch;
        
        
        /*
        GLfloat X1 = CurX;
        GLfloat X2 = CurX+(CellX*mod);
        
        GLfloat Y1 = CurY;
        GLfloat Y2 = CurY+(YOffset*mod);
        */
        
        GLfloat X1 = CurX;
        GLfloat X2 = CurX+stepX;
        
        GLfloat Y1 = CurY+raiseAmount;
        GLfloat Y2 = CurY+stepY+raiseAmount;
        
        
        U1=Col*ColFactor;
        V1=Row*RowFactor;
        U2=U1+(ColFactor*(0.99));
        V2=V1+RowFactor;
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y1);
        
        vertex.push_back(U2);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
      //
        
        vertex.push_back(X1);
        vertex.push_back(Y2);
        
        vertex.push_back(U1);
        vertex.push_back(V1);
        
        
        
        vertex.push_back(X1);
        vertex.push_back(Y1);
        
        vertex.push_back(U1);
        vertex.push_back(V2);
        
        vertex.push_back(X2);
        vertex.push_back(Y2);
        
        vertex.push_back(U2);
        vertex.push_back(V1);
        
        
        GLfloat moveAmount = (GLfloat)Width[text[Loop]]/(GLfloat)CellX;

        moveAmount*=mod;
        
    
        CurX+=moveAmount;
    }
    
    GLfloat howFar =(CurX-oldCurX)/2.0;
    
    for (size_t counter = 0; counter<vertex.size(); counter++){
    
        size_t checkValue = counter%4;
        
        if (checkValue==0){
        
            vertex[counter]-=howFar;
        
        }
    
    }
    
	_vertexSize = (GLuint)vertex.size() / 4;

	if (_vertexSize == 0){
        _vertexSize =0;
		return;
	}
    
    if (vertex.size()>0){
        
        bbL = vertex[0];
        bbT = vertex[1];
        bbR = vertex[vertex.size()-4];
        bbB = vertex[vertex.size()-3];
        
    }

    if (!_vertexArray){
        glGenVertexArraysCon(1, &_vertexArray);
    }
    glBindVertexArrayCon(_vertexArray);
    
    if(!_vertexBuffer){
        glGenBuffers(1, &_vertexBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex.size(), &vertex[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4*4, BUFFER_OFFSET(8));
    
    

    CurX=oldCurX;
    
}

void CBitmapFont::renderColorChangeUI(){
    
    ShaderManager::enableColorShiftUI(color);
    
    if (_vertexSize != 0){
        if (_vertexArray!=0){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TexID);
            glBindVertexArrayCon(_vertexArray);
            glDrawArrays(GL_TRIANGLES, 0, _vertexSize);
            
        }
    }
    
    
}


void CBitmapFont::renderColorChange(){

    ShaderManager::enableColorShift(color);
    
    if (_vertexSize != 0){
        if (_vertexArray!=0){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TexID);
            glBindVertexArrayCon(_vertexArray);
            glDrawArrays(GL_TRIANGLES, 0, _vertexSize);
            
        }
    }


}

void CBitmapFont::renderNoShaderChange(){
    
    if (_vertexSize != 0){
        if (_vertexArray!=0){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TexID);
            glBindVertexArrayCon(_vertexArray);
            glDrawArrays(GL_TRIANGLES, 0, _vertexSize);
            
        }
    }
    
}


void CBitmapFont::renderYMove(){
     ShaderManager::enableUIShaderMove(0,-yPos);
    
    if (_vertexSize != 0){
        if (_vertexArray!=0){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TexID);
            glBindVertexArrayCon(_vertexArray);
            glDrawArrays(GL_TRIANGLES, 0, _vertexSize);
            
        }
    }
    
}


void CBitmapFont::render(){
   // ShaderManager::enableUIShaderMove(0,-yPos);
   
	if (_vertexSize != 0){
    if (_vertexArray!=0){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);
        glBindVertexArrayCon(_vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, _vertexSize);
        
    }
	}

}

void CBitmapFont::customRender(){
    
    if (_vertexSize != 0){
        if (_vertexArray!=0){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TexID);
            glBindVertexArrayCon(_vertexArray);
            
            
            timerTick++;
            
            glDrawArrays(GL_TRIANGLES, 0, _vertexSize-18);
            
           
            long aTime = timerTick%60;
            
            ShaderManager::enableTextureAlphaUI(((double)(aTime)/120.0)+0.33);
            glDrawArrays(GL_TRIANGLES, _vertexSize -18, 6);
            
            aTime = (timerTick+15)%60;
            
            ShaderManager::enableTextureAlphaUI(((double)(aTime)/120.0)+0.33);
            glDrawArrays(GL_TRIANGLES, _vertexSize -12,  6);
            
            aTime = (timerTick+15)%60;
            
            ShaderManager::enableTextureAlphaUI(((double)(aTime)/120.0)+0.33);
            glDrawArrays(GL_TRIANGLES, _vertexSize -6,  6);
            
        }
    }
    
}



// Returns the width in pixels of the specified text
int CBitmapFont::GetWidth(char* Text)
 {
  int Loop,sLen,Size;

  // How many chars in string?
  sLen=(int)strnlen(Text,BFG_MAXSTRING);

  // Add up all width values
   for(Loop=0,Size=0;Loop!=sLen;++Loop)
    {
     Size+=Width[Text[Loop]];
    }

  return Size;
 }