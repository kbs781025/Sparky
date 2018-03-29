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
		void Draw(Shader shader);
	private:
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		std::vector<Texture> m_LoadedTextures;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextuers(aiMaterial* mat, aiTextureType type, std::string typeName);
	};



}}