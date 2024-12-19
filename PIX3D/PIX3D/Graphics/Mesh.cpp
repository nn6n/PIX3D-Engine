#include "Mesh.h"
#include <Core/Core.h>
#include <Platfrom/GL/GLCommands.h>
#include <Platfrom/GL/GLRenderer.h>


namespace
{
    PIX3D::GL::GLTexture LoadMaterialTexture(aiMaterial* mat, aiTextureType type)
    {
        aiString path;
        mat->GetTexture(type, 0, &path);
        
        PIX3D::GL::GLTexture texture;
        texture.LoadFromFile(path.C_Str());
        
        return texture;
    }
}

namespace PIX3D
{

    void PIX3D::StaticMesh::Load(const std::string& path, float scale)
    {
        m_Path = path;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_PreTransformVertices |
            aiProcess_GenNormals |
            aiProcess_CalcTangentSpace |
            aiProcess_JoinIdenticalVertices
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            PIX_ASSERT_MSG(false, importer.GetErrorString());
        }

        ProcessNode(scene->mRootNode, scene);

        // Load Into GPU
        LoadGpuData();
    }

    void PIX3D::StaticMesh::ProcessNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_SubMeshes.push_back(ProcessMesh(mesh, scene));
        }

        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    StaticSubMesh PIX3D::StaticMesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {   
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            StaticMeshVertex vertex;

            // process vertex positions, normals and texture coordinates
            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;

            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;


            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            m_Vertices.push_back(vertex);
        }


        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                m_Indices.push_back(face.mIndices[j]);
        }

        // Load Material
        bool HasMaterial = mesh->mMaterialIndex >= 0;
        bool MaterialPresent = false;
        int MaterialIndex = -1;

        if (HasMaterial)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            
            // Diffuse Material Data
            
            aiColor4D DiffuseColor;
            material->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor);
            {
                aiString path;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

                std::filesystem::path FullPath = (m_Path.parent_path() / path.C_Str()).string();
                
                if (FullPath.has_extension())
                {
                    std::string TextureName = FullPath.stem().string();

                    // Check For Material Existance
                    for (size_t i = 0; i < m_Materials.size(); i++)
                    {
                        if (m_Materials[i].Name == TextureName)
                        {
                            MaterialPresent = true;
                            MaterialIndex = i;
                            break;
                        }
                    }

                    if (!MaterialPresent)
                    {
                        // Create New Material
                        BaseColorMaterial mat;
                        mat.BaseColor = { DiffuseColor.r, DiffuseColor.g, DiffuseColor.b, DiffuseColor.a };
                        mat.BaseColorTexture.LoadFromFile(FullPath.string());
                        mat.Name = TextureName;

                        m_Materials.push_back(mat);
                        MaterialIndex = m_Materials.size() - 1;
                    }
                }
                else
                {
                    // Create New Material
                    BaseColorMaterial mat;
                    mat.BaseColor = glm::vec4(1.0f);
                    mat.BaseColorTexture = GL::GLRenderer::GetDefaultTexture();
                    mat.Name = "Engine Default Texture";

                    m_Materials.push_back(mat);
                    MaterialIndex = m_Materials.size() - 1;
                }
            }
        }

        StaticSubMesh subMesh;
        subMesh.BaseVertex = m_BaseVertex;
        subMesh.BaseIndex = m_BaseIndex;
        subMesh.VerticesCount = mesh->mNumVertices;
        subMesh.IndicesCount = mesh->mNumFaces * 3;
        subMesh.MaterialIndex = MaterialIndex;

        m_BaseVertex += mesh->mNumVertices;
        m_BaseIndex += mesh->mNumFaces * 3;

        return subMesh;
    }

    void StaticMesh::LoadGpuData()
    {
        m_Vertexbuffer.Create();
        m_Vertexbuffer.Fill(BufferData::CreatFrom<StaticMeshVertex>(m_Vertices));

        m_Indexbuffer.Create();
        m_Indexbuffer.Fill(m_Indices);

        m_VertexArray.Create();
        m_VertexArray.AddVertexBuffer(m_Vertexbuffer, sizeof(StaticMeshVertex));
        m_VertexArray.AddIndexBuffer(m_Indexbuffer);

        {
            GL::GLVertexAttrib Position;
            Position.ShaderLocation = 0;
            Position.Type = GL::GLShaderAttribType::FLOAT;
            Position.Count = 3;
            Position.Offset = 0;

            GL::GLVertexAttrib Normal;
            Normal.ShaderLocation = 1;
            Normal.Type = GL::GLShaderAttribType::FLOAT;
            Normal.Count = 3;
            Normal.Offset = 3 * sizeof(float);

            GL::GLVertexAttrib TexCoords;
            TexCoords.ShaderLocation = 2;
            TexCoords.Type = GL::GLShaderAttribType::FLOAT;
            TexCoords.Count = 2;
            TexCoords.Offset = 3 * sizeof(float) + 3 * sizeof(float);

            m_VertexArray.AddVertexAttrib(Position);
            m_VertexArray.AddVertexAttrib(Normal);
            m_VertexArray.AddVertexAttrib(TexCoords);
        }
    }
}
