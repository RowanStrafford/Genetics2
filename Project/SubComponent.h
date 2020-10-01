#pragma once
#include "Cube.h"
#include "MyCylinder.h"
#include "OBJ.h"

class SubComponent
{
public:
	SubComponent();
	SubComponent(Geometry* object);
	~SubComponent();

	void SetOBJ(OBJ * obj);

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

	void Connect(SubComponent * sc);


	Cube* GetCube() { return m_cube; }
	Geometry* GetGeometry() { return m_geometry; }

	int m_index;
	OBJ* m_OBJ;
private:
	Cube* m_cube;
	Geometry* m_geometry;

	

	glm::vec3 m_localPos;
	glm::vec3 m_colour;

};

