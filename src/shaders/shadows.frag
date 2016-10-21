#version 330 core

// Ouput data
layout(location = 0) out float fragmentdepth;
//out vec4 outputColor;

void main(){
    // Not really needed, OpenGL does it anyway
    fragmentdepth = gl_FragCoord.z;
    //outputColor = vec4(1,1,gl_FragCoord.z,1);
}
