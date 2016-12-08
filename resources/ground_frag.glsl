#version 330 core
uniform sampler2D Texture;

in vec2 vTexCoord;
in vec3 vColor;

out vec4 Outcolor;

void main() {
	vec4 BaseColor = vec4(vColor.x, vColor.y, vColor.z, 1);
	vec4 texColor2 = texture(Texture, vTexCoord);

	Outcolor = vec4(texColor2.r*vColor.r, texColor2.g*vColor.g, texColor2.b*vColor.b, 1);
}

