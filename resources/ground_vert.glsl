#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPos;

out vec3 fragNor;
out vec3 lightVec;

void main()
{
	vec3 normal = normalize(mat3(M)*vertNor);
	lightVec = normalize(lightPos - (M*vertPos).xyz);
	
	fragNor = normal;
	
	gl_Position = P * V * M * vertPos;
}

