#include "lighting.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(0));
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
    ProcessInput(deltaTime);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // _lightPos.x = sin(static_cast<float>(glfwGetTime()));
    // _lightPos.z = cos(static_cast<float>(glfwGetTime()));

    lightingShader.SetActive();    
    
    lightingShader.SetVec3("material.ambient", { 1.0f, 0.5f, 0.31f });
    lightingShader.SetVec3("material.diffuse", { 1.0f, 0.5f, 0.31f });
    lightingShader.SetVec3("material.specular", { 0.5f, 0.5f, 0.5f });
    lightingShader.SetFloat("material.shininess", 32.0f);

    float glfwTime = static_cast<float>(glfwGetTime());
    glm::vec3 lightColor{};
    lightColor.x = sin(glfwTime * 2.0f);
    lightColor.y = sin(glfwTime * 0.7f);
    lightColor.z = sin(glfwTime * 1.3f);

    glm::vec3 lightDiffuseColor = lightColor * glm::vec3{0.5f};
    glm::vec3 lightAmbientColor = lightDiffuseColor * glm::vec3{0.2f};

    lightingShader.SetVec3("light.ambient",  lightAmbientColor);
    lightingShader.SetVec3("light.diffuse",  lightDiffuseColor); 
    lightingShader.SetVec3("light.specular", { 1.0f, 1.0f, 1.0f });     
    lightingShader.SetVec3("light.position", _lightPos);

    lightingShader.SetVec3("viewPos", camera.Position);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_windowWidth / (float)_windowHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader.SetMatrix4x4("projection", projection);
    lightingShader.SetMatrix4x4("view", view);

    glm::mat4 model = glm::mat4{ 1.0f };
    glm::mat4 model_IT = glm::transpose(glm::inverse(model));
    lightingShader.SetMatrix4x4("model", model);
    lightingShader.SetMatrix4x4("model_IT", model_IT);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightCubeShader.SetActive();
    lightCubeShader.SetMatrix4x4("projection", projection);
    lightCubeShader.SetMatrix4x4("view", view);
    lightCubeShader.SetVec3("lightDiffuse", lightDiffuseColor);

    model = glm::mat4{ 1.0f };
    model = glm::translate(model, _lightPos);
    model = glm::scale(model, glm::vec3{ 0.2f });
    lightCubeShader.SetMatrix4x4("model", model);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void LightingTutorial::Dispose()
{
    lightCubeShader.Dispose();
    lightingShader.Dispose();

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cubeVAO);

    Application::Dispose();
}

void LightingTutorial::OnKey(int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(_window, true);
    }
}

void LightingTutorial::OnMouse(double xPosIn, double yPosIn)
{
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if(_firstMouse)
    {
        _lastX = xPos;
        _lastY = yPos;
        _firstMouse = false;
    }

    float xOffset = xPos - _lastX;
    float yOffset = _lastY - yPos;

    _lastX = xPos;
    _lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void LightingTutorial::OnScroll(double xOffset, double yOffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

void LightingTutorial::ProcessInput(float deltaTime)
{
    const float cameraSpeed = 10.0f * deltaTime;
    if(glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    }

    if(glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    }

    if(glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    }

    if(glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
    }
}
