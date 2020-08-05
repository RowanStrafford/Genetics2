#include "SubComponent.h"

   

SubComponent::SubComponent()
{
	m_cube = new Cube(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0));
}

SubComponent::~SubComponent()
{

}

void SubComponent::SetColour(glm::vec3 colour)
{
	m_colour = colour;
}

void SubComponent::SetColour()
{
	m_colour.r = (rand() % 100) / 100.0f;
	m_colour.g = (rand() % 100) / 100.0f;
	m_colour.b = (rand() % 100) / 100.0f;
}

void SubComponent::SetTexture(const char* path)
{
	m_cube->LoadTexture(path);
}

void SubComponent::Set(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation)
{
	//m_cube->SetPosition(pos);
	m_cube->SetScale(scale);
	m_cube->SetRotation(rotation);

	SetLocalPosition(pos);
}

void SubComponent::Set(SubComponent* sc)
{
	m_cube->SetPosition(sc->GetCube()->GetPosition());
	m_cube->SetScale(sc->GetCube()->GetScale());
	m_cube->SetRotation(sc->GetCube()->GetRotation());
}

void SubComponent::Draw(Shader * shader)
{
	shader->Use();

	shader->SetVec3("colors", m_colour);		

	m_cube->Draw(shader);
}

void SubComponent::CopyData(SubComponent * sc)
{
	m_cube->SetScale(sc->GetCube()->GetScale());
	m_cube->SetRotation(sc->GetCube()->GetRotation());

	m_localPos = sc->m_localPos;
	m_colour = sc->m_colour;
}


/*
THREE TYPES OF MUTATION FOR RECTANGLE
- UNIFORM SCALE
- AXIS SCALE
- VERTEX SCALING

FOUR TYPES OF MUTATION FOR CYLINDER
- UNIFORM SCALE
- VERTEX SCALE
- HEIGHT MODIFICATION
- TOP AND BOTTOM RADII MODIFICATION
*/

void SubComponent::SetGlobalPosition(glm::vec3 parentPos)
{
	m_cube->SetPosition(parentPos + m_localPos);
}

void SubComponent::Mutate()
{
	unsigned int randMutation = rand() % 3;
	
	switch (randMutation)
	{
	// Uniform scale
	case 0:
		UniformScale();
		break;

	// Axis scale
	case 1:
		AxisScale();
		break;

	// Vertex scale
	case 2:
		UniformScale();

		break;

	default:
		break;
	}

}

void SubComponent::UniformScale()
{
	int scaleVal = rand() % 50 - 25;
	float scaleValFloat = scaleVal / 100.0f;

	glm::vec3 cubeScale = m_cube->GetScale();
	cubeScale.x += scaleValFloat;
	cubeScale.y += scaleValFloat;
	cubeScale.z += scaleValFloat;

	m_cube->SetScale(cubeScale);
}

void SubComponent::AxisScale()
{
	int scaleVal = rand() % 50 - 25;
	float scaleValFloat = scaleVal / 100.0f;

	glm::vec3 cubeScale = m_cube->GetScale();

	int axis = rand() % 3;	

	if (axis == 0)		cubeScale.x += scaleValFloat;
	else if (axis == 1)	cubeScale.y += scaleValFloat;
	else				cubeScale.z += scaleValFloat;

	m_cube->SetScale(cubeScale);
}