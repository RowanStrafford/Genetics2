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
		glm::vec3 newColour;
		newColour.r = (rand() % 100) / 100.0f;
		newColour.g = (rand() % 100) / 100.0f;
		newColour.b = (rand() % 100) / 100.0f;

		SubComponent* newSC1 = new SubComponent();
		newSC1->CopyData(m_inputModel->GetSubComponent(0));
		newSC1->SetTexture("Images/Brick2.png");
		newSC1->Mutate();
		newSC1->SetColour(newColour);

		SubComponent* newSC2 = new SubComponent();
		newSC2->CopyData(m_inputModel->GetSubComponent(1));
		newSC2->SetTexture("Images/Brick2.png");
		newSC2->Mutate();
		newSC2->SetColour(newColour);

		Component* comp = new Component(i + 1);
		comp->AddComponent(newSC1);
		comp->AddComponent(newSC2);
		comp->SetPosition(glm::vec3(i * 5, 0, 0));

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

void GA::Selection()
{
	std::vector<int> weightedScores;

	for (unsigned int i = 0; i < m_populationSize; i++)
	{
		int score = m_vPopulation[i]->GetScore();

		for (unsigned int j = 0; j < score; j++)
		{
			weightedScores.push_back(i);
		}
	}

	std::vector<Component*> tempComponents(m_populationSize);

	for (int index = 0; index < m_populationSize; index++)
	{
		std::vector<int> tempWeights = weightedScores;

		int randomScore1 = rand() % tempWeights.size();
		int randomIndex1 = tempWeights[randomScore1];

		int startIndex, endIndex;

		for (unsigned int i = 0; i < tempWeights.size(); i++)
		{
			if (tempWeights[i] == randomIndex1)
			{
				startIndex = i;

				for (unsigned int j = startIndex; j < tempWeights.size(); j++)
				{
					if (j == tempWeights.size() - 1)
					{
						endIndex = tempWeights.size();
						break;
					}

					if (tempWeights[j] != randomIndex1)
					{
						endIndex = j;
						break;
					}
				}
				break;
			}
		}

		tempWeights.erase(tempWeights.begin() + startIndex, tempWeights.begin() + endIndex);
		int randomScore2 = rand() % tempWeights.size();
		int randomIndex2 = tempWeights[randomScore2];

		tempComponents[index] = Crossover(randomIndex1, randomIndex2, index);
	}	

	m_vPopulation = tempComponents;
	UpdatePopulation();
}

Component* GA::Crossover(int index1, int index2, int arrIndex)
{
	Component* newComp = new Component(arrIndex);

	SubComponent* sc1 = new SubComponent();
	SubComponent* sc2 = new SubComponent();

	sc1->CopyData(m_vPopulation[index1]->GetSubComponent(0));
	sc2->CopyData(m_vPopulation[index2]->GetSubComponent(1));

	sc1->SetTexture("Images/Brick2.png");
	sc2->SetTexture("Images/Brick2.png");

	newComp->AddComponent(sc1);
	newComp->AddComponent(sc2);
	newComp->SetPosition(glm::vec3(arrIndex * 5, 0, 0));

	std::cout << "New child created!" << std::endl;
	std::cout << "Parent : " << index1 << std::endl;
	std::cout << "Parent : " << index2 << std::endl << std::endl;

	return newComp;
}

void GA::Mutation()
{
	// Simply loop through new population and mutate each member random num < mutationRate
}

void GA::IncrimentDisplay(Camera* cam)
{
	m_displayIndex++;

	if (m_displayIndex >= m_vPopulation.size())
	{
		m_displayIndex = 0;
		Selection();
	}


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

void GA::AddScore(int scoreToAdd)
{
	m_vPopulation[m_displayIndex]->AddScore(scoreToAdd);
}

