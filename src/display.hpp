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
        ~Shadows(){
            glDeleteProgram(program);
            glDeleteTextures(1, &depthTexture);
            glDeleteFramebuffers(1, &framebufferName);
        }
};

class Representation{
public:
    virtual void render(GLuint program){}
};

class PlaneRepresentation : public Representation{
    //two triangles put together to make a square
    float *positions;
    float *normals;
    float *uvPositions;
    int *indices;
    int pointCount = 3*6; //3 rows of hexagons.
    GLuint posBO, normBO, dataBO, indexBO, VAO;

    public:
    PlaneRepresentation();
    void render(GLuint program);
    void setup(GLuint program);
    ~PlaneRepresentation(){
        
        delete[] positions;
        delete[] normals;
        delete[] uvPositions;
        delete[] indices;
        
        //posBO, normBO, dataBO, indexBO, VAO
        glDeleteBuffers(1, &indexBO);
        glDeleteBuffers(1, &posBO);
        glDeleteBuffers(1, &dataBO);
        glDeleteBuffers(1, &normBO);
        glDeleteVertexArrays(1, &VAO);
    }
    
};

class SpringRepresentation : public Representation{
    float* positions;
    float* normals;
    float* data;
    int* indices;
    GLuint programId;
    GLuint pBO, nBO,dBO, iBO, vao;
    int pointCount=3*6;
    void prepareBufferes();
public:
    double red = 1;
    double green = 1;
    double blue = 0;
    
    SpringRepresentation(glm::vec3 &a, glm::vec3 &b);
    void render(GLuint program);
    void updateRepresentation(glm::vec3 &a, glm::vec3 &b);
    void refreshBuffers();
    ~SpringRepresentation(){
        
        delete[] positions;
        delete[] indices;
        delete[] normals;
        delete[] data;
        
        glDeleteBuffers(1, &iBO);
        glDeleteBuffers(1, &pBO);
        glDeleteBuffers(1, &dBO);
        glDeleteBuffers(1, &nBO);
        glDeleteVertexArrays(1, &vao);
    }

};

class RodRepresentation : public Representation{
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
    double red = 1;
    double green = 0;
    double blue = 0;
    double radius;
	double ave, std, max;
    RodRepresentation(std::vector<glm::vec3*>,double r, GLuint program);
    void refreshBuffers();
    void render(GLuint program);
    void updateData(std::vector<glm::vec3*> &values);
    ~RodRepresentation(){
        
        delete[] points;
        delete[] indexes;
        delete[] normals;
        delete[] data;
        
        glDeleteBuffers(1, &iBO);
        glDeleteBuffers(1, &pBO);
        glDeleteBuffers(1, &dBO);
        glDeleteBuffers(1, &nBO);
        glDeleteVertexArrays(1, &vao);
    }
};

class MotorRepresentation : public Representation{
    RodRepresentation* stalk;
    SpringRepresentation* headSpring;
    SpringRepresentation* tailSpring;
public:
    MotorRepresentation(
                        Motor* motor,
                        glm::vec3 headAttachment,
                        glm::vec3 tailAttachment, GLuint program
                        );
    
    void updateData(
                Motor* motor,
                glm::vec3 headAttachment,
                glm::vec3 tailAttachment );
    void render(GLuint program);
    void refreshBuffers();
    
    ~MotorRepresentation(){
        delete stalk;
        delete headSpring;
        delete tailSpring;
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
    std::mutex mutex;
    std::mutex* starter;
    std::condition_variable* condition;
    std::vector<Representation*> representations;
    
public:
    Display();
    int initialize();
    int render();
    void shutdown();
    void graphicsLoop();
    int getRunning();
    void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mousePressed(GLFWwindow* window, int button, int mod);
    void mouseReleased(GLFWwindow* window, int button, int mod);
    void mouseMoved(GLFWwindow* window, double x, double y);
    void updateLights();
    void addRepresentation(Representation* rod);
    GLuint getProgram(){return program;}
    double RATE = 0.001;
    int running = 0;
    void moveLights(float dx, float dy, float dz);
    ~Display(){
        for(Representation* rep: representations){
            delete rep;
        }
        delete shadows;
        delete camera;
        glDeleteProgram(program);
    }
};





#endif /* defined(__Display__) */
