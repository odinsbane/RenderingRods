//
//  Display.cpp
//
//

#include "display.hpp"
#include "error.h"
#include <stdio.h>

float bg[] = {0,0,0.4};

Display* main_display;
void keyPressedStatic(GLFWwindow* window, int key, int scancode, int action, int mods){
    main_display->keyPressed(window, key, scancode, action, mods);
};

void windowFocusCallback(GLFWwindow* window, int focused)
{
    if (focused)
    {
        printf("gained\n");
        printf("%d ...\n",glfwGetKey(window, GLFW_KEY_A));

    }
    else
    {
        printf("lost\n");
        printf("%d ...\n",glfwGetKey(window, GLFW_KEY_A));

    }
}



void mousePressedStatic(GLFWwindow* window, int button, int action, int mods){
    if(action==GLFW_PRESS) {
        main_display->mousePressed(window, button, mods);
    } else{
        main_display->mouseReleased(window, button, mods);
    }
}

void mouseMovedStatic(GLFWwindow* window, double x, double y){
    main_display->mouseMoved(window, x, y);
}

Display::Display(){
    main_display=this;
}

int Display::initialize(){
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &width, &height);

#ifndef __APPLE__
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        glfwTerminate();
        printf("Failed to initialize OpenGL context\n");
        return 0;
    }
#endif

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glDepthFunc(GL_LEQUAL);
    //glDepthMask(GL_TRUE);
    //glEnable(GL_BLEND);
    //glDepthRange(0.0f, 1.0f);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    printf("GLSL version %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("GL Version: %s\n", glGetString(GL_VERSION));


    /*
     * Create the program.
     */
    std::ifstream vertexFile("shaders/world.vert");
    long start = vertexFile.tellg();
    vertexFile.seekg(0, std::ios::end);
    long end = vertexFile.tellg();
    char* vertexCStr = new char[1+end-start];
    vertexCStr[end-start] = 0;
    vertexFile.seekg(0, std::ios::beg);
    vertexFile.read(vertexCStr, end-start);
    //const char* vertexCStr = vertexStr.c_str();
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );

    const char* vcs(vertexCStr);
    glShaderSource(vertexShader, 1, &vcs, NULL);
    glCompileShader( vertexShader );

    if(!shaderStatus(vertexShader)){
        exit(1);
    }


    printf("made a window\n");

    std::ifstream fragFile;
    fragFile.open("shaders/color.frag");
    start = fragFile.tellg();
    fragFile.seekg(0, std::ios::end);
    end = fragFile.tellg();
    char* fragCStr = new char[1+end-start];
    fragCStr[end-start] = 0;

    fragFile.seekg(0, std::ios::beg);
    fragFile.read(fragCStr, end-start);
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

    //gcc's strange complaint.
    const char* fcs(fragCStr);
    glShaderSource(fragmentShader, 1, &fcs, NULL);
    glCompileShader( fragmentShader );


    if(!shaderStatus(fragmentShader)){
        exit(1);
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    if(!programStatus(program)){
        exit(1);
    }

    delete[] vertexCStr;
    vertexFile.close();
    delete[] fragCStr;
    fragFile.close();

    GetError();

    camera = new Camera(program);
    
    glfwSetKeyCallback(window, keyPressedStatic);
    glfwSetMouseButtonCallback(window, mousePressedStatic);
    glfwSetCursorPosCallback(window, mouseMovedStatic);
    glfwSetWindowFocusCallback(window, windowFocusCallback);
		
	
	shadows = new Shadows(camera);
	shadows->buildProgram();
    if(shadows->setTarget()){
        printf("texture buffer thing is cool!\n");
    } else{
        printf("failed to buffer\n");
    }
    
    return 0;
    
}

int Display::render(){
    std::lock_guard<std::mutex> lock(mutex);
    
    GLuint depthBuffer = shadows->getDepthBuffer();
    GLuint shadowProgram = shadows->getProgram();
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);
    
    glViewport(0, 0, 1024, 1024);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    //glFrontFace(GL_CCW);
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(Representation* rep: representations){
        rep->render(shadowProgram);
    }
    
    glUseProgram(0);
    
    
    
    
    // Render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,width,height);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);

		
    glClearColor(bg[0], bg[1], bg[2], 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
    GLuint ShadowMapID = glGetUniformLocation(program, "shadowMap");
	glActiveTexture(GL_TEXTURE0);
	GLuint depthTexture = shadows->getTextureID();
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glUniform1i(ShadowMapID, 0);
	
    
    for(Representation* rep: representations){
        rep->render(program);
    }
    
	GetError();
    
    glfwSwapBuffers(window);

    glfwPollEvents();
    if(recording){
		recordFrame();
	}
    return running;

}

void Display::shutdown(){
    glfwTerminate();
}

void Display::graphicsLoop(){
    while(running==0){
        render();
    }
    
    
}



int Display::getRunning(){
    return running;
}


void Display::keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods){
    //printf("%d, %d, %d, %d \n", key, scancode, action, mods);
    if(action!=0){
        switch(key){
            case GLFW_KEY_LEFT:
                //camera->rotate(-0.01f, 0);
                camera->pan(0.01, 0);
                break;
            case GLFW_KEY_RIGHT:
                camera->pan(-0.01, 0);
                //camera->rotate(0.01f,0);
                break;
            case GLFW_KEY_UP:
                //camera->rotate(0, 0.01f);
                camera->pan(0, -0.01);
                break;
            case GLFW_KEY_DOWN:
                camera->pan(0, 0.01f);
                break;
            case GLFW_KEY_Z:
                camera->zoom(0.05f);
                break;
            case GLFW_KEY_A:
                //exit(-1);
                break;
            case GLFW_KEY_Q:
                //printf("%d ...\n",glfwGetKey(window, GLFW_KEY_A));
                camera->zoom(-0.05f);
                break;
            case GLFW_KEY_ESCAPE:
                running=-1;
                break;
            case GLFW_KEY_ENTER:
                break;
            case GLFW_KEY_G:
                moveLights(-0.1f, 0.f, 0.f);
                break;
            case GLFW_KEY_H:
                moveLights(0.1f, 0.f, 0.f);
                break;
            case GLFW_KEY_Y:
                moveLights(0.0f, 0.1f, 0.f);
                break;
            case GLFW_KEY_B:
                moveLights(0.0f, -0.1f, 0.f);
                break;
            case GLFW_KEY_N:
                moveLights(0.f, 0.f, -0.1f);
                break;
            case GLFW_KEY_J:
                moveLights(0.f, 0.f, 0.1f);
                break;
            case GLFW_KEY_R:
                startRecording();
                break;
        }
    }
}



/**
 * @brief shaderStatus
 * For checking if the shader compiled and printing any error messages.
 *
 * @param shader a shader that was compiled or attempted.
 * @return true if the shader didn't fail.
 */
bool shaderStatus(GLuint &shader){
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        const char *strShaderType = "shader";

        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
        return false;
    }


    return true;
}

/**
 * @brief programStatus
 *
 * Checks if the program linked ok.
 *
 * @param program
 * @return
 */
bool programStatus(GLuint &program){
    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if(status==GL_FALSE){
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        return false;
    }
    return true;
}


void Display::mousePressed(GLFWwindow *window, int button, int mod) {
    if(button==GLFW_MOUSE_BUTTON_LEFT){
        dragging=true;
        glfwGetCursorPos(window, &cursor_x, &cursor_y);
    }
}

void Display::mouseReleased(GLFWwindow *window, int button, int mod) {
    if(button==GLFW_MOUSE_BUTTON_LEFT){
        dragging=false;
    }
}

void Display::mouseMoved(GLFWwindow *window, double x, double y){
    if(dragging){
        double delta_x = x - cursor_x;
        double delta_y = y - cursor_y;
		camera->rotate((float)(delta_x*RATE), (float)(delta_y*RATE));
		cursor_x = x;
		cursor_y = y;
    }
}

void Display::moveLights(float dx, float dy, float dz) {
    shadows->setProjectViewMatrix();
    camera->moveLight(dx, dy, dz);

}

void Display::addRepresentation(Representation *rod){
	representations.push_back(rod);
}

void Display::startRecording(){
	if(recording) return;
	recording = true;
	writer = new TiffWriter("testing.tif", height, width);
}

void Display::recordFrame(){
	if( !recording ) return;
	
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, writer->getPixbuf());
	writer->writeFrame();
	if(writer->getCount() > 200){
		finishRecording();
	}
}

void Display::finishRecording(){
	if( !recording ) return;
	writer->close();
	recording = false;
	delete writer;
	writer = 0;
}
