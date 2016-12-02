#include "beads.hpp"

int main(int argc, char** args){
	Display display;
	display.initialize();
	//open a file and pump the display full of rods.
	Reader reader(args[1]);
    reader.load();
	std::vector<Rod*> rods = reader.getRods();
	
	for(Rod* rod: rods){
		RodRepresentation* rep = new RodRepresentation(rod, display.getProgram());
		display.addRodRepresentation(rep);
	}
    std::vector<Link*> links = reader.getLinks();
    for(Link* link: links){
        glm::vec3 pA = rods[link->A]->getLocation(link->aS);
        glm::vec3 pB = rods[link->B]->getLocation(link->bS);
        pB = wrap(reader.width, pA, pB);
        SpringRepresentation *spring = new SpringRepresentation(pA, pB);
        display.addRepresentation(spring);
    }
    PlaneRepresentation* rep = new PlaneRepresentation();
    rep->setup(display.getProgram());
    display.addRepresentation(rep);
	display.graphicsLoop();
    
}

glm::vec3 wrap(double width, glm::vec3 src, glm::vec3 target){
    double x = src.x - wrap(width, src.x - target.x);
    double y = src.y - wrap(width, src.y - target.y);
    double z = src.z - wrap(width, src.z - target.z);
    return glm::vec3(x, y, z);
}

double wrap(double width, double v){
    if(v>width/2){
        return v - width;
    } else if(v<-width/2){
        return v + width;
    } else{
        return v;
    }
}
