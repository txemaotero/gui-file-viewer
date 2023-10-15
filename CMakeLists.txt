cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(GuiFileViewer VERSION 1.0)

# Specify the required C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)

# Add the necessary subdirectories
add_subdirectory(src)
