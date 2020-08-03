#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Texture3;

uniform float ColourR;
uniform float ColourG;
uniform float ColourB;

void main()
{	
	FragColor = vec4(ColourR, ColourG, ColourB, 1.0f);
}