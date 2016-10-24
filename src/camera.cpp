#include "display.hpp"


Camera::Camera(GLuint &program){
    theProgram = program;
    aspect = 1.0f;
    
	location[0] = 0;
	location[1] = 0;
	location[2] = 0;
	
	position[0] = 0;
	position[1] = 0;
	position[2] = 5;
	
	up[0] = 0;
	up[1] = 1;
	up[2] = 0;
	
	light_position = glm::vec3(-0.0,-0.0,5.0);
    setViewMatrices();
	
    printf("initializing lights\n");    

    GetError();
    updateLights();
    
}
    
void Camera::zoom(double dr){
    glm::vec3 dx = position - location;
    float length = glm::length(dx);
    if(length + dr>0){
    	length = (length + dr)/length;
    } else{
    	length = 0.5;
    }
    position = location + dx*length;
    setViewMatrices();
}


void Camera::setViewMatrices(){
    float fFrustumScale = 1.0f; 
    float fzNear = 0.5f;
    
    float fzFar = 30.0f;
    
		
    GLuint perspectiveMatrixUnif = glGetUniformLocation(theProgram, "perspectiveMatrix");
	GLuint orientationMatrixUnif = glGetUniformLocation(theProgram, "orientationMatrix");
    
    perspectiveMatrix = glm::perspective(90.0f, aspect, fzNear, fzFar);
    orientationMatrix = glm::lookAt( position, location, up );
    
    perspectiveMatrix = perspectiveMatrix*orientationMatrix;
	
    
    glUseProgram(theProgram);
    
    glUniformMatrix4fv(orientationMatrixUnif, 1, GL_FALSE, &orientationMatrix[0][0]);
    glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, &perspectiveMatrix[0][0]);
    
    glUseProgram(0);
    
}

void Camera::setDepthMVP(glm::mat4 depthMVP){
	
	GLuint id = glGetUniformLocation(theProgram, "depthBiasMatrix");
	
	printf("depth matrix\n");
	for(int i = 0; i<4; i++){
		for(int j = 0; j<4; j++){
			printf("%f\t", depthMVP[i][j]);
		}
		printf("\n");
	}
	glUseProgram(theProgram);
	glUniformMatrix4fv(id, 1, GL_FALSE, &depthMVP[0][0]);
    glUseProgram(0);
}

bool isNaN(double d){
	if(d>0||d<=0) return false;
	return true;
}
bool anyNaN(glm::vec4 vec){
	return isNaN(vec[0])||isNaN(vec[1])||isNaN(vec[2])||isNaN(vec[3]);
}
void Camera::rotate(float dtheta, float dphi){
	
	
	glm::vec4 nloc = glm::inverse(orientationMatrix)*glm::vec4(dphi, dtheta, 0.0f, 0.0f);
	float angle = glm::length(nloc)*5;
		
	nloc = glm::normalize(nloc);
	
	if(anyNaN(nloc)){
		//angle is probably zero.
		return;
	}

	
	glm::vec3 old = position - location;
	
	float l = glm::length(old);
	old = glm::normalize(old);
	
	glm::mat4 rotater = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(nloc));
	glm::vec3 dir = glm::vec3(rotater*glm::vec4(old, 0.0f));
	up = glm::vec3(rotater*glm::vec4(up, 0.0f));
	
	//make sure it is orthogonal and 1.
	up = glm::normalize(up - dir*glm::dot(dir, up));
	
	position = location + dir*l;
    setViewMatrices();
}

void Camera::pan(float dx, float dy){
	glm::vec3 nloc = glm::vec3(glm::inverse(orientationMatrix)*glm::vec4(-dx, -dy, 0.0f, 0.0f));
	
	location = nloc + location;
	position = nloc + position;
    setViewMatrices();
}

/*
 * Changes the display aspect ratio, such that more of the image is shown in a 
 * particular direction.
 *  
 */
void Camera::resizeWindow(float w, float h){
    aspect = w/h;
    
}

void Camera::updateLights(){

    GLuint lightPositionUniform = glGetUniformLocation(theProgram, "lightPos");
    
    glUseProgram(theProgram);    
    glUniform3fv(lightPositionUniform, 1, &light_position[0]);
    //printf("pos: %d int: %d amb: %d\n", lightPositionUniform, lightIntensityUniform, ambientIntensityUniform);
    
    glUseProgram(0);
}

void Camera::moveLight(float dx, float dy, float dz) {
    light_position[0] += dx;
    light_position[1] += dy;
    light_position[2] += dz;
    updateLights();
}
