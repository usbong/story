#!/bin/bash

# Usbong Automotive: Image Processing Automation
# Usbong Computer Aided Design (CAD): 3D Tool
# Usbong Story System
#
# C++ Computer Language and OpenGL
#
# Compile and Link Multiple Source Files
#
# @company: USBONG
# @author: SYSON, MICHAEL B.
# @date created: 20200930
# @last updated: 20210823
# @website address: http://www.usbong.ph
#
# Reference:
# 1) https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html;
# last accessed: 20200930
#
# Additional Notes:
# 1) SDL Installation: Download .dmg files; Extract to /Library/Frameworks
# --> https://www.libsdl.org/download-2.0.php; last accessed: 20210823
# --> https://www.libsdl.org/projects/SDL_image/; last accessed: 20210823
#

g++ -c mainLinuxHalimbawa.cpp -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers -F/Library/Frameworks

#linux Machine
#g++ -o ./output/UsbongHalimbawaLinux mainLinuxHalimbawa.o -lGL -lGLU -lglut -lSDL2 -lSDL2_image

#macOS Machine
g++ -framework SDL2 -F/Library/Frameworks -framework SDL2_image -framework OpenGL -framework GLUT -o ./output/UsbongHalimbawaMac mainLinuxHalimbawa.o
