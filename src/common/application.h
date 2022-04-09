#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>

struct ApplicationCreateInfo
{
    std::string title = "OpenGL application";
    int windowHeight = 720;
    int windowWidth = 1280;
};

class Application
{
public:
    Application() = delete;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(const ApplicationCreateInfo& info) noexcept;
    int Run();

protected:
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void Dispose();

    virtual void OnKey(int key, int scancode, int action, int mods);
    virtual void OnMouse(double xPos, double yPos);
    virtual void OnScroll(double xOffset, double yOffset);

private:
    bool InitWindow();
    bool InitGL();
    void OnFramebufferSize(int width, int height);

    friend void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    friend void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void MouseCallback(GLFWwindow* window, double xPos, double yPos);
    friend void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

protected:
    GLFWwindow* _window{};

    std::string _title;
    int _windowWidth;
    int _windowHeight;

private:
    bool _isRunning{};
    float _lastTime{};
};
