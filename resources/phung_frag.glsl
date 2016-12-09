#version 330 core 
out vec4 color;
in vec3 fragNor;
in vec3 reflDir;
in vec3 viewDir;
in vec3 lightDir;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float shine;

void main()
{
	vec3 ambRefl = 0.5 * MatAmb;
	vec3 diffRefl = 0.8 * max(0,dot(fragNor,lightDir))*MatDif;
	vec3 specRefl = 0.3 * pow(max(0,dot(viewDir, reflDir)),shine)*MatSpec;
	color = vec4(ambRefl+diffRefl+specRefl, 1.0);
}
