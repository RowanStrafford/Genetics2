#include "Cube.h"



Cube::Cube(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation)
{
	for (unsigned int i = 0; i < 216; i++)
	{
		testVec.push_back(m_vertices[i]);
	}

	m_position = pos;
	m_scale = scale;
	m_rotation = rotation;

	m_model = glm::translate(m_model, m_position);
	m_model = glm::scale(m_model, m_scale);
	m_model = glm::rotate(m_model, 0.0f, m_rotation);

	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * testVec.size(), &testVec[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Cube::LoadTexture(const char* path)
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
};


Cube::~Cube()
{
}

void Cube::SetIndex(int index)
{
	m_index = index;
}

void Cube::Draw(Shader* shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);	

	glBindVertexArray(VAO);

	shader->SetMatrix("model", m_model);

	glDrawArrays(GL_TRIANGLES, 0, 36);

}

void Cube::Update(float deltaTime)
{
	Rotate(deltaTime);
}

void Cube::Update()
{
	m_model = glm::translate(m_model, m_position);
	m_model = glm::scale(m_model, m_scale);
	m_model = glm::rotate(m_model, glm::radians(1.0f), m_rotation);
}

void Cube::Rotate(float deltaTime)
{
	m_model = glm::rotate(m_model, glm::radians(m_rotationVal) * 50.0f * deltaTime, m_rotation);
}

void Cube::Translate(float deltaTime)
{
	m_model = glm::translate(m_model, m_position);
}


