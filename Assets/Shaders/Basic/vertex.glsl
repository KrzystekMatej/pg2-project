#version 460 core

layout ( location = 0 ) in vec4 vertexPosition;

uniform mat4 P;

void main( void )
{
	gl_Position = vertexPosition;
}