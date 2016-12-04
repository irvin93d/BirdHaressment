#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPos;
uniform int shadow = 0;

out vec3 fragNor;
out vec3 reflVec;
out vec3 viewVec;
out vec3 lightVec;

void main()
{
	vec3 normal = normalize(mat3(M)*vertNor);
	lightVec = normalize(lightPos - (M*vertPos).xyz);
	
	reflVec = normalize(mat3(V)*(2*dot(lightVec,normal)*normal-lightVec));

	viewVec = normalize(-(V*M*vertPos).xyz);
	fragNor = normal;
	
	if(shadow == 0)
		gl_Position = P * V * M * vertPos;
	else{
		vec4 MPos = M * vertPos;
		float k = (0.01 - lightPos.y)/(lightVec.y);
		gl_Position = P * V * vec4(k*(lightVec) + lightPos,1);
	}
}

