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
 * @date updated: 20210729
 * @website address: http://www.usbong.ph
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007): 
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 *
 */
//TO-DO: -update: this

#include "MyDynamicObject.h"
#include <stdlib.h>

//added by Mike, 20210725
#include "Level2D.h"

//added by Mike, 20210517
#include "UsbongUtils.h"

//#include "Beam.h"

#include <string.h>

//edited by Mike, 20201226
//#include <windows.h> //Windows Machine
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

#include <stdio.h>

//added by Mike, 20210517
MyDynamicObject::~MyDynamicObject()
{
}

//added by Mike, 202105017
void MyDynamicObject::setUsbongUtils(UsbongUtils* u)
{
     myUsbongUtils = u;
}

//added by Mike, 20201213
void MyDynamicObject::draw() {
}

void MyDynamicObject::hitBy(MyDynamicObject* mdo){
}

bool MyDynamicObject::checkIsCollidable() {
     return isCollidable;
}
void MyDynamicObject::setCollidable(bool c) {
     isCollidable=c;
}

void MyDynamicObject::initializeCollisionSpheres(int ms) {
  csSize=0;
  maxSize = ms;
  collisionSpheres = (float**)malloc(maxSize*sizeof(float*));
  for(i=0; i<maxSize; i++)
    collisionSpheres[i] = (float*)malloc(4*sizeof(float)); //why 4? x y z, and radius
}

void MyDynamicObject::addSphere(float dx, float dy, float dz, float r)
{
	if(csSize!=maxSize)
	{
		collisionSpheres[csSize][0]=dx;
		collisionSpheres[csSize][1]=dy;
		collisionSpheres[csSize][2]=dz;
		collisionSpheres[csSize][3]=r;
		csSize++;
	}
	//else csSize has reached the maxSize
}

float** MyDynamicObject::getCollisionSpheres()
{
   return collisionSpheres;
}

int MyDynamicObject::getNumSpheres()
{
   return csSize;
}

float MyDynamicObject::getX()
{
   return myXPos;
}
float MyDynamicObject::getY()
{
   return myYPos;
}
float MyDynamicObject::getZ()
{
   return myZPos;
}

float MyDynamicObject::getWidth()
{
   return myWidth;
}
float MyDynamicObject::getHeight()
{
   return myHeight;
}


float MyDynamicObject::getDistance(float x1, float y1, float z1, float x2, float y2, float z2)
{
/*
    float dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
    char str[700];
    sprintf(str,"here: %f",dist);
    MessageBox(NULL, str, "Welcome!", MB_OK);
*/
   return /*dist;*/(float)sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
}

bool MyDynamicObject::checkCollision(MyDynamicObject* mdo1, MyDynamicObject* mdo2)
{
    int s1 = mdo1->getNumSpheres();
	int s2 = mdo2->getNumSpheres();
    if(s1==0||s2==0)
    {
    	return false;
    }

    float** cs1 = mdo1->getCollisionSpheres();
    float** cs2 = mdo2->getCollisionSpheres();
    for(int i = 0; i < s1; i++)
    {
    	for(int j = 0; j < s2; j++)
    	{
    		if(getDistance(cs1[i][0] + mdo1->getX(),
						   cs1[i][1] + mdo1->getY(),
						   cs1[i][2] + mdo1->getZ(),
						   cs2[j][0] + mdo2->getX(),
						   cs2[j][1] + mdo2->getY(),
						   cs2[j][2] + mdo2->getZ())
    					<=(cs1[i][3]+cs2[j][3]))
    		{
    			return true;
    	    }
        }
    }        
    return false;
}

bool MyDynamicObject::isIntersectingRect(MyDynamicObject* mdo1, MyDynamicObject* mdo2)
{     
/*
    char str[700];
    sprintf(str,"here: %f",mdo1->getY());//myZPos
    MessageBox(NULL, str, "Welcome!", MB_OK);
*/
/*	//edited by Mike, 20201117
    if (mdo2->getY()+mdo2->getHeight() < mdo1->getY() || //is the bottom of mdo2 above the top of mdo1?
        mdo2->getY() > mdo1->getY()+mdo1->getHeight() || //is the top of mdo2 below bottom of mdo1?
        mdo2->getX()+mdo2->getWidth() < mdo1->getX()  || //is the right of mdo2 to the left of mdo1?
        mdo2->getX() > mdo1->getX()+mdo1->getWidth()) //is the left of mdo2 to the right of mdo1?
        return false;
*/
	//note: computer computation correct, albeit human person may think 
	//based on observation in 3D (3-Dimensions) that beam should hit asteroid
	//alternative collision detection technique 
	//for computer to verify if not intersecting
	//Reference: Jongko, J. et al (2004)
/* //edited by Mike, 20210219	
    if (mdo2->getZ()+mdo2->getHeight() < mdo1->getZ() || //is the bottom of mdo2 above the top of mdo1?
        mdo2->getZ() > mdo1->getZ()+mdo1->getHeight() || //is the top of mdo2 below bottom of mdo1?
        mdo2->getX()+mdo2->getWidth() < mdo1->getX()  || //is the right of mdo2 to the left of mdo1?
        mdo2->getX() > mdo1->getX()+mdo1->getWidth()) //is the left of mdo2 to the right of mdo1?
        return false;
*/
	//TO-DO: -update: to use keyword for object size in z-axis, etc
    if (mdo2->getZ()+mdo2->getWidth() < mdo1->getZ() || //is the bottom of mdo2 above the top of mdo1?
        mdo2->getZ() > mdo1->getZ()+mdo1->getWidth() || //is the top of mdo2 below bottom of mdo1?
        mdo2->getX()+mdo2->getWidth() < mdo1->getX()  || //is the right of mdo2 to the left of mdo1?
        mdo2->getX() > mdo1->getX()+mdo1->getWidth()) {//is the left of mdo2 to the right of mdo1?
        return false;
	}
	
    return true;
}

void MyDynamicObject::collideWith(MyDynamicObject* mdo)
{
/*     
    if (this==we)
	{
		return;
	}
*/
//    	printf(">>");

    if ((!checkIsCollidable())||(!mdo->checkIsCollidable()))    
    {
//    	printf(">>>>>NOT COLLIDABLE");

        return;
    }
       
//    if (checkCollision(this, mdo))
		//edited by Mike, 20210527
//    if (isIntersectingRect(this, mdo))
    if (isIntersectingRectAsPixel(this, mdo))
    {
//    	printf(">>dito>>>>>>>>>>>>>>>>>>>>>>>.");
    	
        this->hitBy(mdo);
        mdo->hitBy(this);
    }
}

//added by Mike, 20210527
bool MyDynamicObject::isIntersectingRectAsPixel(MyDynamicObject* mdo1, MyDynamicObject* mdo2)
{     
/*
    char str[700];
    sprintf(str,"here: %f",mdo1->getY());//myZPos
    MessageBox(NULL, str, "Welcome!", MB_OK);
*/
/*	//edited by Mike, 20201117
    if (mdo2->getY()+mdo2->getHeight() < mdo1->getY() || //is the bottom of mdo2 above the top of mdo1?
        mdo2->getY() > mdo1->getY()+mdo1->getHeight() || //is the top of mdo2 below bottom of mdo1?
        mdo2->getX()+mdo2->getWidth() < mdo1->getX()  || //is the right of mdo2 to the left of mdo1?
        mdo2->getX() > mdo1->getX()+mdo1->getWidth()) //is the left of mdo2 to the right of mdo1?
        return false;
*/
	//note: computer computation correct, albeit human person may think 
	//based on observation in 3D (3-Dimensions) that beam should hit asteroid
	//alternative collision detection technique 
	//for computer to verify if not intersecting
	//Reference: Jongko, J. et al (2004)

    if (mdo2->getYAsPixel()+mdo2->getHeightAsPixel() < mdo1->getYAsPixel() || //is the bottom of mdo2 above the top of mdo1?
        mdo2->getYAsPixel() > mdo1->getYAsPixel()+mdo1->getHeightAsPixel() || //is the top of mdo2 below bottom of mdo1?
        mdo2->getXAsPixel()+mdo2->getWidthAsPixel() < mdo1->getXAsPixel()  || //is the right of mdo2 to the left of mdo1?
        mdo2->getXAsPixel() > mdo1->getXAsPixel()+mdo1->getWidthAsPixel()) {//is the left of mdo2 to the right of mdo1?
        return false;
		}
	
    return true;
}

//added by Mike, 20210517
//use with computer mouse, et cetera
bool MyDynamicObject::collideWithPressedCoordPos(int pressedCoordPosX, int pressedCoordPosY)	
{
		printf("pressedCoordPosX: %i\n",pressedCoordPosX);
		printf("getXAsPixel()+getWidthAsPixel(): %i\n",getXAsPixel()+getWidthAsPixel());
	
	
	//intersecting rectangles
/*	//edited to use pixel instead of vertex points by Mike, 20210517
    if (pressedCoordPosX > myUsbongUtils->autoConvertFromPixelToVertexPointX(getX()+getWidth()) || //pressed coordinate position at right of object
        pressedCoordPosX < myUsbongUtils->autoConvertFromPixelToVertexPointX(getX()) || //pressed coordinate position at left of object
        pressedCoordPosY < myUsbongUtils->autoConvertFromPixelToVertexPointX(getY()) || //pressed coordinate position at top of object
		pressedCoordPosY > myUsbongUtils->autoConvertFromPixelToVertexPointY(getY()+getHeight())) { //pressed coordinate position at right of object

		printf("outside button\n");
		
		return false;
	}	
*/
    if (pressedCoordPosX > getXAsPixel()+getWidthAsPixel() || //pressed coordinate position at right of object
        pressedCoordPosX < getXAsPixel() || //pressed coordinate position at left of object
        pressedCoordPosY < getYAsPixel() || //pressed coordinate position at top of object
		pressedCoordPosY > getYAsPixel()+getHeightAsPixel()) { //pressed coordinate position at right of object

		printf("outside button\n");
		
		return false;
	}	
	
	printf("inside button\n");
	
	return true;
}

//added by Mike, 20210724
//use with Level2D
bool MyDynamicObject::collideWithLevel2DTileRectAsPixel(int iTilePosXAsPixel, int iTilePosYAsPixel, int iTileWidthAsPixel, int iTileHeightAsPixel)	
{
/*
		//TO-DO: update: this
		printf(">>>>>>>\n");
//		printf("iTilePosXAsPixel: %i\n",iTilePosXAsPixel);
//		printf("iTileWidthAsPixel: %i\n",iTileWidthAsPixel);
		printf("getXAsPixel()+getWidthAsPixel(): %i\n",getXAsPixel()+getWidthAsPixel());
*/
//    printf("iTilePosXAsPixel: %i\n",iTilePosXAsPixel); //example: 0
//    printf("iTilePosYAsPixel: %i\n",iTilePosYAsPixel); //example: 0
//    printf("iTilePosXAsPixel+iTileWidthAsPixel: %i\n",iTilePosXAsPixel+iTileWidthAsPixel); //example 0+71
//    printf("iTilePosYAsPixel+iTileHeightAsPixel: %i\n",iTilePosYAsPixel+iTileHeightAsPixel); //example 0+80
//    printf("iTileHeightAsPixel: %i\n",iTileHeightAsPixel); //example 80

  //TO-DO: -update: width and height values

  //example: at grid B1
  printf("getXAsPixel(): %i\n",getXAsPixel()); //example: 56
  printf("getWidthAsPixel(): %i\n",getWidthAsPixel()); //example: 71
//  printf("getXAsPixel()+getWidthAsPixel(): %i\n",getXAsPixel()+getWidthAsPixel()); //example: 127

//    printf("getYAsPixel(): %i\n",getYAsPixel()); //example: 2
//    printf("getHeightAsPixel(): %i\n",getHeightAsPixel()); //example: 80

    //edited by Mike, 20210725
/*    //intersecting rectangles
    if (iTilePosXAsPixel > getXAsPixel()+getWidthAsPixel() || //tile position at right of object
        iTilePosXAsPixel+iTileWidthAsPixel < getXAsPixel() || //tile position at left of object
        iTilePosYAsPixel+iTileHeightAsPixel < getYAsPixel() || //tile position at top of object
				iTilePosYAsPixel > getYAsPixel()+getHeightAsPixel()) { //tile position at bottom of object
*/

    //intersecting rectangles
  //edited by Mike, 20210728    
		//output: stuck in tile
/*		
		if (iTilePosXAsPixel > getXAsPixel()+getWidthAsPixel() +getStepX() || //tile position at right of object
            iTilePosXAsPixel+iTileWidthAsPixel < getXAsPixel() -getStepX() || //tile position at left of object
            iTilePosYAsPixel+iTileHeightAsPixel < getYAsPixel() -getStepY() || //tile position at top of object
            iTilePosYAsPixel > getYAsPixel()+getHeightAsPixel() +getStepY()) { //tile position at bottom of object
*/

/* //edited by Mike, 20210728
		if ((iTilePosXAsPixel > getXAsPixel()+getWidthAsPixel() -getStepX()) || //tile position at right of object
            (iTilePosXAsPixel+iTileWidthAsPixel < getXAsPixel() +getStepX()) || //tile position at left of object
            (iTilePosYAsPixel+iTileHeightAsPixel < getYAsPixel() +getStepY()) || //tile position at top of object
            (iTilePosYAsPixel > getYAsPixel()+getHeightAsPixel() -getStepY())) { //tile position at bottom of object
*/
/*
		if ((iTilePosXAsPixel > getXAsPixel()+getWidthAsPixel()) || //tile position at right of object
            (iTilePosXAsPixel+iTileWidthAsPixel < getXAsPixel()) || //tile position at left of object
            (iTilePosYAsPixel+iTileHeightAsPixel < getYAsPixel()) || //tile position at top of object
            (iTilePosYAsPixel > getYAsPixel()+getHeightAsPixel())) { //tile position at bottom of object
*/
			//verify: if continues with step, collision
			
/* //edited by Mike, 20210729			
			if ((iTilePosXAsPixel > getXAsPixel()+getWidthAsPixel() +getStepX()) || //tile position at right of object
            (iTilePosXAsPixel+iTileWidthAsPixel < getXAsPixel() -getStepX()) || //tile position at left of object
            (iTilePosYAsPixel+iTileHeightAsPixel < getYAsPixel() -getStepY()) || //tile position at top of object
            (iTilePosYAsPixel > getYAsPixel()+getHeightAsPixel() +getStepY())) { //tile position at bottom of object
*/
			//note: we add iOffsetXPosAsPixel and iOffsetYPosAsPixel 
			//to cause need of bigger collision rectangle, before hit

						//tile position at right of object
			if ((iTilePosXAsPixel > getXAsPixel()+getWidthAsPixel()-iOffsetXPosAsPixel +getStepX()) || 
			 			//tile position at left of object
            (iTilePosXAsPixel+iTileWidthAsPixel < getXAsPixel()+iOffsetXPosAsPixel -getStepX()) ||
            //tile position at top of object
            (iTilePosYAsPixel+iTileHeightAsPixel < getYAsPixel()+iOffsetYPosAsPixel -getStepY()) || 
						//tile position at bottom of object            
            (iTilePosYAsPixel > getYAsPixel()+getHeightAsPixel()-iOffsetYPosAsPixel +getStepY())) { 

//			printf("outside tile\n");
			return false;
		}	

/*		//added by Mike, 20210724		
		printf(">>>>>>>\n");
		printf("iTilePosXAsPixel: %i\n",iTilePosXAsPixel);
		printf("iTileWidthAsPixel: %i\n",iTileWidthAsPixel);
		printf("getXAsPixel()+getWidthAsPixel(): %i\n",getXAsPixel()+getWidthAsPixel());
*/		
		printf("inside tile\n");
	
		return true;
}

//added by Mike, 20201016
void MyDynamicObject::destroy()
{
/*	
	for(int i = 0; i < MAX_EXPLOSION_PARTICLES; ++i) {
	    delete [] explosionParticle[i];
	}
	delete [] explosionParticle;
*/
}
