# Usbong Automotive: Story
## Execution Notes
## Windows Machine
--> verified: with Windows 7 machine<br/>
--> <b>system:</b> 64-bit Operating System (OS)<br/>
--> <b>reminder:</b> with 32-bit OS execution speed noticeably slow<br/>
### Steps
1) Download the following libraries, i.e. sets of reusable instructions<br/>
--> 1.1) glut32 (.dll format)<br/><br/>
TO-DO: -update: this<br/><br/>
2) Put the .dll file in the same directory as the output .exe (executable) file<br/> 
<br/>
Done! The .exe file should now execute.
  
## Notes:  
TO-DO: -reverify: continuous usage of Simple Directmedia Layer (SDL)<br/>
<br/>
--> observed: still used with https://github.com/OpenRA/OpenRA; last accessed: 20210817<br/>
--> "Open Source real-time strategy game engine for early Westwood games such as Command & Conquer: Red Alert written in C# using SDL and OpenGL. Runs on Windows, Linux, \*BSD and Mac OS X."<br/>
--> <b>reminder:</b> from history, we learn the following:<br/>
<br/>
<b>"Command & Conquer: Red Alert" (Year 1996):</b> execution speed noticeably fast<br/> 
--> even with Windows XP machines (32-bit OS);<br/>
<br/>
<b>"Command & Conquer: Tiberian Sun" (Year 1999):</b> execution speed became noticeably slow;<br/>
--> from SDL to Non-SDL?<br/>
--> observed: NON-SDL and OpenGL NOT anymore noticeably slow with 64-bit Operating Systems<br/>

### Additional Notes (SDL)
--> verified: with Windows 7 machine<br/>
--> system: 32-bit Operating System (OS)<br/>

1) Download the following libraries, i.e. sets of reusable instructions<br/>
--> 1.1) Simple Directmedia Layer (SDL): Runtime Binaries (.dll format)<br/>
--> https://www.libsdl.org/download-2.0.php; last accessed: 20210817<br/>
--> <b>Example:</b> SDL2-2.0.16-win32-x86 (32-bit Windows)<br/><br/>
--> 1.2) Simple Directmedia Layer (SDL) Image: Runtime Binaries (.dll format)<br/>
--> https://www.libsdl.org/projects/SDL_image/; last accessed: 20210817<br/>
--> <b>Example:</b> SDL2_image-2.0.5-win32-x86.zip (32-bit Windows)<br/><br/>
--> 1.3) gdi32 (.dll format)<br/><br/>
TO-DO: -update: this<br/><br/>

2) Put the .dll files in the same directory as the output .exe (executable) file<br/> 
--> 2.1) Simple Directmedia Layer (SDL): Runtime Binaries (.dll format)<br/>
--> <b>location:</b> "SDL2<version>"<br/><br/>
--> 2.2) Simple Directmedia Layer (SDL) Image: Runtime Binaries (.dll format)<br/>
--> <b>location:</b> "SDL2_image-<version>/lib/x86"<br/>
<br/>
Done! The .exe file should now execute.
  
  

