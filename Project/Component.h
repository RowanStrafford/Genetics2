#pragma once
#include "SubComponent.h"

class Component
{
public:
	Component(int index);
	~Component();

	void AddComponent(SubComponent * sc);

	void Draw(Shader* shader);

	void Update(int index);

	SubComponent* GetSubComponent(int index);
	

private:
	std::vector<SubComponent*> m_vSubComponents;
	unsigned int m_index;	// 0 for input, 1+ for indexes
	glm::vec3 m_colors;
};

