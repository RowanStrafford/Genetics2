#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 m_Pos;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	double m_yaw;
	double m_pitch;

	float m_movementSpeed;
	float m_mouseSensitivity;
	float m_zoom;

	Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1.0f, 0), float yaw = YAW, float pitch = PITCH) : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_zoom(ZOOM)
	{
		m_Pos = position;
		m_WorldUp = up;
		m_yaw = yaw;
		m_pitch = pitch;
		UpdateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_zoom(ZOOM)
	{
		m_Pos = glm::vec3(posX, posY, posZ);
		m_WorldUp = glm::vec3(upX, upY, upZ);
		m_yaw = yaw;
		m_pitch = pitch;
		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = m_movementSpeed * deltaTime;

		if (direction == FORWARD)
		{
			m_Pos += m_Front * velocity;
		}

		if (direction == BACKWARD)
		{
			m_Pos -= m_Front * velocity;
		}

		if (direction == LEFT)
		{
			m_Pos -= m_Right * velocity;
		}

		if (direction == RIGHT)
		{
			m_Pos += m_Right * velocity;
		}
	}

	void ProcessMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch = true)
	{
		xOffset *= m_mouseSensitivity;
		yOffset *= m_mouseSensitivity;

		m_yaw += xOffset;
		m_pitch += yOffset;

		if (constrainPitch)
		{
			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;
		}

		UpdateCameraVectors();
	}

	void ProcessMouseScroll(float yOffset)
	{
		m_zoom -= (float)yOffset;

		if (m_zoom < 1.0f)
			m_zoom = 1.0f;
		if (m_zoom > 45.0f)
			m_zoom = 45.0f;
	}

	void SetPosition(glm::vec3 pos)
	{
		m_Pos = pos;
	}

private:
	void UpdateCameraVectors()
	{
		glm::vec3 tempFront;
		tempFront.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		tempFront.y = sin(glm::radians(m_pitch));
		tempFront.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_Front = glm::normalize(tempFront);

		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}
};

#endif