/*
 * Copyright 2020~2021 SYSON, MICHAEL B.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @company: USBONG
 * @author: SYSON, MICHAEL B.
 * @date created: 20200926
 * @date updated: 20210822
 * @website address: http://www.usbong.ph
 *
 * References:
 * 1) Dev-C++ 5.11 auto-generated OpenGL example project
 *
 * 2) https://www.libsdl.org/download-2.0.php;
 * last accessed: 20200423
 *
 * 3) https://docs.microsoft.com/en-us/windows/win32/inputdev/user-input3
 * last accessed: 20200424
 *
 * 4) Astle, D. and Hawkin, K. (2004). "Beginning OpenGL Game Programming". USA: Premier Press.
 *
 * 5) https://en.wikibooks.org/wiki/OpenGL_Programming/Basics/Structure
 * last accessed: 20200926
 *
 * 6) https://dos.gamebub.com/cpp.php
 * last accessed: 20200921
 *
 * 7) https://chortle.ccsu.edu/Bloodshed/howToGL.html
 * last accessed: 20200927
 * --> instructions to add GLUT library in Dev-C++ Editor 5.11
 *
 * 8) https://stackoverflow.com/questions/5289284/compiling-and-runnin-opengl-glut-program-in-ubuntu-10-10
 * --> last accessed: 20200928
 *
 * 9) https://askubuntu.com/questions/96087/how-to-install-opengl-glut-libraries
 * --> last accessed: 20200928
 *
 * 10) https://www3.ntu.edu.sg/home/ehchua/programming/opengl/HowTo_OpenGL_C.html
 * --> last accessed: 20200928
 *
 * 11) https://stackoverflow.com/questions/2571402/how-to-use-glortho-in-opengl/36046924#36046924;
 * --> last accessed: 20200928
 * --> answers by: Mikepote, 20100408T1912
 * --> answers by: Santilli, Ciro, 20160316T2106
 *
 * 12) https://www.parallelrealities.co.uk/tutorials/;
 * --> last accessed: 20210818
 * --> 12.1) https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php;
 * --> last accessed: 20210818
 * --> 12.2) https://www.parallelrealities.co.uk/tutorials/shooter/shooter3.php;
 * --> last accessed: 2021819
 * --> execution speed OK in 32-bit OS; using SDL and SDL Image, without OpenGL
 *
 * 13) https://wiki.libsdl.org/SDL_SetRenderDrawColor;
 * --> last accessed: 20210818
 *
 * 14) https://wiki.libsdl.org/MigrationGuide;
 * --> last accessed: 20210821
 *
 * Notes:
 * 1) We can use this software tool to extract the compressed (zipped) folder.
 *   https://www.7-zip.org/download.html; last accessed: 20200423
 *
 * 2) OpenGL (Open Graphics Library) Utility Toolkit Library
 *
 * 3) Linux Machine
 * 3.1) Compile Command
 *   g++ main.cpp -o mainOutput -lGL -lGLU -lglut
 *
 * 3.2) Execute Command
 *   ./mainOutput
 *
 * 3.3) Install OpenGL Libraries
 *   sudo apt-get install mesa-utils
 *
 * 3.4) Install GLUT Library
 *   sudo apt-get install freeglut3-dev
 */
 
 //Additional References:
//1) https://discourse.libsdl.org/t/sdlsurface-to-opengl-texture/15597;
//last accessed: 20210823
//output: displays white textured .png image file

//2) https://ubuntuforums.org/archive/index.php/t-1088836.html;
//last accessed: 20210823
 //output: displays loaded .png image file

//TO-DO: -update: this

#include <stdio.h>
#include <stdlib.h>

/*
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

	//added by Mike, 20201209
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
/*
#define WIDTH 800
#define HEIGHT 600
*/
static int myWindowWidthAsPixel=640; //2048;//320
static int myWindowHeightAsPixel=640; //2048;//320


SDL_Window *mySDLWindow = NULL;
SDL_Renderer *mySDLRenderer = NULL;


GLuint LoadTexture(char *filename, int *textw, int *texth)
{
SDL_Surface *surface;
GLenum textureFormat;
GLuint texture;

surface = IMG_Load(filename);
if (!surface){
return 0;
}

switch (surface->format->BytesPerPixel) {
case 4:
if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
textureFormat = GL_BGRA;
else
textureFormat = GL_RGBA;
break;

case 3:
if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
textureFormat = GL_BGR;
else
textureFormat = GL_RGB;
break;
}

*textw = surface->w;
*texth = surface->h;

glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w,
surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

SDL_FreeSurface(surface);

return texture;

}

void DrawTexture(int x, int y, GLuint textureid, int textw, int texth)
{
glBindTexture(GL_TEXTURE_2D, textureid);
glEnable(GL_TEXTURE_2D);

glBegin(GL_QUADS);
glTexCoord2i(0, 0);
glVertex2f(x, y);

glTexCoord2i(1, 0);
glVertex2f(x + textw, y);

glTexCoord2i(1, 1);
glVertex2f(x + textw, y + texth);

glTexCoord2i(0, 1);
glVertex2f(x, y + texth);
glEnd();

glDisable(GL_TEXTURE_2D);
}

void Enable2D()
{
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();

//glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
glOrtho(0, myWindowWidthAsPixel, myWindowHeightAsPixel, 0, -1, 1);


glDisable(GL_DEPTH_TEST);
glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glLoadIdentity();
}

void Disable2D()
{
glMatrixMode(GL_PROJECTION);
glPopMatrix();
glMatrixMode(GL_MODELVIEW);
glPopMatrix();
glEnable(GL_DEPTH_TEST);
}

void initSDL(void)
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;

	windowFlags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	//includes window title bar	
	SDL_DisplayMode mySDLDisplayMode;
	SDL_GetCurrentDisplayMode(0, &mySDLDisplayMode);
	
	myWindowWidthAsPixel=mySDLDisplayMode.w;
	myWindowHeightAsPixel=mySDLDisplayMode.h;
	
	
//	mySDLWindow = SDL_CreateWindow("Pagong Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, myWindowWidthAsPixel, myWindowHeightAsPixel, windowFlags);
/* //edited by Mike, 20210821
	mySDLWindow = SDL_CreateWindow("Pagong Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, myWindowWidthAsPixel, myWindowHeightAsPixel, windowFlags | SDL_WINDOW_BORDERLESS);
*/	
	mySDLWindow = SDL_CreateWindow("Usbong Story Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, myWindowWidthAsPixel, myWindowHeightAsPixel, windowFlags | SDL_WINDOW_BORDERLESS);

	if (!mySDLWindow )
	{
		printf("Failed to open %d x %d window: %s\n", myWindowWidthAsPixel, myWindowHeightAsPixel, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	mySDLRenderer = SDL_CreateRenderer(mySDLWindow, -1, rendererFlags);

	if (!mySDLRenderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}
	
/* //removed by Mike, 20210821	
	//added by Mike, 20210821
//	SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, SDL_DOUBLEBUF | SDL_HWSURFACE);
	SDL_Surface *screen = SDL_SetVideoMode(myWindowWidthAsPixel, myWindowHeightAsPixel, 32, SDL_DOUBLEBUFFER | SDL_SWSURFACE);
*/
	
	//removed by Mike, 20210819
	//SDL_GetRendererOutputSize(mySDLRenderer, &myWindowWidthAsPixel, &myWindowHeightAsPixel);
	
	//added by Mike, 20210821
	SDL_GL_CreateContext(mySDLWindow);
}

void doInput(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;

			case SDL_KEYDOWN:
//				keyDown(&event.key);
				break;

			case SDL_KEYUP:
//				keyUp(&event.key);
				break;

			default:
				break;
		}
	}
}

int main( int argc, char **argv )
{

initSDL();
/*
const SDL_VideoInfo* video;
*/
SDL_Event e;
GLuint texture;
int textw, texth;

/*
if(SDL_Init(SDL_INIT_VIDEO) < 0)
{
fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
exit(1);
}


atexit(SDL_Quit);

video = SDL_GetVideoInfo();
if(video == NULL)
{
fprintf(stderr, "Couldn't get video information: %s\n", SDL_GetError());
exit(1);
}

SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

if(SDL_SetVideoMode(WIDTH, HEIGHT, video->vfmt->BitsPerPixel, SDL_OPENGL) == 0 )
{
fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
exit(1);
}
*/

texture = LoadTexture((char*) "textures/imageSpriteExampleMikeWithoutBG.png", &textw, &texth);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glLoadIdentity();

	while (1)
	{
/*
while(!SDL_PollEvent(&e) || e.type!=2)
{
*/
doInput();

Enable2D();
DrawTexture(0, 0, texture, textw, texth);
Disable2D();

SDL_GL_SwapWindow(mySDLWindow);
//SDL_GL_SwapBuffers();

//SDL_Delay(16);

}

return(0);
}
