#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Tex;

void main()
{
    FragColor = texture(Tex, TexCoord);

	if(FragColor.x > 0.5f)
	{
		FragColor.x = FragColor.x + 0.2f;

	} else
	{
		FragColor.x = FragColor.x + 0.3f;
	}

	if(FragColor.y > 0.5f)
	{
		FragColor.y = FragColor.y + 0.1f;
	} 
	else 
	{
		FragColor.z = FragColor.z + 0.3f;
	}

	if(FragColor.z > 0.5f)
	{
		FragColor.z = FragColor.z + 0.1f;
	} 
	else 
	{
		FragColor.z = FragColor.z + 0.1f;
	}

}