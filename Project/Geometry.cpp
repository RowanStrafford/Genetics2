#include "Geometry.h"

Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

void Geometry::SetupGL()
{
}

void Geometry::Draw(Shader* shader)
{

}

void Geometry::LoadTexture(const char* path)
{
	//Texture stuff
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);
}

void Geometry::UpdateModel()
{
	m_model = glm::translate(m_model, m_position);
	m_model = glm::scale(m_model, m_scale);
	m_model = glm::rotate(m_model, glm::radians(0.0f), m_rotation);
}

void Geometry::CreateVertices()
{
	// Create vertices in here
}

