#pragma once
#include "Shader.h"
#include <vector>
#include "stb_image.h"

class Geometry
{
public:
	Geometry();
	~Geometry();

	virtual void SetupGL();
	virtual void Draw(Shader * shader);

	void LoadTexture(const char * path);

	void SetPosition(glm::vec3 pos) { m_position = pos; }
	void SetScale(glm::vec3 scale) { m_scale = scale; }
	void SetRotation(glm::vec3 rotation) { m_rotation = rotation; }

	// Getters
	glm::vec3 GetPosition() { return m_position; }
	glm::vec3 GetScale() { return m_scale; }
	glm::vec3 GetRotation() { return m_rotation; }

	virtual void UpdateModel();
	virtual void CreateVertices();

protected:
	GLuint VBO, VAO;

	glm::mat4 m_model;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;

	std::vector<float> m_vVertices;

	unsigned int texture;
};

