#pragma once
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	void CalculateBox(std::vector<glm::vec3> vertices);
	void UpdateOuterVertices(float uniformScale, glm::vec3 modelPos);
	float Distance(BoundingBox* box);
	float Distance(glm::vec3 pos);
	glm::vec2 m_xMinMax;
	glm::vec2 m_yMinMax;
	glm::vec2 m_zMinMax;

	float m_width, m_height, m_depth;

	glm::vec3 m_centre;

	std::vector<glm::vec3> m_outerVertices;
};

