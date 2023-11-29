#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out Varyings {
    vec4 color;
} vs_out;

uniform mat4 transform;

void main(){
    //TODO: (Req 7) Change the next line to apply the transformation matrix
    vec4 transformedPosition = transform * vec4(position, 1.0);
    
    gl_Position = transformedPosition;
    
    vs_out.color = color;
}