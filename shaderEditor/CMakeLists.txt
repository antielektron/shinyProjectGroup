
cmake_minimum_required(VERSION 2.8.11)
project(ShaderEditor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

# Find regular OpenGL
find_package(OpenGL)


set(SOURCE_FILES main.cpp myglwidget.cpp mainwindow.cpp linenumberarea.cpp codeeditor.cpp)
add_executable(shaderEditor ${SOURCE_FILES})

# Use the Widgets module from Qt 5.
qt5_use_modules(shaderEditor Widgets)

# Include OpenGL
include_directories(${OPENGL_INCLUDE_DIRS})

# Link against OpenGL
target_link_libraries(shaderEditor ${OPENGL_LIBRARIES})
