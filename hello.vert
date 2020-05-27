#version 400 core

in vec4 attribute_Position; // vertex shader
in vec4 attribute_Color; // attributes

uniform mat4 mvpMatrix;

out vec4 varying_Color; // Outgoing varying data
void main(void) {

	varying_Color = attribute_Color;
        gl_Position = mvpMatrix*attribute_Position;
//         gl_Position = attribute_Position;
}
