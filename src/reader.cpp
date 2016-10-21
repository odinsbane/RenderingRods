#include "reader.hpp"

Reader::Reader(const std::string &filename){
    inputStream = new std::ifstream(filename, std::ifstream::binary);
}


std::vector<Rod*> Reader::loadRods(){

    std::vector<Rod*> rods;
	long time = nextLong();
	
	double width = nextDouble();
	int type = nextInt();
	printf("loading data from %ld :: %f :: %d \n", time, width, type);	
	while(type==0){
		std::vector<glm::vec3*> points;
		double length = nextDouble();
		double k = nextDouble();
		double kappa = nextDouble();
		int n = nextInt();
		
		for(int i = 0; i<n; i++){
		    points.push_back( nextPoint());
		}
		
		type = nextInt();
		rods.push_back( new Rod(length, points));
	}
	//printf("%ld :: %f :: %d \n", time, 1.0, 0);	
	close();
	return rods;  
}
glm::vec3* Reader::nextPoint(){
	double x = nextDouble();
	double y = nextDouble();
	double z = nextDouble();
	return new glm::vec3(x, y, z);
}
int Reader::nextInt(){
    if(closed) return -1;
    inputStream->read((char*)buffer, 4);
    for(int i = 0; i<4; i++){
		data[i] = buffer[3 - i];
    }
	return *((int*)data);
}

long Reader::nextLong(){
    if(closed) return -1;
    
    inputStream->read(buffer, 8);
    for(int i = 0; i<8; i++){
        data[i] = buffer[7 - i];
    }
    return *((long*)data);
}

double Reader::nextDouble(){
    if(closed) return -1;

    inputStream->read((char*)buffer, 8);
        for(int i = 0; i<8; i++){
        data[i] = buffer[7 - i];
    }

    return *((double*)data);
}

void Reader::close(){
    closed=true;
    inputStream->close();
    delete inputStream;
    
}

