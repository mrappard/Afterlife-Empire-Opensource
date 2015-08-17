
#include "TextureGenerator.h"
#include  "FileManager.h"
#include  <map>
#include "SDL.h"
#include "SDL_image.h"

#include "GameClock.h"

#include "windows.h"
#include <wincodec.h>
#include <wincodecsdk.h>

#include <iostream>

static std::map<std::string, GLuint*> textureHolder;
static std::map<std::string, std::pair<int, int>> textureSize;


/*

IStream* CreateStreamOnResource(LPCTSTR lpName, LPCTSTR lpType)
{
	IStream * ipStream = NULL;

	HRSRC hrsrc = FindResource(NULL, lpName, lpType);
	if (hrsrc == NULL)
		goto Return;

	DWORD dwResourceSize = SizeofResource(NULL, hrsrc);
	HGLOBAL hglbImage = LoadResource(NULL, hrsrc);
	if (hglbImage == NULL)
		goto Return;

	LPVOID pvSourceResourceData = LockResource(hglbImage);
	if (pvSourceResourceData == NULL)
		goto Return;

	HGLOBAL hgblResourceData = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
	if (hgblResourceData == NULL)
		goto Return;

	LPVOID pvResourceData = GlobalLock(hgblResourceData);

	if (pvResourceData == NULL)
		goto FreeData;

	CopyMemory(pvResourceData, pvSourceResourceData, dwResourceSize);

	GlobalUnlock(hgblResourceData);

	if (SUCCEEDED(CreateStreamOnHGlobal(hgblResourceData, TRUE, &ipStream)))
		goto Return;

FreeData:
	GlobalFree(hgblResourceData);

Return:
	return ipStream;
}

IWICBitmapSource* LoadBitmapFromStream(IStream * ipImageStream)
{
	IWICBitmapSource* ipBitmap = NULL;

	IWICBitmapDecoder* ipDecoder = NULL;
	if (FAILED(CoCreateInstance(CLSID_WICPngDecoder, NULL, CLSCTX_INPROC_SERVER, __uuidof(ipDecoder), reinterpret_cast<void**>(&ipDecoder))))
		goto Return;

	if (FAILED(ipDecoder->Initialize(ipImageStream, WICDecodeMetadataCacheOnLoad)))
		goto ReleaseDecoder;

	UINT nFrameCount = 0;
	if (FAILED(ipDecoder->GetFrameCount(&nFrameCount)) || nFrameCount != 1)
		goto ReleaseDecoder;

	IWICBitmapFrameDecode* ipFrame = NULL;
	if (FAILED(ipDecoder->GetFrame(0, &ipFrame)))
		goto ReleaseDecoder;

	WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, ipFrame, &ipBitmap);
	ipFrame->Release();

ReleaseDecoder:
	ipDecoder->Release();
Return:
	return ipBitmap;
}


HBITMAP LoadImageFromResources(LPCTSTR lpName, LPCTSTR lpType)
{
	HBITMAP hbmpSplash = NULL;

	IStream* ipImageStream = CreateStreamOnResource(lpName, lpType);
	if (ipImageStream == NULL)
		goto Return;

	IWICBitmapSource* ipBitmap = LoadBitmapFromStream(ipImageStream);
	if (ipBitmap == NULL)
		goto ReleaseStream;

	hbmpSplash = CreateHBITMAP(ipBitmap);
	ipBitmap->Release();

ReleaseStream:
	ipImageStream->Release();

Return:
	return hbmpSplash;
}



void test(){

	LoadImageFromResources(L"Test", _T("PNG"));
}

*/


/*
IStream * CreateStreamOnResource(LPCTSTR lpName, LPCTSTR lpType)
{
	// initialize return value
	IStream * ipStream = NULL;

	// find the resource
	HRSRC hrsrc = FindResource(NULL, lpName, lpType);
	if (hrsrc == NULL)
		goto Return;

	// load the resource
	DWORD dwResourceSize = SizeofResource(NULL, hrsrc);
	HGLOBAL hglbImage = LoadResource(NULL, hrsrc);
	if (hglbImage == NULL)
		goto Return;

	// lock the resource, getting a pointer to its data
	LPVOID pvSourceResourceData = LockResource(hglbImage);
	if (pvSourceResourceData == NULL)
		goto Return;

	// allocate memory to hold the resource data
	HGLOBAL hgblResourceData = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
	if (hgblResourceData == NULL)
		goto Return;

	// get a pointer to the allocated memory
	LPVOID pvResourceData = GlobalLock(hgblResourceData);
	if (pvResourceData == NULL)
		goto FreeData;

	// copy the data from the resource to the new memory block
	CopyMemory(pvResourceData, pvSourceResourceData, dwResourceSize);
	GlobalUnlock(hgblResourceData);

	// create a stream on the HGLOBAL containing the data
	if (SUCCEEDED(CreateStreamOnHGlobal(hgblResourceData, TRUE, &ipStream)))
		goto Return;

FreeData:
	// couldn't create stream; free the memory
	GlobalFree(hgblResourceData);

Return:
	// no need to unlock or free the resource
	return ipStream;
}

IWICBitmapSource * LoadBitmapFromStream(IStream * ipImageStream)
{
	// initialize return value
	IWICBitmapSource * ipBitmap = NULL;

	// load WIC's PNG decoder
	IWICBitmapDecoder * ipDecoder = NULL;
	if (FAILED(CoCreateInstance(CLSID_WICPngDecoder, NULL, CLSCTX_INPROC_SERVER, __uuidof(ipDecoder), reinterpret_cast<void**>(&ipDecoder))))
		goto Return;

	// load the PNG
	if (FAILED(ipDecoder->Initialize(ipImageStream, WICDecodeMetadataCacheOnLoad)))
		goto ReleaseDecoder;

	// check for the presence of the first frame in the bitmap
	UINT nFrameCount = 0;
	if (FAILED(ipDecoder->GetFrameCount(&nFrameCount)) || nFrameCount != 1)
		goto ReleaseDecoder;

	// load the first frame (i.e., the image)
	IWICBitmapFrameDecode * ipFrame = NULL;
	if (FAILED(ipDecoder->GetFrame(0, &ipFrame)))
		goto ReleaseDecoder;

	// convert the image to 32bpp BGRA format with pre-multiplied alpha
	//   (it may not be stored in that format natively in the PNG resource,
	//   but we need this format to create the DIB to use on-screen)
	WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, ipFrame, &ipBitmap);
	ipFrame->Release();

ReleaseDecoder:
	ipDecoder->Release();
Return:
	return ipBitmap;
}

// Creates a 32-bit DIB from the specified WIC bitmap.
HBITMAP CreateHBITMAP(IWICBitmapSource * ipBitmap)
{
	// initialize return value
	HBITMAP hbmp = NULL;

	// get image attributes and check for valid image
	UINT width = 0;
	UINT height = 0;
	if (FAILED(ipBitmap->GetSize(&width, &height)) || width == 0 || height == 0)
		goto Return;

	// prepare structure giving bitmap information (negative height indicates a top-down DIB)
	BITMAPINFO bminfo;
	ZeroMemory(&bminfo, sizeof(bminfo));
	bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bminfo.bmiHeader.biWidth = width;
	bminfo.bmiHeader.biHeight = -((LONG)height);
	bminfo.bmiHeader.biPlanes = 1;
	bminfo.bmiHeader.biBitCount = 32;
	bminfo.bmiHeader.biCompression = BI_RGB;

	// create a DIB section that can hold the image
	void * pvImageBits = NULL;
	HDC hdcScreen = GetDC(NULL);
	hbmp = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pvImageBits, NULL, 0);
	ReleaseDC(NULL, hdcScreen);
	if (hbmp == NULL)
		goto Return;

	// extract the image into the HBITMAP
	const UINT cbStride = width * 4;
	const UINT cbImage = cbStride * height;
	if (FAILED(ipBitmap->CopyPixels(NULL, cbStride, cbImage, static_cast<BYTE *>(pvImageBits))))
	{
		// couldn't extract image; delete HBITMAP
		DeleteObject(hbmp);
		hbmp = NULL;
	}

Return:
	return hbmp;
}

HBITMAP LoadSplashImage()
{
	HBITMAP hbmpSplash = NULL;

	// load the PNG image data into a stream
	IStream * ipImageStream = CreateStreamOnResource(MAKEINTRESOURCE(IDI_SPLASHIMAGE), _T("PNG"));
	if (ipImageStream == NULL)
		goto Return;

	// load the bitmap with WIC
	IWICBitmapSource * ipBitmap = LoadBitmapFromStream(ipImageStream);
	if (ipBitmap == NULL)
		goto ReleaseStream;

	// create a HBITMAP containing the image
	hbmpSplash = CreateHBITMAP(ipBitmap);
	ipBitmap->Release();

ReleaseStream:
	ipImageStream->Release();
Return:
	return hbmpSplash;
}

*/










GLuint * generateTexture(std::string theString, int * size, bool repeat){


	/*

	std::map<std::string, GLuint*>::iterator IT;

	IT = textureHolder.find(theString);

	if (IT != textureHolder.end()){

		return IT->second;

	}

	wchar_t wtext[300];
	mbstowcs(wtext, theString.c_str(), theString.size() + 1);//Plus null
	LPWSTR ptr = wtext;

	
	IWICBitmapLock* lock = NULL;
	IWICBitmap* bitmap;

	GetImageFromFile(ptr, &bitmap);
	if (!bitmap)
		return FALSE;

	WICRect r = { 0 };
	bitmap->GetSize((UINT*)&r.Width, (UINT*)&r.Height);

	BYTE* data = NULL;
	UINT len = 0;

	bitmap->Lock(&r, WICBitmapLockRead, &lock);
	lock->GetDataPointer(&len, &data);

	
	GLuint * texture; // This is a handle to our texture object

	texture = new GLuint;

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, r.Width, r.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);

	

	lock->Release();
	bitmap->Release();

	textureHolder[theString] = texture;



	return texture;
	*/

	/*

	GLuint * texture; // This is a handle to our texture object

	texture = new GLuint;



	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, theString);

	// If there's an error, display it.
	if (error != 0)
	{
		std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
		return 0;
	}

	// Here the PNG is loaded in "image". All the rest of the code is SDL and OpenGL stuff.

	glGenTextures(1, texture);




	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, *texture);

	// Set the texture's stretching properties

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);





	textureHolder[theString] = texture;



	return texture;

	*/



	

	GLuint * texture; // This is a handle to our texture object

	texture = new GLuint;


	SDL_Surface *surface;	// This surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;

	  


	surface = IMG_Load(theString.c_str());

	

	if (surface) {

		// Check that the image's width is a power of 2
	/*	if ((surface->w & (surface->w - 1)) != 0) {
			printf("warning: image.bmp's width is not a power of 2\n");
		}

		// Also check if the height is a power of 2
		if ((surface->h & (surface->h - 1)) != 0) {
			printf("warning: image.bmp's height is not a power of 2\n");
		}
		*/
		// get the number of channels in the SDL surface
		nOfColors = surface->format->BytesPerPixel;
		if (nOfColors == 4)     // contains an alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
#if defined(_WIN32)
				texture_format = GL_RGBA;
#endif

#if defined(__APPLE__ )

			texture_format = GL_BGRA;
#endif




			else
#if defined(_WIN32)
				texture_format = GL_BGRA;
#endif

#if defined(__APPLE__ )

			texture_format = GL_BGRA;
#endif
		}
		else if (nOfColors == 3)     // no alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_RGB;
		}
		else {
			printf("warning: the image is not truecolor..  this will probably break\n");
			// this error should not go unhandled
			texture_format = GL_RGBA;
		}

		// Have OpenGL generate a texture object handle for us
		glGenTextures(1, texture);




		// Bind the texture object
		glBindTexture(GL_TEXTURE_2D, *texture);

		// Set the texture's stretching properties


		if (repeat){
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		}



		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
			texture_format, GL_UNSIGNED_BYTE, surface->pixels);
	
	}
	else {
		printf("SDL could not load image.bmp: %s\n", SDL_GetError());
		if (surface) {
			SDL_FreeSurface(surface);
		}
		size[0] = 0;
		size[1] = 0;
		return 0;
	}

	// Free the SDL_Surface only if it was successfully created
	if (surface) {
		size[0] = surface->w;
		size[1] = surface->h;
		SDL_FreeSurface(surface);
	}

	

	textureHolder[theString] = texture;



	return texture;






	


}


GLuint * generateTextureR(std::string theString){

	int data[2];
	return generateTexture(theString, data, true);


}


GLuint * generateTexture(std::string theString){


	int data[2];
	return generateTexture(theString, data, false);

}



GLuint * generateTexture(DoorDict * dict){

	int data[2];


	GLuint * returnValue = generateTexture(findFile(dict->textureName), data, false);
	dict->textureWidth = data[0];
	dict->textureHeight = data[1];

	return returnValue;


}



GLuint * generateTexture(SpriteDict * dict){

	int data[2];


	GLuint * returnValue = generateTexture(findFile(dict->textureName), data, false);
	dict->textureWidth = data[0];
	dict->textureHeight = data[1];

	return returnValue;
}



GLuint dontStore(std::string theString, int * value){



	
	
	GLuint texture; // This is a handle to our texture object

	SDL_Surface *surface;	// This surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;



	surface = IMG_Load(theString.c_str());



	if (surface) {
		/*
		// Check that the image's width is a power of 2
		if ((surface->w & (surface->w - 1)) != 0) {
			printf("warning: image.bmp's width is not a power of 2\n");
		}

		// Also check if the height is a power of 2
		if ((surface->h & (surface->h - 1)) != 0) {
			printf("warning: image.bmp's height is not a power of 2\n");
		}
		*/
		// get the number of channels in the SDL surface
		nOfColors = surface->format->BytesPerPixel;
		if (nOfColors == 4)     // contains an alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
#if defined(_WIN32)
				texture_format = GL_RGBA;
#endif

#if defined(__APPLE__ )

			texture_format = GL_BGRA;
#endif




			else
#if defined(_WIN32)
				texture_format = GL_BGRA;
#endif

#if defined(__APPLE__ )

			texture_format = GL_BGRA;
#endif
		}
		else if (nOfColors == 3)     // no alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_RGB;
		}
		else {
			printf("warning: the image is not truecolor..  this will probably break\n");
			// this error should not go unhandled
		}

		// Have OpenGL generate a texture object handle for us
		glGenTextures(1, &texture);




		// Bind the texture object
		glBindTexture(GL_TEXTURE_2D, texture);

		// Set the texture's stretching properties
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
			texture_format, GL_UNSIGNED_BYTE, surface->pixels);
		
	}
	else {
		printf("SDL could not load image.bmp: %s\n", SDL_GetError());
		if (surface) {
			SDL_FreeSurface(surface);
		}
		value[0] = 0;
		value[1] = 0;
		return 0;
	}

	// Free the SDL_Surface only if it was successfully created
	if (surface) {
		value[0] = surface->w;
		value[1] = surface->h;
		SDL_FreeSurface(surface);
	}




	return texture;





}


void dumpAllTextures(){


}



GLuint * replaceTexture(DoorDict * dict){
	printf("Replace Texture Door Not Working");
	return 0; }
GLuint * replaceTexture(SpriteDict * dict){
	printf("Remove Texture Sprite Dict is Not Set Up");
	return 0; }
GLuint * replaceTexture(std::string type){
	printf("Remove Texture Type is Not Set Up");
	
	return 0; }
void removeTexture(GLuint * aText){
	printf("Remove Texture is Not Set Up");


}





















