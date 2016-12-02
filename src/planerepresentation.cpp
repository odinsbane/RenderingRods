#include "display.hpp"
PlaneRepresentation::PlaneRepresentation(){
    positions = new float[12]{
        -5, -5, -1,
        -5, 5, -1,
        5, 5, -1,
        5, -5, -1
    };
    normals = new float[12]{
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
    };
    
    float d = -0.5;
    uvPositions = new float[12]{
        0, 0, d,
        0, 0, d,
        0, 0, d,
        0, 0, d
    };
    indices = new int[6]{
        0, 3, 2, 0, 2, 1
    };
}

void PlaneRepresentation::render(GLuint program){
    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void PlaneRepresentation::setup(GLuint program){
    /*
     *    Setup buffers, can be done without using a program.
     *
     */
    glGenBuffers(1, &posBO);
    glBindBuffer(GL_ARRAY_BUFFER, posBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, positions, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &normBO);
    glBindBuffer(GL_ARRAY_BUFFER, normBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, normals, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &dataBO);
    glBindBuffer(GL_ARRAY_BUFFER, dataBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, uvPositions, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &indexBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6, indices, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    
    /*
     *    Setup attribute array.
     *
     */
    GLuint positionAttribute = 0;
    GLuint normalAttribute = 1;
    GLuint dataAttribute = 2;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, posBO);
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, normBO);
    glEnableVertexAttribArray(normalAttribute);
    glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, dataBO);
    glEnableVertexAttribArray(dataAttribute);
    glVertexAttribPointer(dataAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);
    
    glBindVertexArray(0);
}
