#include "meshviewer.hpp"

int main(int argc, char** args){
	if(argc==1){
		printf("usange: meshviewer meshes.bmf\n");
	}
	Display display;
	display.initialize();

    //open a file and pump the display full of rods.
    PlaneRepresentation* rep = new PlaneRepresentation();
    rep->setup(display.getProgram());
    display.addRepresentation(rep);
    display.render();
    BinaryMeshReader reads( args[1] );

    std::vector<Track*> tracks = reads.getMeshes();
    printf("checking %ld tracks\n", tracks.size());
    int first = 1000;
    int last = -1;

    for( Track* track: tracks){
       int f = track->getFirstFrame();
       if( f < first ){
           first = f;
       }
       int l = track->getLastFrame();
       if( l > last ){
           last = l;
       }
    }

    std::vector<MeshRepresentation*> showing;
    int current = first;
    printf("from frame %d to %d\n", first, last);
    while(display.running==0){
        for(MeshRepresentation* mesh: showing){
            display.removeRepresentation( mesh );
            delete mesh;
        }
        showing.clear();
    	for(Track* track : tracks ){
            BinaryMesh* mesh = track->getMesh(current);
            if(mesh != NULL){
                MeshRepresentation* m = new MeshRepresentation( mesh->position_count, mesh->triangle_count );
                m->updatePositions( mesh->positions );
                m->updateTriangleIndices( mesh->triangle_indices );
                m->solidColor( 0.4f, 0.4f, 1.0f);
                m->calculateNormals();
                m->refreshBuffers();
                display.addRepresentation(m);
                showing.push_back(m);
            }
        }
        display.render();
        current+=1;
        if( current > last ){
            current = first;
        }
    }
}

