#pragma once
#include "SubComponent.h"

class Component
{
public:
	Component(int index);
	~Component();

	void AddComponent(SubComponent * sc);
	void Draw(Shader* shader);
	void Update();

	SubComponent* GetSubComponent(int index);
	
	void AddScore(int scoreToAdd) { m_score += scoreToAdd; }
	int GetScore() { return m_score; }

	void SetGlobalPosition(glm::vec3 pos) { m_globalPos = pos; }
	glm::vec3 GetGlobalPosition() { return m_globalPos; }
	
	void ApplyPosition();

	int GetSubComponentSize() { return m_vSubComponents.size(); }

private:
	std::vector<SubComponent*> m_vSubComponents;

	unsigned int m_index;	// 0 for input, 1+ for indexes

	int m_score = 0;

	glm::vec3 m_globalPos;
};

