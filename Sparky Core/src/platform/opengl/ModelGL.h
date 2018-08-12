#pragma once

#include "../../graphics/camera.h"
#include "../../graphics/model.h"
#include "../../maths/maths.h"
#include "../../graphics/ShaderSet.h"
#include "../../graphics/shader.h"
#include <map>

namespace sparky { namespace opengl {
	using namespace sparky;

	class ModelGL
	{
	public:
		ModelGL();
		~ModelGL();

		void Init();
		void SetViewport(int width, int height);
		void Draw();

		void SetMouseLeft(bool flag) { m_MouseLeftDown = flag; };
		void SetMouseRight(bool flag) { m_MouseRightDown = flag; };
		void SetMousePosition(int x, int y) { m_MouseX = x; m_MouseY = y; };

		void RotateCamera(int x, int y);
		void ZoomCamera(int dist);

	private:
		bool m_MouseLeftDown;
		bool m_MouseRightDown;
		int m_MouseX;
		int m_MouseY;

		graphics::ShaderSet m_ShaderSet;
		graphics::Shader* m_CurrentShader;
		graphics::Camera* m_Camera;
		glm::mat4 m_ProjectionMat;
		std::map<std::string, graphics::Model*> m_Models;
	};

} }