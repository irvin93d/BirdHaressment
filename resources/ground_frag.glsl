#version 330 core 
out vec4 color;
in vec3 fragNor;
in vec3 reflVec;
in vec3 viewVec;
in vec3 lightVec;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;

float ka = 0.5;
float kd = 0.7;

void main()
{
	vec3 ambRefl = ka * MatAmb;
	vec3 diffRefl = kd * max(0,dot(fragNor,lightVec))*MatDif;
	color = vec4(ambRefl+diffRefl, 1.0);
}
