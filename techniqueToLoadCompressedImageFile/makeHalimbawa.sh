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
# @last updated: 20210816
# @website address: http://www.usbong.ph
#
# Reference:
# 1) https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html;
# last accessed: 20200930


g++ -c mainLinuxHalimbawa.cpp

g++ -o ./output/UsbongHalimbawaLinux mainLinuxHalimbawa.o -lGL -lGLU -lglut -lSDL2 -lSDL2_image

