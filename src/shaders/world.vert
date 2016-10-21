#version 330 core

//input
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 data;

//uniform vec3 camOffset;
uniform mat4 perspectiveMatrix;
uniform mat4 orientationMatrix;
uniform mat4 depthBiasMatrix;

uniform float low;
uniform float high;
uniform float middle;

out vec4 meshColor;

smooth out vec3 norm;
smooth out vec3 pos;
smooth out vec3 tNorm;
smooth out vec4 shadowCoordinate;

vec4 convertData(vec3 d);

void main() {
		vec4 modelSpace = vec4(position,1);

        //position in camera space.
        vec4 camPosition = perspectiveMatrix*(modelSpace);

        //position in clip space.
        gl_Position = camPosition;
        pos = position;
        norm = normal;

		tNorm = (orientationMatrix*vec4(normal, 0)).xyz;

        meshColor = convertData(data);
        shadowCoordinate = depthBiasMatrix*modelSpace;
        shadowCoordinate = gl_Position;
}

vec4 convertData(vec3 d){
	float f = d.x;
	float r, g, b;
	if(f<middle){
	    //cool colors;
	    r = 1;
	    b = 0;
	    g = 0;
	}else{
	    //warm colors;
	    r = 1;
	    b = 0;
	    g = f>high?1:(high - f)/(high - middle);
	}
	
	return vec4(r, g, b, 1);
	
}
