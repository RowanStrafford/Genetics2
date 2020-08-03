#version 330 core
out vec4 FragColor;

uniform float redVal;

void main()
{
	FragColor = vec4(redVal, 0.17, 0.17, 1.0);
}