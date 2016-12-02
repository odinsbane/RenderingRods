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

glm::vec3 Rod::getLocation(double s){
    double dex = (length/2 + s)/ds;
    if(dex<0){
        return glm::vec3(*points[0]);
    } else if(dex>=points.size()-1){
        return glm::vec3(*points[N-1]);
    } else{
        int low = (int)dex;
        int high = low + 1;
        double f = dex - low;
        glm::vec3 lo = *points[low];
        glm::vec3 hi = *points[high];
        return glm::vec3(
                         lo.x*(1-f) + hi.x*f,
                         lo.y*(1-f) + hi.y*f,
                         lo.z*(1-f) + hi.z*f
                         );
    }
}
