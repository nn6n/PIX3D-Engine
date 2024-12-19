#pragma once
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Platfrom/GL/GLTexture.h>
#include <Platfrom/GL/GLVertexArray.h>
#include <Platfrom/GL/GLShader.h>
#include <filesystem>

namespace PIX3D
{
	struct BaseColorMaterial
	{
		glm::vec4 BaseColor;
		GL::GLTexture BaseColorTexture;
		std::string Name;
	};


	struct StaticMeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct StaticSubMesh
	{
		uint32_t BaseVertex = 0;
		uint32_t BaseIndex = 0;
		uint32_t VerticesCount = 0;
		uint32_t IndicesCount = 0;
		uint32_t MaterialIndex = 0;
	};

	class StaticMesh
	{
	public:
		StaticMesh() = default;

		void Load(const std::string& path, float scale = 1.0f);


		std::vector<StaticSubMesh> GetSubMeshes() const { return m_SubMeshes; }
		std::vector<BaseColorMaterial> GetMaterials() const { return m_Materials; }
		GL::GLVertexArray GetVertexArray() const { return m_VertexArray; }

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		StaticSubMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		void LoadGpuData();

	private:
		std::filesystem::path m_Path;

		std::vector<StaticMeshVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		std::vector<StaticSubMesh> m_SubMeshes;
		std::vector<BaseColorMaterial> m_Materials;

		uint32_t m_BaseVertex, m_BaseIndex = 0;

		// Gpu Data
		GL::GLVertexBuffer m_Vertexbuffer;
		GL::GLIndexBuffer m_Indexbuffer;
		GL::GLVertexArray m_VertexArray;
	};
}
