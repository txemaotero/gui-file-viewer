#include <GLFW/glfw3.h>
#include <string>

class Shader
{
public:
    ~Shader();

    GLuint GetRendererID() { return m_RendererID; }

    static Shader* FromTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

private:
    Shader() = default;

    void LoadFromTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    GLuint CompileShader(GLenum type, const std::string& source);

private:
    GLuint m_RendererID;
};
