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
 * @date created: 20201210
 * @date updated: 20210729
 * @website address: http://www.usbong.ph
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007): 
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 *
 */
 
//TO-DO: -update: this

/* //removed by Mike, 20201210
//added by Mike, 20201130 
#include "CTargaImage.h"
#include "OpenGLCanvas.h"
#include "MyDynamicObject.h"
//added by Mike, 20201019
#include "PolygonUtils.h"
*/

//added by Mike, 20210524
#include <time.h>
#include <stdlib.h>

//added by Mike, 20210720
#include <string>

	
class UsbongUtils
{
private:
    //added by Mike, 20210623
    float fMaxWindowWidth;
    float fMaxWindowHeight;
    
    //added by Mike, 20210701
		float myWindowWidthAsPixel;
		float myWindowHeightAsPixel;

    //added by Mike, 20210626
    float fMyWindowWidthAsPixelRatioToHeightPixel;
    int iMyWindowWidthAsPixelOffset;		
    
    
public:
	//edited by Mike, 20210524
	//UsbongUtils(); //added by Mike, 20210516	
	UsbongUtils() {
		srand(time(NULL));   //init
        
        //added by Mike, 20210623
        fMaxWindowWidth=640.0f;
        fMaxWindowHeight=640.0f;
	}
		
	~UsbongUtils(); //note: add "~" for constructor without any input parameter
    
/*
	Pilot(float xPos, float yPos, float zPos,int windowWidth,int windowHeight);
	~Pilot();
*/
    
    //added by Mike, 20210623
    //TO-DO: -update: this
//    void setWindowWidthHeight(int myWindowWidthAsPixelInput, int myWindowHeightAsPixelInput);
		//edited by Mike, 20210628
    void setWindowWidthHeight(float myWindowWidthAsPixelInput, float myWindowHeightAsPixelInput);
//    void setWindowWidthHeight(float myWindowWidthAsPixelInput, float myWindowHeightAsPixelInput, float myWindowWidthAsPixelRatioToHeightPixelInput);

    //edited by Mike, 20210614
//    void read(char *filename);    
    char* read(char *filename);
    
//    FILE read(char *filename);
// char* read(char *filename);
//char read(char *filename);
// char[] read(char *filename);

	//added by Mike, 202105016
	float autoConvertFromPixelToVertexPointX(int iPointX);
	float autoConvertFromPixelToVertexPointY(int iPointY);
	
	//added by Mike, 20210712
	float autoConvertFromPixelToVertexGridTileWidth(int iGridTileWidth);
	float autoConvertFromPixelToVertexGridTileHeight(int iGridTileHeight);
	
	//added by Mike, 20210720
	int autoIdentifyColumnInputInLevelMapContainer(std::string sInput);
	int autoIdentifyRowInputInLevelMapContainer(std::string sInput);
	
	//added by Mike, 20210729
	std::string autoDeleteQuotationMark(std::string sInput);
	
	//reminder: "rand" NOT random due to there exists pattern
	//example: based on clock time, based on excess value in container
	int getRandWithMax(int iMaxRandNumber){
		return rand()%iMaxRandNumber;
	};
};
