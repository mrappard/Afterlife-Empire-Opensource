#ifndef VIDEO_H

#define VIDEO_H

extern "C"
{


#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

}

#include "OpenGLImports.h"

#include <string>



class Video

{

public:

	Video();

	~Video();



	GLuint _texture;



	void init();
	void openFile(std::string theFile);

	bool Load(const char*);
	unsigned int returnTexture();

	void Play();

	void Stop();

	void Pause();

	bool Update(); // Called every frame or so to advance playing

	void close();

	

private:

	AVFormatContext *pFormatCtx;
	AVCodecContext *pCodecCtxOrig;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	AVFrame * pFrameRGB;
	AVFrame * pFrame;
	uint8_t *buffer;
	AVPacket packet;
	int videoStream;
	struct SwsContext *sws_ctx;

};



#endif
