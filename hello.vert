#version 400 core

in vec4 attribute_Position; // vertex shader
in vec4 attribute_Color; // attributes
in vec2 attribute_texCoord;
in vec3 displacement;
in float scale;

uniform mat4 mvpMatrix;

out vec4 varying_Color; // Outgoing varying data
out vec2 texCoord;
void main(void) {

	varying_Color = attribute_Color;
        gl_Position = mvpMatrix*vec4(scale*attribute_Position.xyz + displacement, 1.0);
        texCoord = attribute_texCoord;
         //gl_Position = attribute_Position;
}