#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <string>
#include <fstream>

#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GlClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));


static void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cerr << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}


// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    // Pass the shader source code to OpenGL
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
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
    if (shaderFile.is_open()) {
        std::string line;
        while (getline(shaderFile, line)) {
            shaderCode += "\n" + line;
        }
        shaderFile.close();
    } else {
        std::cout << "Unable to open file " << path << std::endl;
    }

    return shaderCode;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);


    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int createWindow(GLFWwindow*& window)
{
    glfwSetErrorCallback(
      [](int error, const char* description) { fprintf(stderr, "GLFW Error (%d): %s\n", error, description); });
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLApp", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    return 0;
}

int
main()
{
    GLFWwindow* window;
    if (createWindow(window) != 0) {
        return -1;
    }

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    float clearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };

    float positions[] = { 
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

    unsigned int vao; // Vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    GLCall(glEnableVertexAttribArray(0));


    unsigned int ibo; // Index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    std::string vertexShader = ReadShader("../src/shaders/vertexShader.shader");
    std::string fragmentShader = ReadShader("../src/shaders/fragmentShader.shader");

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    float triangleColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Color Picker");
        if (ImGui::ColorPicker4("Triangle Color", triangleColor)) {
        }
        ImGui::End();


        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUseProgram(shader));

        int triangleColorLocation = glGetUniformLocation(shader, "triangleColor");
        GLCall(glUniform4f(triangleColorLocation, triangleColor[0], triangleColor[1], triangleColor[2], triangleColor[3]));

        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // 6 is the number of indices

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glDeleteProgram(shader);

    // Cleanup and exit
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
