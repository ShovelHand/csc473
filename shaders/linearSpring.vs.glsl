#version 330 core

layout(location = 0) in vec4 vPosition;

uniform mat4 Mat; //compound transform matrix

void main()
{
	gl_Position = Mat * vPosition;
}