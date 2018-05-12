
#include <iostream>
#include "model.h"
#include "../maths/maths.h"
#include "BufferLayout.h"

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
		std::vector<float> vertices, positions, normals, texCoords, tangents;
		BufferLayout layout;

		positions.resize(3);
		layout.PushPosition();

		if (mesh->mTextureCoords[0])
		{
			texCoords.resize(2);
			layout.PushTexCoord();
		}
		if (mesh->HasNormals())
		{
			normals.resize(3);
			layout.PushNormal();
		}
		if (mesh->HasTangentsAndBitangents())
		{
			tangents.resize(6);
			layout.PushTangent();
			layout.PushBinormal();
		}

		GLuint vertexSize = positions.size() + normals.size() + texCoords.size() + tangents.size();
		vertices.reserve(mesh->mNumVertices * vertexSize);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			positions[0] = mesh->mVertices[i].x;
			positions[1] = mesh->mVertices[i].y;
			positions[2] = mesh->mVertices[i].z;

			if (mesh->mTextureCoords[0])
			{
				texCoords[0] = mesh->mTextureCoords[0][i].x;
				texCoords[1] = mesh->mTextureCoords[0][i].y;
			}
			
			if (mesh->HasNormals())
			{
				normals[0] = mesh->mNormals[i].x;
				normals[1] = mesh->mNormals[i].y;
				normals[2] = mesh->mNormals[i].z;
			}

			if (mesh->HasTangentsAndBitangents())
			{
				tangents[0] = mesh->mTangents[i].x;
				tangents[1] = mesh->mTangents[i].y;
				tangents[2] = mesh->mTangents[i].z;

				tangents[3] = mesh->mBitangents[i].x;
				tangents[4] = mesh->mBitangents[i].y;
				tangents[5] = mesh->mBitangents[i].z;
			}

			vertices.insert(vertices.end(), positions.begin(), positions.end());
			vertices.insert(vertices.end(), texCoords.begin(), texCoords.end());
			vertices.insert(vertices.end(), normals.begin(), normals.end());
			vertices.insert(vertices.end(), tangents.begin(), tangents.end());
		}

		std::vector<unsigned int> indices;
		indices.reserve(mesh->mNumFaces * 3);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		std::vector<Texture2D> textures;
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

		return Mesh(vertices, layout, indices, textures);
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
