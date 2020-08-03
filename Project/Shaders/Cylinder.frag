#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Texture1;

void main()
{
	//FragColor = vec4(0.17, 0.17, 0.17, 1.0);
	FragColor = texture(Texture1, TexCoord);
}