#!/bin/bash

# Usbong Automotive: Image Processing Automation
# Usbong Computer Aided Design (CAD): 3D Tool 
# Usbong Map System
# Usbong Story
#
# C++ Computer Language and OpenGL
#
# Compile and Link Multiple Source Files
#
# @company: USBONG
# @author: SYSON, MICHAEL B.
# @date created: 20200930
# @last updated: 20210729
# @website address: http://www.usbong.ph
#
# References:
# 1) https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html;
# last accessed: 20200930
# 2) https://cs.lmu.edu/~ray/notes/opengl/
# last accessed: 20210521

cp -r ./textures ./output/
cp -r ./input ./output/

#rm *.o

g++ -c mainLinux.cpp
g++ -c OpenGLCanvas.cpp
g++ -c MyDynamicObject.cpp
g++ -c RobotShip.cpp
g++ -c Font.cpp
g++ -c Beam.cpp
g++ -c Asteroid.cpp
g++ -c Level.cpp
g++ -c PolygonUtils.cpp
g++ -c CTargaImage.cpp
g++ -c Pilot.cpp
g++ -c UsbongUtils.cpp
g++ -c Button.cpp
g++ -c Ball.cpp
g++ -c Text.cpp
g++ -c Level2D.cpp

#g++ -o mainOutput main.o OpenGLCanvas.o -lGL -lGLU -lglut
# edited by Mike, 20210126
# g++ -o ./output/mainOutputLinux mainLinux.o OpenGLCanvas.o MyDynamicObject.o RobotShip.o Font.o Beam.o Asteroid.o Level.o PolygonUtils.o CTargaImage.o Pilot.o UsbongUtils.o -lGL -lGLU -lglut

#Linux Machine
#g++ -o ./output/UsbongVolleyLinux mainLinux.o OpenGLCanvas.o MyDynamicObject.o RobotShip.o Font.o Beam.o Asteroid.o Level.o PolygonUtils.o CTargaImage.o Pilot.o UsbongUtils.o Button.o -lGL -lGLU -lglut

#macOS Machine
g++ -framework OpenGL -framework GLUT -o ./output/UsbongStoryMac mainLinux.o OpenGLCanvas.o MyDynamicObject.o RobotShip.o Font.o Beam.o Asteroid.o Level.o PolygonUtils.o CTargaImage.o Pilot.o UsbongUtils.o Button.o Ball.o Text.o Level2D.o

#added by Mike, 20210220; removed by Mike, 20210521
#cp -r ./*.o ./objectFilesLinux/
