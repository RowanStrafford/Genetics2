#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Geometry.h"

class MyCylinder : public Geometry
{
public:
	MyCylinder(float baseRadius, float topRadius, float height, int edges);
	~MyCylinder();

	virtual void Draw(Shader* shader);

	virtual void SetupGL();



	void CalculateUnitCircleVertices();
	void BuildVertices();
	std::vector<float> ComputeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	void ClearArrays();
	void GenerateTriangles(int baseCenter, int topCenter);

	void Update(float deltaTime);
	void BuildInterleavedvertices();

	// inline functions
	unsigned int GetVertexCount() const { return (unsigned int)m_vertices.size() / 3; }
	unsigned int GetInterleavedVertexCount() const { return GetVertexCount(); }
	unsigned int GetInterleavedVertexSize() const { return (unsigned int)m_interleavedVertices.size() * sizeof(unsigned int); }
	const float* getInterleavedVertices() const { return &m_interleavedVertices[0]; }
	unsigned int getIndexSize() const { return (unsigned int)m_indices.size() * sizeof(unsigned int); }
	const unsigned int* getIndices() const { return m_indices.data(); }
	unsigned int getIndexCount() const { return (unsigned int)m_indices.size(); }
	int getInterleavedStride() const { return m_interleavedStride; }   // should be 32 bytes
	std::vector<float> GetVertices() { return m_vertices; }
	std::vector<float> GetUnitVertices() { return m_unitVertices; }

	void AddVertex(float x, float y, float z);
	void AddNormal(float x, float y, float z);
	void AddTexCoords(float s, float t);
	void AddIndices(unsigned int index1, unsigned int index2, unsigned int index3);

	float Round(float val);

	void CopyData(MyCylinder * cylinder);

private:
	float m_height;
	int m_edges;
	int m_stackCount = 1;
	float m_baseRadius, m_topRadius;
	unsigned int m_baseIndex;                 
	unsigned int m_topIndex;    

	float m_rotationValue;

	GLuint IBO;

	std::vector<float> m_vertices;	
	std::vector<float> m_normals;
	std::vector<float> m_texCoords;
	std::vector<unsigned int> m_indices;

	std::vector<float> m_unitVertices;
	std::vector<float> m_interleavedVertices;
	int m_interleavedStride;  
};

