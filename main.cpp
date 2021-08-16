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
 * @date updated: 20210817
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
 *
 * 4) Windows Machine: Dev-C++ Editor
 * 4.1) Verify if already added: Project Options->Parameters->Linker:
 *	"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32/libopengl32.a"
 *	"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32/libglu32.a"
 *	"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32/libglut32.a"
 */

/**************************
 * Includes
 *
 **************************/

#include <windows.h>
//#include <winuser.h>

#include <GL/gl.h>
#include <GL/glut.h> //added by Mike, 20200927
#include <GL/glu.h> //added by Mike, 20200926

//added by Mike, 20210816
#ifdef _WIN32 //Windows machine
	#include <SDL.h>
	#include <SDL_image.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif

//added by Mike, 20200930
#include "OpenGLCanvas.h"

//added by Mike, 20200928
//note: "static" in C/C++ = "final" in java
/*	//edited by Mike, 20210714
static int myWindowWidth=640; //320
static int myWindowHeight=640; //320
*/
int myWindowWidth=640; //900; //640; //320
int myWindowHeight=640; //900; //640; //320

//added by Mike, 20201001
OpenGLCanvas *myOpenGLCanvas = NULL;

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
	//added by Mike, 20210111
	KEY_H,
	//added by Mike, 20210121
	KEY_U,

	iNumOfKeyTypes
};

//added by Mike, 20201001
bool pause;

/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);


//added by Mike, 20200928; edited by Mike, 20200928
//void display() { //Linux Machine
void display(HDC hDC) { //Windows Machine
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
  	
	  //edited by Mike, 20200928 
//   glFlush();  // Render now //Linux Machine
     SwapBuffers (hDC); //Windows Machine
}

//added by Mike, 20200930
//TO-DO: -add: auto-generate set of instructions for Windows Machine using set from Linux Machine
//TO-DO: -add: auto-generate set of instructions for Linux Machine using set from Windows Machine
//void displayOpenGLCanvas() {
//removed by Mike, 20201001
/*
void displayOpenGLCanvas(HDC hDC) {
	 //added by Mike, 20200930
	 OpenGLCanvas *myOpenGLCanvas;   	
	 myOpenGLCanvas = new OpenGLCanvas;
	 myOpenGLCanvas->init();
	 myOpenGLCanvas->render();
	 //myOpenGLCanvas->update(); //TO-DO: -add: this

     SwapBuffers (hDC); //Windows Machine
}
*/
/**************************
 * WinMain
 *
 **************************/

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

    /* register window class */
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

    /* create main window */
	//edited by Mike, 20200926
/*
    hWnd = CreateWindow (
      "GLSample", "OpenGL Sample", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, 256, 256,
      NULL, NULL, hInstance, NULL);
*/

/*	//edited by Mike, 20210714
    hWnd = CreateWindow (
      "GLSample", "Usbong Map System", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, myWindowWidth, myWindowHeight,
      NULL, NULL, hInstance, NULL);
*/

//added by Mike, 20210714
//reference: https://stackoverflow.com/questions/60119473/how-to-get-physical-screen-size-on-windows-i-get-strange-values;
//answer by: Horst Walter, 20200207T1854; edited 20200208T0032

myWindowWidth  = static_cast<int>(GetSystemMetrics(SM_CXSCREEN));
myWindowHeight = static_cast<int>(GetSystemMetrics(SM_CYSCREEN));

/* //edited by Mike, 20210714
    hWnd = CreateWindow (
      "GLSample", "Usbong Map System", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, myWindowWidth, myWindowHeight,
      NULL, NULL, hInstance, NULL);
*/

/*	//edited by Mike, 20210714
    hWnd = CreateWindow (
      "GLSample", "Usbong Map System", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, myWindowWidth, myWindowHeight,
      NULL, NULL, hInstance, NULL);
*/
	//edited by Mike, 20210815
	hWnd = CreateWindow(
		"GLSample", "Usbong Story System", 
      WS_POPUPWINDOW | WS_VISIBLE,
		0, 0, myWindowWidth, myWindowHeight, 
		NULL, NULL, hInstance, NULL); 

    // enable OpenGL for the window
    EnableOpenGL (hWnd, &hDC, &hRC);
    
    //added by Mike, 20210714; removed by Mike, 20210714
    //note: we need to identify the screen's max width and height
    //we use myWindowWidth=static_cast<int>(GetSystemMetrics(SM_CXSCREEN));
	//et cetera
//    ShowWindow (hWnd, SW_MAXIMIZE);


/*
	//added by Mike, 20210714
	//Reference: https://stackoverflow.com/questions/615551/how-to-get-width-and-height-from-createwindowex-window-c;
	//answer by: Brian R. Bondy, 20090505T1632
	RECT rect;	
	//if ShowWindow SW_MAXIMIZE AND WindowRect(...), 
	//has alignment problem
	//TO-DO: -reverify: with macOS machine
//	if(GetWindowRect(hWnd, &rect))
	if(GetClientRect(hWnd, &rect))
	{
	  myWindowWidth = rect.right - rect.left;
	  myWindowHeight = rect.bottom - rect.top;
	}
*/

//CRect rectWorkArea;
/*	
	RECT rectWorkArea;		
	SystemParametersInfo(SPI_GETWORKAREA,0,&rectWorkArea,0);

	myWindowWidth = rectWorkArea.right - rectWorkArea.left;
	myWindowHeight = rectWorkArea.bottom - rectWorkArea.top;
*/

/*
MONITORINFO mi;
mi.cbSize = sizeof(mi);
::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi);
rectWorkArea = mi.rcWork;
*/

/*
	RECT WorkArea, WindowSizeNormal, WindowSizeCurent;
	SystemParametersInfo(SPI_GETWORKAREA,0,&WorkArea,0);
	
// Store the current position for use when normalizing the window   
 	WindowSizeNormal.left = WindowSizeCurent.left;   
 	WindowSizeNormal.top = WindowSizeCurent.top;   
 	WindowSizeNormal.right = WindowSizeCurent.right;   
 	WindowSizeNormal.bottom = WindowSizeCurent.bottom;   
 	 
 	// Set new window size and position as maximized   
 	SetWindowPos( hWnd, NULL, WorkArea.left, WorkArea.top, WorkArea.right, WorkArea.bottom, NULL );  

	myWindowWidth = WorkArea.right - WorkArea.left;
	myWindowHeight = WorkArea.bottom - WorkArea.top;
*/

   RECT desktop;
   // Get a handle to the desktop window
   //edited by Mike, 20210714
   const HWND hDesktop = GetDesktopWindow();
//   hWnd = GetDesktopWindow();

   // Get the size of screen to the variable desktop
   //edited by Mike, 20210714
   GetWindowRect(hDesktop, &desktop);
//   GetWindowRect(hWnd, &desktop);

   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   myWindowWidth = desktop.right;
   myWindowHeight = desktop.bottom;
	
/*	//added by Mike, 20210714
    // enable OpenGL for the window
    EnableOpenGL (hWnd, &hDC, &hRC);

    ShowWindow (hWnd, SW_MAXIMIZE);
*/	
    //added by Mike, 20201001; edited by Mike, 20210323
    myOpenGLCanvas = new OpenGLCanvas();
	
	//edited by Mike, 20210714
    //myOpenGLCanvas->init();
    //TO-DO: -add: myOpenGLCanvas->init(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
    myOpenGLCanvas->init(myWindowWidth, myWindowHeight);

	//TO-DO: -add: mouse actions
	//glutMouseFunc(myMouseAction);
	//glutMotionFunc(myMouseMotionAction);
	//glutIgnoreKeyRepeat(1);


	//added by Mike, 20201001
    //init stuff for delay
    int skip=0, currSysTime=0,
        timeElapsed,
        //edited by Mike, 20201204
        //lower value, faster
        //TO-DO: -update: this
        //edited by Mike, 20210114
//        idealFrameTime=33;//60;//33;
		//edited by Mike, 20210116
//        idealFrameTime=24;//33;
		//TO-DO: -reverify: due to skipping animation sequence
		//with 8192x8192 window size and 80x80grid
        idealFrameTime=30;//36;//50;//16;//33;

    pause=0;

    /* program main loop */
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
        	//added by Mike, 20200928
        	//note: glutDisplayFunc(...) requires zlib1.dll
//   			glutDisplayFunc(display); // Register display callback handler for window re-paint

			//edited by Mike, 20200930
			//display(hDC);
			//removed by Mike, 20201001
//			displayOpenGLCanvas(hDC); //output of this = glutDisplayFunc(displayOpenGLCanvas);

			//edited by Mike, 20201001
/*
            // OpenGL animation code goes here
            myOpenGLCanvas->update();

            //do rendering here 
            myOpenGLCanvas->render();
            SwapBuffers (hDC);
*/            

            if (!pause) {
                currSysTime=GetTickCount();
    
                /* OpenGL animation code goes here */
                myOpenGLCanvas->update();

                //do rendering here 
                myOpenGLCanvas->render();
                SwapBuffers (hDC);
				
//				Sleep(1);				

/*	//edited by Mike, 20210815                     
                if (skip > 0)
                   skip = skip-1;
                else {
                     //do rendering here 
                     myOpenGLCanvas->render();
                     SwapBuffers (hDC);

                     timeElapsed=GetTickCount()-currSysTime;
                     if (timeElapsed>idealFrameTime)
                       skip = (timeElapsed/idealFrameTime) - 1;
                     else 
                       Sleep(idealFrameTime - timeElapsed);
//					Sleep(100000); //1
                }
*/
            }
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

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
        
    //added by Mike, 20210714
    //Reference: https://social.msdn.microsoft.com/Forums/vstudio/en-US/fb4de52d-66d4-44da-907c-0357d6ba894c/swmaximize-is-same-as-fullscreen?forum=vcgeneral;
    //last accessed: 20210714
    //answer by: Espen Ruud Schultz Tuesday, 20090303T0743
 	case WM_GETMINMAXINFO:  
         RECT WorkArea; SystemParametersInfo( SPI_GETWORKAREA, 0, &WorkArea, 0 );  
 
         ( ( MINMAXINFO * )lParam )->ptMaxSize.x = ( WorkArea.right - WorkArea.left );  
         ( ( MINMAXINFO * )lParam )->ptMaxSize.y = ( WorkArea.bottom - WorkArea.top );  
         ( ( MINMAXINFO * )lParam )->ptMaxPosition.x = WorkArea.left;  
         ( ( MINMAXINFO * )lParam )->ptMaxPosition.y = WorkArea.top;  
 
         return 0;  
 
/*
	//added by Mike, 20201014
    case WM_CHAR:
        switch (wParam)
        {
           //added by Mike, 20201013
           //reference: 
           //https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
           //last accessed: 20201013
   	       case 0x41: //A key
		        myOpenGLCanvas->keyDown(KEY_A);
                return 0;
		}
		return 0;
*/
    case WM_KEYDOWN:
        switch (wParam)
        {
	        case VK_ESCAPE:
	            PostQuitMessage(0);
	            return 0;
/*	//removed by Mike, 20210130
	        //added by Mike, 20201001
   	       case VK_LEFT:
		        myOpenGLCanvas->keyDown(KEY_LEFT);
                return 0;
   	       case VK_RIGHT:
		        myOpenGLCanvas->keyDown(KEY_RIGHT);
                return 0;
   	       case VK_UP:
		        myOpenGLCanvas->keyDown(KEY_UP);
                return 0;                
   	       case VK_DOWN:
		        myOpenGLCanvas->keyDown(KEY_DOWN);
                return 0;     
   	       case VK_SPACE:
                myOpenGLCanvas->keyDown(KEY_SPACE);
                return 0;
*/                
           //added by Mike, 20201013
           //reference: 
           //https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
           //last accessed: 20201013
   	       case 0x41: //A key
		        myOpenGLCanvas->keyDown(KEY_A);
                return 0;     
   	       case 0x44: //D key
   	       		//edited by Mike, 20210128
		        myOpenGLCanvas->keyDown(KEY_D);
//		        myOpenGLCanvas->keyDown(KEY_RIGHT);
                return 0;
   	       case 0x57: //W key
//   	       case 0x41: //W key
//			case VK_UP:
				//edited by Mike, 20210128
//		        myOpenGLCanvas->keyDown(KEY_UP);//KEY_W);
		        myOpenGLCanvas->keyDown(KEY_W);
                return 0;
   	       case 0x53: //S key
		        myOpenGLCanvas->keyDown(KEY_S);
//		        myOpenGLCanvas->keyDown(KEY_DOWN);
                return 0;     
   	       case 0x4A: //J key
		        myOpenGLCanvas->keyDown(KEY_J);
                return 0;     
   	       case 0x4C: //L key
		        myOpenGLCanvas->keyDown(KEY_L);
                return 0;     
   	       case 0x49: //I key
		        myOpenGLCanvas->keyDown(KEY_I);
                return 0;     
   	       case 0x4B: //K key
		        myOpenGLCanvas->keyDown(KEY_K);
                return 0;     
		   //added by Mike, 20210111     
   	       case 0x48: //H key
		        myOpenGLCanvas->keyDown(KEY_H);
                return 0;
		   //added by Mike, 20210121
   	       case 0x55: //U key
		        myOpenGLCanvas->keyDown(KEY_U);
                return 0;

			//removed by Mike, 20201001 
/*			               
   	       case 13: //ENTER
                myOpenGLCanvas->keyDown(KEY_ENTER);
                return 0;     
   	       case 80: //P
   	            if (myOpenGLCanvas->currentState!=TITLE_SCREEN) {
			        if (pause==0) //false
			          pause=1; //make it true
                    else pause=0;
                }
                return 0;     	        
*/                
        }
        return 0;
		//added by Mike, 20201001
    case WM_KEYUP:
        switch (wParam)
        {
/*	//removed by Mike, 20210130
       	       case VK_LEFT:
			        myOpenGLCanvas->keyUp(KEY_LEFT);
                    return 0;
       	       case VK_RIGHT:
			        myOpenGLCanvas->keyUp(KEY_RIGHT);
                    return 0;
       	       case VK_UP:
			        myOpenGLCanvas->keyUp(KEY_UP);
                    return 0;
       	       case VK_DOWN:
			        myOpenGLCanvas->keyUp(KEY_DOWN);
                    return 0;                        
       	       case VK_SPACE:
			        myOpenGLCanvas->keyUp(KEY_SPACE);
                    return 0;     
*/
	           //added by Mike, 20201013
	           //reference: 
	           //https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	           //last accessed: 20201013
	   	       case 0x41: //A key
			        myOpenGLCanvas->keyUp(KEY_A);
                    return 0;
	   	       case 0x44: //D key
	   	       		//edited by Mike, 20210128
			        myOpenGLCanvas->keyUp(KEY_D);
//			        myOpenGLCanvas->keyUp(KEY_RIGHT);
	                return 0;     
	   	       case 0x57: //W key
	   	       		//edited by Mike, 20210128
			        myOpenGLCanvas->keyUp(KEY_W);
//			        myOpenGLCanvas->keyUp(KEY_UP);
	                return 0;     
	   	       case 0x53: //S key
			        myOpenGLCanvas->keyUp(KEY_S);
//			        myOpenGLCanvas->keyUp(KEY_DOWN);
	                return 0;     	
	   	       case 0x4A: //J key
			        myOpenGLCanvas->keyUp(KEY_J);
	                return 0;     
	   	       case 0x4C: //L key
			        myOpenGLCanvas->keyUp(KEY_L);
	                return 0;     
	   	       case 0x49: //I key
			        myOpenGLCanvas->keyUp(KEY_I);
	                return 0;     
	   	       case 0x4B: //K key
			        myOpenGLCanvas->keyUp(KEY_K);
	                return 0;
				//added by Mike, 20210111     
	   	       case 0x48: //H key
			        myOpenGLCanvas->keyUp(KEY_H);
	                return 0;
			   //added by Mike, 20210121
	   	       case 0x55: //U key
			        myOpenGLCanvas->keyUp(KEY_U);
	                return 0;
/*	//removed by Mike, 20210130
       	       case 13: //ENTER
                    myOpenGLCanvas->keyUp(KEY_ENTER);
                    return 0;
*/                    
        }
        return 0;
    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}


/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
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

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );

}


/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}

/*	//removed by Mike, 20210817
//added by Mike, 20210816
//Reference: https://sites.google.com/site/sdlgamer/beginner/lesson-3;
//last accessed: 20210816
//TO-DO: -reverify: this; linker input file unused because not done
int main(int argc, char *argv[])
{
	return 0;
}
*/
