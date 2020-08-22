#version 400

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;


in vec4 vertex;



void main(void) 
{
	gl_Position = u_modelViewMatrix*u_projectionMatrix *vertex;
}
