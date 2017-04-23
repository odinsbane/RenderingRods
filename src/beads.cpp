#include "beads.hpp"
Reader* loadFile(char* file);
std::vector<RodRepresentation*> rodRepresentations;
std::vector<SpringRepresentation*> springRepresentations;
std::vector<MotorRepresentation*> motorRepresentations;

int main(int argc, char** args){
	if(argc==1){
		printf("usange: beads pointfile00.dat pointfile01.dat ...\n");		
	}
	Display display;
	display.initialize();
	//open a file and pump the display full of rods.
    PlaneRepresentation* rep = new PlaneRepresentation();
    rep->setup(display.getProgram());
    display.addRepresentation(rep);
    display.render();
    
    std::vector<Reader*> readers;
    for(int i = 1; i<argc;i++){
    	readers.push_back(loadFile(args[i]));
    } 
    
    Reader* reader = readers[0];
    
    std::vector<Rod*> rods = reader->getRods();
    double w = reader->width;
    
    for(Rod* rod: rods){
		RodRepresentation* rep = new RodRepresentation(rod->getPoints(), 0.032, display.getProgram());
		rodRepresentations.push_back(rep);
		display.addRepresentation(rep);
	}
    
    std::vector<Link*> links = reader->getLinks();
    for(Link* link: links){
        glm::vec3 pA = rods[link->A]->getLocation(link->aS);
        glm::vec3 pB = rods[link->B]->getLocation(link->bS);
        pB = wrap(w, pA, pB);
        SpringRepresentation *spring = new SpringRepresentation(pA, pB);
        springRepresentations.push_back(spring);
        display.addRepresentation(spring);
	}
    
    std::vector<Motor*> motors = reader->getMotors();
    
    for(Motor* motor: motors){
        glm::vec3 pA, pB;
        if(motor->head>=0){
            pA = rods[motor->head]->getLocation(motor->headS);
            pA = wrap(w, *(motor->points[0]), pA);
        } else{
            pA = glm::vec3(*(motor->points[0]));
        }
        if(motor->tail>=0){
            pB = rods[motor->tail]->getLocation(motor->tailS);
            pB = wrap(w, *(motor->points[1]), pB);
            
        } else{
            pB = glm::vec3(*(motor->points[1]));
        }
        MotorRepresentation* rep = new MotorRepresentation(motor, pA, pB, display.getProgram());
        display.addRepresentation(rep);
        motorRepresentations.push_back(rep);
        
    }
	
    int total = readers.size();
    int count = 0;
    int last=-1;
    
    while(display.running==0){
    	display.render();
    	last = count;
    	count = (count+1)%total;
    	if(count!=last){
    		reader = readers[count];
    		rods = reader->getRods();
    		//update some representations.
    		for(int i = 0; i<rodRepresentations.size(); i++){
    			RodRepresentation* rep = rodRepresentations[i];
    			rep->updateData(rods[i]->getPoints());
    			rep->refreshBuffers();
    		}
    		
    		links = reader->getLinks();
    		for(int i = 0; i<springRepresentations.size();i++){
    		    Link* link = links[i];
    			glm::vec3 pA = rods[link->A]->getLocation(link->aS);
        		glm::vec3 pB = rods[link->B]->getLocation(link->bS);
        		pB = wrap(w, pA, pB);
        		SpringRepresentation *spring = springRepresentations[i];
                spring->updateRepresentation(pA, pB);
                spring->refreshBuffers();
        	}
            
            motors = reader->getMotors();
            for(int i = 0; i<motorRepresentations.size(); i++){
                Motor* motor = motors[i];
                
                glm::vec3 pA, pB;
                if(motor->head>=0){
                    pA = rods[motor->head]->getLocation(motor->headS);
                    pA = wrap(w, *(motor->points[0]), pA);
                } else{
                    pA = glm::vec3(*(motor->points[0]));
                }
                if(motor->tail>=0){
                    pB = rods[motor->tail]->getLocation(motor->tailS);
                    pB = wrap(w, *(motor->points[1]), pB);
                    
                } else{
                    pB = glm::vec3(*(motor->points[1]));
                }
                motorRepresentations[i]->updateData(motor, pA, pB);
                motorRepresentations[i]->refreshBuffers();
            }
    	}
    }
    
    for(Reader* reader: readers){
    	delete reader;
    }
}

Reader* loadFile(char* file){
	Reader* reader = new Reader(file);
    reader->load();
	return reader;
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
