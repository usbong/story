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
 * @date created: 20210613
 * @date updated: 20210815
 * @website address: http://www.usbong.ph
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007):
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 *
 */

//TO-DO: -update: this

//added by Mike, 20201130
#include "CTargaImage.h"

#include "OpenGLCanvas.h"
#include "MyDynamicObject.h"

//added by Mike, 20201019
#include "PolygonUtils.h"

//added by Mike, 20210614
#include <string>

//added by Mike, 20201226
#define TRUE 1
#define FALSE 0

//added by Mike, 20210122; edited by Mike, 20210123
#define MAX_PUNCHING_ANIMATION_COUNT 2

//added by Mike, 20210614; edited by Mike, 20210616
//#define MAX_TEXT_CHAR_ROW 2idrawPressNextSymbolCount
//edited by Mike, 20210618
//#define MAX_TEXT_CHAR_ROW 3
//edited by Mike, 20210618
//per textbox
#define MAX_TEXT_CHAR_ROW 4 //TO-DO: -auto-identify if over MAX; execute pause write action
//note: MAX rows in Random Access Memory (RAM) storage
//reminder: "Random" need not be so, due to existing pattern; albeit "RAM" is still used
#define MAX_TEXT_CHAR_ROW_RAM 100 //TO-DO: -auto-identify if over MAX; execute pause write action

//edited by Mike, 20210615
//#define MAX_TEXT_CHAR_COLUMN 8 //note: 8 with comma to be end of line
//#define MAX_TEXT_CHAR_COLUMN 16 //note: 8 with comma to be end of line
#define MAX_TEXT_CHAR_COLUMN 42 //note: 8 with comma to be end of line

//added by Mike, 20210129
//+reverified: with Windows Machine; 5 with Linux Machine
//#define MAX_WAIT_COUNT 5 //4 //added by Mike, 20210126; edited by Mike, 20210128

//added by Mike, 20210423
//TO-DO: -add: texture definitions in PolygolUtils
//edited by Mike, 20210514
//#define MIKE_TEXTURE_A 7
//removed by Mike, 20210521
//added in OpenGLCanvas.h
//#define BUTTON_TEXTURE_A 7

#ifdef _WIN32
#define MAX_WAIT_COUNT 5 //Windows Machine
#endif

#ifdef linux
#define MAX_WAIT_COUNT 5
#endif

//added by Mike, 20210522
#if defined(__APPLE__)
#define MAX_WAIT_COUNT 5
#endif

//edited by Mike, 20210129
//TO-DO: -add: robotship dash background movement animation
//note: start at 0
//edited by Mike, 20210130
//TO-DO: -reverify: cause why value is 6, not 4 in Linux machine
//edited by Mike, 20210130
//#define PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT 6//4 //10 //added by Mike, 20210128

#ifdef _WIN32
//edited by Mike, 20210203; edited again by Mike, 20210219
#define PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT 4 //10 //added by Mike, 20210128
//	#define MAX_DIRECTIONAL_KEY_DASH_COUNT 6 //10 //added by Mike, 20210128
#endif

#ifdef linux
//edited by Mike, 20210204; edited again by Mike, 20210219
//	#define MAX_DIRECTIONAL_KEY_DASH_COUNT 6//4 //10 //added by Mike, 20210128
#define PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT 4 //added by Mike, 20210128
#endif

//added by Mike, 20210522
#if defined(__APPLE__)
#define PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT 4
#endif

//added by Mike, 20210516
class UsbongUtils;

//TO-DO: -update: this
class Text: public MyDynamicObject
{
private:
    int currentState;
    /*	//removed by Mike, 20201001
     Sound *sound;
     SoundClip *zing;
     */
    
    //added by Mike, 20201201
    int currentMovingState;
    
    /*
     float myXPos;
     float myYPos;
     float myZPos;
     */
    
    //added by Mike, 20210505
    float myOpponentXPos;
    float myOpponentYPos;
    float myOpponentZPos;
    
    float stepX;
    float stepY;
    float stepZ;
    /*
     float myWidthX;
     float myWidthZ;
     float myHeightY;
     */
    //added by Mike, 20210423
    int iCountTaoAnimationFrame;
    float fButtonAnimationFrameOffset;
    
    OpenGLCanvas *myOpenGLCanvas;
    
    //added by Mike, 20210516; removed to public classification by Mike, 20210523
    //	UsbongUtils *myUsbongUtils;
    
    //float boundary;
    
    //    float rotationAngle;
    float rotationAngleRad;
    float rotationStep;
    
    float thrust;
    float thrustMax;
    float xAccel;
    float yAccel;
    float xVel;
    float yVel;
    
    float maxXVel;
    float maxYVel;
    
    int invincibleCounter;
    
    unsigned int myDeathAnimationImg1,
    myDeathAnimationImg2,
    myDeathAnimationImg3,
    myDeathAnimationImg4,
    myDeathAnimationImg5;
    int currentDeathFrame;
    
    //int previousFacingState;
    //added by Mike, 20201201
    int currentFacingState;
    
    //added by Mike, 20210111
    int iFiringBeamCount;
    
    //added by Mike, 20210122
    int iPunchAnimationCount,
    iPunchAnimationCountDelay;
    
    //added by Mike, 20210616
    int iTextAnimationCount,
    iTextAnimationCountDelay,
    iTextCurrentMaxColumnCount;
    
    //added by Mike, 20201130
    CTargaImage *myBodyTexture;
   	unsigned int myBodyTextureObject;
    
    CTargaImage *myHeadTexture;
   	unsigned int myHeadTextureObject;
    
    //added by Mike, 20201201
    char legStates[2];
    char armStates[2];
    
    float legAngles[2];
    float armAngles[2];
    
    //added by Mike, 20201225
    bool bIsFiringBeam;
    bool bHasPressedADirectionalKey; //added by Mike, 20201226
    
    //added by Mike, 20210111
    bool bIsExecutingPunch,
    bIsExecutingDefend, //added by Mike, 20210112; edited by Mike, 20210121
    //removed by Mike, 20210128
    //		 bIsExecutingDash, //added by Mike, 20210126
    bIsDashReady; //added by Mike, 20210126
    
    //added by Mike, 20210128
    //note: exceed size limit at value 7 due to compiler;
    //set to 6 to execute movement
    //Reference: https://stackoverflow.com/questions/216259/is-there-a-max-array-length-limit-in-c;
    //last accessed: 20210128; answer by: Martin York, 20081019T1752; edited 20130125T2158
    //Additional Note: The following do not solve the problem
    //bool* bIsExecutingDashArray;
    //bool* bIsExecutingDashArray = new bool[6];//PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT];
    bool bIsExecutingDashArray[PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT]; //6
    
    //int iInputWaitCount; //we use with dash
    int iInputWaitCountArray[PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT]; //6
    
    //added by Mike, 20210502
    bool bIsPlayer2;
    
    //added by Mike, 20210614; edited by Mike 20210616
    //std::string sCurrentTextContainer[MAX_TEXT_CHAR_COLUMN][MAX_TEXT_CHAR_ROW]; //TO-DO: -add: auto-update max size
    //edited by Mike, 20210618
    //    char cCurrentTextContainer[MAX_TEXT_CHAR_ROW][MAX_TEXT_CHAR_COLUMN];
    char cCurrentTextContainer[MAX_TEXT_CHAR_ROW_RAM][MAX_TEXT_CHAR_COLUMN];
    
    //added by Mike, 20210616
    int iTextCurrentMaxRowCount;
    //added by Mike, 20210617; edited by Mike, 20210619
    int iCurrentMaxColumnCountPerRowContainer[MAX_TEXT_CHAR_ROW];
    //    int iCurrentMaxColumnCountPerRowContainer[MAX_TEXT_CHAR_ROW_RAM];
    
    //added by Mike, 20210618
    int iRowCountPageNumber; //start at zero
    bool isAtMaxTextCharRow;
    int idrawPressNextSymbolCount;
    
    //added by Mike, 20210723
    bool bHasReachedEndOfTextMessage,
    		 bHasPressedKeyToCloseEndOfTextMessage;
    
    GLint tricount;
    GLint isMovingForward;
    
    GLboolean test_pow2(GLushort i);
    void load_tga(char *filename);
    
    //draw texture
    //added by Mike, 20201130
    bool loadTexture(CTargaImage *myTexture, const char *filename, unsigned int *myTextureObject);
    
    //edited by Mike, 20210423
    void setupPrev();
    void setup();
    
    // draws a unit cube
    //void drawCube(float xPos, float yPos, float zPos);
    
    //added by Mike, 20201130
    // draw a unit triangle, Mike Dec. 19, 2006
    void drawTriangle(float xPos, float yPos, float zPos);
    void drawTriangledCube(float xPos, float yPos, float zPos);
    
    //added by Mike, 20201130
    // methods to draw the parts of the robot
    void drawUpperArm(float xPos, float yPos, float zPos);
    void drawLowerArm(float xPos, float yPos, float zPos);
    //removed by Mike, 20201130
    //	void drawAntenna(float xPos, float yPos, float zPos);
    void drawHead(float xPos, float yPos, float zPos);
    void drawBody(float xPos, float yPos, float zPos);
    void drawUpperLeg(float xPos, float yPos, float zPos);
    void drawLowerLeg(float xPos, float yPos, float zPos);
    void drawFoot(float xPos, float yPos, float zPos);
    
    //added by Mike, 20210107
    void drawWeapon(float xPos, float yPos, float zPos);
    
    //added by Mike, 20210127
    //	void autoVerifyDashStateWithKeyDown(int keyCode); //edited by Mike, 20210128
    void autoVerifyDashStateWithKeyDown();
    
    void autoVerifyDashStateWithKeyUp(int keyCode);
    //edited by Mike, 20210128
    //	void setDashStateWithKeyDown();
    void setDashStateWithKeyDown(int keyCode);
    
    //edited by Mike, 20210129
    //	void setDashStateWithKeyUp();
    void setDashStateWithKeyUp(int keyCode);
    
    //added by Mike, 20210618
    void drawPressNextSymbol();
    
public:
    //edited by Mike, 20201115
    //RobotShip();
    //edited by Mike, 20210625
    //    Text(float xPos, float yPos, float zPos,int windowWidth,int windowHeight);
    //edited by Mike, 20210815
//    Text(float xPos, float yPos, float zPos, float windowWidth, float windowHeight);
    Text(float xPos, float yPos, float zPos, float fWindowWidth, float fWindowHeight);
    
    ~Text();
    
    //added by Mike, 20210523
    UsbongUtils *myUsbongUtils;
    
    //added by Mike, 20201226
    //note: we classify this container as public;
    //otherwise; computer notifies us of error when we update value inside container
   	//edited by Mike, 20201227
   	//TO-DO: -update: this to include diagonal directional movement
    //int myKeysDown[4];
    //edited by Mike, 20210102
    //int myKeysDown[10]; //note: does not include KEY_J, KEY_L, KEY_I, KEY_K,
    //edited by Mike, 20210130
    //	int myKeysDown[14]; //note: includes KEY_J, KEY_L, KEY_I, KEY_K,
    int myKeysDown[10]; //note: includes KEY_J, KEY_L, KEY_I, KEY_K,
    
    //virtual ~Robot();
    float rotationAngle;
    
    virtual float getX()
    {
        return myXPos;
    }
    virtual float getY()
    {
        return myYPos;
    }
    virtual float getZ()
    {
        return myZPos;
    }
    virtual float getWidth()
    {
        return myWidth;
    }
    virtual float getHeight()
    {
        return myHeight;
    }
    //int STANDING_STATE;
    //int WALKING_STATE;
    float getRotationAngle(){
        return rotationAngle;
    }
    
    float* getXYZPos();
    
    //added by Mike, 20201230
    float getCurrentFacingState() {
        return currentFacingState;
    }
    
    //added by Mike, 20210102; edited by Mike, 20210106
    void setCurrentFacingState(int iNewFacingState) {
        currentFacingState = iNewFacingState;
    }
    
    //added by Mike, 20210502
    void setAsPlayer2() {
        currentFacingState=FACING_LEFT;
        bIsPlayer2=true;
        
        //note: position: 3,3; width, height; count starts at 0
        //edited by Mike, 20210503
        //		myXPos=0.0f+myWidth*6;
        //		myXPos=0.0f-myWidth*3;
        //		myXPos=0.0f-myWidth*2;
        myXPos=0.0f-myWidth*2.5;
        
        //note: auto-set to be equal with player 1; player 1 Z-position based on Camera Eye
        //myZPos=0.0f+myHeight*3;
    }
    
    
    //added by Mike, 20210111
    //    bool getIsExcutingPunchDefense() {
    bool getIsExecuteWithWeaponReady() {
        if (bIsExecutingPunch) {
            return false;
        }
        //edited by Mike, 20210121
        else if (bIsExecutingDefend) {
            return false;
        }
        return true;
    }
    
    //added by Mike, 20210614MAX_TEXT_CHAR_ROW
    void readInputText(char *inputFilename);
    
    virtual void draw() {
        //edited by Mike, 20210424
        //    	drawPilot();
        //		drawPilotAsQuadWithTexture();
        //edited by Mike, 20210615
        drawTextBackgroundAsQuadWithTexture();
    }
    
    //edited by Mike, 20210615
    void drawTextBackgroundAsQuadWithTexturePrev();
    void drawTextBackgroundAsQuadWithTextureBuggy(); //added by Mike, 20210617
    void drawTextBackgroundAsQuadWithTexture();
    void drawTextBackgroundObject();
    
    /*	//removed by Mike, 20210514
     //added by Mike, 20210423
     void drawPilotAsQuadWithTexture();
     void drawPilotObject();
     // draws the entire robot
     //void drawRobot(float xPos, float yPos, float zPos);
     void drawPilot();
     //    void drawValkyrie();
     //added by Mike, 20201001
     //TO-DO: -add: in ModelPool.cpp
     void drawModelPilot();
     */
    
    // updates the Pilot data
    void updatePrev(float dt);
    //added by Mike, 20210423
    void update(float dt);
    
    //added by Mike, 20210505
    void updateToFaceOpponent(float opponentPosX) {
        myOpponentXPos=opponentPosX; //added by Mike, 20210505
        
        if (getX() < opponentPosX) {
            currentFacingState=FACING_RIGHT;
        }		
        else {
            currentFacingState=FACING_LEFT;		
        }
    }
    
    // changes the robot's state
    void changeState(int s);
    
    void move(int key);
    
    //added by Mike, 20201226
    void keyDown(int keyCode);	
    void keyUp(int keyCode);	
    
    /*	//removed by Mike, 20210522	
     void setOpenGLCanvas(OpenGLCanvas* c);
     */	
    virtual void hitBy(MyDynamicObject* mdo);
    
    //added by Mike, 20201016
    virtual void destroy();
    
    void reset();
    int getState();
};
