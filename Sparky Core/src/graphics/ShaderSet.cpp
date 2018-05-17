#include "ShaderSet.h"

#include <string>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <set>

#include "../platform/opengl/GLCommon.h"
#include "shaders.h"

namespace sparky { namespace graphics {

	const std::string SHADER_DIRECTORY = "res/shaders/";

	static uint64_t GetShaderFileTimestamp(const char* fileName)
	{
		uint64_t timestamp = 0;

		#ifdef _WIN32
		struct __stat64 stFileInfo;
		if (_stat64(fileName, &stFileInfo) == 0)
		{
			timestamp = stFileInfo.st_mtime;
		}
		#else
		struct stat fileStat;

		if (stat(filename, &fileStat) == -1)
		{
			perror(filename);
			return 0;
		}
		#endif

		return timestamp;
	}

	static std::string ShaderStringFromFile(const char* fileName)
	{
		std::ifstream fs(fileName);
		if (!fs)
		{
			return "";
		}

		std::string s(std::istreambuf_iterator<char>{fs}, std::istreambuf_iterator<char>{});

		return s;
	}

	ShaderSet::~ShaderSet()
	{
		for (auto& shader : m_Shaders)
		{
			GLCall(glDeleteShader(shader.second.Handle));
		}

		for (auto& program : m_Programs)
		{
			GLCall(glDeleteProgram(program.second.InternalHandle));
		}
	}

	void ShaderSet::SetVersion(const std::string & version)
	{
		m_Version = version;
	}

	void ShaderSet::SetPreambleFile(const std::string & preambleFileName)
	{
		m_Preamble = SHADER_DIRECTORY + preambleFileName;
	}

	GLuint* ShaderSet::AddProgram(const std::vector<std::pair<std::string, GLenum>>& typedShaders)
	{
		std::vector<const ShaderNameTypePair*> shaderNameTypes;

		for (const auto& shaderNameType : typedShaders)
		{
			ShaderNameTypePair tmpShaderNameType;
			std::string shaderPath = SHADER_DIRECTORY + shaderNameType.first;
			std::tie(tmpShaderNameType.Name, tmpShaderNameType.Type) = std::move(std::make_tuple(shaderPath, shaderNameType.second));

			{
				std::ifstream ifs(tmpShaderNameType.Name);
				if (!ifs)
				{
					fprintf(stderr, "Failed to open shader %s\n", tmpShaderNameType.Name.c_str());
				}
			}

			auto foundShader = m_Shaders.emplace(std::move(tmpShaderNameType), ShaderData{}).first;
			if (!foundShader->second.Handle)
			{
				GLCall(foundShader->second.Handle = glCreateShader(shaderNameType.second));
				// Mask the hash to 16 bits because some implementations are limited to that number of bits.
				// The sign bit is masked out, since some shader compilers treat the #line as signed, and others treat it unsigned.
				foundShader->second.HashName = (int32_t)std::hash<std::string>()(shaderPath) & 0x7FFF;
			}
			shaderNameTypes.push_back(&foundShader->first);
		}

		std::sort(begin(shaderNameTypes), end(shaderNameTypes)); // TODO : sorting vector of pointer?
		shaderNameTypes.erase(std::unique(begin(shaderNameTypes), end(shaderNameTypes)), end(shaderNameTypes));

		auto foundProgram = m_Programs.emplace(shaderNameTypes, Program{}).first;
		if (!foundProgram->second.InternalHandle)
		{
			foundProgram->second.PublicHandle = 0;

			GLCall(foundProgram->second.InternalHandle = glCreateProgram());
			for (const auto shader : shaderNameTypes)
			{
				GLCall(glAttachShader(foundProgram->second.InternalHandle, m_Shaders[*shader].Handle));
			}
		}

		return &foundProgram->second.PublicHandle;
	}

	void ShaderSet::UpdatePrograms()
	{
		std::set<std::pair<const ShaderNameTypePair, ShaderData>*> updatedShaders;
		for (auto& shader : m_Shaders)
		{
			uint64_t timestamp = GetShaderFileTimestamp(shader.first.Name.c_str());
			if (timestamp > shader.second.Timestamp)
			{
				shader.second.Timestamp = timestamp;
				updatedShaders.insert(&shader);
			}
		}

		for (auto shader : updatedShaders)
		{
			std::string version = "#version " + m_Version + "\n";

			std::string define;
			switch (shader->first.Type)
			{
			case GL_VERTEX_SHADER:			define += "#define VERTEX_SHADER\n";			break;
			case GL_FRAGMENT_SHADER:		define += "#define FRAGMENT_SHADER\n";			break;
			case GL_GEOMETRY_SHADER:		define += "#define GEOMETRY_SHADER\n";			break;
			case GL_TESS_CONTROL_SHADER:	define += "#define TES__CONTROL_SHADER\n";		break;
			case GL_TESS_EVALUATION_SHADER:	define += "#define TES_EVALUATION_SHADER\n";	break;
			case GL_COMPUTE_SHADER:			define += "#define COMPUTE_SHADER\n";			break;
			}

			std::string preamble_hash = std::to_string((int32_t)std::hash<std::string>()("preamble") & 0x7FFF);
			std::string preamble = /*"#line 1" + preamble_hash + "\n" + */
									ShaderStringFromFile(m_Preamble.c_str()) + "\n";

			std::string source_hash = std::to_string(shader->second.HashName);
			std::string source = /*"#line 1" + source_hash + "\n" +*/
								 ShaderStringFromFile(shader->first.Name.c_str()) + "\n";

			const char* strings[] =
			{
				version.c_str(),
				define.c_str(),
				preamble.c_str(),
				source.c_str()
			};
			GLint lengths[] =
			{
				(GLint)version.length(),
				(GLint)define.length(),
				(GLint)preamble.length(),
				(GLint)source.length()
			};

			GLCall(glShaderSource(shader->second.Handle, 4, strings, lengths));
			GLCall(glCompileShader(shader->second.Handle));

			GLint status;
			GLCall(glGetShaderiv(shader->second.Handle, GL_COMPILE_STATUS, &status));
			if (!status)
			{
				GLint logLength;
				GLCall(glGetShaderiv(shader->second.Handle, GL_INFO_LOG_LENGTH, &logLength));
				std::vector<char> log(logLength + 1);
				GLCall(glGetShaderInfoLog(shader->second.Handle, logLength, NULL, log.data()));

				std::string log_s = log.data();

				// replace all filename hashes in the error messages with actual filenames
				for (size_t found_preamble; (found_preamble = log_s.find(preamble_hash)) != std::string::npos;) {
					log_s.replace(found_preamble, preamble_hash.size(), "preamble");
				}
				for (size_t found_source; (found_source = log_s.find(source_hash)) != std::string::npos;) {
					log_s.replace(found_source, source_hash.size(), shader->first.Name);
				}

				fprintf(stderr, "Error compiling %s:\n%s\n", shader->first.Name.c_str(), log_s.c_str());
			}
		}

		for (auto& program : m_Programs)
		{
			bool programNeedsRelink = false;
			for (const auto programShader : program.first)
			{
				for (const auto shader : updatedShaders)
				{
					if (&shader->first == programShader)
					{
						programNeedsRelink = true;
						break;
					}
				}

				if (programNeedsRelink)
					break;
			}

			bool canRelink = true;
			if (programNeedsRelink)
			{
				for (const auto programShader : program.first)
				{
					GLint status;
					GLCall(glGetShaderiv(m_Shaders[*programShader].Handle, GL_COMPILE_STATUS, &status));
					if (!status)
					{
						canRelink = false;
						break;
					}
				}
			}

			if (programNeedsRelink & canRelink)
			{
				GLCall(glLinkProgram(program.second.InternalHandle));

				GLint logLength;
				GLCall(glGetProgramiv(program.second.InternalHandle, GL_INFO_LOG_LENGTH, &logLength));
				std::vector<char> log(logLength + 1);
				GLCall(glGetProgramInfoLog(program.second.InternalHandle, logLength, nullptr, log.data()));

				std::string log_s = log.data();

				// replace all filename hashes in the error messages with actual filenames
				std::string preamble_hash = std::to_string((int32_t)std::hash<std::string>()("preamble"));
				for (size_t found_preamble; (found_preamble = log_s.find(preamble_hash)) != std::string::npos;) {
					log_s.replace(found_preamble, preamble_hash.size(), "preamble");
				}
				for (const ShaderNameTypePair* shaderInProgram : program.first)
				{
					std::string source_hash = std::to_string(m_Shaders[*shaderInProgram].HashName);
					for (size_t found_source; (found_source = log_s.find(source_hash)) != std::string::npos;) {
						log_s.replace(found_source, source_hash.size(), shaderInProgram->Name);
					}
				}

				GLint status;
				GLCall(glGetProgramiv(program.second.InternalHandle, GL_LINK_STATUS, &status));

				if (!status)
				{
					fprintf(stderr, "Error Linking!");
				}

				else
				{
					fprintf(stderr, "Successfully linked");
				}

				fprintf(stderr, " program (");
				for (const ShaderNameTypePair* shader : program.first)
				{
					if (shader != program.first.front())
					{
						fprintf(stderr, ", ");
					}

					fprintf(stderr, "%s", shader->Name.c_str());
				}
				fprintf(stderr, ")");
				if (log[0] != '\0')
				{
					fprintf(stderr, ":\n%s\n", log_s.c_str());
				}
				else
				{
					fprintf(stderr, "\n");
				}

				if (!status)
				{
					program.second.PublicHandle = 0;
				}
				else
				{
					program.second.PublicHandle = program.second.InternalHandle;
				}
			}
		}
	}

	Shader* ShaderSet::AddProgramFromExts(const std::vector<std::string>& shaders)
	{
		std::vector<std::pair<std::string, GLenum>> typedShaders;
		for (const auto& shader : shaders)
		{
			size_t extLoc = shader.find_last_of('.');
			if (extLoc == std::string::npos)
			{
				return nullptr;
			}

			GLenum shaderType;

			std::string ext = shader.substr(extLoc + 1);
			if (ext == "vert")
				shaderType = GL_VERTEX_SHADER;
			else if (ext == "frag")
				shaderType = GL_FRAGMENT_SHADER;
			else if (ext == "geom")
				shaderType = GL_GEOMETRY_SHADER;
			else if (ext == "tesc")
				shaderType = GL_TESS_CONTROL_SHADER;
			else if (ext == "tese")
				shaderType = GL_TESS_EVALUATION_SHADER;
			else if (ext == "comp")
				shaderType = GL_COMPUTE_SHADER;
			else
				return nullptr;

			typedShaders.emplace_back(shader, shaderType);
		}

		GLuint* shaderID = AddProgram(typedShaders);

		return new Shader(shaderID);
	}
	Shader* ShaderSet::AddProgramFromCombinedFile(const std::string & fileName, const std::vector<GLenum>& shaderTypes)
	{
		std::vector<std::pair<std::string, GLenum>> typedShaders;

		for (const auto type : shaderTypes)
			typedShaders.emplace_back(fileName, type);

		GLuint* shaderID = AddProgram(typedShaders);

		return new Shader(shaderID);
	}

}}
