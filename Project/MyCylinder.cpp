#include "MyCylinder.h"

MyCylinder::MyCylinder(float baseRadius, float topRadius, float height, int edges)
{
	m_interleavedStride = 32;
	m_height = height;
	m_edges = edges;

	m_baseRadius = baseRadius;
	m_topRadius = topRadius;

	BuildVertices();

	m_position = glm::vec3(0, 0, 0);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_rotation = glm::vec3(0.0f, 1.0f, 0.0f);

	m_model = glm::translate(m_model, m_position);
	m_model = glm::scale(m_model, m_scale);
	SetupGL();

	/*glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);	
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);*/
}

MyCylinder::~MyCylinder()
{

}

void MyCylinder::SetupGL()
{
	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, GetInterleavedVertexSize(), getInterleavedVertices(), GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), getIndices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	int stride = getInterleavedStride();

	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

}

void MyCylinder::LoadTexture(Shader* shader)
{
	//Texture stuff
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("Images/Brick.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);

	shader->SetInt("Texture1", 0);
};




void MyCylinder::CalculateUnitCircleVertices()
{
	const float PI = 3.1415926f;
	float sectorStep = 2 * PI / m_edges;
	float sectorAngle;

	for (int i = 0; i <= m_edges; ++i)
	{
		sectorAngle = i * sectorStep;
		m_unitVertices.push_back(cos(sectorAngle));
		m_unitVertices.push_back(sin(sectorAngle));
		m_unitVertices.push_back(0);
	}
}



void MyCylinder::Create()
{
	CalculateUnitCircleVertices();

	for (int i = 0; i < 2; ++i)
	{
		float h = -m_height / 2.0f + i * m_height;
		float t = 1.0f - i;

		for (int j = 0, k = 0; j < m_edges - 1; ++j, k += 3)
		{
			float ux = m_unitVertices[k];
			float uy = m_unitVertices[k + 1];
			float uz = m_unitVertices[k + 2];

			float ux2 = m_unitVertices[k + 3];
			float uy2 = m_unitVertices[k + 4];
			float uz2 = m_unitVertices[k + 5];

			m_vertices.push_back(ux * m_baseRadius);
			m_vertices.push_back(uy * m_baseRadius);
			m_vertices.push_back(h);

			m_vertices.push_back(ux2 * m_baseRadius);
			m_vertices.push_back(uy2 * m_baseRadius);
			m_vertices.push_back(h);

			m_vertices.push_back(0);
			m_vertices.push_back(0);
			m_vertices.push_back(h);
		}
	}

	int baseCenterIndex = (int)m_vertices.size() / 3;
	int topCenterIndex = baseCenterIndex + m_edges + 1;

	for (int i = 0; i < 2; ++i)
	{
		//float h = -m_height / 2.0f + i * m_height;
		if (i == 1) continue;
		float h = -m_height / 2.0f + 0 * m_height;
		float h2 = -m_height / 2.0f + 1 * m_height;


		float nz = -1 + i * 2;

		m_vertices.push_back(0);	
		m_vertices.push_back(0);
		m_vertices.push_back(h);
	
		for (int j = 0, k = 0; j < m_edges - 1; ++j, k += 3)
		{
			float ux = m_unitVertices[k];
			float uy = m_unitVertices[k + 1];

			float ux2 = m_unitVertices[k + 3];
			float uy2 = m_unitVertices[k + 4];

			// position vector
			m_vertices.push_back(ux * m_baseRadius);             // vx
			m_vertices.push_back(uy * m_baseRadius);             // vy
			m_vertices.push_back(h);                       // vz		

			m_vertices.push_back(ux2 * m_baseRadius);             // vx
			m_vertices.push_back(uy2 * m_baseRadius);             // vy
			m_vertices.push_back(h);     
			
			m_vertices.push_back(ux * m_baseRadius);             // vx
			m_vertices.push_back(uy * m_baseRadius);             // vy
			m_vertices.push_back(h2);  // vz		
		}
	}

	//GenerateTriangles(baseCenterIndex, topCenterIndex);
	Print();
}

void MyCylinder::BuildVertices()
{
	struct Vertex
	{
		float x, y, z, s, t;
	};

	std::vector<Vertex> tempVertices;

	// Indices
	int i, j, k;
	float x, y, z, s, t;
	float radius;

	// Cylinder side
	CalculateUnitCircleVertices();
	
	// calculate temp vertices to store all vertices on top and bottom
	for (i = 0; i <= m_stackCount; ++i)
	{
		z = -(m_height * 0.5f) + (float)i / m_stackCount * m_height;
		radius = m_baseRadius + (float)i / m_stackCount * (m_topRadius - m_baseRadius);
		t = 1.0f - (float)i / m_stackCount;

		for (j = 0, k = 0; j <= m_edges; ++j, k += 3)
		{
			x = m_unitVertices[k];
			y = m_unitVertices[k + 1];
			s = (float)j / m_edges;
			
			Vertex v;
			v.x = x * radius;
			v.y = y * radius;
			v.z = z;
			v.s = s;
			v.t = t;

			tempVertices.push_back(v);
		}
	}

	ClearArrays();

	Vertex v1, v2, v3, v4;
	std::vector<float> n;
	int vi1, vi2;
	int index = 0;

	for (i = 0; i < m_stackCount; ++i)
	{
		vi1 = i * (m_edges + 1);
		vi2 = (i + 1) * (m_edges + 1);

		for (j = 0; j < m_edges; ++j, ++vi1, ++vi2)
		{
			v1 = tempVertices[vi1];
			v2 = tempVertices[vi2];
			v3 = tempVertices[vi1 + 1];
			v4 = tempVertices[vi2 + 1];

			n = ComputeFaceNormal(v1.x, v1.y, v1.z, v3.x, v3.y, v3.z, v2.x, v2.y, v2.z);

			AddVertex(v1.x, v1.y, v1.z);
			AddVertex(v2.x, v2.y, v2.z);
			AddVertex(v3.x, v3.y, v3.z);
			AddVertex(v4.x, v4.y, v4.z);

			AddTexCoords(v1.s, v1.t);
			AddTexCoords(v2.s, v2.t);
			AddTexCoords(v3.s, v3.t);
			AddTexCoords(v4.s, v4.t);

			for (k = 0; k < 4; ++k)
			{
				AddNormal(n[0], n[1], n[2]);
			}

			AddIndices(index, index + 2, index + 1);
			AddIndices(index + 1, index + 2, index + 3);

			index += 4;
		}
	}

	// Base circle
	m_baseIndex = (unsigned int)m_indices.size();
	unsigned int baseVertexIndex = (unsigned int)m_vertices.size() / 3;

	z = -m_height * 0.5f;
	AddVertex(0, 0, z);
	AddNormal(0, 0, -1);
	AddTexCoords(0.5f, 0.5f);

	for (i = 0, j = 0; i < m_edges; ++i, j += 3)
	{
		x = m_unitVertices[j];
		y = m_unitVertices[j + 1];
		AddVertex(x * m_baseRadius, y * m_baseRadius, z);
		AddNormal(0, 0, -1);
		AddTexCoords(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
	}

	for (i = 0, k = baseVertexIndex + 1; i < m_edges; ++i, ++k)
	{
		if (i < m_edges - 1)
			AddIndices(baseVertexIndex, k + 1, k);
		else
			AddIndices(baseVertexIndex, baseVertexIndex + 1, k);
	}

	// Top circle
	m_topIndex = (unsigned int)m_indices.size();
	unsigned int topVertexIndex = (unsigned int)m_vertices.size() / 3;

	z = m_height * 0.5f;
	AddVertex(0, 0, z);
	AddNormal(0, 0, 1);
	AddTexCoords(0.5f, 0.5f);

	for (i = 0, j = 0; i < m_edges; ++i, j += 3)
	{
		x = m_unitVertices[j];
		y = m_unitVertices[j + 1];
		AddVertex(x * m_topRadius, y * m_topRadius, z);
		AddNormal(0, 0, 1);
		AddTexCoords(x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
	}

	for (i = 0, k = topVertexIndex + 1; i < m_edges; ++i, ++k)
	{
		if (i < m_edges - 1)
			AddIndices(topVertexIndex, k, k + 1);
		else
			AddIndices(topVertexIndex, k, topVertexIndex + 1);
	}

	BuildInterleavedvertices();
}

std::vector<float> MyCylinder::ComputeFaceNormal(float x1, float y1, float z1,  // v1
	float x2, float y2, float z2,  // v2
	float x3, float y3, float z3)  // v3
{
	const float EPSILON = 0.000001f;

	std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
	float nx, ny, nz;

	// find 2 edge vectors: v1-v2, v1-v3
	float ex1 = x2 - x1;
	float ey1 = y2 - y1;
	float ez1 = z2 - z1;
	float ex2 = x3 - x1;
	float ey2 = y3 - y1;
	float ez2 = z3 - z1;

	// cross product: e1 x e2
	nx = ey1 * ez2 - ez1 * ey2;
	ny = ez1 * ex2 - ex1 * ez2;
	nz = ex1 * ey2 - ey1 * ex2;

	// normalize only if the length is > 0
	float length = sqrtf(nx * nx + ny * ny + nz * nz);
	if (length > EPSILON)
	{
		// normalize
		float lengthInv = 1.0f / length;
		normal[0] = nx * lengthInv;
		normal[1] = ny * lengthInv;
		normal[2] = nz * lengthInv;
	}

	return normal;
}

void MyCylinder::ClearArrays()
{
	std::vector<float>().swap(m_vertices);
	std::vector<float>().swap(m_normals);
	std::vector<float>().swap(m_texCoords);
	std::vector<unsigned int>().swap(m_indices);
}

void MyCylinder::GenerateTriangles(int baseCenter, int topCenter)
{
	int k1 = 0;
	int k2 = m_edges + 1;

	for (int i = 0; i < m_edges; ++i, ++k1, ++k2)
	{
		m_indices.push_back(k1);
		m_indices.push_back(k1 + 1);
		m_indices.push_back(k2);

		m_indices.push_back(k2);
		m_indices.push_back(k1 + 1);
		m_indices.push_back(k1 + 2);
	}

	for (int i = 0, k = baseCenter + 1; i < m_edges; ++i, ++k)
	{
		if (i < m_edges - 1)
		{
			m_indices.push_back(baseCenter);
			m_indices.push_back(k + 1);
			m_indices.push_back(k);
		}
		else
		{
			m_indices.push_back(baseCenter);
			m_indices.push_back(baseCenter + 1);
			m_indices.push_back(k);
		}
	}

	for (int i = 0, k = topCenter + 1; i < m_edges; ++i, ++k)
	{
		if (i < m_edges - 1)
		{
			m_indices.push_back(topCenter);
			m_indices.push_back(k);
			m_indices.push_back(k + 1);
		}
		else
		{
			m_indices.push_back(topCenter);
			m_indices.push_back(k);
			m_indices.push_back(topCenter + 1);
		}
	}
}

void MyCylinder::Draw(Shader* shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader->Use();
	shader->SetMatrix("model", m_model);
	glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, (void*)0);

}

void MyCylinder::Update(float deltaTime)
{
	m_model = glm::rotate(m_model, m_rotationValue, m_rotation);
}

void MyCylinder::BuildInterleavedvertices()
{
	std::size_t i, j;
	std::size_t count = m_vertices.size();

	for (i = 0, j = 0; i < count - 3; i += 3, j += 2)
	{
		m_interleavedVertices.insert(m_interleavedVertices.end(), &m_vertices[i], &m_vertices[i] + 3);
		m_interleavedVertices.insert(m_interleavedVertices.end(), &m_normals[i], &m_normals[i] + 3);
		m_interleavedVertices.insert(m_interleavedVertices.end(), &m_texCoords[j], &m_texCoords[j] + 2);
	}
}

void MyCylinder::AddVertex(float x, float y, float z)
{
	m_vertices.push_back(x);
	m_vertices.push_back(y);
	m_vertices.push_back(z);
}

void MyCylinder::AddNormal(float x, float y, float z)
{
	m_normals.push_back(x);
	m_normals.push_back(y);
	m_normals.push_back(z);
}

void MyCylinder::AddTexCoords(float s, float t)
{
	m_texCoords.push_back(s);
	m_texCoords.push_back(t);
}

void MyCylinder::AddIndices(unsigned int index1, unsigned int index2, unsigned int index3)
{
	m_indices.push_back(index1);
	m_indices.push_back(index2);
	m_indices.push_back(index3);
}

void MyCylinder::Print()
{
	for (int i = 0; i < m_vertices.size() - 2; i += 3)
	{
		std::cout << Round(m_vertices[i] * 1000.0f) / 1000.0f << " , " << Round(m_vertices[i + 1] * 1000.0f) / 1000.0f << " , " << Round(m_vertices[i + 2] * 1000.0f) / 1000.0f << std::endl;
	}
}

float MyCylinder::Round(float val)
{
	if (val < 0) return ceil(val - 0.5f);
	else return floor(val + 0.5f);
}


