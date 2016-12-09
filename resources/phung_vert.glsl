#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPos;

out vec3 fragNor;
out vec3 reflDir;
out vec3 lightDir;
out vec3 viewDir;

void main()
{
	vec3 normal = normalize(mat3(M)*vertNor);

	lightDir = normalize(lightPos - (M*vertPos).xyz);
	reflDir = normalize(mat3(V)*2*dot(lightDir,normal)*normal-lightDir);
	viewDir = normalize(-(V*M*vertPos).xyz);
	
	fragNor = normal;
	gl_Position = P * V * M * vertPos;
}

