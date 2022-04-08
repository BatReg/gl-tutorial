#include "lighting.h"

#include <glad/glad.h>

#include <iostream>

bool LightingTutorial::Init()
{
    if(!Application::Init())
    {
        return false;
    }

    InitGLBuffers();

    if(!InitShaders())
    {
        std::cout << "Failed to initialize shaders" << std::endl;
        return false;
    }

    return true;
}

void LightingTutorial::InitGLBuffers()
{
    glGenVertexArrays(1, &cubeVAO);
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<void*>(0));
    glEnableVertexAttribArray(0);
}

bool LightingTutorial::InitShaders()
{
    ShaderCreateInfo vertexShaderInfo{};
    vertexShaderInfo.type = ShaderType::Vertex;
    vertexShaderInfo.path = "./shaders/lighting.vs";

    Shader vertexShader{};
    if(!vertexShader.Create(vertexShaderInfo))
    {
        return false;
    }

    ShaderCreateInfo fragmentShaderInfo{};
    fragmentShaderInfo.type = ShaderType::Fragment;
    fragmentShaderInfo.path = "./shaders/lighting.fs";

    Shader fragmentShader;
    if(!fragmentShader.Create(fragmentShaderInfo))
    {
        return false;
    }

    PipelineCreateInfo lightingPipelineCreateInfo = PipelineCreateInfo(vertexShader, fragmentShader);
    if(!lightingShader.Create(lightingPipelineCreateInfo))
    {
        return false;
    }

    fragmentShader.Dispose();
    vertexShader.Dispose();

    vertexShaderInfo.path = "./shaders/light_cube.vs";
    if(!vertexShader.Create(vertexShaderInfo))
    {
        return false;
    }

    fragmentShaderInfo.path = "./shaders/light_cube.fs";
    if(!fragmentShader.Create(fragmentShaderInfo))
    {
        return false;
    }

    PipelineCreateInfo lightCubePipelineCreateInfo = PipelineCreateInfo(vertexShader, fragmentShader);
    if(!lightCubeShader.Create(lightCubePipelineCreateInfo))
    {
        return false;
    }

    fragmentShader.Dispose();
    vertexShader.Dispose();

    return true;
}

void LightingTutorial::Update(float deltaTime)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LightingTutorial::OnKey(int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(_window, true);
    }
}
