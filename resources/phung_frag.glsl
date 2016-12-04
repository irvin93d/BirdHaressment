#version 330 core 
out vec4 color;
in vec3 fragNor;
in vec3 reflVec;
in vec3 viewVec;
in vec3 lightVec;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float shine;

uniform int shadow = 0;

float ka = 0.5;
float kd = 0.7;
float ks = 1.0;

void main()
{
	if(shadow == 0){
		vec3 ambRefl = ka * MatAmb;
		vec3 diffRefl = kd * max(0,dot(fragNor,lightVec))*MatDif;
		vec3 specRefl = ks * pow(max(0,dot(viewVec, reflVec)),shine)*MatSpec;
		color = vec4(ambRefl+diffRefl+specRefl, 1.0);
	}
	else{
		color = vec4(0.0113, 0.1735, 0.0025, 1.0);
	}
}
