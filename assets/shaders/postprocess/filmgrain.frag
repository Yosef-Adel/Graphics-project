#version 330

// The texture holding the scene pixels
uniform sampler2D tex;
uniform float time;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

void main(){

    // Calculate noise and sample texture
    float mdf = 0.1; // increase for noise amount 
    float noise = (fract(sin(dot(tex_coord, vec2(12.9898,78.233)*2.0)) * 43758.5453)); // random function for noise
    // https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
    vec4 tex_vec = texture(tex, tex_coord);
    
    mdf *= sin(time) + 1.0; // animate the effect's strength
    
    vec4 col = tex_vec - noise * mdf;

    // Output to screen
    frag_color = col;
}