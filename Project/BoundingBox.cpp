#include "BoundingBox.h"



BoundingBox::BoundingBox()
{
}


BoundingBox::~BoundingBox()
{
}

void BoundingBox::CalculateBox(std::vector<glm::vec3> vertices)
{
	m_xMinMax = glm::vec2(vertices[0].x, vertices[0].x);
	m_yMinMax = glm::vec2(vertices[0].y, vertices[0].y);
	m_zMinMax = glm::vec2(vertices[0].z, vertices[0].z);

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		float x = vertices[i].x;
		float y = vertices[i].y;
		float z = vertices[i].z;

		if (x < m_xMinMax.x) m_xMinMax.x = x;
		if (x > m_xMinMax.y) m_xMinMax.y = x;

		if (y < m_yMinMax.x) m_yMinMax.x = y;
		if (y > m_yMinMax.y) m_yMinMax.y = y;

		if (z < m_zMinMax.x) m_zMinMax.x = z;
		if (z > m_zMinMax.y) m_zMinMax.y = z;
	}

	m_width =	m_xMinMax.y - m_xMinMax.x;
	m_height =	m_yMinMax.y - m_yMinMax.x;
	m_depth =	m_zMinMax.y - m_zMinMax.x;

	m_centre.x = m_xMinMax.x + (m_width / 2.0f);
	m_centre.y = m_yMinMax.x + (m_height / 2.0f);
	m_centre.z = m_zMinMax.x + (m_depth / 2.0f);

	m_outerVertices.push_back(glm::vec3(m_xMinMax.x, m_yMinMax.x, m_zMinMax.x));	// Bottom Left
	m_outerVertices.push_back(glm::vec3(m_xMinMax.x, m_yMinMax.y, m_zMinMax.x));	// Top Left
	m_outerVertices.push_back(glm::vec3(m_xMinMax.y, m_yMinMax.y, m_zMinMax.x));	// Top Right
	m_outerVertices.push_back(glm::vec3(m_xMinMax.y, m_yMinMax.x, m_zMinMax.x));	// Bottom Right
	m_outerVertices.push_back(glm::vec3(m_xMinMax.x, m_yMinMax.y, m_zMinMax.y));
	m_outerVertices.push_back(glm::vec3(m_xMinMax.y, m_yMinMax.y, m_zMinMax.y));
	m_outerVertices.push_back(glm::vec3(m_xMinMax.y, m_yMinMax.x, m_zMinMax.y));
	m_outerVertices.push_back(glm::vec3(m_xMinMax.x, m_yMinMax.x, m_zMinMax.y));
}

void BoundingBox::UpdateOuterVertices(float uniformScale, glm::vec3 modelPos)
{
	std::vector<glm::vec3> scaledOuterVertices;

	for (unsigned int i = 0; i < m_outerVertices.size(); i++)
	{
		glm::vec3 dir = (m_outerVertices[i] - m_centre) * uniformScale;
		scaledOuterVertices.push_back(m_centre + dir);
	}	

	m_outerVertices = scaledOuterVertices;

	m_width = m_outerVertices[2].x - m_outerVertices[1].x;
	m_height = m_outerVertices[1].y - m_outerVertices[0].y;
	m_depth = m_outerVertices[4].z - m_outerVertices[0].z;

	m_centre = modelPos;
	//m_centre.x = m_outerVertices[0].x + (m_width / 2.0f);
	//m_centre.y = m_outerVertices[0].y + (m_height / 2.0f);
	//m_centre.z = m_outerVertices[0].z + (m_depth / 2.0f);
}

float BoundingBox::Distance(BoundingBox * box)
{
	return glm::distance(m_centre, box->m_centre);	
}

float BoundingBox::Distance(glm::vec3 pos)
{
	return glm::distance(m_centre, pos);	 
}
