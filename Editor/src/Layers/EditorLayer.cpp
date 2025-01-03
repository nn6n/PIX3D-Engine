#include "EditorLayer.h"
#include <imgui.h>

void EditorLayer::OnStart()
{
    // Initialize the scene
    m_Scene = new PIX3D::Scene("IBL Scene");
    m_Scene->OnStart();

    // Initialize editor widgets
    m_LightningWidget = new LightningWidget(m_Scene);
    m_HierarchyWidget = new HierarchyWidget(m_Scene);
    m_InspectorWidget = new InspectorWidget(m_Scene, m_HierarchyWidget);
    m_MaterialWidget = new MaterialWidget(m_Scene, m_HierarchyWidget);

    auto& CurrentProj = Engine::GetCurrentProjectRef();
    int x = 0;
}

void EditorLayer::OnUpdate(float dt)
{
    // Update the scene
    m_Scene->OnUpdate(dt);

    // Handle input for mouse cursor visibility
    if (PIX3D::Input::IsKeyPressed(PIX3D::KeyCode::RightShift))
        ShowMouseCursor = false;
    else if (PIX3D::Input::IsKeyPressed(PIX3D::KeyCode::Escape))
        ShowMouseCursor = true;

    PIX3D::Engine::GetPlatformLayer()->ShowCursor(ShowMouseCursor);

    // Render the scene
    m_Scene->OnRender();

    // Render UI
    RenderMenuBar();
    RenderWidgets();
}

void EditorLayer::OnDestroy()
{
    delete m_LightningWidget;
    m_LightningWidget = nullptr;
    delete m_HierarchyWidget;
    m_HierarchyWidget = nullptr;
    delete m_InspectorWidget;
    m_InspectorWidget = nullptr;
    delete m_MaterialWidget;
    m_MaterialWidget = nullptr;
    delete m_Scene;
    m_Scene = nullptr;
}

void EditorLayer::RenderMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Lightning"))
                m_ShowLightningWidget = !m_ShowLightningWidget;
            if (ImGui::MenuItem("Hierarchy"))
                m_ShowHierarchyWidget = !m_ShowHierarchyWidget;
            if (ImGui::MenuItem("Inspector"))
                m_ShowInspectorWidget = !m_ShowInspectorWidget;
            if (ImGui::MenuItem("Material"))
                m_ShowMaterialWidget = !m_ShowMaterialWidget;

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void EditorLayer::RenderWidgets()
{
    if (m_ShowLightningWidget)
        m_LightningWidget->OnRender();
    if (m_ShowHierarchyWidget)
        m_HierarchyWidget->OnRender();
    if (m_ShowInspectorWidget)
        m_InspectorWidget->OnRender();
    if (m_ShowMaterialWidget)
        m_MaterialWidget->OnRender();
}
