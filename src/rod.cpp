#include "rod.hpp"

Rod* Rod::createRod(int n, double l, glm::vec3 &pos, glm::vec3 &dir){
	ds = l/(N-1);
	length = l;
	N = n;
	for(int i = 0; i<N; i++){
	    
		glm::vec3* p = new glm::vec3( 
			pos.x + dir.x*(0.5*length - i*ds),
			pos.y + dir.y*(0.5*length - i*ds),
			pos.z + dir.z*(0.5*length - i*ds)
		);
		
		points.push_back(p);
	}
	return new Rod(l, points);
}
Rod::Rod(double l, std::vector<glm::vec3*> p){ 
	points = p; 
	N = points.size(); 
	length = l; ds = l/N;
}
		