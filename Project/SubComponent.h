#pragma once
#include "Cube.h"
#include "MyCylinder.h"

class SubComponent
{
public:
	SubComponent();
	~SubComponent();

	void SetColour(glm::vec3 colour);
	void SetColour();

	void SetTexture(const char* path);
	void Set(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation);
	void Set(SubComponent * sc);
	void Draw(Shader* shader);
	void SetGlobalPosition(glm::vec3 parentPos);

	void SetLocalPosition(glm::vec3 pos) { m_localPos = pos; }
	glm::vec3 GetLocalPosition() { return m_localPos; }
	void Mutate();
	void UniformScale();
	void AxisScale();
	void CopyData(SubComponent * sc);


	Cube* GetCube() { return m_cube; }

private:
	Cube* m_cube;

	glm::vec3 m_localPos;
	glm::vec3 m_colour;

};

