#include "model.h"

#include <iostream>

#include "../maths/maths.h"

#include "shaders.h"
#include "BufferLayout.h"
#include "mesh.h"
#include "Texture2D.h"
#include "ForwardRenderer.h"
#include "Material.h"

namespace sparky { namespace graphics {

	Model::Model(const char * path)
	{
		loadModel(path);
	}

	void Model::Draw()
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i].Draw();
		}
	}

	void Model::DrawInstances(unsigned int instanceCount)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i].DrawInstances(instanceCount);
		}
	}

	void Model::SubmitMesh(ForwardRenderer* renderer, const Shader* shader)
	{
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.05f));
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
		{
			renderer->submitMesh(&m_Meshes[i], model, shader);
		}
	}

	void Model::SubmitMesh(ForwardRenderer * renderer, const glm::mat4 & model, const Shader * shader)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
		{
			renderer->submitMesh(&m_Meshes[i], model, shader);
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

		Material* ModelMaterial = nullptr;
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			ModelMaterial = loadMaterialTextuers(material);
		}

		return Mesh(vertices, layout, indices, ModelMaterial);
	}

	Material* Model::loadMaterialTextuers(aiMaterial * mat)
	{
		Material* material = new Material();
		//TODO : material texture loading should be more flexible
		aiTextureType aiTextureTypes[3] = { aiTextureType_DIFFUSE , aiTextureType_SPECULAR , aiTextureType_HEIGHT };
		TextureType MaterialTextureTypes[3] = { TextureType::ALBEDO, TextureType::SPECULAR, TextureType::NORMAL };

		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < mat->GetTextureCount(aiTextureTypes[i]); ++j)
			{
				assert(j < 1); // TODO : Now only one texture of the same kind is supported
				aiString str;
				mat->GetTexture(aiTextureTypes[i], i, &str);

				std::string filePath = m_Directory + '/' + std::string(str.C_Str());
				material->AddTexture(MaterialTextureTypes[i], filePath);
			}
		}

		return material;
	}

}
}
