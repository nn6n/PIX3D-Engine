#include "InspectorWidget.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace
{
    static void DrawVec3Control(const std::string& lable, glm::vec3& value, float resetvalue = 0.0f, float Speed = 0.01f, float colwidth = 80.0f)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFont* BoldFont = io.Fonts->Fonts[0];

        ImGui::PushID(lable.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colwidth);
        ImGui::Text(lable.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 ButtonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, { 0.7f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.4f, 0.4f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(BoldFont);
        if (ImGui::Button("X", ButtonSize))
            value.x = resetvalue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &value.x, Speed);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, { 0.15f, 0.8f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.4f, 0.9f, 0.4f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.8f, 0.15f, 1.0f });
        ImGui::PushFont(BoldFont);
        if (ImGui::Button("Y", ButtonSize))
            value.y = resetvalue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &value.y, Speed);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, { 0.15f, 0.1f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.4f, 0.4f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.1f, 0.8f, 1.0f });
        ImGui::PushFont(BoldFont);
        if (ImGui::Button("Z", ButtonSize))
            value.z = resetvalue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &value.z, Speed);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);
        ImGui::PopID();
    }
}

void InspectorWidget::OnRender()
{
    ImGui::Begin("Inspector");

    auto selectedEntity = m_HierarchyWidget->GetSelectedEntity();
    if (selectedEntity != entt::null)
    {
        // Tag Component
        if (auto* tag = m_Scene->m_Registry.try_get<TagComponent>(selectedEntity))
        {
            char buffer[256];
            strcpy_s(buffer, sizeof(buffer), tag->m_Tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
            {
                tag->m_Tag = std::string(buffer);
            }
        }

        ImGui::SameLine();

        // Add Component Button
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponentPopup");
        }

        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            if (ImGui::MenuItem("Transform"))
            {
                if (!m_Scene->m_Registry.try_get<TransformComponent>(selectedEntity))
                    m_Scene->m_Registry.emplace<TransformComponent>(selectedEntity);
            }
            if (ImGui::MenuItem("Static Mesh"))
            {
                if (!m_Scene->m_Registry.try_get<StaticMeshComponent>(selectedEntity))
                    m_Scene->m_Registry.emplace<StaticMeshComponent>(selectedEntity);
            }
            if (ImGui::MenuItem("Sprite"))
            {
                if (!m_Scene->m_Registry.try_get<SpriteComponent>(selectedEntity))
                    m_Scene->m_Registry.emplace<SpriteComponent>(selectedEntity);
            }
            if (ImGui::MenuItem("Point Light"))
            {
                if (!m_Scene->m_Registry.try_get<PointLightComponent>(selectedEntity))
                    m_Scene->m_Registry.emplace<PointLightComponent>(selectedEntity);
            }
            ImGui::EndPopup();
        }

        // Transform Component
        if (auto* transform = m_Scene->m_Registry.try_get<TransformComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                DrawVec3Control("Position", transform->m_Position);
                DrawVec3Control("Rotation", transform->m_Rotation, 0.0f, 0.1f);
                DrawVec3Control("Scale", transform->m_Scale);
            }
        }

        // Static Mesh Component
        if (auto* mesh = m_Scene->m_Registry.try_get<StaticMeshComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("Static Mesh", ImGuiTreeNodeFlags_DefaultOpen))
            {
                // Add mesh properties like path, scale factor etc.
                if (ImGui::Button("Load Mesh"))
                {
                    auto* platform = PIX3D::Engine::GetPlatformLayer();
                    std::filesystem::path filepath = platform->OpenDialogue(PIX3D::FileDialougeFilter::GLTF);
                    if (!filepath.empty())
                    {
                        mesh->m_Mesh.Load(filepath.string(), 1.0f);
                    }
                }
            }
        }

        // Sprite Component
        if (auto* sprite = m_Scene->m_Registry.try_get<SpriteComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::ColorEdit4("Color", &sprite->m_Color.x);
                ImGui::DragFloat("Tiling Factor", &sprite->m_TilingFactor, 0.1f, 0.0f, 100.0f);

                // Texture preview and change button
                ImVec2 availableRegion = ImGui::GetContentRegionAvail();
                if (sprite->m_Texture.GetHandle())
                {
                    ImGui::Image((ImTextureID)sprite->m_Texture.GetHandle(),
                        { 256.0f, 256.0f }, { 0, 0 }, { 1, 1 });
                }

                if (ImGui::Button("Set Texture"))
                {
                    auto* platform = PIX3D::Engine::GetPlatformLayer();
                    std::filesystem::path filepath = platform->OpenDialogue(PIX3D::FileDialougeFilter::PNG);
                    if (!filepath.empty())
                    {
                        sprite->m_Texture.LoadFromFile(filepath.string(), true);
                    }
                }
                ImGui::SameLine();
                ImGui::Checkbox("Flip", &sprite->flip);
            }
        }

        // point light component
        if (auto* pointlight = m_Scene->m_Registry.try_get<PointLightComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::ColorEdit4("Color", &pointlight->m_Color.x);
                ImGui::SliderFloat("Intensity", &pointlight->m_Intensity, 0.0f, 100.0f);
                ImGui::SliderFloat("Raduis", &pointlight->m_Raduis, 0.0f, 20.0f);
                ImGui::SliderFloat("FallOff", &pointlight->m_Falloff, 0.0f, 5.0f);
            }
        }
    }

    ImGui::End();
}
