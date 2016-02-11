#version 330 core

layout(location = 0) in vec4 vPosition;
in vec3 vpoint;
uniform mat4 Mat; //compound transform matrix

void main()
{
	gl_Position = Mat * vec4(vpoint.x, vpoint.y, vpoint.z, 1.0);
}