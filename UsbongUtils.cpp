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


/**************************
 * Includes
 *
 **************************/

//added by Mike, 20201209
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

//added by Mike, 20201209
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
//#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
#endif

#include <iostream>

//added by Mike, 20201210
#include "UsbongUtils.h"
#include <string.h>

/* //removed by Mike, 20210524
 //added by Mike, 20210516
 UsbongUtils::UsbongUtils()
 {
 }
 */

//added by Mike, 20210516
UsbongUtils::~UsbongUtils()
{
}

//added by Mike, 20210623; edited by Mike, 20210628
//void UsbongUtils::setWindowWidthHeight(float myWindowWidthAsPixelInput, float myWindowHeightAsPixelInput, float myWindowWidthAsPixelRatioToHeightPixelInput)
//edited by Mike, 20210726
//note: auto-converted from int to float
void UsbongUtils::setWindowWidthHeight(float myWindowWidthAsPixelInput, float myWindowHeightAsPixelInput)
{
    fMaxWindowWidth=myWindowWidthAsPixelInput;
    fMaxWindowHeight=myWindowHeightAsPixelInput;
    
//    printf(">>>>> fMaxWindowWidth: %f, fMaxWindowHeight: %f\n",fMaxWindowWidth,fMaxWindowHeight);
    
    //    fMyWindowWidthAsPixelRatioToHeightPixel=myWindowWidthAsPixelRatioToHeightPixelInput;
}

/*
 //Reference: https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings;
 //last accessed: 20201209
 //answer by: Alok Singhal, 20100812T0003
 //edited by Mike, 20210208
 //void UsbongUtils::read(char *inputFilename) {
 //FILE UsbongUtils::read(char *inputFilename) {
 //char* UsbongUtils::read(char *inputFilename) {
 char[] UsbongUtils::read(char *inputFilename) {
	int c;
 //	char* ch;
	FILE *file;
	
	//TO-DO: update: this
	//note: verify: if there exists variation in max size
 //	char* sOutput = new char[100]; //added by Mike, 20210208
	char sOutput[100]; //added by Mike, 20210208
	
	//TO-DO: -update: this
	
	//noted by Mike, 20201210
	//note: if concatenated string exceeds size, "stack smashing detected"; terminated; Aborted (core dumped)
	//I prefer to set a size, instead of dynamically allocate due to increasing likelihood of memory leaks
	//where memory leaks = not deallocated storage in memory, albeit not used by software application
	//identifying not deallocated storage in memory becomes more difficult with increasing use
	char input[60]; //max size
	strcpy(input, "input/");
	strcat(input, inputFilename); //already includes .txt
 //	strcat(input,".txt");
	
 //	printf("dito: %s",input);
 //	file = fopen("input/"+inputFilename, "r"); //.txt file
 //	file = fopen("input/inputHalimbawa.txt", "r"); //.txt file
	file = fopen(input, "r"); //.txt file
	if (file) {
 while ((c = getc(file)) != EOF) {
 //		while ((ch = fgetc(file)) != EOF) {
 //			putchar(c);
 //			strcat(atoi(c),sOutput);
 //			strcat(ch,sOutput);
 char sBuffer[10]; //TO-DO: -update: this
 //			sprintf(sBuffer[10],"%i",c);
 sprintf(sBuffer,"%i",c);
 strcat(sBuffer,sOutput);
 }
 fclose(file);
	}
	
	return sOutput;
 }
 */


//Reference: https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings;
//last accessed: 20201209
//answer by: Alok Singhal, 20100812T0003
//edited by Mike, 20210614
//void UsbongUtils::read(char *inputFilename) {
char* UsbongUtils::read(char *inputFilename) {
    int c;
    FILE *file;
    
    //TO-DO: update: this
    
    //noted by Mike, 20201210
    //note: if concatenated string exceeds size, "stack smashing detected"; terminated; Aborted (core dumped)
    //I prefer to set a size, instead of dynamically allocate due to increasing likelihood of memory leaks
    //where memory leaks = not deallocated storage in memory, albeit not used by software application
    //identifying not deallocated storage in memory becomes more difficult with increasing use
    char input[60]; //max size
    strcpy(input, "input/");
    strcat(input, inputFilename); //already includes .txt
    //	strcat(input,".txt");
    
    //	printf("dito: %s",input);
    
    //	file = fopen("input/"+inputFilename, "r"); //.txt file
    //	file = fopen("input/inputHalimbawa.txt", "r"); //.txt file
    file = fopen(input, "r"); //.txt file
    
    if (file) {
        while ((c = getc(file)) != EOF) {
            putchar(c);
        }
        fclose(file);
    }
}



/*
 #define CHUNK 1024 // read 1024 bytes at a time
 char buf[CHUNK];
 FILE *file;
 size_t nread;
 file = fopen("test.txt", "r");
 if (file) {
 while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
 fwrite(buf, 1, nread, stdout);
 if (ferror(file)) {
 //deal with error
 }
 fclose(file);
 }
 */


/*
 char *buf = malloc(chunk);
 if (buf == NULL) {
 // deal with malloc() failure
 }
 // otherwise do this.  Note 'chunk' instead of 'sizeof buf'
 while ((nread = fread(buf, 1, chunk, file)) > 0) {
 //as above
 }
 */

//added by Mike, 20210516
//TO-DO: -reverify: center draw line
//note: vertex origin 0,0 at center
float UsbongUtils::autoConvertFromPixelToVertexPointX(int iPointX)
{
    //max width and height: 640
    //set in mainLinux.cpp
    //TO-DO: -update: this
    //square window
    /* //removed by Mike, 20210623
     float fMaxWindowWidth=640.0f;
     float fMaxWindowHeight=640.0f;
     */
    float fHalfWindowWidth=fMaxWindowWidth/2;
    float fHalfWindowHeight=fMaxWindowHeight/2;
    
    if (iPointX<fHalfWindowWidth) {
        //note: pixel point origin at top-left
        //		return -0.25+(fHalfWindowWidth-iPointX)/fMaxWindowWidth*(-1); //note: use of parenthesis
        //		return (fHalfWindowWidth-iPointX)/fMaxWindowWidth*(-1); //note: use of parenthesis
        //edited by Mike, 20210512
        //		return (fMaxWindowWidth-iPointX)/fMaxWindowWidth*(-1); //note: use of parenthesis
        //		return (fMaxWindowWidth-iPointX)/fHalfWindowWidth*(-1); //note: use of parenthesis
        
        return (fHalfWindowWidth-iPointX)/fHalfWindowWidth*(-1); //note: use of parenthesis
    }
    else if (iPointX==0) {
        return 0; //note: use of parenthesis
    }
    //edited by Mike, 20210512
    else {
        //	return -0.25+iPointX/fMaxWindowWidth;
        //edited by Mike, 20210512
        //		return iPointX/fMaxWindowWidth;
        return (iPointX-fHalfWindowWidth)/fHalfWindowWidth;
    }
    
    //	return (fMaxWindowWidth-iPointX)/fMaxWindowWidth;
}

//added by Mike, 20210511
//note: vertex origin 0,0 at center
float UsbongUtils::autoConvertFromPixelToVertexPointY(int iPointY)
{
    //max width and height: 640
    //set in mainLinux.cpp
    //TO-DO: -update: this
    //square window
    /*  //removed by Mike, 20210623
     float fMaxWindowWidth=640.0f;
     float fMaxWindowHeight=640.0f;
     */
    float fHalfWindowWidth=fMaxWindowWidth/2;
    float fHalfWindowHeight=fMaxWindowHeight/2;
    
    //TO-DO: -reverify: this
    //max 1.0f
    if (iPointY<=fHalfWindowHeight) {
        //note: pixel point origin at top-left
        //note: pixel point uses inverted y-axis
        //		return 0.25+(fHalfWindowHeight-iPointY)/fMaxWindowHeight; //note: use of parenthesis
        //		return (fHalfWindowHeight-iPointY)/fMaxWindowHeight; //note: use of parenthesis
        //edited by Mike, 20210512
        //		return (fMaxWindowHeight-iPointY)/fMaxWindowHeight; //note: use of parenthesis
        return (fHalfWindowHeight-iPointY)/fHalfWindowHeight; //note: use of parenthesis
    }
    //edited by Mike, 20210512
    else if (iPointY==0) {
        return 0;
    }
    //edited by Mike, 20210512
    else {
        //	return 0.25+iPointY/fMaxWindowHeight*(-1);
        //		return iPointY/fMaxWindowHeight*(-1);	
        //edited by Mike, 20210512
        //		return iPointY/fHalfWindowHeight*(-1);			
        return (iPointY-fHalfWindowHeight)/fHalfWindowHeight*(-1);				
    }
}

//added by Mike, 20210712
float UsbongUtils::autoConvertFromPixelToVertexGridTileWidth(int iGridTileWidth)
{
	return 0.0f+autoConvertFromPixelToVertexPointX(iGridTileWidth);
}

//added by Mike, 20210712
float UsbongUtils::autoConvertFromPixelToVertexGridTileHeight(int iGridTileHeight)
{
	return 0.0f+autoConvertFromPixelToVertexPointY(iGridTileHeight);
}

//added by Mike, 20210720
//example input#1: "2-0"
//output: 2
//example input#2: "99-0"
//output: 99
//reminder: in macOS, use XCode to edit input file, e.g. inputLevel1.csv
//this is instead of TextEdit; due to open and close quotation mark encodings, et cetera
int UsbongUtils::autoIdentifyColumnInputInLevelMapContainer(std::string sInput)
{
    int iLength=sInput.length();
	int iOutput=0;
	int iDigitCountFromRightToLeft=1;
	int iCountLeftToRight=0;
	int iCountRightToLeft=0;
	
	//character from left to right
	for (iCountLeftToRight=0; iCountLeftToRight<iLength; iCountLeftToRight++) {
        
//        std::cout << "sInput[iCountLeftToRight]: " << sInput[iCountLeftToRight] << "\n";
        
		if (sInput[iCountLeftToRight]=='-') {
			break;
		}
	}
    
    //character from right to left
	//note: left-most character, i.e. quotation mark not included
	for (iCountRightToLeft=iCountLeftToRight-1; iCountRightToLeft>0; iCountRightToLeft--) {
			iOutput = iOutput+ (sInput[iCountRightToLeft]-'0')*iDigitCountFromRightToLeft;
			iDigitCountFromRightToLeft = iDigitCountFromRightToLeft*10;
	}

	return iOutput;
}

//TO-DO: -use: A1, B1... AA1, AB1?

//added by Mike, 20210720
//example input#1: "0-3"
//output: 3
//example input#2: "0-28"
//output: 28
int UsbongUtils::autoIdentifyRowInputInLevelMapContainer(std::string sInput)
{
  int iLength=sInput.length();
	int iOutput=0;
	int iDigitCountFromRightToLeft=1;
	int iCountLeftToRight=0;
	int iCountRightToLeft=0;
	int iDelimeterPosition=0;
	
	//character from left to right
	for (iCountLeftToRight=0; iCountLeftToRight<iLength; iCountLeftToRight++) {
		if (sInput[iCountLeftToRight]=='-') {
			break;
		}
	}
	
	iDelimeterPosition=iCountLeftToRight;
	
	//character from right to left	
	//note: left-most character, i.e. quotation mark not included
	//therefore, -2, instead of -1
	for (iCountRightToLeft=iLength-2; iCountRightToLeft>iDelimeterPosition; iCountRightToLeft--) {
			iOutput = iOutput+ (sInput[iCountRightToLeft]-'0')*iDigitCountFromRightToLeft;		
			iDigitCountFromRightToLeft = iDigitCountFromRightToLeft*10;
	}

	return iOutput;
}

//added by Mike, 20210729
//example input#1: "0-3"
//output: 0-3
std::string UsbongUtils::autoDeleteQuotationMark(std::string sInput)
{
  int iLength=sInput.length();
	int iCountLeftToRight=0;	
	std::string sOutput="";
	
	//character from left to right
	for (iCountLeftToRight=0; iCountLeftToRight<iLength; iCountLeftToRight++) {
		if (sInput[iCountLeftToRight]=='"') {
			continue;
		}
		
		sOutput = sOutput + sInput[iCountLeftToRight];		
//		std::cout << "sOutput: " << sOutput << "\n";
	}

	return sOutput;
}
