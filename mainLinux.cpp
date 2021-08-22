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

//TO-DO: -add: installation steps of SDL and SDL Image

//added by Mike, 20210821
//TO-DO: -reverify: output using https://wiki.libsdl.org/MigrationGuide

/**************************
 * Includes
 *
 **************************/

/* //edited by Mike, 20201210
 //#include <windows.h> //removed by Mike, 20200928 due to Linux Machine
 #include <GL/gl.h>
 #include <GL/glut.h> //added by Mike, 20200927
 #include <GL/glu.h> //added by Mike, 20200926
 */

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

//added by Mike, 20210818
//TO-DO: -remvoe: this

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

//added by Mike, 20210818
#ifdef _WIN32 //Windows machine
	#include <SDL.h>
	#include <SDL_image.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif

//added by Mike, 20210821
#include "OpenGLCanvas.h"

//added by Mike, 20201210
#include <time.h>
#include <unistd.h>

//added by Mike, 20201002
#include <iostream>

/**************************
 * Function Declarations
 *
 **************************/
/*
 LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
 WPARAM wParam, LPARAM lParam);
 void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
 void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);
 */

/* //removed by Mike, 20210130
 //added by Mike, 20201002
 #define VK_UP 101
 #define VK_DOWN 103
 #define VK_LEFT 100
 #define VK_RIGHT 102
 */

//note: "static" in C/C++ = "final" in java
/*	//edited by Mike, 20201202
 static int myWindowWidth=1024;//640; //320
 static int myWindowHeight=1024;//640; //320
 */
static int myWindowWidthAsPixel=640; //2048;//320
//edited by Mike, 20210507
static int myWindowHeightAsPixel=640; //2048;//320
//static int myWindowHeight=320; //2048;//320//640

/*	//edited by Mike, 20210818
#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720
*/

//added by Mike, 20210821
OpenGLCanvas *myOpenGLCanvas = NULL;

//added by Mike, 20210818
SDL_Window *mySDLWindow = NULL;
SDL_Renderer *mySDLRenderer = NULL;

//added by Mike, 20210819
int iPilotX;
int iPilotY;
int iCountTaoAnimationFrame;

int myKeysDown[10]; //note: includes KEY_J, KEY_L, KEY_I, KEY_K,

//added by Mike, 20201226
#define TRUE 1
#define FALSE 0

//added by Mike, 20210510
//note: keys and mouseActionIds equal with that in OpenGLCanvas.cpp

//added by Mike, 20201121
enum Keys
{
    KEY_W = 0,
    KEY_S,
    KEY_D,
    KEY_A,
    KEY_J,
    KEY_L,
    KEY_I,
    KEY_K,
    //added by Mike, 20210123
    KEY_H,
    KEY_U,
    //added by Mike, 20210513
    KEY_Z,
    
    iNumOfKeyTypes
};

//added by Mike, 20210510
enum mouseActionIds
{
    MOUSE_LEFT_BUTTON = 0,
    iNumOfMouseActionIdTypes
};

//added by Mike, 20201001; edited by Mike, 20201210
bool bMyPause;

//added by Mike, 20210514
int myMouseActionState;

//Reference: https://www3.ntu.edu.sg/home/ehchua/programming/opengl/HowTo_OpenGL_C.html;
//last accessed: 20200928
void displayExample() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
    
    // Draw a Red 1x1 Square centered at origin
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex2f(-0.5f, -0.5f);    // x, y
    glVertex2f( 0.5f, -0.5f);
    glVertex2f( 0.5f,  0.5f);
    glVertex2f(-0.5f,  0.5f);
    glEnd();
    
    glFlush();  // Render now
}

//added by Mike, 20200928; edited by Mike, 20201202
void display() { //Linux Machine
    //void display(HDC hDC) { //Windows Machine
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
    
    //set TOP-LEFT origin/anchor/reference point; quadrant 4, y-axis inverted; x and y positive
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //TOP-Left origin
    glOrtho(0.0f, //left
            1.0f, //right
            1.0f, //bottom
            0.0f, //top
            0.0f, //zNear; minimum
            1.0f //zFar; maxinum
            );
    
    //draw grid
    //TO-DO: -update: iRowCountMax
    int iRowCountMax=10;
    //TO-DO: -update: iColumnCountMax
    int iColumnCountMax=10;
    
    // Draw a Green Line top-left origin; Quadrant 4, y-axis inverted; x and y positive
    //rows
    for (int iRowCount=0; iRowCount<iRowCountMax; iRowCount++) {
        // Draw a Green Line top-left origin
        glBegin(GL_LINES);
      		glColor3f(0.0f, 1.0f, 0.0f); // Green
      		glVertex2f(0.0f, 0.1f*iRowCount);    // x, y
      		//TO-DO: -add: auto-compute myWindowWidth
      		glVertex2f(1.0f, 0.1f*iRowCount);
        glEnd();
    }
    
    //columns
    for (int iColumnCount=0; iColumnCount<iColumnCountMax; iColumnCount++) {
        // Draw a Green Line top-left origin
        glBegin(GL_LINES);
      		glColor3f(0.0f, 1.0f, 0.0f); // Green
      		glVertex2f(0.1f*iColumnCount, 0.0f);    // x, y
      		//TO-DO: -add: auto-compute myWindowHeight
      		glVertex2f(0.1f*iColumnCount, 1.0f);
        glEnd();
    }
    
    //edited by Mike, 20201202
    //glFlush();  // Render now
    glutSwapBuffers();
}

//added by Mike, 20200930; edited by Mike, 20210821
void displayOpenGLCanvasPrev() {
    //removed by Mike, 20200930
    //Linux Machine
    /*    myOpenGLCanvas = new OpenGLCanvas;
     myOpenGLCanvas->init();
     */
    
    //added by Mike, 20201001
    //init stuff for delay
    int skip=0, currSysTime=0,
    timeElapsed,
    //edited by Mike, 20201205
    //TO-DO: -update: this
    idealFrameTime=33;//60;//33;
    //edited by Mike, 20201210
    //pause=false;//0;
    bMyPause=false;
    
    //		if (!pause) {
    if (!bMyPause) {
        //TO-DO: -add: this
        //removed by Mike, 20201002
        //currSysTime=GetTickCount(); //Windows Machine
        
        /* //edited by Mike, 20201210
         //added by Mike, 20201210
         //time_t now = time(0); //Get the system time
         //printf("time now: %d",(int)now);
         currSysTime = (int) time(0); //Linux Machine
         */
        
        /* OpenGL animation code goes here */
        //removed by Mike, 20210726
//        myOpenGLCanvas->update();
        
        /*		  //edited by Mike, 20201210
         if (skip > 0)
         skip = skip-1;
         else {
         */
        //do rendering here
/* //removed by Mike, 20210818			        
        myOpenGLCanvas->render();
*/
        
        //removed by Mike, 20201002
        //SwapBuffers (hDC); //Windows Machine
        //edited by Mike, 20201205
        //   							glFlush();  // Render now //Linux Machine
        glutSwapBuffers();
        
        //edited by Mike, 20201210
        /*
         //TO-DO: -add: auto-identify if Windows Machine
         //timeElapsed=GetTickCount()-currSysTime;
         timeElapsed=(int)time(0)-currSysTime; //Linux Machine
         
         
         if (timeElapsed>idealFrameTime) {
         skip = (timeElapsed/idealFrameTime) - 1;
         }
         else {
         sleep(idealFrameTime - timeElapsed);
         //pause(idealFrameTime - timeElapsed);
         }
         }
         */
        //added by Mike, 20201002
        //			glutPostRedisplay();
        
        //reference: https://stackoverflow.com/questions/35563360/looping-in-opengl-with-gluttimerfunc;
        //answer by: spookyPuppy, 20160223T0445
        //edited by: datenwolf, 20160223T0631
        //note: void glutTimerFunc(unsigned int numMilliseconds, functionCallback, value);
        //glutTimerFunc(1000 / SCREEN_FPS, Loop, 0);
        //			glutTimerFunc(1000, NULL, 0);
        
    }
}

//added by Mike, 20210822
//TO-DO: -reverify: refresh rate to cause displayed parts of pixel texture to be incorrect
//note: did NOT observe this with openGL + GLUT without SDL
void displayOpenGLCanvas() {
	myOpenGLCanvas->render();
	
/*	//added by Mike, 20210821; edited by Mike, 20210821
  SDL_GL_SwapBuffers();
  SDL_UpdateWindowSurface(mySDLWindow);
*/  
  	SDL_GL_SwapWindow(mySDLWindow);
}

/* //removed by Mike, 20210822
void update(int i) {
	myOpenGLCanvas->update();
}
*/

//added by Mike, 20200930
void updatePrev(int i) {
    //removed by Mike, 20200930
    //Linux Machine
    /*    myOpenGLCanvas = new OpenGLCanvas;
     myOpenGLCanvas->init();
     */
    
    //added by Mike, 20201001
    //init stuff for delay
    int skip=0, currSysTime=0,
    timeElapsed,
    idealFrameTime=60;//33;
    //edited by Mike, 20201210
    //    pause=false;//0;
    bMyPause=false;//0;
    
    
    //edited by Mike, 20201210
    //		if (!pause) {
    if (!bMyPause) {
        //removed by Mike, 20201002
        //currSysTime=GetTickCount(); //Linux Machine
        
        /* OpenGL animation code goes here */
/* //removed by Mike, 20210818        
        myOpenGLCanvas->update();
*/        
        /*				//removed by Mike, 20201002, Linux Machine
         if (skip > 0)
         skip = skip-1;
         else {
         */
        
        /*							//removed by Mike, 20201002, Linux Machine
         timeElapsed=GetTickCount()-currSysTime;
         if (timeElapsed>idealFrameTime)
         skip = (timeElapsed/idealFrameTime) - 1;
         else
         Sleep(idealFrameTime - timeElapsed);
         }
         */
        //added by Mike, 20201002
        glutPostRedisplay();
        
        //TO-DO: -add: auto-identify if Windows Machine
        //reference: https://stackoverflow.com/questions/35563360/looping-in-opengl-with-gluttimerfunc;
        //answer by: spookyPuppy, 20160223T0445
        //edited by: datenwolf, 20160223T0631
        //note: void glutTimerFunc(unsigned int numMilliseconds, functionCallback, value);
        //glutTimerFunc(1000 / SCREEN_FPS, Loop, 0);
        //edited by Mike, 20201202
        //			glutTimerFunc(50, update, 0);
        //edited by Mike, 20201210
        //			glutTimerFunc(70, update, 0);
        //			glutTimerFunc(80, update, 0);
        //edited by Mike, 20210115
        //TO-DO: -reverify: add RobotShip movement animation delay
        //using lower canvas auto-draw update delay
        //glutTimerFunc(40, update, 0);
        
/*	//removed by Mike, 20210822
	//note: uses update(int parameter);        
        glutTimerFunc(50, update, 0);
*/        
    }
}


/*	//removed by Mike, 20210818
//note: special functions, e.g. character keys
//added by Mike, 20201002
void keyDown (unsigned char key, int x, int y)
{
    //note: space bar and escape keys detected, albeit blank output
    //    std::cout << "keydown " << key << "\n";
    
    //added by Mike, 20201121
    switch (key)
    {
            ///			   //removed by Mike, 20201121
             //TO-DO: -add: identify VK_SPACE in Linux Machine
             case VK_SPACE:
             myOpenGLCanvas->keyDown(KEY_SPACE);
             return;
             ////
            
            //added by Mike, 20201013
            //reference:
            //https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
            //last accessed: 20201013
            //TO-DO: -add: auto-identify if Linux Machine
            //edited by Mike, 20201121
            //   	       case 0x41: //A key //Windows Machine
        case (int) 'a': //A key //Linux Machine
            myOpenGLCanvas->keyDown(KEY_A);
            return;
            //edited by Mike, 20201121
            //   	       case 0x44: //D key //Windows Machine
        case (int) 'd': //d key //Linux Machine
            //edited by Mike, 20210128
            myOpenGLCanvas->keyDown(KEY_D);
            //		        myOpenGLCanvas->keyDown(KEY_RIGHT);
            return;
            //edited by Mike, 20201121
            //		   case 0x57: //W key //Windows Machine
        case (int) 'w': //w key //Linux Machine
            //   	       case 0x41: //W key
            //			case VK_UP:
            //edited by Mike, 20210128
            //		        myOpenGLCanvas->keyDown(KEY_UP);//KEY_W);
            myOpenGLCanvas->keyDown(KEY_W);
            return;
            //edited by Mike, 20201121
            //		   case 0x53: //S key //Windows Machine
        case (int) 's': //s key //Linux Machine
            myOpenGLCanvas->keyDown(KEY_S);
            //		        myOpenGLCanvas->keyDown(KEY_DOWN);
            return;
            //edited by Mike, 20201121
            //			case 0x4A: //J key //Windows Machine
        case (int) 'j': //j key //Linux Machine
            myOpenGLCanvas->keyDown(KEY_J);
            return;
            //edited by Mike, 20201121
            //		   case 0x4C: //L key //Windows Machine
        case (int) 'l': //l key //Linux Machine
            myOpenGLCanvas->keyDown(KEY_L);
            return;
            //edited by Mike, 20201121
            //			case 0x49: //I key //Windows Machine
        case (int) 'i': //i key //Linux Machine
            myOpenGLCanvas->keyDown(KEY_I);
            return;
            //edited by Mike, 20201121
            //			case 0x4B: //K key //Windows Machine
        case (int) 'k': //k key //Linux Machine
            myOpenGLCanvas->keyDown(KEY_K);
            return;
            //added by Mike, 20210123
        case (int) 'h': //h key //Linux Machine
            myOpenGLCanvas->keyDown(KEY_H);
            return;
        case (int) 'u': //u key //Linux Machine
            myOpenGLCanvas->keyDown(KEY_U);
            return;
            
            //added by Mike, 20210513
            //TO-DO: -add: ctrl + z
        case (int) 'z': //z key //Linux Machine
            myOpenGLCanvas->keyDown(KEY_Z);
            return;
            
            
            //removed by Mike, 20201001
            ////
//             case 13: //ENTER
//             myOpenGLCanvas->keyDown(KEY_ENTER);
//             return 0;
//             case 80: //P
//             if (myOpenGLCanvas->currentState!=TITLE_SCREEN) {
//             if (pause==0) //false
//			          pause=1; //make it true
//             else pause=0;
//             }
//             return 0;
             ////
    }
}
*/

/*	//removed by Mike, 20210818
//added by Mike, 20201002
void keyUp (unsigned char key, int x, int y)
{
    //    std::cout << "keyup " << key << "\n";
    
    //added by Mike, 20201121
    switch (key)
    {
            ////removed by Mike, 20201121
             ////TO-DO: -add: identify VK_SPACE in Linux Machine
             //case VK_SPACE:
             //myOpenGLCanvas->keyUp(KEY_SPACE);
             //return;
             ////
	           //added by Mike, 20201013
	           //reference:
	           //https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	           //last accessed: 20201013
            //edited by Mike, 20201121
            //			   case 0x41: //A key //Windows Machine
        case (int) 'a': //d key //Linux Machine
            myOpenGLCanvas->keyUp(KEY_A);
            return;
            //edited by Mike, 20201121
	   	       //case 0x44: //D key //Windows Machine
        case (int) 'd': //d key //Linux Machine
            //edited by Mike, 20210128
            myOpenGLCanvas->keyUp(KEY_D);
            //			        myOpenGLCanvas->keyUp(KEY_RIGHT);
            return;
            //edited by Mike, 20201121
            //	   	       case 0x57: //W key //Windows Machine
        case (int) 'w': //w key //Linux Machine
            //edited by Mike, 20210128
            //			        myOpenGLCanvas->keyUp(KEY_UP);
            myOpenGLCanvas->keyUp(KEY_W);
            return;
            //edited by Mike, 20201121
            //	   	       case 0x53: //S key //Windows Machine
        case (int) 's': //s key //Linux Machine
            myOpenGLCanvas->keyUp(KEY_S);
            //			        myOpenGLCanvas->keyUp(KEY_DOWN);
            return;
            //edited by Mike, 20201121
            //			   case 0x4A: //J key //Windows Machine
        case (int) 'j': //j key //Linux Machine
            myOpenGLCanvas->keyUp(KEY_J);
            return;
            //edited by Mike, 20201121
            //				case 0x4C: //L key //Windows Machine
        case (int) 'l': //l key //Linux Machine
            myOpenGLCanvas->keyUp(KEY_L);
            return;
            //edited by Mike, 20201121
            //			case 0x49: //I key //Windows Machine
        case (int) 'i': //i key //Linux Machine
            myOpenGLCanvas->keyUp(KEY_I);
            return;
            //edited by Mike, 20201121
            //			case 0x4B: //K key //Windows Machine
        case (int) 'k': //k key //Linux Machine
            myOpenGLCanvas->keyUp(KEY_K);
            return;
            //added by Mike, 20210123
        case (int) 'h': //h key //Linux Machine
            myOpenGLCanvas->keyUp(KEY_H);
            return;
        case (int) 'u': //u key //Linux Machine
            myOpenGLCanvas->keyUp(KEY_U);
            return;
            
            //added by Mike, 20210513
            //TO-DO: -add: ctrl + z
        case (int) 'z': //z key //Linux Machine
            myOpenGLCanvas->keyUp(KEY_Z);
            return;
            
            ////removed by Mike, 20210130
             //case 13: //ENTER
             //myOpenGLCanvas->keyUp(KEY_ENTER);
             //return;
             ////
    }
    return;
}
*/

//note: special functions, e.g. shift, control, arrow keys
//UP: 101
//DOWN: 103
//LEFT: 100
//RIGHT: 102
//added by Mike, 20201002
void specialKeyDown (int specialKey, int x, int y)
{
    //noted by Mike, 20201121
    //Note: space bar and escape key not detected
    //    std::cout << "keydown " << specialKey << "\n";
    
    switch (specialKey)
    {
            /* //edited by Mike, 20201121
             //added by Mike, 20201001
             case VK_LEFT:
             myOpenGLCanvas->keyDown(KEY_LEFT);
             return;
             case VK_RIGHT:
             myOpenGLCanvas->keyDown(KEY_RIGHT);
             return;
             case VK_UP:
             myOpenGLCanvas->keyDown(KEY_UP);
             return;
             case VK_DOWN:
             myOpenGLCanvas->keyDown(KEY_DOWN);
             return;
             //removed by Mike, 20201002
             //   	       case VK_SPACE:
             //                myOpenGLCanvas->keyDown(KEY_SPACE);
             //                return;
             */
            //TO-DO: -add: this
            //removed by Mike, 20201121
            /*
             case VK_ESCAPE:
	            PostQuitMessage(0);
	            return;// 0;
             */
            //removed by Mike, 20210130
            /*
             //added by Mike, 20201001
             case VK_LEFT:
             myOpenGLCanvas->keyDown(KEY_LEFT);
             return;// 0;
             case VK_RIGHT:
             myOpenGLCanvas->keyDown(KEY_RIGHT);
             return;
             case VK_UP:
             myOpenGLCanvas->keyDown(KEY_UP);
             return;
             case VK_DOWN:
             myOpenGLCanvas->keyDown(KEY_DOWN);
             return;
             */
    }
    return;
    
}
//added by Mike, 20201002
void specialKeyUp (int specialKey, int x, int y)
{
    //    std::cout << "keyup " << specialKey << "\n";
    
    switch (specialKey)
    {
            //removed by Mike, 20201121
            /*
             case VK_LEFT:
             myOpenGLCanvas->keyUp(KEY_LEFT);
             return;
             case VK_RIGHT:
             myOpenGLCanvas->keyUp(KEY_RIGHT);
             return;
             case VK_UP:
             myOpenGLCanvas->keyUp(KEY_UP);
             return;
             case VK_DOWN:
             myOpenGLCanvas->keyUp(KEY_DOWN);
             return;
             //removed by Mike, 20201002
             //   	       case VK_SPACE:
             //                myOpenGLCanvas->keyUp(KEY_SPACE);
             //                return;
             */
            //removed by Mike,  20210130
            /*
             case VK_LEFT:
             myOpenGLCanvas->keyUp(KEY_LEFT);
             return;// 0;
             case VK_RIGHT:
             myOpenGLCanvas->keyUp(KEY_RIGHT);
             return;
             case VK_UP:
             myOpenGLCanvas->keyUp(KEY_UP);
             return;
             case VK_DOWN:
             myOpenGLCanvas->keyUp(KEY_DOWN);
             return;
             */
    }
    return;
}

//added by Mike, 20210510
//Reference: https://stackoverflow.com/questions/13296906/new-to-opengl-glutmousefunc;
//last accessed; 20210510
//question by: Aseel84, 20121108T2007; edited by: Ivanzinho, 20201221T1531
//answer by: Xymostech, 20211108T2317
//note: window's top-left origin
/* //removed by Mike, 20210818
void myMouseAction(int button, int state, int x, int y)
{
    //added by Mike, 20210512
    //TO-DO: -add: identify x and y positions using mouse hover
    
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                printf("DOWN mouse xPos: %i; yPos: %i\n",x,y);
                //				myOpenGLCanvas->mouseActionDown(MOUSE_LEFT_BUTTON);
                
                myOpenGLCanvas->mouseActionDown(MOUSE_LEFT_BUTTON, x, y);
                
                //				myOpenGLCanvas->mouseActionUp(MOUSE_LEFT_BUTTON, x, y);
                
                //added by Mike, 20210514
                myMouseActionState=GLUT_DOWN;
                
                glutPostRedisplay();
            }
            else if (state == GLUT_UP)
            {myOpenGLCanvas
                printf("UP mouse xPos: %i; yPos: %i\n",x,y);
                //				myOpenGLCanvas->mouseActionUp(MOUSE_LEFT_BUTTON);
                
                myOpenGLCanvas->mouseActionUp(MOUSE_LEFT_BUTTON, x, y);
                
                //				myOpenGLCanvas->mouseActionDown(MOUSE_LEFT_BUTTON, x, y);
                
                glutPostRedisplay();
            }
            break;
    }
}
*/

/* //removed by Mike, 20210818
//added by Mike, 20210514
void myMouseMotionAction(int x, int y)
{
    //added by Mike, 20210512
    //TO-DO: -add: identify x and y positions using mouse hover
    
    //added by Mike, 20210515
    //Reference: https://stackoverflow.com/questions/15147515/get-x-position-at-start-of-motion-and-end-using-glutmotionfunc;
    //last accessed: 20210514
    if (myMouseActionState==GLUT_DOWN) {
        myOpenGLCanvas->mouseMotionActionDown(MOUSE_LEFT_BUTTON, x, y);
    }
}
*/

/* //edited by Mike, 20210818
int main(int argc, char** argv) {
    glutInit(&argc, argv);                 // InglutReshapeWindowitialize GLUT
    //   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); //added by Mike, 20201202
    //glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GL_DOUBLE);
    
    //edited by Mike, 20210623
    //   glutInitWindowSize(myWindowWidthAsPixel, myWindowHeightAsPixel);   // Set the window's initial width & height
    glutInitWindowSize(GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT);   // Set the window's initial width & height
    
    
    
    //   glutInitWindowSize(2048,2048);   // Set the window's initial width & height
    
    //edited by Mike, 20201202
    //   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
    
    //edited by Mike, 20201202
    //note: put this command after all the glutInit Window commands
    //edited by Mike, 20210530
    //   glutCreateWindow("Usbong Paint Equation Tool"); // Create a window with the given title
    //edited by Mike, 20210603
    //   glutCreateWindow("Usbong Volley"); // Create a window with the given title
    //edited by Mike, 20210614
    //   glutCreateWindow("Usbong Foot"); // Create a window with the given title
    //TO-DO: -remove: window title
    //edited by Mike, 20210809
	   glutCreateWindow("Usbong Story System"); // Create a window with the given title
    
    //added by Mike, 20210614
    //note: execute after glutCreateWindow(...)
    //can use the F11 keyboard button to switch from Full Screen to set Window Size
    
    //edited by Mike, 20210623
    //TO-DO: -fix: coordinate system of mouse key down and up positions
    glutFullScreen(); //removes Window Title Bar
    
    
    //added by Mike, 20201002
    //Linux Machine
    //note: to receive key presses
    //edited by Mike, 20210726
    myOpenGLCanvas = new OpenGLCanvas();
    
    //edited by Mike, 20210517
    //    myOpenGLCanvas->init();
    //pixel, instead of vertex points
    //TO-DO: -add: i to be iMyWindowWidthAsPixel, et cetera
    //edited by Mike, 20210623
    //    myOpenGLCanvas->init(myWindowWidthAsPixel,myWindowHeightAsPixel);
    myOpenGLCanvas->init(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
//    myOpenGLCanvas->init(768.0f, 768.0f);
        
//    myOpenGLCanvas->init(GLUT_WINDOW_WIDTH,GLUT_WINDOW_HEIGHT);
//    myOpenGLCanvas->init(GLUT_INIT_WINDOW_WIDTH,GLUT_INIT_WINDOW_HEIGHT);
    
    //edited by Mike, 20200930
   	//glutDisplayFunc(display); // Register display callback handler for window re-paint
    
   	//edited by Mike, 20201002
    //	 	glutDisplayFunc(displayOpenGLCanvas); // Register display callback handler for window re-paint
    glutDisplayFunc(displayOpenGLCanvas); // Register display callback handler for window re-paint
    //displayOpenGLCanvas(); //output of this = glutDisplayFunc(displayOpenGLCanvas);
	 		
    update(0);
	 		
    //added by Mike, 20201002
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    ////	//removed by Mike, 20210130
    //glutSpecialFunc(specialKeyDown);
    //glutSpecialUpFunc(specialKeyUp);
    ////
    //added by Mike, 20210510
    glutMouseFunc(myMouseAction);
    
    //added by Mike, 20210514
    //Reference: https://www.opengl.org/resources/libraries/glut/spec3/node51.html;
    //last accessed: 20210514
    ////
    // void glutMotionFunc(void (*func)(int x, int y));
    // void glutPassiveMotionFunc(void (*func)(int x, int y));
    ////
    glutMotionFunc(myMouseMotionAction);
    
    //added by Mike, 20210128
    glutIgnoreKeyRepeat(1);
    
   	glutMainLoop();           // Enter the infinitely event-processing loop
   	
    
    return 0;
}
*/

/* //removed by Mike, 20210818
typedef struct {
	SDL_Renderer *renderer;
	SDL_Window *window;
} app;
*/

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

void keyDown(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
//		if (event->keysym.scancode == SDL_SCANCODE_UP)
		if (event->keysym.scancode == SDL_SCANCODE_W)
		{
//			iPilotY-=4;
			//edited by Mike, 20210822
//			myKeysDown[KEY_W] = TRUE;					
			myOpenGLCanvas->keyDown(KEY_W);
		}

//		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		if (event->keysym.scancode == SDL_SCANCODE_S)
		{
//			iPilotY+=4;
			//edited by Mike, 20210822
//			myKeysDown[KEY_S] = TRUE;	
			myOpenGLCanvas->keyDown(KEY_S);						
		}

//		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		if (event->keysym.scancode == SDL_SCANCODE_A)
		{
//			iPilotX-=4;
			//edited by Mike, 20210822
//			myKeysDown[KEY_A] = TRUE;					
			myOpenGLCanvas->keyDown(KEY_A);
		}

//		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		if (event->keysym.scancode == SDL_SCANCODE_D)
		{
//			iPilotX+=4;
			//edited by Mike, 20210822
//			myKeysDown[KEY_D] = TRUE;			
			myOpenGLCanvas->keyDown(KEY_D);
		}
	}
}

void keyUp(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
//		if (event->keysym.scancode == SDL_SCANCODE_UP)
		if (event->keysym.scancode == SDL_SCANCODE_W)
		{
			//edited by Mike, 20210822
//			myKeysDown[KEY_W] = FALSE;			
			myOpenGLCanvas->keyUp(KEY_W);					
		}

//		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		if (event->keysym.scancode == SDL_SCANCODE_S)
		{
			//edited by Mike, 20210822
//			myKeysDown[KEY_S] = FALSE;					
			myOpenGLCanvas->keyUp(KEY_S);
		}

//		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		if (event->keysym.scancode == SDL_SCANCODE_A)
		{
			//edited by Mike, 20210822
//			myKeysDown[KEY_A] = FALSE;					
			myOpenGLCanvas->keyUp(KEY_A);
		}

//		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		if (event->keysym.scancode == SDL_SCANCODE_D)
		{
			//edited by Mike, 20210822
//			myKeysDown[KEY_D] = FALSE;					
			myOpenGLCanvas->keyUp(KEY_D);
		}
	}
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
				keyDown(&event.key);
				break;

			case SDL_KEYUP:
				keyUp(&event.key);
				break;

			default:
				break;
		}
	}
}

void prepareScene(void)
{
	//edited by Mike, 20210818
//	SDL_SetRenderDrawColor(mySDLRenderer, 96, 128, 255, 255);
//	SDL_SetRenderDrawColor(mySDLRenderer, 255, 0, 0, 255); //red
	//note: SDL color max 255; GIMP color max 100
	SDL_SetRenderDrawColor(mySDLRenderer, 0, 255*0.667, 255*0.494, 255); //blue green
	
	SDL_RenderClear(mySDLRenderer);
}

void presentScene(void)
{
	SDL_RenderPresent(mySDLRenderer);
}

//added by Mike, 20210818
//TO-DO: -put: this in Pilot.cpp, et cetera
SDL_Texture *loadTexture(char *filename)
{
	SDL_Texture *texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(mySDLRenderer, filename);

	return texture;
}


//TO-DO: -reverify: draw refresh rate
//Reference: http://wiki.libsdl.org/SDL_RenderCopy;
//last accessed: 20210818
//TO-DO: -reverify: this
void draw(SDL_Texture *texture, int x, int y)
{
/* //edited by Mike, 20210818
	SDL_Rect dest;
	//note: not initialized?
	dest.x = x;
	dest.y = y;
	
	dest.w = iPilotWidth;
	dest.h = iPilotHeight;
*/	
	//added by Mike, 20210818
	int iPilotWidth=64;
	int iPilotHeight=64;

	
  /* Rectangles for drawing which will specify source (inside the texture)
  and target (on the screen) for rendering our textures. */
  SDL_Rect SrcR;
  SDL_Rect DestR;
  
	iCountTaoAnimationFrame=(iCountTaoAnimationFrame)%3;                    																				    

  SrcR.x = 0+ iCountTaoAnimationFrame*iPilotWidth;
  SrcR.y = 0;
  SrcR.w = iPilotWidth;
  SrcR.h = iPilotHeight;

  DestR.x = x; //myWindowWidthAsPixel / 2 - iPilotWidth / 2;
  DestR.y = y; //myWindowHeightAsPixel / 2 - iPilotHeight / 2;
  DestR.w = iPilotWidth;
  DestR.h = iPilotHeight;
  	
  int iCount;
  for (iCount=0; iCount<iNumOfKeyTypes; iCount++) {
		if (myKeysDown[iCount]==TRUE) {
 			iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;																				
 			break;
		}
  }
  if (iCount==iNumOfKeyTypes) {
 			iCountTaoAnimationFrame=0;																				
  }
	
/*	
	SDL_Rect *myClip;
	myClip->x=0;
	myClip->y=0;
	myClip->w=64;
	myClip->h=64;
	
	dest.w = myClip->w;
	dest.h = myClip->h;
*/	
	SDL_RenderClear(mySDLRenderer);
	
/*	
	dest.w = iPilotWidth;
	dest.h = iPilotHeight;
*/	
//	printf("dest.w: %i\n",dest.w);
		
/*	
	int iDestWidth=dest.x+iPilotWidth;
	int iDestHeight=dest.y+iPilotHeight;
*/		
	//edited by Mike, 20210818; removed by Mike, 20210818
	//SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	
	
//	SDL_QueryTexture(texture, NULL, NULL, &iDestWidth, &iDestHeight);
//	SDL_QueryTexture(texture, NULL, NULL, &iDestWidth, &iDestHeight);

	//scaled down image
/*	
	dest.w = iPilotWidth;
	dest.h = iPilotHeight;
*/
	//Reference: https://www.willusher.io/sdl2%20tutorials/2013/08/27/lesson-5-clipping-sprite-sheets;
	//last accessed: 20210818
	//renderTexture(texture, mySDLRenderer, dest, &myClip);

/*	//removed by Mike, 20210818
//	SDL_RenderDrawRect(mySDLRenderer, &dest);
//	SDL_RenderCopy(mySDLRenderer, texture, NULL, &dest);
	SDL_RenderCopy(mySDLRenderer, texture, nullptr, &dest);
*/	
	
	//edited by Mike, 20210818
	//SDL_RenderCopy(mySDLRenderer, texture, nullptr, &dest);

	SDL_RenderCopy(mySDLRenderer, texture, &SrcR, &DestR);
	SDL_RenderPresent(mySDLRenderer);
}

void update() {
/* //removed by Mike, 20210822
		if (myKeysDown[KEY_W])
		{
			iPilotY-=4;
		}

		if (myKeysDown[KEY_S])
		{
			iPilotY+=4;
		}

		if (myKeysDown[KEY_A])
		{
			iPilotX-=4;
		}

		if (myKeysDown[KEY_D])
		{
			iPilotX+=4;
		}
*/		
 		//added by Mike, 20210822
		myOpenGLCanvas->update();		
}

/*
int mainFromUsbongPagong(int argc, char *argv[])
{
 	//removed by Mike, 20210818
//	memset(&App, 0, sizeof(App));

	initSDL();
	
	//added by Mike, 20210818; edited by Mike, 20210818
	//TO-DO: -add: in Pilot.cpp, et cetera	
////	player.x = 100;
////	player.y = 100;
////	player.texture = loadTexture("gfx/player.png");


////	char* pilotTextureInput = (char*)"textures/imageSpriteExampleMikeWithoutBG.png";
////	SDL_Texture *texture = loadTexture(pilotTextureInput);

	//solution to problem: ISO C++ forbids converting a string constant to 'char*' [-Wwrite-strings]
	SDL_Texture *texture = loadTexture((char*)"textures/imageSpriteExampleMikeWithoutBG.png");

	iPilotX=myWindowWidthAsPixel/2;
	iPilotY=myWindowHeightAsPixel/2;
	
	//added by Mike, 20210819
	iCountTaoAnimationFrame=0;

 	//removed by Mike, 20210818
	//atexit(cleanup);

	while (1)
	{
		prepareScene();

		doInput();
		
		update();
		
		
		//added by Mike, 20210818
//		draw(player.texture, player.x, player.y);
		//edited by Mike, 20210819
//		draw(texture, myWindowWidthAsPixel/2, myWindowHeightAsPixel/2);
		draw(texture, iPilotX, iPilotY);

		presentScene();

		SDL_Delay(16);
	}

	return 0;
}
*/

int main(int argc, char *argv[])
{
 	//removed by Mike, 20210818
//	memset(&App, 0, sizeof(App));

	initSDL();

	myOpenGLCanvas = new OpenGLCanvas();  
  	myOpenGLCanvas->init(myWindowWidthAsPixel,myWindowHeightAsPixel);
  
  //TO-DO: -add: mouse action
  //glutMouseFunc(myMouseAction);

 	//removed by Mike, 20210818
	//atexit(cleanup);

	while (1)
	{
		prepareScene();

		doInput();
		
		update();
		
		//added by Mike, 20210821
		displayOpenGLCanvas();
		
/* //removed by Mike, 20210821		
		//added by Mike, 20210818
//		draw(player.texture, player.x, player.y);
		//edited by Mike, 20210819
//		draw(texture, myWindowWidthAsPixel/2, myWindowHeightAsPixel/2);
		draw(texture, iPilotX, iPilotY);
*/

		//removed by Mike, 20210822
//		presentScene();

		SDL_Delay(16);
	}

	return 0;
}

/**************************
 * WinMain
 *
 **************************/
/*
 int WINAPI WinMain (HINSTANCE hInstance,
 HINSTANCE hPrevInstance,
 LPSTR lpCmdLine,
 int iCmdShow)
 {
 WNDCLASS wc;
 HWND hWnd;
 HDC hDC;
 HGLRC hRC;
 MSG msg;
 BOOL bQuit = FALSE;
 float theta = 0.0f;
 // register window class
 wc.style = CS_OWNDC;
 wc.lpfnWndProc = WndProc;
 wc.cbClsExtra = 0;
 wc.cbWndExtra = 0;
 wc.hInstance = hInstance;
 wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
 wc.hCursor = LoadCursor (NULL, IDC_ARROW);
 wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
 wc.lpszMenuName = NULL;
 wc.lpszClassName = "GLSample";
 RegisterClass (&wc);
 // create main window    glutKeyboardFunc(keyDown);
 glutKeyboardUpFunc(keyUp);
	//added by Mike, 20200926
 int myWindowWidth=640;
 int myWindowHeight=640;
 hWnd = CreateWindow (
 "GLSample", "Usbong OpenGL Halimbawa",
 WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
 0, 0, myWindowWidth, myWindowHeight,
 NULL, NULL, hInstance, NULL);
 // enable OpenGL for the window
 EnableOpenGL (hWnd, &hDC, &hRC);
 // program main loop
 while (!bQuit)
 {
 // check for messages
 if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
 {
 // handle or dispatch messages
 if (msg.message == WM_QUIT)
 {
 bQuit = TRUE;
 }
 else
 {
 TranslateMessage (&msg);
 DispatchMessage (&msg);
 }
 }
 else
 {
 // OpenGL animation code goes here
 glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
 glClear (GL_COLOR_BUFFER_BIT);
 //added by Mike, 20200926
 // setup OpenGL initial state
 glEnable(GL_CULL_FACE);   // cull back faces
 glEnable(GL_DEPTH_TEST);  // enable Z-buffer
 //glEnable(GL_LIGHTING);    // enable lighting
 glEnable(GL_NORMALIZE);   // renormalize all vertex normals
 
 // enable Gouraud shading
 // (this is default--code appears here only for emphasis)
 glShadeModel(GL_SMOOTH);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 //    		glViewport(0-myWindowWidth/2, 0-myWindowHeight/2, myWindowWidth, myWindowHeight);		// reset the viewport to new dimensions
 //TO-DO: -update: this
 //Reference: https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_Introduction.html;
 //last accessed: 20200927
 // Set to 2D orthographic projection with the specified clipping area
 glMatrixMode(GL_PROJECTION);      // Select the Projection matrix for operation
 glLoadIdentity();                 // Reset Projection matrix
 gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set clipping area's left, right, bottom, top
 glViewport(0, 0, myWindowWidth, myWindowHeight);		// reset the viewport to new dimensions
 // Draws two horizontal lines
 glPushMatrix ();
 glBegin(GL_LINES);
 glColor3f (0.0f, 1.0f, 0.0f);
 //TO-DO: -add: grid
 //note: one (1) line, two (2) vertices
 //X,Y coordinate position from center as reference point
 //TO-DO: -update: this
 //X,Y coordinate position from top-left as reference point
 //			  glVertex2f(0.0f, 0.9f);
 //			  glVertex2f(0.9f, 0.9f);
 glVertex2f(0.0f, 0.5f);
 glVertex2f(0.5f, 0.5f);
 glEnd();
 glPopMatrix ();
 SwapBuffers (hDC);    glutKeyboardFunc(keyDown);
 glutKeyboardUpFunc(keyUp);
 
 //TO-DO: -add: read input file with vertices
 //refer: https://all-things-andy-gavin.com/2020/02/27/war-stories-crash-bandicoot/
 //last accessed: 20200926
 }
 }
 // shutdown OpenGL
 DisableOpenGL (hWnd, hDC, hRC);
 // destroy the window explicitly
 DestroyWindow (hWnd);
 return msg.wParam;
 }
 */

/********************
 * Window Procedure
 *
 ********************/
/*
 LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
 WPARAM wParam, LPARAM lParam)
 {
 switch (message)
 {
 case WM_CREATE:
 return 0;
 case WM_CLOSE:
 PostQuitMessage (0);
 return 0;
 case WM_DESTROY:
 return 0;
 case WM_KEYDOWN:
 switch (wParam)
 {
 case VK_ESCAPE:
 PostQuitMessage(0);
 return 0;
 }
 return 0;
 default:
 return DefWindowProc (hWnd, message, wParam, lParam);
 }
 }
 */

/*******************
 * Enable OpenGL
 *
 *******************/
/*
 void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
 {
 PIXELFORMATDESCRIPTOR pfd;
 int iFormat;
 // get the device context (DC)
 *hDC = GetDC (hWnd);
 // set the pixel format for the DC 
 ZeroMemory (&pfd, sizeof (pfd));
 pfd.nSize = sizeof (pfd);
 pfd.nVersion = 1;
 pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
 PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
 pfd.iPixelType = PFD_TYPE_RGBA;
 pfd.cColorBits = 24;
 pfd.cDepthBits = 16;
 pfd.iLayerType = PFD_MAIN_PLANE;
 iFormat = ChoosePixelFormat (*hDC, &pfd);
 SetPixelFormat (*hDC, iFormat, &pfd);
 // create and enable the render context (RC) 
 *hRC = wglCreateContext( *hDC );
 wglMakeCurrent( *hDC, *hRC );
 }
 */

/******************
 * Disable OpenGL
 *
 ******************/
/*
 void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
 {
 wglMakeCurrent (NULL, NULL);
 wglDeleteContext (hRC);
 ReleaseDC (hWnd, hDC);
 }
 */
