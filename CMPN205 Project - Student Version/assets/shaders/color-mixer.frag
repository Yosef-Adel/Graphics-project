#version 330 core

// This shader is designed to work with "triangle.vert" and it receives an
// interpolated varying which represents the vertex color.

in Varyings {
    vec3 color;
} fs_in;

out vec4 frag_color;

// currently the shader just returns the interpalated color varying.
// However, we want to mix the color channels around. We can do this using a 
// color matrix which we will send to the shader as 3 uniforms: red, green, blue.
// Each of these 3 variables will be a vec4. To apply the channel mixing for a
// certain channel (e.g. red), we apply this linear transformation:
// frag_color.r = red.r * fs_in.color.r + red.g + fs_in.color.g + red.b + fs_in.color.b + red.a;
// However, this line is too long to write, so we can simplify it using a dot product
// (which is defined in the "dot" function).

//TODO: (Req 1) Finish this shader and apply the channel mixing using the "dot" function.

uniform vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
uniform vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);

void main(){

    float x = dot(red, vec4(fs_in.color, 0.0) );
    x = x + red.a <= 1.0 ? x + red.a : x;
    float y = dot(green, vec4(fs_in.color, 0.0) );
    y = y + green.a <= 1.0 ? y + green.a : y;
    float z = dot(blue, vec4(fs_in.color, 0.0) );
    z = z + blue.a <= 1.0 ? z + blue.a : z;

    frag_color = 
    vec4(
        x,
        y,
        z,
        1.0
    );

    // float x = dot(red, vec4(fs_in.color, 0.0) );
    // x = x + red.a <= 1.0 ? x + red.a : x;
    // float y = dot(green, vec4(fs_in.color, 0.0) );
    // y = y + green.a <= 1.0 ? y + green.a : y;
    // float z = dot(blue, vec4(fs_in.color, 0.0) );
    // z = z + blue.a <= 1.0 ? z + blue.a : z;
    
    
    // float x = dot(red, vec4(fs_in.color, 0.0) );
    // x = x > 0.0 ? x : red.a;
    // float y = dot(green, vec4(fs_in.color, 0.0) );
    // y = y > 0.0 ? y : green.a;
    // float z = dot(blue, vec4(fs_in.color, 0.0) );
    // z = z > 0.0 ? z : blue.a;

    // frag_color = 
    // vec4(
    //     x,
    //     y,
    //     z,
    //     1.0
    // );

    // the two above work for shader but fail two test cases "-1" in mesh which is the test for color for mesh


    // vec4(
    //                 dot(red, vec4(fs_in.color, 1.0)),
    //                 dot(green, vec4(fs_in.color, 1.0)),
    //                 dot(blue, vec4(fs_in.color, 1.0)),
    //                 1.0); //doesn't work

    // frag_color = vec4(
    //     dot(red, vec4(fs_in.color, 0.0) ),
    //     dot(green, vec4(fs_in.color, 0.0)) ,
    //     dot(blue, vec4(fs_in.color, 0.0)) ,
    //     1.0
    // ); //works partially

    // frag_color = vec4(
    //     red.r * fs_in.color.r + red.g * fs_in.color.g + red.b * fs_in.color.b ,
    //     green.r * fs_in.color.r + green.g * fs_in.color.g + green.b * fs_in.color.b,
    //     blue.r * fs_in.color.r + blue.g * fs_in.color.g + blue.b * fs_in.color.b,
    //     1.0
    // ); //words partially

}