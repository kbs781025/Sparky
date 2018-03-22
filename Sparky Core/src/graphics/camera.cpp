#include "camera.h"
#include <GLFW/glfw3.h>

namespace sparky { namespace graphics
{
	const float Camera::m_CameraSpeed = 2.5f;

	Camera::Camera()
		:
		m_Distance(5.0f),
		m_Position(glm::vec3(0.0f, 0.0f, 5.0f)),
		m_Target(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_Angle(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_ViewMatrix(glm::mat4()),
		m_Forward(glm::vec3(0.0f, 0.0f, -1.0f))
	{
	}

	Camera::Camera(const glm::vec3 & position, const glm::vec3 & target, const glm::vec3 & up)
		:
		m_Distance(5.0f),
		m_Position(position),
		m_Target(target),
		m_Forward(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		lookAt(m_Position, m_Target, up);
	}

	void Camera::setPosition(const glm::vec3 & pos)
	{
		m_Position = pos;
		m_Target = m_Position + m_Forward * m_Distance;
	}

	void Camera::setPosition(float x, float y, float z)
	{
		setPosition(glm::vec3(x, y, z));
	}

	void Camera::setRotation(const glm::vec3 & angle)
	{
		m_Angle = angle;
		m_Forward = rotateForward();
		m_Target = m_Position + m_Forward * m_Distance;
	}

	void Camera::setRotation(float pitch, float yaw, float roll)
	{
		setRotation(glm::vec3(pitch, yaw, roll));
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

	void Camera::lookAt()
	{
		lookAt(m_Position, m_Target);
		//m_ViewMatrix = glm::lookAt(m_Position, m_Target, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void Camera::moveCamera(float frameTime, int key)
	{
		glm::vec3 rightVec = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), -m_Forward);

		switch (key)
		{
		case GLFW_KEY_W:
			setPosition(m_Position + m_Forward * frameTime * m_CameraSpeed);
			break;
		case GLFW_KEY_A:
			setPosition(m_Position - rightVec * frameTime * m_CameraSpeed);
			break;
		case GLFW_KEY_S:
			setPosition(m_Position - m_Forward * frameTime * m_CameraSpeed);
			break;
		case GLFW_KEY_D:
			setPosition(m_Position + rightVec * frameTime * m_CameraSpeed);
			break;
		}
	}

	void Camera::rotateCamera(float deltaX, float deltaY)
	{
		float sensitivity = 0.05f;

		m_Angle.x = m_Angle.x + deltaX * sensitivity;
		m_Angle.y = m_Angle.y + deltaY * sensitivity;
		
		if (m_Angle.x >= 89.9f)
		{
			m_Angle.x = 89.9f;
		}
		else if (m_Angle.x <= -89.9f)
		{
			m_Angle.y = -89.9f;
		}

		rotateForward();
	}

	// Rotate camera to target vector
	// The order of rotation is Roll->Yaw->Pitch (Rx*Ry*Rz)
	// Rx: rotation about X-axis, pitch
	// Ry: rotation about Y-axis, yaw(heading)
	// Rz: rotation about Z-axis, roll
	//    Rx           Ry          Rz
	// |1  0   0| | Cy  0 Sy| |Cz -Sz 0|   | CyCz        -CySz         Sy  |
	// |0 Cx -Sx|*|  0  1  0|*|Sz  Cz 0| = | SxSyCz+CxSz -SxSySz+CxCz -SxCy|
	// |0 Sx  Cx| |-Sy  0 Cy| | 0   0 1|   |-CxSyCz+SxSz  CxSySz+SxCz  CxCy|
	glm::vec3 Camera::rotateForward()
	{
		glm::vec3 newForward;
		float cx, sx, cy, sy;

		cx = cos(m_Angle.x);
		sx = sin(m_Angle.x);
		cy = cos(m_Angle.y);
		sy = sin(m_Angle.y);

		newForward.x = cy;
		newForward.y = sx * sy;
		newForward.z = -cx * sy;
		
		return newForward;
	}
}
}
