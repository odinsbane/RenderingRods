#include "meshrepresentation.hpp"

MeshRepresentation::MeshRepresentation(int points, int triangles){
    pointCount = points;
    triangleCount = triangles;

    positions = new float[points*3];
    data = new float[points*3];
    normals = new float[points*3];
    indices = new int[triangles*3];

    prepareBufferes();
}

void MeshRepresentation::solidColor( float r, float g, float b){
    for(int i = 0; i<pointCount; i++){
       data[3*i] = r;
       data[3*i + 1] = g;
       data[3*i + 2] = b;
    }
}

void MeshRepresentation::updateTriangleIndices( int* triangles ){
    for(int i = 0; i<triangleCount*3; i++){
        indices[i] = triangles[i];
    }

}

void MeshRepresentation::updatePositions(double* next_positions) {
    for(int i = 0; i<pointCount; i++){
        positions[3*i] = (float)( 4 * (next_positions[3*i]) );
        positions[3*i + 1] = (float)( 4 * (next_positions[3*i + 1]) );
        positions[3*i + 2] = (float)( 4 * (0.0625 + next_positions[3*i + 2]) );
    }
}

void MeshRepresentation::calculateNormals(){
    int* counts = new int[pointCount];
    float* normal = new float[3];
    float* a = new float[3];
    float* b = new float[3];

    for( int i = 0; i<pointCount; i++){
        counts[i] = 0;
    }

    for( int i = 0; i<pointCount*3; i++){
        normals[i] = 0;
    }
    for( int dex = 0; dex<triangleCount; dex++){
        int ai = indices[3*dex];
        int bi = indices[3*dex + 1];
        int ci = indices[3*dex + 2];

        for(int i = 0; i<3;i++){
           a[i] = positions[3*bi + i] - positions[3*ai + i];
           b[i] = positions[3*ci + i] - positions[3*ai + i];
        }
        normal[0] = (a[1]*b[2] - a[2]*b[1]);
        normal[1] = (a[2]*b[0] - a[0]*b[2]);
        normal[2] = (a[0]*b[1] - a[1]*b[0]);
        double mag = sqrt( normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2] );

        if( mag > 0){
            normal[0] = normal[0]/mag;
            normal[1] = normal[1]/mag;
            normal[2] = normal[2]/mag;
        }

        normals[3*ai] += normal[0];
        normals[3*ai + 1] += normal[1];
        normals[3*ai + 2] += normal[2];

        normals[3*bi] += normal[0];
        normals[3*bi + 1] += normal[1];
        normals[3*bi + 2] += normal[2];

        normals[3*ci] += normal[0];
        normals[3*ci + 1] += normal[1];
        normals[3*ci + 2] += normal[2];

        counts[ai] += 1;
        counts[bi] += 1;
        counts[ci] += 1;

    }

    for(int i = 0; i<pointCount; i++){
        int c = counts[i];
        double n0 = normals[3*i]/c;
        double n1 = normals[3*i + 1]/c;
        double n2 = normals[3*i + 2]/c;
        double a = sqrt(n0*n0 + n1*n1 + n2*n2);
        if(a > 0){
            normals[ 3*i ] = n0/a;
            normals[ 3*i + 1 ] = n1/a;
            normals[ 3*i + 2 ] = n2/a;
        }
    }


    delete[] a;
    delete[] b;
    delete[] counts;
    delete[] normal;
}

void MeshRepresentation::refreshBuffers(){
    glBindBuffer(GL_ARRAY_BUFFER, pBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pointCount*3, positions, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, nBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pointCount*3, normals, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, dBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pointCount*3, data, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*3*triangleCount, indices, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void MeshRepresentation::prepareBufferes(){
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*3*triangleCount, indices, GL_STREAM_DRAW);
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

void MeshRepresentation::render(GLuint program){
    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, triangleCount*3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
