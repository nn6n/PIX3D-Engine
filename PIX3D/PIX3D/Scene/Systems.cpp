#include "Systems.h"
#include "Components.h"
#include <Platfrom/GL/GLPixelRenderer2D.h>
#include <Platfrom/GL/GLRenderer.h>

namespace PIX3D
{

    void StaticMeshRendererSystem::Render(Scene* scene, entt::registry& registry, GL::IBLMaps& ibl_maps, int point_lights_count)
    {

        // Query all entities that have both TransformComponent and StaticMeshComponent
        auto view = registry.view<TransformComponent, StaticMeshComponent>();

        // Iterate over all matching entities
        view.each([&](TransformComponent& transform, StaticMeshComponent& mesh)
            {
                PIX3D::GL::GLRenderer::RenderMesh(scene->m_DirLight, transform.GetTransformMatrix(), mesh.m_Mesh, ibl_maps, point_lights_count);
            });
    }

    void SpriteRendererSystem::Render(entt::registry& registry)
    {
        // Query all entities that have both TransformComponent and SpriteComponent
        auto view = registry.view<TransformComponent, SpriteComponent>();

        // Iterate over all matching entities
        view.each([&](TransformComponent& transform, SpriteComponent& sprite)
            {
                // Assuming you have a RenderSprite method in your renderer
                if(sprite.m_Texture.IsValid())
                    PIX3D::GL::GLPixelRenderer2D::DrawTexturedQuad(sprite.m_Texture, transform, sprite.m_Color, sprite.m_TilingFactor, sprite.flip);
                else
                    PIX3D::GL::GLPixelRenderer2D::DrawSmoothQuad(transform, sprite.m_Color);
            });
    }
}
