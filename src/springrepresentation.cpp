#include "display.hpp"

SpringRepresentation::SpringRepresentation(glm::vec3 &a, glm::vec3 &b){
    //3x6 points with 3 values a piece
    positions = new float[pointCount*3];
    data = new float[pointCount*3];
    normals = new float[pointCount*3];
    //2x6 quads with 6 indices a piece
    indices = new int[2*6*6];
    int c = 0;
    //N = rod->getPoints()->size();
    for(int i = 0; i<2; i++){
        for(int j = 0; j<6; j++){
            indices[c++] = 6*i + (j+1)%6;
            indices[c++] = 6*i + j;
            indices[c++] = 6*(i+1) + j;
            indices[c++] = 6*(i+1) + j;
            indices[c++] = 6*(i+1) + (j+1)%6;
            indices[c++] = 6*(i) + (j+1)%6;
        }
    }
    
    updateRepresentation(a, b);
    prepareBufferes();
}


void SpringRepresentation::updateRepresentation(glm::vec3 &ptA, glm::vec3 &ptB) {
    float radius = 0.02;
    glm::vec3 r = ptB - ptA;
    glm::vec3 forward = glm::normalize(r);
    //find an up.
    glm::vec3 a = glm::vec3(0, 0, 1) - forward.z*forward;
    if(a.length()==0){
        a = glm::vec3(0, 1, 0) - forward.y*forward;
    }
    a = glm::normalize(a);
    glm::vec3 b = glm::cross(a, forward);
    
    for(int i = 0; i<3; i++){
        
        glm::vec3 middle = ptA + r*(0.5f*i);
        float rad = 1 + 3.0f/4.0f*(i)*(i - 2);
        for(int j = 0; j<6; j++){
            double theta = (j)*M_PI/3.0;
            double c = cos(theta);
            double s = sin(theta);
            
            glm::vec3 proja(a.x*c, a.y*c, a.z*c);
            glm::vec3 projb(b.x*s, b.y*s, b.z*s);
            
            glm::vec3 loc = rad*radius*proja + rad*radius*projb + middle;
            glm::vec3 norm = proja + projb;
            if(rad<1){
                norm = glm::normalize(norm + r);
            }
            int dex = j*3 + (i)*18;
            positions[dex + 0] = loc.x;
            positions[dex + 1] = loc.y;
            positions[dex + 2] = loc.z;
            
            normals[dex + 0] = norm.x;
            normals[dex + 1] = norm.y;
            normals[dex + 2] = norm.z;
            
            data[dex + 0] = 0.f;
            data[dex + 1] = 0.f;
            data[dex + 2] = 1.f;
        }
    }


}

void SpringRepresentation::prepareBufferes(){
    /*
     *    Setup buffers, can be done without using a program.
     *
     */
    glGenBuffers(1, &pBO);
    glBindBuffer(GL_ARRAY_BUFFER, pBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pointCount*3, positions, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &nBO);
    glBindBuffer(GL_ARRAY_BUFFER, nBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pointCount*3, normals, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &dBO);
    glBindBuffer(GL_ARRAY_BUFFER, dBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pointCount*3, data, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &iBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*2*6*6, indices, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    
    /*
     *    Setup attribute array.
     *
     */
    GLuint positionAttribute = 0;
    GLuint normalAttribute = 1;
    GLuint dataAttribute = 2;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, pBO);
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, nBO);
    glEnableVertexAttribArray(normalAttribute);
    glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, dBO);
    glEnableVertexAttribArray(dataAttribute);
    glVertexAttribPointer(dataAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBO);
    
    glBindVertexArray(0);
    
}

void SpringRepresentation::render(GLuint program){
    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 12*2*3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

