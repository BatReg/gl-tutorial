#include "pipeline.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

bool Shader::Create(const ShaderCreateInfo& info)
{
    std::ifstream inputStream;
    std::stringstream stringStream;

    inputStream.open(info.path);

    if(inputStream.fail())
    {
        std::cout << "ERROR::SHADER::" << info.path << "::FAILED_TO_OPEN_FILE" << std::endl;
        id = 0;
        return false;
    }

    stringStream << inputStream.rdbuf();
    inputStream.close();

    std::string shaderString = stringStream.str();
    const char* shaderSource = shaderString.c_str();

    id = glCreateShader(FromShaderTypeToEnum(info.type));
    glShaderSource(id, 1, &shaderSource, nullptr);
    glCompileShader(id);

    int success;
    char infoLog[512];

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::" << info.path << "::COMPILATION_FAILED\n" << infoLog << std::endl;

        Dispose();

        return false;
    }

    return true;
}

void Shader::Dispose()
{
    if(IsValid())
    {
        glDeleteShader(id);
        id = 0;
    }
}

bool Shader::IsValid() const
{
    return id != 0;
}

GLuint Shader::GetId() const
{
    return id;
}

bool Pipeline::Create(const PipelineCreateInfo& info)
{
    id = glCreateProgram();
    glAttachShader(id, info.vertexShader.GetId());
    glAttachShader(id, info.fragmentShader.GetId());

    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;

        Dispose();

        return false;
    }

    return true;
}

void Pipeline::Dispose()
{
    bool isValid = IsValid();
    if(!isValid)
    {
        return;
    }

    id = 0;
}

void Pipeline::SetActive()
{
    glUseProgram(id);
}

GLuint Pipeline::GetId() const
{
    return id;
}

bool Pipeline::IsValid() const
{
    return id != 0;
}

void Pipeline::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Pipeline::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Pipeline::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Pipeline::SetMatrix4x4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

GLenum FromShaderTypeToEnum(ShaderType type)
{
    switch(type)
    {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        default:
            return -1;
    }
}
