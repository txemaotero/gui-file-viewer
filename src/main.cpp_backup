#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "pch.h"
#include "render/render.h"
#include "shaders/shader.h"


// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int createWindow(GLFWwindow*& window)
{
    glfwSetErrorCallback([](int error, const char* description)
                         { fprintf(stderr, "GLFW Error (%d): %s\n", error, description); });
    // Initialize GLFW
    if (!glfwInit())
    {
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
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    return 0;
}

int main()
{
    GLFWwindow* window;
    if (createWindow(window) != 0)
    {
        return -1;
    }

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
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
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
    };

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

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

    Shader* shader = Shader::FromTextFiles("src/shaders/sources/vertexShader.shader", "src/shaders/sources/fragmentShader.shader");


    glUseProgram(shader->GetRendererID());

    float triangleColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Color Picker");
        if (ImGui::ColorPicker4("Triangle Color", triangleColor))
        {
        }
        ImGui::End();


        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUseProgram(shader->GetRendererID()));

        int triangleColorLocation = glGetUniformLocation(shader->GetRendererID(), "triangleColor");
        GLCall(
            glUniform4f(triangleColorLocation, triangleColor[0], triangleColor[1], triangleColor[2], triangleColor[3])
        );

        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // 6 is the number of indices

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete shader;

    // Cleanup and exit
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
