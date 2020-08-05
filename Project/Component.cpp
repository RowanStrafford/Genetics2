#include "Component.h"

Component::Component(int index)
{
	m_index = index;

	//m_colors.r = (rand() % 100) / 100.0f;
	//m_colors.g = (rand() % 100) / 100.0f;
	//m_colors.b = (rand() % 100) / 100.0f;
}

Component::~Component()
{
}

void Component::AddComponent(SubComponent* sc)
{
	m_vSubComponents.push_back(sc);
}

void Component::Draw(Shader* shader)
{
	for (unsigned int i = 0; i < m_vSubComponents.size(); i++)
	{
		m_vSubComponents[i]->Draw(shader);
	}
}

void Component::Update(int index)
{
	for (unsigned int i = 0; i < m_vSubComponents.size(); i++)
	{
		//glm::vec3 newPos = m_vSubComponents[i]->GetCube()->GetPosition();
		//newPos.x += (index * 5);

		//m_vSubComponents[i]->GetCube()->SetPosition(newPos);

		m_vSubComponents[i]->SetGlobalPosition(m_pos);
		m_vSubComponents[i]->GetCube()->Update();
	}
}

SubComponent* Component::GetSubComponent(int index)
{
	return m_vSubComponents[index];
}

void Component::ApplyPosition()
{
	//m_vSubComponents[0].s
}

