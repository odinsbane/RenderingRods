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
        glm::vec3 getLocation(double s);
        std::vector<glm::vec3*>& getPoints(){return points;}
        
};

class Link{
public:
    int A,B;
    double aS;
    double bS;
    
    Link(int a, int b, double as, double bs){
        A = a;
        B = b;
        aS = as;
        bS = bs;
    }
    
};

class Motor{
    
public:
    int head, tail;
    double headS, tailS;
    double headTime, tailTime;
    double length, stiffness, springStiffness, springLength, bindTau;
    
    std::vector<glm::vec3*> points;
    
    Motor(glm::vec3 h, glm::vec3 t){
        points.push_back(new glm::vec3(h));
        points.push_back(new glm::vec3(t));
    }
    
    ~Motor(){
        delete points[0];
        delete points[1];
    }
};

#endif
