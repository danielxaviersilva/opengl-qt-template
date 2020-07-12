#version 400 core

in vec4 varying_Color; // incomming varying data to the
					   // fragment shader
					   // sent from the vertex shader
uniform sampler2D u_Texture;
in vec2 texCoord;

out vec4 FragColor;
void main (void) 
{
	//FragColor = varying_Color;
	//FragColor = 0.5*(texture(u_Texture, texCoord)+ varying_Color);
	FragColor = texture(u_Texture, texCoord);
}