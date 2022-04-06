#include <application.h>
#include <entry_point.h>

#include <iostream>

class LightingTutorial : public Application
{
public:
    LightingTutorial(const ApplicationCreateInfo& info) : Application::Application(info)
    {
    };

protected:
    void Update(float deltaTime) override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OnKey(int key, int scancode, int action, int mods) override
    {
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(_window, true);
        }
    }
};

Application* CreateApplication()
{
    ApplicationCreateInfo info{};
    info.title = "Lighting";

    return new LightingTutorial(info);
}
