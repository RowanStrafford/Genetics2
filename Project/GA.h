#pragma once
#include "Cube.h"
#include "Shader.h"
#include "Component.h"
#include "Camera.h"

#include <vector>

class GA
{
public:
	GA();
	~GA();

	void SetInputModel(Component* model);
	void SetInputModel(SubComponent * model);
	void GenerateInitialPopulation(int populationSize);
	void DisplayPopulation(Shader* shader, Camera* cam);
	void UpdatePopulation();
	// Selection of population
	void Crossover();
	void Mutation();
	void IncrimentDisplay(Camera* cam);
	void RotateCurrentModel(glm::vec3 dir, float delta);
private:
	//std::vector<Cube*> m_vPopulation;
	//std::vector<SubComponent*> m_vPopulation;
	std::vector<Component*> m_vPopulation;
	Component* m_inputModel;
	SubComponent* m_inputModelSC;

	SubComponent* m_sc;
	int m_populationSize;
	int m_displayIndex = 0;
};

