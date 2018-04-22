#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shaders.h"
#include "mesh.h"
#include "texture.h"

namespace sparky { namespace graphics {

	class Model
	{
	public:
		Model(char* path)
		{
			loadModel(path);
		}
		void Draw(Shader& shader, bool textureOn = true);
		void DrawInstances(Shader& shader, unsigned int instanceCount);

		unsigned int getMeshNum() { return m_Meshes.size(); }
		std::vector<Mesh> getMeshes() { return m_Meshes; }
	private:
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		std::vector<Texture2D> m_LoadedTextures;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture2D> loadMaterialTextuers(aiMaterial* mat, aiTextureType type, std::string typeName);
	};



}}