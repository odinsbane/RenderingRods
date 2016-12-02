#include "display.hpp"

Shadows::Shadows(Camera* cam){
	camera = cam;
}

bool Shadows::buildProgram(){
	std::ifstream vertexFile("shaders/shadows.vert");
	long start = vertexFile.tellg();
    vertexFile.seekg(0, std::ios::end);
    long end = vertexFile.tellg();
    char* vertexCStr = new char[1+end-start];
    vertexCStr[end-start] = 0;
    vertexFile.seekg(0, std::ios::beg);
    vertexFile.read(vertexCStr, end-start);

    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );

    const char* vcs(vertexCStr);
    glShaderSource(vertexShader, 1, &vcs, NULL);
    glCompileShader( vertexShader );
	GetError();
    if(!shaderStatus(vertexShader)){
        exit(1);
    }

	
    std::ifstream fragFile;
    fragFile.open("shaders/shadows.frag");
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
    
    return true;

}

void Shadows::setProjectViewMatrix(){
	glm::vec3 light = camera->getLightPosition();
	glm::vec3 location = camera->getTarget();
	    
	glm::vec3 lightInvDir = light - location;
    
    GLuint depthMatrixID = glGetUniformLocation(program, "depthMVP");
	
	// Compute the MVP matrix from the light's point of view
 	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-5.f,5.f,-5.f,5.f,-1.f,5.f);
 	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
 	glm::mat4 depthModelMatrix = glm::mat4(1.0);
 	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
    
 	// Send our transformation to the currently bound shader,
 	// in the "MVP" uniform
    glUseProgram(program);
    glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
    glUseProgram(0);
    glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	
	camera->setDepthMVP(biasMatrix*depthMVP);
}

GLuint Shadows::getDepthBuffer(){
		return framebufferName;
}

bool Shadows::setTarget(){
     // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	 framebufferName = 0;
	 glGenFramebuffers(1, &framebufferName);
	 glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
	 printf("texture buffer @ %d\n", framebufferName);
	 // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	 
	 glGenTextures(1, &depthTexture);
	 glBindTexture(GL_TEXTURE_2D, depthTexture);
     glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	 glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	 
	 glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	 // Always check that our framebuffer is ok
	 if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	setProjectViewMatrix();
	return true;
}

GLuint Shadows::getTextureID(){
	return depthTexture;
}
