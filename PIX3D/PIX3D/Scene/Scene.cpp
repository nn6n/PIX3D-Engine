#include "Scene.h"
#include "Systems.h"
#include <Platfrom/GL/GLPixelRenderer2D.h>

namespace PIX3D
{

	Scene::Scene(const std::string& name)
		: m_Name(name)
	{
	}

	uint32_t Scene::AddGameObject(const std::string& name, const Transform& transform)
	{
		const auto entity = m_Registry.create();
		m_Registry.emplace<TagComponent>(entity, name);
		m_Registry.emplace<TransformComponent>(entity, transform.Position, transform.Rotation, transform.Scale);
		
		return (uint32_t)entity;
	}

	uint32_t Scene::AddStaticMesh(const std::string& name, const Transform& transform, const StaticMesh& static_mesh)
	{
		const auto entity = m_Registry.create();
		m_Registry.emplace<TagComponent>(entity, name);
		m_Registry.emplace<TransformComponent>(entity, transform.Position, transform.Rotation, transform.Scale);
		m_Registry.emplace<StaticMeshComponent>(entity, static_mesh);

		return (uint32_t)entity;
	}

	uint32_t Scene::AddSprite(const std::string& name, const Transform& transform, const Sprite& sprite)
	{
		const auto entity = m_Registry.create();
		m_Registry.emplace<TagComponent>(entity, name);
		m_Registry.emplace<TransformComponent>(entity, transform.Position, transform.Rotation, transform.Scale);
		m_Registry.emplace<SpriteComponent>(entity, sprite.Texture, sprite.Color, sprite.TilingFactor);

		return (uint32_t)entity;
	}

	uint32_t Scene::AddPointLight(const std::string& name, const Transform& transform, const glm::vec4& color)
	{
		const auto entity = m_Registry.create();
		m_Registry.emplace<TagComponent>(entity, name);
		m_Registry.emplace<TransformComponent>(entity, transform.Position, transform.Rotation, transform.Scale);
		m_Registry.emplace<PointLightComponent>(entity, color);

		return (uint32_t)entity;
	}

	void Scene::OnStart()
	{
		// editor camera
		m_Cam3D.Init({ 0.0f, 0.0f, 20.0f });

		// cubemap, ibl
		m_Cubemap.LoadHdrToCubemapGPU("res/hdr/barcelona_rooftop.hdr", m_EnvironmentMapSize);
		m_IBLMaps = PIX3D::GL::IBLCubemapsGenerator::GenerateIBLMaps(m_Cubemap.GetHandle(), 32, 128);

		// point lights
		m_PointLightsBuffer.Create(3, MAX_POINT_LIGHTS * sizeof(PointLightShaderBuffer));
	}

	void Scene::OnUpdate(float dt)
	{
		m_Cam3D.Update(dt);

		// Set PointLights Shader Data
		{
			auto view = m_Registry.view<TransformComponent, PointLightComponent>();
			m_PointLightsCount = (int)view.size_hint();


			std::vector<PointLightShaderBuffer> pointlightsdata;


				// Iterate over all point light entities
				view.each([&](TransformComponent& transform, PointLightComponent& pointlight)
					{
						PointLightShaderBuffer data;
						data.LightPosition = glm::vec4(transform.m_Position.x, transform.m_Position.y, transform.m_Position.z, 0.0f);
						data.LightColor = pointlight.m_Color;
						data.Intensity = pointlight.m_Intensity;
						data.Raduis = pointlight.m_Raduis;
						data.Falloff = pointlight.m_Falloff;

						pointlightsdata.push_back(data);
					});

				m_PointLightsBuffer.FillData(pointlightsdata.data(), pointlightsdata.size() * sizeof(PointLightShaderBuffer));
		}
	}

	void Scene::OnRender()
	{
		// Static Meshes && Skybox
		{
			PIX3D::GL::GLRenderer::Begin(m_Cam3D);


			// Sprites
			{
				PIX3D::GL::GLPixelRenderer2D::Begin(m_Cam3D);

				SpriteRendererSystem::Render(m_Registry);

				// Render PointLights Gizmo Sprite
				{
					// Query all entities that have both TransformComponent and SpriteComponent
					auto view = m_Registry.view<TransformComponent, PointLightComponent>();

					// Iterate over all matching entities
					view.each([&](TransformComponent& transform, PointLightComponent& pointlight)
						{
							transform.m_Scale.x = 0.1f;
							transform.m_Scale.y = 0.1f;
							transform.m_Scale.z = 0.1f;
							PIX3D::GL::GLPixelRenderer2D::DrawTexturedQuad(GL::GLPixelRenderer2D::GetPointLightGizmoIcon(), transform, pointlight.m_Color, 1.0f);
						});
				}

				PIX3D::GL::GLPixelRenderer2D::End();
			}


			StaticMeshRendererSystem::Render(m_Registry, m_IBLMaps, m_PointLightsCount);
			GL::GLRenderer::RenderHdrSkybox(m_CubemapTransform, m_Cubemap);

			PIX3D::GL::GLRenderer::End();
		}
	}
}
