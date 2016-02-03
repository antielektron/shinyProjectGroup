cmake_minimum_required(VERSION 2.8.11)
project(unnamed-project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)

# Find regular OpenGL
find_package(OpenGL REQUIRED)

# Find all source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
add_executable(unnamed-project ${SOURCE_FILES})

# Specify absolute paths when including files
include_directories(src/)

# Use the Widgets module from Qt 5.
qt5_use_modules(unnamed-project Widgets Xml)

# Include OpenGL
include_directories(${OPENGL_INCLUDE_DIRS})

# Link against OpenGL
target_link_libraries(unnamed-project ${OPENGL_LIBRARIES})


# Try to use Bullet3
find_package(Bullet)

# Try to find OpenAL
find_package(OpenAL)
find_package(ALUT)


if (BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
    target_link_libraries(unnamed-project ${BULLET_LIBRARIES})
    add_definitions("-DHAVE_BULLET=1")
endif()

if (OPENAL_FOUND)
    if (ALUT_FOUND)
        include_directories(${ALUT_INCLUDE_DIR})
        include_directories(${OPENAL_INCLUDE_DIR})
        target_link_libraries(unnamed-project ${ALUT_LIBRARY})
        target_link_libraries(unnamed-project ${OPENAL_LIBRARY})
        add_definitions("-DHAVE_OPENAL=1")
    endif()
endif()


# Windows: http://glew.sourceforge.net/index.html
find_package(GLEW REQUIRED)

if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(unnamed-project ${GLEW_LIBRARIES})
endif()
