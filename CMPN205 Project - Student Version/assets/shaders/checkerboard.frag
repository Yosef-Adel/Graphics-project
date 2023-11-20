#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    // Calculate the tile coordinates
    ivec2 coord = ivec2(gl_FragCoord.xy) / size;

    // Use the parity of the sum of the coordinates to select the color
    int index = int(mod(float(coord.x + coord.y), 2.0));

    
    frag_color = vec4(colors[index], 1.0);
}