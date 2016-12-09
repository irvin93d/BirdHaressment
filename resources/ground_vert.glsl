#version  330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPos;

out vec2 vTexCoord;
out vec3 fragNor;
out vec3 lightDir;
out vec3 reflDir;
out vec3 viewDir;

void main() {

	vec3 normal = (M  * vec4(vertNor, 0.0)).xyz;

	lightDir = normalize(lightPos - (M*vec4(vertPos,1)).xyz);
	reflDir = normalize(mat3(V)*(2*dot(lightDir,normal)*normal-lightDir));
	viewDir = normalize(-(V*M*vec4(vertPos,1)).xyz);
	
	/* pass through the texture coordinates to be interpolated */
	vTexCoord = vertTex;

	fragNor = normal;
	gl_Position = P * V * M * vec4(vertPos.xyz, 1.0);
}
