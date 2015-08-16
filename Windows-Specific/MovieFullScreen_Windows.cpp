#include "MovieFullScreen.h"
#include <stdio.h>
#include "FileManager.h"
#include "VideoSDL.h"
#include "GameLogic.h"


static Video * fullScreen = 0;

void initVideo(){
	std::string path = findFile("OpenVideo.mp4");

	fullScreen = new Video();
	fullScreen->init();
	fullScreen->openFile(path);


	//printf("Window Function not Enabled");

}

static GLuint _vertexArray = 0;
static GLuint _vertexBuffer = 0;


static bool fps30 = false;

bool updateVideo(){

	if (fps30){
		fps30 = !fps30;
		return true;
	}
	fps30 = !fps30;



	if (!_vertexArray){

		GLfloat width = GameLogic::width;
		GLfloat height = GameLogic::width * (1080.0 / 1920.0);

	GLfloat gBoxVertexData[] = {

		(GLfloat)(-width),
		(GLfloat)(-height),
		0, 1,


		(GLfloat)(-width),
		(GLfloat)(height),
		0, 0,

		(GLfloat)(width),
		(GLfloat)(-height),
		1, 1,

		(GLfloat)(width),
		(GLfloat)(height),
		1, 0
	};

	if (!_vertexArray){
		glGenVertexArraysCon(1, &_vertexArray);
	}
	glBindVertexArrayCon(_vertexArray);

	if (!_vertexBuffer){
		glGenBuffers(1, &_vertexBuffer);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gBoxVertexData), gBoxVertexData, GL_STATIC_DRAW);


	glEnableVertexAttribArray(GLKVertexAttribPosition);
	glVertexAttribPointer(GLKVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, 4 * 4, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
	glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 4 * 4, BUFFER_OFFSET(8));

	}

		return fullScreen->Update();
	//return true;
}
void renderVideo(){

	if (_vertexArray != 0){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fullScreen->returnTexture());
		glBindVertexArrayCon(_vertexArray);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}



}

void closeVideo(){
	//printf("Window Function not Enabled");
}














