#version 330 core

layout(location = 0) in vec4 vPosition;
in vec3 vpoint;
out vec3 fpoint;


uniform mat4 Mat; //compound transform matrix

void main()
{
	fpoint = vpoint;
	gl_Position = Mat * vec4(vpoint.x, vpoint.y, vpoint.z, 1.0);
}