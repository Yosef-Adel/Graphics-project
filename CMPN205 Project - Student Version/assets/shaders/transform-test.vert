#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

out Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} vs_out;

uniform mat4 transform;

void main(){
    //TODO: (Req 3) Change the next line to apply the transformation matrix

    vec4 transformedPosition = transform * vec4(position, 1.0);
    
    gl_Position = transformedPosition;
    /*
        gl_Position is a built-in output variable that we have to set in the vertex shader.
        It should contain the final position of each vertex in clip space (after all transformations have been applied).
        The value of gl_Position is used by OpenGL for various operations, including clipping and rasterization.
        In other words, OpenGL needs to know where to draw each vertex on the screen, and gl_Position provides this information.
    */

    // No need to change any of the following lines
    vs_out.position = transformedPosition.xyz;;
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    vs_out.normal = normal;
}