//
//  Display.h
//  ParallelBalls
//
//  Created by msmith on 9/8/14.
//  Copyright (c) 2014 paluchlab. All rights reserved.
//
#ifndef __APPLE__
  #include "glad/glad.h"
#else
  #define GLFW_INCLUDE_GLCOREARB
  #define GLFW_NO_GLU
#endif

#include "GLFW/glfw3.h"
#include <fstream>
#ifndef __Display__
#define __Display__

#include <iostream>
#include <ctime>
#include <string>
#include <thread>
#include <condition_variable>
#include <vector>
#include "beads.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "rod.hpp"
#include "error.h"

bool shaderStatus(GLuint &shader);
bool programStatus(GLuint &program);


class Camera {

    GLuint theProgram;
    glm::vec3 position;
    glm::vec3 location;
    glm::vec3 up;
    glm::mat4 orientationMatrix;
    glm::mat4 perspectiveMatrix;
	float aspect;
	glm::vec3 light_position;
    public:

        Camera(GLuint &program);
        void resizeWindow(float w, float h);
        void rotate(float dtheta, float dphi);
        void setViewMatrices();
        void zoom(double dr);
        void pan(float dx, float dy);
        void updateLights();
        void moveLight(float dx, float dy, float dz);
        glm::vec3 getLightPosition(){return light_position;}
		glm::vec3 getTarget(){return location;}
	    void setDepthMVP(glm::mat4 depthMVP);

};

class Shadows{
    GLuint program, depthTexture, framebufferName;
    Camera* camera;
    
	public:
	    Shadows(Camera* cam);
	    GLuint getDepthBuffer();
	    GLuint getProgram(){return program;}
	    bool setTarget();
		bool buildProgram();
        void setProjectViewMatrix();
		GLuint getTextureID();
};

class Representation{
public:
    virtual void render(GLuint program){}
};

class RodRepresentation : public Representation{
    Rod* rod;
    float* points;
    float* normals;
    float* data;
    int* indexes;
	int N;
	GLuint programId;
    GLuint pBO, nBO,dBO, iBO, vao;
    void calculateData(std::vector<glm::vec3*> &values);
    void prepareBuffers();
 public:
	double ave, std, max;
    RodRepresentation(Rod* rod, GLuint program);
    
    void render(GLuint program);
    void updateData(std::vector<glm::vec3*> &values);
    ~RodRepresentation(){
        delete[] points;
        delete[] indexes;
        glDeleteBuffers(1, &iBO);
        glDeleteBuffers(1, &pBO);
        glDeleteBuffers(1, &dBO);
        glDeleteVertexArrays(1, &vao);
    }
};

class Display{
private:
    GLFWwindow* window;

    GLuint program;

    bool dragging = false;
    double cursor_x, cursor_y;
	int rods = 0;
    int height = 800;
    int width = 800;
    int last = 200;
    Camera* camera;
    Shadows* shadows;
    int running = 0;
    std::mutex mutex;
    std::mutex* starter;
    std::condition_variable* condition;
    std::vector<Representation*> representations;
    double ave, std, max;
    void updateColorRange();
    
public:
    Display();
    int initialize();
    int render();
    void shutdown();
    void graphicsLoop();
    int getRunning();
    ~Display(){
    }
    void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mousePressed(GLFWwindow* window, int button, int mod);
    void mouseReleased(GLFWwindow* window, int button, int mod);
    void mouseMoved(GLFWwindow* window, double x, double y);
    void updateLights();
    void addRepresentation(Representation* rod);
    void addRodRepresentation(RodRepresentation* rod);
    GLuint getProgram(){return program;}
    double RATE = 0.001;

    void moveLights(float dx, float dy, float dz);
};





#endif /* defined(__Display__) */
