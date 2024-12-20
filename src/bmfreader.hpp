#ifndef BINARYMESHREADER
#define BINARYMESHREADER

#include <stdio.h>
#include <fstream>
#include <vector>

class BinaryMesh{
    public:
        double* positions;
        int* triangle_indices;
        int* connection_indices;
        int triangle_count, connection_count, position_count;
        ~BinaryMesh(){
            delete[] positions;
            delete[] triangle_indices;
            delete[] connection_indices;
        }
};

class Track{
    std::vector<BinaryMesh*> meshes;
    std::vector<int> frames;
    char* name;
    int first = 100000;
    int last = -1;
public:
    Track(char* name);
    void addMesh( int frame, BinaryMesh* mesh);
    int getFirstFrame(){ return first;}
    int getLastFrame(){ return last;}

    BinaryMesh* getMesh( int frame );
    ~Track(){
        for(BinaryMesh* mesh : meshes){
            if( mesh != NULL ){
                delete mesh;
            }
        }
        delete[] name;
    }

};

class BinaryMeshReader{

    std::vector<Track*> tracks;
    public:
        BinaryMeshReader(const std::string &filename);
        std::vector<Track*> getMeshes(){ return tracks;}
};









#endif
