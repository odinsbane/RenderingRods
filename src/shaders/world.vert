#version 330 core

//input
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 data;

//uniform vec3 camOffset;
uniform mat4 perspectiveMatrix;
uniform mat4 orientationMatrix;
uniform mat4 depthBiasMatrix;

uniform vec3 lightPos;

out vec4 meshColor;

smooth out vec3 norm;
smooth out vec3 pos;
smooth out vec3 tNorm;
smooth out vec4 shadowCoordinate;
smooth out vec3 toLight;
smooth out vec3 transLight;
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
		toLight = lightPos - pos;
		transLight = normalize((orientationMatrix*vec4(toLight,0)).xyz);
        meshColor = vec4(data.x, data.y, data.z, 1);
        //meshColor = vec4(1, 1, 1, 1);
        shadowCoordinate = depthBiasMatrix*modelSpace;
        //shadowCoordinate = gl_Position;
        //shadowCoordinate = ;
        
}


