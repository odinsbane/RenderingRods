#include "beads.hpp"

int main(int argc, char** args){
	Display display;
	display.initialize();
	//open a file and pump the display full of rods.
	Reader reader(args[1]);
	std::vector<Rod*> rods = reader.loadRods();
	
	for(Rod* rod: rods){
		RodRepresentation* rep = new RodRepresentation(rod, display.getProgram());
		display.addRodRepresentation(rep);
	}
    PlaneRepresentation* rep = new PlaneRepresentation();
    rep->setup(display.getProgram());
    display.addRepresentation(rep);
	display.graphicsLoop();
    
}
