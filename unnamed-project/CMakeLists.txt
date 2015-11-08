cmake_minimum_required(VERSION 2.8.11)
project(unnamed-project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

# Find regular OpenGL
find_package(OpenGL REQUIRED)

# Find all source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
add_executable(unnamed-project ${SOURCE_FILES})

# Use the Widgets module from Qt 5.
qt5_use_modules(unnamed-project Widgets)

# Include OpenGL
include_directories(${OPENGL_INCLUDE_DIRS})

# Link against OpenGL
target_link_libraries(unnamed-project ${OPENGL_LIBRARIES})
