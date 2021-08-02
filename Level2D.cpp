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
 * @date updated: 20210802
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

//added by Mike, 20210719
#include <iostream>

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
#include "Level2D.h"


//added by Mike, 20210614; removed by Mike, 20210703
//#include "Font.h"

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
GLboolean Level2D::test_pow2(GLushort i)
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
void Level2D::load_tga(char *filename)
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

//edited by Mike, 20201001
//Level2D::RobotShip(): MyDynamicObject(0,0,300)
//edited by Mike, 20201115
//Level2D::RobotShip(): MyDynamicObject(0,0,0)
//edited by Mike, 20210625
//Level2D::Text(float xPos, float yPos, float zPos, int windowWidth, int windowHeight): MyDynamicObject(xPos,yPos,0.0f, windowWidth, windowHeight)
Level2D::Level2D(float xPos, float yPos, float zPos, float windowWidth, float windowHeight): MyDynamicObject(xPos,yPos,0.0f, windowWidth, windowHeight)
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
/*	//edited by Mike, 20210728    
    myWidthAsPixel=128;
    myHeightAsPixel=64;
*/

/*		//removed by Mike, 20210720
    //TO-DO: -add: auto-set width and height based on grid tile
    myWidthAsPixel=71;
    myHeightAsPixel=80;    
*/
    
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
     /*        if (mdo->getCurrentFacing()==FACING_UP) {
        	mdo->setYPosAsPixel(iTileYPos +this->getHeightAsPixel()+1);
        }
        else if (mdo->getCurrentFacing()==FACING_DOWN) {
        	mdo->setYPosAsPixel(iTileYPos -mdo->getHeightAsPixel()-1);
        }        
        else if (mdo->getCurrentFacing()==FACING_LEFT) {
        	mdo->setXPosAsPixel(iTileXPos +this->getWidthAsPixel()+1);    	
        }
        else if (mdo->getCurrentFacing()==FACING_RIGHT) {
        	mdo->setXPosAsPixel(iTileXPos -mdo->getHeightAsPixel()-1);      	
        }      
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
    
    //TO-DO: -add: eliminate multiple sets of same instructions;
    //e.g. exist in both Level2D.cpp and OpenGLCanvas.cpp
    /* //removed by Mike, 20210712
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
     */
     
    //edited by Mike, 20210724
    iRowCountMax=10;
    iColumnCountMax=18;
    iHeightCountMax=10;
    
    
    /*
     fGridSquareWidth = myWindowWidthAsPixel/iColumnCountMax; //example: 136.60
     fGridSquareHeight = myWindowHeightAsPixel/iRowCountMax; //example: 76.80
     */
    
//    printf("myWindowWidth: %i",myWindowWidth);
    
    fGridSquareWidth = myWindowWidth/iColumnCountMax; //example: 136.60
    fGridSquareHeight = myWindowHeight/iRowCountMax; //example: 76.80
    
    //auto-set width and height based on grid tile
    myWidthAsPixel=fGridSquareWidth;
    myHeightAsPixel=fGridSquareHeight;    
    
    
    /*
     printf("fGridSquareWidth: %f\n",fGridSquareWidth);
     printf("fGridSquareHeight: %f\n",fGridSquareHeight);
     */
    //added by Mike, 20210626
    fMyWindowWidthAsPixelRatioToHeightPixel=1.0f;
    iMyWindowWidthAsPixelOffset=0; //added by Mike, 20210701
    
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
 
    //removed by Mike, 20210423
    /*	loadTexture(myBodyTexture, "bodyTexture.tga", &myBodyTextureObject);
     loadTexture(myHeadTexture, "headTexture.tga", &myHeadTextureObject);
     */
    
    //added by Mike, 20210618
    isAtMaxTextCharRow=false;
    idrawPressNextSymbolCount=0;
    
    //removed by Mike, 20201001; added by Mike, 20210423;
    //removed by Mike, 20210722
//    setup();
    
    setCollidable(true);
    
    //added: function to be reusable
    //    readInputText("inputHalimbawa.txt");
    //edited by Mike, 20210712
    //    readInputText("inputLevel1.csv");
    read("inputLevel1.csv");
    
    //edited by Mike, 20210707
    setupLevel(LEVEL_2D_TEXTURE); //LEVEL_TEXTURE
}

Level2D::~Level2D()
{
}

void Level2D::setupLevel(int myLevelTextureObject)
{
    //removed by Mike, 20201010
    //due to blank output
    //glEnable(GL_DEPTH_TEST);
    
    iLevelTextureObject = myLevelTextureObject;
    
    /* select texture 1 */
    glBindTexture(GL_TEXTURE_2D, iLevelTextureObject);
    
    /* create OpenGL texture out of targa file */
    //edited by Mike, 20210420
    load_tga("textures/level2D.tga");
    //    load_tga("textures/concrete.tga");
    
    /* set texture parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
       
/*	//edited by Mike, 20210722; this is due to displayed image is blurred
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);                    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);                    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
    
    /* unselect texture myFontTextureObject */
    glBindTexture(GL_TEXTURE_2D, 0);
    
    /* setup alpha blending */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
/* //removed by Mike, 20210722    
    // set background color to bluish //set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // unselect texture myFontTextureObject
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // setup alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
*/    
    //removed by Mike, 20201012
    /* set background color to bluish to demonstrate font transparency */
    //    glClearColor(0.0f, 0.0f, 0.25f, 1.0f); /* to demonstrate font transparency */
    
}

//added by Mike, 20210130
//TO-DO: -reverify: this
float* Level2D::getXYZPos()
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
 void Level2D::draw()
 {
	drawRobotShip();
 }
 */

//added by Mike, 20210617
void Level2D::drawPressNextSymbol()
{
    //	  glScalef(0.20f, 0.4f, 1.0f);
    //    glTranslatef(1.0f, 0.5f, 0.0f);
    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPushMatrix();
    
    //note: 640x640; window width x height
    //edited by Mike, 20210702
    //    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(320), myUsbongUtils->autoConvertFromPixelToVertexPointY(555), 0.0f); //-myZPosAsPixel);
    
    /* //removed by Mike, 20210708
     //y-axis origin from bottom
     //add 0.06f as bottom padding
     //    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myWindowWidth/2), myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.80f)+0.1f, 0.0f);
     glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myWindowWidth/2), myUsbongUtils->autoConvertFromPixelToVertexPointY(myWindowHeight*0.80f)+0.06f, 0.0f);
     */
    
    /*
     //added by Mike, 20210708
     //TO-DO: -reverify: with another Machine Window
     //set object anchor to top-left
     //added by Mike, 20210708
     #if defined(__APPLE__)		//note: *2; where 2 = rowCount
     //note: +0.01 in y-axis due to thickness of grid line; TO-DO: -reverify: this using another machine
     //    glTranslatef((0.033f+0.006f)*2, 0.0f-0.01f, 0.0f);
     //    glTranslatef(0.0f+0.006f, 0.0f-0.01f, 0.0f);
     //    glTranslatef((-0.01f), 0.0f-0.01f, 0.0f);
     #else
    	glTranslatef(+0.02f, 0.0f, 0.0f);
     #endif
     //    glTranslatef(-1.0f/2, -1.0f/2, 0.0f);
     //    glTranslatef(1.0f, 0.0f, 0.0f);
     glTranslatef(0.06f, 0.0f, 0.0f);
     glTranslatef(0.55f, 0.0f, 0.0f);
     */
    //TO-DO: -add: auto-compute translate value, e.g. 0.04f
    //    glTranslatef(0.04f, 0.0f, 0.0f);
    //note: 0.033f = 0.06f*0.55f; //due to scale Commands
    //note: +0.006 in x-axis due to thickness of grid line; TO-DO: -reverify: this using another machine
    //note: *2; where 2 = rowCount
    //note: there exists thickness of grid line
    //    glTranslatef(-0.06f*0.55f*3, 0.0f, 0.0f);
    
    
    //edited by Mike, 20210708
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    //    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    
    //edited by Mike, 20210702; edited by Mike, 20210708
    glScalef(0.06f, 0.06f, 1.0f);
    //	  glScalef(0.06f, 0.06f*0.55f, 1.0f);
    //	  glScalef(0.06f*0.55f, 0.06f*0.55f, 1.0f);
    
    //iRowCountMax: 18; iColumnCountMax: 10;
    //10/18=0.55...;
    
    //note: we can identify patterns in the instructions to execute
    //3D techniques, e.g. scale; via the Open Graphics Library (OpenGL)
    //note: multiple scale Commands = multiplication Commands
    //example variation#1:
    //glScalef(0.06f, 0.06f, 1.0f);
    //glScalef(0.55f, 0.55f, 1.0f);
    //example variation#2; 0.06f*0.55f
    //glScalef(0.06f*0.55f, 0.06f*0.55f, 1.0f);
    
    glScalef(0.55f, 0.55f, 1.0f);
    //	  glScalef(0.055f, 0.055f, 1.0f);
    
    glColor3f(1.0f,0.0f,0.0f); //red
    
    glBegin(GL_TRIANGLES);
    //counter-clockwise sequence to auto-draw front face
    //front face left part; triangle at 3rd quadrant; angle: right
    glVertex3f(-1.000000,1.000000,0.000000); //A1
    glVertex3f(-1.000000,-1.000000,0.000000); //C1
    glVertex3f(1.000000,-1.000000,0.000000); //B1
    glEnd();
    
    /*
     //note: use of 3D techniques, e.g. translate, rotate, scale,
     //adds to development time, due to display output verification;
     //fast computers speed-up development time
     
     //added by Mike, 20210708
     glBegin(GL_TRIANGLES);
    	//counter-clockwise sequence to auto-draw front face
    	//front face left part; triangle at 3rd quadrant; angle: right
    	glVertex3f(0.000000,1.000000,0.000000); //A1
    	glVertex3f(-2.000000,-1.000000,0.000000); //C1
    	glVertex3f(1.000000,-1.000000,0.000000); //B1
     glEnd();
     */
    
    glPopMatrix();
}


//edited by Mike, 20210712
void Level2D::drawTileAsQuadWithoutTexture()
{
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPushMatrix();
    
    //glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    //  glColor3f(0.0f, 0.0f, 0.0f); //set to default, i.e. black
    glColor3f(1.0f, 0.0f, 0.0f); //red
    
    //TO-DO: -add: tile with auto-drawn pattern; without using image texture object
    //  	printf (">>>>>>>>>>>>>>>>>>>>>>>>>>>> HALLO");
    
    //  	printf (">>> fGridSquareWidth: %f; fGridSquareHeight: %f",fGridSquareWidth,fGridSquareHeight);
    
    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
    //TO-DO: -reverify: cause of /2.0f/4.0f; and 2.0f/2.0f;
    //		fGridTileWidthVertexPosition = fGridTileWidthVertexPosition/2.0f/3.0f;
    fGridTileWidthVertexPosition = fGridTileWidthVertexPosition/2.0f/4.0f;
    fGridTileHeightVertexPosition = fGridTileHeightVertexPosition/2.0f/2.0f;
    
    //added by Mike, 20210713
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
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //note: vertex position sequence to be auto-drawn
    //counter-clockwise sequence to auto-draw front face
    
    //add this due to 3rd quadrant
    //size of tile width: 0.1f
    //		glTranslatef(-0.1f-0.05f, 0.0f, 0.0f);
    //re-verify: cause of 0.01f; due to Linux machine?
    //		glTranslatef(-0.1f-0.05f-0.01f, 0.0f, 0.0f);
    
    //TO-DO: -update: this
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    //        glTexCoord2f(tx, ty);
    glVertex3f(0.0f, 0.0f, 0.0f);
    
    //        glTexCoord2f(tx + 0.078125f, ty);
    glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    
    //        glTexCoord2f(tx + 0.078125f, ty + 0.125f);
    glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    
    //				glTexCoord2f(tx, ty + 0.125f);
    glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
    
    glPopMatrix();
}

//edited by Mike, 20210716; edited by Mike, 20210719
//void Level2D::drawTileAsQuadWithTexture()
//edited by Mike, 20210720
//void Level2D::drawTileAsQuadWithTexture(std::string sTileId)
void Level2D::drawTileAsQuadWithTexture(std::string sTileId)
{
    
    glBindTexture(GL_TEXTURE_2D, iLevelTextureObject);
    glEnable(GL_TEXTURE_2D);
    
    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625
    
    /*
     glDisable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, 0);
     */
    
    glPushMatrix();

/*	//removed by Mike, 20210724    
    //  glColor3f(0.0f, 0.0f, 0.0f); //set to default, i.e. black
    //edited by Mike, 20210717
    //  	glColor3f(1.0f, 0.0f, 0.0f); //red
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
*/
    
    //TO-DO: -add: tile with auto-drawn pattern; without using image texture object
    //  	printf (">>>>>>>>>>>>>>>>>>>>>>>>>>>> HALLO");
    
//      	printf (">>> fGridSquareWidth: %f; fGridSquareHeight: %f",fGridSquareWidth,fGridSquareHeight);
    
/* //edited by Mike, 20210723; due to insufficient texture pixel for the tile    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
*/

//edited by Mike, 20210725
/*
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth+1);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight+1);
*/
    
//    printf(">>Level2D; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
//    printf(">>Level2D; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
		//TO-DO: -reverify: this
		//edited by Mike, 20210720
//    fGridTileWidthVertexPosition = fGridTileWidthVertexPosition*2.0f; //removed by Mike, 20210720
//	  fGridTileHeightVertexPosition = fGridTileHeightVertexPosition*4.0f;//4.0f;
 
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //added by Mike, 20210713
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
    
    //note: vertex position sequence to be auto-drawn
    //counter-clockwise sequence to auto-draw front face
    
    //add this due to 3rd quadrant
    //size of tile width: 0.1f
    //		glTranslatef(-0.1f-0.05f, 0.0f, 0.0f);
    //re-verify: cause of 0.01f; due to Linux machine?
    //		glTranslatef(-0.1f-0.05f-0.01f, 0.0f, 0.0f);

    //edited by Mike, 20210801
    glTranslatef(0.0f, fGridTileHeightVertexPosition, 0.0f);
//    glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
//    glTranslatef(0.0f, fGridTileHeightVertexPosition*2, 0.0f);
//    glTranslatef(fGridTileWidthVertexPosition, fGridTileHeightVertexPosition, 0.0f);

    //added by Mike, 20210717
    //		glRotatef(180, 1.0f, 0.0f, 0.0f);
    
    float fTx = 0.0f;
    float fTy = 0.0f;
    
    //added by Mike, 20210719
    //dash delimiter
    //example: 1-1 = A1, i.e. Column A Row 1
//    char *ch = strtok(sTileId, "-"); //note: error due to sTileId NOT classified to be char*
//    std::vector<std::string> vString = std::strtok(sTileId, "-");
		//note: sTileId includes quotation marks
		//example: "0-0"		

		//TO-DO: -update: this		
/*		
		char *ch = strdup(sTileId.c_str());
		char *cStarToken = strtok(ch, "-");
		free(ch);
*/		
		
//		char *cStarToken="10";

//		printf(">>ch: %s\n",ch);

//note: outputs
//sCurrentLevelMapContainer[iRowCount][iColumnCount]: "1-0"
//>>ch: � FQ�U

//TO-DO: -add: auto-replace quotation marks, et cetera
/*		
	char *cStarToken="";
	
	cStarToken[0]=sTileId[1];
	cStarToken[1]=sTileId[3];
*/	

		
//		printf(">>cStarToken[0]: %i; cStarToken[1]: %i\n",(cStarToken[0]-'0'),(cStarToken[1]-'0'));
//		printf(">>cStarToken[0]: %i; cStarToken[1]: %i\n",(cStarToken[0]),(cStarToken[1]));
		
/*		
    fTx = 0.0f+0.0625f*((int)cStarToken[0][1]); //column
    fTy = 0.0f+0.0625f*((int)cStarToken[1][0]); //row    
*/
/*
    fTx = 0.0f+0.0625f*(cStarToken[0]-'0'); //column
    fTy = 0.0f+0.0625f*(cStarToken[1]-'0'); //row    
*/

/*	//edited by Mike, 20210720
    fTx = 0.0f+0.0625f*((sTileId[1]-'0')); //column
    fTy = 0.0f+0.0625f*(sTileId[3]-'0'); //row
*/

    //added by Mike, 20210725; removed by Mike, 20210725
    //sTileId="0-0";
//    std::cout << "sTileId: " << sTileId << "\n";
		
	  fTx = 0.0f+0.0625f*(myUsbongUtils->autoIdentifyColumnInputInLevelMapContainer(sTileId)); //column
//	  fTx = 0.0f+2*2*0.0625f*(myUsbongUtils->autoIdentifyColumnInputInLevelMapContainer(sTileId)); //column
//	  fTx = 0.0f+2*0.0625f*(myUsbongUtils->autoIdentifyColumnInputInLevelMapContainer(sTileId)); //column
		
		//edited by Mike, 20210721    
    //texture y-axis; start from bottom; anchor
    //CAPCOM's GHOSTS'N GOBLINS? (Arcade, Family Computer; Year 1985) 
    //May tattoo ng puso ang braso ng mga Dambuhalang Goblin.
    //Umaaligid-aligid sila sa loob ng Gusali;
    //paalala: Goblin = uri ng halimaw; bakemono, i.e. naibang bagay
    //Maaaring dalawang set ng pagsalakay upang mapuksa ang Dambuhalang Goblin.
    //May tapang din ang pagtalon mula sa itaas na palapag upang magbalik at sumalakay muli; 
    //Ito'y sa halip na harapin ang halimaw, at tanggapin nang nakikipaglaban ang tiyak na kamatayan.
    //paalala: ang patay, patay na.
		//16.0f due to tile x16 to be height max
		//0.0625f*16.0f=1.0f
    fTy = 1.0f-0.0625f*(myUsbongUtils->autoIdentifyRowInputInLevelMapContainer(sTileId)); //row    
//    fTy = 1.0f-2*2*0.0625f*(myUsbongUtils->autoIdentifyRowInputInLevelMapContainer(sTileId)); //row    
//    fTy = 1.0f-2*0.0625f*(myUsbongUtils->autoIdentifyRowInputInLevelMapContainer(sTileId)); //row    

    	
		printf(">>>%i\n",(myUsbongUtils->autoIdentifyColumnInputInLevelMapContainer(sTileId)));
		printf(">>>%i\n",(myUsbongUtils->autoIdentifyRowInputInLevelMapContainer(sTileId)));

//    printf(">>>fTx: %f;fTy: %f\n",fTx,fTy);

    
    float fTileSideXAxis = 0.0625f;
//    float fTileSideXAxis = 0.0625f*2*2;
//    float fTileSideXAxis = 0.0625f*2;

    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;
//    float fTileSideYAxis = -0.0625f*2*2;
//    float fTileSideYAxis = -0.0625f*2;
 
		//printf("fGridTileWidthVertexPosition: %f\n",fGridTileWidthVertexPosition);
//    	glColor3f(1.0f, 0.0f, 0.0f); //red

    
    //added by Mike, 20210801; TO-DO: -update: this
    //due flipped vertically
//    glRotatef(240, 0.0f, 0.0f, 1.0f);
//    glRotatef(180, 0.0f, 1.0f, 0.0f);
//    glRotatef(180, 0.0f, 0.0f, 1.0f);
    
		//added by Mike, 20210724
		//TO-DO: -add: animation sequence based on sTileId
		//background color of tile
//-----
/*
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
*/
    //TO-DO: -reverify: cause of not auto-drawn;
    //TO-DO: -fix: auto-drawn flipped horizontal texture image
    
//    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    glColor3f(1.0f, 0.0f, 0.0f); //set to default, i.e. white

    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
//-----		
/*
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glTexCoord2f(fTx, fTy);
    	glVertex3f(0.0f, 0.0f, 0.0f);
    	
    	glTexCoord2f(fTx + fTileSideXAxis, fTy);
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    	
    	glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	
    	glTexCoord2f(fTx, fTy + fTileSideYAxis);
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    glEnd();
*/
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}

//added by Mike, 20210708; edited by Mike, 20210712
//TO-DO: -add: function with tile patterns
//note: we use drawLevelWithTextureUsingInputFile(...)
void Level2D::drawLevelWithTexture()
{
    /*	//removed by Mike, 20210712
     //added by Mike, 20210705
     #if defined(__APPLE__)
     #else
    	glTranslatef(+0.02f, 0.0f, 0.0f);
     #endif
     */
    
    glPushMatrix();
    //    printf(">>>myUsbongUtils->autoConvertFromPixelToVertexPointX(0): %f\n",myUsbongUtils->autoConvertFromPixelToVertexPointdrawTileAsQuadWithTextureX(0));
    
    //added by Mike, 20210712
    //add +1.0f due to 3rd quadrant in the draw function
    //incorrect output if multiple glTranslate(...) Commands
    //    	glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(0+fGridSquareWidth*1.0f), 0.0f, 0.0f);
    
    //column 1; start at 0; note +1.0f to be 2.0f due to 3rd quadrant in drawTileAsQuadWithoutTexture(...)
    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(0.0f+fGridSquareWidth*(1.0f+1.0f)), myUsbongUtils->autoConvertFromPixelToVertexPointY(0.0f+fGridSquareHeight*0.0f), 0.0f);
    
    //edited by Mike, 20210710
    //drawPressNextSymbol();
    //edited by Mike, 20210717
    //    	drawTileAsQuadWithoutTexture();

		//edited by Mike, 20210719
//    drawTileAsQuadWithTexture();
//      drawTileAsQuadWithTexture(sCurrentLevelMapContainer[iRowCount][iColumnCount]);

    glPopMatrix();
    
}

//added by Mike, 20210708; edited by Mike, 20210712
//TO-DO: -add: function with tile patterns
//TO-DO: -update: this
void Level2D::drawLevelWithTextureUsingInputFile()
{
    /*	//removed by Mike, 20210712
     glPushMatrix();
     //column 1; start at 0; note +1.0f to be 2.0f due to 3rd quadrant in drawTileAsQuadWithoutTexture(...)
    	glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(0.0f+fGridSquareWidth*(1.0f+1.0f)), myUsbongUtils->autoConvertFromPixelToVertexPointY(0.0f+fGridSquareHeight*0.0f), 0.0f);
     
    	//edited by Mike, 20210710
    	//drawPressNextSymbol();
    	drawTileAsQuadWithoutTexture();
     glPopMatrix();
     */
    
    /*
     for (int iRowCountToSetDefault=0; iRowCountToSetDefault<MAX_TEXT_CHAR_ROW_RAM; iRowCountToSetDefault++) {
     for (int iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
     //verified: in macOS, with Japanese keyboard ro-maji input, "¥0", backspace is "¥"
     tempText[iRowCountToSetDefault][iColumnCount]='\0';
     }
     }
     */
    //TO-DO: -update: this
    
    //edited by Mike, 20210724
    iRowCountMax=10;
    iColumnCountMax=18;
    iHeightCountMax=10;
    
    for (int iRowCount=0; iRowCount<iRowCountMax; iRowCount++) {
        //iCurrentMaxColumnCountPerRowContainer[iRowCount];
        for (int iColumnCount=0; iColumnCount<iColumnCountMax; iColumnCount++) {
            //                if (cCurrentTextContainer[iRowCount][iColumnCount]) {
  					//edited by Mike, 20210719
//            if (sCurrentLevelMapContainer[iRowCount][iColumnCount].compare("\"A1\"") == 0) { //TRUE
    				//note: "0" for empty, instead of "-1"
    				//with "0", no need to add quotation marks
            if (sCurrentLevelMapContainer[iRowCount][iColumnCount].compare("0") == 0) { //TRUE
            }
            else {
                glPushMatrix();
                	//add +1.0f in x-axis and y-axis due to 3rd quadrant in the draw function
                	//center 0,0,0 origin; vertex positions
  								
  								//edited by Mike, 20210722              	
                	glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(0.0f+fGridSquareWidth*(iColumnCount+1.0f)), myUsbongUtils->autoConvertFromPixelToVertexPointY(0.0f+fGridSquareHeight*(iRowCount+1.0f)), 0.0f);
 										
 										//edited by Mike, 20210719
//                		drawTileAsQuadWithTexture();
//note: incorrect output if we use printf(...) with std::string as input to %s
                
                //added by Mike, 20210725
                //note: use this Command to verify if inputLevel1.csv uses correct quotation mark encoding
                //reminder: we use the same quotation mark for opening and closing
                //in macOS, use XCode to edit input file, e.g. inputLevel1.csv;
                //this is instead of TextEdit
//std::cout << "sCurrentLevelMapContainer[iRowCount][iColumnCount]): " << sCurrentLevelMapContainer[iRowCount][iColumnCount] << "\n";
                
                		drawTileAsQuadWithTexture(sCurrentLevelMapContainer[iRowCount][iColumnCount]);
                glPopMatrix();
            }
        }
    }
    
}

//added by Mike, 20210423
void Level2D::update(float dt)
{
    switch (currentState)
    {
        case INITIALIZING_STATE:
        case MOVING_STATE:
            switch(currentMovingState) {
                case WALKING_MOVING_STATE:
                    break;
                case ATTACKING_MOVING_STATE:                  
                    break;
                    
                default: //STANDING STATE
                    break;//do nothing
            }
            break;
        case IN_TITLE_STATE:
            break;
        default: //STANDING STATE
            break;//do nothing
    }
}

void Level2D::changeState(int s)
{
    currentState=s;
}

//added by Mike, 20201226
void Level2D::keyDown(int keyCode) {
    myKeysDown[keyCode] = TRUE;
    
    //added by Mike, 20210619
    //TO-DO: -reverify: output of over 6 rows in input file
    if (myKeysDown[KEY_K]==TRUE) {
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
    
    //removed by Mike, 20210619
    //added by Mike, 20210127; edited by Mike, 20210128
    //    autoVerifyDashStateWithKeyDown();//keyCode);
}

//added by Mike, 20201227; edited by Mike, 20210128
//void Level2D::setDashStateWithKeyDown() {
void Level2D::setDashStateWithKeyDown(int keyCode) {
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
//void Level2D::autoVerifyDashStateWithKeyDown(int keyCode) {
void Level2D::autoVerifyDashStateWithKeyDown() { //int keyCode) {
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

void Level2D::keyUp(int keyCode) {
    //added by Mike, 20210127
    autoVerifyDashStateWithKeyUp(keyCode);
    
    myKeysDown[keyCode] = FALSE;
}

//added by Mike, 20210127; edited by Mike, 20210126
/*
 void Level2D::setDashStateWithKeyUp() {
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
//void Level2D::setDashStateWithKeyUp() {
void Level2D::setDashStateWithKeyUp(int keyCode) {
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
void Level2D::autoVerifyDashStateWithKeyUp(int keyCode) {
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


void Level2D::move(int key)
{
}

//added by Mike, 20210724; edited by Mike, 20210725
bool Level2D::isLevel2DCollideWith(MyDynamicObject* mdo)
{
    if ((!checkIsCollidable())||(!mdo->checkIsCollidable()))    
    {
    		//printf(">>>>>NOT COLLIDABLE");
        return false;
    }
       
		for (int iRowCount=0; iRowCount<iRowCountMax; iRowCount++) {
        for (int iColumnCount=0; iColumnCount<iColumnCountMax; iColumnCount++) {
    				//note: "0" for empty, instead of "-1"
    				//with "0", no need to add quotation marks
            if (sCurrentLevelMapContainer[iRowCount][iColumnCount].compare("0") == 0) { //TRUE            
            }
            else {
							  	//add +1.0f in x-axis and y-axis due to 3rd quadrant in the draw function
                	//center 0,0,0 origin; vertex positions
                            //edited by Mike, 20210725
                            //note: no need to add 1.0f
                            //mdo as Pilot
                            //edited by Mike, 20210725
//       						if (mdo->collideWithLevel2DTileRectAsPixel(0.0f+fGridSquareWidth*(iColumnCount),0.0f+fGridSquareHeight*(iRowCount), fGridSquareWidth, fGridSquareHeight)) {
                		//edited by Mike, 20210728
//                    if (mdo->collideWithLevel2DTileRectAsPixel(0.0f+fGridSquareWidth*(iColumnCount-1),0.0f+fGridSquareHeight*(iRowCount), fGridSquareWidth, fGridSquareHeight)) {
										//note: no -1 in iColumnCount due to not draw function
                    if (mdo->collideWithLevel2DTileRectAsPixel(0.0f+fGridSquareWidth*(iColumnCount),0.0f+fGridSquareHeight*(iRowCount), fGridSquareWidth, fGridSquareHeight)) {

                printf(">>dito>>>>>>>>>>>>>>>>>>>>>>>.");
    	
        						//this->hitBy(mdo);

                                //added by Mike, 20210725
                                this->hitByAtTile(mdo, sCurrentLevelMapContainer[iRowCount][iColumnCount], 			
                                					0.0f+fGridSquareWidth*(iColumnCount), //note: no -1 in iColumnCount
                                					0.0f+fGridSquareHeight*(iRowCount));
                                
                                //removed by Mike, 20210725
 //       						mdo->hitBy(this);
                                
                                return true;
       						}       																										
		        }
		   }
		}
    
    return false;
}

//added by Mike, 20210725; edited by Mike, 20210728
//void Level2D::hitByAtTile(MyDynamicObject* mdo, std::string sTileId)
void Level2D::hitByAtTile(MyDynamicObject* mdo, std::string sTileId, int iTileXPos, int iTileYPos)
{
    //changeState(DEATH_STATE);
    //setCollidable(false);
    //    myOpenGLCanvas->loseLife();
    
    //added by Mike, 20210725
//    int iTileColumn = myUsbongUtils->autoIdentifyColumnInputInLevelMapContainer(sTileId); //column
//    int iTileRow = myUsbongUtils->autoIdentifyRowInputInLevelMapContainer(sTileId); //row

    std::cout << "sTileId: " << sTileId << "\n";
/*
    std::cout << "iTileXPos: " << iTileXPos << "\n";
    std::cout << "iTileYPos: " << iTileYPos << "\n";
*/

		//edited by Mike, 20210729
    //note: there exist quotation marks in sTileId
    //"0-0": wall
		
		//added by Mike, 20210729
    //note: select saved tile .csv file do not include quotation marks    
		sTileId = myUsbongUtils->autoDeleteQuotationMark(sTileId);

//    std::cout << "autoDeleted sTileId: " << sTileId << "\n";
		
    if (sTileId.compare("0-0") == 0) {//True
//    if (sTileId.compare("\"0-0\"") == 0) {//True
        //OK
//        printf(">>HALLO");
/*
				//note: step *6 result, pushed back hit distance
				//zelda?
        if (mdo->getCurrentFacing()==FACING_UP) {
        	mdo->setYPosAsPixel(iTileYPos +this->getHeightAsPixel()+mdo->getStepY()*6);
        }
        else if (mdo->getCurrentFacing()==FACING_DOWN) {
        	mdo->setYPosAsPixel(iTileYPos -mdo->getHeightAsPixel()-mdo->getStepY()*6);
        }        
        else if (mdo->getCurrentFacing()==FACING_LEFT) {
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>> FACING_LEFT\n");
        
        	mdo->setXPosAsPixel(iTileXPos +this->getWidthAsPixel()+mdo->getStepX()*6);
        }
        else if (mdo->getCurrentFacing()==FACING_RIGHT) {
        	mdo->setXPosAsPixel(iTileXPos -mdo->getWidthAsPixel()-mdo->getStepX()*6);      	
        }   
*/

        if (mdo->getCurrentFacing()==FACING_UP) {
        	mdo->setYPosAsPixel(iTileYPos +this->getHeightAsPixel()+1);
        }
        else if (mdo->getCurrentFacing()==FACING_DOWN) {
        	mdo->setYPosAsPixel(iTileYPos -mdo->getHeightAsPixel()-1);
        }        
        else if (mdo->getCurrentFacing()==FACING_LEFT) {
        	mdo->setXPosAsPixel(iTileXPos +this->getWidthAsPixel()+1);    	
        }
        else if (mdo->getCurrentFacing()==FACING_RIGHT) {
        	//edited by Mike, 20210730
        	mdo->setXPosAsPixel(iTileXPos -mdo->getWidthAsPixel()-1);      	
        }
    }
    //TO-DO: -update: this
    else if (sTileId.compare("0-2") == 0) {//True
        
    }
    
        
        //removed by Mike, 20201001
    /*
     zing = sound->load_sound_clip(RESETSOUND);
     sound->play_sound_clip(zing);
     */

    //removed by Mike, 20210725
    //reset();
}

void Level2D::hitBy(MyDynamicObject* mdo)
{
    //changeState(DEATH_STATE);
    //setCollidable(false);
//    myOpenGLCanvas->loseLife();
    
    //removed by Mike, 20201001
    /*
     zing = sound->load_sound_clip(RESETSOUND);
     sound->play_sound_clip(zing);
     */
    //removed by Mike, 20210725
    //reset();
}

/*	//removed by Mike, 20210522
 void Level2D::setOpenGLCanvas(OpenGLCanvas* c)
 {
 myOpenGLCanvas = c;
 }
 */

void Level2D::reset()
{
/* //removed by Mike, 20210801
    changeState(INITIALIZING_STATE);
    myXPos=0;
    myYPos=0;
    rotationAngle=0.0f;
    thrust=0.0f;
    setCollidable(false);
    invincibleCounter=0;
*/
}
int Level2D::getState()
{
    return currentState;
}

//added by Mike, 20201016
void Level2D::destroy()
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
bool Level2D::loadTexture(CTargaImage *myTexture, const char *filename, unsigned int *myTextureObject)
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

void Level2D::drawTriangledCube(float xPos, float yPos, float zPos)
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


//added by Mike, 20210712
void Level2D::read(char *inputFilename) {
    int c;
    FILE *file;
    
    
    //added by Mike, 20210712
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Level2D read(...)\n");
    
    
    //TO-DO: update: this
    //	char** iCurrentLevelMapContainer = new char[100][100];
    int iRowCount=0;
    int iColumnCount=0;
    
    
    /*	//edited by Mike, 20210305
     for (iRowCount=0; iRowCount<MAX_Z_AXIS_VIEWPORT; iRowCount++) {
     for (iColumnCount=0; iColumnCount<MAX_X_AXIS_VIEWPORT; iColumnCount++) {
     sCurrentLevelMapContainer[iRowCount][iColumnCount]=(char*)"-1";//'G';
     }
     }
     */
    //edited by Mike, 20210310
    /*
     for (iRowCount=0; iRowCount<100; iRowCount++) {
     for (iColumnCount=0; iColumnCount<100; iColumnCount++) {
     sCurrentLevelMapContainer[iRowCount][iColumnCount]=(char*)"-1";//'G';
     }
     }
     */
    /*	//edited by Mike, 20210321
     //add +60 to be 160; where 60 : viewport max
     for (iRowCount=0; iRowCount<160; iRowCount++) {
     for (iColumnCount=0; iColumnCount<160; iColumnCount++) {
     sCurrentLevelMapContainer[iRowCount][iColumnCount]=(char*)"-1";//'G';
     }
     }
     */
    
    for (iRowCount=0; iRowCount<MAX_Z_AXIS_MAP; iRowCount++) {
        for (iColumnCount=0; iColumnCount<MAX_X_AXIS_MAP; iColumnCount++) {
            //edited by Mike, 20210724
//            sCurrentLevelMapContainer[iRowCount][iColumnCount]=(char*)"-1";//'G';
            sCurrentLevelMapContainer[iRowCount][iColumnCount]=(char*)"0";//-1";//'G';
        }
    }
    
    iRowCount=0;
    iColumnCount=0;
				
    //noted by Mike, 20201210
    //note: if concatenated string exceeds size, "stack smashing detected"; terminated; Aborted (core dumped)
    //I prefer to set a size, instead of dynamically allocate due to increasing likelihood of memory leaks
    //where memory leaks = not deallocated storage in memory, albeit not used by software application
    //identifying not deallocated storage in memory becomes more difficult with increasing use
    char input[MAX_INPUT_TEXT_PER_LINE]; //max size
    char inputTextLine[MAX_INPUT_TEXT_PER_LINE]; //max size
    char tempInputTextLine[MAX_INPUT_TEXT_PER_LINE]; //max size
    
    strcpy(input, "input/");
    strcat(input, inputFilename); //already includes .txt
    //	strcat(input,".txt");
    
    //	printf("dito: %s",input);
    
    //	file = fopen("input/"+inputFilename, "r"); //.txt file
    //	file = fopen("input/inputHalimbawa.txt", "r"); //.txt file
    file = fopen(input, "r"); //.txt file
    
    //TO-DO: -reverify: tile positions
    int iCount=0;
    
    if (file) {
        //edited by Mike, 20210210
        //		while ((c = getc(file)) != EOF) {
        while (fgets (input, MAX_INPUT_TEXT_PER_LINE, file)) { /* read each line of input */
            //	putchar(c);
            
            /*	//removed by Mike, 20210210
             char sRow[2] = {(char)c};
             */
            //delimited using new line
            /*			char *chRow = strtok(sRow, "\n");
             */
            sscanf (input, "%s", inputTextLine);
            
            
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
            strcpy(tempInputTextLine,inputTextLine);
            
            //note: add "-1" for empty
            //otherwise, comma as column is skipped
            char *ch = strtok(tempInputTextLine, ",");
            
            while (ch != NULL) {
                //				printf("%i,",iColumnCount);
                
                //TO-DO: use String, instead of char
                //TO-DO: -reverify: output due to "G" not put in container
                //				sCurrentLevelMapContainer[iRowCount][iColumnCount]=&ch;
                sCurrentLevelMapContainer[iRowCount][iColumnCount]=ch;
                
                /*	//edited by Mike, 20210211
                 printf("%s:",ch);
                 printf("%i,",iColumnCount);
                 */
                printf("HALLO %i:",iColumnCount);
                printf("%s,",ch);
                
                iColumnCount=iColumnCount+1;
                ch = strtok(NULL, ",");
            }
            
            //edited by Mike, 20210311
            //			if (iRowCount<100) {
            //edited by Mike, 20210321
            //			if (iRowCount<160) {
            if (iRowCount<MAX_X_AXIS_MAP) {
                iRowCount=iRowCount+1;
            }
            else {
                iRowCount=0;
            }
            
            printf("\n");
        }
        fclose(file);
    }
}

//added by Mike, 20210614
//note: error occurs if excess rows > 1
//TO-DO: -fix: this
void Level2D::readInputText(char *inputFilename) {
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
