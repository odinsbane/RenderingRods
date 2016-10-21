#!/bin/bash
GLFWDIR="/Users/msmith/.local"

g++ -std=c++11 -I"$GLFWDIR"/include -L"$GLFWDIR"/lib -framework iokit -framework cocoa -framework opengl -framework corevideo -lglfw3 -O2 src/*.cpp -o build/beads
