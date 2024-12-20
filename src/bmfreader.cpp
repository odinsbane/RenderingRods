#include "bmfreader.hpp"

/*
 * test.bmf
 * frame: 0 with 2274 position indices with 4536 connection indices and 4536 triangle indices
 * */

int nextInt(std::ifstream* stream);
double nextDouble(std::ifstream* stream);
char* readName(std::ifstream* stream);
int getInt(char* integer);

BinaryMeshReader::BinaryMeshReader(const std::string &filename){
    
    std::ifstream* inputStream = new std::ifstream(filename, std::ifstream::binary);
    int version = nextInt(inputStream);
    int count = nextInt(inputStream);
    printf("mesh verion %d. %d mesh-tracks\n",version, count);

    for(int i = 0; i<count; i++){
        char* name = readName(inputStream);

        Track* track = new Track(name);
//        printf("loading track: %s\n", name);

        int frames = nextInt(inputStream);

        for(int j = 0; j<frames; j++){
            int frame = nextInt(inputStream);

//           printf("loading %d meshes for frame %d \n",frames, frame);

            int p_count = nextInt(inputStream);

            printf("with %d position doubles\n", p_count);
            double* positions = new double[p_count];

            for(int j = 0; j<p_count; j++){
                positions[j] = nextDouble(inputStream);
            }

            int c_count = nextInt(inputStream);
            int* connections = new int[c_count];

            for(int j = 0; j<c_count; j++){
                connections[j] = nextInt(inputStream);
            }


            int t_count = nextInt(inputStream);
            int* triangles = new int[t_count];
            for(int j = 0; j<t_count; j++){
                triangles[j] = nextInt(inputStream);
            }

//           printf("frame %d: %d position doubles, %d connection indices, %d triangle indices.\n",frame, p_count, c_count, t_count);

            BinaryMesh* mesh = new BinaryMesh( );
            mesh->triangle_count = t_count/3;
            mesh->position_count = p_count/3;
            mesh->connection_count = c_count/2;
            mesh->positions = positions;
            mesh->triangle_indices = triangles;
            mesh->connection_indices = connections;

            track->addMesh(frame, mesh);
        }
        tracks.push_back(track);
    }
}

 char* first = new char[8];
 char* second = new char[8];
 
 int nextInt(std::ifstream* stream){
        stream->read(first, 4);
        for(int i = 0; i<4; i++){
            second[i] = first[3 - i];
        }
        return ((int*)second)[0];
 }

 char* readName(std::ifstream* stream){
     for(int i = 0; i<8; i++){
         first[i] = 0;
         second[i] = 0;
     }
     stream->read(first, 2);
     second[0] = first[1];
     second[1] = first[0];
     int count = *((int*)second);
     printf("characters %d\n", count);
     char* name = new char[count + 1];
     stream->read(name, count);
     return name;
 }
 
 double nextDouble(std::ifstream* stream){
     stream->read(first, 8);
     for(int i = 0; i<8; i++){
        second[i] = first[7-i];
     }
     return ((double*)second)[0];
 }

 int getInt(char* integer){
        
        /*unsigned int v = 0;
        for(int i = 0; i<4; i++){
                unsigned char c = (unsigned char)integer[i];
                //v = v + ((integer[i]&0xff)<<(8*(3-i)));
                v = (v<<8) + c;
        }
        printf(" %ud \n",v);*/
        return ((int*)integer)[0];
}

Track::Track(char* name){
   this->name = name;
}

void Track::addMesh( int frame, BinaryMesh* mesh ){
    meshes.push_back(mesh);
    frames.push_back(frame);
    if(frame < first){
        first = frame;
    }
    if(frame > last ){
        last = frame;
    }
}

BinaryMesh* Track::getMesh(int frame){
    for( int i = 0; i<frames.size(); i++){
        if( frames[i] == frame ){
            return meshes[i];
        }

    }
    return NULL;
}
