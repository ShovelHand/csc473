#version 330 core

layout(location = 0) in vec4 vPosition;

uniform mat4 uMVP;

void main()
{
	//vec4 world = MODEL * vec4(vpoint, 1.0);
	gl_Position = uMVP * vPosition;
}
