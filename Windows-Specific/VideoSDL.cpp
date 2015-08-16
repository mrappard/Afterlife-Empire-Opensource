// (c) 2005 Rodrigo Hernandez

#include <assert.h>
#include "OpenGLImports.h"
#include "VideoSDL.h"
#include "DebugTools.h"

#include "GameLogic.h"




Video::Video(){

	pFormatCtx = NULL;
	pCodecCtxOrig = NULL;
	pCodecCtx = NULL;
	pCodec = NULL;
	_texture = 0;
};

void Video::init(){


	av_register_all();
}


void Video::openFile(std::string theFile){
	

	


	// Open video file
	if (avformat_open_input(&pFormatCtx, theFile.c_str(), NULL, 0) != 0){
		DebugTools::spawnErrorWindow("Video Could Not Be Opened");
		return; // Couldn't open file
	}

	if (avformat_find_stream_info(pFormatCtx, NULL) < 0){
		DebugTools::spawnErrorWindow("Video File Stream Could Not Be Found");
		return; // Couldn't find stream information
	}

	av_dump_format(pFormatCtx, 0, theFile.c_str(), 0);

	int i;


	// Find the first video stream
	videoStream = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}

	pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;

	if (videoStream == -1){
		DebugTools::spawnErrorWindow("The Is No Video Stream");
		return; // Didn't find a video stream
	}

	// Get a pointer to the codec context for the video stream
	pCodecCtx = pFormatCtx->streams[videoStream]->codec;


	

	// Find the decoder for the video stream
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		DebugTools::spawnErrorWindow("Unsupported codec!");
		return; // Codec not found
	}
	// Copy context
	pCodecCtx = avcodec_alloc_context3(pCodec);
	if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
		DebugTools::spawnErrorWindow("No Context!");
		return; // Error copying codec context
	}
	// Open codec
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
		DebugTools::spawnErrorWindow("Could not open Codec!");
		return; // Could not open codec
	}


	// Allocate video frame


	pFrame = av_frame_alloc();
	
	// Allocate an AVFrame structure


	pFrameRGB = av_frame_alloc();
	

	if (pFrameRGB == NULL){
		DebugTools::spawnErrorWindow("Could not open Alloc Frame!");
	}



		buffer = NULL;
		int numBytes;
		// Determine required buffer size and allocate buffer
		numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
			pCodecCtx->height);
		buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	
	// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
	// of AVPicture
	avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
		pCodecCtx->width, pCodecCtx->height);


	
	

	// initialize SWS context for software scaling
	sws_ctx = sws_getContext(pCodecCtx->width,
		pCodecCtx->height,
		pCodecCtx->pix_fmt,
		pCodecCtx->width,
		pCodecCtx->height,
		PIX_FMT_RGB24,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
		);
	


	

	
}



bool Video::Update(){

	if (!_texture){
		glGenTextures(1, &_texture);
	
	}


	int frameFinished;

	while (av_read_frame(pFormatCtx, &packet) >= 0) {
		// Is this a packet from the video stream?
		if (packet.stream_index == videoStream) {
			// Decode video frame
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

			// Did we get a video frame?
			if (frameFinished) {
				// Convert the image from its native format to RGB
				sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
					pFrame->linesize, 0, pCodecCtx->height,
					pFrameRGB->data, pFrameRGB->linesize);

				

				glBindTexture(GL_TEXTURE_2D, _texture);


				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				// Edit the texture object's image data using the information SDL_Surface gives us
				glTexImage2D(GL_TEXTURE_2D, 0, 3, *pFrame->linesize, pCodecCtx->height, 0,
					GL_RGB, GL_UNSIGNED_BYTE, pFrameRGB->data[0]);

			
				return  true;
				//SaveFrame(pFrameRGB, pCodecCtx->width,
				//pCodecCtx->height, i);
			}
		}

		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
	}



	return false;


}


void Video::close(){


	// Free the RGB image
	av_free(buffer);
	av_free(pFrameRGB);

	// Free the YUV frame
	av_free(pFrame);

	// Close the codecs
	avcodec_close(pCodecCtx);
	avcodec_close(pCodecCtxOrig);

	// Close the video file
	avformat_close_input(&pFormatCtx);


}


unsigned int Video::returnTexture(){
	return _texture;


}
/*
inline void FindBestTextureSize(unsigned int &w, unsigned int &h)

{

	Uint32 temp = 1;

	while (w>temp)

	{

		temp = temp << 1;

	}

	if (w == h)

	{

		w = h = temp;

		return;

	}

	w = temp;

	temp = 1;

	while (h>temp)

	{

		temp = temp << 1;

	}

	h = temp;

}



Video::Video()

{

	ispaused = false;

	stop = false;

	videotex = 0;

	pFormatCtx = NULL;

	//vid_thread=NULL;

	buffer = NULL;

}

Video::~Video()

{

	//SDL_WaitThread(vid_thread,NULL);

	stop = true;

	// Free the RGB image

	if (buffer != NULL) delete[] buffer;

	av_free(pFrameRGB);

	// Free the YUV frame

	av_free(pFrame);

	// Close the codec

	avcodec_close(pCodecCtx);

	// Close the video file

	if (pFormatCtx != NULL)

	{

		// Close the video file

		av_close_input_file(pFormatCtx);

	}

	if (videotex != 0)

	{

		glDeleteTextures(1, &videotex);

	}

}

bool Video::Load(const char* videofile)

{

	GLenum error;

	//filename=videofile;

	// this should probably be moved somewhere else

	av_register_all();

	// Open video file

	if (av_open_input_file(&pFormatCtx, videofile, NULL, 0, NULL) != 0)

	{

		fprintf(stderr, "Couldn't open file %s\n", videofile);

		return false; // Couldn't open file

	}

	// Retrieve stream information

	if (av_find_stream_info(pFormatCtx)<0)

	{

		fprintf(stderr, "Couldn't find stream information in %s\n", videofile);

		return false; // Couldn't find stream information

	}

	// Dump information about file onto standard error

	dump_format(pFormatCtx, 0, videofile, false);



	// Create Texture for rendering the Video

	glGenTextures(1, &videotex);

	fprintf(stderr, "Tex Name %u\n", videotex);

	if ((error = glGetError()) != GL_NO_ERROR)

	{

		fprintf(stderr, "OpenGL Error %d in Load\n", error);

	}

	glBindTexture(GL_TEXTURE_2D, videotex);

	if ((error = glGetError()) != GL_NO_ERROR)

	{

		if (error == GL_INVALID_ENUM)

		{

			fprintf(stderr, "glBindTexture OpenGL Error GL_INVALID_ENUM Tex Name %u\n", videotex);

		}

		else if (error == GL_INVALID_OPERATION)

		{

			fprintf(stderr, "glBindTexture OpenGL Error GL_INVALID_OPERATION Tex Name %u\n", videotex);

		}

	}



	// Find the first video stream

	videoStream = -1;

	for (int i = 0; i<pFormatCtx->nb_streams; i++)

	{

		if (pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO)

		{

			videoStream = i;

			break;

		}

	}

	if (videoStream == -1)

	{

		fprintf(stderr, "No video stream\n");

		return false; // Didn't find a video stream

	}

	// Get a pointer to the codec context for the video stream

	pCodecCtx = pFormatCtx->streams[videoStream]->codec;



	// Find the decoder for the video stream

	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

	if (pCodec == NULL)

		return false; // Codec not found

	// Inform the codec that we can handle truncated bitstreams -- i.e.,

	// bitstreams where frame boundaries can fall in the middle of packets

	if (pCodec->capabilities & CODEC_CAP_TRUNCATED)

		pCodecCtx->flags |= CODEC_FLAG_TRUNCATED;

	// Open codec

	if (avcodec_open(pCodecCtx, pCodec)<0)

		return false; // Could not open codec



	// Allocate video frame

	pFrame = avcodec_alloc_frame();



	// Allocate an AVFrame structure

	pFrameRGB = avcodec_alloc_frame();

	if (pFrameRGB == NULL)

		return false;



	// Determine required buffer size and allocate buffer

	numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,

		pCodecCtx->height);

	buffer = new uint8_t[numBytes];



	// Assign appropriate parts of buffer to image planes in pFrameRGB

	avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,

		pCodecCtx->width, pCodecCtx->height);

	time_between_frames = int(av_q2d(pCodecCtx->time_base) * 1000);

	fprintf(stderr, "%d/%d TBF %u\n",

		pCodecCtx->time_base.num,

		pCodecCtx->time_base.den, time_between_frames);

	// Allocate OpenGL Texture

	width = pCodecCtx->width;

	height = pCodecCtx->height;

	FindBestTextureSize(width, height);

	fprintf(stderr, "Texture size %dx%d\n", width, height);



	//unsigned char *data=new unsigned char[width*height*3];

	glTexImage2D(GL_TEXTURE_2D,

		0,

		GL_RGB,

		width,

		height,

		0,

		GL_RGB,

		GL_UNSIGNED_BYTE,

		NULL);

	if ((error = glGetError()) != GL_NO_ERROR)

	{

		fprintf(stderr, "glTexImage2D OpenGL Error %d\n", error);

	}

	//delete [] data;



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if ((error = glGetError()) != GL_NO_ERROR)

	{

		fprintf(stderr, "OpenGL Error\n");

	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if ((error = glGetError()) != GL_NO_ERROR)

	{

		fprintf(stderr, "OpenGL Error\n");

	}

	return true;

}



void Video::Play()

{

	ispaused = false;

	stop = false;

}

void Video::Stop()

{

	stop = true;

}



void Video::Pause()

{

	if (ispaused)ispaused = false;

	else ispaused = true;

}





void Video::Update()

{

	GLenum error;

	int frameFinished;

	if (av_read_frame(pFormatCtx, &packet) >= 0)

	{

		// Is this a packet from the video stream?

		if (packet.stream_index == videoStream)

		{

			// Decode video frame

			avcodec_decode_video(pCodecCtx,

				pFrame, &frameFinished,

				packet.data, packet.size);

			// Did we get a video frame?

			if (frameFinished)

			{

				// Convert the image from its native format to RGB

				img_convert((AVPicture *)pFrameRGB,

					PIX_FMT_RGB24,

					(AVPicture*)pFrame,

					pCodecCtx->pix_fmt,

					pCodecCtx->width,

					pCodecCtx->height);

				// --- Add image to texture code here --- //	      

				glBindTexture(GL_TEXTURE_2D, videotex);

				glTexSubImage2D(GL_TEXTURE_2D,

					0,

					0,

					0,

					pCodecCtx->width,

					pCodecCtx->height,

					GL_RGB,

					GL_UNSIGNED_BYTE,

					pFrameRGB->data[0]);

				if ((error = glGetError()) != GL_NO_ERROR)

				{

					if (error == GL_INVALID_OPERATION)

					{

						fprintf(stderr, "glTexSubImage Error GL_INVALID_OPERATION\n");

					}

					else

					{

						fprintf(stderr, "glTexSubImage Error %d\n", error);

					}

				}

				SDL_Delay(time_between_frames);

			}

		}

		// Free the packet that was allocated by av_read_frame

		av_free_packet(&packet);

	}

}



int Video::audio_thread(void *arg)

{

	Video *v = (Video *)arg;

	return 0;

}

*/