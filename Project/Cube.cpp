#include "Cube.h"



Cube::Cube()
{
	CreateVertices();

	SetupGL();
}

Cube::~Cube()
{
}

void Cube::SetupGL()
{
	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vVertices.size(), &m_vVertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
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

	shader->SetVec3("colors", glm::vec3(1.0f, 1.0f, 0.0f));
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

void Cube::CopyData(Cube* cube)
{

}

void Cube::CreateVertices()
{
	m_vVertices = 
					{	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
						 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
						 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
						 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
						-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
						-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

						-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
						 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
						 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
						 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
						-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
						-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

						-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
						-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
						-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
						-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
						-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
						-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

						 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
						 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
						 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
						 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
						 0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
						 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

						-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
						 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
						 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
						 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
						-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
						-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

						-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
						 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
						 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
						 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
						-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
						-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
														};
}


