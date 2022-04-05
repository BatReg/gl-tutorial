#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include <string>

enum class ShaderType
{
    Vertex,
    Fragment
};

struct ShaderCreateInfo
{
    ShaderType type;
    std::string path;
};

class Shader
{
public:
    bool Create(const ShaderCreateInfo& info);
    void Dispose();

    bool IsValid() const;
    GLuint GetId() const;

private:
    GLuint id;
};

struct PipelineCreateInfo
{
    Shader& vertexShader;
    Shader& fragmentShader;
};

class Pipeline
{
public:
    bool Create(const PipelineCreateInfo& info);
    void Dispose();
    void SetActive();

    GLuint GetId() const;
    bool IsValid() const;
    
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetMatrix4x4(const std::string& name, const glm::mat4& value) const;

private:
    GLuint id;
};

static GLenum FromShaderTypeToEnum(ShaderType type);