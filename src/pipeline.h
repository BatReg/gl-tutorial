#pragma once

#include <glad/glad.h>

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

private:
    GLuint id;
};

static GLenum FromShaderTypeToEnum(ShaderType type);