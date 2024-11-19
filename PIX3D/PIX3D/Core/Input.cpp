#include "Input.h"
#include "Application.h"

namespace PIX3D
{
    bool Input::IsKeyPressed(KeyCode keycode)
    {
        return Application::Get().GetPlatformLayer()->IsKeyPressed((int)keycode);
    }

    bool Input::IsKeyReleased(KeyCode keycode)
    {
        return Application::Get().GetPlatformLayer()->IsKeyReleased((int)keycode);
    }

    bool Input::IsMouseButtonPressed(MouseButton mousebutton)
    {
        return Application::Get().GetPlatformLayer()->IsMouseButtonPressed((int)mousebutton);
    }

    bool Input::IsMouseButtonReleased(MouseButton mousebutton)
    {
        return Application::Get().GetPlatformLayer()->IsMouseButtonReleased((int)mousebutton);
    }

    void Input::ResetInput()
    {
        MouseOffset = { 0.0f, 0.0f };
        MouseScroll = { 0.0f, 0.0f };
    }
}
