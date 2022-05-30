#include "lighting.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

static bool LoadTexture(const std::string& imagePath, GLuint* texture);

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

    if(!InitTextures())
    {
        std::cout << "Failed to initialize textures" << std::endl;
        return false;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(0));
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

bool LightingTutorial::InitTextures()
{    
    std::string diffuseTexturePath{ "./assets/textures/container2.png" };
    std::string specularTexturePath{ "./assets/textures/container2_specular.png" };

    if(!LoadTexture(diffuseTexturePath, &diffuseMap))
    {
        std::cout << "Failed to load texture: " << diffuseTexturePath << std::endl;
        return false;
    }

    if(!LoadTexture(specularTexturePath, &specularMap))
    {
        std::cout << "Failed to load texture: " << specularTexturePath << std::endl;
        return false;
    }

    return true;
}

void LightingTutorial::Update(float deltaTime)
{
    ProcessInput(deltaTime);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_windowWidth / (float)_windowHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    DrawObjects(projection, view);
    // DrawLights(projection, view);
}

void LightingTutorial::DrawObjects(const glm::mat4& proj, const glm::mat4& view)
{
    lightingShader.SetActive();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    lightingShader.SetInt("material.diffuse", 0);
    lightingShader.SetInt("material.specular", 1);
    lightingShader.SetFloat("material.shininess", 32.0f);

    lightingShader.SetVec3("light.ambient",  { 0.2f, 0.2f, 0.2f });
    lightingShader.SetVec3("light.diffuse",  { 0.5f, 0.5f, 0.5f });
    lightingShader.SetVec3("light.specular", { 1.0f, 1.0f, 1.0f });
    lightingShader.SetVec3("light.direction", {-0.2f, -1.0f, -0.3f});

    lightingShader.SetVec3("viewPos", camera.Position);

    lightingShader.SetMatrix4x4("projection", proj);
    lightingShader.SetMatrix4x4("view", view);

    glBindVertexArray(cubeVAO);
    for(size_t i = 0; i < cubePositions.size(); i++)
    {
        glm::mat4 model = glm::mat4{ 1.0f };
        model = glm::translate(model, cubePositions[i]);

        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3{1.0f, 0.3f, 0.5f});

        glm::mat4 model_IT = glm::transpose(glm::inverse(model));

        lightingShader.SetMatrix4x4("model", model);
        lightingShader.SetMatrix4x4("model_IT", model_IT);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void LightingTutorial::DrawLights(const glm::mat4& proj, const glm::mat4& view)
{
    lightCubeShader.SetActive();
    lightCubeShader.SetMatrix4x4("projection", proj);
    lightCubeShader.SetMatrix4x4("view", view);
    lightCubeShader.SetVec3("lightDiffuse", { 1.0f, 1.0f, 1.0f });

    glm::mat4 model = glm::mat4{ 1.0f };
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

bool LoadTexture(const std::string& imagePath, GLuint* texture)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width;
    int height;
    int nrChannels;
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);

    GLenum format{};
    if(nrChannels == 1)
    {
        format = GL_RED;
    }
    else if (nrChannels == 3)
    {
        format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
        format = GL_RGBA;
    }

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        stbi_image_free(data);
        return false;
    }

    return true;
}
