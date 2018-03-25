#include "camera.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace sparky { namespace graphics
{
	const float Camera::m_CameraSpeed = 2.5f;

	Camera::Camera()
		:
		m_Position(glm::vec3(0.0f, 0.0f, 5.0f)),
		m_Roll(0.0f), m_Pitch(0.0f), m_Yaw(0.0f),
		m_ViewMatrix(glm::mat4()),
		m_Forward(glm::vec3(0.0f, 0.0f, -1.0f))
	{
	}

	Camera::Camera(const glm::vec3 & position, const glm::vec3 & up)
		:
		m_Position(position),
		m_Roll(0.0f), m_Pitch(0.0f), m_Yaw(0.0f),
		m_Forward(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		lookAt(m_Position, glm::vec3(0.0f, 0.0f, 0.0f), up);
	}

	void Camera::setPosition(const glm::vec3 & pos)
	{
		m_Position = pos;
		updateView();
	}

	void Camera::setPosition(float x, float y, float z)
	{
		setPosition(glm::vec3(x, y, z));
	}

	void Camera::lookAt(const glm::vec3 & position, const glm::vec3 & target, const glm::vec3 & upDir)
	{
		glm::vec3 forward = glm::normalize(position - target);
		glm::vec3 right = glm::normalize(glm::cross(upDir, forward));
		glm::vec3 up = glm::cross(forward, right);

		m_ViewMatrix = glm::mat4();
		m_ViewMatrix[0] = glm::vec4(right.x, up.x, forward.x, 0.0f);
		m_ViewMatrix[1] = glm::vec4(right.y, up.y, forward.y, 0.0f);
		m_ViewMatrix[2] = glm::vec4(right.z, up.z, forward.z, 0.0f);
		
		float translationX = -(right.x * m_Position.x + right.y * m_Position.y + right.z * m_Position.z);
		float translationY = -(up.x * m_Position.x + up.y * m_Position.y + up.z * m_Position.z);
		float translationZ = -(forward.x * m_Position.x + forward.y * m_Position.y + forward.z * m_Position.z);
		m_ViewMatrix[3] = glm::vec4(translationX, translationY, translationZ, 1.0f);
	}

	void Camera::lookAt(const glm::vec3 & position, const glm::vec3 & target)
	{
		lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void Camera::updateView()
	{
		glm::mat4 matYaw;
		glm::mat4 matPitch;
		glm::mat4 matRoll;

		matYaw = glm::rotate(matYaw, glm::radians(m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		matPitch = glm::rotate(matPitch, glm::radians(m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		//matRoll = glm::rotate(matRoll, glm::radians(m_Roll), glm::vec3(0.0f, 0.0f, 1.0f));

		//glm::mat4 matRotate = glm::transpose(matRoll * matPitch * matYaw); // Rotation order : roll -> pitch -> yaw
		glm::mat4 matRotate = glm::transpose(matYaw * matPitch); // Rotation order : roll -> pitch -> yaw

		glm::mat4 matTranslate;
		matTranslate = glm::translate(matTranslate, -m_Position);

		m_ViewMatrix = matRotate * matTranslate;

		m_Forward = -glm::vec3(matRotate[0][2], matRotate[1][2], matRotate[2][2]);
		m_Forward = glm::normalize(m_Forward);
	}

	void Camera::moveCamera(float frameTime, int key)
	{
		glm::vec3 rightVec = glm::cross(m_Forward, glm::vec3(0.0f, 1.0f, 0.0f));
		float moveAmout = m_CameraSpeed * frameTime;
		float dx = 0, dz = 0;
		switch (key)
		{
		case GLFW_KEY_W:
			dz++;
			break;
		case GLFW_KEY_A:
			dx--;
			break;
		case GLFW_KEY_S:
			dz--;
			break;
		case GLFW_KEY_D:
			dx++;
			break;
		}
		setPosition(m_Position + (m_Forward * dz + rightVec * dx) * moveAmout);
	}

	void Camera::rotateCamera(float deltaX, float deltaY)
	{
		float sensitivity = 0.1f;

		float pitchOffset = sensitivity * deltaY;
		float yawOffset = sensitivity * deltaX;

		m_Pitch += pitchOffset;
		m_Yaw-= yawOffset; 

		std::cout << m_Pitch << " " << m_Yaw << std::endl;

		if (m_Pitch >= 89.9f)
		{
			m_Pitch = 89.9f;
		}
		else if (m_Pitch <= -89.8f)
		{
			m_Pitch = -89.8f;
		}

		if (m_Yaw >= 180.0f)
		{
			m_Yaw = -180.0f;
		}
		else if (m_Yaw <= -180.0f)
		{
			m_Yaw = 180.0f;
		}

		updateView();
	}
}
}
