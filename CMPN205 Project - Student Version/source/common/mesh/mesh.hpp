#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        
        /*
        Examples for VBO, EBO, and VAO:
        Vertex Buffer Object (VBO): A VBO is a memory buffer in the high speed memory of your video card designed to hold information about vertices. In your case, a VBO is created with glGenBuffers(1, &VBO);.
        Element Buffer Object (EBO): An EBO is a buffer object just like the VBO, but they are used to store indices that point to vertex attributes. The EBO is created with glGenBuffers(1, &EBO);.
        Vertex Array Object (VAO): A VAO stores all of the links between attributes and your VBO with raw vertex data. The VAO is created with glGenVertexArrays(1, &VAO);.
        */

        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            // Generate and bind a Vertex Array Object
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            // Generate and bind a Vertex Buffer Object
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            // Generate and bind an Element Buffer Object
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), &elements[0], GL_STATIC_DRAW);

            // Set the vertex attribute pointers
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

            // Store the number of elements
            elementCount = static_cast<GLsizei>(elements.size());

            /*
            Vertex Attribute Pointers:

            In OpenGL, the vertex data that you pass into your shaders can have multiple attributes. These attributes can include things like position, color, texture coordinates, normals, etc. Each of these attributes can be represented by a vector of 1 to 4 components, which can be of type float, int, uint, double, or bool.
            The functions glEnableVertexAttribArray and glVertexAttribPointer are used to tell OpenGL how to interpret the vertex data.
            glEnableVertexAttribArray(index): This function enables the generic vertex attribute array specified by index. It tells OpenGL that we want to use this attribute in our shader.
            glVertexAttribPointer(index, size, type, normalized, stride, pointer): This function defines an array of generic vertex attribute data. Here’s what each parameter does:

            index: Specifies the index of the generic vertex attribute to be modified.
            size: Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
            type: Specifies the data type of each component in the array.
            normalized: Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
            stride: Specifies the byte offset between consecutive generic vertex attributes.
            pointer: Specifies an offset of the first component of the first generic vertex attribute in the array.

            The size for each of the Vertex Attribute Pointers is determined by the number of components in each attribute. Here's why each attribute has its specific size:
            - **Position (`ATTRIB_LOC_POSITION`)**: The position of a vertex in 3D space is represented by three components: x, y, and z. Therefore, the size of the position attribute is `3`.
            - **Color (`ATTRIB_LOC_COLOR`)**: The color of a vertex is typically represented by three components: red, green, and blue (RGB). Therefore, the size of the color attribute is `3`.
            - **Texture Coordinates (`ATTRIB_LOC_TEXCOORD`)**: Texture coordinates are usually 2D, so they have two components: u and v (or sometimes referred to as s and t). Therefore, the size of the texture coordinate attribute is `2`.
            - **Normal (`ATTRIB_LOC_NORMAL`)**: A normal is a vector that is perpendicular to a surface, and it is used in lighting calculations. Like the position, it is a vector in 3D space, so it has three components: x, y, and z. Therefore, the size of the normal attribute is `3`.
            These sizes are quite standard in 3D graphics, but depending on your specific needs, you might have different attributes or different sizes. For example, you might include an alpha component in your color attribute, making it a `vec4` and the size `4`. Or you might have 3D texture coordinates, making it a `vec3` and the size `3`. But for most common scenarios, the sizes specified above are used. 
            */

        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
            /*
            Why we aren’t unbinding at the end of drawing:
            Unbinding buffers and arrays can be useful in some cases to avoid accidentally modifying them later. However, since each draw call requires specific buffers and arrays to be bound, you will have to bind them again before each draw call anyway. Therefore, unbinding at the end of each draw call can often be unnecessary, and it can even degrade performance in some cases.
            */
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 2) Write this function
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}