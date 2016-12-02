#version 330 core

out vec4 outputColor;
in vec4 meshColor;
smooth in vec3 pos;
smooth in vec3 norm;
smooth in vec3 tNorm;
smooth in vec4 shadowCoordinate;
smooth in vec3 toLight;
smooth in vec3 transLight;

vec4 lightIntensity = vec4(0.7, 0.7, 0.7, 1);
vec4 ambientIntensity = vec4(0.4, 0.4, 0.4, 1);

uniform sampler2D shadowMap;

vec2 poissonDisk[4] = vec2[](
                             vec2( -0.94201624, -0.39906216 ),
                             vec2( 0.94558609, -0.76890725 ),
                             vec2( -0.094184101, -0.92938870 ),
                             vec2( 0.34495938, 0.29387760 )
                             );

float colorComponent(float mesh, float a, float i);
void main() {
    
	vec3 disp = toLight;
	
	float l = dot(disp, disp);
	if(l>0){
		disp = normalize(disp);
	}
	
	float incidenceCos = dot(norm, disp);
	
	if(incidenceCos<0.01){
        incidenceCos = 0.01;
	} else{
	}
    
    
    
    float visibility = 1;
    float bias = 0.0005;
    float f = 1;
    
    for (int i=0;i<4;i++){
        if ( texture( shadowMap, shadowCoordinate.xy + poissonDisk[i]/1000.0 ).x  <  shadowCoordinate.z-bias ){
            visibility-=0.2;
        }
        if ( texture( shadowMap, shadowCoordinate.xy + poissonDisk[i]/500.0 ).x  <  shadowCoordinate.z-0.5*bias ){
            visibility-=0.025;
        }
        if ( texture( shadowMap, shadowCoordinate.xy + poissonDisk[i]/250.0 ).x  <  shadowCoordinate.z- 0.25*bias ){
            visibility-=0.005;
        }
    }
    
    incidenceCos*=visibility;
    
    
	outputColor = vec4(
		colorComponent(f*meshColor.x,ambientIntensity.x, incidenceCos*lightIntensity.x),
		colorComponent(f*meshColor.y,ambientIntensity.y, incidenceCos*lightIntensity.y),
		colorComponent(f*meshColor.z,ambientIntensity.z, incidenceCos*lightIntensity.z),
		1);
    
    
}


float colorComponent(float mesh, float a, float i){
	
	float c;
	
	c = mesh*a + i;
	if(c<0) c = 0;
	if(c>1) c = 1;
	
	return c;
}
