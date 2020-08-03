#include "Component.h"

Component::Component(int index)
{
	m_index = index;

	m_colors.r = 87 / 255.0f;
	m_colors.g = 78 / 255.0f;
	m_colors.b = 71 / 255.0f;
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
		shader->SetFloat("ColourR", m_colors.r);
		shader->SetFloat("ColourG", m_colors.g);
		shader->SetFloat("ColourB", m_colors.b);
		m_vSubComponents[i]->GetCube()->Draw(shader);
	}
}

void Component::Update(int index)
{
	for (unsigned int i = 0; i < m_vSubComponents.size(); i++)
	{
		glm::vec3 newPos = m_vSubComponents[i]->GetCube()->GetPosition();
		newPos.x += (index * 5);

		m_vSubComponents[i]->GetCube()->SetPosition(newPos);
		m_vSubComponents[i]->GetCube()->Update();
	}
}

SubComponent* Component::GetSubComponent(int index)
{
	return m_vSubComponents[index];
}

