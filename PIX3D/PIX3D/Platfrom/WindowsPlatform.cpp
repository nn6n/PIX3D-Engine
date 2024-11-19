#include "WindowsPlatform.h"
#include <GLFW/glfw3.h>
#include <Core/Core.h>
#include <Core/Application.h>
#include <Platfrom/GL/GLCommands.h>
#include <Core/Input.h>

namespace
{
    bool FirstMouse = true;

    void WindowResizeCallBack(GLFWwindow* window, int width, int height)
    {
        auto& App = PIX3D::Application::Get();
        App.SetWidth(width);
        App.SetHeight(height);
        PIX3D::GL::GLCommands::SetViewPort(width, height);

        auto func = PIX3D::Application::Get().GetWindowSizeCallBackFunc();
        if(func)
            func(width, height);
    }

    void KeyboardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
    { 
        auto func = PIX3D::Application::Get().GetKeyboardCallBackFuncPtr();
        if(func)
            func((PIX3D::KeyCode)key);
    }

    void CursorPositionCallBack(GLFWwindow* window, double xpos, double ypos)
    {
        if (FirstMouse)
        {
            PIX3D::Input::SetMousePosition(glm::vec2(xpos, ypos));
            FirstMouse = false;
        }

        auto lastmouse = PIX3D::Input::GetMousePosition();

        float xoffset = xpos - lastmouse.x;
        float yoffset = lastmouse.y - ypos;

        PIX3D::Input::SetMouseOffset(glm::vec2(xoffset, yoffset));
        PIX3D::Input::SetMousePosition(glm::vec2(xpos, ypos));
    }

    void MouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
    {
        PIX3D::Input::SetMouseScroll(glm::vec2(xoffset, yoffset));
    }
}

namespace PIX3D
{
    void WindowsPlatformLayer::CreatWindow(uint32_t width, uint32_t height, const char* title)
    {
        if (!glfwInit())
            std::cout << "Failed To Initialize GLFW!\n";

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 8);

        m_NativeWindowHandel = glfwCreateWindow(width, height, title, NULL, NULL);

        PIX_ASSERT(m_NativeWindowHandel);

        glfwSetWindowSizeCallback((GLFWwindow*)m_NativeWindowHandel, WindowResizeCallBack);
        glfwSetKeyCallback((GLFWwindow*)m_NativeWindowHandel, KeyboardCallBack);
        glfwSetCursorPosCallback((GLFWwindow*)m_NativeWindowHandel, CursorPositionCallBack);
        glfwSetScrollCallback((GLFWwindow*)m_NativeWindowHandel, MouseScrollCallBack);
    }

    void WindowsPlatformLayer::PollEvents()
    {
        glfwPollEvents();
    }

    void* WindowsPlatformLayer::GetNativeWindowHandel()
    {
        return m_NativeWindowHandel;
    }
    std::pair<uint32_t, uint32_t> WindowsPlatformLayer::GetWindowSize()
    {
        int w, h;
        glfwGetWindowSize((GLFWwindow*)m_NativeWindowHandel, &w, &h);

        return { w, h };
    }

    bool WindowsPlatformLayer::IsKeyPressed(int keycode)
    {
        bool pressed = glfwGetKey((GLFWwindow*)m_NativeWindowHandel, keycode) == GLFW_PRESS;
        return pressed;
    }

    bool WindowsPlatformLayer::IsKeyReleased(int keycode)
    {
        bool released = glfwGetKey((GLFWwindow*)m_NativeWindowHandel, keycode) == GLFW_RELEASE;
        return released;
    }

    bool WindowsPlatformLayer::IsMouseButtonPressed(int mousebutton)
    {
        bool pressed = glfwGetMouseButton((GLFWwindow*)m_NativeWindowHandel, mousebutton) == GLFW_PRESS;
        return pressed;
    }

    bool WindowsPlatformLayer::IsMouseButtonReleased(int mousebutton)
    {
        bool released = glfwGetMouseButton((GLFWwindow*)m_NativeWindowHandel, mousebutton) == GLFW_RELEASE;
        return released;
    }
}
