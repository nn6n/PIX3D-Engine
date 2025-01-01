#pragma once
#include <Utils/entt.hpp>
#include <Platfrom/GL/IBL/IBLCubemapsGenerator.h>
#include "Scene.h"

namespace PIX3D
{

    class StaticMeshRendererSystem
    {
    public:
        static void Render(Scene* scene, entt::registry& registry, GL::IBLMaps& ibl_maps, int point_lights_count);
    };

    class SpriteRendererSystem
    {
    public:
        static void Render(entt::registry& registry);
    };
}
