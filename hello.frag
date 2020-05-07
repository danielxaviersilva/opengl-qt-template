#version 400 core

in vec4 varying_Color; // incomming varying data to the
					   // fragment shader
					   // sent from the vertex shader

out vec4 FragColor;
void main (void) 
{
	FragColor = varying_Color;
}