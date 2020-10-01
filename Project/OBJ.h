#pragma once
#include "Shader.h"
#include "Cube.h"
#include "BoundingBox.h"



class OBJ
{
public:
	OBJ();
	~OBJ();
	bool LoadOBJ(const char* filepath);	
	void CalculateBoundingBox();
	void UpdateBoundingBox();
	void GenerateBuffers();
	void Draw(Shader * shader);
	void Update();

	void SetPosition(glm::vec3 pos) { m_position = pos; }
	void SetScale(glm::vec3 scale) { m_scale = scale; }
	void SetRotation(glm::vec3 rotation) { m_rotation = rotation; }

	void SetTransforms(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation);

	glm::vec3 GetPosition() { return m_position; }
	glm::vec3 GetScale() { return m_scale; }
	glm::vec3 GetRotation() { return m_rotation; }

	const char* m_filePath;

	BoundingBox* m_bb;

	float m_uniformScale;
private:
	std::vector<glm::vec3>	m_vertices;
	std::vector<glm::vec2>	m_uvs;
	std::vector<glm::vec3>  m_normals;

	GLuint vertexBuffer, uvBuffer, VAO;

	glm::mat4 m_model;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;



};

