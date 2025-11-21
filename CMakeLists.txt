# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name, version, and language
project(HandmadeRaylib VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Place the final executable in the project root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)
# Set default debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Use file(GLOB) to automatically find all source files
file(GLOB SOURCES "src/*.cpp")

# Add the executable target
add_executable(game ${SOURCES})

# Add our own 'include' directory to the include path
target_include_directories(game PRIVATE "${PROJECT_SOURCE_DIR}/include")

# --- Find and Link Raylib ---

# Find the raylib package to get its include directory
find_package(raylib CONFIG REQUIRED)
target_include_directories(game PRIVATE ${RAYLIB_INCLUDE_DIRS})

# Explicitly link raylib and its system dependencies, mimicking the Makefile
target_link_libraries(game PRIVATE
    raylib
    GL
    m
    pthread
    dl
    rt
    X11
)

# --- End of Raylib Linking ---
