#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

namespace sparky { namespace graphics {

	class Shader;
	class Texture2D;
	class ForwardRenderer;
	class Material;

	class Model
	{
	public:
		Model(const char* path);
		
		void Draw();
		void DrawInstances(unsigned int instanceCount);

		// TODO : temporary method rendering by renderer
		void SubmitMesh(ForwardRenderer* renderer, const Shader* shader);
		void SubmitMesh(ForwardRenderer* renderer, const glm::mat4& model, const Shader* shader);

		unsigned int getMeshNum() { return m_Meshes.size(); }
		std::vector<Mesh> getMeshes() { return m_Meshes; }
	private:
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		Material* loadMaterialTextuers(aiMaterial* mat);
	};



}}