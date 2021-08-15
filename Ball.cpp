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
 * @date created: 20210524
 * @date updated: 20210815
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

#include "Ball.h"
/* //TO-DO: -add: these
#include "PolygonUtils.h"
#include "ModelPool.h"
*/

//added by Mike, 20210516
#include "UsbongUtils.h"

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
GLboolean Ball::test_pow2(GLushort i)
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
void Ball::load_tga(char *filename)
{
    TARGA_HEADER targa;
    FILE *file;
    GLubyte *data;
    GLuint i;

    file = fopen(filename, "rb");
    if (file == NULL)
        return;

    /* test validity of targa file */
    if (fread(&targa, 1, sizeof(targa), file) != sizeof(targa) ||
        targa.id_field_length != 0 || targa.color_map_type != 0 ||
        targa.image_type_code != 2 || ! test_pow2(targa.width) ||
        ! test_pow2(targa.height) || targa.image_pixel_size != 32 ||
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

//added by Mike, 20210423
void Ball::setup()
{
	//removed by Mike, 20201010
	//due to blank output
    //glEnable(GL_DEPTH_TEST);

    // select texture 1
	glBindTexture(GL_TEXTURE_2D, BALL_TEXTURE_A);
	
    /* create OpenGL texture out of targa file */
	//edited by Mike, 20210420
//    load_tga("textures/armor.tga");	
//    load_tga("textures/imageSpriteExampleMikeWithoutBG.tga");	
//    load_tga("textures/ballExample.tga");
    load_tga("textures/ballExample.tga");
	
	// set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
void Ball::setupPrev()
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

//edited by Mike, 20201001
//Ball::RobotShip(): MyDynamicObject(0,0,300)
//edited by Mike, 20201115
//Ball::RobotShip(): MyDynamicObject(0,0,0)
//edited by Mike, 20210815
//Ball::Ball(float xPos, float yPos, float zPos, int windowWidth, int windowHeight): MyDynamicObject(xPos,yPos,0.0f, windowWidth, windowHeight)
Ball::Ball(float xPos, float yPos, float zPos, float fWindowWidth, float fWindowHeight): MyDynamicObject(xPos,yPos,0.0f, fWindowWidth, fWindowHeight)
{ 
    //edited by Mike, 20201001
	//currentState=IN_TITLE_STATE;//MOVING_STATE;
	currentState=MOVING_STATE;
	//added by Mike, 20201201; edited by Mike, 20210527
//	currentMovingState=IDLE_MOVING_STATE;
	currentMovingState=WALKING_MOVING_STATE;
	
	//added by Mike, 20210527; edited by Mike, 20210528
	bIsMovingDown=false;
	
	//added by Mike, 20210528
	iDirectionXAxis=1; //init go to right side from left side
	iDirectionYAxis=-1; //init go up
	
	//added by Mike, 20210603
	iLastHitByPlayerId=-1;//PILOT_PLAYER_TWO_ID;
	
	//added by Mike, 20210528
	thrustMax=10.0f; //5.0f;//4.0f;
	thrust=1.0f;
    
/*     //removed by Mike, 20210601
    //added by Mike, 20210530
    //-----
    for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
        myXPosAsPixelBallTrailContainer[iCountBallTrail] = 0;//myXPosAsPixel;
        myXPosAsPixelBallTrailContainerTemp[iCountBallTrail] = 0;//myXPosAsPixel;
    }
    for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
        myYPosAsPixelBallTrailContainer[iCountBallTrail] = 0;//myYPosAsPixel;
        myYPosAsPixelBallTrailContainerTemp[iCountBallTrail] = 0;//myYPosAsPixel;
    }
    //-----
*/
    //added by Mike, 20210601
    for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
        fAddXVelBallTrailContainer[iCountBallTrail] = 0.0f;
        
    }
    for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
        fAddYVelBallTrailContainer[iCountBallTrail] = 0.0f;
    }
    
    //added by Mike, 20210601
		iPlayer1BallHitCount=0;
		iPlayer1PartnerBallHitCount=0;
		iPlayer2BallHitCount=0;
		iPlayer2PartnerBallHitCount=0;
    	
		//added by Mike, 20210601
		bIsSetForPartnerSpikeAttack=false;
    
        //added by Mike, 20210603
        bIsSpikeAttack=false;
		
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
//    stepX=0.3;
    stepX=0.2;	
    stepY=0.3;
    stepZ=0.3;

		//added by Mike, 20210527; edited by Mike, 20210528
/*		
		iStepXAsPixel=3;
		iStepYAsPixel=3;
		iStepZAsPixel=3;
*/
		//edited by Mike, 20210528
		iStepXAsPixel=4*-1; //3*-1
		iStepYAsPixel=3;//*-1;
		iStepZAsPixel=3;//*-1;

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
	//added by Mike, 20210517
	//TO-DO: -update: this
	myWidth=1.4f;
  myHeight=1.4f;
	
	//added by Mike, 20210517
	//TO-DO: -add: auto-identify object width and height
/* //edited by Mike, 20210528	
	myWidthAsPixel=128;
  myHeightAsPixel=64;
*/
//note: scale Commands in drawObject(...)
//glScalef(0.20f, 0.4f, 1.0f);			bIsPlayer1
//glScalef(0.5f, 0.5f, 1.0f);			
	myWidthAsPixel=128*0.2/2;
  myHeightAsPixel=64*0.4/2;
    
	//TO-DO: -update: this
	//note: float xPos as parameter to int myXPosAsPixel not correct output	
	//edited by Mike, 20210528
//	myXPosAsPixel=320;//(int)xPos;
	myXPosAsPixel=(int)xPos;	
	myYPosAsPixel=(int)yPos;
	myZPosAsPixel=(int)zPos;
	
//	printf(">>myXPosAsPixel: %i\n",myXPosAsPixel);

	    
/*
    myWidth=0.5f;
    myHeight=0.5f;
*/
	//added by Mike, 20201001; edited again by Mike, 20201001
	//note: initial position, top-left
/*    myXPos=0.0f;
    myYPos=0+myHeight;//0.1f;
*/

	myXPos=xPos;
    myYPos=yPos;
    myZPos=zPos;

	
/*	//edited by Mike, 20210517
	//added by Mike, 20210514
    myYPos=0.0f;bIsPlayer1
	
	//note: position: 3,3; width, height; count starts at 0
	//edited by Mike, 20210502
//    myXPos=0.0f+myWidth*3;
	//edited by Mike, 20210503
//    myXPos=0.0f-myWidth*10;
	//edited by Mike, 20210514
//    myXPos=0.0f-myWidth*9;
//    myXPos=0.0f;
    myXPos=320.0f;
	
    //edited by Mike, 2020116
//    myYPos=0.0f+myHeight*3;

	//added by Mike, 20210503
	//myZPos updated again in another location
	//edited by Mike, 20210514
//	myZPos=0.0f+myHeight*3;
	myZPos=0.0f;
*/
	
/*	//added by Mike, 20201115; edited by Mike, 20210815
	myWindowWidth=windowWidth;
	myWindowHeight=windowHeight;
*/
	fMyWindowWidth=fWindowWidth;
	fMyWindowHeight=fWindowHeight;
	
	//added by Mike, 20210516
	myUsbongUtils = new UsbongUtils();

//    myWidthX=0.5;

    rotationAngle=0.0f;//360.0f;//90.0;
    rotationStep=10.0;//1.0f;    
/*	//removed by Mike, 20210528    
    thrust=0.0f;
    thrustMax=0.8f;
*/
    
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
	
    //init default values
    //previousFacingState=FACING_UP;
    //currentFacingState=FACING_UP;

	//added by Mike, 20201225
	bIsFiringBeam=false;
	
	//added by Mike, 20210126
//	bIsExecutingDash=false, //removed by Mike, 20210128
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
	
	//added by Mike, 20210502
	//note: set this in OpenGLCanvas.cpp
	bIsPlayer2=false;
	
	//removed by Mike, 20201001; added by Mike, 20210423
	setup();
	
    setCollidable(true);    
}

Ball::~Ball()
{
}

//added by Mike, 20210130
//TO-DO: -reverify: this
float* Ball::getXYZPos()
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
void Ball::draw()
{
	drawRobotShip();
}
*/

//added by Mike, 20210423
void Ball::drawAsQuadWithTexture()
{
	//edited by Mike, 20210516
//    glTranslatef(myXPos, myYPos, myZPos);
	//TO-DO: -add: myZPos
//printf("autoConvert; myXPos: \n",myUsbongUtils->autoConvertFromPixelToVertexPointX(0));
//printf("myXPosAsPixel: %i\n",myXPosAsPixel);
	
	//note: myXPos is float; autoConvertFromPixelToVertexPointX(...) input is int
	//auto-converted from float to int due to autoConvertFromPixelToVertexPointX(...) accepts int as parameter
	//edited by Mike, 20210517
//    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos), myZPos);
//    glTranslatef(myXPosAsPixel, myYPosAsPixel, myZPosAsPixel);

/*  //removed by Mike, 20210601
    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixel), myZPosAsPixel);
    
    //added by Mike, 20210601
    glColor3f(1.0f, 1.0f, 1.0f); // white
    
	drawObject();
*/
/* //removed by Mike, 20210602
    //added by Mike, 20210602
    if (bIsSetForPartnerSpikeAttack) {
        glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixel), myZPosAsPixel);

        //note: causes sky background to flash in white
        
        //added by Mike, 20210601
        glColor3f(1.0f, 1.0f, 1.0f); // white
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, BALL_TEXTURE_A);
        
        drawObject();
        
//        bIsSetForPartnerSpikeAttack=false;
    }
    else {
*/
        glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixel), myZPosAsPixel);
        
        /*
         glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixelBallTrailContainer[0]), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixelBallTrailContainer[0]), myZPosAsPixel);
         
         drawObject();
         */
        
        
        //added by Mike, 20210530; removed by Mike, 20210530
        //-----
        //start at 1, instead of 0 due to 0 is the head ball
        for (int iCountBallTrail=1; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
            //    for (int iCountBallTrail=MAX_BALL_TRAIL-1; iCountBallTrail>=0; iCountBallTrail--) {
            
            //        printf("dito %i: %f\n",iCountBallTrail, fAddXVelBallTrailContainer[iCountBallTrail]);
            //note: glTranslate Command; not set position
            glTranslatef(fAddXVelBallTrailContainer[iCountBallTrail], fAddYVelBallTrailContainer[iCountBallTrail], 0.0f);
            
            //        glColor3f(1.0f, 0.0f, 0.0f); // red
            //        glColor3f(1.0f, 0.0f, 0.0f); // red
            //edited by Mike, 20210601
            //glColor3f(1.0f, 0.3f, 0.0f); // orange
            //        glColor4f(1.0f, 0.3f, 0.0f, 0.5f); // orange
            glColor4f(1.0f, 0.3f, 0.0f, 0.2f*(MAX_BALL_TRAIL-iCountBallTrail)); // orange
            
            //TO-DO: add BALL_TEXTURE_B
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, BALL_TEXTURE_A);
            
            drawObject();
        }
        //-----
        //execute glTranslate to make head ball auto-drawn at top
        for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
            //note: glTranslate Command; not set position
            glTranslatef(-fAddXVelBallTrailContainer[iCountBallTrail], -fAddYVelBallTrailContainer[iCountBallTrail], 0.0f);
        }
        
        //added by Mike, 20210601
        glColor3f(1.0f, 1.0f, 1.0f); // white
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, BALL_TEXTURE_A);
    
        drawObject();
    
/*  //removed by Mike, 20210602
    }
*/
}

//added: by Mike, 20210423
//TO-DO: -add: in PolygonPool
//added by Mike, 20210516
//note: origin/anchor is TOP-LEFT
void Ball::drawObject()
{
/*	
	glBegin(GL_LINES);
		//edited by Mike, 20210514
//		glColor3f(1.0f,0.0f,0.0f); //red
		//sky blue color; brighter
		glColor3f(0.69f, 0.84f, 1.0f);

		//diagonal line left to right
		glVertex2f(-1.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);	
	glEnd();
*/	
	
	//added by Mike, 20210422	myZPos
	glPushMatrix();
		//added by Mike, 20210420; removed by Mike, 20210601
/*		glColor3f(1.0f, 1.0f, 1.0f); // white
//		glColor3f(1.0f, 0.0f, 0.0f); // red
	
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, BALL_TEXTURE_A);		
*/
    
		//edited by Mike, 20210515
		fObjectAnimationFrameOffset=0;
	
		//added by Mike, 20210516; removed to after glScale(...) by Mike, 20210516		
		//due to instructions to auto-draw quad using triangles
//		glTranslatef(0.2f, 0.2f, 0.0f);
	
		//TO-DO: -verify: scaled texture object if equal with pixel width and height size
		//use autoConvertFromPixelToVertexPointX, et cetera if exact
	
		//window width and height; 640x640pixels
		//whole texture image sheet 512x256pixels
		//button size: 64x16pixels
//		glScalef(0.25f, 0.4f, 1.0f);		
		glScalef(0.20f, 0.4f, 1.0f);	
		
		//added by Mike, 20210524
		glScalef(0.5f, 0.5f, 1.0f);	

		
		//added by Mike, 20210516
		//due to instructions to auto-draw quad using triangles
		glTranslatef(1.0f, 0.5f, 0.0f);

	
		glBegin(GL_TRIANGLES);
			//counter-clockwise sequence to auto-draw front face
			//triangle#6 //front face left part
			glNormal3f(0.0000,0.0000,0.0000);
			glTexCoord2f(0.0+fObjectAnimationFrameOffset,0.0);	//A1; face left
			glVertex3f(-1.000000,1.000000,0.000000); //A1

			glNormal3f(0.0000,0.0000,0.0000);
			glTexCoord2f(0.0+fObjectAnimationFrameOffset,1.0);	//C1; face left
			glVertex3f(-1.000000,-1.000000,0.000000); //C1	

			glNormal3f(0.0000,0.0000,0.0000);
			glTexCoord2f(0.25+fObjectAnimationFrameOffset,1.0); //B1; face left
			glVertex3f(1.000000,-1.000000,0.000000); //B1

			//triangle#12 //front face right part		
			glNormal3f(0.0000,0.0000,0.0000);
			glTexCoord2f(0.0+fObjectAnimationFrameOffset,0.0);	//A2; face lefT
			glVertex3f(-1.000000,1.000000,0.000000); //A2

			glNormal3f(0.0000,0.0000,0.0000);
			glTexCoord2f(0.25+fObjectAnimationFrameOffset,1.0); //C2; face left
			glVertex3f(1.000000,-1.000000,0.000000); //C2	
	
			glNormal3f(0.0000,0.0000,0.0000);
			glTexCoord2f(0.25+fObjectAnimationFrameOffset,0.0); //B2; face left
			glVertex3f(1.000000,1.000000,0.000000); //B2
		glEnd();
	
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix(); //added by Mike, 20210422
}

//added by Mike, 20210423
void Ball::update(float dt)
{
    switch (currentState)
    {
           case INITIALIZING_STATE:
           case MOVING_STATE:      
				switch(currentMovingState) {
		           case WALKING_MOVING_STATE:
		           				//added by Mike, 20210527
/*		           				
		           	      myYPosAsPixel+=-stepY;
		           	      myXPosAsPixel+=-stepX;
*/
											//TO-DO: -add: velocity
											
//printf("myYPosAsPixel: %i\n",myYPosAsPixel);
											
											//note: 0,0 origin/anchor at top-left
											//edited by Mike, 20210528
//											if (myYPosAsPixel<=160) {
											//note: max in axis is connected with step x, y, z
											//to create system
											//TO-DO: -create: system to quickly update input values,
											//e.g., max in axis, step x, y, and z
/* //edited by Mike, 20215028
											if (myYPosAsPixel<=100) {												
												bIsMovingDown=true;
											}
				
											if (bIsMovingDown) {
		           	      	myYPosAsPixel+=iStepYAsPixel;
											}
											else {
	           	      		myYPosAsPixel+=-iStepYAsPixel;
											}								
		           	      myXPosAsPixel+=-iStepXAsPixel;	
*/		           	      
/*
                    yAccel = (cos(rotationAngleRad)*thrust);
                    xAccel = -(sin(rotationAngleRad)*thrust);
*/

                    //added by Mike, 20210528
                    //gravity
//										if (yAccel<0) { //going up; pull down
										if (!bIsMovingDown) {
											if (thrust<=0) {
	/*										xAccel+=2.0f;
												yAccel+=2.0f;
	*/
	//											thrust=4.0f;		
																
                    	  //accelerate
                      	thrust+=0.3f;//0.5f;//0.1f;//0.06f;
																					
												iDirectionYAxis=1; //go down											
												bIsMovingDown=true;
										  }	
										}
										
										
										//TO-DO: -update: this
                    //xAccel = thrust;
                    if (bIsSetForPartnerSpikeAttack) {
                        //edited by Mike, 20210602
                    	xAccel = 1.0f; //0.0f
                        //added by Mike, 20210602
/*                        myXPos = 0;
                        myXPosAsPixel = 0;
*/
//                    	iDirectionXAxis = iDirectionXAxis*-1;
                        
                        //edited by Mike, 20210603
                        iDirectionXAxis = iDirectionXAxis;
//                        iDirectionXAxis=iDirectionXAxis*-1;
                        
//                        bIsSetForPartnerSpikeAttack=false; //removed by Mike, 20210602
                    	
//                    	xVel=0.0f;                  	
                    }
                    //added by Mike, 20210603
                    //note: football kick to the bottom stone tile
                    else if (bIsSpikeAttack) {
                        xAccel = 4.0f;
                        yAccel = 1.0f;
                        
                        //edited by Mike, 20210603
                        iDirectionXAxis = iDirectionXAxis;
//                        iDirectionXAxis=iDirectionXAxis*-1;
                        
                        iDirectionYAxis=1; //go down
                        bIsMovingDown=true;
                        
//                        bIsSpikeAttack=false; //removed by Mike, 20210603
                    }
                    else {
                    	xAccel = 4.0f;//thrust;      
                    	
//                    	xVel=iDirectionXAxis*xAccel;//*2.0f;            
                    }
                    
                    yAccel = thrust; //thrust*-1; //go up
                   

										
/*										
										//land
										if (myYPos>320) {
											yVel=0;
										}
										
*/
					//edited by Mike, 20210528
//                    xVel=xAccel*iDirectionXAxis;
										//edited by Mike, 20210601
//                    xVel=iDirectionXAxis*4.0f;//*2.0f;
//
                    xVel=iDirectionXAxis*xAccel;//*2.0f;
                    
//                    xVel=iDirectionXAxis*1.0f;

										//edited by Mike, 20210531
                    yVel=yAccel*iDirectionYAxis;
//                    yVel=0.0f;

//TO-DO: -add: auto-draw rectangle collision detection box
/* //removed by Mike, 20210602
										//added by Mike, 20210602
										if (yVel>=0.0f) {
											bIsMovingDown=true;
										}
*/

/*
     								if (xVel > maxXVel) {
     									xVel = maxXVel;
     								}
                    else if (xVel < -maxXVel) {
                    	xVel = -maxXVel;
                    }
                    
                    if (yVel > maxYVel) {
                    	yVel = maxYVel;
                    }
                    else if (yVel < -maxYVel) {
                    	yVel = -maxYVel;
                    }           
*/

                    myXPos+=xVel;
                    myYPos+=yVel;
                        
                        //added by Mike, 20210530; removed by Mike, 20210530
/*
                        //note: sword/gyro slash output
//-----
                        for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
                            
                            //shift positions in container; put newest 1st
                            for (iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL-1; iCountBallTrail++) {
                                myXPosAsPixelBallTrailContainer[iCountBallTrail+1] = myXPosAsPixelBallTrailContainer[iCountBallTrail];
                            }
                            myXPosAsPixelBallTrailContainer[0] = myXPosAsPixel;
                        }

                        for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
                            
                            //shift positions in container; put newest 1st
                            for (iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL-1; iCountBallTrail++) {
                                myYPosAsPixelBallTrailContainer[iCountBallTrail+1] = myYPosAsPixelBallTrailContainer[iCountBallTrail];
                            }
                            myYPosAsPixelBallTrailContainer[0] = myYPosAsPixel;
                        }
                        
//-----
*/
                        
	           	      myXPosAsPixel+=xVel;
	           	      myYPosAsPixel+=yVel;

//added by Mike 20210530; edited by Mike, 20210531
/*
                        //-----
												for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
                        	myXPosAsPixelBallTrailContainerTemp[iCountBallTrail] = myXPosAsPixelBallTrailContainer[iCountBallTrail];
                        }
                        
												for (int iCountBallTrail=1; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
                            myXPosAsPixelBallTrailContainer[iCountBallTrail] = myXPosAsPixelBallTrailContainerTemp[iCountBallTrail-1];
                        }
                        myXPosAsPixelBallTrailContainer[0] = myXPosAsPixel;                        

												//--                        
												for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
                        	myYPosAsPixelBallTrailContainerTemp[iCountBallTrail] = myYPosAsPixelBallTrailContainer[iCountBallTrail];
                        }
                        
 												for (int iCountBallTrail=1; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
                            myYPosAsPixelBallTrailContainer[iCountBallTrail] = myYPosAsPixelBallTrailContainerTemp[iCountBallTrail-1];
                        	}
                          myYPosAsPixelBallTrailContainer[0] = myYPosAsPixel;                                              
                        //-----
*/                        
                        //-----
/*                        
												for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
                        	myXPosAsPixelBallTrailContainerTemp[iCountBallTrail] = myXPosAsPixelBallTrailContainer[iCountBallTrail];
                        }
*/                        
                        for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
                            //myXPosAsPixelBallTrailContainer[iCountBallTrail] = myXPosAsPixel+iCountBallTrail*0.01f*-1;
//                            [iCountBallTrail] = myXPosAsPixel;//+iCountBallTrail*0.01f*-1;
                            
                            //myXPosAsPixelBallTrailContainer[iCountBallTrail] = myXPosAsPixel-xVel;//+iCountBallTrail*0.01f*-1;
                            //myXPosAsPixelBallTrailContainer[iCountBallTrail] = (myXPosAsPixel-iCountBallTrail/2);//*-1;//+iCountBallTrail*0.01f*-1;

//                            printf("myXPosAsPixel %i: %i\n",iCountBallTrail,myXPosAsPixel);
                            
//                            myXPosAsPixelBallTrailContainer[iCountBallTrail] = myXPosAsPixel;
 
                            fAddXVelBallTrailContainer[iCountBallTrail] = xVel/100.0f*iCountBallTrail/5.0f*-1;

                        }

												//--
                        for (int iCountBallTrail=0; iCountBallTrail<MAX_BALL_TRAIL; iCountBallTrail++) {
                            fAddYVelBallTrailContainer[iCountBallTrail] = yVel/100.0f*iCountBallTrail/5.0f;//*-1;
                        }                        

                        //-----
                        
					if (!bIsMovingDown) {                  
				    	//note: deaccelerate 
                    	if (thrust>0) {
                            thrust-=0.3f;//0.2f;//0.1f; //0.06f; //0.03f; //0.02f
                    	}
                    	else {
                    		thrust=0;
                    	}				           	      
                    }
                    //move down
                    else {
                    	  //accelerate
                      	thrust+=0.2f; //0.3f; //0.5f; //0.03f; //0.02f; //0.01f
                        
                        //added by Mike, 20210528
                        if (thrust<thrustMax) {
                        }
                        else {
                            thrust=thrustMax;
                        }
                    }
                  
	
//printf(">> myYPosAsPixel: %i\n",myYPosAsPixel);
		                break;
		           case ATTACKING_MOVING_STATE:
		            	break;
				   case IDLE_MOVING_STATE:
				   		xVel=0;		
				   		myXPos+=xVel;	
/*	//TO-DO: -add: this				               	
				   		//note: deaccelerate 
                    	if (thrust>0) {
                      		thrust-=0.2f;//0.1f; //0.06f; //0.03f; //0.02f
                    	}
                    	else {
                    		thrust=0;
                    	}		
*/                    	
		          		break;      
		            default: //STANDING STATE		            
		              break;//do nothing    
				}

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
           		else if (myXPos >= myWindowWidth/100) myXPos = 0.0f+myWidth/8; //if right side
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

void Ball::changeState(int s)
{
  currentState=s;                  
}

//added by Mike, 20201226
void Ball::keyDown(int keyCode) {
	myKeysDown[keyCode] = TRUE;

	//added by Mike, 20210127; edited by Mike, 20210128
	autoVerifyDashStateWithKeyDown();//keyCode);
}

//added by Mike, 20201227; edited by Mike, 20210128
//void Ball::setDashStateWithKeyDown() {
void Ball::setDashStateWithKeyDown(int keyCode) {
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

//added by Mike, 20201226; edited by Mike, 20210128
//void Ball::autoVerifyDashStateWithKeyDown(int keyCode) {
void Ball::autoVerifyDashStateWithKeyDown() { //int keyCode) {
	//edited by Mike, 20210128
	//if (myKeysDown[KEY_RIGHT]==TRUE) {
	//edited by Mike, 20210130
//	if ((myKeysDown[KEY_RIGHT]==TRUE) || (myKeysDown[KEY_D]==TRUE)) {		
	if (myKeysDown[KEY_D]==TRUE) {
		setDashStateWithKeyDown(KEY_D);
	}
	//edited by Mike, 20210130
//	else if ((myKeysDown[KEY_UP]==TRUE) || (myKeysDown[KEY_W]==TRUE)) {
	else if (myKeysDown[KEY_W]==TRUE) {
		setDashStateWithKeyDown(KEY_W);//KEY_UP);
	}
	else if (myKeysDown[KEY_A]==TRUE) {
		setDashStateWithKeyDown(KEY_A);
	}
	else if (myKeysDown[KEY_S]==TRUE) {
		setDashStateWithKeyDown(KEY_S);
	}
}

void Ball::keyUp(int keyCode) {
	//added by Mike, 20210127
	autoVerifyDashStateWithKeyUp(keyCode);

	myKeysDown[keyCode] = FALSE;	
}

//added by Mike, 20210127; edited by Mike, 20210126
/*
void Ball::setDashStateWithKeyUp() {
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
//void Ball::setDashStateWithKeyUp() {
void Ball::setDashStateWithKeyUp(int keyCode) {
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
		if (bIsExecutingDashArray[KEY_W]) {
			bIsExecutingDashArray[KEY_W]=false;//KEY_UP);
			bIsDashReady=false;
		}
		else if (bIsExecutingDashArray[KEY_A]) {
			bIsExecutingDashArray[KEY_A]=false;
			bIsDashReady=false;
		}
		else if (bIsExecutingDashArray[KEY_S]) {
			bIsExecutingDashArray[KEY_S]=false;
			bIsDashReady=false;
		}
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
void Ball::autoVerifyDashStateWithKeyUp(int keyCode) {
	//added by Mike, 20210126; edited by Mike, 20210128
//	if (keyCode==KEY_RIGHT) {
	//edited by Mike, 20210130
//	if ((keyCode==KEY_RIGHT) || (keyCode==KEY_D)) {
	if (keyCode==KEY_D) {
		//edited by Mike, 20210128
//		if (myKeysDown[KEY_RIGHT]==TRUE) {
		if (myKeysDown[KEY_D]==TRUE) {
			//edited by Mike, 20210129
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_D);
		}
	}
	//edited by Mike, 20210130
//	else if ((keyCode==KEY_UP) || (keyCode==KEY_W)) {
	else if (keyCode==KEY_W) {
		//edited by Mike, 20210130
//		if ((myKeysDown[KEY_UP]==TRUE) || (myKeysDown[KEY_W]==TRUE)) {
		if (myKeysDown[KEY_W]==TRUE) {
			//edited by Mike, 20210129
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_W);
		}
	}
	else if (keyCode==KEY_A) {
		if (myKeysDown[KEY_A]==TRUE) {
			//edited by Mike, 20210129
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_A);
		}
	}
	else if (keyCode==KEY_S) {
		if (myKeysDown[KEY_S]==TRUE) {
			//edited by Mike, 20210129
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_S);
		}
	}	
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


void Ball::move(int key)
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

   //added by Mike, 20201226; removed by Mike, 20201226
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

        //TO-DO: -add: spike attack
    //removed by Mike, 20210602
/*
       //added by Mike, 20210602
       case KEY_J:
           //removed by Mike, 20210602
           //pulls ball down even without colliding with Pilot;
           //drop ball
           //bIsSetForPartnerSpikeAttack=true;
           myKeysDown[KEY_J]=true;
           break;
       case -KEY_J:
           //removed by Mike, 20210602
           //pulls ball down even without colliding with Pilot;
           //drop ball
           //bIsSetForPartnerSpikeAttack=true;
           myKeysDown[KEY_J]=false;
           break;
*/
           
//	 case KEY_UP: //removed by Mike, 20210130
     case KEY_W:
          //isMovingForward=1;
          //myZPos-=1.0f;
/*          if (thrust<thrustMax)
            thrust+=0.1f;
*/  

	//added by Mike, 20210111
	if (bIsExecutingPunch) {
	}
	//added by Mike, 20210121
	else if (bIsExecutingDefend) {
	}	
	else {    
/*	//removed by Mike, 20210502		
          //added by Mike, 20201001; edited by Mike, 20201116
//	      myYPos+=-stepY;
	      myZPos+=-stepZ;

			//added by Mike, 20210127; edited by Mike, 20210128
//			if (bIsExecutingDash) {
				//edited by Mike, 20210130
				if (bIsExecutingDashArray[KEY_W]) {			
	
	
	//			if ((bIsExecutingDashArray[KEY_UP]) || (bIsExecutingDashArray[KEY_W])) {			
	//			if ((bIsExecutingDashArray[KEY_W])) {
					myZPos+=-stepZ;
				}
*/				
	}
	
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_UP;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
			  	//removed by Mike, 20210425
			   //TO-DO: -add: this
//      		currentFacingState=FACING_UP;		  	
		  }
		  
		  //added by Mike, 20201226; edited by Mike, 20210502
//   		  currentMovingState=WALKING_MOVING_STATE;
		  currentMovingState=IDLE_MOVING_STATE;		    
          break;
          
//     case KEY_DOWN:  //removed by Mike, 20210130
     case KEY_S: //added by Mike, 20210128		   
/*     	  //added by Mike, 20201001
          if (thrust<thrustMax)
            thrust+=-0.1f;
*/

	//added by Mike, 20210111
	if (bIsExecutingPunch) {
	}
	//added by Mike, 20210121
	else if (bIsExecutingDefend) {
	}
	else {
/* //removed by Mike, 20210502		
          //added by Mike, 20201001; edited by Mike, 20201116
//	      myYPos+=stepY;
	      myZPos+=stepZ;

		//added by Mike, 20210127; edited by Mike, 20210128
//			if (bIsExecutingDash) {
		if ((bIsExecutingDashArray[KEY_S])) {		
	    	myZPos+=stepZ;
		}
*/				
	}
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_DOWN;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
			//removed by Mike, 20210425
		    //TO-DO: -add: this			  
          	//currentFacingState=FACING_DOWN;
		  }

		  //added by Mike, 20201226; edited by Mike, 20210502
//   		currentMovingState=WALKING_MOVING_STATE;
			currentMovingState=IDLE_MOVING_STATE;		   
          break;      
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
	else {			
//added by Mike, 20210504		   
/*			printf(">> myXPos: %f\n",myXPos);
			printf(">> stepX: %f\n",stepX);
*/			
		
		//max world left border
/*		//edited  by Mike, 20210504
		  //added by Mike, 20201001            
		  myXPos+=-stepX;

			//added by Mike, 20210127; edited by Mike, 20210128
	//			if (bIsExecutingDash) {
			if ((bIsExecutingDashArray[KEY_A])) {		
				myXPos+=-stepX;
			}
*/		
		//do not execute step if already at border
		if ((myXPos) <= -21.0f) {
		}
		else {
				//added by Mike, 20201001            
				myXPos+=-stepX;

				//added by Mike, 20210127; edited by Mike, 20210128
		//			if (bIsExecutingDash) {
				if ((bIsExecutingDashArray[KEY_A])) {		
					myXPos+=-stepX;
				}
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
//removed by Mike, 20210502			  
/*          	currentFacingState=FACING_LEFT;
*/
		  }

		  //added by Mike, 20201226
   		  currentMovingState=WALKING_MOVING_STATE;
          break;      
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
	else {
/* //edited by Mike, 20210504		
          //added by Mike, 20201001            
	      myXPos+=stepX;

		//added by Mike, 20210126; edited by Mike, 20210128
//			if (bIsExecutingDash) {
			if ((bIsExecutingDashArray[KEY_D])) {
				myXPos+=stepX;
			}
*/
//		printf("myXPos: %f",myXPos);
		//do not execute step if already at border
		if ((myXPos) >= 4.0f) {
		}
		else {
	      	myXPos+=stepX;

			if ((bIsExecutingDashArray[KEY_D])) {
				myXPos+=stepX;
			}
		}		
	}
		   
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_RIGHT;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
          	currentFacingState=FACING_RIGHT;
		  }
		  

		  //added by Mike, 20201226
   		  currentMovingState=WALKING_MOVING_STATE;
		  break;
		//added by Mike, 20201201
		default:
		  currentMovingState=IDLE_MOVING_STATE;
		  bIsFiringBeam=false; //added by Mike, 20201226
		  //removed by Mike, 20210123
		  //bIsExecutingPunch=false; //added by Mike, 20210111
		  
		  bIsExecutingDefend=false; //added by Mike, 20210121
		  break;		  		  
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

	//added by Mike, 20210502
	//TO-DO: -add: FACING_LEFT based on opponent position, e.g. left of pilot

	
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
}

//Reference: https://en.wikipedia.org/wiki/Dead_or_Alive_Xtreme_Beach_Volleyball;
//last accessed: 20210602
//Gameplay: Button Controls Action
void Ball::hitBy(MyDynamicObject* mdo)
{
    
    //TO-DO: -add: animation notification during spike attack

    
		//edited by Mike, 20210527; removed by Mike, 20210527
//		bIsMovingDown=false;
    
    //added by Mike, 20210613
    //TO-DO: -verify: notify Pilot to execute KICK ATTACK animation;
    //observed Ball moves in correct direction after KEY_K attack
//    dynamic_cast<Pilot*>(mdo)->keyDown(KEY_K);
    //removed by Mike, 20210613
/*    dynamic_cast<Pilot*>(mdo)->move(KEY_K);
*/
		//added by Mike, 20210603
    if (dynamic_cast<Pilot*>(mdo)->getIsPlayer1()) {
    	if (iLastHitByPlayerId==PILOT_PLAYER_ID) {
    		return;
    	}
    	else {
    		iLastHitByPlayerId=PILOT_PLAYER_ID;
            //added by Mike, 20210613
//            dynamic_cast<Pilot*>(mdo)->move(KEY_K);
    	}
    }
    if (dynamic_cast<Pilot*>(mdo)->getIsPlayer1Partner()) {
    	if (iLastHitByPlayerId==PILOT_PLAYER_PARTNER_ID) {
    		return;
    	}
    	else {
    		iLastHitByPlayerId=PILOT_PLAYER_PARTNER_ID;
            //added by Mike, 20210613
//            dynamic_cast<Pilot*>(mdo)->move(KEY_K);
    	}
    }
    if (dynamic_cast<Pilot*>(mdo)->getIsPlayer2()) {
    	if (iLastHitByPlayerId==PILOT_PLAYER_TWO_ID) {
    		return;
    	}
    	else {
    		iLastHitByPlayerId=PILOT_PLAYER_TWO_ID;
            //added by Mike, 20210613
//            dynamic_cast<Pilot*>(mdo)->move(KEY_K);
    	}
    }
    if (dynamic_cast<Pilot*>(mdo)->getIsPlayer2Partner()) {
    	if (iLastHitByPlayerId==PILOT_PLAYER_TWO_PARTNER_ID) {
    		return;
    	}
    	else {
    		iLastHitByPlayerId=PILOT_PLAYER_TWO_PARTNER_ID;
            //added by Mike, 20210613
//            dynamic_cast<Pilot*>(mdo)->move(KEY_K);
    	}
    }    
 

		//added by Mike, 20210601
		if (!bIsMovingDown) {
		  return;
		}
    
    //added by Mike, 20210613
    dynamic_cast<Pilot*>(mdo)->move(KEY_K);
    
    //identify which Pilot Player hit the ball
    //added by Mike, 20210601
    if (dynamic_cast<Pilot*>(mdo)->getIsPlayer1()) {
    	if (iPlayer1BallHitCount>1) {
    		setEnd();
    		return;
    	}
    	else {
				iPlayer1BallHitCount++;    	
    	}    	
    }

    if (dynamic_cast<Pilot*>(mdo)->getIsPlayer1Partner()) {
    	if (iPlayer1PartnerBallHitCount>1) {
    		setEnd();
    		return;
    	}
    	else {
				iPlayer1PartnerBallHitCount++;    	
    	}    	
    }

    if (dynamic_cast<Pilot*>(mdo)->getIsPlayer2()) {
    	if (iPlayer2BallHitCount>1) {
    		setEnd();
    		return;
    	}
    	else {
				iPlayer2BallHitCount++;    	
    	}
    }    
       
    if (dynamic_cast<Pilot*>(mdo)->getIsPlayer2Partner()) {
    	if (iPlayer2PartnerBallHitCount>1) {
    		setEnd();
    		return;
    	}
    	else {
				iPlayer2PartnerBallHitCount++;    	
    	}
    }    
    
    //added by Mike, 20210602
    //TO-DO: -update: this
    
/*  //removed by Mike, 20210613
    //edited by Mike, 20210602
    if ((iPlayer1BallHitCount+iPlayer1PartnerBallHitCount) > 3) { //2
			setEnd();
			return;    
    }
   
   	//edited by Mike, 20210602
    if ((iPlayer2BallHitCount+iPlayer2PartnerBallHitCount) > 3) { //2
			setEnd();
			return;    
    }
*/
    
    //added by Mike, 20210602
    //TO-DO: update: to bIsSetForSpikeAttack
    if (bIsSetForPartnerSpikeAttack==true) {
    		//update only direction in x-axis
        iDirectionXAxis=iDirectionXAxis*-1;
    }
    //removed by Mike, 20210603
//    bIsSetForPartnerSpikeAttack=false;

    //reverified: cause why bIsSpikeAttack not executed when KEY_K is pressed; unlike bIsSetForPartnerSpikeAttack with KEY_J
    //due to multiple hits with the same Pilot Player
    
    //TO-DO: -add: execute this if bIsSetForPartnerSpikeAttack == true
    //added by Mike, 20210603
    if (bIsSpikeAttack==true) {
        //TO-DO: -add: auto-update based on directional key pressed
        //update only direction in x-axis
        //edited by Mike, 20210603
       iDirectionXAxis=iDirectionXAxis*-1;
        
/*  //note: causes curving ball, with commented out parts in myKeysDown[KEY_K]
        bIsMovingDown=true;
        iDirectionXAxis=iDirectionXAxis;//*-1;
        iDirectionYAxis=iDirectionXAxis*-1;
*/
    }
    //removed by Mike, 20210603    
//    bIsSpikeAttack=false;

    
/*  //edited by Mike, 20210602
    //TO-DO: -add: verify button pressed
    if (dynamic_cast<Pilot*>(mdo)->getIsPlayer1()) { //player 1 default position bottom left side
    	//set for spike attack
    	//player1 partner
    	bIsSetForPartnerSpikeAttack=true;
    }
*/
    
/*  //removed by Mike, 20210603
    //added by Mike, 20210603
    bIsSetForPartnerSpikeAttack=false;
    bIsSpikeAttack=false;
*/
    
    if ((dynamic_cast<Pilot*>(mdo)->getIsPlayer1())
    || (dynamic_cast<Pilot*>(mdo)->getIsPlayer1Partner())
    || (dynamic_cast<Pilot*>(mdo)->getIsPlayer2())
    || (dynamic_cast<Pilot*>(mdo)->getIsPlayer2Partner()))
    {
    
        //TO-DO: -add: auto-update based on directional key pressed
        //can cause reusable wave motion; viewed as combination of moving top of ball and to the side
/*        if (myKeysDown[KEY_J]) {
            //set for spike attack
            bIsSetForPartnerSpikeAttack=true;
//            iDirectionXAxis=iDirectionXAxis*-1;
    					updateDirection();
        }
*/        
        if (myKeysDown[KEY_J]) {
            //set for spike attack
            bIsSetForPartnerSpikeAttack=true;
//            iDirectionXAxis=iDirectionXAxis*-1;
            //added by Mike, 20210603; removed by Mike, 20210603
//            updateDirection();
        }
        //added by Mike, 20210603
        else {
        	bIsSetForPartnerSpikeAttack=false;
//    updateDirection(); //removed by Mike, 20210603        	
        }
                
        if (myKeysDown[KEY_K]) {
            //spike attack
            bIsSpikeAttack=true;
/*
            //added by Mike, 20210603
            //--
            bIsMovingDown=true;
            iDirectionXAxis=iDirectionXAxis*-1;
            iDirectionYAxis=iDirectionXAxis*-1;
            //--
*/
        }
        //added by Mike, 20210603
        else {
        	bIsSpikeAttack=false;
        }
/*
        //added by Mike, 20210603
        else {
            updateDirection();
        }
*/
    }
    
/*   //removed by Mike, 20210603 
    myKeysDown[KEY_J]=false;
    myKeysDown[KEY_K]=false;
*/
		//added by Mike, 20210602
    if ((dynamic_cast<Pilot*>(mdo)->getIsPlayer1())
     || (dynamic_cast<Pilot*>(mdo)->getIsPlayer1Partner())) {

			iPlayer2BallHitCount=0;
			iPlayer2PartnerBallHitCount=0;
    }

    if ((dynamic_cast<Pilot*>(mdo)->getIsPlayer2())
     || (dynamic_cast<Pilot*>(mdo)->getIsPlayer2Partner())) {

			iPlayer1BallHitCount=0;
			iPlayer1PartnerBallHitCount=0;
    }
 
    //removed by Mike, 20210603
    updateDirection();
		
		//added by Mike, 20210528
		//TO-DO: -add: charge via key held
/*		
    if (thrust<thrustMax) {
//      thrust+=0.1f;      
      thrust+=1.0f;
		}
*/
      //edited by Mike, 20210530
      thrust=thrustMax; //thrustMax*2.0f;
//    thrust=thrustMax*2.0f;
				   
    
/*
     //changeState(DEATH_STATE);
     //setCollidable(false);
    myOpenGLCanvas->loseLife();
 
    //removed by Mike, 20201001
/////	zing = sound->load_sound_clip(RESETSOUND);
////	sound->play_sound_clip(zing);	

    reset();    
*/
    
}

/*	//removed by Mike, 20210522	
void Ball::setOpenGLCanvas(OpenGLCanvas* c)
{
     myOpenGLCanvas = c;
}
*/

void Ball::reset()
{
    //edited by Mike, 20210528
    //changeState(INITIALIZING_STATE);
    currentState=MOVING_STATE;
    currentMovingState=WALKING_MOVING_STATE;
    
     myXPos=320.0f;
     //edited by Mike, 20210528
     //myYPos=320.0f;//0;
     myYPos=240.0f;//0;

     myZPos=0.0f;//0;
    
     myXPosAsPixel=(int)myXPos;
     myYPosAsPixel=(int)myYPos;
     myZPosAsPixel=(int)myZPos;
    
     rotationAngle=0.0f;
     thrust=0.0f;
    
     //edited by Mike, 20210528
     //setCollidable(false);
    setCollidable(true);

    invincibleCounter=0;
    
    //added by Mike, 20210528
    bIsMovingDown=false;
    
    //edited by Mike, 20210606
    iDirectionXAxis=1; //init go to right side from left side
//    iDirectionXAxis=-1; //init go to right side from left side

    iDirectionYAxis=-1; //init go up
    thrustMax=10.0f; //5.0f;//4.0f;
    thrust=1.0f;
    
    //added by Mike, 20210601
    iPlayer1BallHitCount=0;
		iPlayer1PartnerBallHitCount=0;
		iPlayer2BallHitCount=0;
		iPlayer2PartnerBallHitCount=0;    
		
		//added by Mike, 20210602; edited by Mike, 20210606
//    bIsSetForPartnerSpikeAttack=true;//false;
    bIsSetForPartnerSpikeAttack=false;
    
    //added by Mike, 20210603
    bIsSpikeAttack=false;
		iLastHitByPlayerId=-1;        
}

//added by Mike, 20210606
void Ball::bounceFromWindowWall()
{
    //edited by Mike, 20210528
    //changeState(INITIALIZING_STATE);
    currentState=MOVING_STATE;
    currentMovingState=WALKING_MOVING_STATE;
    
/*    
     myXPos=320.0f;
     //edited by Mike, 20210528
     //myYPos=320.0f;//0;
     myYPos=240.0f;//0;
     myZPos=0.0f;//0;
    
     myXPosAsPixel=(int)myXPos;
     myYPosAsPixel=(int)myYPos;
     myZPosAsPixel=(int)myZPos;
*/
    
     rotationAngle=0.0f;
     thrust=0.0f;
    
     //edited by Mike, 20210528
     //setCollidable(false);
    setCollidable(true);

    invincibleCounter=0;
    
    //added by Mike, 20210528
    bIsMovingDown=false;
    
/*    
    //edited by Mike, 20210606
    iDirectionXAxis=1; //init go to right side from left side
//    iDirectionXAxis=-1; //init go to right side from left side

    iDirectionYAxis=-1; //init go up
    thrustMax=10.0f; //5.0f;//4.0f;
    thrust=1.0f;
*/
  
   updateDirection();
   //set Ball to always go up
   iDirectionYAxis=-1; //added by Mike, 20210606
    
    //added by Mike, 20210601
    iPlayer1BallHitCount=0;
		iPlayer1PartnerBallHitCount=0;
		iPlayer2BallHitCount=0;
		iPlayer2PartnerBallHitCount=0;    
		
		//added by Mike, 20210602; edited by Mike, 20210606
//    bIsSetForPartnerSpikeAttack=true;//false;
    bIsSetForPartnerSpikeAttack=false;
    
    //added by Mike, 20210603
    bIsSpikeAttack=false;
		iLastHitByPlayerId=-1;        
}

int Ball::getState()
{
    return currentState;
}

//added by Mike, 20201016
void Ball::destroy()
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
bool Ball::loadTexture(CTargaImage *myTexture, const char *filename, unsigned int *myTextureObject)
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

void Ball::drawTriangledCube(float xPos, float yPos, float zPos)
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

void Ball::drawUpperArm(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		//glColor3f(1.0f, 0.0f, 0.0f);	// red
		glColor3f(1.0f, 1.0f, 1.0f);
        glTranslatef(xPos, yPos, zPos);
		glScalef(0.1f, 0.2f, 0.1f);		
        drawTriangledCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Ball::drawLowerArm(float xPos, float yPos, float zPos)
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
void Ball::drawAntenna(float xPos, float yPos, float zPos)
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

void Ball::drawHead(float xPos, float yPos, float zPos)
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

void Ball::drawBody(float xPos, float yPos, float zPos)
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

void Ball::drawUpperLeg(float xPos, float yPos, float zPos)
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

void Ball::drawLowerLeg(float xPos, float yPos, float zPos)
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

void Ball::drawFoot(float xPos, float yPos, float zPos)
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
void Ball::drawWeapon(float xPos, float yPos, float zPos)
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
