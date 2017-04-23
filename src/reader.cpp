#include "reader.hpp"

Reader::Reader(const std::string &filename){
    inputStream = new std::ifstream(filename, std::ifstream::binary);
}

std::vector<Rod*> Reader::getRods(){
    return rods;
}

std::vector<Link*> Reader::getLinks(){
    return links;
}

std::vector<Motor*> Reader::getMotors(){
    return motors;
}

void Reader::load(){
	closed=false;
    
	long time = nextLong();
	
	width = nextDouble();
	int type = nextInt();
	printf("loading data from %ld :: %f :: %d \n", time, width, type);	
	while(type!=-1){
        if(type==0){
            std::vector<glm::vec3*> points;
            double length = nextDouble();
            double k = nextDouble();
            double kappa = nextDouble();
            int n = nextInt();
            
            for(int i = 0; i<n; i++){
                points.push_back( nextPoint());
            }
            rods.push_back( new Rod(length, points));
        } else if(type==1){

            double l = nextDouble();
            double k = nextDouble();
            int a = nextInt();
            double as = nextDouble();
            int b = nextInt();
            double bs = nextDouble();
            links.push_back(new Link(a, b, as, bs));
            
        } else if(type==2){
            
             double stalkLength = nextDouble();
             double stalkStiffness = nextDouble();
             double springLength = nextDouble();
             double springStiffness = nextDouble();
             double bindTau = nextDouble();
            
             double x0 = nextDouble();
             double y0 = nextDouble();
             double z0 = nextDouble();
            
             double x1 = nextDouble();
             double y1 = nextDouble();
             double z1 = nextDouble();
             Motor* motor = new Motor(glm::vec3(x0,y0,z0), glm::vec3(x1, y1, z1));
            motor->length = stalkLength;
            motor->stiffness = stalkStiffness;
            motor->springStiffness = springStiffness;
            motor->springLength = springLength;
            motor->bindTau = bindTau;
            
            
            int aDex = nextInt();
            motor->head=aDex;
            if(aDex>=0) {
                motor->headS = nextDouble();
                motor->headTime = nextDouble();
             }
             
             int bDex = nextInt();
            motor->tail = bDex;
             if(bDex>=0) {
                 motor->tailS = nextDouble();
                 motor->tailTime = nextDouble();
             }
            
            motors.push_back(motor);
            
            
        } else if(type==3){
            int dex = nextInt();
            double loc = nextDouble();
            double fx = nextDouble();
            double fy = nextDouble();
            double fz = nextDouble();
        }
		type = nextInt();
	}
	//printf("%ld :: %f :: %d \n", time, 1.0, 0);	
	close();
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

