#include <fstream>
#include <iostream>

#include "shader.h"
#include "utils.h"

static GLuint CompileShader(GLuint type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();

    // Pass the shader source code to OpenGL
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        // Get the length of the error message
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // Get the error message
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        // Print the error message
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!"
                  << std::endl;
        std::cout << message << std::endl;

        // Delete the shader
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static std::string ReadShader(const std::string& path)
{
    std::ifstream shaderFile(path, std::ios::in);

    std::string shaderCode;
    if (shaderFile.is_open())
    {
        std::string line;
        while (getline(shaderFile, line))
        {
            shaderCode += "\n" + line;
        }
        shaderFile.close();
    }
    else
    {
        std::cout << "Unable to open file " << path << std::endl;
    }

    return shaderCode;
}

Shader* Shader::FromTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    Shader* shader = new Shader();
    shader->LoadFromTextFiles(vertexShaderPath, fragmentShaderPath);
    return shader;
}

void Shader::LoadFromTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vertexSource = ReadShader(vertexShaderPath);
    std::string fragmentSource = ReadShader(fragmentShaderPath);

    GLuint program = glCreateProgram();
    int glShaderIDIndex = 0;

    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    glAttachShader(program, vertexShader);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        LOG_ERROR("%s", infoLog.data());
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_RendererID = program;
}

GLuint Shader::CompileShader(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);

    const GLchar* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, 0);

    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(shader);

        LOG_ERROR("%s", infoLog.data());
    }

    return shader;
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}
