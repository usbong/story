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
 * @date created: 20200930
 * @date updated: 20210820
 * @website address: http://www.usbong.ph
 *
 * Reference: 
 * 1) Astle, D. and Hawkin, K. (2004). "Beginning OpenGL game programming". USA: Thomson Course Technology
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007): 
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 * 
 */
 
//added by Mike, 20210728
//TO-DO: -update: Pilot does not enter gap of tile grid x1
//add: offset in tile grid to make inner rectangle for collision detection
 
//added by Mike, 20210130
//reverify: Robotship does not execute correctly
//when down and left buttons are pressed while firing beam down
//AND when up and left buttons are pressed while firing beam up		  
//in Windows Machine
//problem did not occur on Linux Machine (with external USB keyboard)
//added by Mike, 20210131
//note: add use of external USB keyboard solves the problem		  

//TO-DO: -update: this

//added by Mike, 20210201
//TO-DO: -delete: excess instructions
//TO-DO: increase lower right arm angle to go up y-axis

//added by Mike, 20210202
//TO-DO: -update: instructions when diagonal movement is combined with attack and defend keys


#include <stdio.h>
#include <math.h>

/*	//removed by Mike, 20201121
#include <gl/gl.h>
#include <gl/glu.h>
*/

//added by Mike, 20201121
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
//#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
#endif

/*	//removed by Mike, 20210820
//added by Mike, 20210816
#ifdef _WIN32 //Windows machine
	#include <SDL.h>
	#include <SDL_image.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif
*/

//added by Mike, 20210820
//TO-DO: -reverify: http://www.nongnu.org/pngpp/doc/0.2.9/;
//last accessed: 20210820
//#include "png.hpp"
//#include <pngpp/png.hpp>
#include <png.hpp>

#include "Pilot.h"
/* //TO-DO: -add: these
#include "PolygonUtils.h"
#include "ModelPool.h"
*/

//added by Mike, 20210523
#include "UsbongUtils.h"

//added by Mike, 20210725
#include "Level2D.h"

#include <string.h>

//added by Mike, 20201217
//Reference: https://stackoverflow.com/questions/34152424/autodetection-of-os-in-c-c;
//answer by: Jeegar Patel, 20151208T0940
//auto-identify if Windows Machine
#ifdef _WIN32
	#include <windows.h> //Windows Machine
#endif
/*
#ifdef linux
    printf("In Linux");
#endif
*/

//#include "ModelPool.h"

//added by Mike, 20210130
//TO-DO: -reverify: fire beam down, move left up not OK
//TO-DO: -reverify: fire beam up, move left down not OK

//edited by Mike, 20201014
//added by Mike, 20201001
enum Keys
{
/*	//edited by Mike, 20210128
	KEY_UP = 0,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_SPACE,
	//edited by Mike, 20201013
	KEY_ENTER,
	//TO-DO: -verify: gamepad
	KEY_A,
	KEY_D,
	KEY_W,
	KEY_S,
*/
	//TO-DO: -verify: gamepad
/*	//edited by Mike, 20210129
	KEY_A = 0,
	KEY_D,
	KEY_W,
	KEY_S,
*/
	KEY_W = 0,
	KEY_S,
	KEY_D,
	KEY_A,
/* //removed by Mike, 20210130
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_SPACE,
	KEY_ENTER,
*/
	KEY_J,
	KEY_L,
	KEY_I,
	KEY_K,
	KEY_H, //added by Mike, 20210111
	KEY_U, //added by Mike, 20210121

    //added by Mike, 20210603
    KEY_Z,
    
	//added by Mike, 20201226
	iNumOfKeyTypes
};

//object: Cube.005_0
//body
typedef struct
{
    GLubyte id_field_length;
    GLubyte color_map_type;
    GLubyte image_type_code;
    GLubyte ignore[9];
    GLushort width;
    GLushort height;
    GLubyte image_pixel_size;
    GLubyte image_descriptor;
} TARGA_HEADER;

//TO-DO: -put: in MyDynamicObject
GLboolean Pilot::test_pow2(GLushort i)
{
    while (i % 2 == 0)
        i /= 2;
    if (i == 1)
        return GL_TRUE;
    else
        return GL_FALSE;
}

//TO-DO: -put: in MyDynamicObject
//Note: [Warning] deprecated conversion from string constant to 'char*' [-Wwrite-strings]
void Pilot::load_tga(char *filename)
{
    TARGA_HEADER targa;
    FILE *file;
    GLubyte *data;
    GLuint i;

    file = fopen(filename, "rb");
    if (file == NULL)
        return;

    /* test validity of targa file */
/*  //edited by Mike, 20210816
    if (fread(&targa, 1, sizeof(targa), file) != sizeof(targa) ||
        targa.id_field_length != 0 || targa.color_map_type != 0 ||
        targa.image_type_code != 2 || ! test_pow2(targa.width) ||
        ! test_pow2(targa.height) || targa.image_pixel_size != 32 ||
        targa.image_descriptor != 8)
*/        
    if (fread(&targa, 1, sizeof(targa), file) != sizeof(targa) ||
        targa.id_field_length != 0 || targa.color_map_type != 0 ||
        targa.image_type_code != 2 || ! test_pow2(targa.width) ||
        ! test_pow2(targa.height) || targa.image_pixel_size != 8 ||
        targa.image_descriptor != 8)
    {
        fclose(file);
        free(data);
        return;
    }

    /* read targa file into memory */
    data = (GLubyte *) malloc(targa.width * targa.height * 4);
    if (fread(data, targa.width * targa.height * 4, 1, file) != 1)
    {
        fclose(file);
        return;
    }

    /* swap texture bytes so that BGRA becomes RGBA */
    for (i = 0; i < targa.width * targa.height * 4; i += 4)
    {
        GLbyte swap = data[i];
        data[i] = data[i + 2];
        data[i + 2] = swap;
    }

    /* build OpenGL texture */
    fclose(file);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, targa.width, targa.height,
                      GL_RGBA, GL_UNSIGNED_BYTE, data);
    free(data);
}

//added by Mike, 20210816
//TO-DO: -put: in MyDynamicObject
//Note: [Warning] deprecated conversion from string constant to 'char*' [-Wwrite-strings]
//TO-DO: -reverify: this; www.stackoverflow.com
void Pilot::load_png(char *filename)
{
	GLuint texture;
	
	SDL_Surface *surfacePart1;
	surfacePart1 = IMG_Load(filename);
/*
	SDL_Surface *surface;
	surface = IMG_Load(filename);
*/	
	
//SDL_DisplayFormatAlpha(surface);
	
	SDL_Surface *surface = SDL_ConvertSurfaceFormat(
    surfacePart1, SDL_PIXELFORMAT_ARGB8888, 0);
	
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D, texture);

/*	
glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);	
*/
	
int iMode = NULL;

if(surface->format->BytesPerPixel == 3) {iMode = GL_RGB;}
else if(surface->format->BytesPerPixel == 4) {iMode = GL_RGBA;}

//glTexImage2D(GL_TEXTURE_2D, 0, Mode, image->w, image->h, 0, Mode, GL_UNSIGNED_BYTE, image->pixels);


	glTexImage2D(GL_TEXTURE_2D, 0, iMode, surface->w, surface->h, 
				 0, iMode, GL_UNSIGNED_BYTE, surface->pixels);
/*
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 
				 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
*/

/*				 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA,surface->w, surface->h, 
				 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
*/
/*	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, surface->w, surface->h, 
					  GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
*/					  
/*
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
*/	
	SDL_FreeSurface(surface);	
	SDL_FreeSurface(surfacePart1);

/*    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, targa.width, targa.height,
                      GL_RGBA, GL_UNSIGNED_BYTE, data);
*/                      
}

//added by Mike, 20210423
void Pilot::setup()
{
	//removed by Mike, 20201010
	//due to blank output
    //glEnable(GL_DEPTH_TEST);

    // select texture 1
	glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
	
    /* create OpenGL texture out of targa file */
	//edited by Mike, 20210420
//    load_tga("textures/armor.tga");	
	//edited by Mike, 20210816
//    load_tga("textures/imageSpriteExampleMikeWithoutBG.tga");	
    load_png("textures/imageSpriteExampleMikeWithoutBG.png");	
	
	// set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

/*	//edited by Mike, 20210723; this is due to displayed image is blurred
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);                    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);                    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

/*
    // select texture 1
	glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_B);
	
    // create OpenGL texture out of targa file
    load_tga("textures/armor.tga");	
	
	// set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
    // select texture 1
	glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_C);
	
    // create OpenGL texture out of targa file
    load_tga("textures/armor.tga");	
	
	// set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
*/
	
    /* unselect texture myFontTextureObject */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* setup alpha blending */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}


//TO-DO: -update: this
void Pilot::setupPrev()
{
    GLuint i, j;
	
	/* select and enable texture 1 */
    glBindTexture(GL_TEXTURE_2D, 1);
	
	/* create OpenGL texture out of targa file */
    load_tga("textures/body/halimbawa.body.tga");
	
    /* set texture parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//.../home/unit_member/Documents/USBONG/R&D/With C++ Computer Language and OpenGL/UsbongAutomotive/Linux
	
	/* select and enable texture 5 */
    glBindTexture(GL_TEXTURE_2D, 5);
	
	/* create OpenGL texture out of targa file */
    load_tga("textures/cockpit/halimbawa.cockpit.tga");
	
    /* set texture parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	//...
	
	/* select and enable texture 8 */
    glBindTexture(GL_TEXTURE_2D, 8);
	
	/* create OpenGL texture out of targa file */
    load_tga("textures/wings/halimbawa.wings.tga");
	
/*	
	//EXPLOSION/ DEATH ANIMATION
	    // retrieve "unused" texture object
	glGenTextures(1, &myDeathAnimationImg1);
    glBindTexture(GL_TEXTURE_2D, myDeathAnimationImg1);
    load_tga("textures/explosion/explosion1.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    // retrieve "unused" texture object
	glGenTextures(1, &myDeathAnimationImg2);
    glBindTexture(GL_TEXTURE_2D, myDeathAnimationImg2);
    load_tga("textures/explosion/explosion2.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenTextures(1, &myDeathAnimationImg3);
    glBindTexture(GL_TEXTURE_2D, myDeathAnimationImg3);
    load_tga("textures/explosion/explosion3.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    // retrieve "unused" texture object
	glGenTextures(1, &myDeathAnimationImg4);
    glBindTexture(GL_TEXTURE_2D, myDeathAnimationImg4);
    load_tga("textures/explosion/explosion4.tga");
	    // retrieve "unused" texture object
	glGenTextures(1, &myDeathAnimationImg5);
    glBindTexture(GL_TEXTURE_2D, myDeathAnimationImg5);
    load_tga("textures/explosion/explosion5.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/	
	/* unselect and disable texture 1 */
    glBindTexture(GL_TEXTURE_2D, 0);
	
	/* setup alpha blending */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

//added by Mike, 20201001
//TO-DO: -add: this in ModelPool.cpp
void Pilot::drawModelPilot() {
/*	//edited by Mike, 20201019
	//added by Mike, 20201002
	//to make anchor/origin/reference point start at top-left
    glTranslatef(0.0f, 0.1f, 0.0f);   
	
	//Reference: https://www3.ntu.edu.sg/home/ehchua/programming/opengl/HowTo_OpenGL_C.html;
	//last accessed: 20200928 
   //note: 0.5f = half of the window width or height
   // Draw a Red 1x1 Square centered at origin
   //TO-DO: -update: this due to output is 1 x 2 box, width x height
   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
//    //edited by Mike, 20201001  
//	  glVertex2f(-0.1f, -0.1f);    // x, y
//      glVertex2f( 0.1f, -0.1f);
//      glVertex2f( 0.1f,  0.1f);
//      glVertex2f(-0.1f,  0.1f);
      
	  //1x1 box
	  glVertex2f(0.0f, 0.0f);    // x, y
      glVertex2f( 0.0f, -0.1f);
      glVertex2f( 0.1f,  -0.1f);
      glVertex2f(0.1f,  0.0f);
   glEnd();    
*/   

/*
	glRotatef(15.0, 1.0, 0.0, 0.0);
	glRotatef(30.0f, 0.0, 1.0, 0.0);
	glScalef(0.75, 0.75, 0.75);
*/

	//removed by Mike, 20201020
/*	glRotatef(-10.0, 1.0, 0.0, 0.0);
	glRotatef(35.0f, 0.0, 1.0, 0.0);
*/
	
	//added by Mike, 20201023; removed by Mike, 20201114
	//set square face with no color fill 
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	    glColor3f(1.0f, 0.0f, 0.0f); // Red
	
		//edited by Mike, 20201023
	//	drawCube();
		drawCube(myWidth); //myWidth = myHeight

	//added by Mike, 20201023; removed by Mike, 20201114
	//set square face with color fill 	
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

//added by Mike, 20210522
//TO-DO: -identify cause of incorrect position in grid for player2

//edited by Mike, 20201001
//Pilot::RobotShip(): MyDynamicObject(0,0,300)
//edited by Mike, 20201115
//Pilot::RobotShip(): MyDynamicObject(0,0,0)
//edited by Mike, 20210815
//Pilot::Pilot(float xPos, float yPos, float zPos, int windowWidth, int windowHeight): MyDynamicObject(xPos,yPos,0.0f, windowWidth, windowHeight)
Pilot::Pilot(float xPos, float yPos, float zPos, float fWindowWidth, float fWindowHeight): MyDynamicObject(xPos,yPos,0.0f, fWindowWidth, fWindowHeight)
{ 
    //edited by Mike, 20201001
	//currentState=IN_TITLE_STATE;//MOVING_STATE;
	currentState=MOVING_STATE;
	//added by Mike, 20201201
	currentMovingState=IDLE_MOVING_STATE;
	
	//added by Mike, 20210611
	fTaoAnimationFrameOffsetYAxis=0.0f;
	
	//added by Mike, 20210810
	iShieldEffectCount=0;
	iGlowEffectCount=0;
	iGlowFadeEffectCount=0;

//    myXPos=0.0;
//    myYPos=0.0;
    //myYPos=300.0;
//    myZPos=300.0;
/*    
    stepX=0.01;
    stepY=0.01;
    stepZ=0.01;
*/    
/*	//edited by Mike, 20201023
	//added by Mike, 20201001
    stepX=0.03;
    stepY=0.03;
*/
/*	//edited by Mike, 20201116
    stepX=0.1;
    stepY=0.1;
*/
	
    //edited by Mike, 20201025
	//edited again by Mike, 20210114
	//reverified double step due to double the size of Window	
	//4096x4096; update in main.cpp
	//OpenGLCanvas update sleep delay, instead of step
/*	//edited by Mike, 20210116
    stepX=0.3;
    stepY=0.3;
    stepZ=0.3;
*/
	//edited by Mike, 20210505
	//note: if set to 0.3; noticeable zoom-in, zoom-out cycling movement due to quick speed
	//observed: Samurai Spirits IV's created world executes such camera eye movement
	//that becomes noticeable with background zoom-in, zoom-out via cycling movement
	//edited by Mike, 20210522
/*	
//    stepX=0.3;
    stepX=0.2;
	
    stepY=0.3;
    stepZ=0.3;
*/
/* //edited by Mike, 20210523
    stepX=0.4;	
    stepY=0.4;
    stepZ=0.4;
*/
    /* //edited by Mike, 20210605
    stepX=0.4*4;
    stepY=0.4*4;
    stepZ=0.4*4;
*/
/* //edited by Mike, 20210725
    //note: we use integer with myXPos, et cetera
    stepX=0.5*4;
    stepY=0.5*4;
    stepZ=0.5*4;
*/
    //note: we use integer with myXPos, et cetera
    //added by Mike, 20210725
    //execute *2; Sonic the Hedgehog? due to speed and image blurs?
    //sticks to wall to cause acceleration effect?
    
   //edited by Mike, 20210726
/*   //due to we add to pixel position, instead of vertex position
    stepX=0.5*4*2;
    stepY=0.5*4*2;
    stepZ=0.5*4*2;
*/
    
/*	  //removed by Mike, 20210726
		//TO-DO: -add: auto-compute grid tile
    stepX=32;
    stepY=32;
    stepZ=32;
*/
    
    invincibleCounter=0;
    currentDeathFrame=0;

	//edited by Mike, 20201001
/*  myWidth=2.0f;//1.5;
    myHeight=2.0f;//1.5;
*/    
/*	//edited by Mike, 20201023
    myWidth=0.1f;
    myHeight=0.1f;
*/

	//edited by Mike, 20201201    
	//TO-DO: -update: this
/*	myWidth=1.0f;
    myHeight=1.0f;
*/
	
/* //edited by Mike, 20210522	
	myWidth=1.4f;
    myHeight=1.4f;
*/
		//edited by Mike, 20210723
	//TO-DO: -add: auto-identify tile width and height
/*
		myWidth=4.0f;
    myHeight=4.0f;
*/  
    
  //added by Mike, 20210523; edited by Mike, 20210527
  //note: these are for size of whole image, i.e. not clipped to be only select parts
/*	myWidthAsPixel=128; //64*2
  myHeightAsPixel=64*4;
*/  
/*
	myWidthAsPixel=128; //64*2
  myHeightAsPixel=192; //64*3
*/
/*	//removed by Mike, 20210528
	myWidthAsPixelMax=64*8;
	myHeightAsPixelMax=64*4;
*/
	
//note:	glScalef(0.2f/2, 0.4f/2, 1.0f);			
	//added by Mike, 20210527; edited by Mike, 20210611
/*	myWidthAsPixel=128*0.2/2;
    myHeightAsPixel=192*0.4/2;
*/

		//edited by Mike, 20210723
	//TO-DO: -add: auto-identify tile width and height
/*
    myWidthAsPixel=64;
    myHeightAsPixel=64;
*/
/* //edited by Mike, 20210725
    myWidthAsPixel=64+32;
    //edited by Mike, 20210725
//    myHeightAsPixel=64*2+32;
    myHeightAsPixel=64+32;
*/
/*
    myWidthAsPixel=16;
    myHeightAsPixel=16;
*/

/*
//removed by Mike, 20210729
    //TO-DO: -add: auto-set width and height based on grid tile
    //note: we use image texture scale COMMANDS, et cetera
    //edited by Mike, 20210729
    //add offset due to non-transparent sprite image smaller than tile size
    myWidthAsPixel=71;
    myHeightAsPixel=80;
*/
    
	  //added by Mike, 20210726
		//TO-DO: -add: auto-compute grid tile
/*	//removed by Mike, 20210727		
    stepX=myWidthAsPixel;// 32;
    stepY=myHeightAsPixel; //32;
    stepZ=myWidthAsPixel; //32
*/

/*	//removed by Mike, 20210528
//added by Mike, 20210528
	myWidthAsPixel=64;
    myHeightAsPixel=64;
*/
    
	//added by Mike, 20210523
	//edited by Mike, 202105028
	myXPos=xPos;
    myYPos=0.0f;
    myZPos=zPos;
  	
/*
	myXPos=0.0f;
	myYPos=0.0f;
  myZPos=0.0f;
*/
    
	//TO-DO: -update: this
	//note: float xPos as parameter to int myXPosAsPixel not correct output	
/* //edited by Mike, 20210523	
	myXPosAsPixel=320;//xPos; //320;//(int)xPos;
	myYPosAsPixel=320;//yPos; //(int)yPos;
	myZPosAsPixel=0.0f; //(int)zPos;    
*/

/* //edited by Mike, 20210726
	//note: we use zPos for the yPosAsPixel
	myXPosAsPixel=xPos; //320;//(int)xPos;
	myYPosAsPixel=zPos;//yPos; //(int)yPos;
	myZPosAsPixel=0.0f;//(int)zPos;    
*/
	myXPosAsPixel=xPos;
	myYPosAsPixel=yPos;
	myZPosAsPixel=zPos;


/* //removed by Mike, 20210523
//	myXPos=0.0f;
	myXPos=xPos;
	
    //edited by Mike, 2020116
//    myYPos=0.0f+myHeight*3;

	//added by Mike, 20210503
	//myZPos updated again in another location
	//edited by Mike, 20210521	
//    myZPos=0.0f+myHeight*3;
    myZPos=0.0f;
*/
	
/*	//added by Mike, 20201115; edited by Mike, 20210815
	myWindowWidth=windowWidth;
	myWindowHeight=windowHeight;
*/
	fMyWindowWidth=fWindowWidth;
	fMyWindowHeight=fWindowHeight;
	
/*	
	printf(">>Pilot.cpp: myWindowWidth: %i\n",myWindowWidth);
	printf(">>Pilot.cpp: myWindowHeight: %i\n",myWindowHeight);
*/	
	//added by Mike, 20210727
  //TO-DO: -add: receive as input the values for the following to execute cascading values
  iRowCountMax=10;
  iColumnCountMax=18;
  iHeightCountMax=10;

/* //edited by Mike, 20210815    	
  fGridSquareWidth = myWindowWidth/iColumnCountMax; //example: 136.60
  fGridSquareHeight = myWindowHeight/iRowCountMax; //example: 76.80
*/	
  fGridSquareWidth = fMyWindowWidth/iColumnCountMax; //example: 136.60
  fGridSquareHeight = fMyWindowHeight/iRowCountMax; //example: 76.80
	
	  //added by Mike, 20210726
		//note: auto-compute based on grid tile
		//TO-DO: -add: acceleration
	//edited by Mike, 20210728		
/*	
  	stepX=fGridSquareWidth/8;
    stepY=fGridSquareHeight/8;
    stepZ=fGridSquareWidth/8;
*/
  	//edited by Mike, 20210807	
  	stepX=fGridSquareWidth/10;
//  	stepX=fGridSquareWidth/10*2;

  	//edited by Mike, 20210807	
    stepY=fGridSquareHeight/10;
//    stepY=fGridSquareHeight/10*2;

    stepZ=fGridSquareWidth/10;
	
/*
  	stepX=fGridSquareWidth/9;
    stepY=fGridSquareHeight/9;
    stepZ=fGridSquareWidth/9;
*/

	//added by Mike, 20210729
    //auto-set width and height based on grid tile
    //note: we use image texture scale COMMANDS, et cetera
    //add offset due to non-transparent sprite image smaller than tile size
/*    iOffsetXPosAsPixel=16;
    iOffsetYPosAsPixel=16;
*/    
/*
    iOffsetXPosAsPixel=0;
    iOffsetYPosAsPixel=0;
*/

/* //removed by Mike, 20210730
    //edited by Mike, 20210729;
    //TO-DO: -reverify: setXPosAsPixel(...) after collision detected with tile; not centered?
    //-reverify: with another machine; previously iOffsetXPosAsPixel=12; OK in Linux machine due to window screen width and height?
//    iOffsetXPosAsPixel=12;
    iOffsetXPosAsPixel=20;
    iOffsetYPosAsPixel=12;
*/
    
/*    
    myWidthAsPixel=fGridSquareWidth-iOffsetXPosAsPixel;
    myHeightAsPixel=fGridSquareHeight-iOffsetYPosAsPixel;
*/	
    myWidthAsPixel=fGridSquareWidth;
    myHeightAsPixel=fGridSquareHeight;

    //edited by Mike, 20210730;
    //TO-DO: -reverify: setXPosAsPixel(...) after collision detected with tile; not centered?
    //-reverify: with another machine; previously iOffsetXPosAsPixel=12; OK in Linux machine due to window screen width and height?
//    iOffsetXPosAsPixel=12;

/*  //note: execution crash problem in Windows 7 machine; 
	//due to multiply by fraction to output integer, i.e. whole number?
	//OK Compile & Run after Rebuild All Command
	iOffsetXPosAsPixel=fGridSquareWidth*0.28;
    iOffsetYPosAsPixel=fGridSquareHeight*0.15;	
*/
/*  execution crash problem in Windows 7 machine; 
    iOffsetXPosAsPixel=fGridSquareWidth*28/100;
    iOffsetYPosAsPixel=fGridSquareHeight*15/100;	

    iOffsetXPosAsPixel=20;
    iOffsetYPosAsPixel=12;	

	iOffsetXPosAsPixel=30;
    iOffsetYPosAsPixel=12;	
*/
	//OK Compile & Run after Rebuild All Command
//    iOffsetXPosAsPixel=fGridSquareWidth*28/100;
//    iOffsetYPosAsPixel=fGridSquareHeight*15/100;	

	iOffsetXPosAsPixel=fGridSquareWidth*0.28;
    iOffsetYPosAsPixel=fGridSquareHeight*0.15;	


	//added by Mike, 20210728
	bHasHitWall=false;
	
	//added by Mike, 20210523
	myUsbongUtils = new UsbongUtils();
	//added by Mike, 20210726; edited by Mike, 20210727
//  myUsbongUtils->setWindowWidthHeight(windowWidth, windowHeight);
	//edited by Mike, 20210815
//  	myUsbongUtils->setWindowWidthHeight(myWindowWidth, myWindowHeight);
  	myUsbongUtils->setWindowWidthHeight(fMyWindowWidth, fMyWindowHeight);
    
//    myWidthX=0.5;

    rotationAngle=0.0f;//360.0f;//90.0;
    rotationStep=10.0;//1.0f;    
    thrust=0.0f;
    thrustMax=0.8f;
    xVel;
    yVel;
    //edited by Mike, 20201001
    maxXVel=0.04f;//1.0f;
    maxYVel=0.04f;//1.0f;

    //boundary = 8.0f;

    tricount = 120;
    isMovingForward = 0;

	//added by Mike, 20210122
	iPunchAnimationCountDelay=0;
	//added by Mike, 20210123
	iPunchAnimationCount=0;

    //added by Mike, 20210613
    bIsExecutingKick=false;
    iKickAnimationCount=0;
    iKickAnimationCountDelay=0;

    //added by Mike, 20210805
    bIsExecutingPunch=false;
    bIsExecutingDefend=false;

    
    //init default values
    //previousFacingState=FACING_UP;
    //currentFacingState=FACING_UP;

	//added by Mike, 20201225
	bIsFiringBeam=false;
	
	//added by Mike, 20210126
    bIsExecutingDash=false, //removed by Mike, 20210128; added again by Mike, 20210805

    bIsDashReady=false;
	//edited by Mike, 20210128
//	iInputWaitCount=0;
	
	//added by Mike, 20210202
	for (int iCount=0; iCount<iNumOfKeyTypes; iCount++) {
		myKeysDown[iCount]=false;
	}	
	
	//added by Mike, 20210128
	for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
		bIsExecutingDashArray[iCount]=false;
		
		//added by Mike, 20210128
		iInputWaitCountArray[iCount]=0;
	}
	
    //edited by Mike, 20201201; edited by Mike, 20210502
//	currentFacingState=FACING_UP;
	currentFacingState=FACING_RIGHT;
    //added by Mike, 20210729
    prevFacingState=FACING_RIGHT;

	armAngles[LEFT] = 0.0;
	armAngles[RIGHT] = 0.0;
	legAngles[LEFT] = 0.0;
	legAngles[RIGHT] = 0.0;

	armStates[LEFT] = FORWARD_STATE;
	armStates[RIGHT] = BACKWARD_STATE;

	legStates[LEFT] = FORWARD_STATE;
	legStates[RIGHT] = BACKWARD_STATE;

	//removed by Mike, 20210423
/*	loadTexture(myBodyTexture, "bodyTexture.tga", &myBodyTextureObject);
	loadTexture(myHeadTexture, "headTexture.tga", &myHeadTextureObject);	
*/
	
	//added by Mike, 20210502; edited by Mike, 20210601
	//note: set this in OpenGLCanvas.cpp	
	bIsPlayer1=false;
	bIsPlayer1Partner=false;
	bIsPlayer2=false;
	bIsPlayer2Partner=false;	
	
	
	//removed by Mike, 20201001; added by Mike, 20210423
	setup();
	
    setCollidable(true);    
}

Pilot::~Pilot()
{
}

//added by Mike, 20210130
//TO-DO: -reverify: this
float* Pilot::getXYZPos()
{
      //float myXYZ[3];
      float* myXYZ;
      myXYZ = (float*) malloc(3*sizeof(float));

      myXYZ[0]=myXPos;
      myXYZ[1]=myYPos;
      myXYZ[2]=myZPos;
      
      return myXYZ;
}

/* //removed by Mike, 20201217
//added by Mike, 20201213
void Pilot::draw()
{
	drawRobotShip();
}
*/


//added by Mike, 20210423
void Pilot::drawPilotAsQuadWithTexture()
{
	//edited by Mike, 20210523
    //glTranslatef(myXPos, myYPos, myZPos);
//	printf(">> myXPos:%f, myYPos:%f, myZPos:%f;\n",myXPos,myYPos,myZPos);
		
/* //removed by Mike, 20210725
	//TO-DO: -update: this
	myXPosAsPixel=(int)myXPos;
	//edited by Mike, 20210725
*/
/* //removed by Mike, 20210608
void Pilot::drawPilotObjectPrevLargeTexture() 
{	
	//added by Mike, 20210422	
	glPushMatrix();
		//added by Mike, 20210420
		glColor3f(1.0f, 1.0f, 1.0f); // white
//		glColor3f(1.0f, 0.0f, 0.0f); // red
	
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);		
		
		//added by Mike, 20210523
		glRotatef(180, 1.0f, 0.0f, 0.0f);
		
		//edited by Mike, 20210515
//		fButtonAnimationFrameOffset=0;
	
		//added by Mike, 20210516; removed to after glScale(...) by Mike, 20210516		
		//due to instructions to auto-draw quad using triangles
//		glTranslatef(0.2f, 0.2f, 0.0f);
	
		//TO-DO: -verify: scaled texture object if equal with pixel width and height size
		//use autoConvertFromPixelToVertexPointX, et cetera if exact
		
		//window width and height; 640x640pixels
		//whole texture image sheet 512x256pixels
		//button size: 64x16pixels
//		glScalef(0.25f, 0.4f, 1.0f);		
		//edited by Mike, 20210523
//		glScalef(0.20f, 0.4f, 1.0f);		
//glScalef(3.2f, 3.2f, 3.2f);		
		glScalef(0.2f/2, 0.4f/2, 1.0f);		
		
		//added by Mike, 20210516
		//due to instructions to auto-draw quad using triangles
		glTranslatef(1.0f, 0.5f, 0.0f);		
	
if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
        //added by Mike, 20210424
        //notes: use folding paper to assist in quickly identifying location, e.g. texture coordinates
        //set vertex positions clock-wise
        //      glRotatef(45, 0.0f, 1.0f, 0.0f); //slanted to wall facing left
        glBegin(GL_TRIANGLES);
        //triangle#6 //back face left part
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A1; face left
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2
//        glVertex3f(-1.000000,4.000000,-1.000000); //A1
        glVertex3f(-1.000000,1.000000,-1.000000); //A1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //B1; face left
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C2
        glVertex3f(1.000000,-1.000000,-1.000000); //B1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C1; face left
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //B1
        glVertex3f(-1.000000,-1.000000,-1.000000); //C1
        
        //triangle#12 //back face right part
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A2; face lefT

        glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2
//        glVertex3f(-1.000000,4.000000,-1.000000); //A2
        glVertex3f(-1.000000,1.000000,-1.000000); //A2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2; face left
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A1
//        glVertex3f(1.000000,4.000000,-1.000000); //B2
        glVertex3f(1.000000,1.000000,-1.000000); //B2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //C2; face left
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C1
        glVertex3f(1.000000,-1.000000,-1.000000); //C2
        glEnd();
    }
    else {
        glBegin(GL_TRIANGLES);
        //triangle#6 //back face left part
        glNormal3f(0.0000,0.0000,-1.0000);
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);
//        glVertex3f(-1.000000,4.000000,-1.000000); //A1
        glVertex3f(-1.000000,1.000000,-1.000000); //A1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210420
        //	glTexCoord2f(1.0+iTaoAnimationFrameOffset,1.0);
        //	glTexCoord2f(0.5+fTaoAnimationFrameOffset,1.0);
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0);
        glVertex3f(1.000000,-1.000000,-1.000000); //B1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);
        glVertex3f(-1.000000,-1.000000,-1.000000); //C1
        
        
        //triangle#12 //back face right part
        glNormal3f(0.0000,0.0000,-1.0000);
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);
//        glVertex3f(-1.000000,4.000000,-1.000000); //A2
        glVertex3f(-1.000000,1.000000,-1.000000); //A2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210420
        //	glTexCoord2f(1.0+iTaoAnimationFrameOffset,0.0);
        //	glTexCoord2f(0.5+fTaoAnimationFrameOffset,0.0);
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0);
//        glVertex3f(1.000000,4.000000,-1.000000); //B2
        glVertex3f(1.000000,1.000000,-1.000000); //B2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210420	
        //	glTexCoord2f(1.0+iTaoAnimationFrameOffset,1.0);
        //	glTexCoord2f(0.5+fTaoAnimationFrameOffset,1.0);
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0);	
        glVertex3f(1.000000,-1.000000,-1.000000); //C2	
        glEnd();
    }
    	
	
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix(); //added by Mike, 20210422
}
*/

/* //removed by Mike, 20210727
//    myYPosAsPixel=(int)myZPos;
    myYPosAsPixel=(int)myYPos;
*/    
    
/*    
	//added by Mike, 20210523
	//printf("myYPos: %f",myYPos);
	printf(">>Pilot.cpp; myXPosAsPixel: %i\n",myXPosAsPixel);
	printf(">>Pilot.cpp; myYPosAsPixel: %i\n",myYPosAsPixel);
*/

	//edited by Mike, 20210523
//    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixel), myZPosAsPixel);
    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixel), 0);
		
	
//    printf(">>\n");
    
/* //removed by Mike, 20210521	
	glScalef(1.2f, 1.2f, 1.2f); //noticeable vertical movement; reverify scale to trans ratio	
	
	float fDistanceBetweenPlayer1And2 = sqrt((getX()-myOpponentXPos)*(getX()-myOpponentXPos));
	printf("fDistanceBetweenPlayer1And2: %f",fDistanceBetweenPlayer1And2);

	float fScale=1.0f+(1.0f-(fDistanceBetweenPlayer1And2/20.0f));

	//edited by Mike, 20210506IN_TITLE_STATE
	if (fScale>2.0f) {
		fScale=2.0f;
	}
		
  	//noticeable vertical movement		
	else if (fScale<1.2f) {
		fScale=1.2f;
	}

	//note: ratio: 1.0f : 0.375f; 2.0f : 0.75f
	//where: 1.0f = scale
	//0.375f = trans
	float fTrans= fScale*0.375f;
//		float fTrans= fScale*0.25f;

	printf("fScale: %f",fScale);		

	glScalef(fScale, fScale, fScale);			
	glTranslatef(0.0f, 0.0f, -fTrans); //negative to move backward in z-axis
*/	
	
	
    switch (currentState)
    {
/*
           case MOVING_STATE://DYING_STATE:
                glColor3f(1.0f, 1.0f, 1.0f); //white
                if (currentDeathFrame<5) {
                    switch(currentDeathFrame)
                    {
                      case 0:
                        drawMyPlane(myDeathAnimationImg1);
                        break;
                      case 1:
                        drawMyPlane(myDeathAnimationImg2);
                        break;
                      case 2:
                        drawMyPlane(myDeathAnimationImg3);
                        break;
                      case 3:
                        drawMyPlane(myDeathAnimationImg4);
                        break;
                      case 4:
                        drawMyPlane(myDeathAnimationImg5);
                        //changeState(MOVING_STATE);
                        break;
                    }
                }
                currentDeathFrame=(currentDeathFrame+1)%4;
                break;
*/
/* //removed by Mike, 20201014
           case INITIALIZING_STATE:
                if (invincibleCounter==10) {
                  changeState(MOVING_STATE);
                  setCollidable(true);
                }
                else invincibleCounter++;
*/
				
            case MOVING_STATE:
							switch(currentMovingState) {
		            		 case IDLE_MOVING_STATE:
							//added by Mike, 20210420
							//TO-DO: -add: this in key movement
							//note: 3 animation frames; .tga image file has 4 frames @128x256, i.e. width x height
							iCountTaoAnimationFrame=3;
                        						//edited by Mike, 20210609
						//                        fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.125; //0.25;
                        						fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.25;
                        							//removed by Mike, 20210610
						//                        iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;
												
												//added by Mike, 20210611
												//TO-DO: -add: identify which Unit Member hit the Ball
												//TO-DO: -add: image with kick
												
              					fTaoAnimationFrameOffsetYAxis=0.75f; //added by Mike, 20210611
	
													//added by Mike, 20210809
//												drawPilotObjectGlow();
																								
												//added by Mike, 20210810; //removed by Mike, 20210815
//												drawPilotObjectGlowFade();
												//with GlowFadePart1(), SFIII's GIL?
//												drawPilotObjectGlowFadePart1(); //added by Mike, 20210811
												
												//FACING_UP...
												drawPilotObject();
												
												//added by Mike, 20210809
//		     								drawShieldEffectAsQuadWithTexture();

												//added by Mike, 20210811
												//star power?; after drawPilotObject()
//												drawPilotObjectGlow();

												break;
											case WALKING_MOVING_STATE:
							//added by Mike, 20210420
							//TO-DO: -add: this in key movement
							//note: 3 animation frames; .tga image file has 4 frames @128x256, i.e. width x height
							iCountTaoAnimationFrame=(iCountTaoAnimationFrame)%3;
							
						//	fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.5;
                        						//edited by Mike, 20210609
						//                        fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.125; //0.25;
                        						fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.25;
                       						
							
						//	printf("iCountTaoAnimationFrame: %i",iCountTaoAnimationFrame);
							iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;		
							//printf("iTaoAnimationFrameOffset: %i",iTaoAnimationFrameOffset);
																		
              					fTaoAnimationFrameOffsetYAxis=0.75f; //added by Mike, 20210611
																	
                                    
                                    //added by Mike, 20210809
//                                    drawPilotObjectGlow();

												//added by Mike, 20210810; removed by Mike, 20210815
//												drawPilotObjectGlowFade();
												//with GlowFadePart1(), SFIII's GIL?												
												//drawPilotObjectGlowFadePart1(); //added by Mike, 20210811
                                    
												drawPilotObject();
												
												//added by Mike, 20210811
												//star power?; after drawPilotObject()												
//												drawPilotObjectGlow();
												
												
												//added by Mike, 20210805; edited by Mike, 20210805
												//drawAccelerationEffectAsQuadWithTexture();
                                    if (prevFacingState==currentFacingState) {
//                                        if (bIsExecutingDash) {
/*                                        if (bIsExecutingDashArray[KEY_D] ||
                                            bIsExecutingDashArray[KEY_A]) {
*/
                                        if (bIsExecutingDash) {
                                        }
                                        else {
                                            if (bIsExecutingDashArray[KEY_D] ||
                                                bIsExecutingDashArray[KEY_A]) {
                                                bIsExecutingDash=true;
                                            }

                                            if (bIsExecutingDash) {
                                                drawAccelerationEffectAsQuadWithTexture();
                                            }
                                        }
                                    }
                                    else {
                                        drawAccelerationEffectAsQuadWithTexture();
                                    }
                                    
												break;
												
												case ATTACKING_MOVING_STATE:
												
printf(">>>>>>>>>>>>>>> ATTACK DITO");												
              							//note: 3 animation frames; .tga image file has 4 frames @128x256, i.e. width x height
              							iCountTaoAnimationFrame=0;
              							fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.25;
              							fTaoAnimationFrameOffsetYAxis=0.25f; //added by Mike, 20210611
              							
							/*											//removed by Mike, 20210611
              							iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;
							*/                       
              							drawPilotObject(); //added by Mike, 20210603
              							break;
													}
                							break;
            						case IN_TITLE_STATE:
               						break;						
    						}    

	//removed by Mike, 20201001
//    glPopMatrix();	// pop back to original coordinate system
}

//added by Mike, 20210727
void Pilot::drawPilotObject()
{
    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);
    
    //added by Mike, 20210809;
    //edited by Mike, 20210811
    //note: remove: this to cause blank output in color white, et cetera
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //added by Mike, 20210803
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    
//    glColor3f(1.0f, 1.0f, 0.0f); //yellow
//    glColor3f(0.0f, 0.0f, 0.0f); //black; removes additional color
//    glColor3f(1.0f, 0.0f, 0.0f); //red

    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
/*    
    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
*/    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
		
		//added by Mike, 20210727        
    glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
    
    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

		//added by Mike, 20210724
		//background color of tile
//-----
/*
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
*/

/*	//removed by Mike, 20210727
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
*/   	
//-----		

		//added by Mike, 20210727
		//due flipped vertically
		glRotatef(180, 0.0f, 0.0f, 1.0f);		
		
    //note: vertex position sequence to be auto-drawn
    //vertex positions sequence: counter-clockwise sequence to auto-draw front face		
    //edited by Mike, 20210729
//    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
    if ((currentFacingState==FACING_RIGHT) ||
        (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
            (prevFacingState==FACING_RIGHT))
        || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {

        //note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f); //0.5f);
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();
    }
    //edited by Mike, 20210729
//    else if ((currentFacingState==FACING_LEFT) || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
    else if ((currentFacingState==FACING_LEFT) ||
             (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
              (prevFacingState==FACING_LEFT))
             || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
			//note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: counter-clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
//				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	            
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);   
        glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);     	
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();    	
		}
		//TO-DO: -add: facing_up, facing_down
		else {
		}

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}

//added by Mike, 20210809; edited by Mike, 20210810
void Pilot::drawPilotObjectGlow()
{
    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
//    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
//    glColor3f(1.0f, 0.67f, 0.0f); //gold
//    glColor3f(0.0f, 0.0f, 0.0f); //black
		//edited by Mike, 20210810
//    glColor3f(1.0f, 0.0f, 0.0f); //red

		iGlowEffectCount=(iGlowEffectCount+1)%2;
		if (iGlowEffectCount==0) {
    	glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //set to default, i.e. red
		}
		else {
    	glColor4f(1.0f, 0.33f, 0.0f, 0.60f); //orange; 60% transparent
		}


    //added by Mike, 20210809
    //https://stackoverflow.com/questions/4361023/opengl-es-1-1-how-to-change-texture-color-without-losing-luminance;
    //last accessed: 20210809T1402
    //answer by: Ozirus, 20101206T2236
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    
    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
/*    
    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
*/    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
		
		//added by Mike, 20210727        
    glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
    
    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

		//added by Mike, 20210724
		//background color of tile
//-----
/*
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
*/

/*	//removed by Mike, 20210727
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
*/   	
//-----		
		
		//added by Mike, 20210809
        glScalef(1.08f, 1.08f, 1.0f);

		//added by Mike, 20210727
		//due flipped vertically
		glRotatef(180, 0.0f, 0.0f, 1.0f);		

        //added by Mike, 20210809
        //note: FACING LEFT OR RIGHT, graphics library translate COMMAND equal
        glTranslatef(0.004f, 0.008f, 0.0f);

    //note: vertex position sequence to be auto-drawn
    //vertex positions sequence: counter-clockwise sequence to auto-draw front face		
    //edited by Mike, 20210729
//    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
    if ((currentFacingState==FACING_RIGHT) ||
        (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
            (prevFacingState==FACING_RIGHT))
        || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {

        //added by Mike, 20210809
//        glTranslatef(0.004f, 0.008f, 0.0f);

        //note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f); //0.5f);
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();
    }
    //edited by Mike, 20210729
//    else if ((currentFacingState==FACING_LEFT) || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
    else if ((currentFacingState==FACING_LEFT) ||
             (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
              (prevFacingState==FACING_LEFT))
             || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {

        
        //added by Mike, 20210809
//        glTranslatef(0.004f, 0.008f, 0.0f);

            //note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: counter-clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
//				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	            
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);   
        glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);     	
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();    	
		}
		//TO-DO: -add: facing_up, facing_down
		else {
		}

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}

//added by Mike, 20210810; edited by Mike, 20210811
void Pilot::drawPilotObjectGlowFadePart1()
{
    //added by Mike, 20210810
    // Disable AutoTexture Coordinates
/*    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
*/
    
//    glEnable(GL_TEXTURE_2D);                    // Enable 2D Texture Mapping
//    glDisable(GL_DEPTH_TEST);                   // Disable Depth Testing
/*
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);               // Set Blending Mode
    glEnable(GL_BLEND);                     // Enable Blending
*/
    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //added by Mike, 20210809
    //https://stackoverflow.com/questions/4361023/opengl-es-1-1-how-to-change-texture-color-without-losing-luminance;
    //last accessed: 20210809T1402
    //answer by: Ozirus, 20101206T2236
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    
    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
/*    
    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
*/    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
		
		int iGlowFadeEffectCountMax=10;

//      glScalef(1.08f, 1.08f, 1.0f);
		
        //reference: http://nehe.gamedev.net/tutorial/radial_blur__rendering_to_a_texture/18004/;
        //last accessed: 20210810
        //TO-DO: -reverify: this
		for (iGlowFadeEffectCount=0; iGlowFadeEffectCount<iGlowFadeEffectCountMax; iGlowFadeEffectCount++) {		
			//from red to orange to yellow
//    	glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //red
//    	glColor4f(1.0f, 0.0825f*iGlowFadeEffectCount, 0.0f, 1.0f);
            //edited by Mike, 20210810
            glColor4f(1.0f, 1.0f-0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
            //from yellow to orange to red
//            glColor4f(1.0f, 0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
            
			if (iGlowFadeEffectCount==0) {
    		glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);

      	glScalef(1.08f, 1.08f, 1.0f);
    	}
    					
			//edited by Mike, 20210810
//      glScalef(1.08f, 1.08f, 1.0f);
//      glScalef(1.08f*(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f)), 1.08f, 1.0f);
//      glScalef(1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f);
      glScalef(1.0f+(iGlowFadeEffectCount*0.001f), 1.0f+(iGlowFadeEffectCount*0.001f), 1.0f);
		
			if (iGlowFadeEffectCount==0) {
				//due flipped vertically
				glRotatef(180, 0.0f, 0.0f, 1.0f);			
			}

      //added by Mike, 20210809
      //note: FACING LEFT OR RIGHT, graphics library translate COMMAND equal
//TO-DO: -reverify: this
      glTranslatef(0.004f, 0.008f, 0.0f);
      glTranslatef(-0.0032f, -0.0072f, 0.0f);

/*
      glTranslatef(0.004f*iGlowFadeEffectCount, 0.008f*iGlowFadeEffectCount, 0.0f);
      glTranslatef(-0.0032f*iGlowFadeEffectCount, -0.0072f*iGlowFadeEffectCount, 0.0f);
*/

//removed by Mike, 20210810
//		}

    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

    //note: vertex position sequence to be auto-drawn
    //vertex positions sequence: counter-clockwise sequence to auto-draw front face		
    //edited by Mike, 20210729
//    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
    if ((currentFacingState==FACING_RIGHT) ||
        (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
            (prevFacingState==FACING_RIGHT))
        || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {

        //added by Mike, 20210809
//        glTranslatef(0.004f, 0.008f, 0.0f);

        //note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f); //0.5f);
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();
    }
    //edited by Mike, 20210729
//    else if ((currentFacingState==FACING_LEFT) || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
    else if ((currentFacingState==FACING_LEFT) ||
             (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
              (prevFacingState==FACING_LEFT))
             || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {

        
        //added by Mike, 20210809
//        glTranslatef(0.004f, 0.008f, 0.0f);

            //note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: counter-clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
//				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	            
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);   
        glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);     	
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();    	
		}
		//TO-DO: -add: facing_up, facing_down
		else {
		}

    //added by Mike, 20210810
    }
            
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}

//added by Mike, 20210811
void Pilot::drawPilotObjectGlowFade()
{
    //added by Mike, 20210810
    // Disable AutoTexture Coordinates
    /*    glDisable(GL_TEXTURE_GEN_S);
     glDisable(GL_TEXTURE_GEN_T);
     */
    
    //    glEnable(GL_TEXTURE_2D);                    // Enable 2D Texture Mapping
    //    glDisable(GL_DEPTH_TEST);                   // Disable Depth Testing
    /*
     glBlendFunc(GL_SRC_ALPHA,GL_ONE);               // Set Blending Mode
     glEnable(GL_BLEND);                     // Enable Blending
     */
    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //added by Mike, 20210809
    //https://stackoverflow.com/questions/4361023/opengl-es-1-1-how-to-change-texture-color-without-losing-luminance;
    //last accessed: 20210809T1402
    //answer by: Ozirus, 20101206T2236
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
    //    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    
    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625
    
    glPushMatrix();
    /*
     printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
     printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
     */
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
    //    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
    
    int iGlowFadeEffectCountMax=5;//3;//5; //10;
    
    //      glScalef(1.08f, 1.08f, 1.0f);
    
    //reference: http://nehe.gamedev.net/tutorial/radial_blur__rendering_to_a_texture/18004/;
    //last accessed: 20210810
    //TO-DO: -reverify: this
    
		for (int iCount=0; iCount<4; iCount++) {
		    
//            glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iCount*0.2f);
            
    	for (iGlowFadeEffectCount=0; iGlowFadeEffectCount<iGlowFadeEffectCountMax; iGlowFadeEffectCount++) {
        	//from red to orange to yellow
        	//    	glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //red
        	//    	glColor4f(1.0f, 0.0825f*iGlowFadeEffectCount, 0.0f, 1.0f);
        	//edited by Mike, 20210810
	//        glColor4f(1.0f, 1.0f-0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
	//        glColor4f(1.0f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
	//        glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
            //edited by Mike, 20210812
//        	glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
            //from orange to red; TO-DO: -reverify: due to iCount does not start from inside
            if (iCount<2) {
                glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
            }
            else {
                glColor4f(1.0f, 0.3f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
            }
	
        	
        	//from yellow to orange to red
        	//            glColor4f(1.0f, 0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
        	
        	if ((iGlowFadeEffectCount==0) and (iCount==0)){
            	glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);            	
            	glScalef(1.08f, 1.08f, 1.0f);
        	}
        	
        	//edited by Mike, 20210810
        	//      glScalef(1.08f, 1.08f, 1.0f);
        	//      glScalef(1.08f*(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f)), 1.08f, 1.0f);
        	//      glScalef(1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f);
        	glScalef(1.0f+(iGlowFadeEffectCount*0.001f), 1.0f+(iGlowFadeEffectCount*0.001f), 1.0f);
        	
        	if ((iGlowFadeEffectCount==0) and (iCount==0)) {
            	//due flipped vertically
            	glRotatef(180, 0.0f, 0.0f, 1.0f);
        	}
        	
		  		        	
        	//added by Mike, 20210809
        	//note: FACING LEFT OR RIGHT, graphics library translate COMMAND equal
        	//TO-DO: -reverify: this
        	//edited by Mike, 20210811
	//        glTranslatef(0.004f, 0.008f, 0.0f);
	//      glTranslatef(0.002f, 0.002f, 0.0f);
	//        glTranslatef(0.002f, -0.008f, 0.0f);
        	
        	int iDirection=1;

        	if (iCount%2!=0) {
        		iDirection=iDirection*(-1);
        	}
        	
//        	glTranslatef(0.002f*iDirection, 0.0f, 0.0f);

        	//glTranslatef(0.002f*iDirection, 0.002f, 0.0f);
//					if ((iGlowFadeEffectCount==0) and (iCount==0)){

            if ((iGlowFadeEffectCount==0)){
//        		glTranslatef(0.002f*iDirection, 0.002f, 0.0f);
        		glTranslatef(0.002f*iDirection, 0.002f, 0.0f);
            }

//            glTranslatef(0.002f*iDirection*iGlowFadeEffectCount, 0.002f*iDirection*iGlowFadeEffectCount, 0.0f);


        	//even number
        	if (iGlowFadeEffectCount%2==0) {
	//            glTranslatef(0.002f, -0.008f, 0.0f);
                //edited by Mike, 20210812
//            	glTranslatef(0.002f*iDirection, 0.002f*(iDirection*(-1)), 0.0f);
                glTranslatef((0.002f*(iCount+1))*iDirection, (0.002f*(iCount+1))*(iDirection*(-1)), 0.0f);
        	}
        	//NOT even number
        	else {
	//            glTranslatef(0.002f, 0.008f, 0.0f);
                //edited by Mike, 20210812
//            	glTranslatef(0.002f*iDirection, -0.002f*(iDirection*(-1)), 0.0f);
                glTranslatef((0.002f*(iCount+1))*iDirection, (-0.002f*(iCount+1))*(iDirection*(-1)), 0.0f);
            }

            
        	//removed by Mike, 20210811
	//        glTranslatef(-0.0032f, -0.0072f, 0.0f);
	
        	
        	/*
         	glTranslatef(0.004f*iGlowFadeEffectCount, 0.008f*iGlowFadeEffectCount, 0.0f);
         	glTranslatef(-0.0032f*iGlowFadeEffectCount, -0.0072f*iGlowFadeEffectCount, 0.0f);
         	*/
        	
        	//removed by Mike, 20210810
        	//		}
        	
        	float fTx = 0.0f;
        	float fTy = 0.0f;
        	
        	float fTileSideXAxis = 0.0625f;
        	//from bottom; anchor; start fTy at 1.0f
        	float fTileSideYAxis = -0.0625f;
        	
        	//note: vertex position sequence to be auto-drawn
        	//vertex positions sequence: counter-clockwise sequence to auto-draw front face
        	//edited by Mike, 20210729
        	//    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
        	if ((currentFacingState==FACING_RIGHT) ||
            	(((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
             	(prevFacingState==FACING_RIGHT))
            	|| (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
            	
            	//added by Mike, 20210809
            	//        glTranslatef(0.004f, 0.008f, 0.0f);
            	//added by Mike, 20210811
            	//flash blur effect; shadow?
	//            glTranslatef(0.0000032f*iGlowFadeEffectCount, 0.0000000072f*iGlowFadeEffectCount, 0.0f);
	//            glTranslatef(0.000001f*iGlowFadeEffectCount, 0.000001f*iGlowFadeEffectCount, 0.0f);
	//            glTranslatef(0.000001f*iGlowFadeEffectCount, 0.0f, 0.0f);
            	
            	
            	//note: vertex positions sequence: counter-clock-wise
            	//note:texture positions sequence: clock-wise
            	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
            	//glTexCoord2f(fTx, fTy);
            	glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glVertex3f(0.0f, 0.0f, 0.0f);
            	
            	//glTexCoord2f(fTx + fTileSideXAxis, fTy);
            	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
            	
            	//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
            	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            	
            	//glTexCoord2f(fTx, fTy + fTileSideYAxis);
            	glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f); //0.5f);
            	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            	glEnd();
        	}
        	//edited by Mike, 20210729
        	//    else if ((currentFacingState==FACING_LEFT) || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
        	else if ((currentFacingState==FACING_LEFT) ||
                 	(((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
                  	(prevFacingState==FACING_LEFT))
                 	|| (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
            	
            	
            	//added by Mike, 20210809
            	//        glTranslatef(0.004f, 0.008f, 0.0f);
            	
            	//note: vertex positions sequence: counter-clock-wise
            	//note:texture positions sequence: counter-clock-wise
            	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
            	//glTexCoord2f(fTx, fTy);
            	//				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glVertex3f(0.0f, 0.0f, 0.0f);
            	
            	//glTexCoord2f(fTx + fTileSideXAxis, fTy);
            	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
            	
            	//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
            	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            	glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            	
            	//glTexCoord2f(fTx, fTy + fTileSideYAxis);
            	//glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);     	
            	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            	glEnd();    	
        	}
        	//TO-DO: -add: facing_up, facing_down
        	else {
        	}
        	
        	//added by Mike, 20210810
    	}
    }
    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}


//added by Mike, 20210811; removed by Mike, 20210811
//TO-DO: -reverify: glow fade computer instructions
/*
void Pilot::drawPilotObjectGlowFade()
{
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //added by Mike, 20210809
    //https://stackoverflow.com/questions/4361023/opengl-es-1-1-how-to-change-texture-color-without-losing-luminance;
    //last accessed: 20210809T1402
    //answer by: Ozirus, 20101206T2236
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
    //    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    
    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625
    
    glPushMatrix();
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
    //    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
    
    int iGlowFadeEffectCountMax=5; //10;
    
    //      glScalef(1.08f, 1.08f, 1.0f);
    
    //reference: http://nehe.gamedev.net/tutorial/radial_blur__rendering_to_a_texture/18004/;
    //last accessed: 20210810
    //TO-DO: -reverify: this
    for (iGlowFadeEffectCount=0; iGlowFadeEffectCount<iGlowFadeEffectCountMax; iGlowFadeEffectCount++) {
        //from red to orange to yellow
        //    	glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //red
        //    	glColor4f(1.0f, 0.0825f*iGlowFadeEffectCount, 0.0f, 1.0f);
        //edited by Mike, 20210810
        //        glColor4f(1.0f, 1.0f-0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
        //        glColor4f(1.0f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
        //        glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
        glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
        
        
        //from yellow to orange to red
        //            glColor4f(1.0f, 0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
        
        if (iGlowFadeEffectCount==0) {
            glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
            
            glScalef(1.08f, 1.08f, 1.0f);
        }
        
        //edited by Mike, 20210810
        //      glScalef(1.08f, 1.08f, 1.0f);
        //      glScalef(1.08f*(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f)), 1.08f, 1.0f);
        //      glScalef(1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f);
        glScalef(1.0f+(iGlowFadeEffectCount*0.001f), 1.0f+(iGlowFadeEffectCount*0.001f), 1.0f);
        
        if (iGlowFadeEffectCount==0) {
            //due flipped vertically
            glRotatef(180, 0.0f, 0.0f, 1.0f);
        }
        
        //added by Mike, 20210809
        //note: FACING LEFT OR RIGHT, graphics library translate COMMAND equal
        //TO-DO: -reverify: this
        //edited by Mike, 20210811
        //        glTranslatef(0.004f, 0.008f, 0.0f);
        //      glTranslatef(0.002f, 0.002f, 0.0f);
        //        glTranslatef(0.002f, -0.008f, 0.0f);
        glTranslatef(0.002f, 0.0f, 0.0f);
        
        //even number
        if (iGlowFadeEffectCount%2==0) {
            //            glTranslatef(0.002f, -0.008f, 0.0f);
            glTranslatef(0.002f, 0.002f, 0.0f);
        }
        //NOT even number
        else {
            //            glTranslatef(0.002f, 0.008f, 0.0f);
            glTranslatef(0.002f, -0.002f, 0.0f);
        }
        
        //removed by Mike, 20210811
        //        glTranslatef(-0.0032f, -0.0072f, 0.0f);
        
        //removed by Mike, 20210810
        //		}
        
        float fTx = 0.0f;
        float fTy = 0.0f;
        
        float fTileSideXAxis = 0.0625f;
        //from bottom; anchor; start fTy at 1.0f
        float fTileSideYAxis = -0.0625f;
        
        //note: vertex position sequence to be auto-drawn
        //vertex positions sequence: counter-clockwise sequence to auto-draw front face
        //edited by Mike, 20210729
        //    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
        if ((currentFacingState==FACING_RIGHT) ||
            (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
             (prevFacingState==FACING_RIGHT))
            || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
            
            //added by Mike, 20210809
            //        glTranslatef(0.004f, 0.008f, 0.0f);
            //added by Mike, 20210811
            //flash blur effect; shadow?
            //            glTranslatef(0.0000032f*iGlowFadeEffectCount, 0.0000000072f*iGlowFadeEffectCount, 0.0f);
            //            glTranslatef(0.000001f*iGlowFadeEffectCount, 0.000001f*iGlowFadeEffectCount, 0.0f);
            //            glTranslatef(0.000001f*iGlowFadeEffectCount, 0.0f, 0.0f);
            
            
            //note: vertex positions sequence: counter-clock-wise
            //note:texture positions sequence: clock-wise
            glBegin(GL_QUADS); // Each set of 4 vertices form a quad
            //glTexCoord2f(fTx, fTy);
            glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glVertex3f(0.0f, 0.0f, 0.0f);
            
            //glTexCoord2f(fTx + fTileSideXAxis, fTy);
            glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
            
            //glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
            glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            
            //glTexCoord2f(fTx, fTy + fTileSideYAxis);
            glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f); //0.5f);
            glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            glEnd();
        }
        //edited by Mike, 20210729
        //    else if ((currentFacingState==FACING_LEFT) || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
        else if ((currentFacingState==FACING_LEFT) ||
                 (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
                  (prevFacingState==FACING_LEFT))
                 || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
            
            
            //added by Mike, 20210809
            //        glTranslatef(0.004f, 0.008f, 0.0f);
            
            //note: vertex positions sequence: counter-clock-wise
            //note:texture positions sequence: counter-clock-wise
            glBegin(GL_QUADS); // Each set of 4 vertices form a quad
            //glTexCoord2f(fTx, fTy);
            //				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glVertex3f(0.0f, 0.0f, 0.0f);
            
            //glTexCoord2f(fTx + fTileSideXAxis, fTy);
            //glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
            
            //glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
            //glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            
            //glTexCoord2f(fTx, fTy + fTileSideYAxis);
            //glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            glEnd();
        }
        //TO-DO: -add: facing_up, facing_down
        else {
        }
        
        //added by Mike, 20210810
    }
    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}
*/

//added by Mike, 20210805
void Pilot::drawAccelerationEffectAsQuadWithTexture()
{
/*	//removed by Mike, 20210805    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);
*/    
    //added by Mike, 20210803
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white

    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
    
    printf("drawAccelerationEffectAsQuadWithTexture\n");
    
    //TO-DO: -update: this
/*    
    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
*/
    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -

/*		
		//added by Mike, 20210727        
    glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
*/
    
    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

 		//put at back of Pilot's feet
/* 		
    if ((currentFacingState==FACING_RIGHT) ||
        (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
            (prevFacingState==FACING_RIGHT))
        || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {

    }
    else if ((currentFacingState==FACING_LEFT) ||
             (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
              (prevFacingState==FACING_LEFT))
             || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
             
         glTranslatef(0.0f+fGridTileWidthVertexPosition*2, 0.0f, 0.0f);
		}		
		else if (currentFacingState==FACING_UP) {
		}
		else if (currentFacingState==FACING_DOWN) {
		}
*/

    //added by Mike, 20210724
    //background color of tile
    //-----
    /*
     glDisable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, 0);
     */
    
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    //    glColor3f(1.0f, 0.0f, 0.0f); //red

    //added by Mike, 20210805
    if (currentFacingState==FACING_RIGHT) {
        //added by Mike, 20210805
        //note: remove when drawing quad, instead of circle
        //note: circle radius 0.25f; OK; reminder: anchor top-left
        //        glTranslatef(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
        //        glTranslatef(0.0f-fGridTileWidthVertexPosition/2.0f, 0.0f, 0.0f);
        
        glTranslatef(0.0f+fGridTileWidthVertexPosition/2.0f, 0.0f-fGridTileHeightVertexPosition/1.1f, 0.0f);
    }
    else if (currentFacingState==FACING_LEFT) {
        //note: circle radius 0.25f; OK; reminder: anchor top-left
        //glTranslatef(0.0f+fGridTileWidthVertexPosition*2, 0.0f, 0.0f);
        //note: y-axis:  0.0f-fGridTileHeightVertexPosition/2.0f at middle
//        glTranslatef(0.0f+fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition/1.1f, 0.0f);
        glTranslatef(0.0f+fGridTileWidthVertexPosition/2.0f, 0.0f-fGridTileHeightVertexPosition/1.1f, 0.0f);
    }
    
    //Reference: https://stackoverflow.com/questions/46258919/drawing-circle-with-gl-polygon-radius-out-of-scale;
    //question by: emic, 20170916T2216;
    //edited by: Rabbid76, 20180531T1930
    //answer by: Rabbid76, 20170917T0735
    
    //TO-DO: -reverify: this due to drawn shape ellipse, not circle
    //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    //glOrtho( -myWindowWidth/2.0f, myWindowWidth/2.0f, -myWindowHeight/2.0f, myWindowHeight/2.0f, -1.0, 1.0 );
    //note: -reverify: anchor; facing right at center, facing left at back; trailing
  
/* //edited by Mike, 20210815    
    printf("myWindowWidth: %i\n",myWindowWidth); //example output: 1366
    printf("myWindowHeight: %i\n",myWindowHeight); //example output: 768
*/    
    //		glScalef(0.56f, 1.0f, 1.0f);
    printf("fMyWindowWidth: %f\n",fMyWindowWidth); //example output: 1366
    printf("fMyWindowHeight: %f\n",fMyWindowHeight); //example output: 768

    
    //TO-DO: -add: auto-identify if computer monitor rectangle, i.e. NOT square;
    //AND which side is longer
    //edited by Mike, 20210815
//    glScalef(myWindowHeight/(myWindowWidth*1.0f), 1.0f, 1.0f);
    glScalef(fMyWindowHeight/(fMyWindowWidth*1.0f), 1.0f, 1.0f);
    
//--

    //TO-DO: -add: in loop, increasing size AND movement
for (int iCount=0; iCount<3; iCount++) {
    
    if (currentFacingState==FACING_RIGHT) {
        glTranslatef(0.0f-fGridTileWidthVertexPosition/4.0f, 0.0f, 0.0f);
    }
    else if (currentFacingState==FACING_LEFT) {
        glTranslatef(0.0f+fGridTileWidthVertexPosition/4.0f, 0.0f, 0.0f);
    }
	else {
	  	glPopMatrix();
	  	return;
	}
    
/*    
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
*/   	

        //added by Mike, 20210805
        //glScalef(1.0f*(iCount+1), 1.0f*(iCount+1), 1.0f);
        //glScalef(0.5f*(iCount+1), 0.5f*(iCount+1), 1.0f);
        glScalef(0.6f*(iCount+1), 0.6f*(iCount+1), 1.0f);
    
		float fCircleCenterX=0.0f;
		float fCircleCenterY=0.0f;
		float fPI=3.14f;
        float fRadius=0.02f;//0.25f; //0.1f; //0.25f;
				
		//draw ellipse
    glBegin(GL_POLYGON);
        for(double i = 0; i < 2 * fPI; i += fPI / 24) {
            glVertex3f((cos(i) * fRadius) + fCircleCenterX,(sin(i) * fRadius) + fCircleCenterY, 0.0);
        }
    glEnd();   	
   			
		//reset scaled shape
		glScalef(1.0f, 1.0f, 1.0f);  	
}
//-----		


/*	//removed by Mike, 20210805    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
*/
    
    glPopMatrix();
}

//added by Mike, 20210809
//sonic shield?
void Pilot::drawShieldEffectAsQuadWithTexture()
{
/*	//removed by Mike, 20210805    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);
*/    
    //added by Mike, 20210803
//    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
		//note: alpha last input parameter
//    glColor4f(0.0f, 1.0f, 0.85f, 0.80f); //set to default, i.e. green + blue; 80% transparent
		//note: fully charged to be ON; red charging; from Android
		//reminder: additional computation due to transparent effect
    glColor4f(0.18f, 1.0f, 0.16f, 0.60f); //set to default, i.e. lime green; 60% transparent

		iShieldEffectCount=(iShieldEffectCount+1)%2;
		if (iShieldEffectCount==0) {
			return;
		}



    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
    
    printf("drawAccelerationEffectAsQuadWithTexture\n");
    
    //TO-DO: -update: this
/*    
    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
*/
    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -

/*		
		//added by Mike, 20210727        
    glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
*/
    
    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

    //added by Mike, 20210724
    //background color of tile
    //-----
    /*
     glDisable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, 0);
     */
    
    //added by Mike, 20210809
    glTranslatef(0.0f, 0.0f+fGridTileHeightVertexPosition/3.0f, 0.0f);

    //added by Mike, 20210805
    if (currentFacingState==FACING_RIGHT) {
        //added by Mike, 20210805
        //note: remove when drawing quad, instead of circle
        //note: circle radius 0.25f; OK; reminder: anchor top-left
        //        glTranslatef(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
        //        glTranslatef(0.0f-fGridTileWidthVertexPosition/2.0f, 0.0f, 0.0f);
        
        glTranslatef(0.0f+fGridTileWidthVertexPosition/2.0f, 0.0f-fGridTileHeightVertexPosition/1.1f, 0.0f);
    }
    else if (currentFacingState==FACING_LEFT) {
        //note: circle radius 0.25f; OK; reminder: anchor top-left
        //glTranslatef(0.0f+fGridTileWidthVertexPosition*2, 0.0f, 0.0f);
        //note: y-axis:  0.0f-fGridTileHeightVertexPosition/2.0f at middle
//        glTranslatef(0.0f+fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition/1.1f, 0.0f);
        glTranslatef(0.0f+fGridTileWidthVertexPosition/2.0f, 0.0f-fGridTileHeightVertexPosition/1.1f, 0.0f);
    }
    
    //Reference: https://stackoverflow.com/questions/46258919/drawing-circle-with-gl-polygon-radius-out-of-scale;
    //question by: emic, 20170916T2216;
    //edited by: Rabbid76, 20180531T1930
    //answer by: Rabbid76, 20170917T0735
    
    //TO-DO: -reverify: this due to drawn shape ellipse, not circle
    //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    //glOrtho( -myWindowWidth/2.0f, myWindowWidth/2.0f, -myWindowHeight/2.0f, myWindowHeight/2.0f, -1.0, 1.0 );
    //note: -reverify: anchor; facing right at center, facing left at back; trailing
/*    
    printf("myWindowWidth: %i\n",myWindowWidth); //example output: 1366
    printf("myWindowHeight: %i\n",myWindowHeight); //example output: 768
    //		glScalef(0.56f, 1.0f, 1.0f);
*/
    
    //TO-DO: -add: auto-identify if computer monitor rectangle, i.e. NOT square;
    //AND which side is longer
    //edited by Mike, 20210815
//    glScalef(myWindowHeight/(myWindowWidth*1.0f), 1.0f, 1.0f);
    glScalef(fMyWindowHeight/(fMyWindowWidth*1.0f), 1.0f, 1.0f);
    
//--
/*
    //TO-DO: -add: in loop, increasing size AND movement
for (int iCount=0; iCount<3; iCount++) {
    
    if (currentFacingState==FACING_RIGHT) {
        glTranslatef(0.0f-fGridTileWidthVertexPosition/4.0f, 0.0f, 0.0f);
    }
    else if (currentFacingState==FACING_LEFT) {
        glTranslatef(0.0f+fGridTileWidthVertexPosition/4.0f, 0.0f, 0.0f);
    }
	else {
	  	glPopMatrix();
	  	return;
	}
*/
    
/*    
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
*/   	

        //added by Mike, 20210805
        //glScalef(1.0f*(iCount+1), 1.0f*(iCount+1), 1.0f);
        //glScalef(0.5f*(iCount+1), 0.5f*(iCount+1), 1.0f);
/*        glScalef(0.6f*(iCount+1), 0.6f*(iCount+1), 1.0f);
*/    
		float fCircleCenterX=0.0f;
		float fCircleCenterY=0.0f;
		float fPI=3.14f;
    float fRadius=0.2f; //0.25f; //0.02f;//0.25f; //0.1f; //0.25f;
				
		//draw ellipse
    glBegin(GL_POLYGON);
        for(double i = 0; i < 2 * fPI; i += fPI / 24) {
            glVertex3f((cos(i) * fRadius) + fCircleCenterX,(sin(i) * fRadius) + fCircleCenterY, 0.0);
        }
    glEnd();   	
   			
		//reset scaled shape
		glScalef(1.0f, 1.0f, 1.0f);		
/*   	
}*/
//-----		


/*	//removed by Mike, 20210805    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
*/
    
    glPopMatrix();
}


/*
//added by Mike, 20210805
//Tile Block delivery trailer?
void Pilot::drawAccelerationEffectAsQuadWithTextureTileBlock()
{
    //added by Mike, 20210803
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white

    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
    
    printf("drawAccelerationEffectAsQuadWithTexture\n");
    
    //TO-DO: -update: this
    
//    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
//    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000

    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
    
    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

		//added by Mike, 20210724
		//background color of tile

//    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    glColor3f(1.0f, 0.0f, 0.0f); //red
    
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
   	
//-----		

    glPopMatrix();
}
*/

//added: by Mike, 20210423
//TO-DO: -add: in PolygonPool
//note: image textured quad as 3D object bends as its positions go outward from center
//edited by Mike, 20210523
void Pilot::drawPilotObjectWith3DVertexPositions()
{	
	//added by Mike, 20210422	
	glPushMatrix();
		//added by Mike, 20210420
		glColor3f(1.0f, 1.0f, 1.0f); // white
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);		

	//	glTranslatef(30.0f, 0.0f, 25.0f); //removed by Mike, 20210424
		glRotatef(180, 1.0f, 0.0f, 0.0f);

		//added by Mike, 20210522
		//TO-DO: -reverify: cause of blurry image texture output
		//added by Mike, 20210523
		//TO-DO: -reverify: Pilot movement, e.g. horizontally, vertically, due to not exact with grid coordinate
	
		//added by Mike, 20210422; edited by Mike, 20210424
		//TO-DO: -reverify: quad size and texture size
	//    glScalef(1.5f, 1.5f, 1.0f);	
		//edited by Mike, 20210521
	//  glScalef(1.2f, 2.0f, 1.0f);	
		//edited by Mike, 20210522
        //edited by Mike, 20210523
//    glScalef(5.0f, 5.0f, 1.0f);
//        glScalef(10.0f, 10.0f, 1.0f);
    glScalef(3.2f, 3.2f, 3.2f);
    
	//    glScalef(10.f, 10.0f, 1.0f);	
//		glScalef(20.0f, 20.0f, 1.0f);	

	/*	//removed by Mike, 20210423	
		//edited by Mike, 20210420
		//note: 3 animation frames; .tga image file has 4 frames @128x256, i.e. width x height
	//	iCountTaoAnimationFrame=(iCountTaoAnimationFrame)%2;
		iCountTaoAnimationFrame=(iCountTaoAnimationFrame)%3;

	//	fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.5;
		fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.25;

		printf("iCountTaoAnimationFrame: %i",iCountTaoAnimationFrame);

		iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;		
		//printf("iTaoAnimationFrameOffset: %i",iTaoAnimationFrameOffset);
	*/	


		//added by Mike, 20210424
	/*    if (currentFacingState==FACING_LEFT) 
		{  
		} 
		else */
	//edited by Mike, 20210425
	/*	if (currentFacingState==FACING_RIGHT) {    
	*/
	//TO-DO: -update: this
    
    //added by Mike, 20210523
    //TO-DO: -fix: problem with objects, e.g. Pilot, bend if position not at center
    /* //edited by Mike, 20210523
     //note: vertex coordinate positions to create quad using triangles not exact with input image file causes blurry output
     //image textured quad scale size also causes blurry output if set to be too small
    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
			//added by Mike, 20210424
			//notes: use folding paper to assist in quickly identifying location, e.g. texture coordinates 
			//set vertex positions clock-wise
	//      glRotatef(45, 0.0f, 1.0f, 0.0f); //slanted to wall facing left
			glBegin(GL_TRIANGLES);	
				//triangle#6 //back face left part
				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424
	//			glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A1; face left
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2		
				glVertex3f(-1.000000,1.000000,-1.000000); //A1

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424		
	//			glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //B1; face left
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C2
				glVertex3f(1.000000,-1.000000,-1.000000); //B1

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424				
	//			glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C1; face left
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //B1 
				glVertex3f(-1.000000,-1.000000,-1.000000); //C1	

				//triangle#12 //back face right part		
				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424
	//			glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A2; face lefT
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2		
				glVertex3f(-1.000000,1.000000,-1.000000); //A2

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424
	//			glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2; face left
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A1
				glVertex3f(1.000000,1.000000,-1.000000); //B2

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424
	//			glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //C2; face left
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C1	
				glVertex3f(1.000000,-1.000000,-1.000000); //C2	
			glEnd();
		}
		else {	
			glBegin(GL_TRIANGLES);	
				//triangle#6 //back face left part
				glNormal3f(0.0000,0.0000,-1.0000);
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	
				glVertex3f(-1.000000,1.000000,-1.000000); //A1

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210420
			//	glTexCoord2f(1.0+iTaoAnimationFrameOffset,1.0);
			//	glTexCoord2f(0.5+fTaoAnimationFrameOffset,1.0);
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0);
				glVertex3f(1.000000,-1.000000,-1.000000); //B1

				glNormal3f(0.0000,0.0000,-1.0000);
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	
				glVertex3f(-1.000000,-1.000000,-1.000000); //C1	


				//triangle#12 //back face right part		
				glNormal3f(0.0000,0.0000,-1.0000);
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	
				glVertex3f(-1.000000,1.000000,-1.000000); //A2

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210420	
			//	glTexCoord2f(1.0+iTaoAnimationFrameOffset,0.0);
			//	glTexCoord2f(0.5+fTaoAnimationFrameOffset,0.0);
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0);
				glVertex3f(1.000000,1.000000,-1.000000); //B2

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210420	
			//	glTexCoord2f(1.0+iTaoAnimationFrameOffset,1.0);
			//	glTexCoord2f(0.5+fTaoAnimationFrameOffset,1.0);
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0);	
				glVertex3f(1.000000,-1.000000,-1.000000); //C2	
			glEnd();
		}	
*/
    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
        //added by Mike, 20210424
        //notes: use folding paper to assist in quickly identifying location, e.g. texture coordinates
        //set vertex positions clock-wise
        //      glRotatef(45, 0.0f, 1.0f, 0.0f); //slanted to wall facing left
        glBegin(GL_TRIANGLES);
        //triangle#6 //back face left part
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A1; face left
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2
        glVertex3f(-1.000000,4.000000,-1.000000); //A1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //B1; face left
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C2
        glVertex3f(1.000000,-1.000000,-1.000000); //B1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C1; face left
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //B1
        glVertex3f(-1.000000,-1.000000,-1.000000); //C1
        
        //triangle#12 //back face right part
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A2; face lefT
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2
        glVertex3f(-1.000000,4.000000,-1.000000); //A2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2; face left
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A1
        glVertex3f(1.000000,4.000000,-1.000000); //B2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //C2; face left
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C1
        glVertex3f(1.000000,-1.000000,-1.000000); //C2
        glEnd();
    }
    else {
        glBegin(GL_TRIANGLES);
        //triangle#6 //back face left part
        glNormal3f(0.0000,0.0000,-1.0000);
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);
        glVertex3f(-1.000000,4.000000,-1.000000); //A1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210420
        //	glTexCoord2f(1.0+iTaoAnimationFrameOffset,1.0);
        //	glTexCoord2f(0.5+fTaoAnimationFrameOffset,1.0);
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0);
        glVertex3f(1.000000,-1.000000,-1.000000); //B1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);
        glVertex3f(-1.000000,-1.000000,-1.000000); //C1
        
        
        //triangle#12 //back face right part
        glNormal3f(0.0000,0.0000,-1.0000);
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);
        glVertex3f(-1.000000,4.000000,-1.000000); //A2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210420
        //	glTexCoord2f(1.0+iTaoAnimationFrameOffset,0.0);
        //	glTexCoord2f(0.5+fTaoAnimationFrameOffset,0.0);
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0);
        glVertex3f(1.000000,4.000000,-1.000000); //B2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210420	
        //	glTexCoord2f(1.0+iTaoAnimationFrameOffset,1.0);
        //	glTexCoord2f(0.5+fTaoAnimationFrameOffset,1.0);
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0);	
        glVertex3f(1.000000,-1.000000,-1.000000); //C2	
        glEnd();
    }
    
    
		//added by Mike, 20210422
		glScalef(1.0f, 1.0f, 1.0f);	

		glRotatef(-180, 1.0f, 0.0f, 0.0f);
	//	glTranslatef(-30.0f, 0.0f, -25.0f); //removed by Mike, 20210424

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix(); //added by Mike, 20210422	
}

void Pilot::updatePrev(float dt)
{
    switch (currentState)
    {
           case INITIALIZING_STATE:
           case MOVING_STATE:      
				switch(currentMovingState) {
		           case WALKING_MOVING_STATE:
		           		//added by Mike, 20201227; edited by Mike, 20201229
						//TO-DO: -reverify: that angles are correct in WALKING_STATE
						//TO-DO: -reverify: sequence of upper and lower arms 
						if ((armAngles[LEFT]==0) && (armAngles[RIGHT]==0)) {
							armAngles[LEFT] = 8.3f;
							armAngles[RIGHT] = -10.0f*3-5.0f;
							legAngles[LEFT] = 0.0f;
							legAngles[RIGHT] = 0.0f;		    
							
							//added by Mike, 20201229
							armStates[0] == FORWARD_STATE;        
							armStates[1] == BACKWARD_STATE;        
						}
		           		
		           	
		                //note: Code structure taken from Dave Astle and Kevin Hawkins's code 
		                //("Beginning OpenGL Game Programming", Chapter 4)
		                //-Mike, Dec. 21, 2006
		            	// if leg is moving forward, increase angle, else decrease angle
		            	for (char side = 0; side < 2; side++)
		            	{
		            		// arms
/* 	//edited by Mike, 20201204							
		            		if (armStates[side] == FORWARD_STATE)
		            			armAngles[side] += 25.0f * dt;//20.0f * dt;
		            		else
		            			armAngles[side] -= 30.0f * dt;//20.0f * dt;
*/
		            		if (armStates[side] == FORWARD_STATE)
		            			armAngles[side] += 8.3f * dt;//20.0f * dt;
		            		else
		            			armAngles[side] -= 10.0f * dt;//20.0f * dt;
							
		            		// change state if exceeding angles
		            		if (armAngles[side] >= 25.0f) //15.0f
		            			armStates[side] = BACKWARD_STATE;
		            		else if (armAngles[side] <= -45.0f) //15.0f
		            			armStates[side] = FORWARD_STATE;
		            
		            		// legs
/*							//edited by Mike, 20201204
							if (legStates[side] == FORWARD_STATE)
		            			legAngles[side] += 15.0f * dt;
		            		else
		            			legAngles[side] -= 15.0f * dt;
*/
							if (legStates[side] == FORWARD_STATE)
		            			legAngles[side] += 5.0f * dt;
		            		else
		            			legAngles[side] -= 5.0f * dt;
							
		            		// change state if exceeding angles
		            		if (legAngles[side] >= 15.0f) //15.0f
		            			legStates[side] = BACKWARD_STATE;
		            		else if (legAngles[side] <= -15.0f)
		            			legStates[side] = FORWARD_STATE;		
		            	}                
		                break;

		            //added by Mike, 20210121
		            case ATTACKING_MOVING_STATE:
		            	if (bIsExecutingPunch) {
		            		if (iPunchAnimationCount<MAX_PUNCHING_ANIMATION_COUNT) {
								//edited by Mike, 20210122
		            			//iPunchAnimationCount+=1;
								if ((iPunchAnimationCountDelay)%2==0) {
									iPunchAnimationCount+=1;
									//added by Mike, 20210123
									iPunchAnimationCountDelay=0;
								}
								iPunchAnimationCountDelay+=1;
							}
/*							else {
								if (iPunchAnimationCountDelay<5) {
									iPunchAnimationCountDelay+=1;
								}
							}
*/
							//added by Mike, 20210123
							//+added: no continuous punch via hold punch button
							else {
								//edited by Mike, 20210123; edited again by Mike, 20210124
								if (iPunchAnimationCountDelay<0) { //<5
								}
								else {
									//edited by Mike, 20210123
		    						if (myKeysDown[KEY_U]==FALSE) {  
										bIsExecutingPunch=false;
										iPunchAnimationCount=0;
										iPunchAnimationCountDelay=0;

										//added by Mike, 20210124
								   		armAngles[RIGHT]=0.0f;
										armAngles[LEFT]=0.0f;
									} 
								}
								iPunchAnimationCountDelay+=1;
							}
						}
						
						//added by Mike, 20210124
						if (bIsExecutingDefend) {
    						if (myKeysDown[KEY_H]==FALSE) {  
								bIsExecutingDefend=false;

								//added by Mike, 20210124
						   		armAngles[RIGHT]=0.0f;
								armAngles[LEFT]=0.0f;
							} 
						}						
		            	break;
		                
		            default: //STANDING STATE		            
		              break;//do nothing    
				}

/* //removed by Mike, 20210126				
				//added by Mike, 20210126
				if (myKeysDown[KEY_RIGHT]==FALSE) {
					bIsDashReady=false;
					bIsExecutingDash=false;				
				}
*/

				//added by Mike, 20210126; edited by Mike, 20210128
//				if (myKeysDown[KEY_RIGHT]==FALSE) {
				if (myKeysDown[KEY_D]==FALSE) {
//					if (iInputWaitCount<MAX_WAIT_COUNT) {
					if (iInputWaitCountArray[KEY_D]<MAX_WAIT_COUNT) {
//						iInputWaitCount+=1;
						iInputWaitCountArray[KEY_D]+=1;
					}
				}
				if (myKeysDown[KEY_A]==FALSE) {
					if (iInputWaitCountArray[KEY_A]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_A]+=1;
					}
				}
/*	//removed by Mike, 20210812				
				if (myKeysDown[KEY_W]==FALSE) {
					if (iInputWaitCountArray[KEY_W]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_W]+=1;
					}
				}
				if (myKeysDown[KEY_S]==FALSE) {
					if (iInputWaitCountArray[KEY_S]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_S]+=1;
					}
				}
*/
				
				//TO-DO: -add: these
           		//added by Mike, 20201001
           		rotationAngle=0; //TO-DO: -update: this
           		
//                if (isMovingForward)
//                { 
  					//removed by Mike, 20201014
                    /* rotationAngle in degrees, convert to radians */
                    //im not yet sure why, but i have to deduct 90 to rotationAngle
//                    rotationAngleRad = (rotationAngle) * 3.141593f / 180.0f;

/*                  //removed by Mike, 20201001
                    yAccel = (cos(rotationAngleRad)*thrust);
                    xAccel = -(sin(rotationAngleRad)*thrust);
*/

/*
                   rotationAngle+=1;
                   char str[700];                                       
                   sprintf(str,"xAccel: %f",xAccel);
                   MessageBox(NULL, str, "Welcome!", MB_OK);
*/

/*
					//added by Mike, 20201001                   
					//TO-DO: -update: max acceleration
                    if (yAccel>0.01f) {
						thrust=0.01f;                    
					}
                    if (xAccel>0.01f) {
						thrust=0.01f;                    
					}
*/

/*					//removed by Mike, 20201001
                    xVel=xAccel;
                    yVel=yAccel;
*/                    
                    
/*					//removed by Mike, 20201014                    
                    if (xVel > maxXVel) xVel = maxXVel;
                    else if (xVel < -maxXVel) xVel = -maxXVel;
                    if (yVel > maxYVel) yVel = maxYVel;
                    else if (yVel < -maxYVel) yVel = -maxYVel;
                    myXPos+=xVel;
                    myYPos+=yVel;
                   
				    //note: deaccelerate 
                    if (thrust>0)
                      thrust-=0.02f; //0.01f
                    else thrust=0;
*/                                        

/* //removed by Mike, 20210424			
				//Note: Use these with update to OpenGLCanvas
           		//wrap the world 
           		//edited by Mike, 20201116
//           		if (myXPos <= 0.0f) myXPos = 20-myWidth/8; //if left side
           		if (myXPos <= 0.0f) myXPos = myWindowWidth/100-myWidth/8; //if left side
           		else if (myXPos >= myWindowWidth/100) myXPos = 0.0f+myWidth/8; //if right side

           		if (myZPos >= myWindowHeight/100) myZPos = 0.0f+myHeight/8; //if bottom side
           		//edited by Mike, 20201116
//           		else if (myZPos <= 0.0f) myZPos = 20-myHeight/8; //if top side
           		else if (myZPos <= 0.0f) myZPos = myWindowHeight/100-myHeight/8; //if top side
*/           		           		

/*
          char str[700];                                       
          sprintf(str,"myXPos: %f",myXPos);
          MessageBox(NULL, str, "Welcome!", MB_OK);
*/
//                   isMovingForward=0; 
//                }
                break;
            case IN_TITLE_STATE:                
                  rotationAngle+=5;//rotationStep;
                break;
            default: //STANDING STATE
              break;//do nothing    
    }
}

//added by Mike, 20210423; edited by Mike, 20210807
//TO-DO: -update: this
void Pilot::update(float dt)
{

    switch (currentState)
    {
           case INITIALIZING_STATE:
           case MOVING_STATE:   
           //removed by Mike, 20210807
/*              
				switch(currentMovingState) {
		           case WALKING_MOVING_STATE:
                        //added by Mike, 20210613
                        bIsExecutingKick=false;
                        iKickAnimationCount=0;
                        iKickAnimationCountDelay=0;
		                break;
		            case ATTACKING_MOVING_STATE:
                        //added by Mike, 20210613
                        if (bIsExecutingKick) {
                            if (iKickAnimationCount<MAX_KICKING_ANIMATION_COUNT) {
                                if ((iKickAnimationCountDelay)%2==0) {
                                    iKickAnimationCount+=1;
                                    iKickAnimationCountDelay=0;
                                }
                                iKickAnimationCountDelay+=1;
                            }
                            //added by Mike, 20210613
                            //+added: no continuous kick via hold kick button
                            else {
                                if (iKickAnimationCountDelay<0) { //<5
                                }
                                else {
                        //added by Mike, 20210613
                        //TO-DO: -fix: no need to release pressed KEY_K
                                    
                        //removed by Mike, 20210613
//                                    if (myKeysDown[KEY_K]==FALSE) {
                                        bIsExecutingKick=false;
                                        iKickAnimationCount=0;
                                        iKickAnimationCountDelay=0;
  //removed by Mike, 20210613
////                                        armAngles[RIGHT]=0.0f;
////                                        armAngles[LEFT]=0.0f;

                                    
//removed by Mike, 20210613
//                                    }
                                }
                                iKickAnimationCountDelay+=1;
                            }
                        }
                        
                        
		            	if (bIsExecutingPunch) {
		            		if (iPunchAnimationCount<MAX_PUNCHING_ANIMATION_COUNT) {
								if ((iPunchAnimationCountDelay)%2==0) {
									iPunchAnimationCount+=1;
									iPunchAnimationCountDelay=0;
								}
								iPunchAnimationCountDelay+=1;
							}
							//added by Mike, 20210123
							//+added: no continuous punch via hold punch button
							else {
								//edited by Mike, 20210123; edited again by Mike, 20210124
								if (iPunchAnimationCountDelay<0) { //<5
								}
								else {
									//edited by Mike, 20210123
		    						if (myKeysDown[KEY_U]==FALSE) {  
										bIsExecutingPunch=false;
										iPunchAnimationCount=0;
										iPunchAnimationCountDelay=0;

										//added by Mike, 20210124
								   		armAngles[RIGHT]=0.0f;
										armAngles[LEFT]=0.0f;
									} 
								}
								iPunchAnimationCountDelay+=1;
							}
						}
						
						if (bIsExecutingDefend) {
    						if (myKeysDown[KEY_H]==FALSE) {  
								bIsExecutingDefend=false;

								//added by Mike, 20210124
						   		armAngles[RIGHT]=0.0f;
								armAngles[LEFT]=0.0f;
							} 
						}						
		            	break;
		                
		            default: //STANDING STATE		            
		              break;//do nothing    
				}
*/

				if (myKeysDown[KEY_D]==FALSE) {
					if (iInputWaitCountArray[KEY_D]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_D]+=1;
					}
				}
				if (myKeysDown[KEY_A]==FALSE) {
					if (iInputWaitCountArray[KEY_A]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_A]+=1;
					}
				}
/*	//removed by Mike, 20210812				
				if (myKeysDown[KEY_W]==FALSE) {
					if (iInputWaitCountArray[KEY_W]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_W]+=1;
					}
				}
				if (myKeysDown[KEY_S]==FALSE) {
					if (iInputWaitCountArray[KEY_S]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_S]+=1;
					}
				}
*/

           		rotationAngle=0; //TO-DO: -update: this

/* //removed by Mike, 20210424						
				//Note: Use these with update to OpenGLCanvas
           		//wrap the world 
           		if (myXPos <= 0.0f) myXPos = myWindowWidth/100-myWidth/8; //if left side
           		else if (myXPos >= myWindowWidth/100) myXPos = 0.0f+myWidth/8; //if right side

           		if (myZPos >= myWindowHeight/100) myZPos = 0.0f+myHeight/8; //if bottom side
           		else if (myZPos <= 0.0f) myZPos = myWindowHeight/100-myHeight/8; //if top side
*/           		           		
			

				//added by Mike, 20210503
				//Note: Use these with update to OpenGLCanvas
				//max world 
/*			
           		if (myXPos <= 15.0f) myXPos = 0-15.0f; //if left side
           		else if (myXPos >= myWindowWidth/1fGridTileWidthVertexPosition00) myXPos = 0.0f+myWidth/8; //if right side
*/			
/*	//removed by Mike, 20210504
			printf(">> myXPos: %f\n",myXPos);
			printf(">> stepX: %f\n",stepX);
				//TO-DO: -update: instructions to do not execute step if already at border
*/				
/*			
           		if ((myXPos-stepX) <= -20.70f) myXPos = 0.0f-21.0f+stepX; //+myWidth+stepX; //if left side
           		else if (myXPos >= myWindowWidth/100) myXPos = 0.0f+myWidth/8; //if right side
*/
/*	//removed by Mike, 20210504			
           		if ((myXPos-stepX) <= -21.0f) myXPos = 0.0f-21.0f+stepX; //if left side
           		else if (myXPos+stepX >= 4.0f) myXPos = 0.0f+4.0f-stepX; //if right side
*/
				break;
            case IN_TITLE_STATE:                
                  rotationAngle+=5;//rotationStep;
                break;
            default: //STANDING STATE
              break;//do nothing    
    }
}

void Pilot::changeState(int s)
{
  currentState=s;                  
}

//added by Mike, 20201226
void Pilot::keyDown(int keyCode) {
	myKeysDown[keyCode] = TRUE;
	
	//added by Mike, 20210812
	myKeysDown[KEY_W] = FALSE;
	myKeysDown[KEY_S] = FALSE;

	//added by Mike, 20210127; edited by Mike, 20210128
	autoVerifyDashStateWithKeyDown();//keyCode);
}

//added by Mike, 20201227; edited by Mike, 20210128
//void Pilot::setDashStateWithKeyDown() {
void Pilot::setDashStateWithKeyDown(int keyCode) {
	if (bIsDashReady==true) {
/*		//edited by Mike, 20210128
		if (iInputWaitCount<MAX_WAIT_COUNT) {
			//edited by Mike, 20210128
			//bIsExecutingDash=true;
			bIsExecutingDashArray[keyCode]=true;			
		}
*/		
		if (iInputWaitCountArray[keyCode]<MAX_WAIT_COUNT) {
			//edited by Mike, 20210129
//			bIsExecutingDashArray[keyCode]=true;			
			//verify if any directional key already executes dash
			for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
				if (bIsExecutingDashArray[keyCode]) {
					return;
				}							
			}

			bIsExecutingDashArray[keyCode]=true;
		}
	}
}

//added by Mike, 20210521
//TO-DO: -reverify: dash for KEY's A and D

//added by Mike, 20201226; edited by Mike, 20210128
//void Pilot::autoVerifyDashStateWithKeyDown(int keyCode) {
void Pilot::autoVerifyDashStateWithKeyDown() { //int keyCode) {
	//edited by Mike, 20210128
	//if (myKeysDown[KEY_RIGHT]==TRUE) {
	//edited by Mike, 20210130
//	if ((myKeysDown[KEY_RIGHT]==TRUE) || (myKeysDown[KEY_D]==TRUE)) {		
	if (myKeysDown[KEY_D]==TRUE) {
		setDashStateWithKeyDown(KEY_D);
	}
	//edited by Mike, 20210130
//	else if ((myKeysDown[KEY_UP]==TRUE) || (myKeysDown[KEY_W]==TRUE)) {
/*	//removed by Mike, 20210812				
	else if (myKeysDown[KEY_W]==TRUE) {
		setDashStateWithKeyDown(KEY_W);//KEY_UP);
	}
*/	
	else if (myKeysDown[KEY_A]==TRUE) {
		setDashStateWithKeyDown(KEY_A);
	}
/*	//removed by Mike, 20210812					
	else if (myKeysDown[KEY_S]==TRUE) {
		setDashStateWithKeyDown(KEY_S);
	}
*/	
}

void Pilot::keyUp(int keyCode) {
	//added by Mike, 20210127
	autoVerifyDashStateWithKeyUp(keyCode);

	myKeysDown[keyCode] = FALSE;	
}

//added by Mike, 20210127; edited by Mike, 20210126
/*
void Pilot::setDashStateWithKeyUp() {
	if (bIsExecutingDash) {
		//edited by Mike, 20210128
//		bIsExecutingDash=false;
		for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
			bIsExecutingDashArray[iCount]=false;			
		}

		bIsDashReady=false;			
	}
	else {
		bIsDashReady=true;			
		iInputWaitCount=0;
	}
}
*/
//added by Mike, 20210127; edited by Mike, 20210129
//void Pilot::setDashStateWithKeyUp() {
void Pilot::setDashStateWithKeyUp(int keyCode) {
	//edited by Mike, 20210128
	bool bIsExecutingDash=false;

	//added by Mike, 20210202
	//if removed, dash persists
	for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
		if (bIsExecutingDashArray[iCount]) {
			bIsExecutingDash=true;
			break;
		}			
	}	

	if (bIsExecutingDash) {
		//if (bIsExecutingDashArray[KEY_RIGHT]) {
		//edited by Mike, 20210130
//		if ((bIsExecutingDashArray[KEY_RIGHT]) || (bIsExecutingDashArray[KEY_D])) {		
		if (bIsExecutingDashArray[KEY_D]) {
			bIsExecutingDashArray[KEY_D]=false;
			bIsDashReady=false;
		}
		//edited by Mike, 20210130
//		else if ((bIsExecutingDashArray[KEY_UP]) || (bIsExecutingDashArray[KEY_W])) {
/*	//removed by Mike, 20210812				
		if (bIsExecutingDashArray[KEY_W]) {
			bIsExecutingDashArray[KEY_W]=false;//KEY_UP);
			bIsDashReady=false;
		}
*/		
		else if (bIsExecutingDashArray[KEY_A]) {
			bIsExecutingDashArray[KEY_A]=false;
			bIsDashReady=false;
		}
/*	//removed by Mike, 20210812						
		else if (bIsExecutingDashArray[KEY_S]) {
			bIsExecutingDashArray[KEY_S]=false;
			bIsDashReady=false;
		}
*/		
	}
	else {
		bIsDashReady=true;			
		//edited by Mike, 20210128		
//		iInputWaitCount=0;

		//edited by Mike, 20210129
/*		for (int iCountKey=0; iCountKey<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCountKey++) {
			iInputWaitCountArray[iCountKey]=0;
		}		
*/		
		iInputWaitCountArray[keyCode]=0;
	}
	
}

//added by Mike, 20210127
void Pilot::autoVerifyDashStateWithKeyUp(int keyCode) {
	//added by Mike, 20210126; edited by Mike, 20210128
//	if (keyCode==KEY_RIGHT) {
	//edited by Mike, 20210130
//	if ((keyCode==KEY_RIGHT) || (keyCode==KEY_D)) {
	if (keyCode==KEY_D) {
		//edited by Mike, 20210128
//		if (myKeysDown[KEY_RIGHT]==TRUE) {
		if (myKeysDown[KEY_D]==TRUE) {
			//edited by Mike, 20210129myZPosAsPixel
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_D);
		}
	}
	//edited by Mike, 20210130
//	else if ((keyCode==KEY_UP) || (keyCode==KEY_W)) {
/*	//removed by Mike, 20210812				
	else if (keyCode==KEY_W) {
		//edited by Mike, 20210130
//		if ((myKeysDown[KEY_UP]==TRUE) || (myKeysDown[KEY_W]==TRUE)) {
		if (myKeysDown[KEY_W]==TRUE) {
			//edited by Mike, 20210129
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_W);
		}
	}
*/	
	else if (keyCode==KEY_A) {
		if (myKeysDown[KEY_A]==TRUE) {
			//edited by Mike, 20210129
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_A);
		}
	}
/*	//removed by Mike, 20210812					
	else if (keyCode==KEY_S) {
		if (myKeysDown[KEY_S]==TRUE) {
			//edited by Mike, 20210129
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_S);
		}
	}	
*/	
	//removed by Mike, 20210128
/*
	else {
		//edited by Mike, 20210128
//		bIsExecutingDash=false;
		for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
			bIsExecutingDashArray[iCount]=false;			
		}
		
		bIsDashReady=false;			
	}
*/	
}


void Pilot::move(int key)
{
   //Note: Unit member as Pilot has to release hold of directional keys,
   //so that RobotShip faces in the correct direction;
   //holding the fire beam key, e.g. KEY_I, causes RobotShip 
   //to move without changing the direction that it faces
	
   //added by Mike, 20201201; removed by Mike, 20201226
//   currentMovingState=WALKING_MOVING_STATE;

   //added by Mike, 20201225; removed by Mike, 20201225
   //bIsFiringBeam=false;
   
   //TO-DO: -add: bIsMovingLeft, etc, while facing up, etc

   //added by Mike, 20201226; removed by Mike, 2myZPosAsPixel0201226
//   myKeysDown[key] = TRUE;	

	//removed by Mike, 20210203
/*	//added by Mike, 20210203
	if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_W])) {
        currentFacingState=FACING_LEFT_AND_UP;
	}
	else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_W])) {
        currentFacingState=FACING_RIGHT_AND_UP;
	}
	//added by Mike, 20210202
	else if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_S])) {
        currentFacingState=FACING_LEFT_AND_DOWN;
	}
	else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_S])) {
        currentFacingState=FACING_RIGHT_AND_DOWN;
	}
*/

   switch (key)
   {
     //added by Mike, 20201218
     case KEY_I:
     	//removed by Mike, 20201225
/*   		  
		  currentMovingState=ATTACKING_MOVING_STATE;		   
          currentFacingState=FACING_UP;
*/          
          //added by Mike, 20201225
          bIsFiringBeam=true;
          
          //added by Mike, 20201226
          //iNumOfKeyTypes
          bHasPressedADirectionalKey=false;
          //based on enum Keys 
          //edited by Mike, 20210129
          //TO-DO: +reverified: Robotship does not execute correctly
		  //when down and left buttons are pressed while firing beam down
		  //AND when up and left buttons are pressed while firing beam up		  
		  //in Windows Machine
		  //problem did not occur on Linux Machine (with external USB keyboard)			
		  //added by Mike, 20210131
		  //note: add use of external USB keyboard solves the problem		  

//          for (int iCount=0; iCount<10; iCount++) {
		   //edited by Mike, 20210130
		   //note: in RobotShip.h set PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT=4, not 6,
		   //causes RobotShip movement error
		   //edited by Mike, 20210202
          for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
//          for (int iCount=0; iCount<4; iCount++) {
			  if (myKeysDown[iCount]==TRUE) {
          		bHasPressedADirectionalKey=true;
   		    	break;
			}
		  }
		  
		  if (!bHasPressedADirectionalKey) {
		  	currentMovingState=ATTACKING_MOVING_STATE;		   		  	
		  }          
          break;
     //added by Mike, 20201226
     case -KEY_I:
//		  currentMovingState=IDLE_MOVING_STATE;
          bIsFiringBeam=false;
          //removed by Mike, 20201226
//          bHasPressedADirectionalKey=false; //added by Mike, 20201226

   		  if (currentMovingState==WALKING_MOVING_STATE) {   		  	
		  }
		  //added by Mike, 20201226
 		  else if (currentMovingState==ATTACKING_MOVING_STATE) {   		  	
		  }
		  else {
		  	currentMovingState=IDLE_MOVING_STATE;
		  }			
          break;	

     //added by Mike, 2021011
     //TO-DO: -update: this to be defend using shield
     case KEY_H:
     		 //added by Mike, 20210809; removed by Mike, 20210809
//		     drawShieldEffectAsQuadWithTexture();
     
/* //removed by Mike, 20210809; executes kick Command     
          bIsExecutingDefend=true;
          
          bHasPressedADirectionalKey=false;
          //based on enum Keys 
          //edited by Mike, 20210129
//          for (int iCount=0; iCount<10; iCount++) {
          for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
   		    if (myKeysDown[iCount]==TRUE) {
          		bHasPressedADirectionalKey=true;
   		    	break;
			}
		  }
		  
//		  if (!bHasPressedADirectionalKey) {
		  	currentMovingState=ATTACKING_MOVING_STATE;		   		  	
//		  }         
*/ 
          break;
     case -KEY_H:
/*          //removed by Mike, 20210124
			bIsExecutingDefend=false;
*/
   		  if (currentMovingState==WALKING_MOVING_STATE) {   		  	
		  }
		  //added by Mike, 20201226
 		  else if (currentMovingState==ATTACKING_MOVING_STATE) {   		  	
		  }
		  else {
		  	currentMovingState=IDLE_MOVING_STATE;
		  }			
          break;
	
	//added by Mike, 20210121
    case KEY_U:
		  //removed by Mike, 20210123
//          bIsExecutingPunch=true;
		            
		  //added by Mike, 20210122; edited by Mike, 202101213
		  if ((iPunchAnimationCount==0)){// or (iPunchAnimationCount>=MAX_PUNCH_ANIMATION_COUNT)) {		  	
            bIsExecutingPunch=true;
		  }
		             
          bHasPressedADirectionalKey=false;

          //based on enum Keys 
          //edited by Mike, 20210129
//          for (int iCount=0; iCount<10; iCount++) {
          for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
		   	if (myKeysDown[iCount]==TRUE) {
          		bHasPressedADirectionalKey=true;
   		    	break;
			}
		  }

		  
//		  if (!bHasPressedADirectionalKey) {
		  	currentMovingState=ATTACKING_MOVING_STATE;		   		  	
//		  }          
          break;
     case -KEY_U:
    	//TO-DO: -reverify: arm angles after release of punch button and then press move down
     	
   		  if (currentMovingState==WALKING_MOVING_STATE) {   		  	
		  }
		  //added by Mike, 20201226
 		  else if (currentMovingState==ATTACKING_MOVING_STATE) {   		  	
		  }
		  else {
		  	currentMovingState=IDLE_MOVING_STATE;
		  }			
          break;

/* //removed by Mike, 20210806
//	 case KEY_UP: //removed by Mike, 20210130
     case KEY_W:
       //added by Mike, 20210111
       if (bIsExecutingPunch) {
	   }
       //added by Mike, 20210121
	   else if (bIsExecutingDefend) {
	   }
       //added by Mike, 20210613
       else if (bIsExecutingKick) {
       }
       else {
//added by Mike, 20210521
//----------
         //added by Mike, 20210725
         if (myLevel2D->isLevel2DCollideWith(this)) {
//            printf(">>>>COLLISION!");
//							currentMovingState=IDLE_MOVING_STATE;
							bHasHitWall=true;
             return;
        	//		break;
         }
         else {         
         			//added by Mike, 20210728
         			//edited by Mike, 20210729; TO-DO: -update: this
         		  if ((bHasHitWall) and (getCurrentFacingState()==FACING_UP)) {
//         		  if ((bHasHitWall)) {
         		  	return;
         		  }
         		           		  
         		                    //edited by Mike, 20210805
                //in macOS machine, Pilot does not move up due to gravity;
                //OK in LUBUNTU machine; JUMP height low; exerting effort to move up, but pulled by heavy force
//            	myYPosAsPixel+=-stepY;
//            	myYPosAsPixel+=(-stepY*2);
//            	myYPosAsPixel+=(-stepY*1.1);
  
  						 //edited by Mike, 20210806           
//             iStepYCountMax=20;
             iStepYCountMax=10;

             if (iStepYCount>=iStepYCountMax) {
             			//added by Mike, 20210806;
             			//removed by Mike, 20210806;
             			//Yosshi- Dinosaur?
//             		return;

            			currentMovingState=IDLE_MOVING_STATE;
              		currentFacingState=FACING_UP;
                 
                 break;
             }
             else {
                myYPosAsPixel+=(-stepY*1.1);
                iStepYCount+=1;
             }

		 					bHasHitWall=false;
		 		}
			//added by Mike, 20210521		
//----------		
       }
	
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_UP;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
					//added by Mike, 20210728; edited by Mike, 20210729
					//TO-DO: -update: this
              //added by Mike, 20210729; removed by Mike, 20210729;
              //yoko scroll
//              prevFacingState=currentFacingState;
              
              currentFacingState=FACING_UP;
		  }
		  
          currentMovingState=WALKING_MOVING_STATE;
          break;

		   //edited by Mike, 20210812
           return;
//           break;
*/

/* //removed by Mike, 20210806          
//     case KEY_DOWN:  //removed by Mike, 20210130
     case KEY_S: //added by Mike, 20210128
				//added by Mike, 20210111
				if (bIsExecutingPunch) {
				}
				//added by Mike, 20210121
				else if (bIsExecutingDefend) {
				}
    			//added by Mike, 20210613
    			else if (bIsExecutingKick) {
    			}
				else {
         			//added by Mike, 20210725
         			if (myLevel2D->isLevel2DCollideWith(this)) {
			//            printf(">>>>COLLISION!");
//										currentMovingState=IDLE_MOVING_STATE;
										bHasHitWall=true;
        						return;
         			}
         			else {				
					  			//added by Mike, 20210728
         					if ((bHasHitWall) and (getCurrentFacingState()==FACING_DOWN)) {
         		  			return;
         					}
        					if (getIsPlayer1()) { //Player1: Unit Chief
            					myYPosAsPixel+=stepY;
        					}
        					else {
            					myYPosAsPixel+=stepY/2;
        					}
                                }

 if ((bIsExecutingDashArray[KEY_S])) {
            			myYPosAsPixel+=stepY*2;
					}
							bHasHitWall=false;
				}
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_DOWN;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
              //added by Mike, 20210729; removed by Mike, 20210729
              //yoko scroll
//              prevFacingState=currentFacingState;

              //added by Mike, 20210728
              currentFacingState=FACING_DOWN;
		  }

           currentMovingState=WALKING_MOVING_STATE;
          break;
        //added by Mike, 20210806; edited by Mike, 20210812
           return;
//			break;
            */
			
			
//     case KEY_LEFT: //removed by Mike, 20210130
     case KEY_A: //added by Mike, 20210128		   
     		//removed by Mike, 20201001
//          rotationAngle+=rotationStep;
/*		//removed by Mike, 20201014
     	  //added by Mike, 20201001
          if (thrust<thrustMax)
            thrust+=-0.1f;
*/
					//added by Mike, 20210111
					if (bIsExecutingPunch) {
					}
					//added by Mike, 20210121
					else if (bIsExecutingDefend) {
					}
    				//added by Mike, 20210613
    				else if (bIsExecutingKick) {
    				}
					else {
         				//added by Mike, 20210728
         				if (myLevel2D->isLevel2DCollideWith(this)) {
				//            printf(">>>>COLLISION!");
//											currentMovingState=IDLE_MOVING_STATE;
											bHasHitWall=true;
        							return;
         				}
         				else {
										//added by Mike, 20210728
         						if ((bHasHitWall) and (getCurrentFacingState()==FACING_LEFT)) {
         							return;
         						}
        					
								//edited by Mike, 20210728
								//TO-DO: -add: acceleration
/*								
        					myXPosAsPixel+=-stepX;
							
									if ((bIsExecutingDashArray[KEY_A])) {		
											//edited by Mike, 20210527
										myXPosAsPixel+=-stepX;
									}
*/									
         					if (getIsPlayer1()) { //Player1: Unit Chief
            					myXPosAsPixel+=-stepX;
         					}
         					else {
            					myXPosAsPixel+=-stepX;
         					}
																											
         					if (bIsExecutingDashArray[KEY_A]) {
								//edited by Mike, 20210807
            					//myXPosAsPixel+=-stepX*2;   
								//dash Command not executed during free fall
								//TO-DO: -update: this due to incorrect output
								//if during free fall, at WALKING_MOVING_STATE
								//dash Command still executed;
								//TURBO?
            					if (currentMovingState==IDLE_MOVING_STATE) {
            					}
								else {
            						myXPosAsPixel+=-stepX*2;						
								}           
		 					}									
											
									bHasHitWall=false;
								}					
					}
	
/*          
          char str[700];                                       
          sprintf(str,"rotationAngle: %f",rotationAngle);
          MessageBox(NULL, str, "Welcome!", MB_OK);
*/
		   
		   
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_LEFT;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
              //added by Mike, 20210729; edited by Mike, 20210729
              //prevFacingState=currentFacingState;
//              if (currentFacingState==FACING_LEFT) {
              if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_LEFT)) {
                  prevFacingState=currentFacingState;
              }

              //added by Mike, 20210502
              currentFacingState=FACING_LEFT;
		  }

           //edited by Mike, 20210613
           currentMovingState=WALKING_MOVING_STATE;
/* //removed by Mike, 20210613
           if (bIsExecutingKick) {
               currentMovingState=ATTACKING_MOVING_STATE;
           }
*/
          break;
          
      //note: combination of FACING RIGHT and DOWN actions cause spin movement          
//     case KEY_RIGHT: //removed by Mike, 20210130
     case KEY_D: //added by Mike, 20210128
		   //removed by Mike, 20201001
//          rotationAngle-=rotationStep;

/*		//removed by Mike, 20201014
     	  //added by Mike, 20201001
          if (thrust<thrustMax)
            thrust+=0.1f;
*/

				//added by Mike, 20210111
				if (bIsExecutingPunch) {
				}
				//added by Mike, 20210121
				else if (bIsExecutingDefend) {
				}
    		//added by Mike, 20210613
    		else if (bIsExecutingKick) {
    		}
				else {
      			//added by Mike, 20210728
      			if (myLevel2D->isLevel2DCollideWith(this)) {
			//            printf(">>>>COLLISION!");
//							currentMovingState=IDLE_MOVING_STATE;
							bHasHitWall=true;
        			return;
      			}
      			else {
							//added by Mike, 20210728
         			if ((bHasHitWall) and (getCurrentFacingState()==FACING_RIGHT)) {
         				return;
         			}      			
      			
							//edited by Mike, 20210728
/*							//TO-DO: -add: acceleration
        			//myXPosAsPixel+=stepX*2;
        			myXPosAsPixel+=stepX;
					
							if ((bIsExecutingDashArray[KEY_D])) {
								//edited by Mike, 20210527
								myXPosAsPixel+=stepX;
							}		
*/							
         			if (getIsPlayer1()) { //Player1: Unit Chief
            			myXPosAsPixel+=stepX;
         			}
         			else {
            			myXPosAsPixel+=stepX;
         			}
					
         			if (bIsExecutingDashArray[KEY_D]) {
         				//edited by Mike, 20210807
            			//myXPosAsPixel+=stepX*2;   
						//dash Command not executed during free fall            			
            			if (currentMovingState==IDLE_MOVING_STATE) {
            			}
						else {
            				myXPosAsPixel+=stepX*2;						
						}           			         			
		 			}									
							
							bHasHitWall=false;
						}
				}
		   		
	  		//added by Mike, 20201201; edited by Mike, 20201225
        		//currentFacingState=FACING_RIGHT;
	  		if (bIsFiringBeam) {	      	
				}
				else {
                    //added by Mike, 20210729; edited by Mike, 20210729
                    //prevFacingState=currentFacingState;
                    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_LEFT)) {
                        prevFacingState=currentFacingState;
                    }
                    
                    currentFacingState=FACING_RIGHT;
				}
		  
           //edited by Mike, 20210613
           currentMovingState=WALKING_MOVING_STATE;
/*  //removed by Mike, 20210613
           if (bIsExecutingKick) {
               currentMovingState=ATTACKING_MOVING_STATE;
           }
*/
		  break;
		  
		  //added by Mike, 20210611
			case KEY_K:
                //added by Mike, 20210613
                if ((iKickAnimationCount==0)){// or (iPunchAnimationCount>=MAX_PUNCH_ANIMATION_COUNT)) {
                    bIsExecutingKick=true;
                }
           
           bHasPressedADirectionalKey=false;
           
           //based on enum Keys
           for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
               if (myKeysDown[iCount]==TRUE) {
                   bHasPressedADirectionalKey=true;
                   break;
               }
           }
           
           //removed to hitBy(...) by Mike, 20210612
           currentMovingState=ATTACKING_MOVING_STATE;
           break;
           //added by Mike, 20210613
       case -KEY_K:
           //TO-DO: -reverify: arm angles after release of kick button and then press move down
           if (currentMovingState==WALKING_MOVING_STATE) {
           }
           else if (currentMovingState==ATTACKING_MOVING_STATE) {
           }
           else {
               currentMovingState=IDLE_MOVING_STATE;
           }			
           break;
                      					  			
		//added by Mike, 20201201
		default:
          //edited by Mike, 20210604
		  //currentMovingState=IDLE_MOVING_STATE;
//           if (getIsPlayer1()) { //note: Player1 as Unit Chief
             currentMovingState=IDLE_MOVING_STATE;
//           }
           
		  bIsFiringBeam=false; //added by Mike, 20201226
		  //removed by Mike, 20210123
		  //bIsExecutingPunch=false; //added by Mike, 20210111
		  
		  bIsExecutingDefend=false; //added by Mike, 20210121
           
          //added by Mike, 20210805
          iStepYCount=0;
          bIsExecutingDash=false;
          
          //added by Mike, 20210807
          if (myLevel2D->isLevel2DCollideWith(this)) {          	
          }
          else {
          	//+gravity when at free fall
          	//TO-DO: -add: animation sprite image?
          	//note: stepY*2, et cetera is over what the ground/floor tile can push up
          	//edited by Mike, 20210812
			myYPosAsPixel+=stepY;
			
			//added by Mike, 20210812
			//note: this set of instructions NOT executed 
			//when there exists input to execute LEFT or RIGHT movement
			//TO-DO: -update: instructions when computer receives both LEFT and RIGHT input Commands
			if (myLevel2D->isLevel2DCollideWith(this)) {    
			}
			else {
				myYPosAsPixel+=stepY; ///2.0
			}			
          }
		  break;		  		  
   }

    //added by Mike, 20210613
    if (bIsExecutingKick) {
        currentMovingState=ATTACKING_MOVING_STATE;
    }

    
	//added by Mike, 20210111
	if (bIsExecutingPunch) {
		currentMovingState=ATTACKING_MOVING_STATE;
		bIsFiringBeam=false;
	}

	//added by Mike, 20210121
	if (bIsExecutingDefend) {
		currentMovingState=ATTACKING_MOVING_STATE;
		bIsFiringBeam=false;
	}   

/*  //edited by Mike, 20210725
	//added by Mike, 20210724
	//note: in previous computer instructions, we used z-pos for the y-pos now
	//TO-DO: -update: this
	myXPosAsPixel=(int)myXPos;
    //edited by Mike, 20210725
//	myYPosAsPixel=(int)myZPos;
    myYPosAsPixel=(int)myYPos;
*/


	//added by Mike, 20210502
	//TO-DO: -add: FACING_LEFT based on opponent position, e.g. left of pilot

/* //removed by Mike, 20210613
	//added by Mike, 20210104
//	if (!bIsExecutingDefend) {	
	if (!bIsFiringBeam) {
		//added by Mike, 20210203
		//TO-DO: -reverify: fire beam, move diagonal, move non-diagonal direction
		//added by Mike, 20210201
		if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_W])) {
			//removed by Mike, 20210502
//	        currentFacingState=FACING_LEFT_AND_UP;
		}
		else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_W])) {
	        currentFacingState=FACING_RIGHT_AND_UP;
		}
		//added by Mike, 20210202
		else if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_S])) {
			//removed by Mike, 20210502			
//	        currentFacingState=FACING_LEFT_AND_DOWN;
		}
		else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_S])) {
	        currentFacingState=FACING_RIGHT_AND_DOWN;
		}
	}
*/
    //added by Mike, 20210804
    //gravity
    if (myLevel2D->isLevel2DCollideWith(this)) {
        bHasHitWall=true;
        return;
    }
    else {        
        if (bHasHitWall) {
            return;
        }
        
        if (getIsPlayer1()) { //Player1: Unit Chief
        		//edited by Mike, 20210807
//            myYPosAsPixel+=(stepY*1.2);
            myYPosAsPixel+=stepY;
//            myYPosAsPixel+=(stepY*2);
        }
    }
    bHasHitWall=false;

/*
    if (bHasHitWall) {
        return;
    }
    
    if (getIsPlayer1()) { //Player1: Unit Chief
        myYPosAsPixel+=stepY;
    }
*/
}

void Pilot::hitBy(MyDynamicObject* mdo)
{
/*	//removed by Mike, 20210611stepX=fGridSquareWidth/8;
		//auto-identify if Ball object
    if (dynamic_cast<Ball*>(mdo)->getIsBall()) {    	
		}
*/
        //added by Mike, 20210613; edited by Mike, 20210613
/*
        bIsExecutingKick=true;
        currentMovingState=ATTACKING_MOVING_STATE;
        iKickAnimationCount=0;
*/
  
/* //removed by Mike, 20210725
    //edited by Mike, 20210613
//    move(KEY_K);
    if (!mdo->getIsMovingDown()) {
        move(KEY_K);
    }
*/

/*	//removed by Mike, 20210729
        //added by Mike, 20210613
        if (myKeysDown[KEY_D]==TRUE) {
            currentFacingState=FACING_RIGHT;
        }
        else if (myKeysDown[KEY_A]==TRUE) {
            currentFacingState=FACING_LEFT;
        }
*/
    
    
//		mdo->updateDirection();

	//removed by Mike, 20210527
/*
     //changeState(DEATH_STATE);
     //setCollidable(false);
    myOpenGLCanvas->loseLife();
    
    //removed by Mike, 20201001
	////zing = sound->load_sound_clip(RESETSOUND);
	////sound->play_sound_clip(zing);	

    reset();
*/        
}

/*	//removed by Mike, 20210522
void Pilot::setOpenGLCanvas(OpenGLCanvas* c)
{
     myOpenGLCanvas = c;
}
*/

void Pilot::reset()
{
/*  //removed by Mike, 20210801
     changeState(INITIALIZING_STATE);
     myXPos=0;
     myYPos=0;
     rotationAngle=0.0f;
     thrust=0.0f;
    
    //removed by Mike, 20210725
    //setCollidable(false);
 
    invincibleCounter=0;
    
    //added by Mike, 20210613
    bIsExecutingKick=false;
    iKickAnimationCount=0;
    iKickAnimationCountDelay=0;
*/
}
int Pilot::getState()
{
    return currentState;
}

//added by Mike, 20201016
void Pilot::destroy()
{
/*	
	for(int i = 0; i < MAX_EXPLOSION_PARTICLES; ++i) {
	    delete [] explosionParticle[i];
	}
	delete [] explosionParticle;
*/
}

//added by Mike, 20201130
//TO-DO: -add: in PolygonUtils
//--------------------------------------------
bool Pilot::loadTexture(CTargaImage *myTexture, const char *filename, unsigned int *myTextureObject)
{
    //note: Code taken from Dave Astle and Kevin Hawkins's code 
    //("Beginning OpenGL Game Programming", Chapter 7)
    //with slight modifications from Mike
    //-Mike, Dec. 21, 2006
	// enable 2D texturing
	glEnable(GL_TEXTURE_2D);

    myTexture = new CTargaImage;
   	if (!myTexture->Load(filename))//opengl_logo.tga //background.tga //rock.tga
		return false;

    // retrieve "unused" texture object
	//glGenTextures(1, &myTextureObject);
	glGenTextures(1, myTextureObject);

	// bind the texture object
	//glBindTexture(GL_TEXTURE_2D, myTextureObject);
	glBindTexture(GL_TEXTURE_2D, *myTextureObject);

	// minimum required to set the min and mag texture filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// now that the texture object is bound, specify a texture for it
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myTexture->GetWidth(), myTexture->GetHeight(),
				 0, GL_RGB, GL_UNSIGNED_BYTE, myTexture->GetImage());
	
	return true;
}

void Pilot::drawTriangledCube(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		glBegin(GL_TRIANGLES);
		//note: must be in correct order, counter-clockwise
            //glColor3f(1.0f, 0.0f, 0.0f);	// red
            //note: this must be clock-wise so that it will face backwards
		    //front face		    
            //part 1  
            glVertex3f(0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);          

            //part 2
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);

            //glColor3f(0.5f, 0.0f, 0.0f);	// dark red
            //back face 
            //part 1
            glVertex3f(0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            //part 2
            glVertex3f(-0.5f, 0.5f, 0.5f);          
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, 0.5f);
            
    		//glColor3f(0.0f, 1.0f, 0.0f);	// green
            //right face
            glVertex3f(0.5f, -0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            //part 2
            glVertex3f(0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);          

    		//glColor3f(0.0f, 0.5f, 0.0f);	// dark green
            //left face
            //part 1
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, 0.5f);          

            //part 2
            glVertex3f(-0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);

    		//glColor3f(0.0f, 0.0f, 1.0f);	// blue
            //top face
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            //part 2
            glVertex3f(-0.5f, 0.5f, 0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);          

    		//glColor3f(0.0f, 0.0f, 0.5f);	// dark blue
            //bottom face
            glVertex3f(0.5f, -0.5f, 0.5f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);          

            //part 2
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            glVertex3f(0.5f, -0.5f, 0.5f);
		glEnd();
	glPopMatrix();     
}

void Pilot::drawUpperArm(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		//glColor3f(1.0f, 0.0f, 0.0f);	// red
		glColor3f(1.0f, 1.0f, 1.0f);
        glTranslatef(xPos, yPos, zPos);
		glScalef(0.1f, 0.2f, 0.1f);		
        drawTriangledCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Pilot::drawLowerArm(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		//glColor3f(0.5f, 0.0f, 0.0f);	// dark red
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(0.1f, 0.3f, 0.1f);		// arm is a 1x4x1 cube
        drawTriangledCube(0.0f, 0.0f, 0.0f);
		//drawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

/*	//removed by Mike, 20201130
void Pilot::drawAntenna(float xPos, float yPos, float zPos)
{
		glColor3f(0.2f, 0.2f, 0.2f);	// dark gray
        //left side
        glPushMatrix();        
    		glTranslatef(-0.12f, 0.0f, 0.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    		glScalef(0.1f, 0.6f, 0.1f);
            drawTriangle(0.0f, 0.0f, 0.0f);
        glPopMatrix();
        //right side
        glPushMatrix();        
    		glTranslatef(0.12f, 0.0f, 0.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    		glScalef(0.1f, 0.6f, 0.1f);
            drawTriangle(0.0f, 0.0f, 0.0f);
        glPopMatrix();
}
*/

void Pilot::drawHead(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		//removed by Mike, 20201130
//        drawAntenna(0.0f, 0.0f, 0.0f);		
		glColor3f(1.0f, 1.0f, 1.0f);	// white		
		//glScalef(2.0f, 2.0f, 2.0f);		// head is a 2x2x2 cube
		glScalef(0.2f, 0.2f, 0.2f);

		glBindTexture(GL_TEXTURE_2D, myHeadTextureObject);
        glEnable(GL_TEXTURE_2D); //added by Mike, Dec.21,2006

		//edited by Mike, 20201204
		//glColor3f(1.0f, 1.0f, 1.0f);//default: white
		glColor3f(0.97f, 0.09f, 0.0f);//red

    	glBegin(GL_TRIANGLE_STRIP);
    	    //note: In "glTexCoord2f(1.0, 0.0);", if 1.0 is changed to 2.0, the plane is made up of 4 images instead of just 1 whole image 
    		glTexCoord2f(1.0, 0.0); 
            glVertex3f(0.5f, -0.5f, -0.5f);
    		glTexCoord2f(0.0, 0.0); 
            glVertex3f(-0.5f, -0.5f, -0.5f);
    		//note: i don't know why, but i have to create a triangle first, 
            //then connect the last point of the square next, -Mike, Dec. 22, 2006
            //a
            glTexCoord2f(1.0, 1.0);	
            glVertex3f(0.5f, 0.5f, -0.5f);
    		//b
            glTexCoord2f(0.0, 1.0);	
            glVertex3f(-0.5f, 0.5f, -0.5f);
    	glEnd();    	
        drawTriangledCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Pilot::drawBody(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		//glColor3f(0.0f, 0.0f, 1.0f);	// blue
		//glColor3f(0.0f, 0.0f, 0.0f);	// black
        glTranslatef(xPos, yPos, zPos);
//		glScalef( 0.2f, 0.2f, 0.1f);
//		drawSphere();
		glScalef(0.3f, 0.5f, 0.2f);
		glBindTexture(GL_TEXTURE_2D, myBodyTextureObject);
        glEnable(GL_TEXTURE_2D); //added by Mike, Dec.21,2006

		//edited by Mike, 20201204
	    //glColor3f(1.0f, 1.0f, 1.0f);//default: white
	    glColor3f(0.06f, 0.54f, 0.0f);	//green

    	glBegin(GL_TRIANGLE_STRIP);
    	    //note: In "glTexCoord2f(1.0, 0.0);", if 1.0 is changed to 2.0, the plane is made up of 4 images instead of just 1 whole image 
    		glTexCoord2f(1.0, 0.0); 
            glVertex3f(0.5f, -0.5f, -0.5f);
    		glTexCoord2f(0.0, 0.0); 
            glVertex3f(-0.5f, -0.5f, -0.5f);
    		//note: i don't know why, but i have to create a triangle first, 
            //then connect the last point of the square next, -Mike, Dec. 22, 2006
            //a
            glTexCoord2f(1.0, 1.0);	
            glVertex3f(0.5f, 0.5f, -0.5f);
    		//b
            glTexCoord2f(0.0, 1.0);	
            glVertex3f(-0.5f, 0.5f, -0.5f);
    	glEnd();    	
        drawTriangledCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Pilot::drawUpperLeg(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		
		glScalef(0.1f, 0.25f, 0.1f);	
		//glColor3f(1.0f, 1.0f, 0.0f);	// yellow
		glColor3f(0.8f, 0.8f, 0.8f); //gray
        //drawCube(0.0f, 0.0f, 0.0f);
        drawTriangledCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Pilot::drawLowerLeg(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		
		// draw the foot
		glPushMatrix();
			glTranslatef(0.0f, 0.25f, -0.1f);
			//drawFoot(0.0f, -5.0f, 0.0f);
			//removed by Mike, 20201201
//			drawFoot(0.0f, -0.4f, 0.0f);
		glPopMatrix();		
		
		glScalef(0.1f, 0.25f, 0.1f);		// leg is a 1x5x1 cube
		//glColor3f(0.8f, 0.8f, 0.0f);	// yellow
        glColor3f(1.0f, 1.0f, 1.0f); //white
        drawTriangledCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Pilot::drawFoot(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		//glColor3f(1.0f, 1.0f, 1.0f);
		glColor3f(0.2f, 0.2f, 0.2f);	// dark gray
		glTranslatef(xPos, yPos, zPos);
		//glScalef(1.0f, 0.5f, 3.0f);
		glScalef(0.1f, 0.05f, 0.3f);
        drawTriangledCube(0.0f, 0.0f, 0.0f);
		//drawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//added by Mike, 20210107
//TO-DO: -update: this
void Pilot::drawWeapon(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		
		//edited by Mike, 20201207
//		glScalef(0.1f, 0.25f, 0.1f);	
		glScalef(0.1f, 0.25f, 0.7f);		
//		glScalef(0.5f, 0.3f, 0.7f);
		//TO-DO: -update: this
		glColor3f(0.55f, 0.55f, 0.55f); //gray
        drawTriangledCube(0.0f, 0.0f, 0.0f);

/*		glTranslatef(xPos, yPos, zPos);
		glScalef(0.1f, 0.25f, 0.4f);		
		glColor3f(0.4f, 0.4f, 0.4f); //gray
        drawTriangledCube(0.0f, 0.0f, 0.0f);
*/        
	glPopMatrix();
}

//--------------------------------------------
