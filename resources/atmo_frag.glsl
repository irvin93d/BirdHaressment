#version 330 core 
out vec4 color;
in float distance;

void main()
{
	color = vec4(vec3(0.1,0.6,1)*sqrt(1/distance), 1.0);
}
