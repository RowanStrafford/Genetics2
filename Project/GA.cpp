#include "GA.h"

#include <stdlib.h>     
#include <time.h>  

GA::GA()
{
}


GA::~GA()
{
}

void GA::SetInputModel(Component* component)
{
	m_inputModel = component;
}

void GA::SetInputModel(SubComponent* model)
{
	m_inputModelSC = model;
}

void GA::GenerateInitialPopulation(int populationSize)
{
	srand(time(NULL));

	m_populationSize = populationSize;

	for (unsigned int i = 0; i < m_populationSize; i++)
	{
		SubComponent* newSC1 = new SubComponent();
		newSC1->CopyData(m_inputModel->GetSubComponent(0));
		newSC1->SetTexture("Images/Brick.jpg");
		newSC1->Mutate();

		SubComponent* newSC2 = new SubComponent();
		newSC2->CopyData(m_inputModel->GetSubComponent(1));
		newSC2->SetTexture("Images/Brick.jpg");
		newSC2->Mutate();

		Component* comp = new Component(i + 1);
		comp->AddComponent(newSC1);
		comp->AddComponent(newSC2);

		m_vPopulation.push_back(comp);
		//m_vPopulation.push_back(newSC);
	}

	UpdatePopulation();
}

void GA::DisplayPopulation(Shader* shader, Camera* cam)
{
	for (unsigned int i = 0; i < m_populationSize; i++)
	{
		//m_vPopulation[i]->GetCube()->Draw(shader);
		m_vPopulation[i]->Draw(shader);
	}
}

void GA::UpdatePopulation()
{
	for (unsigned int i = 0; i < m_populationSize; i++)
	{
		//m_vPopulation[i]->GetCube()->Update();
		m_vPopulation[i]->Update(i);
	}
}

void GA::Crossover()
{
	// Loop through population and repeatedly produce new offspring
}

void GA::Mutation()
{
	// Simply loop through new population and mutate each member random num < mutationRate
}

void GA::IncrimentDisplay(Camera* cam)
{
	m_displayIndex++;

	if (m_displayIndex >= m_vPopulation.size()) m_displayIndex = 0;

	glm::vec3 newPos = m_vPopulation[m_displayIndex]->GetSubComponent(0)->GetCube()->GetPosition();
	newPos.z += 1.5f;
	newPos.y += 0.5f;

	cam->SetPosition(newPos);
}

void GA::RotateCurrentModel(glm::vec3 dir, float delta)
{
	// This function will call 'Rotate' within the component class in future

	m_vPopulation[m_displayIndex]->GetSubComponent(0)->GetCube()->SetRotation(dir);
	m_vPopulation[m_displayIndex]->GetSubComponent(1)->GetCube()->SetRotation(dir);

	m_vPopulation[m_displayIndex]->GetSubComponent(0)->GetCube()->m_rotationVal = 1.0f;
	m_vPopulation[m_displayIndex]->GetSubComponent(1)->GetCube()->m_rotationVal = 1.0f;

	m_vPopulation[m_displayIndex]->GetSubComponent(0)->GetCube()->Update(delta);
	m_vPopulation[m_displayIndex]->GetSubComponent(1)->GetCube()->Update(delta);
}

