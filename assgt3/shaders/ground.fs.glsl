#version 330 core

uniform vec3 fColour;

//out vec4 colour;
out vec3 color;
in vec2 uv;
uniform sampler2D tex;
uniform sampler2D tex_mirror;

void main()
{
	float window_width = textureSize(tex_mirror, 0).x;
	float window_height = textureSize(tex_mirror, 0).y;   
    float ScreenU =     gl_FragCoord.x/window_width;
    float ScreenV = 1 - gl_FragCoord.y/window_height;
    vec3 color_from_texture = texture(tex,uv).rgb;
    vec3 color_from_mirror = texture(tex_mirror,vec2(ScreenU,ScreenV)).rgb;
    color = mix(color_from_texture, color_from_mirror, vec3(.15));

	color = (color_from_mirror).rgb;
}