#include "SubComponent.h"

   

SubComponent::SubComponent()
{
	//m_cube = new Cube();
	//m_geometry = new Geometry();	
	m_OBJ = new OBJ();
}

SubComponent::SubComponent(Geometry* object)
{
	m_geometry = object;
}


SubComponent::~SubComponent()
{

}

void SubComponent::SetOBJ(OBJ* obj)
{
	m_OBJ = obj;
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
	// m_cube->LoadTexture(path);

	m_geometry->LoadTexture(path);
}

void SubComponent::Set(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation)
{
	//m_cube->SetScale(scale);
	//m_cube->SetRotation(rotation);

	//SetLocalPosition(pos);

	//m_geometry->SetScale(scale);
	//m_geometry->SetRotation(rotation);

	m_OBJ->SetScale(scale);
	m_OBJ->SetRotation(rotation);

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

	//m_cube->Draw(shader);
	
	//m_geometry->Draw(shader);
	m_OBJ->Draw(shader);
}

void SubComponent::CopyData(SubComponent * sc)
{
	//m_cube->SetScale(sc->GetCube()->GetScale());
	//m_cube->SetRotation(sc->GetCube()->GetRotation());	

	m_OBJ = new OBJ();
	m_OBJ->LoadOBJ(sc->m_OBJ->m_filePath);
	m_OBJ->GenerateBuffers();

	m_OBJ->SetScale(sc->m_OBJ->GetScale());
	m_OBJ->SetRotation(sc->m_OBJ->GetRotation());
	m_OBJ->SetPosition(sc->m_OBJ->GetPosition());
	//m_geometry->SetScale(sc->GetGeometry()->GetScale());
	//m_geometry->SetRotation(sc->GetGeometry()->GetRotation());

	m_localPos = sc->m_localPos;
	m_colour = sc->m_colour;
}

void SubComponent::Connect(SubComponent* sc)
{
	float dist = m_OBJ->m_bb->Distance(sc->m_OBJ->m_bb);

	float movement = dist - (m_OBJ->m_bb->m_height / 2.0f) - (sc->m_OBJ->m_bb->m_height / 2.0f);

	m_OBJ->SetPosition(m_OBJ->GetPosition() - glm::vec3(0, movement, 0));
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
	//m_cube->SetPosition(parentPos + m_localPos);

	//m_geometry->SetPosition(parentPos + m_localPos);

	m_OBJ->SetPosition(parentPos + m_localPos);
}

void SubComponent::Mutate()
{
	unsigned int randMutation = rand() % 3;
	randMutation = 0;

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
	int scaleVal = rand() % 100 - 50;
	float scaleValFloat = scaleVal / 100.0f;

	glm::vec3 scale = m_OBJ->GetScale();
	scale.x += scaleValFloat;
	scale.y += scaleValFloat;
	scale.z += scaleValFloat;

	m_OBJ->m_uniformScale = m_OBJ->m_uniformScale + scaleValFloat;
	m_OBJ->SetScale(scale);

	m_OBJ->CalculateBoundingBox();
}

void SubComponent::AxisScale()
{
	int scaleVal = rand() % 50 - 25;
	float scaleValFloat = scaleVal / 100.0f;

	glm::vec3 scale = m_OBJ->GetScale();

	int axis = rand() % 3;	

	if (axis == 0)		scale.x += scaleValFloat;
	else if (axis == 1)	scale.y += scaleValFloat;
	else				scale.z += scaleValFloat;

	m_OBJ->SetScale(scale);
}