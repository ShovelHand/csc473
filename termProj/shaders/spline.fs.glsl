#version 330 core

uniform vec3 fColour;

out vec4 colour;

void main()
{
	colour = vec4(fColour, 1);
}