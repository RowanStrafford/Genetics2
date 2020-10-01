#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Texture3;
uniform vec3 colors;

void main()
{	
	FragColor = vec4(colors.x, colors.y, colors.z, 1.0f);
}