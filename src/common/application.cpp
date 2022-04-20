#include "application.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

Application::Application(const ApplicationCreateInfo& info) noexcept : 
    _title(info.title), _windowWidth(info.windowWidth), _windowHeight(info.windowHeight)
{
}

int Application::Run()
{
    if(!Init())
    {
        std::cout << "Application initialization failed!" << std::endl;
        return -1;
    }

    _isRunning = true;

    float deltaTime = 0;
    while(_isRunning)
    {
        _isRunning = !glfwWindowShouldClose(_window);

        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - _lastTime;
        _lastTime = currentTime;

        Update(deltaTime);

        glfwPollEvents();
        glfwSwapBuffers(_window);
    }

    Dispose();
    return 0;
}

bool Application::Init()
{
    if(!InitWindow())
    {
        std::cout << "Failed to initialize window" << std::endl;
        return false;
    }

    if(!InitGL())
    {
        std::cout << "Failed to initialize OpenGL" << std::endl;
        return false;
    }

    return true;
}

void Application::Update(float)
{
}

void Application::Dispose()
{
    glfwTerminate();
}

void Application::OnKey(int, int, int, int)
{
}

void Application::OnMouse(double, double)
{
}

void Application::OnScroll(double, double)
{
}

bool Application::InitWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_windowWidth, _windowHeight, _title.c_str(), nullptr, nullptr);
    if(_window == nullptr)
    {
        std::cout << "Failed to create a GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, reinterpret_cast<void*>(this));
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(_window, FramebufferSizeCallback);
    glfwSetKeyCallback(_window, KeyCallback);
    glfwSetCursorPosCallback(_window, MouseCallback);
    glfwSetScrollCallback(_window, ScrollCallback);

    return true;
}

bool Application::InitGL()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initalize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, _windowWidth, _windowHeight);
    glEnable(GL_DEPTH_TEST);

    return true;
}

void Application::OnFramebufferSize(int width, int height)
{
    glViewport(0, 0, width, height);
    _windowWidth = width;
    _windowHeight = height;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->OnFramebufferSize(width, height);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->OnKey(key, scancode, action, mods);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->OnMouse(xPos, yPos);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->OnScroll(xOffset, yOffset);
}
