#pragma once
#include <Graphics/Mesh.h>
#include <Platfrom/GL/GLRenderer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PIX3D
{
	struct TagComponent
	{
	public:
		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string tag)
			: m_Tag(tag)
		{
		}

		std::string m_Tag = "";
	};


	struct TransformComponent
	{
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: m_Position(position), m_Rotation(rotation), m_Scale(scale)
		{
		}

		operator const glm::mat4() const
		{
			return GetTransformMatrix();
		}

		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 Transform = glm::mat4(1.0f);
			Transform = glm::translate(Transform, m_Position);
			Transform = glm::rotate(Transform, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			Transform = glm::rotate(Transform, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			Transform = glm::rotate(Transform, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			Transform = glm::scale(Transform, m_Scale);
			return Transform;

			/*
			return glm::translate(glm::mat4(1.0f), m_Position) *
				glm::toMat4(glm::quat(glm::vec3(glm::radians(m_Rotation.x), glm::radians(m_Rotation.y), glm::radians(m_Rotation.z)))) *
				glm::scale(glm::mat4(1.0f), m_Scale);
			*/

		}

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
	};

	struct SpriteComponent
	{
	public:
		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent& other) = default;
		SpriteComponent(const GL::GLTexture& texture, const glm::vec4& color, float tilingfactor)
			: m_Texture(texture), m_Color(color)
		{
		}

		SpriteComponent(const glm::vec4& color)
			: m_Color(color)
		{
		}

		GL::GLTexture m_Texture;
		glm::vec4 m_Color = { 1.0, 1.0, 1.0, 1.0 };
		float m_TilingFactor = 1.0f;
		bool flip = false;
	};


	struct StaticMeshComponent
	{
	public:
		StaticMeshComponent() = default;
		StaticMeshComponent(const StaticMeshComponent& other) = default;
		StaticMeshComponent(const StaticMesh& static_mesh)
			: m_Mesh(static_mesh)
		{
		}

		StaticMesh m_Mesh;
	};

	struct PointLightComponent
	{
	public:
		PointLightComponent() = default;
		PointLightComponent(const PointLightComponent& other) = default;
		PointLightComponent(const glm::vec4& color)
			: m_Color(color)
		{
		}

		glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float m_Intensity = 1.0f;
		float m_Raduis = 1.0f;
		float m_Falloff = 1.0f;
	};
}
