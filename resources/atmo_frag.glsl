#version 330 core 
out vec4 color;
in float distance;

void main()
{
	color = vec4(0.7*vec3(0.1,0.6,1)*pow(1/distance,1.2), 1.0);
}
