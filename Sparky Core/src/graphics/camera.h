#pragma once
#include "../maths/maths.h"

namespace sparky { namespace graphics
{
	class Camera
	{
	public:
		Camera();
		Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);

		// Getter
		glm::vec3 getCameraPosition() const { return m_Position; }
		glm::vec3 getTarget() const { return m_Target; }
		glm::vec3 getAngle() const { return m_Angle; }
		glm::mat4 getViewMatrix() const { return m_ViewMatrix; }
		glm::vec3 getForward() const { return m_Forward; }

		void lookAt();

		// Update each frame
		void moveCamera(float frameTime, int key);
		void rotateCamera(float deltaX, float deltaY);

	private:
		// Setter
		// Translating
		void setPosition(const glm::vec3& pos);
		void setPosition(float x, float y, float z);

		// Rotating
		void setRotation(const glm::vec3& angle);
		void setRotation(float pitch, float yaw, float roll);
		glm::vec3 rotateForward();

		// LookAt
		void lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& upDir);
		void lookAt(const glm::vec3& position, const glm::vec3& target);

	private:
		float m_Distance;
		glm::vec3 m_Position;
		glm::vec3 m_Target;
		glm::vec3 m_Angle;
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Forward;
		static const float m_CameraSpeed;

	};

} }
