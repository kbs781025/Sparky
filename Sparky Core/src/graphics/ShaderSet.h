#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <GL/glew.h>

namespace sparky { namespace graphics {

	class Shader;

	class ShaderSet
	{
	private:
		using ShaderHandle = GLuint;
		using ProgramHandle = GLuint;

		struct ShaderNameTypePair
		{
			std::string Name;
			GLenum Type;
			bool operator<(const ShaderNameTypePair& rhs) const
			{
				return std::tie(Name, Type) < std::tie(rhs.Name, rhs.Type);
			}
		};

		struct ShaderData
		{
			ShaderHandle Handle;
			uint64_t Timestamp;
			int32_t HashName;
		};

		struct Program
		{
			ProgramHandle PublicHandle;
			ProgramHandle InternalHandle;
		};

	private:
		std::string m_Version;
		std::string m_Preamble;
		std::map<ShaderNameTypePair, ShaderData> m_Shaders;
		std::map<std::vector<const ShaderNameTypePair*>, Program> m_Programs;
	private:
		GLuint* AddProgram(const std::vector<std::pair<std::string, GLenum>>& typedShaders);
	public:
		ShaderSet() = default;
		~ShaderSet();
		void SetVersion(const std::string& version);
		void SetPreambleFile(const std::string& preambleFileName);
		void UpdatePrograms();

		// vertex shader: .vert
		// fragment shader: .frag
		// geometry shader: .geom
		// tessellation control shader: .tesc
		// tessellation evaluation shader: .tese
		// compute shader: .comp
		// eg: AddProgramFromExts({"foo.vert", "bar.frag"});
		Shader* AddProgramFromExts(const std::vector<std::string>& shaders);

		// eg: AddProgramFromCombinedFile("shader.glsl", { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER });
		//
		// The defines are as follows:
		// vertex shader: VERTEX_SHADER
		// fragment shader: FRAGMENT_SHADER
		// geometry shader: GEOMETRY_SHADER
		// tessellation control shader: TESS_CONTROL_SHADER
		// tessellation evaluation shader: TESS_EVALUATION_SHADER
		// compute shader: COMPUTE_SHADER
		//
		// Example combined file shader:
		//     #ifdef VERTEX_SHADER
		//     void main() { /* your vertex shader main */ }
		//     #endif
		//
		//     #ifdef FRAGMENT_SHADER
		//     void main() { /* your fragment shader main */ }
		//     #endif
		Shader* AddProgramFromCombinedFile(const std::string& fileName, const std::vector<GLenum>& shaderTypes);
	};

}}