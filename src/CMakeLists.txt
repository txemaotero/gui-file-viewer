# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Glad REQUIRED)
find_package(imgui REQUIRED)

# Add your main executable
add_executable(OpenGLApp main.cpp)

# Link libraries
target_link_libraries(OpenGLApp PRIVATE OpenGL::GL glfw Glad::Glad imgui::imgui)

