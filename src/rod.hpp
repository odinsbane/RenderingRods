#ifndef __ROD__
#define __ROD__

#include <vector> 
#include <math.h>
#include <glm/vec3.hpp>

class Rod{
    std::vector<glm::vec3*> points;
    int N; //number of segments
    double length;
    double ds;
	public:
		Rod(double l, std::vector<glm::vec3*> p);
		~Rod(){
			for(glm::vec3* point: points){
			    delete point;
			}
		}
		Rod* createRod(int N, double l, glm::vec3 &pos, glm::vec3 &dir);
        glm::vec3* getPoint(int dex){ return points[dex];}
        std::vector<glm::vec3*>& getPoints(){return points;}
        
};

#endif
