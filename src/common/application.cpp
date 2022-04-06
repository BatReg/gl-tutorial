#include "application.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stb/stb_image.h>

#include <iostream>

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

Application::Application(const ApplicationCreateInfo& info) noexcept : _info(info)
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

        ProcessInput();

        Update(deltaTime);

        glfwPollEvents();
        glfwSwapBuffers(_window);
    }

    Dispose();
    return 0;
}

bool Application::Init()
{
    stbi_set_flip_vertically_on_load(true);

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

void Application::Update(float deltaTime)
{
}

void Application::Dispose()
{
    glfwTerminate();
}

void Application::ProcessInput()
{
}

void Application::OnKey(int key, int scancode, int action, int mods)
{
}

void Application::OnMouse(double xPos, double yPos)
{
}

void Application::OnScroll(double xOffset, double yOffset)
{
}

bool Application::InitWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_info.windowWidth, _info.windowHeight, _info.title.c_str(), nullptr, nullptr);
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

    glViewport(0, 0, _info.windowWidth, _info.windowHeight);
    glEnable(GL_DEPTH_TEST);

    return true;
}

void Application::OnFramebufferSize(int width, int height)
{
    glViewport(0, 0, width, height);
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->OnFramebufferSize(width, height);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->OnKey(key, scancode, action, mods);
}

static void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->OnMouse(xPos, yPos);
}

static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->OnScroll(xOffset, yOffset);
}
