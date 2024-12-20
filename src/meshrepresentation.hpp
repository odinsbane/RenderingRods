#include "display.hpp"

#ifndef __MeshRepresentation__
#define __MeshRepresentation__

class MeshRepresentation : public Representation{
    float* positions;
    float* normals;
    float* data;
    int* indices;
    int triangleCount;
    int pointCount;
    GLuint programId;
    GLuint pBO, nBO,dBO, iBO, vao;
    void prepareBufferes();
public:

    MeshRepresentation(int positions, int triangles);
    void render(GLuint program);
    void refreshBuffers();
    void solidColor(float r, float g, float b);
    void updateTriangleIndices( int* triangles );
    void updatePositions(double* next_positions);
    void calculateNormals();
    ~MeshRepresentation(){

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





#endif
