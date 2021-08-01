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
 * @date updated: 20210801
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


//TO-DO: -reverify: in Win7 machine, cause of noticeable delay in action after input Command
//System type: 32-bit Operating System
//Processor: Genuine Intel(R) CPU T2130 @1.86 GHz
//RAM: 3.00 GB

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

#include "Button.h"
/* //TO-DO: -add: these
 #include "PolygonUtils.h"
 #include "ModelPool.h"
 */

//added by Mike, 20210613
#include "Text.h"


//added by Mike, 20210614
#include "Font.h"

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
GLboolean Text::test_pow2(GLushort i)
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
void Text::load_tga(char *filename)
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
void Text::setup()
{
    //removed by Mike, 20201010
    //due to blank output
    //glEnable(GL_DEPTH_TEST);
    
    // select texture 1
    glBindTexture(GL_TEXTURE_2D, TEXT_TEXTURE_A);
    
    /* create OpenGL texture out of targa file */
    //edited by Mike, 20210420
    //    load_tga("textures/armor.tga");
    //    load_tga("textures/imageSpriteExampleMikeWithoutBG.tga");
    load_tga("textures/textExample.tga");
    
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
void Text::setupPrev()
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
//Text::RobotShip(): MyDynamicObject(0,0,300)
//edited by Mike, 20201115
//Text::RobotShip(): MyDynamicObject(0,0,0)
//edited by Mike, 20210625
//Text::Text(float xPos, float yPos, float zPos, int windowWidth, int windowHeight): MyDynamicObject(xPos,yPos,0.0f, windowWidth, windowHeight)
Text::Text(float xPos, float yPos, float zPos, float windowWidth, float windowHeight): MyDynamicObject(xPos,yPos,0.0f, windowWidth, windowHeight)
{
    //edited by Mike, 20201001
    //currentState=IN_TITLE_STATE;//MOVING_STATE;
    currentState=MOVING_STATE;
    //added by Mike, 20201201
    currentMovingState=IDLE_MOVING_STATE;
    
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
    myWidthAsPixel=128;
    myHeightAsPixel=64;
    //TO-DO: -update: this
    //note: float xPos as parameter to int myXPosAsPixel not correct output
    //edited by Mike, 20210613
    myXPosAsPixel=(int)xPos;//320;//(int)xPos;
    myYPosAsPixel=(int)yPos;
    myZPosAsPixel=(int)zPos;
    
    //    printf(">>myXPosAsPixel: %i\n",myXPosAsPixel);
    
    
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
     myYPos=0.0f;
     
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
    
    //added by Mike, 20201115
    myWindowWidth=windowWidth;
    myWindowHeight=windowHeight;
    
    //added by Mike, 20210626
    fMyWindowWidthAsPixelRatioToHeightPixel=1.0f;
    iMyWindowWidthAsPixelOffset=0;
    
    if (myWindowWidth!=myWindowHeight) {
        //added by Mike, 20210626
        //note: causes zero value with myWindowHeight/myWindowWidth;
        //add *1.0f, to be myWindowHeight/(myWindowWidth*1.0f);
        //NOT myWindowHeight/myWindowWidth*1.0f;
        //edited by Mike, 20210628
        //        fMyWindowWidthAsPixelRatioToHeightPixel= 0.56222;
        fMyWindowWidthAsPixelRatioToHeightPixel= myWindowHeight/(myWindowWidth*1.0f);
        
        //note: width value > height value
        //TO-DO: -add: auto-update
        //height: 768; width: 1366
        iMyWindowWidthAsPixelOffset=(myWindowWidth-myWindowHeight)/2;
        //			iMyWindowWidthAsPixelOffset=(myWindowWidth-myWindowHeight);
        
        printf(">>>DITO: %f",fMyWindowWidthAsPixelRatioToHeightPixel);
        printf(">>>DITO: myWindowHeight: %i",myWindowHeight);
        printf(">>>DITO: myWindowWidth: %i",myWindowWidth);
        printf(">>>DITO: iMyWindowWidthAsPixelOffset: %i",iMyWindowWidthAsPixelOffset);
        
        myWindowWidth = myWindowHeight; //myWindowWidthAsPixelInput;
        //    	myWindowHeightAsPixel = myWindowHeightAsPixelInput;
    }
    
    
    //added by Mike, 20210516
    myUsbongUtils = new UsbongUtils();
    myUsbongUtils->setWindowWidthHeight(myWindowWidth, myWindowHeight); //added by Mike, 20210626
    
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
    
    //added by Mike, 20210616
    iTextCurrentMaxColumnCount=0;
    //added by Mike, 20210617
    iTextCurrentMaxRowCount=0;
    
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
    
    
    //added by Mike, 20210723
    bHasReachedEndOfTextMessage=false;
    bHasPressedKeyToCloseEndOfTextMessage=false;
    
    //added by Mike, 20210618
    isAtMaxTextCharRow=false;
    idrawPressNextSymbolCount=0;
    
    //removed by Mike, 20201001; added by Mike, 20210423
    setup();
    
    setCollidable(true);
    
    //added by Mike, 20210614
    //added: function to be reusable
    //  myUsbongUtils->read("inputHalimbawa.txt");
    readInputText("inputHalimbawa.txt");
    
    //added by Mike, 20210614
    setupFont(FONT_TEXTURE);
}

Text::~Text()
{
}

//added by Mike, 20210130
//TO-DO: -reverify: this
float* Text::getXYZPos()
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
 void Text::draw()
 {
	drawRobotShip();
 }
 */

//added by Mike, 20210617
void Text::drawPressNextSymbol()
{
    //	  glScalef(0.20f, 0.4f, 1.0f);
    //    glTranslatef(1.0f, 0.5f, 0.0f);
    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPushMatrix();
    
    //note: 640x640; window width x height
    //edited by Mike, 20210702
    //    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(320), myUsbongUtils->autoConvertFromPixelToVertexPointY(555), 0.0f); //-myZPosAsPixel);
    
    //y-axis origin from bottom
    //add 0.06f as bottom padding
    //edited by Mike, 20210706
    //    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myWindowWidth/2), myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.80f)+0.1f, 0.0f);
    //    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myWindowWidth/2), myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.80f)+0.06f, 0.0f);
    //TO-DO: -reverify: in LinuxOS machine
    //edited by Mike, 20210725
//    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myWindowWidth/2), myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.75f), 0.0f);
    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myWindowWidth/2), myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.95f), 0.0f);
    
    
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    
    //edited by Mike, 20210702
    //	  glScalef(0.08f, 0.08f, 1.0f);
    glScalef(0.06f, 0.06f, 1.0f);
    
    //added by Mike, 20201724
    glScalef(0.5f, 0.5f, 1.0f);
		  
    //auto-scale to Window Width to Height
    //    glScalef(fMyWindowWidthAsPixelRatioToHeightPixel,fMyWindowWidthAsPixelRatioToHeightPixel,1.0f);
    
    glColor3f(1.0f,0.0f,0.0f); //red
    
    
    glBegin(GL_TRIANGLES);
    //counter-clockwise sequence to auto-draw front face
    //front face left part; triangle at 3rd quadrant; angle: right
    glVertex3f(-1.000000,1.000000,0.000000); //A1
    glVertex3f(-1.000000,-1.000000,0.000000); //C1
    glVertex3f(1.000000,-1.000000,0.000000); //B1
    glEnd();
    
    glPopMatrix();
}

//added by Mike, 20210617
void Text::drawTextBackgroundAsQuadWithTexture()
{
    //added by Mike, 20210723
    if (bHasReachedEndOfTextMessage) {
        if(bHasPressedKeyToCloseEndOfTextMessage) {
            return;
        }
    }
    
    
    //added by Mike, 20210614
    //note; add glPushMatrix() and glPopMatrix()
    glPushMatrix();
    
    //edited by Mike, 20210725
    //note: we remove excess glTranslatef(...) and glScalef(...);
    //observation: increase in delay to finish caused by among others:
    //No clear history list of executed glTranslatef(...) and glScalef(...)
    //to a certain computer instruction via file and line number
    //reminder: each glScalef(...) is combined with glTranslatef(...) due to anchor position, e.g. top-left, is also changed
//    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixel), myZPosAsPixel);

    
    //        glScalef(2.5f,5.0f,1.0f);
    //edited by Mike, 20210616
    //        glScalef(2.0f,2.0f,1.0f);
    //        glScalef(2.5f,4.0f,1.0f);
    //edited by Mike, 20210627
    //    glScalef(5.0f,4.0f,1.0f);
    
    //edited by Mike, 20210723
    //    glScalef(5.0f,2.5f,1.0f);
    
/*  //removed by Mike, 20210725
    //edited by Mike, 20210723
    //note: displayed output to be equal
    //edited by Mike, 20210724
    //TO-DO: -reverify: this due to caused by
    //incorrect actual window width and height computation
    //Windows Machine
#ifdef _WIN32
    glTranslatef(0.4f,0.0f,0.0f);
#else
    //in Linux machine, not equal with this instruction
    glTranslatef(-myUsbongUtils->autoConvertFromPixelToVertexPointX(iMyWindowWidthAsPixelOffset), 0.0f, 0.0f);
#endif
    
    //added by Mike, 20201121
#if defined(__APPLE__)
    glTranslatef(-0.02f,0.0f,0.0f);
#endif
*/
    
    //edited by Mike, 20210724; removed by Mike, 20210725
/*    glScalef(3.0f,2.5f,1.0f);
    
    //    glScalef(3.4f,2.5f,1.0f);
    //    glScalef(3.2f,2.5f,1.0f);
    //auto-scale Window Width to Height
    //    glScalef(fMyWindowWidthAsPixelRatioToHeightPixel,1.0f,1.0f);
    
    glTranslatef(0.0f, -0.1f, 0.0f);
*/
    
//    glScalef(fMyWindowWidthAsPixelRatioToHeightPixel,1.0f,1.0f);
    
    
    //TO-DO: -update: draw instructions
    drawTextBackgroundObject();
    
    glScalef(1.0f,1.0f,1.0f);
    
    //added by Mike, 20210613; removed by Mike, 20210725
    //    glTranslatef(-myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel), -myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixel), -myZPosAsPixel);
    
    glPopMatrix();
    
    if (isAtMaxTextCharRow) {
        if ((idrawPressNextSymbolCount)%2==0) {
            drawPressNextSymbol();
        }
        idrawPressNextSymbolCount=idrawPressNextSymbolCount+1;
    }
    
    //added by Mike, 20210614
    //set TOP-LEFT origin/anchor/reference point; quadrant 4, y-axis inverted; x and y positive
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //TOP-LEFT origin
    glOrtho(0.0f, //left
            1.0f, //right
            1.0f, //bottom
            0.0f, //top
            0.0f, //zNear; minimum
            1.0f //zFar; maximum
            );
    
    //added by Mike, 20210626; removed by Mike, 20210628
    //TO-DO: -reverify: this action
    //    glTranslatef(-myUsbongUtils->autoConvertFromPixelToVertexPointX(iMyWindowWidthAsPixelOffset), 0.0f, 0.0f);
    
    //removed by Mike, 20210702
    //auto-scale to Window Width to Height
    //    glScalef(fMyWindowWidthAsPixelRatioToHeightPixel,1.0f,1.0f);
    
    //added by Mike, 20210628
    //TO-DO: -reverify: instructions in macOS due to OK in Linux machine
    
    //note: adds to development time;
    //1) identifying correct positions due to there exist pixel and vertex positions
    //2) identifying axis origin from where to add
    //3) identifying object anchor to which steps are added
    //identified delays shall add computation time during movement in galaxy
    //speed-up via machine tool, e.g. RobotShip, assists Human Pilot
    
    //added by Mike, 20210706
    //note: reminded: difficulty in Math, i.e. Maparaang Pagbibilang,
    //due to no quick feedback; example: no quick output using select inputs
    //computer as machine tool assists in speed-up of output
    
    //edited by Mike, 20210704
    //added +0.02f in x-axis as margin so text not too near left border of text background image
    //y-axis add from window's top
    //    glTranslatef(-myUsbongUtils->autoConvertFromPixelToVertexPointX(iMyWindowWidthAsPixelOffset)+0.02f, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    
    //edited by Mike, 20210705
    //note: output (macOS): -0.4f; window origin left 0.0f; right max 1.0f
    //note: output (LinuxOS): -0.221354f; window origin left 0.0f; right max 1.0f
    //    printf(">>>>> myUsbongUtils->autoConvertFromPixelToVertexPointX(iMyWindowWidthAsPixelOffset): %f\n",myUsbongUtils->autoConvertFromPixelToVertexPointX(iMyWindowWidthAsPixelOffset));
    
    //output (macOS); iMyWindowWidthAsPixelOffset: 240
    //output (LinuxOS); iMyWindowWidthAsPixelOffset: 299
    //    printf(">>>>> iMyWindowWidthAsPixelOffset: %i\n",iMyWindowWidthAsPixelOffset);
    
    //    printf(">>>>> myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel): %f\n",myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel));
    
    //TO-DO: -reverify: in Linux OS
    //add to vertex position x-axis: 0.2f
    //edited by Mike, 20210705
    //    glTranslatef(0.2f, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f); //macOS
    //    glTranslatef(0.24f, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f); //LinuxOS
    
    //    glTranslatef(0.221354f, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    //    glTranslatef(1.0f*0.20f+0.04f, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    //    glTranslatef(0.4f, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    
    //note: used in x-axis input computation, relationship to value of iMyWindowWidthAsPixelOffset
    //edited by Mike, 20210706
    //TO-DO: -reverify: in macOS;
    //    glTranslatef(iMyWindowWidthAsPixelOffset/1000.0f-0.05f, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    //edited by Mike, 20210706
    //    glTranslatef(iMyWindowWidthAsPixelOffset/1000.0f-0.055f, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    //    glTranslatef(iMyWindowWidthAsPixelOffset/1000.0f-0.04f, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    
    //TO-DO: -reverify: in LinuxOS;
    //TO-DO: -reverify: in macOS;
    
    //note: adds to development time, variation in Window Width and Height
    //machine manufacturer auto-verifies if correct display output
    //using own machine's Window Width and Height inputs
    //Is Microsoft's strategy?
    //at present, USBONG verifies display output using available manufactured machines
    
    //reminder: we use auto-scale Window Width to Height
    //edited by Mike, 20210706
    //    glTranslatef(-myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.6f), -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    //    glTranslatef(-myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.62f), -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    
    //note: 0.2f : macOS
    //iMyWindowWidthAsPixelOffset=299;
    //TO-DO: -reverify: computation with another available machine's Window width and height
    //note: (240.0f-299.0f)*(-1)=59.0f
    float fMyWindowWidthAsVertexOffsetInput=(240.0f-iMyWindowWidthAsPixelOffset)*(-1);
    float fMyWindowWidthAsVertexOffset=0.20f+(fMyWindowWidthAsVertexOffsetInput/59.0f*0.04);
    
    //    printf(">>>>>>>>>> fMyWindowWidthAsVertexOffset: %f\n",fMyWindowWidthAsVertexOffset);
    
    //    glTranslatef(-myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*(0.6f)), -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    //    glTranslatef(-myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*(0.6f+fMyWindowWidthAsVertexOffset)), -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    //edited by Mike, 20210725
    //note: from TOP; y-axis
//    glTranslatef(fMyWindowWidthAsVertexOffset, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.65f), 0.0f);
    glTranslatef(fMyWindowWidthAsVertexOffset, -myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.75f), 0.0f);
    
    //auto-scale Window Width to Height
    glScalef(fMyWindowWidthAsPixelRatioToHeightPixel,1.0f,1.0f);
    
    //edited by Mike, 20210626
    glScalef(0.26f,0.26f,1.0f);
    
/* //removed by Mike, 20210801 due to incorrect output using 1366x768 computer monitor; TO-DO: -reverify
    //Windows Machine
#ifdef _WIN32
    //y-axis from bottom
    //glTranslatef(-myUsbongUtils->autoConvertFromPixelToVertexPointX(myWindowWidth*0.01f), myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.01f), 0.0f);
    glTranslatef(0.5f, 0.3f, 0.0f);
    glScalef(0.8f,0.8f,1.0f);

#endif
*/
    //removed by Mike, 20210702
    //    glTranslatef(0.26f, 1.1f, 0.0f); //note: y-axis; inverted; +value goes down
    
    //font
    // select and enable texture FONT_TEXTURE
    glBindTexture(GL_TEXTURE_2D, FONT_TEXTURE);
    glEnable(GL_TEXTURE_2D);
    
    //edited by Mike, 20210615
    //draw text using Font texture
    
    //edited by Mike, 20210617
    //char tempText[50];
    //row, column
    //edited by Mike, 20210618
    //    char tempText[MAX_TEXT_CHAR_ROW][MAX_TEXT_CHAR_COLUMN];
    char tempText[MAX_TEXT_CHAR_ROW_RAM][MAX_TEXT_CHAR_COLUMN];
    
    //		 sprintf(tempText,"USBONG");
    //		 sprintf(tempText,sCurrentTextContainer[0]);
    //edited by Mike, 20210615
    //		 sprintf(tempText,sCurrentTextContainer[0][0].c_str());
    
    //removed by Mike, 20210626
    //    glScalef(0.5f,0.5f,1.0f);
    
    
    //    printf("iTextCurrentMaxRowCount: %i\n",iTextCurrentMaxRowCount);
    
    int iRowCount;
    //edited by Mike, 20210618
    //    for (iRowCount=0; iRowCount<iTextCurrentMaxRowCount; iRowCount++) {
    for (iRowCount=0; iRowCount<iTextCurrentMaxRowCount;) {
        //removed by Mike, 20210617
        //        strcpy(tempText, "");
        
        //        printf("iRowCount: %i\n",iRowCount);
        
        
        //added by Mike, 20210617
        //TO-DO: -reverify: pattern used by computer to add in memory storage;
        //example: pattern based on clock time
        //TO-DO: -reuse: incident; re-verify: if used to auto-generate monsters in select computer game dungeons
        //edited by Mike, 20210617
        //clear tempText container not all cleared due to previously used storage in computer memory causes excess text characters to appear
        //note: select excess text characters NOT in cCurrentTextContainer
        //added by computer in memory storage for use in another set of instructions
        //strcpy(tempText,"");
        //edited by Mike, 20210618
        //        for (int iRowCountToSetDefault=0; iRowCountToSetDefault<MAX_TEXT_CHAR_ROW; iRowCountToSetDefault++) {draw_string
        for (int iRowCountToSetDefault=0; iRowCountToSetDefault<MAX_TEXT_CHAR_ROW_RAM; iRowCountToSetDefault++) {
            for (int iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
                tempText[iRowCountToSetDefault][iColumnCount]='\0'; //verified: in macOS, with Japanese keyboard ro-maji input, "¥0", backspace is "¥"
            }
        }
        
        /*        printf("iTextCurrentMaxColumnCount: %i\n",iTextCurrentMaxColumnCount);
         */
        //edited by Mike, 20210617
        //        for (int iColumnCount=0; iColumnCount<iTextCurrentMaxColumnCount; iColumnCount++) {
        for (int iColumnCount=0; iColumnCount<iCurrentMaxColumnCountPerRowContainer[iRowCount]; iColumnCount++) {
            //edited by Mike, 20210618
            //            tempText[iRowCount][iColumnCount]=cCurrentTextContainer[iRowCount][iColumnCount];
            //note: MAX_TEXT_CHAR_ROW=4
            tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iColumnCount]=cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iColumnCount];
            
            //            printf("cCurrentTextContainer[%i][%i]: %c\n",iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW, iColumnCount, cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iColumnCount]);
        }
        
        //        		printf(">>>>>iRowCount: %i\n",iRowCount);
        //        		printf(">>>>>%s\n",tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
        
        //TO-DO: -add: remaining Font Characters, e.g. small letters, digits
        //TO-DO: -update: font character position in texture image file
        //	   					draw_string(0.1f, 1.2f, 0.0f, tempText);
        draw_string(0.05f, 1.2f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
        
     			//added by Mike, 20210615
     			//new line
     			//edited by Mike, 20210617
        //     			glTranslatef(0.0f,0.1f+0.1f*iRowCount,0.0f);
     			glTranslatef(0.0f,0.1f+0.05f,0.0f);
        
        //edited by Mike, 20210617
        //if ((iTextAnimationCountDelay>=10) || (iTextAnimationCountDelay<=0)){	//TO-DO: -update: MAX delay
        
        /*	//removed by Mike, 20210617
         if ((iTextAnimationCountDelay>=5) || (iTextAnimationCountDelay<=0)){	//TO-DO: -update: MAX delay
         */
        //							iTextAnimationCount+=1;
        iTextAnimationCountDelay=0;
        
        //            printf(">>>>iRowCount: %i; iTextCurrentMaxRowCount: %i\n",iRowCount, iTextCurrentMaxRowCount);
        
        
        //        iTextCurrentMaxColumnCount++;
        //edited by Mike, 20210618; removed by Mike, 20210618
        if ((iRowCount)==(iTextCurrentMaxRowCount-1)) {
            //        if ((iRowCount)==(iTextCurrentMaxRowCount)) {
            //            printf(">\n");
            
            iCurrentMaxColumnCountPerRowContainer[iRowCount]++;
            
            //            printf(">>>>>>>>>>>>>>>>>>>>>>>> DITO\n");
            
            //added by Mike, 20210617
            if (iCurrentMaxColumnCountPerRowContainer[iRowCount]>=MAX_TEXT_CHAR_COLUMN) {
                iCurrentMaxColumnCountPerRowContainer[iRowCount]=MAX_TEXT_CHAR_COLUMN;
            }
            //edited by Mike, 20210618
            /*            else {
            	iCurrentMaxColumnCountPerRowContainer[iRowCount]++;
             }
             */
        }
        
        //added by Mike, 20210617
        //new line/row
        //identify if all characters in row done
        //        printf(">>cCurrentTextContainer[iRowCount][iCurrentMaxColumnCountPerRowContainer[iRowCount]]: %c\n",cCurrentTextContainer[iRowCount][iCurrentMaxColumnCountPerRowContainer[iRowCount]]);
        
        //edited by Mike, 20210618
        //            if (cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iCurrentMaxColumnCountPerRowContainer[iRowCount]]=='\n') {//'\n'){ //new line; "\0" empty character
        if (cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iCurrentMaxColumnCountPerRowContainer[iRowCount]-1]=='\n') {//'\n'){ //new line; "\0" empty character
            
            //            printf("iTextCurrentMaxRowCount-1: %i\n",iTextCurrentMaxRowCount-1);
            //            printf("iRowCount: %i\n",iRowCount);
            
            /*                if (iTextCurrentMaxRowCount>=3) {
             iTextCurrentMaxRowCount=3;
             }
             */
            //                    iTextCurrentMaxRowCount=4;
            
            //TO-DO: -add: instructions to auto-identify end row by removing empty rows after reading input file
            //if next row is already empty
            //row, column
            if (cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iCurrentMaxColumnCountPerRowContainer[iTextCurrentMaxRowCount]]=='\0') {
                iTextCurrentMaxRowCount=iTextCurrentMaxRowCount;
            }
            else {
                if ((iRowCount)==(iTextCurrentMaxRowCount-1)) {
                    iTextCurrentMaxRowCount++;
                }
                
                //added by Mike, 20210618
                //if has reached end of rows, no need to execute this
                //TO-DO: -add: auto-identify if at MAX row
                if (cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW+1][0]=='\0') {
                    printf(">>>>>>>>>>>>>>> WAKAS!\n");
                    
                    bHasReachedEndOfTextMessage=true;
                    
                    break;
                }
                else {
                    if (iRowCount>=MAX_TEXT_CHAR_ROW) {
                        iRowCountPageNumber++;
                        iTextCurrentMaxRowCount=1;
                        iRowCount=-1; //note: we add 1 near the closing of the for loop
                    }
                }
            }
            
            //edited by Mike, 20210618
            //re-set isAtMaxTextCharRow to FALSE after button press
            if ((iRowCount+1)>=MAX_TEXT_CHAR_ROW) {
                iRowCount=3;
                //                			iRowCountPageNumber=0; //removed by Mike, 20210618
                iTextCurrentMaxRowCount=4;
                isAtMaxTextCharRow=true;
            }
            
            //printf("iTextCurrentMaxRowCount: %i\n",iTextCurrentMaxRowCount);
            
            //added by Mike, 20210617
            //TO-DO: fix: next row only iTextCurrentMaxColumnCount=1
            
            
        }
        else {
            break;
        }
        
        //        printf(">>\n");
        
        /*	//removed by Mike, 20210617
         
         }
         */
        iTextAnimationCountDelay+=1;
        
        //added by Mike, 20210618
        iRowCount=iRowCount+1;
        
        
        //     		}
        
    }
				
    glScalef(1.0f,1.2f,1.0f);
				
	   glDisable(GL_TEXTURE_2D);
	   glBindTexture(GL_TEXTURE_2D, 0);
	   
    /*	   //added by Mike, 20210618; removed by Mike, 20210618
     if (isAtMaxTextCharRow) {
     drawPressNextSymbol();
     }
     */
}


//added: by Mike, 20210423
//TO-DO: -add: in PolygonPool
//added by Mike, 20210516
//note: origin/anchor is TOP-LEFT
void Text::drawTextBackgroundObject()
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
    
    //added by Mike, 20210422
    glPushMatrix();
    //added by Mike, 20210420
    glColor3f(1.0f, 1.0f, 1.0f); // white
    //		glColor3f(1.0f, 0.0f, 0.0f); // red
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TEXT_TEXTURE_A);
    
    //edited by Mike, 20210515
    fButtonAnimationFrameOffset=0;
    
    //added by Mike, 20210516; removed to after glScale(...) by Mike, 20210516
    //due to instructions to auto-draw quad using triangles
    //		glTranslatef(0.2f, 0.2f, 0.0f);
    
    //TO-DO: -verify: scaled texture object if equal with pixel width and height size
    //use autoConvertFromPixelToVertexPointX, et cetera if exact
    
/*
    printf(">>iMyWindowWidthAsPixelOffset: %i",iMyWindowWidthAsPixelOffset); //example: 240
    printf(">>myWindowWidth: %i",myWindowWidth); //example 800; //offset already subtracted
*/
    
    //TO-DO: -reverify: in another machine
//     glTranslatef(0.23f, 0.0f, 0.0f); //note: no need to execute glTranslatef(...)
//     glScalef(1.0f-0.23f, 1.0f, 1.0f); //OK in macOS machine
//    glScalef(1.0f-(iMyWindowWidthAsPixelOffset*1.0f)/(myWindowWidth+iMyWindowWidthAsPixelOffset), 1.0f, 1.0f); //OK also in macOS machine
    glScalef(1.0f-(iMyWindowWidthAsPixelOffset*1.0f)/(myWindowWidth), 1.0f, 1.0f); //OK

    //added by Mike, 20210725
    //TO-DO: -reverify: set of instructions with another machine's screen/monitor
    //glTranslatef(0.12f, 0.2f, 0.0f);
//    glTranslatef(0.2f, 0.2f, 0.0f);
//    glTranslatef(0.2f, 0.4f, 0.0f);
    
    //window width and height; 640x640pixels
    //whole texture image sheet 512x256pixels
    //button size: 64x16pixels
    //		glScalef(0.25f, 0.4f, 1.0f);
    //    glScalef(0.20f, 0.4f, 1.0f);
    //    glScalef(0.25f, 0.4f, 1.0f);
//    glScalef(0.28f, 0.4f, 1.0f);
    
    /* //edited by Mike, 20210725
     //added by Mike, 20210516
     //due to instructions to auto-draw quad using triangles
     //    glTranslatef(1.0f, 0.5f, 0.0f);
     glTranslatef(0.4f, 0.5f, 0.0f);
     */
    
    //added by Mike, 20210724
    //note: adding background texture causes delay in execution
    //verified: using GL_TRIANGLES and GL_QUAD
    //note: we can stop Pilot movement due to input Command
    //when display text is executing
    
    //edited by Mike, 20210724
    glBegin(GL_TRIANGLES);
    //counter-clockwise sequence to auto-draw front face
    //triangle#6 //front face left part
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.0+fButtonAnimationFrameOffset,0.0);	//A1; face left
    glVertex3f(-1.000000,1.000000,0.000000); //A1
    
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.0+fButtonAnimationFrameOffset,1.0);	//C1; face left
    glVertex3f(-1.000000,-1.000000,0.000000); //C1
    
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.25+fButtonAnimationFrameOffset,1.0); //B1; face left
    glVertex3f(1.000000,-1.000000,0.000000); //B1
    
    //triangle#12 //front face right part
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.0+fButtonAnimationFrameOffset,0.0);	//A2; face lefT
    glVertex3f(-1.000000,1.000000,0.000000); //A2
    
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.25+fButtonAnimationFrameOffset,1.0); //C2; face left
    glVertex3f(1.000000,-1.000000,0.000000); //C2
    
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.25+fButtonAnimationFrameOffset,0.0); //B2; face left
    glVertex3f(1.000000,1.000000,0.000000); //B2
    glEnd();
    
    
    /*  //added by Mike, 20210724
     glDisable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, 0);
     glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
     
     float fGridTileWidthVertexPosition=1.0f;
     float fGridTileHeightVertexPosition=1.0f;
     //counter-clockwise sequence to auto-draw front face
     glBegin(GL_QUADS); // Each set of 4 vertices form a quad
     //    	glTexCoord2f(fTx, fTy);
     //	    glTexCoord2f(0.0+fButtonAnimationFrameOffset,0.0);	//A1; face left
     //    	glVertex3f(0.0f, 0.0f, 0.0f);
     //    	glVertex3f(-1.0f, 1.0f, 0.0f);
    	glVertex3f(-1.0f, 0.0f, 0.0f);
    	
     //    	glTexCoord2f(fTx + fTileSideXAxis, fTy);
    	glVertex3f(-1.0f, -1.0f, 0.0f);
    	
     //    	glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
    	glVertex3f(1.0f, -1.0f, 0.0f);
    	
     //    	glTexCoord2f(fTx, fTy + fTileSideYAxis);
     //    	glVertex3f(1.0f, 1.0f, 0.0f);
    	glVertex3f(1.0f, 0.0f, 0.0f);
     glEnd();
     */
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix(); //added by Mike, 20210422
}

//added by Mike, 20210423
void Text::update(float dt)
{
    switch (currentState)
    {
        case INITIALIZING_STATE:
        case MOVING_STATE:
            switch(currentMovingState) {
                case WALKING_MOVING_STATE:
                    break;
                case ATTACKING_MOVING_STATE:
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

void Text::changeState(int s)
{
    currentState=s;
}

//added by Mike, 20201226
void Text::keyDown(int keyCode) {
    myKeysDown[keyCode] = TRUE;
    
    //added by Mike, 20210619
    //TO-DO: -reverify: output of over 6 rows in input file
    if (myKeysDown[KEY_K]==TRUE) {
        //edited by Mike, 20210723
        if (bHasReachedEndOfTextMessage) {
            bHasPressedKeyToCloseEndOfTextMessage=true;
        }
        else {
            if (isAtMaxTextCharRow) {
                isAtMaxTextCharRow=false;
                
                iRowCountPageNumber++;
                iTextCurrentMaxRowCount=1;
                
                //next row; reminder: MAX_TEXT_CHAR_ROW=4
                for(int iCount=0; iCount<MAX_TEXT_CHAR_ROW; iCount++) {
                    iCurrentMaxColumnCountPerRowContainer[iCount]=1;
                }
            }
        }
    }
    
    //removed by Mike, 20210619
    //added by Mike, 20210127; edited by Mike, 20210128
    //    autoVerifyDashStateWithKeyDown();//keyCode);
}

//added by Mike, 20201227; edited by Mike, 20210128
//void Text::setDashStateWithKeyDown() {
void Text::setDashStateWithKeyDown(int keyCode) {
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
//void Text::autoVerifyDashStateWithKeyDown(int keyCode) {
void Text::autoVerifyDashStateWithKeyDown() { //int keyCode) {
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

void Text::keyUp(int keyCode) {
    //added by Mike, 20210127
    autoVerifyDashStateWithKeyUp(keyCode);
    
    myKeysDown[keyCode] = FALSE;
}

//added by Mike, 20210127; edited by Mike, 20210126
/*
 void Text::setDashStateWithKeyUp() {
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
//void Text::setDashStateWithKeyUp() {
void Text::setDashStateWithKeyUp(int keyCode) {
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
void Text::autoVerifyDashStateWithKeyUp(int keyCode) {
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


void Text::move(int key)
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
void Text::hitBy(MyDynamicObject* mdo)
{
    //changeState(DEATH_STATE);
    //setCollidable(false);
    myOpenGLCanvas->loseLife();
    
    //removed by Mike, 20201001
    /*
     zing = sound->load_sound_clip(RESETSOUND);
     sound->play_sound_clip(zing);
     */
    reset();
}

/*	//removed by Mike, 20210522
 void Text::setOpenGLCanvas(OpenGLCanvas* c)
 {
 myOpenGLCanvas = c;
 }
 */

void Text::reset()
{
    changeState(INITIALIZING_STATE);
    myXPos=0;
    myYPos=0;
    rotationAngle=0.0f;
    thrust=0.0f;
    setCollidable(false);
    invincibleCounter=0;
}
int Text::getState()
{
    return currentState;
}

//added by Mike, 20201016
void Text::destroy()
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
bool Text::loadTexture(CTargaImage *myTexture, const char *filename, unsigned int *myTextureObject)
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

void Text::drawTriangledCube(float xPos, float yPos, float zPos)
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

void Text::drawUpperArm(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    //glColor3f(1.0f, 0.0f, 0.0f);	// red
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(xPos, yPos, zPos);
    glScalef(0.1f, 0.2f, 0.1f);
    drawTriangledCube(0.0f, 0.0f, 0.0f);
    glPopMatrix();
}

void Text::drawLowerArm(float xPos, float yPos, float zPos)
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
 void Text::drawAntenna(float xPos, float yPos, float zPos)
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

void Text::drawHead(float xPos, float yPos, float zPos)
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

void Text::drawBody(float xPos, float yPos, float zPos)
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

void Text::drawUpperLeg(float xPos, float yPos, float zPos)
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

void Text::drawLowerLeg(float xPos, float yPos, float zPos)
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

void Text::drawFoot(float xPos, float yPos, float zPos)
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
void Text::drawWeapon(float xPos, float yPos, float zPos)
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


//added by Mike, 20210614
//note: error occurs if excess rows > 1
//TO-DO: -fix: this
void Text::readInputText(char *inputFilename) {
    int c;
    FILE *file;
    
    //TO-DO: update: this
    //	char** iCurrentLevelMapContainer = new char[100][100];
    int iRowCount=0;
    int iColumnCount=0;
    
    //TO-DO: -update: this
    /*	int MAX_TEXT_CHAR_ROW=2;
     int MAX_TEXT_CHAR_COLUMN=8;
     */
    
    //edited by Mike, 20210618
    for (iRowCount=0; iRowCount<MAX_TEXT_CHAR_ROW; iRowCount++) {
        //    for (iRowCount=0; iRowCount<MAX_TEXT_CHAR_ROW_RAM; iRowCount++) {
        for (iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
            //edited by Mike, 20210616
            //sCurrentTextContainer[iRowCount][iColumnCount]=(char*)"-1";//'G';
            //edited by Mike, 20210617
            //            cCurrentTextContainer[iRowCount][iColumnCount]='\n';
            //            cCurrentTextContainer[iRowCount][iColumnCount]='\0';
            
            //added by Mike, 20210617
            iCurrentMaxColumnCountPerRowContainer[iRowCount]=1;
        }
    }
    
    //added by Mike, 20210618
    for (iRowCount=0; iRowCount<MAX_TEXT_CHAR_ROW_RAM; iRowCount++) {
        for (iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
            cCurrentTextContainer[iRowCount][iColumnCount]='\0';
        }
    }
    
    
    
    /*
     //added by Mike, 20210618
     for (iRowCount=0; iRowCount<MAX_TEXT_CHAR_ROW_RAM; iRowCount++) {
     for (iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
     iCurrentMaxColumnCountPerRowContainer[iRowCount]=1;
     }
     }
     */
    
    iRowCount=0;
    iColumnCount=0;
    
    //added by Mike, 20210617
    iTextCurrentMaxRowCount=1;
    
    //added by Mike, 20210618
    iRowCountPageNumber=0; //start at zero
    
				
    //noted by Mike, 20201210
    //note: if concatenated string exceeds size, "stack smashing detected"; terminated; Aborted (core dumped)
    //I prefer to set a size, instead of dynamically allocate due to increasing likelihood of memory leaks
    //where memory leaks = not deallocated storage in memory, albeit not used by software application
    //identifying not deallocated storage in memory becomes more difficult with increasing use
    //edited by Mike, 20210615
    //	char input[MAX_TEXT_CHAR_COLUMN]; //max size
    //TO-DO: -update: this; if total size of the input filename > total size of container, in macOS abort trap 6 error
    char input[100]; //max size in Char of input filename
    
    char inputTextLine[MAX_TEXT_CHAR_COLUMN]; //max size
    char tempInputTextLine[MAX_TEXT_CHAR_COLUMN]; //max size
    
    //added by Mike, 20210618
    //TO-DO: -add: auto-notify Unit member if input over max
    
    strcpy(input, "input/");
    strcat(input, inputFilename); //already includes .txt
    //	strcat(input,".txt");
    
    //	printf("dito: %s",input);
    
    //	file = fopen("input/"+inputFilename, "r"); //.txt file
    //	file = fopen("input/inputHalimbawa.txt", "r"); //.txt file
    file = fopen(input, "r"); //.txt file
    
    //TO-DO: -reverify: tile positions
    int iCount=0;
    
    strcpy(tempInputTextLine,""); //added by Mike, 20210615
    
    if (file) {
        //edited by Mike, 20210210
        //		while ((c = getc(file)) != EOF) {
        //edited by Mike, 20210516
        //		while (fgets (input, MAX_TEXT_CHAR_COLUMN, file)) { /* read each line of input */
        while (fgets (inputTextLine, MAX_TEXT_CHAR_COLUMN, file)) { /* read each line of input */
            
            //	putchar(c);
            
            /*	//removed by Mike, 20210210
             char sRow[2] = {(char)c};
             */
            //delimited using new line
            /*			char *chRow = strtok(sRow, "\n");
             */
            
            //            printf(">>> input: %s\n",input);
            //			sscanf (input, "%s", inputTextLine);
            
            //			iCount=0;
            //input text per line
            //			printf("%i;\n",iCount);
            //			printf("%i;",iCount);
            
            iCount=iCount+1;
            
            //added by Mike, 20210208
            iColumnCount=0;
            
            //removed by Mike, 20210210
            /*
             //			char s[2] = {0};
             //			*s = c;
             //edited by Mike, 20210209
             //				char s[2] = {c};
             //				char s[2] = {itoa(c)};
             char s[2] = {(char)c};
             
             //delimited using comma
             char *ch = strtok(s, ",");
             */
            //edited by Mike, 20210616
            //TO-DO: -verify: add tool for computer to notify Unit member if max characters per row already exceeded
            //at present, 19 characters including space
            //TO-DO: -add: auto-put excess characters in next row
            //TO-DO: -fix: text background image texture file after scale up
            //TO-DO: -add: remaining Font characters
            //TO-DO: -update: Font characters to use handwritting
            //TO-DO: -reverify: max rows due to text length long in Filipino language
            //TO-DO: -add: auto-write text per character
            //TO-DO: -add: auto-pause for text with several rows
            strcpy(tempInputTextLine,inputTextLine);
            
            printf(">>> inputTextLine: %s\n",inputTextLine);
            
            
            /* 	//edited by Mike, 20210616
             //note: add "-1" for empty
             //otherwise, comma as column is skipped
             //edited by Mike, 20210615
             //			char *ch = strtok(tempInputTextLine, ",");
             char *ch = strtok(tempInputTextLine, "\n");
             
             while (ch != NULL) {
             //				printf("%i,",iColumnCount);
             MAX_TEXT_CHAR_ROW_RAM
             //TO-DO: use String, instead of char
             //TO-DO: -reverify: output due to "G" not put in container
             //				sCurrentLevelMapContainer[iRowCount][iColumnCount]=&ch;
             sCurrentTextContainer[iRowCount][iColumnCount]=ch;
             //				printf("%i:",iColumnCount);
             //				printf("%s,",ch);
             
             iColumnCount=iColumnCount+1;
             
             //edited by Mike, 20210615
             //				  ch = strtok(NULL, ",");
             ch = strtok(NULL, "\n");
             }
             
             */
            
            //added by Mike, 20210617
            /*            if (cCurrentTextContainer[iRowCount][iCharCount]=='\0') {
             break;
             }
             */
            
            //added by Mike, 20210617
            //TO-DO: -add: trim to input text line
            
            int iCharCount;
            for (iCharCount=0; iCharCount<strlen(tempInputTextLine); iCharCount++) {
                cCurrentTextContainer[iRowCount][iCharCount]=tempInputTextLine[iCharCount];
                
                printf("cCurrentTextContainer[%i][%i]: %c",iRowCount, iCharCount, tempInputTextLine[iCharCount]);
            }
            //added by Mike, 20210617
            cCurrentTextContainer[iRowCount][iCharCount]='\n';
            
            
            
            iColumnCount=iColumnCount+1;
            
            //removed by Mike, 20210617
            //edited by Mike, 20210311
            //			if (iRowCount<100) {
            //edited by Mike, 20210321
            //			if (iRowCount<160) {
            //edited by Mike, 20210618
            //            if (iRowCount<MAX_TEXT_CHAR_ROW) {
            if (iRowCount<MAX_TEXT_CHAR_ROW_RAM) {
                iRowCount=iRowCount+1;
                
                //TO-DO: -notify: Unit member if over MAX_TEXT_CHAR_ROW_RAM
            }
            else {
                iRowCount=0;
            }
            
            //removed by Mike, 20210617
            //            iTextCurrentMaxRowCount=iTextCurrentMaxRowCount+1;
            
            printf("\n");
        }
        fclose(file);
        
        //added by Mike, 20210615
        //        free(tempInputTextLine);
    }
}

//--------------------------------------------
