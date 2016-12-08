#version 330 core
uniform sampler2D Texture;

in vec2 vTexCoord;
in vec3 fragNor;
in vec3 reflDir;
in vec3 viewDir;
in vec3 lightDir;

out vec4 Outcolor;

void main() {
	vec4 texColor = texture(Texture, vTexCoord);

	vec3 ambRefl = 0.15*vec3(1.0,1.0,1.0);
	vec3 diffRefl = 2*max(0,dot(fragNor,lightDir))*vec3(1.0,1.0,1.0);
	vec3 specRefl = 1.0 * pow(max(0,dot(viewDir, reflDir)),10)*vec3(1.0,1.0,1.0);
	vec3 color = ambRefl + diffRefl + specRefl;

	Outcolor = vec4(texColor.r*color.r, texColor.g*color.g, texColor.b*color.b, 1);
	//Outcolor = vec4(color.r, color.g, color.b, 1);
}

