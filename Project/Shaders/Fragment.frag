#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Texture3;
uniform vec3 colors;

void main()
{	
	FragColor = texture(Texture3, TexCoord) * colors;
}