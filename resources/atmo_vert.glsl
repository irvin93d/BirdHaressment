#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPos;

out float distance;

void main()
{
	distance = length((M*vertPos).xyz - lightPos) / length(vec3(0,0,0) - lightPos);
	gl_Position = P * V * M * vertPos;
}

