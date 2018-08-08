#pragma once
#include "../maths/maths.h"

namespace sparky { namespace graphics
{
	class Camera
	{
	public:
		Camera(const glm::vec3& position, const glm::vec3& up);

		// Getter
		inline const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		inline const glm::vec3& getCameraPosition() const { return m_Position; }
		inline const glm::vec3& getForward() const { return m_Forward; }
		inline const glm::vec3& getAngle() const { return glm::vec3(m_Roll, m_Pitch, m_Yaw); }

		void updateView();

		// Update each frame
		void moveCamera(float frameTime, int key); // for variable timestep
		void rotateCamera(float deltaX, float deltaY);
		void moveCamera(int key); // for fixed timestep

	private:
		// Translating
		void setPosition(const glm::vec3& pos);
		void setPosition(float x, float y, float z);

		// LookAt
		void lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& upDir);
		void lookAt(const glm::vec3& position, const glm::vec3& target);

	private:
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position;
		glm::vec3 m_Forward;

		float m_Roll;
		float m_Pitch;
		float m_Yaw;

		static const float m_MoveSpeed;
		static const float m_RotationSpeed;
	};

} }
