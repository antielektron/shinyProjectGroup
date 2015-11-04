cmake_minimum_required(VERSION 2.8.11)
project(Test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

# Find the QtOpenGL library
# find_package(Qt5OpenGL)

# Find regular OpenGL
find_package(OpenGL)

set(SOURCE_FILES main.cpp utility.cpp OpenGLWidget.cpp Model.cpp)
add_executable(test ${SOURCE_FILES})

# Use the Widgets module from Qt 5.
qt5_use_modules(test Widgets)

# qt5_use_modules(test OpenGl)

# Include OpenGL
include_directories(${OPENGL_INCLUDE_DIRS})

# Link against OpenGL
target_link_libraries(test ${OPENGL_LIBRARIES})

# Use the OpenGL module from Qt 5.
# qt5_use_modules(test OpenGL)

# copy shaders to build directory:

add_custom_command(TARGET test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shader
        $<TARGET_FILE_DIR:test>/shader)

add_custom_command(TARGET test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/test.obj
        $<TARGET_FILE_DIR:test>/test.obj)

