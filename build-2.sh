#!/bin/bash

export GLFW_PC="/usr/lib/x86_64-linux-gnu/pkgconfig/glfw3.pc"
export GLFW_CFLAGS=$(pkg-config --cflags "$GLFW_PC")
export GLFW_LIBS=$(pkg-config --static --libs "$GLFW_PC")
export GLAD_HOME=glad

g++ -std=c++20 -I$GLAD_HOME/include $GLFW_CFLAGS -c src/bmfreader.cpp src/camera.cpp src/display.cpp src/meshrepresentation.cpp  src/motorrepresentation.cpp src/planerepresentation.cpp src/reader.cpp src/rod.cpp src/rodrepresentation.cpp src/shadows.cpp src/springrepresentation.cpp src/tiffwriter.cpp $GLAD_HOME/src/glad.c \
-ltiff $GLFW_LIBS

mv *.o ./build/objs

g++ -std=c++20 -I$GLAD_HOME/include $GLFW_CFLAGS -o build/rodviewer src/beads.cpp ./build/objs/*.o -ltiff $GLFW_LIBS
g++ -std=c++20 -I$GLAD_HOME/include $GLFW_CFLAGS -o build/meshviewer src/meshviewer.cpp ./build/objs/*.o -ltiff $GLFW_LIBS
