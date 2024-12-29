#include "Mesh.h"
#include <Core/Core.h>
#include <Platfrom/GL/GLCommands.h>
#include <Platfrom/GL/GLRenderer.h>
#include <fstream>


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

    bool FileExists(const std::string& name)
    {
        std::ifstream f(name.c_str());
        return f.good();
    }
}

namespace PIX3D
{

    void PIX3D::StaticMesh::Load(const std::string& path, float scale)
    {
        m_Path = path;
        m_Scale = scale;

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

    void StaticMesh::FillMaterialBuffer()
    {
        {
            auto* gpu_material_buffer_data = (MaterialGPUShader_InfoBufferData*)m_MaterialInfoDataStorageBuffer.MapBuffer();

            for (auto& mat : m_Materials)
            {
                gpu_material_buffer_data->UseAlbedoTexture = mat.UseAlbedoTexture;
                gpu_material_buffer_data->UseNormalTexture = mat.UseNormalTexture;
                gpu_material_buffer_data->UseMetallicRoughnessTexture = mat.UseMetallicRoughnessTexture;
                gpu_material_buffer_data->useAoTexture = mat.useAoTexture;
                gpu_material_buffer_data->UseEmissiveTexture = mat.UseEmissiveTexture;

                gpu_material_buffer_data->BaseColor = mat.BaseColor;
                gpu_material_buffer_data->EmissiveColor = mat.EmissiveColor;

                gpu_material_buffer_data->Metalic = mat.Metalic;
                gpu_material_buffer_data->Roughness = mat.Roughness;
                gpu_material_buffer_data->Ao = mat.Ao;

                gpu_material_buffer_data++;
            }

            m_MaterialInfoDataStorageBuffer.UnMapBuffer();
        }

        /*
        {
            auto* gpu_material_buffer_data = (MaterialGPUShader_TextureBufferData*)m_MaterialTextureDataStorageBuffer.MapBuffer();

            for (auto& mat : m_Materials)
            {
                gpu_material_buffer_data->AlbedoTexture = mat.AlbedoTexture.GetVramTextureID();
                gpu_material_buffer_data->NormalTexture = mat.NormalTexture.GetVramTextureID();
                gpu_material_buffer_data->MetalRoughnessTexture = mat.MetalRoughnessTexture.GetVramTextureID();
                gpu_material_buffer_data->AoTexture = mat.AoTexture.GetVramTextureID();
                gpu_material_buffer_data->EmissiveTexture = mat.EmissiveTexture.GetVramTextureID();

                gpu_material_buffer_data++;
            }

            m_MaterialTextureDataStorageBuffer.UnMapBuffer();
        }
        */
    }

    void StaticMesh::Destroy()
    {
        m_MaterialTextureDataStorageBuffer.Destroy();
        m_MaterialInfoDataStorageBuffer.Destroy();
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
            vertex.Position.x = mesh->mVertices[i].x * m_Scale;
            vertex.Position.y = mesh->mVertices[i].y * m_Scale;
            vertex.Position.z = mesh->mVertices[i].z * m_Scale;

            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;

            if (mesh->HasTangentsAndBitangents())
            {
                vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
                vertex.BiTangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            }

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
            

            // Create New Material
            BaseColorMaterial mat;
            
            // Get Base Color
            aiColor4D DiffuseColor;
            material->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor);
            mat.BaseColor = { DiffuseColor.r, DiffuseColor.g, DiffuseColor.b, 1.0f };

            // Get Emissive Color
            aiColor4D EmissiveColor;
            material->Get(AI_MATKEY_COLOR_EMISSIVE, EmissiveColor);
            mat.EmissiveColor = { EmissiveColor.r, EmissiveColor.g, EmissiveColor.b, 1.0f };

            // Fill Metalic
            float Metallic = 0.0f;
            if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_METALLIC_FACTOR, &Metallic))
                mat.Metalic = Metallic;

            // Fill Roughness
            float Roughness = 1.0f;
            if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_ROUGHNESS_FACTOR, &Roughness))
                mat.Roughness = Roughness;

            // Load AlbedoMap
            {
                aiString path;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
                std::filesystem::path FullPath = (m_Path.parent_path() / path.C_Str()).string();
                
                if (FileExists(FullPath.string())) // AlbedoMap Found
                {
                    mat.UseAlbedoTexture = true;
                    mat.AlbedoTexture.LoadFromFile(FullPath.string(), true);
                }
                else // Default Albedo
                {
                    mat.UseAlbedoTexture = false;
                    mat.AlbedoTexture = GL::GLRenderer::GetDefaultAlbedoTexture();
                }
            }

            // Load NormalMap
            {
                aiString path;
                material->GetTexture(aiTextureType_NORMALS, 0, &path);
                std::filesystem::path FullPath = (m_Path.parent_path() / path.C_Str()).string();

                if (FileExists(FullPath.string())) // NormalMap Found
                {
                    mat.UseNormalTexture = true;
                    mat.NormalTexture.LoadFromFile(FullPath.string());
                }
                else
                {
                    mat.UseNormalTexture = false;
                    mat.NormalTexture = GL::GLRenderer::GetDefaultNormalTexture();
                }
            }

            // Load MetalRoughnessMap
            {
                aiString path;
                material->GetTexture(aiTextureType_METALNESS, 0, &path);
                std::filesystem::path FullPath = (m_Path.parent_path() / path.C_Str()).string();

                if (FileExists(FullPath.string())) // RoughnessMap Found
                {
                    mat.UseMetallicRoughnessTexture = true;
                    mat.MetalRoughnessTexture.LoadFromFile(FullPath.string());
                }
                else
                {
                    mat.UseMetallicRoughnessTexture = false;
                    mat.MetalRoughnessTexture = GL::GLRenderer::GetDefaultBlackTexture();
                }
            }

            // Load AoMap
            {
                aiString path;
                material->GetTexture(aiTextureType_LIGHTMAP, 0, &path);
                std::filesystem::path FullPath = (m_Path.parent_path() / path.C_Str()).string();

                if (FileExists(FullPath.string())) // RoughnessMap Found
                {
                    mat.useAoTexture = true;
                    mat.AoTexture.LoadFromFile(FullPath.string());
                }
                else
                {
                    mat.useAoTexture = false;
                    mat.AoTexture = GL::GLRenderer::GetDefaultWhiteTexture();
                }
            }

            // Load EmissiveMap
            {
                aiString path;
                material->GetTexture(aiTextureType_EMISSIVE, 0, &path);
                std::filesystem::path FullPath = (m_Path.parent_path() / path.C_Str()).string();

                if (FileExists(FullPath.string())) // RoughnessMap Found
                {
                    mat.UseEmissiveTexture = true;
                    mat.EmissiveTexture.LoadFromFile(FullPath.string());
                }
                else
                {
                    mat.UseEmissiveTexture = false;
                    mat.EmissiveTexture = GL::GLRenderer::GetDefaultBlackTexture();
                }
            }

            m_Materials.push_back(mat);
            MaterialIndex = m_Materials.size() - 1;
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

            GL::GLVertexAttrib Tangent;
            Tangent.ShaderLocation = 2;
            Tangent.Type = GL::GLShaderAttribType::FLOAT;
            Tangent.Count = 3;
            Tangent.Offset = 3 * sizeof(float) + 3 * sizeof(float);

            GL::GLVertexAttrib BiTangent;
            BiTangent.ShaderLocation = 3;
            BiTangent.Type = GL::GLShaderAttribType::FLOAT;
            BiTangent.Count = 3;
            BiTangent.Offset = 3 * sizeof(float) + 3 * sizeof(float) + 3 * sizeof(float);

            GL::GLVertexAttrib TexCoords;
            TexCoords.ShaderLocation = 4;
            TexCoords.Type = GL::GLShaderAttribType::FLOAT;
            TexCoords.Count = 2;
            TexCoords.Offset = 3 * sizeof(float) + 3 * sizeof(float) + 3 * sizeof(float) + 3 * sizeof(float);

            m_VertexArray.AddVertexAttrib(Position);
            m_VertexArray.AddVertexAttrib(Normal);
            m_VertexArray.AddVertexAttrib(Tangent);
            m_VertexArray.AddVertexAttrib(BiTangent);
            m_VertexArray.AddVertexAttrib(TexCoords);
        }

        //m_MaterialTextureDataStorageBuffer.Create(1, m_SubMeshes.size() * sizeof(MaterialGPUShader_TextureBufferData));
        m_MaterialInfoDataStorageBuffer.Create(2, m_SubMeshes.size() * sizeof(MaterialGPUShader_InfoBufferData));
        FillMaterialBuffer();
    }
}
