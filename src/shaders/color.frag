#version 330 core

out vec4 outputColor;
in vec4 meshColor;
smooth in vec3 pos;
smooth in vec3 norm;
smooth in vec3 tNorm;
smooth in vec4 shadowCoordinate;

vec4 lightIntensity = vec4(0.3, 0.3, 0.3, 1);
vec4 ambientIntensity = vec4(0.8, 0.8, 0.8, 1);

uniform vec3 lightPos;
uniform sampler2DShadow shadowMap;

float colorComponent(float mesh, float a, float i);
void main() {
    
	vec3 disp = lightPos - pos;
	
	float l = dot(disp, disp);
	if(l>0){
		disp = normalize(disp);
	}
	
	float incidenceCos = dot(norm, disp);
	
	if(incidenceCos<0){
	} else{
		incidenceCos/pow(l, 0.1);
	}
	
	float chi = dot(tNorm, disp);
	
	outputColor = vec4(
		colorComponent(meshColor.x,ambientIntensity.x, incidenceCos*lightIntensity.x),  
		colorComponent(meshColor.y,ambientIntensity.y, incidenceCos*lightIntensity.y),  
		colorComponent(meshColor.z,ambientIntensity.z, incidenceCos*lightIntensity.z),  
		1);
	float visibility = texture(shadowMap, vec3(shadowCoordinate.xy, shadowCoordinate.z/shadowCoordinate.w));
    outputColor = outputColor*visibility;
}


float colorComponent(float mesh, float a, float i){
	
	float c;
	
	c = mesh*a + i;
	if(c<0) c = 0;
	if(c>1) c = 1;
	
	return c;
}
