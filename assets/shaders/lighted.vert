#version 330

uniform vec3 eye; 
uniform mat4 VP;
uniform mat4 M; //local to world
uniform mat4 M_IT; //local to world inverse transpose


layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
layout(location=2) in vec2 tex_coord;
layout(location=3) in vec3 normal;

out Varyings 
{
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;


void main()
{

    vec3 world = (M * vec4(position, 1.0)).xyz; //local ro world for each vertex

    gl_Position = VP * vec4(world, 1.0); // position of it at screen
    fs_in.color = color; 
    fs_in.tex_coord = tex_coord;
    fs_in.normal = normalize((M_IT * vec4(normal, 0.0)).xyz); // normal vector at world
    fs_in.view = eye - world; //direction of looking for cam
    fs_in.world = world;
}
