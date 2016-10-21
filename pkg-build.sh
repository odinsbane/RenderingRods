#!/bin/bash

export GLFW_PC="$GLFW3_HOME"/lib/pkgconfig/glfw3.pc
export GLFW_CFLAGS=$(pkg-config --cflags "$GLFW_PC")
export GLFW_LIBS=$(pkg-config --static --libs "$GLFW_PC")
export GLAD_HOME=glad

g++ -std=c++11 -I$GLAD_HOME/include $GLFW_CFLAGS src/*.cpp $GLAD_HOME/src/glad.c \
$GLFW_LIBS -o build/beads

cd ..
