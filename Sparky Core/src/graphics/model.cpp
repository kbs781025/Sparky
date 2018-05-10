
#include <iostream>
#include "model.h"
#include "../maths/maths.h"
#include "Vertex.h"

namespace sparky { namespace graphics {

	void Model::Draw(Shader& shader, bool textureOn)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i].Draw(shader, textureOn);
		}
	}

	void Model::DrawInstances(Shader & shader, unsigned int instanceCount)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i].DrawInstances(shader, instanceCount);
		}
	}

	void Model::loadModel(const std::string & path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs  | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			__debugbreak();
			return;
		}
		m_Directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode * node, const aiScene * scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture2D> textures;

		vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.m_Position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_Normal = vector;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.m_TexCoord = vec;
			}
			else
			{
				vertex.m_TexCoord = glm::vec2(0.0f, 0.0f);
			}

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.m_Tangent.x = mesh->mTangents[i].x;
				vertex.m_Tangent.y = mesh->mTangents[i].y;
				vertex.m_Tangent.z = mesh->mTangents[i].z;

				vertex.m_BiTangent.x = mesh->mBitangents[i].x;
				vertex.m_BiTangent.y = mesh->mBitangents[i].y;
				vertex.m_BiTangent.z = mesh->mBitangents[i].z;
			}
			else
			{
				vertex.m_Tangent.x = 0.3f;
				vertex.m_Tangent.y = 0.3f;
				vertex.m_Tangent.z = 0.3f;

				vertex.m_BiTangent.x = 0.3f;
				vertex.m_BiTangent.y = 0.3f;
				vertex.m_BiTangent.z = 0.3f;
			}
			

			vertices.push_back(vertex);
		}

		indices.reserve(mesh->mNumFaces * 3);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture2D> diffuseMaps = loadMaterialTextuers(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			
			std::vector<Texture2D> specularMaps = loadMaterialTextuers(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			std::vector<Texture2D> normalMaps = loadMaterialTextuers(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture2D> Model::loadMaterialTextuers(aiMaterial * mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture2D> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			bool skip = false;
			mat->GetTexture(type, i, &str);
			for (unsigned int i = 0; i < m_LoadedTextures.size(); i++)
			{
				if (std::strcmp(m_LoadedTextures[i].getFilePath().c_str(), str.C_Str()) == 0) // should return texture file name not path
				{
					textures.push_back(m_LoadedTextures[i]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				std::string filePath = m_Directory + '/' + std::string(str.C_Str());
				Texture2D texture(filePath, typeName);
				textures.push_back(texture);
				m_LoadedTextures.push_back(texture);
			}
		}

		return textures;
	}

}
}
