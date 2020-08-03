#pragma once
#include "Cube.h"
#include "MyCylinder.h"

class SubComponent
{
public:
	SubComponent();
	~SubComponent();
	void SetTexture(const char* path);
	void Set(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation);
	void Set(SubComponent * sc);
	void Mutate();
	void UniformScale();
	void AxisScale();
	void CopyData(SubComponent * sc);


	Cube* GetCube() { return m_cube; }

private:
	Cube* m_cube;
};

