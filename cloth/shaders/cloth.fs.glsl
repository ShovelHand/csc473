#version 330 core

in vec3 fpoint;
out vec4 fColor;

void main()
{
	fColor = vec4(0.2, 0.2, 1, 1);

	//lighting
	vec3 L = vec3(-0.5,0.3,-3); //lightsource
	float intensity = 1.5;

	float light = intensity*max(0.0, dot(normalize(fpoint), normalize(L)));
	fColor += vec3(light,light,light);
	
}