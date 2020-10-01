#include "OBJ.h"



OBJ::OBJ()
{
	m_uniformScale = 1.0f;
}


OBJ::~OBJ()
{
}

bool OBJ::LoadOBJ(const char * filepath)
{
	m_filePath = filepath;

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	errno_t err;
	FILE* filepoint;

	err = fopen_s(&filepoint, filepath, "r");

	if (filepoint != NULL)
	{
		while (1)
		{
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(filepoint, "%s", lineHeader);
			if (res == EOF)
				break;

			if (strcmp(lineHeader, "v") == 0) {
				glm::vec3 vertex;
				fscanf(filepoint, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf(filepoint, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf(filepoint, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(filepoint, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}

		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			glm::vec3 normal = temp_normals[normalIndex - 1];

			m_vertices.push_back(vertex);
			m_uvs.push_back(uv);
			m_normals.push_back(normal);
		}

		fclose(filepoint);
	}

	CalculateBoundingBox();
	return true;
}


void OBJ::CalculateBoundingBox()
{
	m_bb = new BoundingBox();

	m_bb->CalculateBox(m_vertices);	
	m_bb->UpdateOuterVertices(m_uniformScale, m_position);
}

void OBJ::UpdateBoundingBox()
{
	for (unsigned int i = 0; i < m_vertices.size(); i++)
	{

	}
}

void OBJ:: GenerateBuffers()
{
	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);
}

void OBJ::Draw(Shader* shader)
{
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//glBindVertexArray(vertexBuffer);

	shader->SetMatrix("model", m_model);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

void OBJ::Update()
{
	m_model = glm::translate(m_model, m_position);
	m_model = glm::scale(m_model, m_scale);
	m_model = glm::rotate(m_model, glm::radians(1.0f), m_rotation);
}

void OBJ::SetTransforms(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation)
{
	m_position = pos;
	m_scale = scale; 
	m_rotation = rotation; 
}
