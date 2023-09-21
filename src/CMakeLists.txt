# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Glad REQUIRED)
find_package(imgui REQUIRED)

file(GLOB_RECURSE IMGUI_BACKEND 
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendors/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendors/imgui/backends/imgui_impl_opengl3.cpp)
# Add your main executable
add_executable(OpenGLApp main.cpp ${IMGUI_BACKEND})

# Link libraries
target_link_libraries(OpenGLApp PRIVATE OpenGL::GL glfw glad::glad imgui::imgui)
target_include_directories(OpenGLApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../vendors/imgui/backends)

