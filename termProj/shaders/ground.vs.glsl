#version 330 core

layout(location = 0) in vec4 vPosition;

uniform mat4 Mat;
in vec3 vpoint;

void main()
{
	gl_Position = Mat * vec4(vpoint.x, vpoint.y, vpoint.z, 1.0);
}
