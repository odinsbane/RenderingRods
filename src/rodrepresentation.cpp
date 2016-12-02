#include "display.hpp"

RodRepresentation::RodRepresentation(Rod* rod, GLuint program){
	
    std::vector<glm::vec3*> pts = rod->getPoints();
    N = pts.size();
    points = new float[6*N*3];
    normals = new float[6*N*3];
    data = new float[6*N*3];
    
    updateData(pts);
    indexes = new int[(N-1)*12*3 ];
    int c = 0;
    //N = rod->getPoints()->size();
    for(int i = 0; i<N-1; i++){
    	for(int j = 0; j<6; j++){
    		indexes[c++] = 6*i + (j+1)%6;
    		indexes[c++] = 6*i + j;
    		indexes[c++] = 6*(i+1) + j;
    		indexes[c++] = 6*(i+1) + j;
    		indexes[c++] = 6*(i+1) + (j+1)%6;
    		indexes[c++] = 6*(i) + (j+1)%6;
    	}
    }
    //indexes[0] = 0;
    //indexes[1] = 6;
    //indexes[2] = 1;
	prepareBuffers();
	
}

void RodRepresentation::render(GLuint program){
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 12*(N-1)*3, GL_UNSIGNED_INT, 0);	
	glBindVertexArray(0);
	glUseProgram(0);
}

/*
	Each point will have a color associated with it, that color will be based on the
	local curvature for now.
*/
void RodRepresentation::calculateData(std::vector<glm::vec3*> &values){
	max = 0;
	ave = 0;
	std = 0;
    for(int i = 1; i<values.size()-1; i++){
    	glm::vec3 t0 = glm::normalize((*values[i]) - (*values[i-1]));
		glm::vec3 t1 = glm::normalize((*values[i+1]) - (*values[i]));
		
    	double f = glm::length(t1 - t0);
    	t1 = t1 - t0;
    	ave += f;
    	std += f*f;
    	if(f>max){
    	    max = f;
    	}
    	
    	for(int j = 0; j<6; j++){
			int dex = j*3 + (i)*18;
			data[dex + 0] = f;
			data[dex + 1] = 0;
			data[dex + 2] = 0;
		}
    }
    ave = ave/values.size();
    std = std/values.size();
    
    std = sqrt(std - ave*ave);
    
    for(int i = 0; i<values.size(); i+=values.size()-1){
    	for(int j = 0; j<6; j++){
			int dex = j*3 + (i)*18;
			data[dex + 0] = 0;
			data[dex + 1] = 0;
			data[dex + 2] = 0;
		}
    }
    
    
}
void RodRepresentation::updateData(std::vector<glm::vec3*> &values){
	double radius = 0.032;
	for(int i = 1; i<values.size(); i++){
		glm::vec3 forward = glm::normalize((*values[i]) - (*values[i-1]));
		
		//find an up.
		glm::vec3 a = glm::vec3(0, 0, 1) - forward.z*forward;
		if(a.length()==0){
			a = glm::vec3(0, 1, 0) - forward.y*forward;
		}
		a = glm::normalize(a);
		
		glm::vec3 b = glm::cross(a, forward);
		
		for(int j = 0; j<6; j++){
			double theta = (j)*M_PI/3.0;
			double c = cos(theta);
			double s = sin(theta);
			
			glm::vec3 proja(a.x*c, a.y*c, a.z*c);
			glm::vec3 projb(b.x*s, b.y*s, b.z*s);
			glm::vec3 loc = glm::vec3(
				values[i-1]->x + radius*proja.x + radius*projb.x,
				values[i-1]->y + radius*proja.y + radius*projb.y,
				values[i-1]->z + radius*proja.z + radius*projb.z 
			 );
			 glm::vec3 norm(
			 	proja.x + projb.x,
				proja.y + projb.y,
				proja.z + projb.z 
			);
			int dex = j*3 + (i-1)*18;
			points[dex + 0] = loc.x;
			points[dex + 1] = loc.y;
			points[dex + 2] = loc.z;
			
			normals[dex + 0] = norm.x;
			normals[dex + 1] = norm.y;
			normals[dex + 2] = norm.z;
		}
		if(i==values.size()-1){
			for(int j = 0; j<6; j++){
				double theta = (j)*M_PI/3.0;
				double c = cos(theta);
				double s = sin(theta);
			
				glm::vec3 proja(a.x*c, a.y*c, a.z*c);
				glm::vec3 projb(b.x*s, b.y*s, b.z*s);
				glm::vec3 loc = glm::vec3(
					values[i]->x + radius*proja.x + radius*projb.x,
					values[i]->y + radius*proja.y + radius*projb.y,
					values[i]->z + radius*proja.z + radius*projb.z 
				 );
				glm::vec3 norm(
			 		proja.x + projb.x,
					proja.y + projb.y,
					proja.z + projb.z 
				);
				 
				int dex = j*3 + (i)*18;
				points[dex + 0] = loc.x;
				points[dex + 1] = loc.y;
				points[dex + 2] = loc.z;
				
				normals[dex + 0] = norm.x;
				normals[dex + 1] = norm.y;
				normals[dex + 2] = norm.z;
			}		
		} 
	}
	calculateData(values);
	/*
	for(int i = 0; i<N*6; i++){
		printf("(");
		//each collection of points.
		for(int j = 0; j<3; j++){
			int s = i*3;
			printf("%f, ",points[s]+j);
		}
		printf(")\n");
	}*/
}

void RodRepresentation::prepareBuffers(){
	//updateData(pts);
	/*for(int i = 0; i<(N-1); i++){
    	for(int j = 0; j<12; j++){
    		printf("( %0.7d, %0.7d, %0.7d ) \n", indexes[i*36 + j*3], indexes[i*36 + j*3 +1], indexes[i*36 + j*3 + 2]);
    		for(int k = 0; k<3; k++){
	    		printf("->( %f, %f, %f ) %d \n", 
	    			normals[3*indexes[i*36 + j*3 + k]-0], 
	    			normals[3*indexes[i*36 + j*3 + k]+1], 
	    			normals[3*indexes[i*36 + j*3 + k]+2],
	    			indexes[i*36 + j*3 + k]);
			}
    	}
		printf("\n");
    	printf("\n");
    }*/
	
	glGenBuffers(1, &pBO);
    glBindBuffer(GL_ARRAY_BUFFER, pBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*3*N, points, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &nBO);
    glBindBuffer(GL_ARRAY_BUFFER, nBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*3*N, normals, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &dBO);
	glBindBuffer(GL_ARRAY_BUFFER, dBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*3*N, data, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &iBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6*2*(N-1)*3, indexes, GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	
	/*
	GLuint positionAttribute = glGetAttribLocation(programId, "position");
	GLuint normalAttribute = glGetAttribLocation(programId, "normal");
	GLuint dataAttribute = glGetAttribLocation(programId, "data");
	*/
	GLuint positionAttribute = 0;
	GLuint normalAttribute = 1;
	GLuint dataAttribute = 2;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, pBO);
	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, nBO);
	glEnableVertexAttribArray(normalAttribute);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, dBO);
	glEnableVertexAttribArray(dataAttribute);
	glVertexAttribPointer(dataAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBO);

	glBindVertexArray(0);
	
}
