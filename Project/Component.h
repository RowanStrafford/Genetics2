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
	
	void AddScore(int scoreToAdd) { m_score += scoreToAdd; }
	int GetScore() { return m_score; }

	void SetPosition(glm::vec3 pos) { m_pos = pos; }
	glm::vec3 GetPosition() { return m_pos; }
	
	void ApplyPosition();

	int GetSubComponentSize() { return m_vSubComponents.size(); }

private:
	std::vector<SubComponent*> m_vSubComponents;
	unsigned int m_index;	// 0 for input, 1+ for indexes
	glm::vec3 m_colors;

	int m_score = 0;

	glm::vec3 m_pos;
};

