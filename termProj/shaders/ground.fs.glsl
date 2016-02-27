#version 330 core

uniform vec3 fColour;

out vec4 colour;
in vec2 uv;
uniform sampler2D tex;
//uniform sampler2D tex_mirror;

void main()
{
	colour = texture(tex,uv).rgba;
}